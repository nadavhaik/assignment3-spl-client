//
// Created by ditto on 05-Jan-22.
//
#include <vector>
using namespace std;

#ifndef ASSIGNMENT3_SPL_CLIENT_CASTER_H
#define ASSIGNMENT3_SPL_CLIENT_CASTER_H
class Caster {
public:
    static short bytesToShort(char* bytesArr);
    static short byteVectorToShort(vector<char> vectorList);
    static vector<char> shortToBytesVector(short num);
};
#endif //ASSIGNMENT3_SPL_CLIENT_CASTER_H
