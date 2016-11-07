#ifndef SM4ENCDEC_H_INCLUDED
#define SM4ENCDEC_H_INCLUDED

/*
功能：加密PlainText所指数据
入口：
char *PlainText    需要加密的数据
出口：
int *outLen        加密后数据的长度
返回：
char *             加密后的数据
*/
unsigned char *encrypt(unsigned char *PlainText, int inLen, int *outLen);

/*
功能：解密CipherText所指数据
入口：
char *CipherText   需要解密的数据
int TextLen        解密数据的长度
返回：
char *             解密后的数据
*/
unsigned char *decrypt(unsigned char *CipherText,int textLen,int *OutLen);



#endif // SM4ENCDEC_H_INCLUDED
