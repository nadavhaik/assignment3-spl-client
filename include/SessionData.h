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
    void run();
private:
    ConnectionHandler ch;
    bool loggedIn = false;
    bool shouldStop = false;
    mutex connectionLock;
    void fetchNotifications();
    ServerToClientMessage *communicate(const ClientToServerMessage &message);
};


#endif //ASSIGNMENT3_SPL_CLIENT_SESSIONDATA_H
