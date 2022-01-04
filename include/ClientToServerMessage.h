using namespace std;
#include <vector>
#include <string>

#ifndef ASSIGNMENT3_SPL_CLIENT_CLIENTTOSERVERMESSAGE_H
#define ASSIGNMENT3_SPL_CLIENT_CLIENTTOSERVERMESSAGE_H


enum cts_message_type {
    REGISTER = 1,
    LOGIN = 2,
    LOGOUT = 3,
    FOLLOW_OR_UNFOLLOW = 4,
    POST = 5,
    PRIVATE_MESSAGE = 6,
    LOGGED_IN_STATES = 7,
    STATISTICS = 8,
    BLOCK = 12,
    FETCH_NOTIFICATION = 13
};

class ClientToServerMessage{
public:
    ClientToServerMessage(cts_message_type type, string command);
    virtual const vector<char> encode()=0;
private:
    cts_message_type type;
    string command;
};

class RegisterMessage : public ClientToServerMessage {
public:
    explicit RegisterMessage(const string &command);
    const vector<char> encode();
private:
    string username;
    string password;
    string birthday;
};

class LoginMessage : public ClientToServerMessage {
public:
    LoginMessage(const string &command);
    const vector<char> encode();
private:
    string username;
    string password;
    bool captcha;
};

class LogoutMessage : public ClientToServerMessage {
public:
    LogoutMessage(const string &command, string userName);
    const vector<char> encode();
};

class FollowOrUnfollowMessage : public ClientToServerMessage {
public:
    FollowOrUnfollowMessage(const string &command, string userName);
    const vector<char> encode();
private:
    bool follow;
    string otherUserName;
};

class PostMessage : public ClientToServerMessage {
public:
    PostMessage(const string &command, string userName);
    const vector<char> encode();
private:
    string content;

};

class PMMessage : public ClientToServerMessage {
public:
    PMMessage(const string &command, string userName);
    const vector<char> encode();
private:
    string content;
    string sendingTimeAndDate;
    string otherUserName;
};

class LoggedInStates : public ClientToServerMessage {
public:
    LoggedInStates(const string &command, string userName);
    const vector<char> encode();
};

class StatisticsMessage : public ClientToServerMessage {
public:
    StatisticsMessage(const string &command, string userName);
    const vector<char> encode();
private:
    vector<string> userNamesList;
};

class BlockMessage : public ClientToServerMessage {
public:
    BlockMessage(const string &command, string userName);
    const vector<char> encode();
private:
    string otherUserName;
};

class FetchNotificationMessage : public ClientToServerMessage {
public:
    FetchNotificationMessage(const string &command, string userName);
    const vector<char> encode();

};




#endif //ASSIGNMENT3_SPL_CLIENT_CLIENTTOSERVERMESSAGE_H
