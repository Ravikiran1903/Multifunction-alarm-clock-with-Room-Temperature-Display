//lcd.c

#include <LPC21xx.H>
#include "types.h"
#include "defines.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "delay.h"//lcd.c

void writeLCD(u8 byte)
{
	//select write operation
	IOCLR0=1<<LCD_RW;
	//write byte to lcd data pins
	WRITEBYTE(IOPIN0,LCD_DATA,byte);
	//provide high to low pulse
	IOSET0=1<<LCD_EN;
	delayus(1);
	IOCLR0=1<<LCD_EN;
	delayms(2);
}

void cmdLCD(u8 cmdByte)
{
	//set rs pin for cmd/inst reg
  IOCLR0=1<<LCD_RS;
  //write cmd byte to cmd reg
  writeLCD(cmdByte); 	
}

void Init_LCD(void)
{
	//cfg p0.8(d0) to p0.15(d7),
	//p0.16(rs),p0.17(rw),p0.18(en) 
	//as gpio output pins
	IODIR0|=((0xFF<<LCD_DATA)|
	         (1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN));
	
	//power on delay
	delayms(15);
	cmdLCD(0x30);
	delayms(4);
	delayus(100);
	cmdLCD(0x30);
	delayus(100);
	cmdLCD(0x30);
	cmdLCD(MODE_8BIT_2LINE);
	cmdLCD(DSP_ON_CUR_ON);
	cmdLCD(CLEAR_LCD);
	cmdLCD(SHIFT_CUR_RIGHT);
}

void charLCD(u8 asciiVal)
{
	//sel data reg
	IOSET0=1<<LCD_RS;
	//write ascii value via data reg to ddram
	writeLCD(asciiVal);
}

void strLCD(s8 *str)
{
	while(*str)
		charLCD(*str++);
}

void u32LCD(u32 n)
{
	s32 i=0;
	u8 a[10];
	
	if(n==0)
	{
		charLCD('0');
	}
	else
	{
	  while(n>0)
    {
		  a[i++]=(n%10)+48;
      n/=10;			
    }
    for(--i;i>=0;i--)
      charLCD(a[i]);		
	}
}

void s32LCD(s32 num)
{
	if(num<0)
	{
		charLCD('-');
		num=-num;
	}
	u32LCD(num);
}

void F32LCD(f32 fnum,u32 nDP)
{
	u32 num,i;
	if(fnum<0.0)
	{
		charLCD('-');
		fnum=-fnum;
	}
	num=fnum;
	u32LCD(num);
	charLCD('.');
	for(i=0;i<nDP;i++)
	{
		fnum=(fnum-num)*10;
		num=fnum;
		charLCD(num+48);
	}
}

void buildCGRAM(u8 *p,u8 nBytes)
{
	u32 i;
	//point to cgram start
	cmdLCD(GOTO_CGRAM_START);
	//select data reg
	IOSET0=1<<LCD_RS;
	
	for(i=0;i<nBytes;i++)
	{
		//write to cgram vi data reg
		writeLCD(p[i]);
	}
	//point back to ddram start/display
	cmdLCD(GOTO_LINE1_POS0);
}
