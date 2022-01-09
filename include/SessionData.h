#include <string>
#include <mutex>
#include "ConnectionHandler.h"
#include "ClientToServerMessage.h"
#include "ServerToClientMessage.h"
using namespace std;

class communication_exception : std::exception {};

#ifndef ASSIGNMENT3_SPL_CLIENT_SESSIONDATA_H
#define ASSIGNMENT3_SPL_CLIENT_SESSIONDATA_H

class SessionData {
public:
    SessionData(string ip, short port);
    ~SessionData();
    static void run(shared_ptr<SessionData> sd);
    static void fetchNotifications(shared_ptr<SessionData> sd);
    ServerToClientMessage *communicate(ClientToServerMessage &message);
    bool isLoggedIn();
    bool getShouldStop();
    void stop();
private:
    ConnectionHandler ch;
    bool loggedIn = false;
    bool shouldStop = false;
    mutex connectionLock;
};


#endif //ASSIGNMENT3_SPL_CLIENT_SESSIONDATA_H
