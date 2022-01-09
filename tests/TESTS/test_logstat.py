import traceback

from tests_functions import *
from datetime import datetime


def test():
    client_nadav = RunningClient()
    try:
        client_nadav.assert_command_returns("REGISTER nadavh nadavh 29-03-1996", "ACK 1")
        client_nadav.assert_command_returns("LOGIN nadavh nadavh 1", "ACK 2")
        client_nadav.assert_command_returns("LOGSTAT", "ACK 7 25 0 0 0")

    finally:
        try:
            client_nadav.close_client()
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
