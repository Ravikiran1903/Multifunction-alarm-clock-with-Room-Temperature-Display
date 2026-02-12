#include<lpc21xx.h>
#define buzzer_pin 30

#define buzzer_on() IOSET0=(1<<buzzer_pin)
#define buzzer_off() IOCLR0=(1<<buzzer_pin)
