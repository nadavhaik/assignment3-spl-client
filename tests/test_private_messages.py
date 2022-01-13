import traceback

from tests_functions import *
from datetime import datetime
import sys


def test():
    valgrind_mode = "-valgrind" in sys.argv
    client_nadav = RunningClient(valgrind_mode)
    client_niv = RunningClient(valgrind_mode)
    client_limor = RunningClient(valgrind_mode)
    try:

        client_nadav.assert_command_returns("REGISTER nadavh nadavh 29-03-1996", "ACK 1")
        client_nadav.assert_command_returns("LOGIN nadavh nadavh 1", "ACK 2")

        client_niv.assert_command_returns("REGISTER niv niv 01-04-1996", "ACK 1")
        client_niv.assert_command_returns("LOGIN niv niv 1", "ACK 2")

        client_limor.assert_command_returns("REGISTER limor limor 29-05-1963", "ACK 1")
        client_limor.assert_command_returns("LOGIN limor limor 1", "ACK 2")

        client_nadav.assert_command_returns("PM niv MA KORE?", "ERROR 6")

        client_nadav.assert_command_returns("FOLLOW 0 niv", "ACK 4 niv")
        client_nadav.assert_command_returns("PM niv Trump is the president", "ACK 6")
        client_niv.assert_got_output("NOTIFICATION PM nadavh <filtered> is the president "
                                     + datetime.today().strftime("%d-%m-%Y"))

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
