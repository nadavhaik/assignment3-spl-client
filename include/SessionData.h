#include <string>
#include "ConnectionHandler.h"
using namespace std;


#ifndef ASSIGNMENT3_SPL_CLIENT_SESSIONDATA_H
#define ASSIGNMENT3_SPL_CLIENT_SESSIONDATA_H

class SessionData {
public:
    SessionData(string ip, string port);
    void run();
private:
    ConnectionHandler ch;
    void fetchNotifications();
    bool loggedIn = false;
    bool shouldStop = false;
};


#endif //ASSIGNMENT3_SPL_CLIENT_SESSIONDATA_H
