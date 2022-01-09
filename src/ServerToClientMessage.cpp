//
// Created by ditto on 04-Jan-22.
//

#include "../include/ServerToClientMessage.h"
#include "../include/Caster.h"


ServerToClientMessage::ServerToClientMessage(stc_message_type type, vector<char> bytes)
        :bytes(bytes), notificationType(type){}

int ServerToClientMessage::getType() {
    return notificationType;
}


NotificationMessage::NotificationMessage(const vector<char> &bytes)
        : ServerToClientMessage(NOTIFICATION, bytes), n_type(), author(), content() {
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

NotificationMessage::~NotificationMessage()=default;

ErrorMessage::ErrorMessage(const vector<char> &bytes):
        ServerToClientMessage(ERROR, bytes), messageOP(){
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

ErrorMessage::~ErrorMessage()=default;

AckMessage::AckMessage(const vector<char> &bytes, int originalMessageType):
        ServerToClientMessage(ACK, bytes), messageOP(), content(), originalMessageType(originalMessageType)  {
    decode();
}

void AckMessage::decode() {
    vector<char> notificationBytes = {bytes[2], bytes[3]};
    messageOP = Caster::byteVectorToShort(notificationBytes);
    if(originalMessageType == LOGGED_IN_STATES || originalMessageType == STATISTICS) {
        for(size_t i = 4; i < bytes.size()-1; i+=8) {
            char ageBytes[] = {bytes[i], bytes[i+1]};
            short age = Caster::bytesToShort(ageBytes);
            char numOfPostsBytes[] = {bytes[i+2], bytes[i+3]};
            short numOfPosts = Caster::bytesToShort(numOfPostsBytes);
            char followersBytes[] = {bytes[i+4], bytes[i+5]};
            short followers = Caster::bytesToShort(followersBytes);
            char followingBytes[] = {bytes[i+6], bytes[i+7]};
            short following = Caster::bytesToShort(followingBytes);

            content += to_string(age);
            content += " ";
            content += to_string(numOfPosts);
            content += " ";
            content += to_string(followers);
            content += " ";
            content += to_string(following);
            content += " ";
        }
        content = content.substr(0,content.size()-1);
    } else {
        for(size_t i = 4; i < bytes.size()-1; i++)
            content += bytes[i];
    }

}

std::string AckMessage::toString() {
    string s = "ACK ";
    s += to_string(messageOP);
    if(!content.empty()) {
        s += " ";
        s += content;
    }

    return s;
}

AckMessage::~AckMessage()=default;
