#include "../snCrypto.h"
#include "szq.h"

/*
* 有关sbox与rsbox
*     sbox与rsbox均可自定义。
* 但由于通信双方需处于同一算法数据的情况下，此为默认sbox与rsbox
* -------------------------------------------- sbox --------------------------------------------
* 0xf3, 0x54, 0xca, 0x34, 0x4f, 0xef, 0x55, 0x31, 0x0e, 0x50, 0xe4, 0x61, 0x17, 0xb2, 0x14, 0x94,
* 0xd2, 0x3d, 0x3f, 0x80, 0x84, 0x27, 0xd9, 0x1d, 0x4c, 0x12, 0x5c, 0x1e, 0x0f, 0x7e, 0x4a, 0x58,
* 0xd0, 0xe5, 0x3b, 0x03, 0x90, 0x62, 0x0c, 0xbf, 0xdc, 0xb1, 0xbe, 0x57, 0x21, 0xf2, 0xfc, 0x82,
* 0xbb, 0xf1, 0x6b, 0xda, 0xe7, 0xc4, 0x47, 0x39, 0xff, 0xa0, 0xd5, 0x66, 0xec, 0xb4, 0x48, 0x44,
* 0xc7, 0x08, 0x69, 0x13, 0x85, 0x25, 0x43, 0xe0, 0x7a, 0x4d, 0x67, 0xee, 0x1b, 0x41, 0x36, 0x30,
* 0xfe, 0xfb, 0xa2, 0x92, 0xdd, 0xfd, 0x95, 0x3e, 0x0d, 0x71, 0x9f, 0x53, 0x93, 0xc3, 0x42, 0x4b,
* 0x6a, 0xc8, 0xa5, 0x4e, 0xb3, 0x89, 0x29, 0x2b, 0x0a, 0x18, 0xc6, 0x00, 0x64, 0x26, 0x3a, 0x2d,
* 0x6c, 0xc5, 0x32, 0xd4, 0xd1, 0x88, 0x20, 0x7b, 0x87, 0x5e, 0xa6, 0x5f, 0x86, 0x5a, 0xb7, 0xde,
* 0xb5, 0x0b, 0xaa, 0xab, 0xa3, 0xa4, 0xce, 0x77, 0x9e, 0x8f, 0x1f, 0x19, 0x02, 0xf9, 0xed, 0xcd,
* 0x09, 0x91, 0xeb, 0x40, 0x46, 0x22, 0x1a, 0xe8, 0xc2, 0x70, 0x78, 0x28, 0x63, 0x79, 0x97, 0x83,
* 0x75, 0x2f, 0xf4, 0x15, 0xf0, 0x2e, 0x38, 0x7d, 0x5d, 0xd8, 0x23, 0x2a, 0x07, 0xbd, 0x8a, 0xf5,
* 0x9b, 0x11, 0x6d, 0x99, 0x6f, 0x98, 0xdb, 0xf6, 0x7c, 0xaf, 0x35, 0xe9, 0x60, 0x10, 0xe1, 0x7f,
* 0xa1, 0x9a, 0x24, 0x6e, 0x72, 0xb0, 0x9d, 0xe2, 0xae, 0xc0, 0x45, 0x49, 0x06, 0x52, 0x51, 0xd6,
* 0xac, 0xbc, 0xa8, 0xcc, 0xf7, 0x96, 0xa7, 0x05, 0x9c, 0xdf, 0xb8, 0xcf, 0xfa, 0x2c, 0x65, 0x76,
* 0x74, 0xe3, 0x59, 0xd3, 0xd7, 0x3c, 0x73, 0xf8, 0x8c, 0x5b, 0xb9, 0xad, 0xba, 0x01, 0x37, 0xb6,
* 0x56, 0xc9, 0xa9, 0x81, 0x1c, 0x68, 0xea, 0xc1, 0x8d, 0x8e, 0xcb, 0x04, 0x16, 0x8b, 0x33, 0xe6
*
* ------------------------------------------- rsbox --------------------------------------------
* 0x6b, 0xed, 0x8c, 0x23, 0xfb, 0xd7, 0xcc, 0xac, 0x41, 0x90, 0x68, 0x81, 0x26, 0x58, 0x08, 0x1c,
* 0xbd, 0xb1, 0x19, 0x43, 0x0e, 0xa3, 0xfc, 0x0c, 0x69, 0x8b, 0x96, 0x4c, 0xf4, 0x17, 0x1b, 0x8a,
* 0x76, 0x2c, 0x95, 0xaa, 0xc2, 0x45, 0x6d, 0x15, 0x9b, 0x66, 0xab, 0x67, 0xdd, 0x6f, 0xa5, 0xa1,
* 0x4f, 0x07, 0x72, 0xfe, 0x03, 0xba, 0x4e, 0xee, 0xa6, 0x37, 0x6e, 0x22, 0xe5, 0x11, 0x57, 0x12,
* 0x93, 0x4d, 0x5e, 0x46, 0x3f, 0xca, 0x94, 0x36, 0x3e, 0xcb, 0x1e, 0x5f, 0x18, 0x49, 0x63, 0x04,
* 0x09, 0xce, 0xcd, 0x5b, 0x01, 0x06, 0xf0, 0x2b, 0x1f, 0xe2, 0x7d, 0xe9, 0x1a, 0xa8, 0x79, 0x7b,
* 0xbc, 0x0b, 0x25, 0x9c, 0x6c, 0xde, 0x3b, 0x4a, 0xf5, 0x42, 0x60, 0x32, 0x70, 0xb2, 0xc3, 0xb4,
* 0x99, 0x59, 0xc4, 0xe6, 0xe0, 0xa0, 0xdf, 0x87, 0x9a, 0x9d, 0x48, 0x77, 0xb8, 0xa7, 0x1d, 0xbf,
* 0x13, 0xf3, 0x2f, 0x9f, 0x14, 0x44, 0x7c, 0x78, 0x75, 0x65, 0xae, 0xfd, 0xe8, 0xf8, 0xf9, 0x89,
* 0x24, 0x91, 0x53, 0x5c, 0x0f, 0x56, 0xd5, 0x9e, 0xb5, 0xb3, 0xc1, 0xb0, 0xd8, 0xc6, 0x88, 0x5a,
* 0x39, 0xc0, 0x52, 0x84, 0x85, 0x62, 0x7a, 0xd6, 0xd2, 0xf2, 0x82, 0x83, 0xd0, 0xeb, 0xc8, 0xb9,
* 0xc5, 0x29, 0x0d, 0x64, 0x3d, 0x80, 0xef, 0x7e, 0xda, 0xea, 0xec, 0x30, 0xd1, 0xad, 0x2a, 0x27,
* 0xc9, 0xf7, 0x98, 0x5d, 0x35, 0x71, 0x6a, 0x40, 0x61, 0xf1, 0x02, 0xfa, 0xd3, 0x8f, 0x86, 0xdb,
* 0x20, 0x74, 0x10, 0xe3, 0x73, 0x3a, 0xcf, 0xe4, 0xa9, 0x16, 0x33, 0xb6, 0x28, 0x54, 0x7f, 0xd9,
* 0x47, 0xbe, 0xc7, 0xe1, 0x0a, 0x21, 0xff, 0x34, 0x97, 0xbb, 0xf6, 0x92, 0x3c, 0x8e, 0x4b, 0x05,
* 0xa4, 0x31, 0x2d, 0x00, 0xa2, 0xaf, 0xb7, 0xd4, 0xe7, 0x8d, 0xdc, 0x51, 0x2e, 0x55, 0x50, 0x38
*/

