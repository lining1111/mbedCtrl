//
// Created by lining on 5/31/21.
//

#include <cstring>
#include "dev_chargingPile/AntsCalls.h"

using namespace antsCalls;

int main(int argc, char **argv) {
    CommonHead head;
    head.cmd = GetAllPortsStatus;

    uint8_t buf[64];
    bzero(buf, sizeof(buf));
    uint8_t len = 0;
    Pack(head, nullptr, 0, buf, &len);

    CommonHead head_get;
    CommonTail tail_get;
    uint8_t buf_in[21] = {0x66, 0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x02, 0x01, 0x01, 0x01, 0x01,
                          0x01, 0x01, 0x01, 0x01, 0x01, 0x1b};
    uint8_t buf_data[16];
    uint8_t buf_data_len = 0;
    Unpack(buf_in, sizeof(buf_in), head_get, buf_data, &buf_data_len, tail_get);

    return 0;
}