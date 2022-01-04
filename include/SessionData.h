#include <string>
#include <mutex>
#include "ConnectionHandler.h"
#include "ClientToServerMessage.h"
#include "ServerToClientMessage.h"
using namespace std;


#ifndef ASSIGNMENT3_SPL_CLIENT_SESSIONDATA_H
#define ASSIGNMENT3_SPL_CLIENT_SESSIONDATA_H

class SessionData {
public:
    SessionData(string ip, short port);
    ~SessionData();
    void run();
    ServerToClientMessage *communicate(ClientToServerMessage &message);
    bool isLoggedIn();
    bool getShouldStop();
private:
    ConnectionHandler ch;
    bool loggedIn = false;
    bool shouldStop = false;
    mutex connectionLock;
};


#endif //ASSIGNMENT3_SPL_CLIENT_SESSIONDATA_H
