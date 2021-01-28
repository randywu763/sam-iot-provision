"""
(Virtual) serial port related functionality
"""

import sys
import os
from time import sleep
from serial import Serial
from serial.serialutil import SerialException
import serial.tools.list_ports
if sys.platform.startswith("linux"):
    import grp


def portwait(port, timeout=30, tick=1):
    """
    Wait for virtual serial port to become ready, because access may
    fail initially due to USB enumeration taking a while (happens
    after kit is plugged in, or debugger FW is upgraded)
    Error codes explained (on Linux):
    EBUSY  may last for a few seconds after connection/FWupgrade.
    ENOENT may occur briefly immediately after connection
    EPERM  may occur on Linux for a similar period as EBUSY
    None   on Windows.

    """
    err = 0
    to = timeout
    while to > 0:
        try:
            with Serial(port, 115200) as com:
                if to != timeout:
                    print("\n")
                return 0
        except SerialException as e:
            # Only print countdown after having failed once, hence the dodgy logic.
            # Normally this should not happen.
            if to == timeout:
                print("Waiting for virtual serial port {}:...".format(port),
                      end="", flush=True)
            print("\b\b\b{:3d}".format(to), end="", flush=True)
            err = e.errno or 127
            to -= 1
            if to > 0:
                sleep(tick)

    print("\nTimed out waiting for {} (errno={}: {})".format(port, err, os.strerror(err)))
    return err


def check_access(port):
    """
    Check if user has access to kit's virtual serial port.
    In many Linux ditstros access requires membership in group 'dialout',
    which is not default.
    """
    portwait(port)
    if port and not sys.platform.startswith("win32"):
        try:
            with open(port, "r") as p: pass # "w" would try to create file if non-existent.
        except IOError as e:
            print(e)
            if isinstance(e, PermissionError) and sys.platform.startswith("linux"):
                print("Provisioning requires access to port '{}', please correct."
                      .format(port))
                # If there is a group named "dialout" and user is not member, print advice.
                try:
                    dialout = grp.getgrnam("dialout")
                    if not dialout.gr_gid in os.getgroups():
                        print("Need to be member of group 'dialout' in order to access '{}'"
                              .format(port))
                        print("To fix: console command 'sudo adduser $USER dialout',"
                              " then log out and in again")
                except:
                    pass
            return False
    return True


def portguess(serial_number=None):
    """
    Try to determine kit virtual port automatically.
    """

    nedbg_vidpid = "03EB:2175" # == "nEDBG CMSIS-DAP", name not reported on Windows
    matches = []
    usbports = [p for p in serial.tools.list_ports.comports() if "USB" in p.hwid]
    for port in usbports:
        if serial_number and serial_number == port.serial_number:
            matches = [port]            # guaranteed unique
            break
        #elif args.tool and args.tool in [port.product, port.description]:
        #    matches.append(port)
        elif nedbg_vidpid in port.hwid:  # identifies nEDBG
            matches.append(port)

    if len(matches) == 1:
        tool = matches[0]
        if not check_access(tool.device):  #and "provision" in args.actions:
            sys.exit(1) # No point in even trying if we got here...
        return tool.device

    elif len(matches) > 1:
        # This should not happen because already eliminated in get_usable_kits()
        print("Multiple candidate kits found, please choose one:", file=sys.stderr)
        for port in matches:
            print("{}\t{}\t{}".format(port.device, port.description, port.hwid), file=sys.stderr)
        return None
    else:
        print("No usable kits found. Is your IoT kit connected?", file=sys.stderr)
        return None
