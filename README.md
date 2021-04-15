# SAM-IoT Development Board Provisioning Tools Package

This tools package is required for provisioning a Microchip SAM-IoT development board for use with Amazon Web Services (AWS) or Microsoft Azure Device Provisioning Service (DPS).  These Cloud services require IoT edge devices to authenticate themselves via TLS client authentication, which means X.509-based signer and client certificates need to be generated and stored in the WINC1510 Wi-Fi network controller on the SAM-IoT development board.

The Google Cloud Platform (GCP) does not use TLS client authentication (it uses JSON Web Token (JWT) in the MQTT password), so this provisioning tool is not required for use with GCP.

The following two tasks are required to properly provision the WINC1510 Wi-Fi network controller on the SAM-IoT board:

## WINC1510 Internal Firmware Update

> The WINC1510 firmware must be minimum version 19.6.5 to support TLS client authentication using X.509 certificates.  Navigate to the **SAM_IoT_WINC_Updater** sub-folder and follow the instructions.

## Certificates Generation/Storage into the WINC1510

> Follow the instructions in the **Generating Certificates for SAM-IoT** document.
