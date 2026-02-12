//project_main.c

#include <LPC21xx.H>
#include "types.h"

#include "lcd.h"
#include "rtc.h"
#include "delay.h"
#include "kpm.h"
#include "menu.h"
#include "lcd_defines.h"
#include "buzzer.h"
#include "adc.h"


volatile u32 Int_flag=0;
volatile u32 g_alarm_ringing=0;
s32 g_alarm_hour = -1; // -1 means "not set"
s32 g_alarm_minute = -1;
s32 g_alarm_second = -1;
s32 g_alarm_enabled = 0;     // 1 = alarm is armed
//volatile u32 g_alarm_ringing = 0; // 1 = alarm is ringing

f32 eAR;
u32 adcval;

u8 alarmsymbol[8]={
0x0E,0x1F,0x11,0x1B,0x11,0x1F,0x0A,0x00
};

int main(void)
{
	s32 hour,minute,second;
	s32 date,month,year,day;
	Init_LCD();
	buildCGRAM(alarmsymbol,8);
	RTC_Init();
	Init_KPM();
	Init_ADC();
	interrupt_lcd();
	//Rtc_alarm_Init();
	
	IODIR0|=(1<<buzzer_pin);
	buzzer_off();
	//cmdLCD(CLEAR_LCD);
	//strLCD("system reday...");
	//delayms(1000);
	while(1)
	{
		if (g_alarm_ringing == 1) 
        {
            buzzer_on();
            cmdLCD(GOTO_LINE2_POS0 + 15);
            charLCD(0); // Show symbol

            if (ColScan() == 0) // Key press to silence
            {
                g_alarm_ringing = 0;
                buzzer_off();
                // g_soft_alarm_enabled = 0; // Already disabled
            }
        }
		else if(Int_flag==1)
		{
			menu();
		}
		else
		{
		 GetRTCTimeInfo(&hour,&minute,&second);
     GetRTCDateInfo(&date,&month ,&year);
		 GetRTCDay(&day);
		 DisplayRTCTime(hour,minute,second);
     DisplayRTCDate(date,month,year);
		 DisplayRTCDay(day);
		 Read_ADC(1,&adcval,&eAR);

		 cmdLCD(GOTO_LINE1_POS0+10);
		// cmdLCD(GOTO_LINE2_POS0);

		 u32LCD(eAR*100);
		 charLCD(0xDF);
		 charLCD('c');
		 delayms(1000);
		 cmdLCD(DSP_ON_CUR_OFF);
		 if (g_alarm_enabled == 1)
            {
                if (hour == g_alarm_hour && 
                    minute == g_alarm_minute && 
                    second == g_alarm_second)
                {
                    g_alarm_ringing = 1; // Start ringing
                    g_alarm_enabled = 0; // Disarm alarm
                }
            }

            // Logic to show/hide alarm symbol
            cmdLCD(GOTO_LINE2_POS0 + 15);
            if (g_alarm_enabled == 1)
            {
                charLCD(0); // Show alarm symbol
            }
            else
            {
                charLCD(' '); // Show blank space
            }
        }
    }
}
