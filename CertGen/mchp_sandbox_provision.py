"""
This script implements the "sandbox" mchp provisioning method, using device certificate from ECC.
It is intended to be invoked from iotprovison, but can also be run stand-alone.
"""
import sys
from time import sleep
from glob import glob
#from os.path import isfile, isdir, basename
import os.path
import hashlib
from serial import Serial
from OpenSSL import crypto
from cryptography.hazmat.primitives import serialization
from serial.serialutil import SerialException


if __package__:
    from ..target.fw_protocol import fw_request
    from .. import mchp_kit_common as kit
else:
    sys.path.append("..")
    from target.fw_protocol import fw_request
    import mchp_kit_common as kit


# def get_signer_cert(device_cert):
#     """
#     From the set of production signer certificates, return the one
#     with subject matching the device certificate issuer. All candidate
#     certificate files are assumed to be in PEM format and reside in a
#     single folder.
#     CURRENTLY NOT USED BECAUSE WE CAN GET THE SIGNER CERT FROM FW.

#     """
#     if not kit.SIGNER_CERTS_FOLDER:
#         return (None, None)

#     if os.path.isdir(kit.SIGNER_CERTS_FOLDER):
#         files = glob(kit.SIGNER_CERTS_FOLDER + "/*.*")
#     else:
#         print("{} is not a directory".format(kit.SIGNER_CERTS_FOLDER), file=sys.stderr)
#         return (None, None)

#     for file in files:
#         if os.path.isfile(file):
#             try:
#                 with open(file, "rb") as f:
#                     signer_ca_cert = crypto.load_certificate(crypto.FILETYPE_PEM, f.read())
#                     if signer_ca_cert.get_subject() == device_cert.get_issuer():
#                         return (signer_ca_cert, file)
#             except Exception as e:
#                 print("Error loading signer cert {}: {}".format(file, e), file=sys.stderr)
#                 continue

#     print("No matching signer cert found for issuer {}".format(device_cert.issuer), file=sys.stderr)
#     return (None, None)


def provision(port, filename="device_mchp_sandbox.pem", baud=115200, dumphex=False):
    """
    Read out device certificate from kit, save it to file, extract "thing name"
    (AKA subject key identifier), save these items to WINC flash for easy
    access by application.
    """
    try:
        com = Serial(port, baud, inter_byte_timeout=2, timeout=5)
    except SerialException as e:
        print("Could not open {}: {}".format(port, e), file=sys.stderr)
        return None

    thing_name = None

    if fw_request(com, "wincErs", msg="\nErase WINC TLS certificate sector") is None:
        return None
    print('    Done.')


    signer_cert = fw_request(com, "<signCert", msg="\nRequest signer certificate", dump=dumphex)
    if signer_cert:
        x509cert = crypto.load_certificate(crypto.FILETYPE_ASN1, bytes.fromhex(signer_cert))

        # Save signer cert to file, we don't really need to do this, consider
        # removing in final version or make optional..
        id = ((x509cert.get_subject().get_components()[-1][-1]).split()[-1]).decode()
        signer_filename = os.path.join(kit.CERTS_FOLDER, "signer_{}.pem".format(id))
        print("\nSaving signer certifcate to {}".format(signer_filename))
        with open(signer_filename, "wb") as f:
            f.write(crypto.dump_certificate(crypto.FILETYPE_PEM, x509cert))

    ## The alternative way to get the cigner cert: Pick it from a local
    ## collection of the 128 possible signer certs.
    # signer_cert, filename = get_signer_cert(x509cert)
    # if signer_cert:
    #     if fw_request(com, ">caCert",
    #                   [crypto.dump_certificate(crypto.FILETYPE_ASN1, signer_cert).hex()],
    #                   msg="\nSend Signer Certificate {}".format(basename(filename)),
    #                   dump=dumphex) is not None:
    #         sleep(1)
    #         print('    Done.\n')
    #     else:
    #         return None
    # else:
    #     return None

    device_cert = fw_request(com, "<devCert", msg="\nRequest device certificate", dump=dumphex)
    if device_cert:
        # Convert certicate to PEM and save to file.
        x509cert = crypto.load_certificate(crypto.FILETYPE_ASN1, bytes.fromhex(device_cert))

        with open(filename, "wb") as f:
            f.write(crypto.dump_certificate(crypto.FILETYPE_PEM, x509cert))
            print("\nSaved device certificate in '{}'".format(filename))

        for i in range(0, x509cert.get_extension_count()):
            if x509cert.get_extension(i).get_short_name() == b"subjectKeyIdentifier":
                # cert.get_data() returns additional bytes so use __str__ representation instead
                thing_name = str(x509cert.get_extension(i)).replace(":", "").lower()
                break
        else:
            pubkey = crypto.dump_publickey(crypto.FILETYPE_ASN1, x509cert.get_pubkey())
            thing_name = hashlib.sha1(pubkey[-65:]).hexdigest()
    else:
        com.close()
        return thing_name

    # Send signer cert to kit
    if fw_request(com, ">caCert", [signer_cert],
                  msg="\nSend Signer Certificate {}".format(os.path.basename(signer_filename)),
                                                            dump=dumphex) is None:
        return None

    # Send device cert up again for firmware to correctly save it to WINC
    if fw_request(com, ">devCert", [device_cert], msg="Send Device Certificate",
                  dump=dumphex) is None:
        return None
    print('    Done.\n')
    sleep(1)

    if fw_request(com, "xferCerts", ["4"], msg="Transfer certificates to WINC",
                  dump=dumphex) is None:
        return None
    sleep(1)
    print('    Done.\n')

    if fw_request(com, ">thingName", [thing_name], msg="Save thing name in WINC",
                  dump=dumphex) is None:
        return None
    sleep(1)
    print('    Done.\n')

    if fw_request(com, "lock", msg="\nLocking ECC slots 10-11") is None:
        return None
    print('    Done.')

    print('Done provisioning thing {}'.format(thing_name))
    com.close()
    return thing_name


if __name__ == '__main__':
    ############################################################################
    if len(sys.argv) <= 1:
        print("Usage: {} <port-name>".format(sys.argv[0]),
              file=sys.stderr)
        sys.exit(1)
    else:
        PORT = str(sys.argv[1])

    try:
        #kit.SIGNER_CERTS_FOLDER = sys.argv[2]
        sys.exit(provision(PORT, dumphex=True) is None)
    except kit.mchpZTKitError as e:
        print(e)
        sys.exit(1)
