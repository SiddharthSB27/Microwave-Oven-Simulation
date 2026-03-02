/*
 * File:   micro_oven.c
 * Author: balaj
 *
 * Created on 21 January, 2026, 11:44 PM
 */

//PORTb,PORTd,TRISb are predefined as address
#include <xc.h>
//turn off WDT
#include "clcd.h"
#include "micro_oven.h"
#include "matrix_keypad.h"
//#pragma config WDTE = OFF
unsigned char min,sec;
unsigned int temp;
extern unsigned char screen_flag ;
extern unsigned char reset_flag ;
unsigned char pre_heat;

void display_power_screen(void )
{
    for(unsigned char i = 0;i<16;i++)
    {
        clcd_putch(BAR ,LINE1(i));
        
        __delay_ms(100);
    }
    clcd_print("Powering ON", LINE2(3));
    clcd_print("Microwave Oven",LINE3(2));
    for(unsigned char i = 0;i<16;i++)
    {
        clcd_putch(BAR ,LINE4(i));
         __delay_ms(100);

    }
   __delay_ms(1000);
}

void display_modes(void)
{
        clcd_print("1.MICRO MODE", LINE1(3));
        clcd_print("2.GRILL MODE", LINE2(3));
        clcd_print("3.CONVECTION MODE", LINE3(3));
        clcd_print("4.START MODE", LINE4(3));


}
void set_time(unsigned char key,unsigned char reset_flag)
{
    static unsigned char blink_pos = 0;
    static unsigned char key_count = 0;
    static unsigned char delay , display_blank;
    if( reset_flag == RESET_FLAG){
        min = 0;
        sec = 0;
        key = 0;
        key_count = 0;
        blink_pos = 0;
    }
    //PRINTING THE FORMAT
    clcd_print("Set Time (MM:SS)",LINE1(0));
    clcd_print("TIME: ", LINE2(0));
    clcd_print(":",LINE2(8));
    clcd_print("*:CLEAR,#:ENTER",LINE4(0));
    
    //read the time from keypad,set time
    if (key != '*' && key != '#' && key != ALL_RELEASED) // then key = 2,1,2,3,4,5,6,7,8,9
    {
        //blink_pos = 0 then seconds if 1 then minute
        key_count++;
        if(key_count <= 2)
        {
            sec = sec*10 + key;
            blink_pos = 0;
        }
        else if(key<=4)//key_count more than 2 for minutes
        {
            min = min*10 + key;
            blink_pos = 1;
        
        }
    }
    //* clear time
    else if(key == '*')
    {
        if(blink_pos == 0)
        {
            sec = 0;
            key_count = 0;
        }
        else if(blink_pos == 1)
        {
            min = 0;
            key_count = 2;
        
        }
    
    }
    //#start cooking
    else if(key == '#')
    {
        clear_screen();
        // turn on fan
        FAN = ON;
        //turn on timer
        TMR2ON = 1;
        //change screen to display time
        screen_flag = DISPLAY_TIME ;
    }
    
    if(delay++ == 5)
    {  
        delay = 0;
        display_blank = !display_blank;
     }
    if(display_blank)
    {
        if(blink_pos == 0)
        {
            clcd_print(" ", LINE2(9));
            clcd_print(" ", LINE2(10));

        }
        else
        {
            clcd_print(" ", LINE2(6));
            clcd_print(" ", LINE2(7));
        }
    
    }
    else
    {
        clcd_putch(sec / 10 + '0',LINE2(9));
        clcd_putch(sec % 10 + '0',LINE2(10));
        
        clcd_putch(min / 10 + '0',LINE2(6));
        clcd_putch(min % 10 + '0',LINE2(7));

    }
}
void set_temp(unsigned  char key,unsigned char reset_flag)
{
    static unsigned char key_count = 0;
    static unsigned char delay , display_blank,blink_pos;
    if( reset_flag == RESET_FLAG){
        temp = 0;
        key = 0;
        key_count = 0;
        blink_pos = 0;
    }
     if(delay++ == 5)
    {  
        delay = 0;
        display_blank = !display_blank;
     }
    //PRINTING THE FORMAT
    clcd_print("Set Temp (*C)",LINE1(0));
    clcd_print("TEMP:", LINE2(0));
    clcd_print("*:CLEAR,#:ENTER",LINE4(0));

    
    //read the temp.
    
       if (key != '*' && key != '#' && key != ALL_RELEASED) // then key = 2,1,2,3,4,5,6,7,8,9
    {
        //blink_pos = 0 then seconds if 1 then minute
        key_count++;
        if(key_count <= 4)//3 times we read temp.
        {
            temp = temp*10 + key;
        }

    }
    //* clear time
    else if(key == '*')
    {
        temp = 0;
        key_count = 0;
    
    }
    //#start cooking
    else if(key == '#')
    {
        if(temp > 180)
        {
            temp = 180;
        }
        clear_screen();
        //display preheating
        pre_heat = 60;
        //TURN TIMER ON
        TMR2ON = 1;
            //implement pre heating.
        clcd_print("Pre-heating",LINE1(3));
        clcd_print("Time Rem: ",LINE3(0));
        clcd_putch('s',LINE3(12));

        while(pre_heat)
        {
        clcd_putch((pre_heat) / 100 + '0',LINE3(9));
        clcd_putch((pre_heat/10) % 10 + '0',LINE3(10));
        clcd_putch((pre_heat%10) + '0',LINE3(11));
        }
        TMR2ON = 0;
        clear_screen();
        screen_flag = MICRO_MODE;

    }
    
    
    if(display_blank)
    {
            clcd_print(" ", LINE2(7));      
            clcd_print(" ", LINE2(8));
            clcd_print(" ", LINE2(9));       
    
    }
    else
    {
        clcd_putch(temp / 100 + '0',LINE2(7));
        clcd_putch((temp/10) % 10 + '0',LINE2(8));
        clcd_putch((temp%10) + '0',LINE2(9));
    }
    



}

void display_time(void)
{
    clcd_print("TIME = ",LINE1(1));
        clcd_putch(sec / 10 + '0',LINE1(12));
        clcd_putch(sec % 10 + '0',LINE1(13));
        clcd_print(":",LINE1(11));
        clcd_putch(min / 10 + '0',LINE1(9));
        clcd_putch(min % 10 + '0',LINE1(10));
        clcd_print("4.START/RESUME",LINE2(2));
        clcd_print("5.PAUSE",LINE3(2));
        clcd_print("6.STOP",LINE4(2));
        
        if(min == 0 && sec==0)
        {
            clear_screen();
            TMR2ON = OFF;
            FAN= 0;
            clcd_print("TIME UP!!!",LINE2(4));
            clcd_print("ENJOY YOUR MEAL",LINE3(0));
            __delay_ms(2000);
            clear_screen();
            __delay_ms(2000);
            screen_flag = MENU_SCREEN;
        }

}
void heat_food(void)
{
    sec = 30;
    min = 0;
    TMR2ON = 1;
    FAN = 1;
    screen_flag = DISPLAY_TIME;
    
}


/*void init_config() {

}

void main(void) {
    init_config();
    while (1) {

    }

}*/
