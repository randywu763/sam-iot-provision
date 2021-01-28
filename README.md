# SAM-IoT Provisioning Tool

This tools package is required for provisioning a Microchip SAM-IoT WG development board for use with Amazon Web Services (AWS) or Microsoft Azure Device Provisioning Service (DPS).  These Cloud services require IoT edge devices to authenticate themselves via TLS client authentication, which means X.509-based signer and client certificates need to be generated and stored in the ATWINC1510 Wi-Fi network controller on the SAM-IoT WG development board.

The Google Cloud Platform (GCP) does not use TLS client authentication (it uses JSON Web Token (JWT) in the MQTT password), so this provisioning tool is not required for use with GCP.

The following two things are required to properly provision the ATWINC1510 Wi-Fi network controller on the SAM-IoT board:

## (1) ATWINC1510 Firmware Update

The internal firmware for the ATWINC1510 must be version 19.6.5 or greater.  If you know for sure that your ATWINC1510 module meets this requirement, this step can be skipped.

In the "SAM-IoT WINC15x0 FW Upgrade" folder, go into the "FAST Updater" sub-folder and follow the directions in the README.TXT file.  If for any reason the "Fast Updater" fails to properly update the ATWINC15x0 firmware, try repeating the process with the set of files in the "SLOW Updater" sub-folder.

## (2) Certificates Generation & Storage into the ATWINC1510

In the main SAM-IoT Provisioning Tool folder, open the "Certificates Provisioning for SAM-IoT" document and follow the instructions.
