#include "snc.h"

static const snByte SNC_sbox[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x25, 0xd1, 0x84, 0xb8, 0x48, 0x35, 0x4a, 0x78, 0x79, 0x74, 0x60, 0xc7, 0x0e, 0xbc, 0x32, 0x30,
    0x91, 0x18, 0xe5, 0xdc, 0xe4, 0x90, 0x81, 0xc8, 0x65, 0x2b, 0xc3, 0x9d, 0x67, 0xa5, 0xf9, 0xdf,
    0x13, 0xf6, 0x1b, 0xd4, 0xa9, 0xb7, 0xc2, 0x5a, 0x85, 0xac, 0x41, 0x8e, 0xeb, 0x72, 0xef, 0xad,
    0x88, 0x5e, 0x1f, 0x6a, 0x52, 0x80, 0xdd, 0x16, 0xbe, 0x46, 0xf4, 0x3f, 0x94, 0x64, 0x04, 0xa8,
    0xb4, 0xc1, 0x8f, 0xee, 0x82, 0x1e, 0x8b, 0x01, 0x20, 0x11, 0xae, 0x8d, 0x17, 0x1c, 0x68, 0xab,
    0x8a, 0xfc, 0xf0, 0x50, 0xf2, 0x42, 0x0a, 0x59, 0x58, 0x6c, 0x45, 0x3e, 0x89, 0xb5, 0xa1, 0x71,
    0x73, 0x4c, 0xbd, 0xd9, 0x2f, 0x4f, 0xed, 0xdb, 0xb2, 0xaf, 0x4e, 0xff, 0x9f, 0x0b, 0x51, 0xe7,
    0x5c, 0x75, 0x53, 0x63, 0xc6, 0x6e, 0x0d, 0xf5, 0x5d, 0x47, 0x23, 0xb0, 0xda, 0xa4, 0xb6, 0x93,
    0x54, 0x19, 0x10, 0xd0, 0xfa, 0xc5, 0xd8, 0x97, 0x87, 0x3c, 0x95, 0x4b, 0xd3, 0x3d, 0xcb, 0xbb,
    0x33, 0x05, 0x0c, 0x4d, 0x08, 0x7d, 0x24, 0x5b, 0x0f, 0x3b, 0xce, 0xe3, 0xe8, 0x2e, 0xba, 0x06,
    0x57, 0xec, 0x86, 0x1d, 0x29, 0x9e, 0xa6, 0xc9, 0x31, 0x66, 0xe1, 0xfe, 0xcc, 0x7e, 0x8c, 0x1a,
    0x9a, 0x03, 0x44, 0xa7, 0xd2, 0x36, 0x2c, 0x83, 0xe2, 0x69, 0x7a, 0xe9, 0x37, 0x96, 0xb1, 0x02,
    0xea, 0x28, 0x9b, 0x21, 0xfd, 0x77, 0x3a, 0xcf, 0xf7, 0xca, 0x7b, 0x98, 0xa3, 0xc4, 0xe0, 0x6b,
    0xb9, 0xf8, 0xa2, 0x22, 0x43, 0xfb, 0xde, 0x26, 0x2d, 0xc0, 0xb3, 0xaa, 0x27, 0x7f, 0xe6, 0xbf,
    0x39, 0x9c, 0x38, 0x40, 0x09, 0x5f, 0x76, 0x34, 0xa0, 0x14, 0x49, 0x6f, 0xd5, 0xd6, 0x00, 0x12,
    0x55, 0x70, 0xf3, 0x62, 0x92, 0x56, 0x6d, 0x15, 0x2a, 0x61, 0xcd, 0x99, 0x7c, 0xf1, 0xd7, 0x07
};

