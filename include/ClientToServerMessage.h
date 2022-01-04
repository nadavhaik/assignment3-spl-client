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
    virtual vector<char> encode() const=0;
private:
    cts_message_type type;
    string command;
};

class RegisterMessage : public ClientToServerMessage {
public:
    explicit RegisterMessage(const string &command);
    vector<char> encode() const override;
private:
    string username;
    string password;
    string birthday;
};

class LoginMessage : public ClientToServerMessage {
public:
    LoginMessage(const string &command);
    vector<char> encode() const override;
private:
    string username;
    string password;
    bool captcha;
};

class LogoutMessage : public ClientToServerMessage {
public:
    LogoutMessage(const string &command, string userName);
    vector<char> encode() const override;
};

class FollowOrUnfollowMessage : public ClientToServerMessage {
public:
    FollowOrUnfollowMessage(const string &command, string userName);
    vector<char> encode() const override;
private:
    bool follow;
    string otherUserName;
};

class PostMessage : public ClientToServerMessage {
public:
    PostMessage(const string &command, string userName);
    vector<char> encode() const override;
private:
    string content;

};

class PMMessage : public ClientToServerMessage {
public:
    PMMessage(const string &command, string userName);
    vector<char> encode() const override;
private:
    string content;
    string sendingTimeAndDate;
    string otherUserName;
};

class LoggedInStates : public ClientToServerMessage {
public:
    LoggedInStates(const string &command, string userName);
    vector<char> encode() const override;
};

class StatisticsMessage : public ClientToServerMessage {
public:
    StatisticsMessage(const string &command, string userName);
    vector<char> encode() const override;
private:
    vector<string> userNamesList;
};

class BlockMessage : public ClientToServerMessage {
public:
    BlockMessage(const string &command, string userName);
    vector<char> encode() const override;
private:
    string otherUserName;
};

class FetchNotificationMessage : public ClientToServerMessage {
public:
    FetchNotificationMessage(const string &command, string userName);
    vector<char> encode() const override;

};




#endif //ASSIGNMENT3_SPL_CLIENT_CLIENTTOSERVERMESSAGE_H
