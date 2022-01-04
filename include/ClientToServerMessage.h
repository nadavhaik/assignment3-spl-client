using namespace std;
#include <vector>

#ifndef ASSIGNMENT3_SPL_CLIENT_CLIENTTOSERVERMESSAGE_H
#define ASSIGNMENT3_SPL_CLIENT_CLIENTTOSERVERMESSAGE_H


enum cts_message_type {
    REGISTER = 1,
    LOGIN = 2,
    LOGOUT = 3,
    FOLLOW_OR_UNFOLLOW = 4,
    POST = 5,
    PRIVATE_MESSAGE = 6,
    LOGGED_IN_STATES = 7,
    STATISTICS = 8,
    BLOCK = 12,
    FETCH_NOTIFICATION = 13
};

class ClientToServerMessage{
public:
    ClientToServerMessage(cts_message_type type);
    virtual char* encode()=0;
};

#endif //ASSIGNMENT3_SPL_CLIENT_CLIENTTOSERVERMESSAGE_H
