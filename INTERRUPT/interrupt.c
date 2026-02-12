//interrupt.c

#include "types.h"
#include <LPC21xx.h>
#include "pin_function_defines.h"
#include "pin_connect_block.h"
#include "rtc.h"
#include "delay.h"
#include "lcd.h"
#include "menu.h"
#include "kpm.h"
#include "lcd_defines.h"
#include "buzzer.h"

#define EINT1_VIC_CHANNEL 15
extern volatile u32 Int_flag;

extern s32 g_alarm_hour;
extern s32 g_alarm_minute;
extern s32 g_alarm_second;
extern s32 g_alarm_enabled;
extern volatile u32 g_alarm_ringing;


void eint1_isr(void) __irq;	
void interrupt_lcd(void)
{
	CfgPortPinFunc(0,3,PIN_FUNC4);  //p0.3
	
	
	//cfg VIC peripheral:
	//1. cfg eint0,eint1 interrupt type as irq
	//   def are irq
	//   VICIntSelect = 0;
	//2. enable eint0,eint1 in VIC 
	VICIntEnable |= 1<<EINT1_VIC_CHANNEL;
	//3. cfg eint0 for v.irq slot higest priority
	VICVectCntl0 = (1<<5)|EINT1_VIC_CHANNEL;
	//4. load eint0_isr addr in reg
	VICVectAddr0 = (u32 )eint1_isr;
	EXTMODE|= (1<<1);
	EXTPOLAR&=~(1<<1);
}