// 此sbox与rsbox由"sbox_init"与"rsbox_init"两个函数生成
#ifndef __SZQ_SBOX__ // szq sbox
static const uint8_t sbox[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0xf3, 0x54, 0xca, 0x34, 0x4f, 0xef, 0x55, 0x31, 0x0e, 0x50, 0xe4, 0x61, 0x17, 0xb2, 0x14, 0x94,
    0xd2, 0x3d, 0x3f, 0x80, 0x84, 0x27, 0xd9, 0x1d, 0x4c, 0x12, 0x5c, 0x1e, 0x0f, 0x7e, 0x4a, 0x58,
    0xd0, 0xe5, 0x3b, 0x03, 0x90, 0x62, 0x0c, 0xbf, 0xdc, 0xb1, 0xbe, 0x57, 0x21, 0xf2, 0xfc, 0x82,
    0xbb, 0xf1, 0x6b, 0xda, 0xe7, 0xc4, 0x47, 0x39, 0xff, 0xa0, 0xd5, 0x66, 0xec, 0xb4, 0x48, 0x44,
    0xc7, 0x08, 0x69, 0x13, 0x85, 0x25, 0x43, 0xe0, 0x7a, 0x4d, 0x67, 0xee, 0x1b, 0x41, 0x36, 0x30,
    0xfe, 0xfb, 0xa2, 0x92, 0xdd, 0xfd, 0x95, 0x3e, 0x0d, 0x71, 0x9f, 0x53, 0x93, 0xc3, 0x42, 0x4b,
    0x6a, 0xc8, 0xa5, 0x4e, 0xb3, 0x89, 0x29, 0x2b, 0x0a, 0x18, 0xc6, 0x00, 0x64, 0x26, 0x3a, 0x2d,
    0x6c, 0xc5, 0x32, 0xd4, 0xd1, 0x88, 0x20, 0x7b, 0x87, 0x5e, 0xa6, 0x5f, 0x86, 0x5a, 0xb7, 0xde,
    0xb5, 0x0b, 0xaa, 0xab, 0xa3, 0xa4, 0xce, 0x77, 0x9e, 0x8f, 0x1f, 0x19, 0x02, 0xf9, 0xed, 0xcd,
    0x09, 0x91, 0xeb, 0x40, 0x46, 0x22, 0x1a, 0xe8, 0xc2, 0x70, 0x78, 0x28, 0x63, 0x79, 0x97, 0x83,
    0x75, 0x2f, 0xf4, 0x15, 0xf0, 0x2e, 0x38, 0x7d, 0x5d, 0xd8, 0x23, 0x2a, 0x07, 0xbd, 0x8a, 0xf5,
    0x9b, 0x11, 0x6d, 0x99, 0x6f, 0x98, 0xdb, 0xf6, 0x7c, 0xaf, 0x35, 0xe9, 0x60, 0x10, 0xe1, 0x7f,
    0xa1, 0x9a, 0x24, 0x6e, 0x72, 0xb0, 0x9d, 0xe2, 0xae, 0xc0, 0x45, 0x49, 0x06, 0x52, 0x51, 0xd6,
    0xac, 0xbc, 0xa8, 0xcc, 0xf7, 0x96, 0xa7, 0x05, 0x9c, 0xdf, 0xb8, 0xcf, 0xfa, 0x2c, 0x65, 0x76,
    0x74, 0xe3, 0x59, 0xd3, 0xd7, 0x3c, 0x73, 0xf8, 0x8c, 0x5b, 0xb9, 0xad, 0xba, 0x01, 0x37, 0xb6,
    0x56, 0xc9, 0xa9, 0x81, 0x1c, 0x68, 0xea, 0xc1, 0x8d, 0x8e, 0xcb, 0x04, 0x16, 0x8b, 0x33, 0xe6
};
#endif // szq sbox

