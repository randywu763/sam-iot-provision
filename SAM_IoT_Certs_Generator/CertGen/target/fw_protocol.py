"""
Implement the provisioning firmware communication protocol.
Commands and responses are 0-terminated strings as described below.

Command syntax: <cmd>:[<arg>[,<arg>...]] NOTE multiple args not
                                         yet implemented in FW!
                <arg> is hex-encoded blob

Response: <hex> | "OK" | "ERROR:"<xxxx>
          <hex> is hex-encoded blob, <xxxx> 16-bit hex error code
"""

import sys
from time import sleep
from hexdump import hexdump
if __package__:
    from .fw_error import format_error
else:
    from fw_error import format_error


def fw_request(com, cmd, args=None, msg="", dump=False):
    """
    Send a request to the kit provisioning FW, receive response.
    Optionally print a message and/or hexdump response.
    Error message printed to stderr.
    Due to pyserial lacks explicit mechanism for detecting timeout,
    (like raising exception), a protocol workaround had to be found
    (non-empty positive response from FW)
    """
    args = args or []
    if msg:
        print(msg)
    buffer = ("{}:{}\0").format(cmd, ",".join(args)).encode()
    if dump:
        print("{}: length {}".format(cmd, len(buffer)))
    com.write(buffer)
    response = com.read_until(b'\x00')[:-1].decode("utf-8")
    print("BUF => {}".format(response))
    if response.startswith("OK"):  # Positive response without data
        return ""
    elif response.startswith("ERROR"):
        print(format_error(response, cmd), file=sys.stderr)
        return None
    elif response:                 # Positive response with data
        if dump:
            hexdump(bytes.fromhex(response))
        return response
    else:
        print("ERROR: Timeout waiting for response from FW (cmd = {})".format(cmd),
              file=sys.stderr)
        return None



def demo_fw_command(com, cmd, args=[]):
    """
    Send a request to demo FW CLI, return response.
    """
    EOT = b'\\04'  # end of transmission from target
    buffer = ("{} {}\n").format(cmd, ",".join(args)).encode()
    sleep(0.1)     # Mystery delay between requests seems to avoid mixed-up responses
    # Experimental: Try to send a single character at a time to solve instability
    for c in buffer:
        buf = [c]
        com.write(buf)
        com.flush()
    response = com.read_until(EOT)[:-1].decode("utf-8", errors="ignore")
    return response
