//lcd.h


#include "types.h"
void Init_LCD(void);
void writeLCD(u8 byte);
void cmdLCD(u8 cmd);
void charLCD(u8 asciiVal);
void strLCD(s8 *);

void u32LCD(u32);
void s32LCD(s32);
void f32LCD(f32,u32);
void buildCGRAM(u8 *,u8 );
