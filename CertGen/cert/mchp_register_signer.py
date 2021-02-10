from datetime import datetime, timezone, timedelta
from argparse import ArgumentParser
import re
import boto3
import botocore
from cryptography import x509
from cryptography.hazmat.primitives import hashes, serialization
from mchp_kit_common import *


def mchp_register_signer(
        signer_ca_key_path=SIGNER_CA_KEY_FILENAME,
        signer_ca_cert_path=SIGNER_CA_CERT_FILENAME,
        signer_ca_ver_cert_path=SIGNER_CA_VER_CERT_FILENAME,
        mchp_profile='default'
):
    kit_info = read_kit_info()

    # Read the signer CA key file needed to sign the verification certificate
    print('\nReading signer CA key file, %s' % signer_ca_key_path)
    if not os.path.isfile(signer_ca_key_path):
        raise mchpZTKitError('Failed to find signer CA key file, ' + signer_ca_key_path + '. Have you run ca_create_signer_csr first?')
    with open(signer_ca_key_path, 'rb') as f:
        signer_ca_priv_key = serialization.load_pem_private_key(
            data=f.read(),
            password=None,
            backend=crypto_be)

    # Read the signer CA certificate to be registered with mchp IoT
    print('\nReading signer CA certificate file, %s' % signer_ca_cert_path)
    if not os.path.isfile(signer_ca_cert_path):
        raise mchpZTKitError('Failed to find signer CA certificate file, ' + signer_ca_cert_path + '. Have you run ca_create_signer first?')
    with open(signer_ca_cert_path, 'rb') as f:
        signer_ca_cert = x509.load_pem_x509_certificate(f.read(), crypto_be)

    # Create an mchp session with the credentials from the specified profile
    print('\nInitializing mchp IoT client')
    try:
        mchp_session = boto3.session.Session(profile_name=mchp_profile)
    except botocore.exceptions.ProfileNotFound as e:
        if mchp_profile == 'default':
            raise mchpZTKitError('mchp profile not found. Please make sure you have the mchp CLI installed and run "mchp configure" to setup profile.')
        else:
            raise mchpZTKitError('mchp profile not found. Please make sure you have the mchp CLI installed and run "mchp configure --profile %s" to setup profile.' % mchp_profile)
    # Create a client to the mchp IoT service
    mchp_iot = mchp_session.client('iot')
    print('    Profile:  %s' % mchp_session.profile_name)
    print('    Region:   %s' % mchp_session.region_name)
    print('    Endpoint: %s' % mchp_iot._endpoint)


    # Request a registration code required for registering a CA certificate (signer)
    print('\nGetting CA registration code from mchp IoT')
    reg_code = mchp_iot.get_registration_code()['registrationCode']
    print('    Code: %s' % reg_code)

    # Generate a verification certificate around the registration code (subject common name)
    print('\nGenerating signer CA mchp verification certificate')
    builder = x509.CertificateBuilder()
    builder = builder.serial_number(random_cert_sn(16))
    builder = builder.issuer_name(signer_ca_cert.subject)
    builder = builder.not_valid_before(datetime.utcnow().replace(tzinfo=timezone.utc))
    builder = builder.not_valid_after(builder._not_valid_before + timedelta(days=1))
    builder = builder.subject_name(x509.Name([x509.NameAttribute(x509.oid.NameOID.COMMON_NAME, reg_code)]))
    builder = builder.public_key(signer_ca_cert.public_key())
    signer_ca_ver_cert = builder.sign(
        private_key=signer_ca_priv_key,
        algorithm=hashes.SHA256(),
        backend=crypto_be)

    # Write signer CA certificate to file for reference
    with open(signer_ca_ver_cert_path, 'wb') as f:
        print('    Saved to ' + f.name)
        f.write(signer_ca_ver_cert.public_bytes(encoding=serialization.Encoding.PEM))

    print('\nRegistering signer CA with mchp IoT')
    try:
        # TODO: Provide options when this fails (already exists, too many CAs with same name, etc...)
        response = mchp_iot.register_ca_certificate(
            caCertificate=signer_ca_cert.public_bytes(encoding=serialization.Encoding.PEM).decode('ascii'),
            verificationCertificate=signer_ca_ver_cert.public_bytes(encoding=serialization.Encoding.PEM).decode('ascii'),
            setAsActive=True,
            allowAutoRegistration=True)
        ca_id = response['certificateId']
    except botocore.exceptions.ClientError as e:
        if e.response['Error']['Code'] == 'ResourceAlreadyExistsException':
            print('    This CA certificate already exists in mchp IoT')
            ca_id = re.search('ID:([0-9a-zA-Z]+)', e.response['Error']['Message']).group(1)
        else:
            raise
    print('    ID: ' + ca_id)

    print('\nGetting mchp IoT device endpoint')
    response = mchp_iot.describe_ca_certificate(certificateId=ca_id)
    # Replace the response datetime objects with an ISO8601 string so the dict is json serializable
    all_datetime_to_iso8601(response)

    kit_info['certificateDescription'] = response['certificateDescription']
    response = mchp_iot.describe_endpoint()
    kit_info['endpointAddress'] = response['endpointAddress']
    print('    Hostname: ' + kit_info['endpointAddress'])

    save_kit_info(kit_info)

    print('\nDone')


if __name__ == '__main__':
    # Create argument parser to document script use
    parser = ArgumentParser(description='Register a signer CA with mchp IoT')
    parser.add_argument(
        '--profile',
        dest='profile',
        nargs='?',
        default='default',
        metavar='name',
        help='mchp profile name (uses default if omitted)')
    args = parser.parse_args()

    try:
        mchp_register_signer(mchp_profile=args.profile)
    except mchpZTKitError as e:
        # Print kit errors without a stack trace
        print(e)
