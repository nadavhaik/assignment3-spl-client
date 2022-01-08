from tests_functions import *

build()
client = RunningClient()

try:
    client.assert_command_returns("LOGIN nadavh nadavh 1", "ERROR 2")

    client.assert_command_returns("REGISTER nadavh nadavh 29-03-1996", "ACK 1")
    client.assert_command_returns("REGISTER nadavh 1234 01-05-1965", "ERROR 1")

    client.assert_command_returns("LOGIN nadavh 1234 1", "ERROR 2")
    client.assert_command_returns("LOGIN nadavh nadavh 0", "ERROR 2")
    client.assert_command_returns("LOGIN nadavh nadavh 1", "ACK 2")


finally:
    client.close_client()
