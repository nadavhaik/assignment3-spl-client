#include "../include/SessionData.h"
#include <chrono>
#include <thread>

int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }

    std::string host = argv[1];
    short port = atoi(argv[2]);
    try {
        shared_ptr<SessionData> s = make_shared<SessionData>(host, port);
        std::thread t1(SessionData::run, s);
        std::thread t2(SessionData::fetchNotifications, s);
        t2.detach();
        t1.join();
        if(t2.joinable())
            t2.join();
        this_thread::sleep_for(chrono::milliseconds(500));

    } catch(connection_exception &e) {
        std::cerr << "Could not connect to host " << argv[1] << " in port " << argv[2] << std::endl << std::endl;
        return -1;
    }

    return 0;

}