static const snByte SNC_rsbox[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0xee, 0x47, 0xbf, 0xb1, 0x3e, 0x91, 0x9f, 0xff, 0x94, 0xe4, 0x56, 0x6d, 0x92, 0x76, 0x0c, 0x98,
    0x82, 0x49, 0xef, 0x20, 0xe9, 0xf7, 0x37, 0x4c, 0x11, 0x81, 0xaf, 0x22, 0x4d, 0xa3, 0x45, 0x32,
    0x48, 0xc3, 0xd3, 0x7a, 0x96, 0x00, 0xd7, 0xdc, 0xc1, 0xa4, 0xf8, 0x19, 0xb6, 0xd8, 0x9d, 0x64,
    0x0f, 0xa8, 0x0e, 0x90, 0xe7, 0x05, 0xb5, 0xbc, 0xe2, 0xe0, 0xc6, 0x99, 0x89, 0x8d, 0x5b, 0x3b,
    0xe3, 0x2a, 0x55, 0xd4, 0xb2, 0x5a, 0x39, 0x79, 0x04, 0xea, 0x06, 0x8b, 0x61, 0x93, 0x6a, 0x65,
    0x53, 0x6e, 0x34, 0x72, 0x80, 0xf0, 0xf5, 0xa0, 0x58, 0x57, 0x27, 0x97, 0x70, 0x78, 0x31, 0xe5,
    0x0a, 0xf9, 0xf3, 0x73, 0x3d, 0x18, 0xa9, 0x1c, 0x4e, 0xb9, 0x33, 0xcf, 0x59, 0xf6, 0x75, 0xeb,
    0xf1, 0x5f, 0x2d, 0x60, 0x09, 0x71, 0xe6, 0xc5, 0x07, 0x08, 0xba, 0xca, 0xfc, 0x95, 0xad, 0xdd,
    0x35, 0x16, 0x44, 0xb7, 0x02, 0x28, 0xa2, 0x88, 0x30, 0x5c, 0x50, 0x46, 0xae, 0x4b, 0x2b, 0x42,
    0x15, 0x10, 0xf4, 0x7f, 0x3c, 0x8a, 0xbd, 0x87, 0xcb, 0xfb, 0xb0, 0xc2, 0xe1, 0x1b, 0xa5, 0x6c,
    0xe8, 0x5e, 0xd2, 0xcc, 0x7d, 0x1d, 0xa6, 0xb3, 0x3f, 0x24, 0xdb, 0x4f, 0x29, 0x2f, 0x4a, 0x69,
    0x7b, 0xbe, 0x68, 0xda, 0x40, 0x5d, 0x7e, 0x25, 0x03, 0xd0, 0x9e, 0x8f, 0x0d, 0x62, 0x38, 0xdf,
    0xd9, 0x41, 0x26, 0x1a, 0xcd, 0x85, 0x74, 0x0b, 0x17, 0xa7, 0xc9, 0x8e, 0xac, 0xfa, 0x9a, 0xc7,
    0x83, 0x01, 0xb4, 0x8c, 0x23, 0xec, 0xed, 0xfe, 0x86, 0x63, 0x7c, 0x67, 0x13, 0x36, 0xd6, 0x1f,
    0xce, 0xaa, 0xb8, 0x9b, 0x14, 0x12, 0xde, 0x6f, 0x9c, 0xbb, 0xc0, 0x2c, 0xa1, 0x66, 0x43, 0x2e,
    0x52, 0xfd, 0x54, 0xf2, 0x3a, 0x77, 0x21, 0xc8, 0xd1, 0x1e, 0x84, 0xd5, 0x51, 0xc4, 0xab, 0x6b
};

#define SNC_SBOX(x)  (SNC_sbox[(x)])
#define SNC_RSBOX(x) (SNC_rsbox[(x)])

