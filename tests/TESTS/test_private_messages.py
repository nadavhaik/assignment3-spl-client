from tests_functions import *
from datetime import datetime

build()
client_nadav = RunningClient()
client_niv = RunningClient()
client_limor = RunningClient()

try:
    client_nadav.assert_command_returns("REGISTER nadavh nadavh 29-03-1996", "ACK 1")
    client_nadav.assert_command_returns("LOGIN nadavh nadavh 1", "ACK 2")

    client_niv.assert_command_returns("REGISTER niv niv 01-04-1996", "ACK 1")
    client_niv.assert_command_returns("LOGIN niv niv 1", "ACK 2")

    client_limor.assert_command_returns("REGISTER limor limor 29-05-1963", "ACK 1")
    client_limor.assert_command_returns("LOGIN limor limor 1", "ACK 2")

    client_nadav.assert_command_returns("PM niv MA KORE?", "ERROR 6")

    client_nadav.assert_command_returns("FOLLOW 0 niv", "ACK 4 niv")
    client_nadav.assert_command_returns("PM niv Trump your mom is Trump I Trump her in the Trump", "ACK 6")
    client_niv.assert_got_output("NOTIFICATION PM nadavh <filtered> your mom is <filtered> I <filtered> her in the "
                                 "<filtered> " + datetime.today().strftime("%d-%m-%Y"))



finally:
    client_nadav.close_client()
    client_niv.close_client()
    client_limor.close_client()
