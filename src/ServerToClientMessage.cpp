//
// Created by ditto on 04-Jan-22.
//

#include "../include/ServerToClientMessage.h"
#include "Casters.cpp"


ServerToClientMessage::ServerToClientMessage(stc_message_type type, vector<char> bytes): type(type), bytes(bytes) {}


NotificationMessage::NotificationMessage(const vector<char> &bytes)
        : ServerToClientMessage(NOTIFICATION, bytes) {
    decode();
}

void NotificationMessage::decode() {
    char notificationBytes[] = {bytes[2]};
    short typeCode = bytesToShort(notificationBytes);
    n_type = (typeCode == 0) ? PM : POST;


}

std::string NotificationMessage::to
