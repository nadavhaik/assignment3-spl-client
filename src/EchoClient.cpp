#include "../include/SessionData.h"

int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }

    std::string host = argv[1];
    short port = atoi(argv[2]);
    try {
        SessionData s(host, port);
        s.run();
    } catch(connection_exception e) {
        std::cerr << "Could not connect to host " << argv[1] << " in port " << argv[2] << std::endl << std::endl;
        return -1;
    }

    return 0;

}

