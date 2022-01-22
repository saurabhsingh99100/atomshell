#include <W25Q64.h>
#include <stdlib.h>
#include <stdio.h>

int fn_spiflash_init()
{
    W25Q64_init();
}

char man_spiflash_init [] = 
"Initialize SPI flash\n";

int fn_spiflash_read(int argc, char **argv)
{
    unsigned int addr = xtoi(argv[1]);
    unsigned int n = atoi(argv[2]);
    
    char buf[100];
    W25Q64_read(buf, n, addr);

    int print_coloumns = 4;

    unsigned int word = 0;

    for(int i=0; i<n; i++)
    {
        if(i%print_coloumns==0)
        {
            putchar('\n'); puts("0x"); puthex(addr+i, 6, false); puts(" : ");
            word = 0;
        }
        
        puthex(buf[i], 2, false); putchar(' ');
        word = (word >> 8) | (((unsigned int)buf[i]) << 24);

        if(i%print_coloumns==print_coloumns-1)
        {
            puts("(0x"); puthex(word, 8, false); putchar(')');
        }

    }
}

char man_spiflash_read [] = 
"Reaad bytes from SPI flash\n"
"Usage: > spiflash_readbytes <start address (in hex)> <length (in dec)>\n";

