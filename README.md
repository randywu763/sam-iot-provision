# SAM-IoT Provisioning Tool

This tools package is required for provisioning a Microchip SAM-IoT WG development board for use with Amazon Web Services (AWS) or Microsoft Azure Device Provisioning Service (DPS).  These Cloud services require IoT edge devices to authenticate themselves via TLS client authentication, which means X.509-based signer and client certificates need to be generated and stored in the ATWINC1510 Wi-Fi network controller on the SAM-IoT WG development board.

The Google Cloud Platform (GCP) does not use TLS client authentication (it uses JSON Web Token (JWT) in the MQTT password), so this provisioning tool is not required for use with GCP.

The following two tasks are required to properly provision the ATWINC1510 Wi-Fi network controller on the SAM-IoT board:

## [1] ATWINC1510 Firmware Update

The internal firmware for the ATWINC1510 must be version 19.6.5 or greater to support TLS client authentication using X.509 certificates.  If you know for sure that your ATWINC1510 module meets this requirement, this step can be skipped.

In the "SAM-IoT WINC15x0 FW Upgrade" folder, you can select one of 3 choices:

(a) "v19_6_5 (fast)" - Updates the WINC1510 firmware using the version 19.6.5 binary image (the automated script takes approximately 5 minutes to complete)

(b) "v19_6_5 (slow)" - Updates the WINC1510 firmware using the version 19.6.5 binary image but uses a longer, more reliable process than the "fast" version (the automated script takes approximately 15 minutes to complete)

(c) "v19_7_3" - Updates the WINC1510 firmware using the version 19.7.3 binary image based on a process similar to the "slow" version (the automated script takes approximately 15 minutes to complete)

Each of these folders contains a README.TXT file which outlines the exact procedure to follow.

## [2] Certificates Generation & Storage into the ATWINC1510

In the main SAM-IoT Provisioning Tool folder, open the "Certificates Provisioning for SAM-IoT" document and follow the instructions.