#ifndef __SZQ_RSBOX__ // szq rsbox
static const uint8_t rsbox[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x6b, 0xed, 0x8c, 0x23, 0xfb, 0xd7, 0xcc, 0xac, 0x41, 0x90, 0x68, 0x81, 0x26, 0x58, 0x08, 0x1c,
    0xbd, 0xb1, 0x19, 0x43, 0x0e, 0xa3, 0xfc, 0x0c, 0x69, 0x8b, 0x96, 0x4c, 0xf4, 0x17, 0x1b, 0x8a,
    0x76, 0x2c, 0x95, 0xaa, 0xc2, 0x45, 0x6d, 0x15, 0x9b, 0x66, 0xab, 0x67, 0xdd, 0x6f, 0xa5, 0xa1,
    0x4f, 0x07, 0x72, 0xfe, 0x03, 0xba, 0x4e, 0xee, 0xa6, 0x37, 0x6e, 0x22, 0xe5, 0x11, 0x57, 0x12,
    0x93, 0x4d, 0x5e, 0x46, 0x3f, 0xca, 0x94, 0x36, 0x3e, 0xcb, 0x1e, 0x5f, 0x18, 0x49, 0x63, 0x04,
    0x09, 0xce, 0xcd, 0x5b, 0x01, 0x06, 0xf0, 0x2b, 0x1f, 0xe2, 0x7d, 0xe9, 0x1a, 0xa8, 0x79, 0x7b,
    0xbc, 0x0b, 0x25, 0x9c, 0x6c, 0xde, 0x3b, 0x4a, 0xf5, 0x42, 0x60, 0x32, 0x70, 0xb2, 0xc3, 0xb4,
    0x99, 0x59, 0xc4, 0xe6, 0xe0, 0xa0, 0xdf, 0x87, 0x9a, 0x9d, 0x48, 0x77, 0xb8, 0xa7, 0x1d, 0xbf,
    0x13, 0xf3, 0x2f, 0x9f, 0x14, 0x44, 0x7c, 0x78, 0x75, 0x65, 0xae, 0xfd, 0xe8, 0xf8, 0xf9, 0x89,
    0x24, 0x91, 0x53, 0x5c, 0x0f, 0x56, 0xd5, 0x9e, 0xb5, 0xb3, 0xc1, 0xb0, 0xd8, 0xc6, 0x88, 0x5a,
    0x39, 0xc0, 0x52, 0x84, 0x85, 0x62, 0x7a, 0xd6, 0xd2, 0xf2, 0x82, 0x83, 0xd0, 0xeb, 0xc8, 0xb9,
    0xc5, 0x29, 0x0d, 0x64, 0x3d, 0x80, 0xef, 0x7e, 0xda, 0xea, 0xec, 0x30, 0xd1, 0xad, 0x2a, 0x27,
    0xc9, 0xf7, 0x98, 0x5d, 0x35, 0x71, 0x6a, 0x40, 0x61, 0xf1, 0x02, 0xfa, 0xd3, 0x8f, 0x86, 0xdb,
    0x20, 0x74, 0x10, 0xe3, 0x73, 0x3a, 0xcf, 0xe4, 0xa9, 0x16, 0x33, 0xb6, 0x28, 0x54, 0x7f, 0xd9,
    0x47, 0xbe, 0xc7, 0xe1, 0x0a, 0x21, 0xff, 0x34, 0x97, 0xbb, 0xf6, 0x92, 0x3c, 0x8e, 0x4b, 0x05,
    0xa4, 0x31, 0x2d, 0x00, 0xa2, 0xaf, 0xb7, 0xd4, 0xe7, 0x8d, 0xdc, 0x51, 0x2e, 0x55, 0x50, 0x38
};
#endif // szq rsbox

