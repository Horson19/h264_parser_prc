//
//  annexb_reader.cpp
//  h264_parser
//
//  Created by haocheng on 2021/2/9.
//
#include <iostream>
#include "annexb_reader.hpp"

AnnexbReader::AnnexbReader(std::string &filePath):
mfilePath(filePath)
{
}

AnnexbReader::~AnnexbReader() {
    close();
}

bool AnnexbReader::checkStartCode(int &startCodeLen, uint8_t *bufPtr, int bufLen) {
    if (bufLen <= 2) {
        startCodeLen = 0;
        return false;
    }
    if (bufLen >= 4) {
        if (bufPtr[0] == 0 && bufPtr[1] == 0) {
            if (bufPtr[2] == 1) {
                startCodeLen = 3;
                return true;
            } else if (bufPtr[2] == 0 && bufPtr[3] == 1) {
                startCodeLen = 4;
                return true;
            }
        }
    }
    if (bufLen == 3) {
        if (bufPtr[0] == 0 && bufPtr[1] == 0 && bufPtr[2] == 1) {
            startCodeLen = 3;
            return true;
        }
    }
    
    return false;
}

int AnnexbReader::ReadNALU(uint8_t *data, int *dataLen, int *startcodeLen) {
    int read_len = 0;
    int has_start_code = 0;
    int tmp_startCode_len = 0;
    int end_pos = -1;
    int nalu_len = 0;
    
    while (1) {
        if (bufferLen <= 0) {
            read_len = readFile();
            if (read_len <= 0) {
                isEnd = 1;
                goto __end;
            }
        }
        
        if (!(has_start_code =
              checkStartCode(tmp_startCode_len,
                             buffer,
                             bufferLen))) {
            break;
        }
        
        *startcodeLen = tmp_startCode_len;
        
        //opt: kmp?
        for (int i = tmp_startCode_len; i < bufferLen; i++) {
            has_start_code = checkStartCode(tmp_startCode_len,
                                            buffer + i,
                                            bufferLen - i);
            if (has_start_code) {
                end_pos = i;
                break;
            }
        }
        
        if (end_pos > 0) {
            
            nalu_len = end_pos;
            
            memcpy(data, buffer, end_pos);
            uint8_t *tmp_buf = (uint8_t *)malloc(bufferLen - end_pos);
            memcpy(tmp_buf, buffer + end_pos, bufferLen - end_pos);
            
            if (buffer != nullptr) {
                free(buffer);
                buffer = nullptr;
            }
            
            buffer = tmp_buf;
            bufferLen = bufferLen - end_pos;
            
            *dataLen = nalu_len;
            return 1;
            
        } else {
__end:
            if (isEnd) {
                nalu_len = bufferLen;
                memcpy(data, buffer, bufferLen);
                if (buffer != nullptr) {
                    free(buffer);
                    buffer = nullptr;
                }
                buffer = nullptr;
                *dataLen = nalu_len;
                return 0;
            }
            read_len = readFile();
            if (read_len <= 0) {
                isEnd = true;
            }
        }
    }
    
    return -1;
}

int AnnexbReader::open() {
    f = fopen(mfilePath.c_str(), "rb");
    if (f == nullptr) {
        return -1;
    }
    return 0;
}

int AnnexbReader::close() {
    if (f != nullptr) {
        fclose(f);
        f = nullptr;
    }
    
    if (buffer != nullptr) {
        free(buffer);
        buffer = nullptr;
    }
    
    return 0;
}

int AnnexbReader::readFile() {
    int read_len = 1024;
    
    uint8_t *tmp_buf = (uint8_t *)malloc(read_len);
    int len = (int)fread(tmp_buf, 1, read_len, f);
    
    if (len > 0) {
        uint8_t *target_buf = (uint8_t *)malloc(bufferLen + len);
        //move data
        memcpy(target_buf, buffer, bufferLen);
        memcpy(target_buf + bufferLen, tmp_buf, len);
        
        bufferLen += len;
        
        if (buffer != nullptr) {
            free(buffer);
            buffer = nullptr;
        }
        
        buffer = target_buf;
    }
    
    free(tmp_buf);
    return len;
}
