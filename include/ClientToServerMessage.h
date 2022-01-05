using namespace std;
#include <vector>
#include <string>

#ifndef ASSIGNMENT3_SPL_CLIENT_CLIENTTOSERVERMESSAGE_H
#define ASSIGNMENT3_SPL_CLIENT_CLIENTTOSERVERMESSAGE_H

class parsing_exception : std::exception {};

enum cts_message_type {
    REGISTER = 1,
    LOGIN = 2,
    LOGOUT = 3,
    FOLLOW_OR_UNFOLLOW = 4, // stopped implementing here
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
    ~ClientToServerMessage()=default;
    virtual vector<char> encode()=0;
    int getType();
private:
    cts_message_type type;
protected:
    string command;
};

class RegisterMessage : public ClientToServerMessage {
public:
    explicit RegisterMessage(const string &command);
    vector<char> encode() override;
private:
    string username;
    string password;
    string birthday;
};

class LoginMessage : public ClientToServerMessage {
public:
    explicit LoginMessage(const string &command);
    vector<char> encode() override;
private:
    string username;
    string password;
    bool captcha; // where do we check the captcha
};

class LogoutMessage : public ClientToServerMessage {
public:
    explicit LogoutMessage(const string &command);
    vector<char> encode() override;
};

class FollowOrUnfollowMessage : public ClientToServerMessage {
public:
    explicit FollowOrUnfollowMessage(const string &command);
    vector<char> encode() override;
private:
    bool follow;
    string otherUserName;
};

class PostMessage : public ClientToServerMessage {
public:
    PostMessage(const string &command);
    vector<char> encode() override;
private:
    string content;

};

class PMMessage : public ClientToServerMessage {
public:
    PMMessage(const string &command);
    vector<char> encode() override;
private:
    string content;
    string sendingTimeAndDate;
    string otherUserName;
};

class LoggedInStates : public ClientToServerMessage {
public:
    LoggedInStates(const string &command);
    vector<char> encode() override;
};

class StatisticsMessage : public ClientToServerMessage {
public:
    StatisticsMessage(const string &command);
    vector<char> encode() override;
private:
    vector<string> userNamesList;
};

class BlockMessage : public ClientToServerMessage {
public:
    BlockMessage(const string &command);
    vector<char> encode() override;
private:
    string otherUserName;
};

class FetchNotificationMessage : public ClientToServerMessage {
public:
    FetchNotificationMessage();
    vector<char> encode() override;

};




#endif //ASSIGNMENT3_SPL_CLIENT_CLIENTTOSERVERMESSAGE_H
