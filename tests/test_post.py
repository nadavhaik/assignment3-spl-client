import traceback

from tests_functions import *
from datetime import datetime


def test():
    client_nadav = RunningClient()
    client_niv = RunningClient()
    client_limor = RunningClient()
    try:

        client_nadav.assert_command_returns("REGISTER nadavh nadavh 29-03-1996", "ACK 1")
        client_nadav.assert_command_returns("LOGIN nadavh nadavh 1", "ACK 2")

        client_niv.assert_command_returns("REGISTER niv niv 01-04-1996", "ACK 1")
        client_niv.assert_command_returns("LOGIN niv niv 1", "ACK 2")

        client_nadav.assert_command_returns("POST @niv MA KORE?", "ACK 5")

        client_niv.assert_got_output("NOTIFICATION Public nadavh @niv MA KORE?")
        try:
            client_niv.close_client()
        except Exception:
            print(traceback.format_exc())
        client_nadav.assert_command_returns("POST @niv MA HAMATZAV?", "ACK 5")
        client_niv = RunningClient()
        client_niv.assert_command_returns("LOGIN niv niv 1", "ACK 2")
        client_niv.assert_got_output("NOTIFICATION Public nadavh @niv MA HAMATZAV?")

        client_nadav.assert_command_returns("FOLLOW 0 niv", "ACK 4 niv")

        client_limor.assert_command_returns("REGISTER limor limor 29-05-1963", "ACK 1")
        client_limor.assert_command_returns("LOGIN limor limor 1", "ACK 2")
        client_limor.assert_command_returns("FOLLOW 0 niv", "ACK 4 niv")

        client_niv.assert_command_returns("POST YOU ARE ON MY DICK", "ACK 5")

        client_nadav.assert_got_output("NOTIFICATION Public niv YOU ARE ON MY DICK")
        client_limor.assert_got_output("NOTIFICATION Public niv YOU ARE ON MY DICK")


    finally:
        try:
            client_nadav.close_client()
        except Exception:
            print(traceback.format_exc())
        try:
            client_niv.close_client()
        except Exception:
            print(traceback.format_exc())
        try:
            client_limor.close_client()
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
