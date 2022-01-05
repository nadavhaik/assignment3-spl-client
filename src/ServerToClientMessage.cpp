//
// Created by ditto on 04-Jan-22.
//

#include "../include/ServerToClientMessage.h"
#include "../include/Caster.h"


ServerToClientMessage::ServerToClientMessage(stc_message_type type, vector<char> bytes): notificationType(type), bytes(bytes) {}

int ServerToClientMessage::getType() {
    return notificationType;
}


NotificationMessage::NotificationMessage(const vector<char> &bytes)
        : ServerToClientMessage(NOTIFICATION, bytes) {
    decode();
}

void NotificationMessage::decode() {
    char notificationBytes[] = {bytes[2]};
    short typeCode = Caster::bytesToShort(notificationBytes);
    n_type = (typeCode == 0) ? PM_NOTIFICATION : POST_NOTIFICATION;
    size_t i = 3;
    char byte = bytes[i];
    string s;
    while(byte != '\0') {
        s+=byte;
        i++;
        byte = bytes[i];
    }
    author = s;

    i++;
    byte = bytes[i];
    s = "";
    while(byte != '\0') {
        s+=byte;
        i++;
        byte = bytes[i];
    }
    content = s;
}

std::string NotificationMessage::toString() {
    string s;
    s += "NOTIFICATION ";
    if(n_type == PM_NOTIFICATION)
        s += "PM ";
    else
        s += "Public ";
    s += author;
    s += " ";
    s += content;

    return s;

}

ErrorMessage::ErrorMessage(const vector<char> &bytes):
        ServerToClientMessage(ERROR, bytes){
    decode();
}

void ErrorMessage::decode() {
    vector<char> notificationBytes = {bytes[2], bytes[3]};
    messageOP = Caster::byteVectorToShort(notificationBytes);
}

std::string ErrorMessage::toString() {
    string s = "ERROR ";
    s += to_string(messageOP);
    return s;
}

AckMessage::AckMessage(const vector<char> &bytes, int originalMessageType)
        :ServerToClientMessage(ACK, bytes){
    decode();
}

void AckMessage::decode() {
    vector<char> notificationBytes = {bytes[2], bytes[3]};
    messageOP = Caster::byteVectorToShort(notificationBytes);
    for(size_t i = 4; i < bytes.size()-1; i++)
        content += bytes[i];
}

std::string AckMessage::toString() {
    string s = "ACK ";
    s += to_string(messageOP);
    s += " ";
    s += content;

    return s;
}
