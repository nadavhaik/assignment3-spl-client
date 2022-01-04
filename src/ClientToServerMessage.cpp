//
// Created by niv19 on 04/01/2022.
//
#include "../include/ClientToServerMessage.h"
#include "Casters.cpp"

using namespace std;

ClientToServerMessage::ClientToServerMessage(cts_message_type type, string command): type(type), command(command) {}

RegisterMessage::RegisterMessage(const string &command)
    : ClientToServerMessage(REGISTER, command) {
    encode();
}

vector<char> RegisterMessage::encode() {
    vector<string> result;
    split(result, str, boost::is_any_of(" "));
    short op = opcode(result[0]);
    vector<char> encodedCommand = new vector<char>;

    return vector<char>();
}