//*-----------------------Private Function--------------------------------*//
static snVoid SNC_keyExtension(snByte *iv, snByte *key)
{
    static snSize_t i;
    static snByte buf;

    for(i = 0; i < SNC_KEYLEN; ++i) {
        buf = 
            key[i]                     ^ key[(i + 1)  % SNC_KEYLEN] ^
            key[(i + 4)  % SNC_KEYLEN] ^ key[(i + 5)  % SNC_KEYLEN] ^
            key[(i + 8)  % SNC_KEYLEN] ^ key[(i + 9)  % SNC_KEYLEN] ^
            key[(i + 12) % SNC_KEYLEN] ^ key[(i + 13) % SNC_KEYLEN] ^
            key[(i + 16) % SNC_KEYLEN] ^ key[(i + 17) % SNC_KEYLEN] ^
            key[(i + 20) % SNC_KEYLEN] ^ key[(i + 21) % SNC_KEYLEN] ^
            key[(i + 24) % SNC_KEYLEN] ^ key[(i + 25) % SNC_KEYLEN] ^
            key[(i + 28) % SNC_KEYLEN] ^ key[(i + 29) % SNC_KEYLEN] ^
            iv[(i + 7) % SNC_BLOCKLEN] ^ iv[(i + 16) % SNC_BLOCKLEN];

        iv[i % SNC_BLOCKLEN] ^= buf;

        buf = 
            key[(i + 2)  % SNC_KEYLEN] ^ key[(i + 3)  % SNC_KEYLEN] ^
            key[(i + 6)  % SNC_KEYLEN] ^ key[(i + 7)  % SNC_KEYLEN] ^
            key[(i + 10) % SNC_KEYLEN] ^ key[(i + 11) % SNC_KEYLEN] ^
            key[(i + 14) % SNC_KEYLEN] ^ key[(i + 15) % SNC_KEYLEN] ^
            key[(i + 18) % SNC_KEYLEN] ^ key[(i + 19) % SNC_KEYLEN] ^
            key[(i + 22) % SNC_KEYLEN] ^ key[(i + 23) % SNC_KEYLEN] ^
            key[(i + 26) % SNC_KEYLEN] ^ key[(i + 27) % SNC_KEYLEN] ^
            key[(i + 29) % SNC_KEYLEN] ^ key[(i + 31) % SNC_KEYLEN] ^
            iv[(i + 7) % SNC_BLOCKLEN] ^ iv[(i + 16) % SNC_BLOCKLEN];

        iv[(i + 17) % SNC_BLOCKLEN] ^= buf;

        key[(i + 7)  % SNC_KEYLEN] = (buf ^ key[(i + 7)  % SNC_KEYLEN]) ^ (SNC_KEYLEN % (i + 1));
        key[(i + 14) % SNC_KEYLEN] = (buf ^ key[(i + 14) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 21) % SNC_KEYLEN] = (buf ^ key[(i + 21) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 28) % SNC_KEYLEN] = (buf ^ key[(i + 28) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 35) % SNC_KEYLEN] = (buf ^ key[(i + 35) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 42) % SNC_KEYLEN] = (buf ^ key[(i + 42) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 47) % SNC_KEYLEN] = (buf ^ key[(i + 47) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 51) % SNC_KEYLEN] = (buf ^ key[(i + 51) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 54) % SNC_KEYLEN] = (buf ^ key[(i + 54) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 63) % SNC_KEYLEN] = (buf ^ key[(i + 63) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 67) % SNC_KEYLEN] = (buf ^ key[(i + 67) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 73) % SNC_KEYLEN] = (buf ^ key[(i + 73) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 79) % SNC_KEYLEN] = (buf ^ key[(i + 79) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 85) % SNC_KEYLEN] = (buf ^ key[(i + 85) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));
        key[(i + 92) % SNC_KEYLEN] = (buf ^ key[(i + 92) % SNC_KEYLEN]) + (SNC_KEYLEN % (i + 1));

        key[i] = buf;
    }
}

static snVoid SNC_SubBytes(sncState_t *state)
{
    register sn_u32 i;

    for(i = 0; i < SNC_NK; ++i) {
        (*state)[0][i] = SNC_SBOX((*state)[0][i]);
        (*state)[1][i] = SNC_SBOX((*state)[1][i]);
        (*state)[2][i] = SNC_SBOX((*state)[2][i]);
        (*state)[3][i] = SNC_SBOX((*state)[3][i]);
        (*state)[4][i] = SNC_SBOX((*state)[4][i]);
        (*state)[5][i] = SNC_SBOX((*state)[5][i]);
        (*state)[6][i] = SNC_SBOX((*state)[6][i]);
        (*state)[7][i] = SNC_SBOX((*state)[7][i]);
    }
}

