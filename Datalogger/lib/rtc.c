#include "rtc.h"

# include <util/delay.h>

uint8_t ds1307_get(uint8_t reg)
{
	static uint8_t temp;
	DS1307Read(reg ,&temp); // Чтение регистра
	
	return (((temp & 0xF0) >> 4)*10)+(temp & 0x0F);
}

void time_change(uint8_t time, uint8_t reg)
{
	time++; // Увеличиваем минуты на 1
	switch(reg)
	{
		case 0x00: { if(time>59) time=0; break;} 
		case 0x01: { if(time>59) time=0; break;}
		case 0x02: { if(time>23) time=0; break;}
		case 0x03: { if(time>7)  time=1; break;}
		case 0x04: { if(time>29) time=1; break;}
		case 0x05: { if(time>12) time=1; break;}
		case 0x06: { if(time>99) time=0; break;}
	}
	// Преобразуем из двоичной системы в BCD и записываем в DS1307
	uint8_t temp;
	temp = ((time/10) << 4)|(time%10);
	DS1307Write(reg, temp);
	_delay_ms(10);
}
