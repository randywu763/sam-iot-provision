# Generating Certificates and Provisioning the Microchip SAM-IoT WG Development Board

## Overview

In this section, you will generate root/signer/device keys & certificates which will be used to provision the WINC1510 Wi-Fi network controller on the SAM-IoT WG Development Board.  This process programs the flash memory of the MCU (SAMD21) on the SAM-IoT board so any application firmware that was previously programmed will be overwritten with this provisioning firmware.

## Procedure

1. Connect the SAM-IoT WG Development Board to the Host PC using a standard micro-USB cable

2. Open a `File Explorer` window and determine the drive `letter` assigned to `CURIOSITY` 

    <img src=".//media/image_01.png" />

3. Determine the COM port number for your `Curiosity Virtual COM port`.  You can find the COM port number by opening the Windows `Device Manager` &gt; expand `Ports(COM & LPT)` &gt; take note of the specific `(COM<X>)` associated with the Curiosity Virtual COM Port

    <img src=".//media/image_02.png" />

4. Download the latest [Python](https://www.python.org/downloads/) release. Run the installation setup program and then do the following (do not click `Install Now`):

    - Check `Add Python <X.Y> to PATH` and click `Customize installation`
        <img src=".//media/image_03.png" />
    - Confirm `pip (download and install other Python packages)` is checked and click `Next` to start the installation process
        <img src=".//media/image_04.png" />

5. Confirm that the Windows `Path` environment variable contains a pointer to the Python installation directory.  For example, if Python was installed into `C:\Python`, the `Path` variable should include it

    <img src=".//media/image_05.png" />

6. Using a `File Explorer` window, navigate to the `CertGen` subfolder

7. Open the `sam_iot_cert_gen.bat` file using any text editor

8. Locate the following line in the batch file and modify the drive `letter` corresponding to `CURIOSITY`

    > copy SAM_IoT_CertGen.hex `D`:\

9. Locate the following line in the batch file and modify the COM port `number` corresponding to `Curiosity Virtual COM port`

    > python provision_samiot.py com`29`

10. Save changes to the batch file and exit the text editor

11. Open a `PowerShell` window > click on `Start` > type `PowerShell` in the Search field > `Open`.  Navigate to the `CertGen` subfolder and then execute the following command:

    ```
    .\sam_iot_cert_gen
    ```
    For example:

    <img src=".//media/image_06.png" />

12. When the provisioning process has successfully completed, the final output message displayed will be `"Done provisioning thing"`

    <img src=".//media/image_07.png" />
  
    If any errors were encountered, confirm that the `CURIOSITY` drive `letter` and COM port `number` of the `Curiosity Virtual COM Port` are all correctly set in the `sam_iot_cert_gen.bat` file

## Examples of Certificates

The below screenshot shows examples of certificates generated for a SAM-IoT board that has an ATECC608A Secure Element which was pre-programmed with a unique serial number of 01239E946F011C66FE

An online tool such as a [Certificate Decoder](https://www.sslshopper.com/certificate-decoder.html) can be used to display the pertinent information for a particular certificate by simply pasting the text of the certificate into a browser

<img src=".//media/image_08.png" />

## Summary

The following files are generated during this process:

- Root CA Certificate (`root-ca.crt`)
- Root CA Key (`root-ca.key`)
- Signer CA Certificate (`signer-ca.crt`)
- Signer CA Certificate Signing Request (CSR) (`signer-ca.csr`)
- Signer CA Key (`signer-ca.key`)
- Device Certificate (`device.crt`)

The following files are stored in the WINC1510:

- Signer CA Certificate (`signer-ca.crt`)
- Device Certificate (`device.crt`)