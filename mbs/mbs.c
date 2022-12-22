#include "mbs.h"

static const uint8_t sbox[256] = {
    0xb4, 0x1b, 0xd5, 0x2d, 0xe2, 0x44, 0x96, 0xd0, 0x65, 0x66, 0x9c, 0x4e, 0xed, 0x73, 0xfa, 0xac,
    0x54, 0x62, 0xce, 0x39, 0x40, 0xcb, 0xfc, 0xcc, 0x25, 0xc6, 0xd1, 0x86, 0xbb, 0x82, 0xbf, 0x05,
    0x0b, 0x9b, 0x3a, 0xf1, 0xb6, 0xd8, 0x6b, 0xdc, 0xa0, 0x4b, 0x7a, 0x6c, 0x8a, 0xb9, 0xd2, 0xfd,
    0x49, 0xbc, 0xf2, 0xb0, 0x03, 0x5b, 0x1a, 0x4a, 0x8b, 0x47, 0xf3, 0xfe, 0xaf, 0xab, 0x2b, 0x68,
    0x6f, 0x41, 0x45, 0x6d, 0xb1, 0x2f, 0xc9, 0x84, 0xa7, 0x3c, 0x56, 0x0a, 0xc1, 0x17, 0x3f, 0x99,
    0x2a, 0x5a, 0x3b, 0xc0, 0x38, 0x23, 0xe6, 0x21, 0x34, 0x31, 0x4f, 0xad, 0x72, 0xee, 0x4c, 0xb3,
    0xcd, 0x04, 0x42, 0x8f, 0x7c, 0x7e, 0x70, 0x48, 0xbe, 0xe1, 0xd9, 0xf4, 0x30, 0xd7, 0xe0, 0xd3,
    0xa6, 0x29, 0x5f, 0x5e, 0x83, 0xf7, 0x60, 0xa5, 0x3e, 0x79, 0x91, 0x10, 0x0d, 0xe8, 0xba, 0x81,
    0xa8, 0x20, 0x85, 0x90, 0xd6, 0x87, 0xb2, 0xdb, 0xe9, 0x26, 0x35, 0x58, 0x12, 0x57, 0x5c, 0xc4,
    0xbd, 0xef, 0xc3, 0x2e, 0x69, 0xa2, 0x1e, 0xd4, 0x11, 0x94, 0x0f, 0x3d, 0x19, 0x27, 0x78, 0x08,
    0x4d, 0x6e, 0x00, 0x13, 0xcf, 0x71, 0x67, 0x53, 0xda, 0x16, 0x8e, 0x88, 0x43, 0x76, 0x46, 0x9f,
    0x24, 0xb8, 0xdf, 0x64, 0xfb, 0x89, 0xf9, 0x9a, 0x7b, 0x9d, 0x9e, 0x98, 0xc2, 0x93, 0xde, 0xec,
    0xf0, 0xca, 0xc7, 0x50, 0x97, 0xae, 0x07, 0x32, 0xb7, 0x61, 0x95, 0xe4, 0x55, 0xe3, 0xa3, 0x75,
    0x09, 0xa9, 0x7d, 0xc5, 0x51, 0xf8, 0x8d, 0x52, 0xf5, 0xa1, 0xa4, 0xff, 0x8c, 0x28, 0x14, 0x7f,
    0xe5, 0x6a, 0x0c, 0x37, 0x92, 0x22, 0xaa, 0x0e, 0xf6, 0xea, 0x33, 0xeb, 0xe7, 0x1f, 0x15, 0x18,
    0x59, 0x06, 0x02, 0x1c, 0x36, 0x77, 0x74, 0x2c, 0x63, 0xc8, 0x1d, 0xb5, 0x01, 0x5d, 0x80, 0xdd
};

