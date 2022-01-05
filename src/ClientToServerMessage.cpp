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
    : ClientToServerMessage(REGISTER) {
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

LoginMessage::LoginMessage(const string &command)
    : ClientToServerMessage(LOGIN) {
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

FollowOrUnfollowMessage::FollowOrUnfollowMessage(const string &command)
        : ClientToServerMessage(FOLLOW_OR_UNFOLLOW) {
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

PostMessage::PostMessage(const string &command) : ClientToServerMessage(POST) {
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    if(result.size() < 2)
        throw parsing_exception();
    for(size_t i=1; i<result.size(); i++) {
        content += result[i];
        content += " ";
    }
    content = content.substr(0, content.size()-2);
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


PMMessage::PMMessage(const string &command) : ClientToServerMessage(PRIVATE_MESSAGE) {
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    if(result.size() < 3)
        throw parsing_exception();
    otherUserName = result[1];
    for(size_t i=2; i<result.size(); i++) {
        content += result[i];
        content += " ";
    }
    content = content.substr(0, content.size()-2);

    auto t = time(nullptr);
    auto tm = *localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M");
    sendingTimeAndDate = oss.str();
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

LoggedInStates::LoggedInStates(const string &command): ClientToServerMessage(LOGGED_IN_STATES) {}

vector<char> LoggedInStates::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);

    return encodedCommand;
}

StatisticsMessage::StatisticsMessage(const string &command) : ClientToServerMessage(STATISTICS) {
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

BlockMessage::BlockMessage(const string &command) : ClientToServerMessage(BLOCK) {
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

FetchNotificationMessage::FetchNotificationMessage() : ClientToServerMessage(FETCH_NOTIFICATION) {}

vector<char> FetchNotificationMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector((short)getType());
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);

    return encodedCommand;
}