#define SZQ_GET_SBOX(n)  (sbox[(n)])
#define SZQ_GET_RSBOX(n) (rsbox[(n)])

void keyExtension(szq_ctx *ctx, uint8_t *keyBuf, uint8_t r)
{
    register uint16_t i;
    register uint8_t buf;
    for(i = 0; i < SZQ_BLOCKSIZE; ++i) {
        buf = ctx->RK[r][i] ^ ctx->RK[r][SZQ_RANGE_SUB(i, 7)] ^ ctx->RK[r][(SZQ_BLOCKSIZE / 3 * 2)];
        buf = ((i ^ buf) - r) ^ ctx->IV[i];
        keyBuf[i] = SZQ_INIT_RK(buf, ctx->IV[i], i, r);
        keyBuf[i] = SZQ_GET_SBOX(keyBuf[i] ^ 0xc9);
        // while(!keyBuf[i])
            keyBuf[i] = SZQ_GET_SBOX(((keyBuf[i] ^ buf) + ((i^r) ^ (i*r))) & 0xff);
        ctx->IV[i] = keyBuf[i];
    }
}

void ColumnMix(szq_state_t *state)
{
    uint8_t r, i;
}

void szq_init_ctx(szq_ctx *ctx, uint8_t *key)
{
    register uint16_t r, i;
    register uint8_t buf;
    uint8_t keyBuf[SZQ_BLOCKSIZE];

    memcpy(keyBuf, key, SZQ_BLOCKSIZE);
    for(r = 0; r < SZQ_ROUNDS; ++r) {
        memcpy(ctx->RK[r], keyBuf, SZQ_BLOCKSIZE);
        keyExtension(ctx, keyBuf, r);
    }
}

static void szq_Cipher(szq_state_t *state, szq_state_t *keyBuf)
{
    register uint8_t x, y;
    for(x = 0; x < SZQ_MAXIMUM; ++x) {
        for(y = 0; y < SZQ_MAXIMUM; ++y) {
            (*state)[x][y] = SZQ_GET_SBOX((*state)[x][y]);
            (*state)[x][y] = SZQ_E((*state)[x][y], (*keyBuf)[x][y]);
        }
    }
}

static void szq_InvCipher(szq_state_t *state, szq_state_t *keyBuf)
{
    register uint8_t x, y;
    for(x = 0; x < SZQ_MAXIMUM; ++x) {
        for(y = 0; y < SZQ_MAXIMUM; ++y) {
            (*state)[x][y] = SZQ_D((*state)[x][y], (*keyBuf)[x][y]);
            (*state)[x][y] = SZQ_GET_RSBOX((*state)[x][y]);
        }
    }
}

void szq_cbc_encrypt(szq_ctx *ctx, uint8_t *buf, size_t size)
{
    if (size % SZQ_BLOCKSIZE)
        return;
    
    register size_t r, i, n;
    uint8_t *buf_ptr = buf;
    for(r = 0; r < SZQ_ROUNDS; ++r) {
        for(i = n = 0; i < size; i += SZQ_BLOCKSIZE, ++n) {
            szq_Cipher((szq_state_t *)buf_ptr, (szq_state_t *)ctx->RK[r]);
            buf_ptr += SZQ_BLOCKSIZE;
        }
        buf_ptr -= SZQ_BLOCKSIZE * n;
    }
}

void szq_cbc_decrypt(szq_ctx *ctx, uint8_t *buf, size_t size)
{
    if (size % SZQ_BLOCKSIZE)
        return;
    
    register size_t r, i, n;
    register uint8_t *buf_ptr = buf;
    for(r = 0; r < SZQ_ROUNDS; ++r) {
        for(i = n = 0; i < size; i += SZQ_BLOCKSIZE, ++n) {
            szq_InvCipher((szq_state_t *)buf_ptr,
                (szq_state_t *)ctx->RK[SZQ_ROUNDS - r - 1]);
            buf_ptr += SZQ_BLOCKSIZE;
        }
        buf_ptr -= SZQ_BLOCKSIZE * n;
    }
}