static snVoid SNC_InvSubBytes(sncState_t *state)
{
    register sn_u32 i;

    for(i = 0; i < SNC_NK; ++i) {
        (*state)[0][i] = SNC_RSBOX((*state)[0][i]);
        (*state)[1][i] = SNC_RSBOX((*state)[1][i]);
        (*state)[2][i] = SNC_RSBOX((*state)[2][i]);
        (*state)[3][i] = SNC_RSBOX((*state)[3][i]);
        (*state)[4][i] = SNC_RSBOX((*state)[4][i]);
        (*state)[5][i] = SNC_RSBOX((*state)[5][i]);
        (*state)[6][i] = SNC_RSBOX((*state)[6][i]);
        (*state)[7][i] = SNC_RSBOX((*state)[7][i]);
    }
}

#define ROWS_MIX_ENCODE(x, y, z, p) (x = ((x ^ y) - z) ^ p)
#define ROWS_MIX_DECODE(x, y, z, p) (x = ((x ^ p) + z) ^ y)

static snVoid SNC_RowsMix(sncState_t *state)
{
    register sn_u32 i;

    for(i = 0; i < SNC_NB; ++i) {
        ROWS_MIX_ENCODE((*state)[i][0 % SNC_NK], (*state)[i][3 % SNC_NK], (*state)[i][5 % SNC_NK], 0x07);
        ROWS_MIX_ENCODE((*state)[i][1 % SNC_NK], (*state)[i][4 % SNC_NK], (*state)[i][6 % SNC_NK], 0x0d);
        ROWS_MIX_ENCODE((*state)[i][2 % SNC_NK], (*state)[i][5 % SNC_NK], (*state)[i][7 % SNC_NK], 0x17);
        ROWS_MIX_ENCODE((*state)[i][3 % SNC_NK], (*state)[i][6 % SNC_NK], (*state)[i][0 % SNC_NK], 0x29);
        ROWS_MIX_ENCODE((*state)[i][4 % SNC_NK], (*state)[i][7 % SNC_NK], (*state)[i][1 % SNC_NK], 0x43);
        ROWS_MIX_ENCODE((*state)[i][5 % SNC_NK], (*state)[i][0 % SNC_NK], (*state)[i][2 % SNC_NK], 0x4f);
        ROWS_MIX_ENCODE((*state)[i][6 % SNC_NK], (*state)[i][1 % SNC_NK], (*state)[i][3 % SNC_NK], 0x61);
        ROWS_MIX_ENCODE((*state)[i][7 % SNC_NK], (*state)[i][2 % SNC_NK], (*state)[i][4 % SNC_NK], 0x71);
    }
}

static snVoid SNC_InvRowsMix(sncState_t *state)
{
    register sn_u32 i;

    for(i = 0; i < SNC_NB; ++i) {
        ROWS_MIX_DECODE((*state)[i][7 % SNC_NK], (*state)[i][2 % SNC_NK], (*state)[i][4 % SNC_NK], 0x71);
        ROWS_MIX_DECODE((*state)[i][6 % SNC_NK], (*state)[i][1 % SNC_NK], (*state)[i][3 % SNC_NK], 0x61);
        ROWS_MIX_DECODE((*state)[i][5 % SNC_NK], (*state)[i][0 % SNC_NK], (*state)[i][2 % SNC_NK], 0x4f);
        ROWS_MIX_DECODE((*state)[i][4 % SNC_NK], (*state)[i][7 % SNC_NK], (*state)[i][1 % SNC_NK], 0x43);
        ROWS_MIX_DECODE((*state)[i][3 % SNC_NK], (*state)[i][6 % SNC_NK], (*state)[i][0 % SNC_NK], 0x29);
        ROWS_MIX_DECODE((*state)[i][2 % SNC_NK], (*state)[i][5 % SNC_NK], (*state)[i][7 % SNC_NK], 0x17);
        ROWS_MIX_DECODE((*state)[i][1 % SNC_NK], (*state)[i][4 % SNC_NK], (*state)[i][6 % SNC_NK], 0x0d);
        ROWS_MIX_DECODE((*state)[i][0 % SNC_NK], (*state)[i][3 % SNC_NK], (*state)[i][5 % SNC_NK], 0x07);
    }
}

