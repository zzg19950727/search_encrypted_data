#include "sm4EncDec.h"
#include <string.h>
#include "sm4.h"

/*
功能：加密PlainText所指数据
入口：
char *PlainText    需要加密的数据
出口：
int *outLen        加密后数据的长度
返回：
char *             加密后的数据
*/
unsigned char *encrypt(unsigned char *PlainText, int inLen, int *outLen)
{
	unsigned char key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	unsigned char iv[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10};
	int textLen = inLen;
	unsigned char *Text;//缓冲区首址
	sm4_context ctx;
	int originalLen;
	int pad;
	int i;

	//采用PKCS#5方式进行短块处理
	originalLen=textLen;
	pad=textLen%16;
	if(pad==0)
	{
		textLen+=16;
		Text=(unsigned char *)malloc(textLen*sizeof(unsigned char));
		for(i=textLen-1;i>textLen-1-16;i--)Text[i]=0;
	}
	else
	{
		textLen+=16-pad;
		Text=(unsigned char *)malloc(textLen*sizeof(unsigned char));
		for(i=textLen-1;i>textLen-1-(16-pad);i--)Text[i]=16-pad;
	}

	memcpy(Text,PlainText,originalLen);

	sm4_setkey_enc(&ctx,key);
	sm4_crypt_cbc(&ctx,1,textLen,iv,Text,Text);

	*outLen = textLen;
    return Text;
}

/*
功能：解密CipherText所指数据
入口：
char *CipherText   需要解密的数据
int TextLen        解密数据的长度
出口：
int *outLen        解密后数据的长度
返回：
char *             解密后的数据
*/
unsigned char *decrypt(unsigned char *CipherText,int textLen,int *OutLen)
{
    unsigned char key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	unsigned char iv[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10};
	unsigned char *Text=(unsigned char *)malloc(textLen);
	sm4_context ctx;
	int pad;

    sm4_setkey_dec(&ctx,key);
	sm4_crypt_cbc(&ctx,0,textLen,iv,CipherText,Text);

	//短块处理
 	pad=Text[textLen-1];
	if(pad==0)textLen-=16;
	else textLen-=pad;

    *OutLen = textLen;
	return Text;
}

