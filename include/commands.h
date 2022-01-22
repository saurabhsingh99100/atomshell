#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "data.h"

///////////////////////////////////////////////////////////
// System Functions

int fn_man(int argc, char **argv)
{
    for(unsigned int i=0; i<functions_lut_sz; i++)
    {
        if(!strcmp(argv[1], functions_lut_ptr[i].name))
        {
            // Match!
            printf("================ Man Page for %s ================\n", argv[1]);
            printf("%s\n", functions_lut_ptr[i].man);
            return 0;
        }
    }
    printf("Unrecognized Command \"%s\"\n", argv[1]);
}

char man_man [] = 
"Show manual for command\n"
"Usage: > man <command>\n";



int fn_help()
{
    printf("Available commands:\n");
    for(unsigned int i=0; i<functions_lut_sz; i++)
    {
        printf("%s", functions_lut_ptr[i].name);
        for(int l=strlen(functions_lut_ptr[i].name); l<21; l++) putchar(' ');
        // printf(": %s\n", functions_lut_ptr[i].man);
        
        char * ptr = functions_lut_ptr[i].man;
        
        while(*ptr != '\n') // 1st line
            putchar(*ptr++);

        while(*ptr) // other lines
        {
            if(*(ptr-1)=='\n' && ptr!=functions_lut_ptr[i].man)
                for(int m=0; m<25; m++) putchar(' ');
            putchar(*ptr);
            
            ptr++;
        }
        putchar('\n');
    }
    
}

char man_help [] = 
"Show all available commands\n";



int fn_clear()
{
    printf("\33[H\33[2J");
}

char man_clear [] =
"Clear Screen\n";



int fn_exit(int argc, char ** argv)
{
    if(argc>1)
    {
        int wait = atoi(argv[1]);
        printf("Shutting Down in %d seconds", wait);
    
        while(wait--)
        {
            putchar('.');
            sleep(50);
        }
        putchar('\n');
    }
    else
        printf("Shutting Down\n");
    global_vars.rebooted=true;
    return 0;
}

char man_exit [] =
"Shut Down system\n"
"Usage: > exit [time*]\n"
"*argument is optional\n";



int fn_echotokens(int argc, char **argv)
{
    if(!strcmp(argv[1], "true"))
        global_vars.echo_tokens=true;
    else if(!strcmp(argv[1], "false"))
        global_vars.echo_tokens=false;
    else
        printf("Invalid argument: %s\n", argv[1]);
            
    return 0;
}

char man_echotokens [] =
"Echo command tokens\n"
"Usage: > echotokens [true/false]\n";



int fn_echo(int argc, char **argv)
{
    for(int i=1; i<argc; i++)
    {
        if(argv[i][0] != '$')
            printf("%s ", argv[i]);
        else
        {
            if(!strcmp(argv[i], "$banner"))
            {
                RLEdecode(banner, 942);
            }
            else 
                printf("Unknown system variable \"%s\"", argv[i]);
        }
    }
    putchar('\n');
}

char man_echo [] =
"Echo provided string\n";



int fn_sysinfo()
{
    printf("=== Sysinfo ==================================\n");
    printf("System:\n");
    printf("\tArch       :  %s\n", systeminfo.arch);
    printf("\tByte Order :  %s\n", systeminfo.byte_ordering);
    printf("\tCPU        :  %s\n", systeminfo.cpu);
    printf("\tBoard      :  %s\n", systeminfo.board);
    printf("\tClock      :  %s\n", systeminfo.clkfreq);
    printf("\nMemory Map:\n");
    for(int i=0; i<(sizeof(systemmem)/sizeof(MemDevice)); i++)
    {
        puts("\t0x"); puthex(systemmem[i].base, 8, false); puts("-0x"); 
        puthex(systemmem[i].base+systemmem[i].sz, 8, false); 
        printf(" : %s\n", systemmem[i].name);
    }
    printf("==============================================\n");
}

char man_sysinfo [] = 
"Print System info and memory map\n";



///////////////////////////////////////////////////////////
// User Functions
#include "spi-flash.h"
#include "lcdtest.h"
#include "gpiotest.h"
#include "memtest.h"


// The array with all the functions.
Command functions_lut[] = 
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // System commands

    {.name="man",           .expected_args=1,   .ftype=1,   .man=man_man,               .ptr.funptr1=fn_man},
    {.name="help",          .expected_args=0,   .ftype=0,   .man=man_help,              .ptr.funptr0=fn_help},
    {.name="clear",         .expected_args=0,   .ftype=0,   .man=man_clear,             .ptr.funptr0=fn_clear},
    {.name="exit",          .expected_args=-1,  .ftype=1,   .man=man_exit,              .ptr.funptr1=fn_exit},
    {.name="echotokens",    .expected_args=1,   .ftype=1,   .man=man_echotokens,        .ptr.funptr1=fn_echotokens},
    {.name="sysinfo",       .expected_args=0,   .ftype=0,   .man=man_sysinfo,           .ptr.funptr0=fn_sysinfo},
    {.name="echo",          .expected_args=-1,  .ftype=1,   .man=man_echo,              .ptr.funptr1=fn_echo},
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // User commands
    
    // LCD
    {.name="lcd_init",      .expected_args=0,   .ftype=0,   .man=man_lcd_init,          .ptr.funptr0=fn_lcd_init},
    {.name="lcd_clear",     .expected_args=0,   .ftype=0,   .man=man_lcd_clear,         .ptr.funptr0=fn_lcd_clear},
    {.name="lcd_write",     .expected_args=-1,  .ftype=1,   .man=man_lcd_write,         .ptr.funptr1=fn_lcd_write},
    {.name="lcd_setcursor", .expected_args=2,   .ftype=1,   .man=man_lcd_setcursor,     .ptr.funptr1=fn_lcd_setcursor},
    
    // GPIO test
    {.name="gpio_setmode",  .expected_args=2,   .ftype=1,   .man=man_gpio_setmode,      .ptr.funptr1=fn_gpio_setmode},
    {.name="gpio_getmode",  .expected_args=1,   .ftype=1,   .man=man_gpio_getmode,      .ptr.funptr1=fn_gpio_getmode},
    {.name="gpio_read",     .expected_args=1,   .ftype=1,   .man=man_gpio_read,         .ptr.funptr1=fn_gpio_read},
    {.name="gpio_write",    .expected_args=2,   .ftype=1,   .man=man_gpio_write,        .ptr.funptr1=fn_gpio_write},
    {.name="gpio_tone",     .expected_args=3,   .ftype=1,   .man=man_gpio_tone,         .ptr.funptr1=fn_gpio_tone},

    // Memtest
    {.name="peek",          .expected_args=1,   .ftype=1,   .man=man_peek,              .ptr.funptr1=fn_peek},
    {.name="poke",          .expected_args=2,   .ftype=1,   .man=man_poke,              .ptr.funptr1=fn_poke},
    {.name="load",          .expected_args=2,   .ftype=1,   .man=man_load,              .ptr.funptr1=fn_load},
    {.name="run",           .expected_args=1,   .ftype=1,   .man=man_run,               .ptr.funptr1=fn_run},
    
    // SPI
    {.name="spiflash_init", .expected_args=0,   .ftype=0,   .man=man_spiflash_init,     .ptr.funptr0=fn_spiflash_init},
    {.name="spiflash_read", .expected_args=2,   .ftype=1,   .man=man_spiflash_read,     .ptr.funptr1=fn_spiflash_read}
};

#endif //__COMMANDS_H__