#include "../include/SessionData.h"

#include <string>
#include <vector>
using namespace std;
char *toBytesMessage(vector<char> v) {
    char *c = new char[v.size()+1];
    for(size_t i = 0; i<v.size(); i++)
        c[i] = v[i];
    c[v.size()] = ';';
    return c;
}

SessionData::SessionData(string ip, short port): ch(ip, port) {}

void SessionData::run() {

}

ServerToClientMessage *SessionData::communicate(const ClientToServerMessage &message) {
    char *bytes = toBytesMessage(message.encode());

}


