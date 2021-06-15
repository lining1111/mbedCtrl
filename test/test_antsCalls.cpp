//
// Created by lining on 5/31/21.
//

#include <cstring>
#include <cstdio>
#include "dev_chargingPile/AntsCalls.h"

using namespace antsCalls;

int main(int argc, char **argv) {
//    CommonHead head;
//    head.cmd = GetAllPortsStatus;
//    uint8_t data[1] = {0x00};
//
//    uint8_t buf[64];
//    bzero(buf, sizeof(buf));
//    uint8_t len = 0;
//    Pack(head, data, sizeof(data), buf, &len);



    CommonHead head;
    head.cmd = 0xff;
    char *data = "865553059745549";

    uint8_t buf[64];
    bzero(buf, sizeof(buf));
    uint8_t len = 0;
    Pack(head, (uint8_t *) data, strlen(data) + 1, buf, &len);

    for (int i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");


    CommonHead head_get;
    CommonTail tail_get;
    uint8_t buf_in[21] = {0x66, 0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x02, 0x01, 0x01, 0x01, 0x01,
                          0x01, 0x01, 0x01, 0x01, 0x01, 0x1b};
    uint8_t buf_data[16];
    uint8_t buf_data_len = 0;
    Unpack(buf_in, sizeof(buf_in), head_get, buf_data, &buf_data_len, tail_get);

    return 0;
}