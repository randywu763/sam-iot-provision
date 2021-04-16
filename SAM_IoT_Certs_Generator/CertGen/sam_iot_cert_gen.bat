REM *** Updating Python Modules ***
python -m pip install --upgrade pip
pip install -r py_modules.txt
cd cert
REM *** Generating ROOT Certificate & Key ***
python ca_create_root.py
REM *** Generating SIGNER Key & Certificate Signing Request (CSR) ***
python ca_create_signer_csr.py
REM *** Generating SIGNER Certificate ***
python ca_create_signer.py
REM *** Provisioning SAM-IoT Development Board ***
cd ..
python provision_samiot.py com3
REM *** If an error occurs, make sure the COM port setting is correct ***
