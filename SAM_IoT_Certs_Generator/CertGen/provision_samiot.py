"""
This script implements the "manual" mchp provisioning method, using
self-generated root and signer certificates.
It is intended to be invokerd from iotprovison, but can also be run stabd-alone.
"""

import os
import sys
import datetime
import binascii
from time import sleep
import pytz
from cryptography import x509
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives import serialization
from serial import Serial
from serial.serialutil import SerialException

import mchp_kit_common as kit
from target.fw_protocol import fw_request

DEFAULT_BAUD_RATE = 115200




def provision(port, filename=kit.DEVICE_CERT_FILENAME, baud=DEFAULT_BAUD_RATE, dumphex=False, signer=None):
    """
    Do the actual provisioning. This will generate a device certificate,
    and save it along with the CA signer cerificate in WINC flash
    Returns the "Thing name" (SKI) if successful.
    Generated certificates and thing name are saved to files as well.
    """
    try:
        com = Serial(port, baud, inter_byte_timeout=2, timeout=5)
    except SerialException as e:
        print("Could not open {}: {}".format(port, e), file=sys.stderr)
        return None

    print('\nLoading root CA certificate')
    if not os.path.isfile(kit.ROOT_CA_CERT_FILENAME):
        raise kit.mchpZTKitError('Failed to find root CA certificate file, '
                                + kit.ROOT_CA_CERT_FILENAME
                                + '. Have you run ca_create_root first?')
    with open(kit.ROOT_CA_CERT_FILENAME, 'rb') as f:
        print('    Loading from ' + f.name)
        root_ca_cert = x509.load_pem_x509_certificate(f.read(), kit.crypto_be)

    print('\nLoading signer CA key')
    if not os.path.isfile(kit.SIGNER_CA_KEY_FILENAME):
        raise kit.mchpZTKitError('Failed to find signer CA key file, '
                                + kit.SIGNER_CA_KEY_FILENAME
                                + '. Have you run ca_create_signer_csr first?')
    with open(kit.SIGNER_CA_KEY_FILENAME, 'rb') as f:
        print('    Loading from ' + f.name)
        signer_ca_priv_key = serialization.load_pem_private_key(
            data=f.read(),
            password=None,
            backend=kit.crypto_be)

    print('\nLoading signer CA certificate')
    if not os.path.isfile(kit.SIGNER_CA_CERT_FILENAME):
        raise kit.mchpZTKitError('Failed to find signer CA certificate file, '
                                + kit.SIGNER_CA_CERT_FILENAME
                                + '. Have you run ca_create_signer first?')
    with open(kit.SIGNER_CA_CERT_FILENAME, 'rb') as f:
        print('    Loading from ' + f.name)
        signer_ca_cert = x509.load_pem_x509_certificate(f.read(), kit.crypto_be)

    if fw_request(com, "wincErs", msg="\nErase WINC TLS certificate sector") is None:
        return None
    print('    Done.')

    csr = fw_request(com, "<genCsr", msg="\nRequest device CSR", dump=dumphex)
    if not csr:
        return None
    ser_number = fw_request(com, "<serNum", msg="\nRequest device Serial Number", dump=dumphex)
    if not ser_number:
        return None
    ser_number = "sn" + ser_number
    print('\nSerial Number is '+ ser_number)

    dev_public_key = fw_request(com, "<devKey", msg="\nRequest device Public Key", dump=dumphex)
    if not dev_public_key:
        return None
    print('\nPublic Key is '+ dev_public_key)



    ############################################################################
    device_csr = x509.load_der_x509_csr(binascii.a2b_hex(csr), kit.crypto_be)
    #if not device_csr.is_signature_valid:
    #    raise kit.mchpZTKitError('Device CSR has invalid signature.')
    with open(kit.DEVICE_CSR_FILENAME, 'wb') as f:
        print('    Saving to ' + f.name)
        f.write(device_csr.public_bytes(encoding=serialization.Encoding.PEM))
        print('    Done.')
    #print('\nDevice public key is : \n'+(device_csr.public_key()).decode('ascii'))

    print('\nGenerating device certificate from CSR')
    # Build certificate
    builder = x509.CertificateBuilder()
    builder = builder.issuer_name(signer_ca_cert.subject)
    # Device cert must have minutes and seconds set to 0
    builder = builder.not_valid_before(datetime.datetime.now(tz=pytz.utc).replace(minute=0, second=0))
    # Should be year 9999, but this doesn't work on windows
    builder = builder.not_valid_after(datetime.datetime(3000, 12, 31, 23, 59, 59))
    builder = builder.subject_name(device_csr.subject)
    #builder = builder.subject_name(x509.Name([
    #    x509.NameAttribute(x509.oid.NameOID.ORGANIZATION_NAME, u'Microchip Technology Inc'),
    #    x509.NameAttribute(x509.oid.NameOID.COMMON_NAME, ser_number)]))
    builder = builder.public_key(device_csr.public_key())
    # Device certificate is generated from certificate dates and public key
    builder = builder.serial_number(kit.device_cert_sn(16, builder))
    # Add in extensions specified by CSR
    for extension in device_csr.extensions:
        builder = builder.add_extension(extension.value, extension.critical)
    # Subject Key ID is used as the thing name and MQTT client ID and is required for this demo
    builder = builder.add_extension(
        x509.SubjectKeyIdentifier.from_public_key(builder._public_key),
        critical=False)
    issuer_ski = signer_ca_cert.extensions.get_extension_for_class(x509.SubjectKeyIdentifier)
    builder = builder.add_extension(
        x509.AuthorityKeyIdentifier.from_issuer_subject_key_identifier(issuer_ski.value),
        critical=False)

    # Sign certificate
    device_cert = builder.sign(
        private_key=signer_ca_priv_key,
        algorithm=hashes.SHA256(),
        backend=kit.crypto_be)

    for extension in device_cert.extensions:
        if extension.oid._name != 'subjectKeyIdentifier':
            continue # Not the extension we're looking for, skip
        thing_name = binascii.b2a_hex(extension.value.digest).decode('ascii')

    # Save certificate for reference
    with open(filename, 'wb') as f:
        print('    Saving to ' + f.name)
        f.write(device_cert.public_bytes(encoding=serialization.Encoding.PEM))
        print('    Done.\n')

    print('\nProvisioning device with mchp IoT credentials\n')
    pub_nums = root_ca_cert.public_key().public_numbers()
    pubkey = pub_nums.x.to_bytes(32, byteorder='big', signed=False)
    pubkey += pub_nums.y.to_bytes(32, byteorder='big', signed=False)

    sleep(1)

    if fw_request(com, ">caCert",
                  [signer_ca_cert.public_bytes(encoding=serialization.Encoding.DER).hex()],
                  msg="Send Signer Certificate", dump=dumphex) is None:
        return None
    print('    Done.\n')
    sleep(1)

    if fw_request(com, ">devCert",
                  [device_cert.public_bytes(encoding=serialization.Encoding.DER).hex()],
                  msg="Send Device Certificate", dump=dumphex) is None:
        return None
    print('    Done.\n')
    sleep(1)


    if fw_request(com, "xferCerts", ["2"], msg="Transfer certificates to WINC",
                  dump=dumphex) is None:
        return None
    sleep(1)
    print('    Done.\n')

    if fw_request(com, ">thingName", [thing_name], msg="Save thing name in WINC",
                  dump=dumphex) is None:
        return None
    print('    Done.\n')
    sleep(1)

    ############################################################################
    kitInfoJSON = kit.read_kit_info()
    print('EndPoint is ',kitInfoJSON['endpointAddress'])
    endPoint = kitInfoJSON['endpointAddress']
    if fw_request(com, ">endPoint", [endPoint], msg="Save EndPoint in WINC",
                  dump=dumphex) is None:
        return None
    print('    Done.\n')
    sleep(1)
    ############################################################################

    ############################################################################

    print('Done provisioning thing {}'.format(thing_name))
    com.close()
    return thing_name


if __name__ == '__main__':
    ############################################################################
    if len(sys.argv) <= 1:
        print("Usage: {} <port-name>".format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)
    else:
        PORT = str(sys.argv[1])

    try:
        sys.exit(provision(PORT, dumphex=True) is None)
    except kit.mchpZTKitError as e:
        print(e)
        sys.exit(1)
