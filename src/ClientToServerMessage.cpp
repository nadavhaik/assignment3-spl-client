//
// Created by niv19 on 04/01/2022.
//
#include "../include/ClientToServerMessage.h"
#include "../include/Caster.h"
#include <boost/algorithm/string.hpp>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

ClientToServerMessage::ClientToServerMessage(cts_message_type type): type(type) {}

int ClientToServerMessage::getType() {
    return type;
}

RegisterMessage::RegisterMessage(const string &command)
    : ClientToServerMessage(REGISTER), username(), password(), birthday() {
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    if(result.size() != 4)
        throw parsing_exception();
    username = result[1];
    password = result[2];
    birthday = result[3];
}

vector<char> RegisterMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);
    for (char &c : username)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');
    for (char &c : password)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');
    for (char &c : birthday)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');

    return encodedCommand;
}

RegisterMessage::~RegisterMessage()=default;

LoginMessage::LoginMessage(const string &command)
    :ClientToServerMessage(LOGIN), username(), password(), captcha() {
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    if(result.size() != 4)
        throw parsing_exception();
    split(result, command, boost::is_any_of(" "));
    username = result[1];
    password = result[2];
    captcha = result[3] == "1";
}

vector<char> LoginMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);

    for (char &c : username)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');

    for (char &c : password)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');
    if (captcha)
        encodedCommand.push_back((char)1);
    else
        encodedCommand.push_back((char)0);

    return encodedCommand;
}

LoginMessage::~LoginMessage()=default;

LogoutMessage::LogoutMessage(const string &command)
    : ClientToServerMessage(LOGOUT) {
    if(command != "LOGOUT")
        throw parsing_exception();
}

vector<char> LogoutMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);
    return encodedCommand;
}

LogoutMessage::~LogoutMessage()=default;

FollowOrUnfollowMessage::FollowOrUnfollowMessage(const string &command)
        :ClientToServerMessage(FOLLOW_OR_UNFOLLOW), follow(false), otherUserName() {
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    if(result.size() != 3)
        throw parsing_exception();
    follow = result[1] == "0";
    otherUserName = result[2];
}

vector<char> FollowOrUnfollowMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);
    if(follow)
        encodedCommand.push_back((char)0);
    else
        encodedCommand.push_back((char)1);
    for (char &c : otherUserName)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');

    return encodedCommand;
}

FollowOrUnfollowMessage::~FollowOrUnfollowMessage()=default;

PostMessage::PostMessage(const string &command) :ClientToServerMessage(POST), content() {
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    if(result.size() < 2)
        throw parsing_exception();
    for(size_t i=1; i<result.size(); i++) {
        content += result[i];
        content += " ";
    }
    content = content.substr(0, content.size()-1);
}

vector<char> PostMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);
    for(char c : content)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');
    return encodedCommand;
}

PostMessage::~PostMessage()=default;


PMMessage::PMMessage(const string &command)
        :ClientToServerMessage(PRIVATE_MESSAGE), otherUserName(), content(), sendingTimeAndDate() {
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    if(result.size() < 3)
        throw parsing_exception();
    otherUserName = result[1];
    for(size_t i=2; i<result.size(); i++) {
        content += result[i];
        content += " ";
    }
    content = content.substr(0, content.size()-1);

    auto t = time(nullptr);
    auto *tm = localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%d-%m-%Y %H:%M");
    sendingTimeAndDate = oss.str();
    delete tm;
}

vector<char> PMMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);
    for(char &c : otherUserName)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');
    for(char &c : content)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');
    for(char &c : sendingTimeAndDate)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');

    return encodedCommand;
}

PMMessage::~PMMessage()=default;

LoggedInStates::LoggedInStates(const string &command): ClientToServerMessage(LOGGED_IN_STATES) {}

vector<char> LoggedInStates::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);

    return encodedCommand;
}

LoggedInStates::~LoggedInStates()=default;

StatisticsMessage::StatisticsMessage(const string &command) : ClientToServerMessage(STATISTICS), usernames() {
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    if(result.size() != 2)
        throw parsing_exception();
    usernames = result[1];
}

vector<char> StatisticsMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);
    for(char &c : usernames)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');

    return encodedCommand;
}

StatisticsMessage::~StatisticsMessage()=default;

BlockMessage::BlockMessage(const string &command) : ClientToServerMessage(BLOCK), otherUserName() {
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    if(result.size() != 2)
        throw parsing_exception();
    otherUserName = result[1];
}

vector<char> BlockMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);
    for(char &c : otherUserName)
        encodedCommand.push_back(c);
    encodedCommand.push_back('\0');

    return encodedCommand;
}

BlockMessage::~BlockMessage()=default;

FetchNotificationMessage::FetchNotificationMessage() : ClientToServerMessage(FETCH_NOTIFICATION) {}

vector<char> FetchNotificationMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);

    return encodedCommand;
}

FetchNotificationMessage::~FetchNotificationMessage()=default;

