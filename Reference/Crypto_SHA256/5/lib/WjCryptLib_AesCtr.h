////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  WjCryptLib_AesCtr
//
//  Implementation of AES CTR stream cipher.
//
//  Depends on: CryptoLib_Aes
//
//  AES CTR is a stream cipher using the AES block cipher in counter mode.
//  This implementation works on both little and big endian architectures.
//
//  This is free and unencumbered software released into the public domain - November 2017 waterjuice.org
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  IMPORTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include "WjCryptLib_Aes.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  TYPES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define AES_CTR_IV_SIZE             8

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  TYPES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// AesCtrContext
// Do not modify the contents of this structure directly.
typedef struct
{
    AesContext      Aes;
    uint8_t         IV [AES_CTR_IV_SIZE];
    uint64_t        StreamIndex;
    uint64_t        CurrentCipherBlockIndex;
    uint8_t         CurrentCipherBlock [AES_BLOCK_SIZE];
} AesCtrContext;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AesCtrInitialise
//
//  Initialises an AesCtrContext with an already initialised AesContext and a IV. This function can quickly be used
//  to change the IV without requiring the more length processes of reinitialising an AES key.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    AesCtrInitialise
    (
        AesCtrContext*      Context,                // [out]
        AesContext const*   InitialisedAesContext,  // [in]
        uint8_t const       IV [AES_CTR_IV_SIZE]    // [in]
    );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AesCtrInitialiseWithKey
//
//  Initialises an AesCtrContext with an AES Key and an IV. This combines the initialising an AES Context and then
//  running AesCtrInitialise. KeySize must be 16, 24, or 32 (for 128, 192, or 256 bit key size)
//  Returns 0 if successful, or -1 if invalid KeySize provided
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int
    AesCtrInitialiseWithKey
    (
        AesCtrContext*      Context,                // [out]
        uint8_t const*      Key,                    // [in]
        uint32_t            KeySize,                // [in]
        uint8_t const       IV [AES_CTR_IV_SIZE]    // [in]
    );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AesCtrSetStreamIndex
//
//  Sets the current stream index to any arbitrary position. Setting to 0 sets it to the beginning of the stream. Any
//  subsequent output will start from this position
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    AesCtrSetStreamIndex
    (
        AesCtrContext*      Context,                // [in out]
        uint64_t            StreamIndex             // [in]
    );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AesCtrXor
//
//  XORs the stream of byte of the AesCtrContext from its current stream position onto the specified buffer. This will
//  advance the stream index by that number of bytes.
//  Use once over data to encrypt it. Use it a second time over the same data from the same stream position and the
//  data will be decrypted.
//  InBuffer and OutBuffer can point to the same location for in-place encrypting/decrypting
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    AesCtrXor
    (
        AesCtrContext*      Context,                // [in out]
        void const*         InBuffer,               // [in]
        void*               OutBuffer,              // [out]
        uint32_t            Size                    // [in]
    );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AesCtrOutput
//
//  Outputs the stream of byte of the AesCtrContext from its current stream position. This will advance the stream
//  index by that number of bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    AesCtrOutput
    (
        AesCtrContext*      Context,                // [in out]
        void*               Buffer,                 // [out]
        uint32_t            Size                    // [in]
    );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AesCtrXorWithKey
//
//  This function combines AesCtrInitialiseWithKey and AesCtrXor. This is suitable when encrypting/decypting data in
//  one go with a key that is not going to be reused.
//  This will used the provided Key and IV and generate a stream that is XORed over Buffer.
//  Returns 0 if successful, or -1 if invalid KeySize provided
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int
    AesCtrXorWithKey
    (
        uint8_t const*      Key,                    // [in]
        uint32_t            KeySize,                // [in]
        uint8_t const       IV [AES_CTR_IV_SIZE],   // [in]
        void const*         InBuffer,               // [in]
        void*               OutBuffer,              // [out]
        uint32_t            BufferSize              // [in]
    );
