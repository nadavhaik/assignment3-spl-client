//
// Created by niv19 on 04/01/2022.
//
#include "../include/ClientToServerMessage.h"
#include "Casters.cpp"
#include <boost/algorithm/string.hpp>

using namespace std;

ClientToServerMessage::ClientToServerMessage(cts_message_type type, string command): type(type), command(command) {}

RegisterMessage::RegisterMessage(const string &command)
    : ClientToServerMessage(REGISTER, command) {}

vector<char> RegisterMessage::encode() {
    vector<char> encodedCommand;
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    vector<char> opCode = shortToBytesVector(1);
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);

    username = result[1];
    for (int i = 0; i < username.size(); i++) {
        encodedCommand.push_back(username[i]);
    }
    encodedCommand.push_back('\0');

    password = result[2];
    for (int i = 0; i < password.size(); i++) {
        encodedCommand.push_back(password[i]);
    }
    encodedCommand.push_back('\0');

    birthday = result[3];
    for (int i = 0; i < birthday.size(); i++) {
        encodedCommand.push_back(birthday[i]);
    }
    encodedCommand.push_back('\0');

    return encodedCommand;
}

LoginMessage::LoginMessage(const string &command)
    : ClientToServerMessage(LOGIN, command) {}

vector<char> LoginMessage::encode() {
    vector<char> encodedCommand;
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    vector<char> opCode = shortToBytesVector(2);
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);

    username = result[1];
    for (int i = 0; i < username.size(); i++) {
        encodedCommand.push_back(username[i]);
    }
    encodedCommand.push_back('\0');

    password = result[2];
    for (int i = 0; i < password.size(); i++) {
        encodedCommand.push_back(password[i]);
    }
    encodedCommand.push_back('\0');

    captcha = result[3] == "1";
    if (result[3] == "1") {
        encodedCommand.push_back((char)1);
    }
    else{
        encodedCommand.push_back((char)0);
    }

    return encodedCommand;
}

LogoutMessage::LogoutMessage(const string &command)
    : ClientToServerMessage(LOGOUT, command), {}

vector<char> LogoutMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = shortToBytesVector(3);
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);
    return encodedCommand;
}

FollowOrUnfollowMessage::FollowOrUnfollowMessage(const string &command)
        : ClientToServerMessage(FOLLOW_OR_UNFOLLOW, command), {}

vector<char> FollowOrUnfollowMessage::encode() {
    vector<char> encodedCommand;
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    vector<char> opCode = shortToBytesVector(4);
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);
    follow = result[1] == "0";
    if(follow)
        encodedCommand.push_back((char)0);
    else
        encodedCommand.push_back((char)1);
    
    otherUserName = result[2];
    for (int i = 0; i < otherUserName.size(); i++) {
        encodedCommand.push_back(otherUserName[i]);
    }
    encodedCommand.push_back('\0');

    return encodedCommand;
}
