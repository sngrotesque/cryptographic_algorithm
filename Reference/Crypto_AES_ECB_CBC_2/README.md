> git@github.com:kellencataldo/aes_lib.git

## AES 128-Bit Encryption and Decryption Library in both ECB and CBC Modes

### About

**Warning: Please do not use this library where data protection is a serious concern. This is a library designed for hobby usage.**

This is a small AES encryption and decryption library written in C. It is designed to work on 128 bit blocks in both Electronic Code Book (ECB) and Cipher Block Chaining (CBC) modes. Space was my main concern, and as such I tried to design this library to have a small stack footprint. Feel free to use this however you want. I personally used it to test the cryptopals challenge, and then check memory usage on one of my Arduino boards. If you want an overview of AES encryption go [here](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard). If you want to see the differences between ECB and CBC modes go [here](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Common_modes). 

### Usage

Standard usage can be seen in the test.c file. The functions aes_ecb_encrypt() and 	aes_ecb_decrypt() take a pointer to the message to be encrypted/decrypted, the key to be used, and the size of the message. aes_cbc_encrypt() and aes_cbc_decrypt() take the same arguments as well as an Initilization Vector (IV). The data pointed to by the message pointer will then by encrypted or decrypted for the size given.

This library does not error check or pad as there are several different ways that padding could be implemented. The message must be divisible by 16 bytes (128 bits) and both the key and the IV must be exactly 16 bytes. 

Note: It is my own personal recommendation that you only ever encrypt things in CBC mode. ECB mode has several issues, most notably the fact that the same piece of 16 byte plain text will always encrypt to the same 16 bytes. For example, if I was encrypting a document that had my name, "Kellen L Cataldo" (16 bytes) appear more than once in the plain text, the exact same sequence of characters will appear multiple times in the encrypted text. This makes it relatively easy to detect if a file has been encrypted using AES ECB mode. 

### How it works

This library was implemented with size being the key concern. As such, it does not duplicate the plain text of the message to be encrypted or decrypted, and instead encrypts the data in place. 

Another place where space is saved is I do not have any Galois multiplication tables. Instead, I perform the Galois multiplication myself using a variation of the Russian peasant multiplication algorithm. In this way, I am saving space by not needing to include the 256 byte multiplication tables for encryption (2, 3) or the multiplication tables for decryption (9, 11, 13, 14). This is fairly unusual for a cryptography library as most implementations add these tables to increase the speed of encryption.

To reiterate, this library is for hobby and educational usage only. Please, please, please do not use this library to encrypt sensitive data. 
