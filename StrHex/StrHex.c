#include "StrHex.h"
#include <ctype.h>
#include <stdlib.h>

/*
功能：将十六进制数据转换成字符串形式
入口：
unsigned char *hex  十六进制数据
int len             十六进制数据的长度
返回：
char *              转换后的字符串
备注：转换后的字符串长度是十六进制数据长度的2倍
*/
char *hex2str(unsigned char *hex,int len)
{
    int i;
    char dec2hex[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    char *str = (char*)malloc(2*len*sizeof(unsigned char));
    for(i=0;i<len;i++)
    {
        str[2*i] = dec2hex[hex[i]>>4];
        str[2*i+1] = dec2hex[hex[i]&0xF];
    }
    return str;
}

/*
功能：将字符串形式的十六进制数据转换成十六进制
入口：
char *str           字符串形式的十六进制数据
int len             字符串形式的十六进制数据的长度
返回：
char *              转换后的十六进制数据
备注：该函数只能转换由hex2str函数生成的字符串形式的十六进制数据，因为10以上的数据都是用小写字母表示的
*/
unsigned char *str2hex(char *str,int len)
{
    int i;
    unsigned char *hex = (unsigned char*)malloc((len/2)*sizeof(unsigned char));

    for(i=0;i<len/2;i++)
    {
        if(isdigit(str[2*i]))
        {
            hex[i] = (str[2*i]-48)<<4;
        }
        else{
            hex[i] = (str[2*i]-97+10)<<4;
        }

        if(isdigit(str[2*i+1]))
        {
            hex[i]|= (str[2*i+1]-48)&0xF;
        }
        else{
            hex[i] |= (str[2*i+1]-97+10)&0xF;
        }
    }
    return hex;
}

