#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdint.h>
#include "base64.h"
using namespace std;

const uint8_t key1[256] = {
    0x4b, 0x74, 0x4a, 0xad, 0xb7, 0xcb, 0x40, 0xe9, 0x5f, 0x50, 0xf0, 0x6f, 0xa8, 0x13, 0xc7, 0x2d, 0xb5, 0x32, 0xc1, 0x69, 0xbb, 0x54, 0x3f, 0xf5,
    0x35, 0x77, 0x3a, 0x7e, 0xd8, 0xca, 0x78, 0xb1, 0x8d, 0xa5, 0xfd, 0x83, 0x6d, 0x91, 0xc8, 0x48, 0x6b, 0xef, 0x2b, 0x5d, 0xaa, 0x10, 0xe4, 0xc4,
    0x38, 0x5e, 0x17, 0xa6, 0x5a, 0xf2, 0xc0, 0xab, 0x71, 0x87, 0xf6, 0xeb, 0x89, 0xfe, 0x0d, 0x04, 0x34, 0x41, 0x70, 0x7d, 0x43, 0x18, 0x29, 0xba,
    0xd3, 0x5c, 0xd0, 0xfb, 0x42, 0x49, 0x3b, 0x07, 0x25, 0x79, 0x7f, 0xdd, 0x15, 0x3c, 0x05, 0xee, 0x2e, 0x37, 0x14, 0xce, 0xe7, 0xa0, 0x8b, 0x66,
    0x11, 0x0a, 0x6e, 0xb2, 0xec, 0x95, 0x7c, 0x1b, 0x46, 0x28, 0x64, 0x09, 0x56, 0x88, 0x36, 0x3d, 0xe0, 0x82, 0xbc, 0xfa, 0x86, 0x93, 0xea, 0xcd,
    0xa7, 0x16, 0xf3, 0x76, 0xb3, 0xbf, 0x06, 0x55, 0x92, 0x31, 0xd7, 0xf8, 0x53, 0x75, 0x90, 0xe2, 0xed, 0x9e, 0x19, 0xc9, 0xd6, 0x39, 0xd4, 0xe8,
    0x52, 0x00, 0x0e, 0x21, 0x27, 0x3e, 0x1f, 0xcf, 0x30, 0xff, 0x63, 0x26, 0x02, 0xc6, 0x0f, 0x85, 0xb9, 0x1d, 0xd1, 0x4f, 0xb6, 0x58, 0x8f, 0xda,
    0x1c, 0x6a, 0x2a, 0x9f, 0x45, 0x59, 0xe1, 0x67, 0x2c, 0x97, 0x9a, 0xc5, 0x8a, 0x8e, 0x20, 0x4c, 0x2f, 0x9b, 0xd5, 0xcc, 0x0b, 0xf9, 0xf1, 0x6c,
    0x4d, 0xb0, 0x9c, 0x7b, 0xd2, 0xe6, 0x1a, 0x12, 0x44, 0x84, 0x8c, 0xbd, 0x99, 0x4e, 0x60, 0x72, 0xa3, 0x62, 0xa2, 0xdb, 0x73, 0xfc, 0x47, 0x51,
    0x68, 0xdf, 0xa1, 0x9d, 0x24, 0xb8, 0x0c, 0x33, 0x03, 0x1e, 0xbe, 0xb4, 0xde, 0x81, 0xa4, 0xc2, 0xf4, 0x57, 0xd9, 0x80, 0xae, 0xc3, 0x96, 0xaf,
    0xf7, 0xa9, 0x65, 0x7a, 0xe3, 0xdc, 0x23, 0x01, 0x5b, 0xe5, 0x22, 0x98, 0x08, 0x94, 0xac, 0x61
};

