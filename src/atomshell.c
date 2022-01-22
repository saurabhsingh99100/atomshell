#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <serial.h>
#include <gpio.h>
#include <lcd.h>

// command buffer size
#define CMDBUFSZ 64

// console prompt
char prompt [] = "\33[36mroot> \33[0m";

///////////////////////////////////////////////////////
// Global variables

typedef struct 
{
    bool echo_tokens;
    bool rebooted;
}
GlobalVars;

GlobalVars global_vars = 
{
    .echo_tokens=false,
    .rebooted=false
};

///////////////////////////////////////////////////////
// Commands

// Union for pointer to different function types
typedef union
{
    int (*funptr0)();
    int (*funptr1)(int argc, char ** argv);
} fprototypes;

// Command Entry
typedef struct
{
    char name[20];
    int expected_args;
    int ftype;
    char * man;
    fprototypes ptr;
} Command;

Command *functions_lut_ptr = NULL;
unsigned int functions_lut_sz = 0;

// Include other headers
#include "data.h"
#include "util.h"
#include "commands.h"

#include <math.h>

int console()
{
    while(!global_vars.rebooted)
    {
        // Initialize buffer
        char cmdbuf [CMDBUFSZ];
        memset(cmdbuf, 0, sizeof(cmdbuf));

        // Get command
        printf("%s", prompt);
        gets(cmdbuf, sizeof(cmdbuf), 1, prompt);

        // Tokenize command
        char *argv[8];
        int   argc;
        for(argc=0;argc<8 && (argv[argc]=strtok(argc==0?cmdbuf:NULL," "));argc++)
        {
            if(global_vars.echo_tokens) printf("argv[%d] = [%s]\n",argc,argv[argc]);
        }

        // no command entered
        if(strlen(cmdbuf)==0)
            continue;

        // Call corresponding function
        int retcode = -1;
        
        unsigned int fnum=0;
        bool found=false;
        for(fnum=0; fnum<functions_lut_sz; fnum++)
        {    
            if(!strcmp(functions_lut[fnum].name, argv[0]))
            {
                // Match!
                found=true;

                // Check expected argc
                if(functions_lut[fnum].expected_args>=0)
                {
                    if(argc-1<functions_lut[fnum].expected_args)
                    {
                        printf("Too few arguments for command \"%s\"\n for help, check man <command>\n", argv[0]);
                        break;
                    }
                    else if(argc-1>functions_lut[fnum].expected_args)
                    {
                        printf("Too many arguments for command \"%s\"\n for help, check man <command>\n", argv[0]);
                        break;
                    }
                }

                switch (functions_lut[fnum].ftype)
                {
                    case 0: retcode=functions_lut[fnum].ptr.funptr0(); break;
                    case 1: retcode=functions_lut[fnum].ptr.funptr1(argc, argv); break;
                    
                default:
                    break;
                }
            }

            if(found)
                break;
        }
        if(found==false)
            printf("Unrecognized Command \"%s\"\n", argv[0]);
    }
}


int main()
{
    // Initializations
    functions_lut_ptr = functions_lut;
    functions_lut_sz = sizeof(functions_lut)/sizeof(Command);
    
    serial_init(B_9600);
    fn_clear();

    // Boot sequence
    printf("Booting.");
    gpio_init();    putchar('.');
    lcd_init();     putchar('.');


    printf(" Press ENTER to continue...\n");
    while(getchar()!='\r')
    {/* wait */}

    // Print Banner
    RLEdecode(banner, 942);

    printf("Welcome to RISCV-Atom!\n");
    printf("Type \"help\" to show a list of all available commands\n");
    
    // Jump to console
    int code = console();
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               