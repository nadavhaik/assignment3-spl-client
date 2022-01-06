#include "../include/SessionData.h"
#include "../include/ServerToClientMessage.h"
#include <string>
#include <vector>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "../include/Caster.h"

using namespace std;
const unsigned int FETCHING_INTERVAL = 500; // Milliseconds

char *toBytesMessage(const vector<char> &v) {
    char *c = new char[v.size()+1];
    for(size_t i=0; i<v.size(); i++)
        c[i] = v[i];
    c[v.size()] = ';';
    return c;
}

void fetchNotifications(SessionData *sd) {
    while(!sd->getShouldStop()) {
        if(sd->isLoggedIn()) {
            FetchNotificationMessage *message = new FetchNotificationMessage();
            ServerToClientMessage *response = sd->communicate(*message);
            if(response->getType() == 9) {
                cout << response->toString() << endl;
            }
            delete message;
            delete response;
        }
        this_thread::sleep_for(chrono::milliseconds(FETCHING_INTERVAL));
    }
}

SessionData::SessionData(string ip, short port): ch(ip, port) {
    if(!ch.connect())
        throw connection_exception();
}
SessionData::~SessionData() {
    ch.close();
}
bool SessionData::isLoggedIn() {
    return loggedIn;
}
bool SessionData::getShouldStop() {
    return shouldStop;
}


void SessionData::run() {
    std::thread t(fetchNotifications, this);
    t.detach();
    while(!shouldStop) {
        string input_command;
        getline(cin, input_command);
        ClientToServerMessage *message = nullptr;
        try {
            if (input_command.rfind("REGISTER ", 0) == 0)
                message = new RegisterMessage(input_command);
            else if (input_command.rfind("LOGIN ", 0) == 0)
                message = new LoginMessage(input_command);
            else if (input_command == "LOGOUT")
                message = new LogoutMessage(input_command);
            else if (input_command.rfind("FOLLOW ", 0) == 0)
                message = new FollowOrUnfollowMessage(input_command);
            else if (input_command.rfind("POST ", 0) == 0)
                message = new PostMessage(input_command);
            else if (input_command.rfind("PM ", 0) == 0)
                message = new PMMessage(input_command);
            else if (input_command == "LOGSTAT")
                message = new LoggedInStates(input_command);
            else if (input_command.rfind("STAT ", 0) == 0)
                message = new StatisticsMessage(input_command);
            else if (input_command.rfind("BLOCK ", 0) == 0)
                message = new BlockMessage(input_command);
            else
                throw parsing_exception();
        } catch (parsing_exception &e) {
            cout << "Illegal command" << endl;
            continue;
        }


        ServerToClientMessage *response = communicate(*message);
        cout << response->toString() << endl;
        if(message->getType() == 2 && response->getType() == 10) // LOGIN + ACK
            loggedIn = true;
        if(message->getType() == 3 && response->getType() == 10) { // LOGOUT + ACK
            loggedIn = false;
            shouldStop = true;
        }
        delete message;
        delete response;
    }
    if(t.joinable()) // else it died before
        t.join();

}

ServerToClientMessage *SessionData::communicate(ClientToServerMessage &message) {
    connectionLock.lock();
    vector<char> bytesVec = message.encode();
    char *bytes = toBytesMessage(bytesVec);
    while(!ch.sendBytes(bytes, (int)bytesVec.size()+1));
    string response;
    while(!ch.getFrameAscii(response, ';'));
    connectionLock.unlock();

    vector<char> responseBytes;
    for(char c : response)
        responseBytes.push_back(c);

    ServerToClientMessage *responseMessage = nullptr;
    char opCodeBytes[] = {responseBytes[0], responseBytes[1]};
    short opCode = Caster::bytesToShort(opCodeBytes);

    switch(opCode) {
        case 9:
            responseMessage = new NotificationMessage(responseBytes);
            break;
        case 10:
            responseMessage = new AckMessage(responseBytes, message.getType());
            break;
        case 11:
            responseMessage = new ErrorMessage(responseBytes);
            break;
    }

    delete bytes;
    return responseMessage;

}



