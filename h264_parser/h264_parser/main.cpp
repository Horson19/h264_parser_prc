//
//  main.cpp
//  h264_parser
//
//  Created by haocheng on 2021/2/9.
//

#include <iostream>
#include "annexb_reader.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    int rc = 0;
    uint8_t *nalu_buf = NULL;
    int buf_size = 1024 * 1024;
    int start_code_len = 0;
    int data_len = 0;
    nalu_buf = (uint8_t *)malloc(buf_size);
    
    string path = "/Users/haocheng/Desktop/test.h264";
    AnnexbReader reader(path);
    rc = reader.open();
    
    if (rc < 0) {
        exit(1);
    }
    
    int i = 0;
    while ((rc = reader.ReadNALU(nalu_buf, &data_len, &start_code_len)) > 0) {
        int forbidden_zero_bit = (nalu_buf[start_code_len] >> 7) & 0x1;
        
        int nal_ref_idc = (nalu_buf[start_code_len] >> 5) & 0x3;
        int nal_unit_type = nalu_buf[start_code_len] & 0x1f;
        printf("forbidden_zero_bit: %d, nal_ref_idc: %d, nal_unit_type: %d\n", forbidden_zero_bit, nal_ref_idc, nal_unit_type);
        
        printf("number: %d\n", i++);
        printf("start code len is %d\n", start_code_len);
        printf("buf len is %d\n", data_len);
        printf("<------------------------------>\n");
        
    }
    
    free(nalu_buf);
    return 0;
}