static const uint8_t rsbox[256] = {
    0xa2, 0xfc, 0xf2, 0x34, 0x61, 0x1f, 0xf1, 0xc6, 0x9f, 0xd0, 0x4b, 0x20, 0xe2, 0x7c, 0xe7, 0x9a,
    0x7b, 0x98, 0x8c, 0xa3, 0xde, 0xee, 0xa9, 0x4d, 0xef, 0x9c, 0x36, 0x01, 0xf3, 0xfa, 0x96, 0xed,
    0x81, 0x57, 0xe5, 0x55, 0xb0, 0x18, 0x89, 0x9d, 0xdd, 0x71, 0x50, 0x3e, 0xf7, 0x03, 0x93, 0x45,
    0x6c, 0x59, 0xc7, 0xea, 0x58, 0x8a, 0xf4, 0xe3, 0x54, 0x13, 0x22, 0x52, 0x49, 0x9b, 0x78, 0x4e,
    0x14, 0x41, 0x62, 0xac, 0x05, 0x42, 0xae, 0x39, 0x67, 0x30, 0x37, 0x29, 0x5e, 0xa0, 0x0b, 0x5a,
    0xc3, 0xd4, 0xd7, 0xa7, 0x10, 0xcc, 0x4a, 0x8d, 0x8b, 0xf0, 0x51, 0x35, 0x8e, 0xfd, 0x73, 0x72,
    0x76, 0xc9, 0x11, 0xf8, 0xb3, 0x08, 0x09, 0xa6, 0x3f, 0x94, 0xe1, 0x26, 0x2b, 0x43, 0xa1, 0x40,
    0x66, 0xa5, 0x5c, 0x0d, 0xf6, 0xcf, 0xad, 0xf5, 0x9e, 0x79, 0x2a, 0xb8, 0x64, 0xd2, 0x65, 0xdf,
    0xfe, 0x7f, 0x1d, 0x74, 0x47, 0x82, 0x1b, 0x85, 0xab, 0xb5, 0x2c, 0x38, 0xdc, 0xd6, 0xaa, 0x63,
    0x83, 0x7a, 0xe4, 0xbd, 0x99, 0xca, 0x06, 0xc4, 0xbb, 0x4f, 0xb7, 0x21, 0x0a, 0xb9, 0xba, 0xaf,
    0x28, 0xd9, 0x95, 0xce, 0xda, 0x77, 0x70, 0x48, 0x80, 0xd1, 0xe6, 0x3d, 0x0f, 0x5b, 0xc5, 0x3c,
    0x33, 0x44, 0x86, 0x5f, 0x00, 0xfb, 0x24, 0xc8, 0xb1, 0x2d, 0x7e, 0x1c, 0x31, 0x90, 0x68, 0x1e,
    0x53, 0x4c, 0xbc, 0x92, 0x8f, 0xd3, 0x19, 0xc2, 0xf9, 0x46, 0xc1, 0x15, 0x17, 0x60, 0x12, 0xa4,
    0x07, 0x1a, 0x2e, 0x6f, 0x97, 0x02, 0x84, 0x6d, 0x25, 0x6a, 0xa8, 0x87, 0x27, 0xff, 0xbe, 0xb2,
    0x6e, 0x69, 0x04, 0xcd, 0xcb, 0xe0, 0x56, 0xec, 0x7d, 0x88, 0xe9, 0xeb, 0xbf, 0x0c, 0x5d, 0x91,
    0xc0, 0x23, 0x32, 0x3a, 0x6b, 0xd8, 0xe8, 0x75, 0xd5, 0xb6, 0x0e, 0xb4, 0x16, 0x2f, 0x3b, 0xdb
};

#define MBS_GET_SBOX(n)  (sbox[(n)])
#define MBS_GET_RSBOX(n) (rsbox[(n)])

void mbs_init_ctx(mbs_ctx *ctx)
{
    uint8_t round, index;
    for(round = 0; round < MBS_ROUNDS; ++round) {
        memcpy(ctx->keySet[round], ctx->key, MBS_BLOCKSIZE);
        for(index = 0; index < MBS_BLOCKSIZE; ++index) {
            ctx->key[index] = MBS_SUB(
                ctx->key[index],
                ctx->key[MBS_KEY_RANGE(index)],
                MBS_GET_SBOX(index),
                MBS_GET_SBOX(MBS_BOX_RANGE(index)),
                (((index + round) << 1) ^ 0xff));
        }
    }
}

void mbs_encrypt(mbs_ctx *ctx, uint8_t *buf, size_t size)
{
    uint8_t round;
    size_t index;
    for(round = 0; round < MBS_ROUNDS; ++round) {
        for(index = 0; index < size; ++index) {
            buf[index] = MBS_E(buf[index],
                ctx->keySet[round][index % MBS_BLOCKSIZE],
                MBS_GET_SBOX(ctx->keySet[round][index % MBS_BLOCKSIZE]));
        }
    }
}

void mbs_decrypt(mbs_ctx *ctx, uint8_t *buf, size_t size)
{
    uint8_t round;
    size_t index;
    for(round = 0; round < MBS_ROUNDS; ++round) {
        for(index = 0; index < size; ++index) {
            buf[index] = MBS_D(buf[index],
                ctx->keySet[MBS_ROUNDS - round-1][index % MBS_BLOCKSIZE],
                MBS_GET_SBOX(ctx->keySet[MBS_ROUNDS - round-1][index % MBS_BLOCKSIZE]));
        }
    }
}
