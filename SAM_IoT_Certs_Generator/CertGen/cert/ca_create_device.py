from datetime import datetime, timezone
from cryptography import x509
from cryptography.hazmat.primitives import hashes, serialization
from mchp_kit_common import *
from ca_create_device_csr import add_signer_extensions


def ca_create_device(
        device_csr_path=DEVICE_CSR_FILENAME,
        device_cert_path=DEVICE_CERT_FILENAME,
        signer_ca_key_path=SIGNER_CA_KEY_FILENAME,
        signer_ca_cert_path=SIGNER_CA_CERT_FILENAME
):
    print('\nLoading device CA CSR')
    if not os.path.isfile(device_csr_path):
        raise mchpZTKitError('Failed to find signer CA CSR file, ' + device_csr_path + '. Have you run ca_create_device_csr first?')
    with open(device_csr_path, 'rb') as f:
        print('    Loading from ' + f.name)
        device_csr = x509.load_pem_x509_csr(f.read(), crypto_be)
    if not device_csr.is_signature_valid:
        raise RuntimeError('Signer CA CSR has invalid signature.')

    print('\nLoading signer CA key')
    if not os.path.isfile(signer_ca_key_path):
        raise mchpZTKitError('Failed to find signer CA key file, ' + signer_ca_key_path + '. Have you run ca_create_root first?')
    with open(signer_ca_key_path, 'rb') as f:
        print('    Loading from ' + f.name)
        root_ca_priv_key = serialization.load_pem_private_key(
            data=f.read(),
            password=None,
            backend=crypto_be)

    print('\nLoading signer CA certificate')
    if not os.path.isfile(signer_ca_cert_path):
        raise mchpZTKitError('Failed to find signer CA certificate file, ' + signer_ca_cert_path + '. Have you run ca_create_root first?')
    with open(signer_ca_cert_path, 'rb') as f:
        print('    Loading from ' + f.name)
        root_ca_cert = x509.load_pem_x509_certificate(f.read(), crypto_be)

    # Create device CA certificate
    print('\nGenerating device certificate from CSR')
    # Please note that the structure of the signer certificate is part of certificate definition in the SAMG55 firmware
    # (g_cert_elements_1_signer). If any part of it is changed, it will also need to be changed in the firmware.
    # The cert2certdef.py utility script can help with regenerating the cert_def_1_signer.c file after making changes.
    builder = x509.CertificateBuilder()
    builder = builder.serial_number(random_cert_sn(16))
    builder = builder.issuer_name(root_ca_cert.subject)
    #builder = builder.AuthorityKeyIdentifier
    not_before = datetime.utcnow().replace(tzinfo=timezone.utc)
    if not_before.month == 2 and not_before.day == 29:
        # Compressed certs don't handle leap years, fudge the date a little
        not_before.day = 28
    builder = builder.not_valid_before(not_before)
    builder = builder.not_valid_after(not_before.replace(year=not_before.year + 10))
    builder = builder.subject_name(device_csr.subject)
    builder = builder.public_key(device_csr.public_key())
    builder = add_signer_extensions(
        builder=builder,
        authority_cert=root_ca_cert)
    # Sign signer certificate with root
    signer_ca_cert = builder.sign(
        private_key=root_ca_priv_key,
        algorithm=hashes.SHA256(),
        backend=crypto_be)

    # Write signer CA certificate to file
    with open(device_cert_path, 'wb') as f:
        print('    Saving to ' + f.name)
        f.write(signer_ca_cert.public_bytes(encoding=serialization.Encoding.PEM))

    print('\nDone')


if __name__ == '__main__':
    try:
        ca_create_device()
    except mchpZTKitError as e:
        # Print kit errors without a stack trace
        print(e)
