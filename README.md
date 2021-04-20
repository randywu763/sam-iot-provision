# SAM-IoT WG Development Board (EV75S95A) Provisioning Tools Package for Windows

## Overview

This Windows tools package is required for provisioning a Microchip SAM-IoT WG development board (Part No. EV75S95A) for use with Amazon Web Services (AWS) or Microsoft Azure Device Provisioning Service (DPS).  These Cloud services require IoT edge devices to authenticate themselves via TLS client authentication, which means X.509-based signer and client certificates need to be generated and stored in the WINC1510 Wi-Fi network controller on the SAM-IoT WG development board.

> `NOTE`: This tools package will only work for Microchip development board Part No. EV75S95A.  Other variants of the SAM-IoT board (designated by different part numbers) will require different provisioning tools corresponding to those other boards.

The Google Cloud Platform (GCP) does not use TLS client authentication (it uses JSON Web Token (JWT) in the MQTT password), so this provisioning tool is not required for use with GCP.

The following two tasks are required to properly configure & provision the WINC1510 Wi-Fi network controller on the SAM-IoT WG development board.  Please perform both tasks in the following order:

## 1. WINC1510 Internal Firmware Upgrade

The WINC1510 internal firmware version must be at least 19.6.5 to support TLS client authentication using X.509 certificates.  It is recommended to use the latest firmware version that has been published on the [WINC1500 product page](https://www.microchip.com/wwwproducts/en/ATWINC1500).  The complete procedure is outlined in the [`SAM_IoT_WINC_Upgrader`](SAM_IoT_WINC_Upgrader\README.md) folder in this repo.

## 2. Certificates Generation/Storage into the WINC1510

X.509 certificates need to be generated and then stored into the WINC1510.  The complete procedure is outlined in the [`SAM_IoT_Certs_Generator`](SAM_IoT_WINC_Certs_Generator\README.md) folder in this repo.
