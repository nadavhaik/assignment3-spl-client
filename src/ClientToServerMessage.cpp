//
// Created by niv19 on 04/01/2022.
//
#include "../include/ClientToServerMessage.h"
#include "../include/Caster.h"
#include <boost/algorithm/string.hpp>

using namespace std;

ClientToServerMessage::ClientToServerMessage(cts_message_type type, string command): type(type), command(command) {}

int ClientToServerMessage::getType() {
    return type;
}

RegisterMessage::RegisterMessage(const string &command)
    : ClientToServerMessage(REGISTER, command) {
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
    vector<char> opCode = Caster::shortToBytesVector(1);
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
    : ClientToServerMessage(LOGIN, command) {
    vector<string> result;
    if(result.size() != 4)
        throw parsing_exception();
    split(result, command, boost::is_any_of(" "));
    username = result[1];
    password = result[2];
    captcha = result[3] == "1";
}

vector<char> LoginMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector(2);
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
    : ClientToServerMessage(LOGOUT, command) {
    if(command != "LOGOUT")
        throw parsing_exception();
}

vector<char> LogoutMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector(3);
    encodedCommand.push_back(opCode[0]);
    encodedCommand.push_back(opCode[1]);
    return encodedCommand;
}

FollowOrUnfollowMessage::FollowOrUnfollowMessage(const string &command)
        : ClientToServerMessage(FOLLOW_OR_UNFOLLOW, command) {
    vector<string> result;
    split(result, command, boost::is_any_of(" "));
    if(result.size() != 3)
        throw parsing_exception();
    follow = result[1] == "0";
    otherUserName = result[2];
}

vector<char> FollowOrUnfollowMessage::encode() {
    vector<char> encodedCommand;
    vector<char> opCode = Caster::shortToBytesVector(4);
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