static snVoid SNC_ColumnShift(sncState_t *state)
{
    static snByte buf;

    buf = (*state)[0][0];
    (*state)[0][0] = (*state)[1][0]; (*state)[1][0] = (*state)[2][0];
    (*state)[2][0] = (*state)[3][0]; (*state)[3][0] = (*state)[4][0];
    (*state)[4][0] = (*state)[5][0]; (*state)[5][0] = (*state)[6][0];
    (*state)[6][0] = (*state)[7][0]; (*state)[7][0] = buf;

    buf = (*state)[0][1];
    (*state)[0][1] = (*state)[2][1]; (*state)[2][1] = (*state)[4][1];
    (*state)[4][1] = (*state)[6][1]; (*state)[6][1] = buf;

    buf = (*state)[1][1];
    (*state)[1][1] = (*state)[3][1]; (*state)[3][1] = (*state)[5][1];
    (*state)[5][1] = (*state)[7][1]; (*state)[7][1] = buf;

    buf = (*state)[0][2];
    (*state)[0][2] = (*state)[3][2]; (*state)[3][2] = (*state)[6][2];
    (*state)[6][2] = (*state)[1][2]; (*state)[1][2] = (*state)[4][2];
    (*state)[4][2] = (*state)[7][2]; (*state)[7][2] = (*state)[2][2];
    (*state)[2][2] = (*state)[5][2]; (*state)[5][2] = buf;

    buf = (*state)[0][3]; (*state)[0][3] = (*state)[4][3]; (*state)[4][3] = buf;
    buf = (*state)[1][3]; (*state)[1][3] = (*state)[5][3]; (*state)[5][3] = buf;
    buf = (*state)[2][3]; (*state)[2][3] = (*state)[6][3]; (*state)[6][3] = buf;
    buf = (*state)[3][3]; (*state)[3][3] = (*state)[7][3]; (*state)[7][3] = buf;
}

