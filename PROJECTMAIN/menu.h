//menu.h

#include "types.h"
extern s32 g_alarm_hour;
extern s32 g_alarm_minute;	
extern s32 g_alarm_second;
extern s32 g_alarm_active;
extern volatile u32 g_alarm_ringing;

void interrupt_lcd(void);
void menu(void);
void edit_rtc(void);
u32 get_validated_input(u8 row, u8 col,u8 max_digits);
void set_alarm(void);
