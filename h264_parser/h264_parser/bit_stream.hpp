//
//  bit_stream.hpp
//  h264_parser
//
//  Created by haocheng on 2021/2/12.
//

#ifndef bit_stream_hpp
#define bit_stream_hpp

#include <iostream>

class BitStream {
    
public:
    BitStream(uint8_t *buf, int size);
    ~BitStream();
    int readU1();
    int readU(int n);
    int readUE();
    int readSE();
    
private:
    uint8_t *mP;
    int     mSize;
    uint8_t *mStart;
    int     mBitLeft;
    
};

#endif /* bit_stream_hpp */