static snVoid SNC_InvColumnShift(sncState_t *state)
{
    static snByte buf;

    buf = (*state)[7][3]; (*state)[7][3] = (*state)[3][3]; (*state)[3][3] = buf;
    buf = (*state)[6][3]; (*state)[6][3] = (*state)[2][3]; (*state)[2][3] = buf;
    buf = (*state)[5][3]; (*state)[5][3] = (*state)[1][3]; (*state)[1][3] = buf;
    buf = (*state)[4][3]; (*state)[4][3] = (*state)[0][3]; (*state)[0][3] = buf;

    buf = (*state)[5][2];
    (*state)[5][2] = (*state)[2][2]; (*state)[2][2] = (*state)[7][2];
    (*state)[7][2] = (*state)[4][2]; (*state)[4][2] = (*state)[1][2];
    (*state)[1][2] = (*state)[6][2]; (*state)[6][2] = (*state)[3][2];
    (*state)[3][2] = (*state)[0][2]; (*state)[0][2] = buf;

    buf = (*state)[7][1];
    (*state)[7][1] = (*state)[5][1]; (*state)[5][1] = (*state)[3][1];
    (*state)[3][1] = (*state)[1][1]; (*state)[1][1] = buf;
    buf = (*state)[6][1];
    (*state)[6][1] = (*state)[4][1]; (*state)[4][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[0][1];(*state)[0][1] = buf;

    buf = (*state)[7][0];
    (*state)[7][0] = (*state)[6][0]; (*state)[6][0] = (*state)[5][0];
    (*state)[5][0] = (*state)[4][0]; (*state)[4][0] = (*state)[3][0];
    (*state)[3][0] = (*state)[2][0]; (*state)[2][0] = (*state)[1][0];
    (*state)[1][0] = (*state)[0][0]; (*state)[0][0] = buf;
}

static snVoid SNC_XorWithIV(sncState_t *buf, sncState_t *iv)
{
    register sn_u32 i;
    for(i = 0; i < SNC_NK; ++i) {
        (*buf)[0][i] ^= (*iv)[0][i];
        (*buf)[1][i] ^= (*iv)[1][i];
        (*buf)[2][i] ^= (*iv)[2][i];
        (*buf)[3][i] ^= (*iv)[3][i];
        (*buf)[4][i] ^= (*iv)[4][i];
        (*buf)[5][i] ^= (*iv)[5][i];
        (*buf)[6][i] ^= (*iv)[6][i];
        (*buf)[7][i] ^= (*iv)[7][i];
    }
}

static snVoid SNC_BitSwitch(sncState_t *state)
{
    register sn_u32 i;
    for(i = 0; i < SNC_NK; ++i) {
        (*state)[0][i] = bitSwitch((*state)[0][i]);
        (*state)[1][i] = bitSwitch((*state)[1][i]);
        (*state)[2][i] = bitSwitch((*state)[2][i]);
        (*state)[3][i] = bitSwitch((*state)[3][i]);
        (*state)[4][i] = bitSwitch((*state)[4][i]);
        (*state)[5][i] = bitSwitch((*state)[5][i]);
        (*state)[6][i] = bitSwitch((*state)[6][i]);
        (*state)[7][i] = bitSwitch((*state)[7][i]);
    }
}

static snVoid SNC_Cipher(sncState_t *state, sncState_t *RoundKey)
{
    register sn_u32 i;

    SNC_SubBytes(state);

    for(i = 0; i < SNC_NK; ++i) {
        (*state)[0][i] ^= (*RoundKey)[0][i];
        (*state)[1][i] ^= (*RoundKey)[1][i];
        (*state)[2][i] ^= (*RoundKey)[2][i];
        (*state)[3][i] ^= (*RoundKey)[3][i];
        (*state)[4][i] ^= (*RoundKey)[4][i];
        (*state)[5][i] ^= (*RoundKey)[5][i];
        (*state)[6][i] ^= (*RoundKey)[6][i];
        (*state)[7][i] ^= (*RoundKey)[7][i];
#if defined(SNC_512) || defined(SNC_768)
        (*state)[0][i] ^= (*RoundKey)[0][i + 4];
        (*state)[1][i] ^= (*RoundKey)[1][i + 4];
        (*state)[2][i] ^= (*RoundKey)[2][i + 4];
        (*state)[3][i] ^= (*RoundKey)[3][i + 4];
        (*state)[4][i] ^= (*RoundKey)[4][i + 4];
        (*state)[5][i] ^= (*RoundKey)[5][i + 4];
        (*state)[6][i] ^= (*RoundKey)[6][i + 4];
        (*state)[7][i] ^= (*RoundKey)[7][i + 4];
#endif // #if defined(SNC_512) || defined(SNC_768)
#if defined(SNC_768)
        (*state)[0][i] ^= (*RoundKey)[0][i + 8];
        (*state)[1][i] ^= (*RoundKey)[1][i + 8];
        (*state)[2][i] ^= (*RoundKey)[2][i + 8];
        (*state)[3][i] ^= (*RoundKey)[3][i + 8];
        (*state)[4][i] ^= (*RoundKey)[4][i + 8];
        (*state)[5][i] ^= (*RoundKey)[5][i + 8];
        (*state)[6][i] ^= (*RoundKey)[6][i + 8];
        (*state)[7][i] ^= (*RoundKey)[7][i + 8];
#endif // #if defined(SNC_768)
    }


    SNC_RowsMix(state);
    SNC_ColumnShift(state);
}

static snVoid SNC_InvCipher(sncState_t *state, sncState_t *RoundKey)
{
    register sn_u32 i;

    SNC_InvColumnShift(state);
    SNC_InvRowsMix(state);

    for(i = 0; i < SNC_NK; ++i) {
#if defined(SNC_768)
        (*state)[0][i] ^= (*RoundKey)[0][i + 8];
        (*state)[1][i] ^= (*RoundKey)[1][i + 8];
        (*state)[2][i] ^= (*RoundKey)[2][i + 8];
        (*state)[3][i] ^= (*RoundKey)[3][i + 8];
        (*state)[4][i] ^= (*RoundKey)[4][i + 8];
        (*state)[5][i] ^= (*RoundKey)[5][i + 8];
        (*state)[6][i] ^= (*RoundKey)[6][i + 8];
        (*state)[7][i] ^= (*RoundKey)[7][i + 8];
#endif // #if defined(SNC_768)
#if defined(SNC_512) || defined(SNC_768)
        (*state)[0][i] ^= (*RoundKey)[0][i + 4];
        (*state)[1][i] ^= (*RoundKey)[1][i + 4];
        (*state)[2][i] ^= (*RoundKey)[2][i + 4];
        (*state)[3][i] ^= (*RoundKey)[3][i + 4];
        (*state)[4][i] ^= (*RoundKey)[4][i + 4];
        (*state)[5][i] ^= (*RoundKey)[5][i + 4];
        (*state)[6][i] ^= (*RoundKey)[6][i + 4];
        (*state)[7][i] ^= (*RoundKey)[7][i + 4];
#endif // #if defined(SNC_512) || defined(SNC_768)
        (*state)[0][i] ^= (*RoundKey)[0][i];
        (*state)[1][i] ^= (*RoundKey)[1][i];
        (*state)[2][i] ^= (*RoundKey)[2][i];
        (*state)[3][i] ^= (*RoundKey)[3][i];
        (*state)[4][i] ^= (*RoundKey)[4][i];
        (*state)[5][i] ^= (*RoundKey)[5][i];
        (*state)[6][i] ^= (*RoundKey)[6][i];
        (*state)[7][i] ^= (*RoundKey)[7][i];
    }

    SNC_InvSubBytes(state);
}

//*--------------------------Public Function-----------------------------*//
snVoid SNC_init_ctx(SNC_ctx *ctx, const snByte *keyBuf)
{
    static sn_u32 r;
    static snByte key[SNC_KEYLEN];
    static snByte iv[SNC_BLOCKLEN];

    memcpy(key, keyBuf, SNC_KEYLEN);
    memcpy(iv, ctx->iv, SNC_BLOCKLEN);

    for(r = 0; r < SNC_NR; ++r) {
        memcpy(ctx->rk[r], key, SNC_KEYLEN);
        SNC_keyExtension(iv, key);
    }
}

snVoid SNC_ECB_Encrypt(SNC_ctx *ctx, snByte *buf, snSize_t size)
{
    register snSize_t r;
    register snSize_t i;

    sncState_t *bufState = (sncState_t *)buf;

    size /= SNC_BLOCKLEN;

    for(r = 0; r < SNC_NR; ++r) {
        for(i = 0; i < size; ++i) {
            SNC_Cipher((bufState + i), (sncState_t *)ctx->rk[r]);
        }
    }
}

snVoid SNC_ECB_Decrypt(SNC_ctx *ctx, snByte *buf, snSize_t size)
{
    register snSize_t r;
    register snSize_t i;

    sncState_t *bufState = (sncState_t *)buf;

    size /= SNC_BLOCKLEN;

    for(r = 0; r < SNC_NR; ++r) {
        for(i = 0; i < size; ++i) {
            SNC_InvCipher((bufState + i), (sncState_t *)ctx->rk[SNC_NR - r - 1]);
        }
    }
}

snVoid SNC_CBC_Encrypt(SNC_ctx *ctx, snByte *buf, snSize_t size)
{
    sncState_t *bufState = (sncState_t *)buf;
    sncState_t *ivState = (sncState_t *)ctx->iv;
    register snSize_t r;
    register snSize_t i;

    size /= SNC_BLOCKLEN;

    for(r = 0; r < SNC_NR; ++r) {
        for(i = 0; i < size; ++i) {
            SNC_XorWithIV((bufState + i), ivState);
            SNC_Cipher((bufState + i), (sncState_t *)ctx->rk[r]);
            ivState = (bufState + i);
        }
    }
}

snVoid SNC_CBC_Decrypt(SNC_ctx *ctx, snByte *buf, snSize_t size)
{
    sncState_t *bufState = (sncState_t *)buf;
    sncState_t *ivState = snNull;
    register snSize_t r;
    register snSize_t i;

    size /= SNC_BLOCKLEN;

    for(r = 0; r < SNC_NR; ++r) {
        for(i = 0; i < size; ++i) {
            ivState = (bufState + i);
            SNC_InvCipher((bufState + i), (sncState_t *)ctx->rk[SNC_NR - r - 1]);
            SNC_XorWithIV((bufState + i), ivState);
        }
    }
}
