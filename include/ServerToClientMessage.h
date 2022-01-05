using namespace std;
#include <vector>
#include <string>
#include "ClientToServerMessage.h"
#ifndef ASSIGNMENT3_SPL_CLIENT_SERVERTOCLIENTMESSAGE_H
#define ASSIGNMENT3_SPL_CLIENT_SERVERTOCLIENTMESSAGE_H
class connection_exception : std::exception {};

enum stc_message_type {
    NOTIFICATION = 9,
    ACK = 10,
    ERROR = 11
};

enum notification_type {
    POST_NOTIFICATION,
    PM_NOTIFICATION
};

class ServerToClientMessage {
public:
    explicit ServerToClientMessage(stc_message_type type, vector<char> bytes);
    ~ServerToClientMessage()=default;
    virtual std::string toString()=0;
    int getType();
protected:
    vector<char> bytes;
    stc_message_type notificationType;
};

class NotificationMessage : public ServerToClientMessage {
public:
    explicit NotificationMessage(const vector<char> &bytes);
    void decode();
    std::string toString() override;
private:
    notification_type n_type;
    std::string author;
    std::string content;
};

class AckMessage : public ServerToClientMessage {
public:
    explicit AckMessage(const vector<char> &bytes, int originalMessageType);
    void decode();
    std::string toString() override;
private:
    short messageOP;
    string content;
    int originalMessageType;
};

class ErrorMessage : public ServerToClientMessage {
public:
    explicit ErrorMessage(const vector<char> &bytes);
    void decode();
    std::string toString() override;
private:
    short messageOP;
};




#endif //ASSIGNMENT3_SPL_CLIENT_SERVERTOCLIENTMESSAGE_H
