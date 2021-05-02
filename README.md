# SAM-IoT WG Development Board (EV75S95A) Provisioning Tools Package for Windows

## Overview

This Windows tools package is required for provisioning a Microchip SAM-IoT WG development board (Part No. EV75S95A) for use with Amazon Web Services (AWS) or Microsoft Azure Device Provisioning Service (DPS).  These Cloud services require IoT edge devices to authenticate themselves via TLS client authentication, which means X.509-based signer and client certificates need to be generated and stored in the WINC1510 Wi-Fi network controller on the SAM-IoT WG development board.

> `NOTE`: This tools package will only work for Microchip development board Part No. EV75S95A.  Other variants of the SAM-IoT board (designated by different part numbers) will require different provisioning tools corresponding to those other boards.

The Google Cloud Platform (GCP) does not use TLS client authentication (it uses JSON Web Token (JWT) in the MQTT password), so this provisioning tool is not required for use with GCP services.

The following two tasks are required to properly configure & provision the WINC1510 Wi-Fi network controller on the SAM-IoT WG development board:

1. WINC1510 Internal Firmware Upgrade

    The WINC1510 internal firmware version must be at least 19.6.5 to support TLS client authentication using X.509 certificates.  It is recommended to use the latest firmware version that has been published on the [WINC1500 product page](https://www.microchip.com/wwwproducts/en/ATWINC1500).

2. Device Certificate Generation and Storage into the WINC1510

    The X.509 device certificate (which is uniquely specific to each SAM-IoT development board) needs to be generated and then stored into the WINC1510.  The signer certificate needs to be stored into the WINC1510 as well.  Both signer and device certificates need to be used by the WINC1510 in order to complete the TLS handshake with the authentication server.
    
    This provisioning tool uses example "mock" root/signer keys/certificates during the generation phase of the device certificate.  If you want to use specific root and/or signer credentials, those files need to be copied over to replace the existing example "mock" credential files in the [./SAM_IoT_Certs_Generator/CertGen/cert/](./SAM_IoT_Certs_Generator/CertGen/cert/) folder.
    
    For example, to use a specific root certificate, the existing `root-ca.crt` file needs to be replaced with the desired root certificate.  Similarly, to use a specific signer certificate, the existing `signer-ca.crt` file needs to be replaced with the desired signer certificate.  You may also need to update/overwrite the existing files containing the keys (`*.key`) and the file containing the signer CSR (`signer-ca.csr`).

## Mock Root and Signer Certificates

1. Example "mock" `root` CA certificate provided as the default used by this tools package

    <img src=".//media/Root_Cert_Info.png" />

2. Example "mock" `signer` certificate provided as the default used by this tools package

    <img src=".//media/Signer_Cert_Info.png" />

## Procedure

1. Connect the SAM-IoT WG Development Board to the Host PC (running Windows) using a standard micro-USB cable

2. Open a `File Explorer` window and determine the drive `letter` that Windows assigned to `CURIOSITY` 

    <img src=".//media/image_00.png" />

3. Determine the COM port number for your `Curiosity Virtual COM port`.  You can find the COM port number by opening the Windows `Device Manager` &gt; expand `Ports(COM & LPT)` &gt; take note of the specific `(COM<X>)` associated with the Curiosity Virtual COM Port

    <img src=".//media/image_01.png" />

4. Edit the `iotprovision.bat` file using any text editor of your choice

5. Locate the following line in the batch file and note the 4 command line arguments 

    > CALL sam-iot-provision `[arg1] [arg2] [arg3] [arg4]`

6. Modify (if needed) the `first` argument for your desired Cloud service (e.g. aws or azure)

    > CALL sam-iot-provision `azure` 19.6.5 3 D

7. Modify (if needed) the `second` argument which sets the target WINC firmware version to be programmed

    > CALL sam-iot-provision azure `19.6.5` 3 D

8. Modify (if needed) the `third` argument to match your Curiosity Virtual COM port `number`

    > CALL sam-iot-provision azure 19.6.5 `3` D

9. Modify (if needed) the `fourth` argument for the drive `letter` corresponding to CURIOSITY

    > CALL sam-iot-provision azure 19.6.5 3 `D`

10. Save the changes to the batch file and exit the text editor

11. Open a command line window (e.g. Command Prompt or PowerShell) and execute the `iotprovision.bat` file

12. When the provisioning script has completed execution, the files for the security certificates (`*.crt`), keys (`*.key`), and signing requests (`*.csr`) can all be accessed from the [./SAM_IoT_Certs_Generator/CertGen/cert/](./SAM_IoT_Certs_Generator/CertGen/cert/) folder

    <img src=".//media/image_02.png" />