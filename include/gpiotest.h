#ifndef __GPIOTEST_H__
#define __GPIOTEST_H__

#include <gpio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int fn_gpio_setmode(int argc, char **argv)
{
    int pin = atoi(argv[1]);
    if(!strcmp(argv[2], "input"))
        gpio_setmode(pin, INPUT);
    else if (!strcmp(argv[2], "output"))
        gpio_setmode(pin, OUTPUT);
    else
        printf("Invalid pin mode: %s\n", argv[2]);
}

char man_gpio_setmode [] = 
"Set mode of a GPIO pin\n"
"Usage: > gpio_setmode [pin] [input/output]\n";



int fn_gpio_getmode(int argc, char **argv)
{
    int pin = atoi(argv[1]);
    (gpio_getmode(pin)==INPUT) ? puts("input\n") : puts("output\n");
}

char man_gpio_getmode [] = 
"Get mode of a GPIO pin\n"
"Usage: > gpio_getmode [pin]\n";



int fn_gpio_read(int argc, char **argv)
{
    int pin = atoi(argv[1]);
    gpio_read(pin) ? puts("high\n") : puts("low\n");
}

char man_gpio_read [] = 
"Read the specified GPIO pin\n"
"Usage: > gpio_read [pin]>\n";



int fn_gpio_write(int argc, char **argv)
{
    int pin = atoi(argv[1]);
    if(!strcmp(argv[2], "high"))
        gpio_write(pin, HIGH);
    else if (!strcmp(argv[2], "low"))
        gpio_write(pin, LOW);
    else
        printf("Invalid state: %s\n", argv[2]);
}

char man_gpio_write [] = 
"Write to specified GPIO pin\n"
"Usage: > gpio_write [pin] [high/low]\n";



int fn_gpio_tone(int argc, char **argv)
{
    // 1st arg: pin
    int pin = atoi(argv[1]);

    // 2nd arg: freq
    int freq = atoi(argv[2]);

    // 3rd arg: duration
    int duration = atoi(argv[3]);

    printf("Generating a tone of %d hz on pin %d for %d seconds...   ", freq, pin, duration);

    // period in ms
    int period = 1000/freq;
    unsigned int reqd_cycles = (duration*1000)/period;

    // save gpio pin direction
    gpio_direction old_dirn = gpio_getmode(pin);
    if(old_dirn == INPUT)
        gpio_setmode(pin, OUTPUT);  // change to output

    gpio_write(pin, LOW);
    for(unsigned int i=0; i<reqd_cycles; i++)   // gen square wave
    {
        gpio_write(pin, HIGH);
        sleep(period/2);
        gpio_write(pin, LOW);
        sleep(period/2);
    }

    // restore gpio pin direction
    if(old_dirn == INPUT)
        gpio_setmode(pin, INPUT);
    
    puts("done!\n");
}

char man_gpio_tone [] = 
"Generate tone of specified frequency\n"
"Usage: > gpio_tone [pin] [freq(in hz)] [duration (in s)]>\n";


#endif //__GPIOTEST_H__
