# SAM-IoT WG Development Board Provisioning Tools Package (for Microchip Part No. EV75S95A only) 

## Overview

This tools package is required for provisioning a Microchip SAM-IoT WG development board (Part No. EV75S95A) for use with Amazon Web Services (AWS) or Microsoft Azure Device Provisioning Service (DPS).  These Cloud services require IoT edge devices to authenticate themselves via TLS client authentication, which means X.509-based signer and client certificates need to be generated and stored in the WINC1510 Wi-Fi network controller on the SAM-IoT WG development board.

> NOTE: This tools package will only work for Microchip development board Part No. EV75S95A

The Google Cloud Platform (GCP) does not use TLS client authentication (it uses JSON Web Token (JWT) in the MQTT password), so this provisioning tool is not required for use with GCP.

The following two tasks are required to properly provision the WINC1510 Wi-Fi network controller on the SAM-IoT WG development board:

## 1. WINC1510 Internal Firmware Upgrade

The WINC1510 internal firmware version must be at least 19.7.3 to support TLS client authentication using X.509 certificates.  Navigate to the `SAM_IoT_WINC_Upgrader` folder and follow the instructions.

## 2. Certificates Generation/Storage into the WINC1510

X.509 certificates need to be generated and then stored into the WINC1510.  Navigate to the  `SAM_IoT_Certs_Generator` folder and follow the instructions.
