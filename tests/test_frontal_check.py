import traceback

from tests_functions import *
from datetime import datetime


def test():
    client_biden = RunningClient()
    client_trump = RunningClient()
    client_obama = RunningClient()
    try:
        client_biden.assert_command_returns("REGISTER biden bidy123 02-04-1991", "ACK 1")
        # client_biden.assert_command_returns("LOGIN hilary hela1 1", "ERROR 2")
        client_biden.assert_command_returns("LOGIN biden bidy123 1", "ACK 2")
        client_obama.assert_command_returns("REGISTER obama bambam 14-03-1993", "ACK 1")
        client_obama.assert_command_returns("LOGIN obama bambam 1", "ACK 2")
        client_biden.assert_command_returns("FOLLOW 0 obama", "ACK 4 obama")
        client_biden.assert_command_returns("PM obama I am the damn president!", "ACK 6")
        client_obama.assert_got_output("NOTIFICATION PM obama I am the damn president!"
                                     "<filtered> " + datetime.today().strftime("%d-%m-%Y"))

    finally:
        try:
            client_biden.close_client()
        except Exception:
            print(traceback.format_exc())
        try:
            client_trump.close_client()
        except Exception:
            print(traceback.format_exc())
        try:
            client_obama.close_client()
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
