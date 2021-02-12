//
//  bit_stream.cpp
//  h264_parser
//
//  Created by haocheng on 2021/2/12.
//

#include "bit_stream.hpp"

BitStream::BitStream(uint8_t *buf, int size):
mBitLeft(8),
mP(buf),
mStart(buf),
mSize(size)
{
}

BitStream::~BitStream()
{
}

int BitStream::readU1() {
    int result = 0;
    
    if (mBitLeft <= 0) {
        mBitLeft = 8;
        mP++;
        if (mP - mStart > mSize) {
            return -1;
        }
    }
    
    result = *mP >> --mBitLeft;
    return result;
}

int BitStream::readU(int n) {
    int result = 0;
    
    for (int i = 0; i < n; i++)
        result |= readU1() << (n - i - 1);
    
    return result;
}

int BitStream::readUE() {
    int r = 0;
    int i = 0;
    while (readU1() == 0 && i < 32) {
        i++;
    }
    r = 1 << i;
    r |= readU(i) - 1;
    return r;
}

int BitStream::readSE() {
    int r = 0;
    int i = 0;
    int s = 0;
    while (readU1() == 0 && i < 32) {
        i++;
    }
    
    r = 1 << (i - 1);
    r |= readU(i - 1);
    s = readU1();
    
    r = s == 1 ? -r : r;
    return r;
}
