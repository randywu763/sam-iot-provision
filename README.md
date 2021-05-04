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
    
## Chain of Trust

By default, this provisioning tool is configured to create a mock chain of trust before generating the device certificate (the [ChainOfTrust](./SAM_IoT_Certs_Generator/ChainOfTrust/) folder is initially empty so the tool will recognize this). To use specific root and/or signer credentials, those files need to be copied into the [ChainOfTrust](./SAM_IoT_Certs_Generator/ChainOfTrust/) folder (the **exact** file names as shown below need to be used in order for the tool to recognize the purpose/contents of each file).

<img src=".//media/image_00.png" />

In summary, at the time the provisioning script is launched:

- If the [ChainOfTrust](./SAM_IoT_Certs_Generator/ChainOfTrust/) folder is **empty** (the default state before the provisioning script is executed for the first time), the tool will generate a mock chain of trust (root key/certificate, signer key/certificate/CSR)
- If the [ChainOfTrust](./SAM_IoT_Certs_Generator/ChainOfTrust/) folder is **populated**, the tool uses that set of files for the Chain of Trust
- Whatever set of Chain of Trust files is used during provisioning will be automatically saved to the [ChainOfTrust](./SAM_IoT_Certs_Generator/ChainOfTrust/) folder so that the same credentials will be used the next time the tool is invoked (i.e. whenever the provisioning script is launched again)

## Examples of Mock Certificates

The following shows mock certificates that the provisioning tool can generate for evaluation purposes:

1. **Root** Certificate issued by Root CA from Microchip Technology Inc

    <img src=".//media/Root_Cert_Info.png" />

2. **Signer** Certificate issued by Root CA from Microchip Technology Inc

    <img src=".//media/Signer_Cert_Info.png" />

3. **Device** Certificate issued by Microsoft Signer from Microsoft Inc

    <img src=".//media/Device_Cert_Info.png" />

## Procedure

1. Connect the SAM-IoT WG Development Board to the Host PC (running Windows) using a standard micro-USB cable

2. Open a **File Explorer** window and determine the drive `letter` that Windows assigned to `CURIOSITY` 

    <img src=".//media/image_01.png" />

3. Determine the COM port number for your `Curiosity Virtual COM port`.  You can find the COM port number by opening the Windows **Device Manager** &gt; expand `Ports(COM & LPT)` &gt; take note of the specific `(COM<X>)` associated with the Curiosity Virtual COM Port

    <img src=".//media/image_02.png" />

4. Edit the `iotprovision.bat` file using any text editor of your choice

5. Locate the following line in the batch file and note that 4 command line arguments need to be set 

    > CALL sam-iot-provision.bat `[arg1] [arg2] [arg3] [arg4]`

    - Set the **first** argument for your desired Cloud service (e.g. aws or azure)

        > CALL sam-iot-provision.bat `azure` 19.7.3 3 D

    - Set the **second** argument which sets the target WINC firmware version to be programmed

        > CALL sam-iot-provision.bat azure `19.7.3` 3 D

    - Set the **third** argument to match your Curiosity Virtual COM port `number`

        > CALL sam-iot-provision.bat azure 19.7.3 `3` D

    - Set the **fourth** argument for the drive `letter` corresponding to CURIOSITY

        > CALL sam-iot-provision.bat azure 19.7.3 3 `D`

6. Save the changes to the batch file and exit the text editor

7. Open a command line window (e.g. Command Prompt, PowerShell) and execute `.\iotprovision.bat`

8. When the provisioning script has completed execution, the files for the security certificates (`*.crt`/`*.pem`), keys (`*.key`), and signing requests (`*.csr`) can all be accessed from the [ChainOfTrust](./SAM_IoT_Certs_Generator/ChainOfTrust/) folder

    <img src=".//media/image_03.png" />
