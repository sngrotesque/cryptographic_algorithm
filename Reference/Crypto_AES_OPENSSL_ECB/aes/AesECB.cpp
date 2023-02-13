/********************************************************
Copyright (C),  2016-2018,
FileName:		AesECB
Author:        	woniu201
Email:         	wangpengfei.201@163.com
Created:       	2018/09/11
Description:	ʵ��AES128-ECB��ʽ���ܺͽ��ܣ���䷽ʽzeropadding
********************************************************/
#include <iostream>
#include <string>
#include <stdlib.h>

#include "openssl/evp.h"  
#include "openssl/aes.h"
using namespace std;

#pragma comment(lib, "libeay32.lib")

/************************************
@ Brief:		����
@ Author:		woniu201 
@ Created:		2018/09/11
@ Return:            
************************************/
int EncryptECB(unsigned char* inData, int inDataLen,  unsigned char* outData, unsigned char* key)
{
	AES_KEY  aesKey;

	int block = 0;
	if (inDataLen%16 == 0)
	{
		block = inDataLen/16;
	}
	else
	{
		block = inDataLen/16 + 1;
	}

	AES_set_encrypt_key(key, 128, &aesKey);


	for (int i=0; i<block; i++)
	{
		AES_ecb_encrypt(inData+i*16, outData+i*16, &aesKey, AES_ENCRYPT);
	}

	return 0;
}


/************************************
@ Brief:		����
@ Author:		woniu201 
@ Created:		2018/09/11
@ Return:            
************************************/
int DecryptECB(unsigned char* inData, unsigned char* outData, unsigned char* key)
{
	AES_KEY  aesKey;
	int block = strlen((char*)inData) /16;
	AES_set_decrypt_key(key, 128, &aesKey);

	for (int i=0; i<block; i++)
	{
		AES_ecb_encrypt(inData+i*16, outData+i*16, &aesKey, AES_DECRYPT);
	}
	return 0;
}


int main()
{
	unsigned char key[128] = {0};
	unsigned char data[512] = {0};
	memcpy(data, "abcdefghijklmnopqrstuvwxyz", strlen("abcdefghijklmnopqrstuvwxyz"));
	memcpy(key, "1234567812345678", strlen("1234567812345678"));

	cout << "ԭ�����ݣ�" << data << endl;
	unsigned char outData[1024] = {0};
	
	//================����=======================//
	EncryptECB(data,  strlen((char*)data), outData, key);
	cout << "���ܺ������(hex)��";
	for (int i=0; i<strlen((char*)outData); i++)
	{
		printf("%02x", outData[i]);
	}
	cout << endl;

	//=================����=======================//
	unsigned char plainTxt[128] = {0};
	DecryptECB(outData, plainTxt, key);
	cout << "���ܺ����ݣ�" << plainTxt << endl;
	getchar();
}