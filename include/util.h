#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>

// RLE
void RLEdecode(char *str, unsigned int length)
{
    unsigned int i = 0;
    while(i < length) 
    {
        char chr = str[i];
        for(int j=0; j<str[i+1]; j++)
            putchar(chr);
        i+=2;
    }
}

#endif //__UTIL_H__