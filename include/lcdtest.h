#ifndef __LCDTEST_H__
#define __LCDTEST_H__

#include <lcd.h>
#include <stdlib.h>

int fn_lcd_init()
{
    lcd_init();
}

char man_lcd_init [] = 
"Initialize LCD screen\n";



int fn_lcd_clear()
{
    lcd_clear();
}

char man_lcd_clear [] = 
"Clear LCD screen\n";



int fn_lcd_write(int argc, char **argv)
{
    int i=1;
    while(i<argc)
    {
        lcd_putstr(argv[i]); lcd_putchar(' ');
        i++;
    }
}

char man_lcd_write [] = 
"Write a string on LCD string\n"
"Usage: > lcd_write [string]\n";



int fn_lcd_setcursor(int argc, char **argv)
{
    int row = atoi(argv[1]);
    int col = atoi(argv[2]);
    lcd_setCursor(row, col);
}

char man_lcd_setcursor [] = 
"Set cursor on lcd screen\n"
"Usage: > lcd_setcursor [row] [coloumn]\n";


#endif // __LCDTEST_H__
