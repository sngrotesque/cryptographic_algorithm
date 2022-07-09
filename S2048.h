#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef __SN_OBJECT__
#define __SN_OBJECT__ 1

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define BLOCK_SIZE        256  // 256 Bytes
#define PADDING_DATA      0x70 // 填充值 0xb3
#define NUMBER_OF_ROUNDS  7    // 加密解密轮数

#define BIN_R(x) (x ^ 0xFF)

struct S2048_ctx {
    u8 *data;
    u8 *token;
    u64 len;
};

// 初始混淆值 (可自定义)
const static u8 sbox[256] = {
    0xC0, 0xC0, 0x1F, 0xC0, 0x8B, 0xA8, 0x77, 0xB9,
    0x79, 0xD1, 0x3C, 0x52, 0xC9, 0xDB, 0x5D, 0x32,
    0xF2, 0x8C, 0xA1, 0x23, 0xDE, 0x3E, 0xE7, 0x68,
    0x68, 0xDA, 0xA2, 0x6D, 0x88, 0x63, 0x2A, 0x8C,
    0xE3, 0x9E, 0x5B, 0x9F, 0x5E, 0xA5, 0x79, 0x60,
    0x4C, 0x6E, 0xC7, 0x61, 0x23, 0xB6, 0x72, 0xE2,
    0x8D, 0x5D, 0x71, 0xDC, 0x49, 0x71, 0xD8, 0x9B,
    0x29, 0x95, 0x7F, 0xCD, 0x56, 0xB9, 0xD3, 0xA2,
    0xE5, 0xA3, 0x24, 0x13, 0xC2, 0x52, 0x1A, 0x6D,
    0x4D, 0x4E, 0x2B, 0x99, 0xAB, 0xEC, 0x69, 0x2D,
    0x93, 0xDB, 0xA6, 0x1E, 0x2E, 0x9E, 0x61, 0x5E,
    0xE5, 0x3B, 0x23, 0x3E, 0xD7, 0x90, 0xEA, 0xA7,
    0x35, 0x92, 0xAE, 0x53, 0xB0, 0xC8, 0x42, 0x31,
    0x3B, 0x6C, 0x1E, 0xCD, 0x6B, 0xBB, 0x8F, 0x6F,
    0xB8, 0x32, 0x39, 0x57, 0x7E, 0x66, 0xE3, 0x4A,
    0x32, 0xA5, 0xEE, 0x5D, 0xD5, 0x7F, 0x23, 0xC9,
    0x9E, 0xA8, 0x5E, 0xC0, 0x33, 0xEE, 0xEA, 0x71,
    0x4E, 0xA1, 0x26, 0x8A, 0x53, 0x76, 0xEC, 0x6B,
    0xB3, 0x5E, 0x4D, 0x44, 0x61, 0x8F, 0x68, 0x39,
    0xC1, 0x65, 0x46, 0x1B, 0xEE, 0xA6, 0x2A, 0x96,
    0x43, 0xE4, 0x6A, 0xAF, 0x5A, 0xA5, 0x18, 0x94,
    0x47, 0xCD, 0x13, 0x25, 0xAC, 0x20, 0xCB, 0xEB,
    0x76, 0x2A, 0x6B, 0xD5, 0x44, 0xB5, 0x1F, 0xE4,
    0x1E, 0xE4, 0x1D, 0x4D, 0x77, 0xA5, 0x7D, 0x35,
    0x10, 0x11, 0x26, 0xAA, 0x20, 0xC3, 0x4C, 0xE0,
    0x62, 0x81, 0x91, 0x8C, 0x73, 0x5F, 0x6A, 0x66,
    0x8D, 0x98, 0xC1, 0x7E, 0xA5, 0x38, 0xA3, 0x25,
    0x51, 0xC4, 0xEB, 0x18, 0x5D, 0xCB, 0x75, 0x58,
    0xC3, 0x7F, 0x79, 0x6C, 0x46, 0xA4, 0x8E, 0xDC,
    0x68, 0x10, 0xA5, 0xA9, 0x8B, 0x3A, 0x5C, 0xC6,
    0xEF, 0x8E, 0x51, 0x45, 0x31, 0xB3, 0x34, 0x43,
    0x94, 0xDE, 0xF0, 0x89, 0x29, 0xD4, 0xA9, 0xEB
};
#endif

#ifndef __SN_FUNCTION__
#define __SN_FUNCTION__ 1

static void S2048_Padding(struct S2048_ctx *data)
{
	u64 padoffset = BLOCK_SIZE - data->len % BLOCK_SIZE;
    u64 padding_n = data->len + padoffset;
    u8 *temp = (u8 *)malloc(padding_n + 1);
    memcpy(temp, data->data, data->len);
    temp[padding_n] = 0x00;
    for(u64 x = data->len; x < padding_n - 1; ++x) {
        temp[x] = PADDING_DATA;
    } data->data = temp;

    if(padoffset == 256) {
        data->data[padding_n - 2] = data->data[padding_n - 1] = 0xff;
    } else {
        data->data[padding_n - 1] = padoffset;
    }
    data->len = padding_n;
}

static void S2048_Key_Padding(struct S2048_ctx *data)
{
    size_t key_n = strlen((char *)data->token);
    u8 *temp = (u8 *)malloc(BLOCK_SIZE + 1);
    memcpy(temp, data->token, key_n);
    temp[BLOCK_SIZE] = 0x00;
    for(u32 x = key_n, index = 0; x < 256; ++x, ++index) {
        if(index == key_n) index = 0;
        temp[x] = data->token[index];
    } data->token = temp;
}

// 密钥生成函数
static u8 **Round_key_obfuscation(u8 *master_key)
{
    u8 **key_set = (u8 **)malloc(NUMBER_OF_ROUNDS * 8);
    u8 *key_temp = (u8 *)malloc(BLOCK_SIZE), temp;
    for(u32 rounds = 0; rounds < NUMBER_OF_ROUNDS; ++rounds) {
        key_set[rounds] = (u8 *)malloc(BLOCK_SIZE);
        memcpy(key_set[rounds], master_key, BLOCK_SIZE);
        for(u32 x = 0; x < BLOCK_SIZE; ++x) {
            if(x == 0) {
                temp = key_set[rounds][x] ^ key_set[rounds][255];
            } else {
                temp = key_set[rounds][x] ^ key_set[rounds][x-1];
            }
            temp = temp ^ key_set[rounds][174];
            temp = (x ^ temp - rounds) ^ sbox[x];
            key_temp[x] = BIN_R(temp);
        }
        master_key = key_temp;
    }
    return key_set;
}

// 加密函数
static int S2048_ENCRYPT(struct S2048_ctx *data)
{
    u8 **key = Round_key_obfuscation(data->token);
    u8 keyindex; u64 rounds, x;
    for(rounds = 0; rounds < NUMBER_OF_ROUNDS; ++rounds) {
        for(x = keyindex = 0; x < data->len; ++x, ++keyindex) {
            data->data[x] = BIN_R(data->data[x] ^ key[rounds][keyindex]);
        }
    }
    return 0;
}

// 解密函数
static int S2048_DECRYPT(struct S2048_ctx *data)
{
    return 0;
}
#endif


