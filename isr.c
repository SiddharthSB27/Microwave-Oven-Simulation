#include <xc.h>

extern unsigned char min,sec;
extern unsigned char pre_heat;

void __interrupt() isr(void)
{
    static unsigned int count = 0;
    
    if (TMR2IF == 1)
    {    
        if (++count == 20000) // 1sec
        {
            count = 0;
            if(sec >0){
                sec--;
            }
            if(sec == 0){
                if(min>0){
                 min--;
                 sec = 60;
                }
            }
            if(pre_heat > 0)
            {
                pre_heat--;
            }
        }
        
        TMR2IF = 0;
    }
}