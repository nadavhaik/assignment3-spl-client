#include <stdlib.h>

#include "../include/connectionHandler.h"
#include "../include/ClientToServerMessage.h"
#include "../include/ServerToClientMessage.h"
#include "Casters.cpp"
#include "../include/SessionData.h"


/**

* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)

*/

int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }

    std::string host = argv[1];
    short port = atoi(argv[2]);
    SessionData s(host, port);
    s.run();

    return 0;

}

