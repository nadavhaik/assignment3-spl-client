#include <vector>
using namespace std;



short bytesToShort(char* bytesArr){
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}

void shortToBytes(short num, char* bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}
short byteVectorToShort(vector<char> vectorList) {
    char *a = new char[vectorList.size()];
    for(size_t i=0; i<vectorList.size(); i++) {
        a[i] = vectorList[i];
    }
    short res = bytesToShort(a);
    delete[] a;
    return res;
}

vector<char> shortToBytesList(short num) {
    vector<char> v;
    v.push_back(((num >> 8) & 0xFF));
    v.push_back((num & 0xFF));

    return v;
}