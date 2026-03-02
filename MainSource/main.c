/*
 * File:   main.c
 */

#include <xc.h>
#include "clcd.h"
#include "micro_oven.h"
#include "matrix_keypad.h"
#include "timers.h"

#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)
    unsigned char screen_flag = POWER_SCREEN;

extern unsigned char min,sec;

static void init_config(void) {
    init_clcd();
    init_matrix_keypad();
    init_timer2();
    //Configure fan
    FAN_DDR = 0;
    GIE  = 1;   // Global Interrupt Enable
    PEIE = 1;  // Peripheral Interrupt Enable

}

void main(void) {
    init_config();
    unsigned char key;
    unsigned char reset_flag;

    while (1) {
        //update the screen flag based on key press
        key = read_matrix_keypad(STATE);//return the pressed key
        if(screen_flag == MENU_SCREEN)
        {
            if(key == 1){
                screen_flag = MICRO_MODE;
                clear_screen();
                clcd_print("Power = 900W",LINE2(2));
                __delay_ms(3000);
                clear_screen();
                reset_flag = RESET_FLAG;
            }
            else if(key == 2){
                screen_flag = GRILL_MODE;
                reset_flag = RESET_FLAG;
                clear_screen();
            }
            else if(key == 3){
                screen_flag = CONVECTION_MODE;
                reset_flag = RESET_FLAG;
                clear_screen();
            }
            else if(key == 4){
                screen_flag = START_MODE;
                clear_screen();
            }            
        }
        //4-start,5-pause,6-stop
        else if(screen_flag == DISPLAY_TIME)
        {
            if(key == 4)
            {
                sec = sec + 30;
                if(sec>60)
                {
                    min++;
                    sec = sec-60;
                }
            }
            if(key == 5)
            {
                screen_flag = PAUSE;
            }
            else if(key == 6)
            {
                screen_flag = STOP;
            }
        }
        else if(screen_flag == PAUSE)
        {
            if(key == 4)
            {
                //start cooing
                TMR2ON = 1;
                FAN = 1;
                screen_flag = DISPLAY_TIME ;
            }
        }
        
        switch( screen_flag){
        // implement power screen
            case POWER_SCREEN :
                                display_power_screen();
                                clear_screen();
                                screen_flag = MENU_SCREEN;
                                break;
                                
            case MENU_SCREEN :
                                display_modes();
                                break;
                                
            case MICRO_MODE :
                                set_time(key, reset_flag);
                                break;
            case DISPLAY_TIME :
                                display_time();
                                break;
            case GRILL_MODE :
                                set_time(key,reset_flag);
                                break;
            case CONVECTION_MODE :
                //allow user to set temp.
                                set_temp(key,reset_flag);
                                break;
                               
            case START_MODE :
                                heat_food();
                                break;

                
            case PAUSE :
                                TMR2ON =0;
                                FAN = 0;
                                break;
            case STOP:
                                TMR2ON = 0;
                                FAN = 0;
                                clear_screen();
                                screen_flag = MENU_SCREEN;
                                break;
               
             
                            
        }
        reset_flag = RESET_NOTHING ;
        
    }
    return;

}
