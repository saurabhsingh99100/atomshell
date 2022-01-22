#ifndef __MEMTEST_H__
#define __MEMTEST_H__
#include <stdlib.h>
#include <serial.h>

long unsigned int xtou64(const char *str)
{
    long unsigned int res = 0;
    char c;

    while ((c = *str++)) {
        char v = (c & 0xF) + (c >> 6) | ((c >> 3) & 0x8);
        res = (res << 4) | (long unsigned int) v;
    }

    return res;
} 

int fn_peek(int argc, char **argv)
{
    unsigned int address = xtou64(argv[1]);
    printf("0x%x : 0x%x 0x%x 0x%x 0x%x\n", address, *((volatile char*) address), *((volatile char*) address+1), 
        *((volatile char*) address+2), *((volatile char*) address+3));
}

char man_peek [] = 
"Peek at a memory loction\n"
"Usage:  peek [addr (in hex)]\n";

int fn_poke(int argc, char **argv)
{
    unsigned int address = xtou64(argv[1]);
    int data = (int) xtou64(argv[2]);

    printf("0x%x : 0x%x <= [0x%x]\n", address, *((volatile int*) address), data);
    *((volatile int*) address) = data;
}

char man_poke [] = 
"Poke data at a memory loction\n"
"Usage:  poke [addr (in hex)] [data (in hex)]\n";


int fn_load(int argc, char **argv)
{
    unsigned int base = xtou64(argv[1]);
    unsigned int size = xtou64(argv[2]);

    printf("Waiting for a binary image of %d bytes... (will be loaded at 0x%x)\n", size, base);

    char * ptr = (char*) base;
    char byte;
    for(unsigned long int i=0; i<size; i++)
    {
        byte = serial_read();
        *((volatile char*) ptr++) = byte;
    }

    printf("\nOK!\n");
}

char man_load [] = 
"Load a binary file transmitted over UART into memory\n"
"Usage:  load [base_addr (in hex)] [size (in bytes, decimal)]\n";


int fn_run(int argc, char **argv)
{
    register unsigned int entry = xtou64(argv[1]);
    
    // Save Context
    __asm__ volatile ("addi sp, sp, -64");
    __asm__ volatile ("sw x1,  0(sp)");
    __asm__ volatile ("sw x5,  4(sp)");
    __asm__ volatile ("sw x6,  8(sp)");
    __asm__ volatile ("sw x7,  12(sp)");
    __asm__ volatile ("sw x10, 16(sp)");
    __asm__ volatile ("sw x11, 20(sp)");
    __asm__ volatile ("sw x12, 24(sp)");
    __asm__ volatile ("sw x13, 28(sp)");
    __asm__ volatile ("sw x14, 32(sp)");
    __asm__ volatile ("sw x15, 36(sp)");
    __asm__ volatile ("sw x16, 40(sp)");
    __asm__ volatile ("sw x17, 44(sp)");
    __asm__ volatile ("sw x28, 48(sp)");
    __asm__ volatile ("sw x29, 52(sp)");
    __asm__ volatile ("sw x30, 56(sp)");
    __asm__ volatile ("sw x31, 60(sp)");


    __asm__ volatile ("nop");
    __asm__ volatile ("mv  a0, %0" :: "r"(entry));
    __asm__ volatile ("jalr ra, 0(a0)");
    __asm__ volatile ("nop");
    
    
    // Restore Context
    __asm__ volatile ("lw x1,  0(sp)");
    __asm__ volatile ("lw x5,  4(sp)");
    __asm__ volatile ("lw x6,  8(sp)");
    __asm__ volatile ("lw x7,  12(sp)");
    __asm__ volatile ("lw x10, 16(sp)");
    __asm__ volatile ("lw x11, 20(sp)");
    __asm__ volatile ("lw x12, 24(sp)");
    __asm__ volatile ("lw x13, 28(sp)");
    __asm__ volatile ("lw x14, 32(sp)");
    __asm__ volatile ("lw x15, 36(sp)");
    __asm__ volatile ("lw x16, 40(sp)");
    __asm__ volatile ("lw x17, 44(sp)");
    __asm__ volatile ("lw x28, 48(sp)");
    __asm__ volatile ("lw x29, 52(sp)");
    __asm__ volatile ("lw x30, 56(sp)");
    __asm__ volatile ("lw x31, 60(sp)");
    __asm__ volatile ("addi sp, sp, 64");
}

char man_run [] = 
"Start Executing a file from specified address\n"
"Usage:  run [entry addr (in hex)]\n";

#endif //__MEMTEST_H__