import traceback

from tests_functions import *
from datetime import datetime


def test():
    client_nadav = RunningClient()
    client_niv = RunningClient()
    try:
        client_nadav.assert_command_returns("REGISTER nadavh nadavh 29-03-1996", "ACK 1")
        client_nadav.assert_command_returns("LOGIN nadavh nadavh 1", "ACK 2")
        client_nadav.assert_command_returns("BLOCK niv", "ERROR 12")

        client_niv.assert_command_returns("REGISTER niv niv 01-04-1996", "ACK 1")
        client_niv.assert_command_returns("LOGIN niv niv 1", "ACK 2")

        client_nadav.assert_command_returns("FOLLOW 0 niv", "ACK 4 niv")
        client_niv.assert_command_returns("FOLLOW 0 nadavh", "ACK 4 nadavh")

        client_niv.assert_command_returns("STAT nadavh", "ACK 8 25 0 1 1")
        client_nadav.assert_command_returns("STAT niv", "ACK 8 25 0 1 1")

        client_nadav.assert_command_returns("BLOCK niv", "ACK 12")
        client_nadav.assert_command_returns("STAT nadavh", "ACK 8 25 0 0 0")
        client_nadav.assert_command_returns("STAT niv", "ERROR 8")
        client_niv.assert_command_returns("STAT nadavh", "ERROR 8")
        client_niv.assert_command_returns("STAT niv", "ACK 8 25 0 0 0")

        client_nadav.assert_command_returns("POST @niv where are you???", "ACK 5")
        client_niv.assert_got_output("")
        client_niv.assert_command_returns("FOLLOW 0 nadavh", "ERROR 4")
        client_nadav.assert_command_returns("LOGSTAT", "ACK 7 25 1 0 0")



    finally:
        try:
            client_nadav.close_client()
        except Exception:
            print(traceback.format_exc())
        try:
            client_niv.close_client()
        except Exception:
            print(traceback.format_exc())

def main():
    RunningServer.get_instance().start(ServerArchitecture.TPC)
    test()
    RunningServer.get_instance().stop()

    RunningServer.get_instance().start(ServerArchitecture.REACTOR)
    test()
    RunningServer.get_instance().stop()


if __name__ == "__main__":
    main()