void eint1_isr(void) __irq
{
	Int_flag=1;
	//clear eint0 status in External Interrupt Peripheral
	EXTINT=1<<1;
	//clear eint0 status in VIC peripheral
	VICVectAddr=0;
}
u32 get_validated_input(u8 row, u8 col,u8 max_digits)
{
	/*
	'0'-'9' =numbers
	'c' =  backspace
	'*' =key 13 (backspace)
	'#'  = (enter)
	*/
	u8 key;
	u32 value=0;
	u32 digits_entered=0;
	u32 start_pos=(row==1)?(GOTO_LINE1_POS0+col):(GOTO_LINE2_POS0+col);
	cmdLCD(start_pos);
	while(1)
	{
		key=KeyScan();
		if(key>='0'&&key<='9')
		{
			if(digits_entered<max_digits)
			{
				value=(value*10)+(key-'0');
			  charLCD(key);
		    digits_entered++;
		   }
		 }
		else if(key=='D')
		{
			if(digits_entered>0)
			{
				value=value/10;
				digits_entered--;
				cmdLCD(start_pos+digits_entered);
				charLCD(' ');
				cmdLCD(start_pos+digits_entered);
			}
		}
		else if (key=='#')
		{
			return value;
		}
	}
}
		
			
void menu(void)
{
	u8 key;
	cmdLCD(CLEAR_LCD);
	while(Int_flag)
   {
			cmdLCD(0x01);
			cmdLCD(GOTO_LINE1_POS0);
			strLCD("1.alarm setting");
			cmdLCD(GOTO_LINE2_POS0);
			strLCD("2.alarm time 3.exit");
			do
			{
			key=KeyScan();
			}while(key==0);
			switch(key)
			{
				case '1': edit_rtc();
	                  cmdLCD(CLEAR_LCD);
                     break;
				case '2'	: set_alarm();
                    cmdLCD(CLEAR_LCD);
                    break;
				case '3'	: Int_flag=0;
                    break;
					}
				}
   cmdLCD(CLEAR_LCD);
}
void edit_rtc(void)
{
	s32 t_hrs,t_min,t_sec,date,month,year,day;
	u8 key;
	GetRTCTimeInfo( &t_hrs, &t_min, &t_sec);
	GetRTCDateInfo( &date, &month,  &year);
	while(1)
	{
		cmdLCD(CLEAR_LCD);
		strLCD("1.time 2.date");
		cmdLCD(GOTO_LINE2_POS0);
		strLCD("3.day 4.exit");
		do
    {
		key=KeyScan();
		}while(key==0);
		switch (key)
		{
			case '1' :  cmdLCD(CLEAR_LCD);
			             strLCD("set_time(HH:MM:SS)");
		               cmdLCD(GOTO_LINE2_POS0);
			             strLCD("HH:");	
                   t_hrs=(get_validated_input(2,3,2));
                   cmdLCD(GOTO_LINE2_POS0+5);
			             strLCD("MM:");
                   t_min=(get_validated_input(2,8,2));
                   cmdLCD(GOTO_LINE2_POS0+10);
			             strLCD("SS:");
			             t_sec=(get_validated_input(2,13,2));
                   if(t_hrs>23||t_min>59||t_sec>59)
									 {
										 cmdLCD(CLEAR_LCD);
										 strLCD("invalid time!");
									 }
									 else
									 {
										 SetRTCTimeInfo(t_hrs,t_min,t_sec);
										 cmdLCD(CLEAR_LCD);
										 strLCD("time_updated!");
									 }
									 delayms(1000);
									 break;
		

			case '2'	:  cmdLCD(CLEAR_LCD);
									 strLCD("set_date(DD:MM:YYYY)");
                   cmdLCD(GOTO_LINE2_POS0);
									 strLCD("DD:");
									 date=(get_validated_input(2,3,2));
                   cmdLCD(GOTO_LINE2_POS0+6);
			             strLCD("MM:");
                   month=(get_validated_input(2,9,2));
                   //cmdLCD(GOTO_LINE2_POS0+12);
									 cmdLCD(CLEAR_LCD);
									 strLCD("set date(DD:MM:YYYY)");
									 cmdLCD(GOTO_LINE2_POS0);
	                 strLCD("yyyy:");
			             year=(get_validated_input(2,5,4));
                   if(date>31||month<1||month>12||year<2000)
									 {
										 cmdLCD(CLEAR_LCD);
										 strLCD("invalid date!");
									 }
									 else
									 {
										 SetRTCDateInfo(date,month,year);
										 cmdLCD(CLEAR_LCD);
										 strLCD("date_updated!");
									 }
									 break;
		
			case '3'   :             cmdLCD(CLEAR_LCD);
									 strLCD("Setday(0-6)");
									 cmdLCD(GOTO_LINE2_POS0);
									 strLCD("day: ");
									 day=get_validated_input(2,5,1);
									 if(day>6)
									 {
										 cmdLCD(CLEAR_LCD);
										 strLCD("invalid day!");
									 }
									 else
									 {
										 SetRTCDay(day);
										 cmdLCD(CLEAR_LCD);
										 strLCD("day updated!");
									 }
									 delayms(1000);
									 break;		 
			case '4'	 :		return;		 
		}
	}
}
void set_alarm(void)
{
    u8 key;
    s32 hrs, min, sec;
    while (1)
    {
        cmdLCD(CLEAR_LCD);
        strLCD("1.set time 2.Enable");
        cmdLCD(GOTO_LINE2_POS0);
        strLCD("3.Disable 4.exit");
        
        do
        {
            key = KeyScan();
        } while (key == 0);
        
        switch (key)
        {
            case '1': // Set Alarm Time
                cmdLCD(CLEAR_LCD);
                strLCD("set_time(HH:MM:SS)");
                cmdLCD(GOTO_LINE2_POS0);
                strLCD("HH:");
                hrs = (get_validated_input(2, 3, 2));
                cmdLCD(GOTO_LINE2_POS0 + 5);
                strLCD("MM:");
                min = (get_validated_input(2, 8, 2));
                cmdLCD(GOTO_LINE2_POS0 + 10);
                strLCD("SS:");
                sec = (get_validated_input(2, 13, 2));
                
                if (hrs > 23 || min > 59 || sec > 59)
                {
                    cmdLCD(CLEAR_LCD);
                    strLCD("invalid time!");
                    // Invalidate the time
                    g_alarm_hour = -1;
                }
                else
                {
                    // Save to our global variables
                    g_alarm_hour = hrs;
                    g_alarm_minute = min;
                    g_alarm_second = sec;
                    
                    cmdLCD(CLEAR_LCD);
                    strLCD("ALARM TIME SET!");
                }
                delayms(1000);
                break;

            case '2': // Enable Alarm
                if (g_alarm_hour == -1)
                {
                    cmdLCD(CLEAR_LCD);
                    strLCD("Set time first!");
                }
                else
                {
                    g_alarm_ringing = 0;
                    buzzer_off();
                    g_alarm_enabled = 1; // Arm the alarm
                    cmdLCD(CLEAR_LCD);
                    strLCD("ALArm enabled");
                }
                delayms(1000);
                break;

            case '3': // Disable Alarm
                g_alarm_ringing = 0;
                buzzer_off();
                g_alarm_enabled = 0; // Disarm the alarm
                cmdLCD(CLEAR_LCD);
                strLCD("Alarm disabled");
                delayms(1000);
                break;

            case '4':
                return;
        }
    }
}
