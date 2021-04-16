"""
Programming related functions, using pymcuprog backend API.
"""
import os
import sys
import logging

from pymcuprog.backend import Backend, SessionConfig
from pymcuprog.toolconnection import ToolUsbHidConnection
from pymcuprog.deviceinfo.memorynames import MemoryNames

#@@FIXME: Use logging rather than print below.

class Programmer(object):
    """
    Class to interface pymcuprog backend.
    """
    # packpath is only required for PIC devices.
    PACKPATH_TPL = "picpack/{device}"

    def __init__(self, serial=None, loglevel=logging.WARNING, installdir="."):
        self.serial = serial
        self.backend = Backend(loglevel=loglevel)
        self.kits = []
        self.tool = None
        self.packpath = None

        tools = self.backend.get_available_hid_tools()

        # Need to read info from each tool in order to get device name etc.
        for tool in tools:
            # If a (partial) serial number was specified by user, only consider matching tool(s).
            if serial and serial != tool.serial_number[-len(serial):]:
                continue
            try:
                self.backend.connect_to_tool(ToolUsbHidConnection(serial_number=tool.serial_number))
                self.kits.append(self.backend.read_tool_info())
            except Exception as e:
                # If we get here the tool is definitely unsuitable, print error and ignore it.
                print("Error, tool = {}: {}".format(tool.product_string, e),
                      file=sys.stderr)
            finally:
                self.backend.disconnect_from_tool()

            if len(self.kits) == 1:
                self.tool = self.kits[0]
                dev = self.tool["device_name"].lower()
                self.packpath = os.path.join(installdir, self.PACKPATH_TPL.format(device=dev))


    def get_usable_kits(self):
        """
        Get a list of usable kits. Programming methods below will only
        work if there is exactly one usable kit in the list.
        """
        return self.kits


    def program_hexfile(self, filename, memory=MemoryNames.FLASH, erase=True, verify=False):
        """
        Program target with hex file. Returns: True if OK
        """
        try:
            # Set up session
            self.backend.connect_to_tool(ToolUsbHidConnection(serial_number=self.tool["serial"]))
            sessionconfig = SessionConfig(self.tool["device_name"])
            sessionconfig.packpath = self.packpath
            self.backend.start_session(sessionconfig)
            if erase and memory in [MemoryNames.FLASH, MemoryNames.EEPROM]:
                print("Erasing {}...".format(memory))
                self.backend.erase(memory)
            print("Programming {}...".format(memory))
            self.backend.write_hex_to_target(filename)
            if verify:
                print("Verifying {}...".format(memory))
                return self.backend.verify_hex(filename)
            return True

        #except Exception as e:  # No, let it propagate

        finally:
            # Clean up session
            self.backend.end_session()
            self.backend.disconnect_from_tool()


    def program_data(self, data, memory=MemoryNames.FLASH, offset=0, erase=True, verify=False):
        """
        Program target with memory data, assumed to be a byte array. Returns: True if OK
        """
        try:
            # Set up session
            self.backend.connect_to_tool(ToolUsbHidConnection(serial_number=self.tool["serial"]))
            sessionconfig = SessionConfig(self.tool["device_name"])
            sessionconfig.packpath = self.packpath
            self.backend.start_session(sessionconfig)
            # Attempting to erase other memory types may have unexpected side effects.
            # See also DSG-1408
            if erase and memory in [MemoryNames.FLASH, MemoryNames.EEPROM]:
                print("Erasing {}...".format(memory))
                self.backend.erase(memory)
            print("Programming {}...".format(memory))
            self.backend.write_memory(data, memory, offset_byte=offset)
            if verify:
                print("Verifying {}...".format(memory))
                return self.backend.verify_memory(data, memory, offset_byte=offset)
            return True

        #except Exception as e: # No, let it propagate

        finally:
            # Clean up session
            self.backend.end_session()
            self.backend.disconnect_from_tool()


    def read_data(self, memory=MemoryNames.FLASH, offset=0, size=0):
        """
        Read data from device memory. This function only supports reading a
        single memory, and returns a single bytearray. size=0 means read
        the whole memory.
        """
        try:
            # Set up session
            self.backend.connect_to_tool(ToolUsbHidConnection(serial_number=self.tool["serial"]))
            sessionconfig = SessionConfig(self.tool["device_name"])
            sessionconfig.packpath = self.packpath
            self.backend.start_session(sessionconfig)
            memories = self.backend.read_memory(memory, offset_byte=offset, numbytes=size)
            return memories[0].data

        #except Exception as e: # No, let it propagate

        finally:
            # Clean up session
            self.backend.end_session()
            self.backend.disconnect_from_tool()