const uint8_t key2[256] = {
    0xdc, 0x6a, 0x99, 0x95, 0xe2, 0x9d, 0x64, 0x83, 0x53, 0xd4, 0x9d, 0x7d, 0xc1, 0x26, 0xb7, 0x9b, 0x60, 0x1d, 0xf2, 0x25, 0xcf, 0xfc, 0xba, 0xad,
    0x05, 0x0e, 0x3e, 0xab, 0x4d, 0x2a, 0x72, 0xab, 0xdb, 0x3b, 0x6e, 0xa5, 0x56, 0x9f, 0xd9, 0x6c, 0x1b, 0x11, 0x6e, 0xb3, 0x29, 0xbd, 0xb3, 0xd2,
    0x48, 0x86, 0xe2, 0x4d, 0x6d, 0x70, 0xff, 0x54, 0xb2, 0x9d, 0x28, 0xe1, 0x74, 0xf5, 0xb8, 0x8e, 0x29, 0x93, 0xfa, 0xf6, 0x37, 0x33, 0x59, 0x53,
    0xa7, 0x50, 0x64, 0x7e, 0x5e, 0xea, 0xd5, 0x88, 0xad, 0x22, 0x5e, 0x92, 0xcb, 0x7e, 0x40, 0x13, 0xd9, 0xf3, 0x03, 0xe5, 0x90, 0xf2, 0xa0, 0xa9,
    0x2f, 0x76, 0x57, 0xb8, 0x12, 0x0b, 0x52, 0x94, 0x5f, 0x2d, 0x74, 0xea, 0xb4, 0xff, 0x54, 0x35, 0x26, 0xb7, 0xd6, 0x01, 0x7c, 0xf9, 0x71, 0xe7,
    0xfd, 0x3a, 0xdf, 0xe6, 0xb4, 0xba, 0x71, 0xc6, 0xca, 0x3b, 0x11, 0x7c, 0xb4, 0x75, 0xe8, 0x67, 0x4f, 0x25, 0x3f, 0x81, 0x66, 0x27, 0x2f, 0x34,
    0x26, 0x89, 0x3b, 0x69, 0xd7, 0x43, 0xd2, 0xfd, 0x1d, 0x86, 0x6c, 0x81, 0x88, 0x04, 0xf6, 0xd1, 0x36, 0x82, 0xaf, 0x21, 0x4f, 0x47, 0x13, 0x78,
    0x76, 0x46, 0xb4, 0x3b, 0xbb, 0xd6, 0x89, 0x28, 0xc0, 0x51, 0xf3, 0x14, 0x49, 0xfc, 0x62, 0x15, 0xbb, 0x7f, 0x1e, 0xf6, 0xa4, 0xe6, 0xf3, 0xd8,
    0x37, 0x04, 0x02, 0x29, 0x9a, 0xa3, 0xe8, 0x47, 0xe2, 0xc7, 0x81, 0x4a, 0xab, 0x16, 0x51, 0x41, 0xb9, 0x7e, 0x8e, 0x96, 0x1b, 0x35, 0x36, 0x29,
    0x16, 0xb3, 0x1c, 0x1a, 0xcf, 0x3c, 0x3b, 0xe5, 0x24, 0x1e, 0xd6, 0x31, 0xf5, 0x59, 0x7b, 0x02, 0xaa, 0x48, 0xa2, 0x40, 0x32, 0x2c, 0x5c, 0xa4,
    0x90, 0xc6, 0xb5, 0xef, 0xac, 0x0d, 0xa6, 0x49, 0x3b, 0x92, 0x06, 0xb8, 0xee, 0x03, 0xa4, 0xce
};

uint32_t x, y, num, keyIndex, strIndex;

uint8_t *encrypt(const uint8_t *str, const uint32_t Len)
{
    uint8_t *res = new uint8_t[Len];
    keyIndex = 0;
    for (x = 0; x < Len; ++x) {
        if (keyIndex == 256) {
            keyIndex = 0;
        }
        res[x] = str[x] + 9 ^ key1[keyIndex];

        keyIndex++;
    }

    // return b64en(res, Len);
    return res;
}

uint32_t Plen(const uint8_t *str)
{
    const uint8_t end[5] = {"\xB7\x77\xF7\xC7"};
    uint32_t x, len = 0;
    for (x = 0;; ++x) {if (str[x] == end[0] && str[x + 1] == end[1] && str[x + 2] == end[2] && str[x + 3] == end[3]) {break;} len++; continue;}
    return len;
}

void PRINT_HEX(const uint8_t *str, uint32_t len)
{
    for(int x = 0; x < len; x++) {
        if (x == 0) {cout << "\t";}
        printf("%02x  ", str[x]);
        if ((x+1) % 32 == 0) {
            cout << "\n\t";
        }
    }
    cout << endl;
}

void PRINT_CHR(const uint8_t *str, uint32_t len)
{
    for(int x = 0; x < len; x++) {
        if (x == 0) {cout << "\t";}
        printf("%c", str[x]);
        if ((x+1) % 126 == 0) {
            cout << "\n\t";
        }
    }
    cout << endl;
}