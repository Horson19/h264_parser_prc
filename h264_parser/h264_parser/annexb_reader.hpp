//
//  annexb_reader.hpp
//  h264_parser
//
//  Created by haocheng on 2021/2/9.
//

#ifndef annexb_reader_hpp
#define annexb_reader_hpp

#include <iostream>

class AnnexbReader {
public:
    AnnexbReader(std::string &filePath);
    ~AnnexbReader();
    
    int open();
    
    int close();
    
    int ReadNALU(uint8_t *data, int *dataLen, int *startcodeLen);
    
private:
    int readFile();
    bool checkStartCode(int &startCodeLen, uint8_t *bufPtr, int bufLen);
    
    std::string mfilePath;
    FILE *f = nullptr;
    uint8_t *buffer = nullptr;
    int bufferLen = 0;
    int isEnd = 0;
};

#endif /* annexb_reader_hpp */
