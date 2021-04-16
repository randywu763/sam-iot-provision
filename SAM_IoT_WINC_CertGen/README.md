## This MPLAB X project uses the SAM IoT board and provision the WINC with your certificate to connect to your account.

Two methods
1) SandBox: Copy the compress DEVICE & SIGNER CERT to the WINC Flash
2) Personal Account: Create a CSR signed by ECC private KEY,  send it to Python Scripts who signs it with you AWS Signer Certs (loaded into your own account).  Device CERT and Signer CERT are than sent back to SAM IoT to be copied into the WINC.
