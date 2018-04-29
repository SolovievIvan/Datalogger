#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "lib/lcd_lib.h"
#include "lib/rtc.h"
#include <avr/eeprom.h>
#include "lib/atmega-adc.h"


uint8_t hour, minute, second, day, month, week, year;


void setup(void);
void int1_init(void);
void get_time(void);
void write_time(void);
//void initADC();


//uint16_t ReadADC(uint8_t ADCport);
//uint16_t analog_value = 0;

int button,buttonA,buttonB,buttonC;
unsigned char arr[8];
unsigned char i;
char num_buffer[10];


int main(void)
{


	setup(); //настройка



	 LCD4_gotoxy(1,1);
	 LCD4_write_string("Diplom");
	 LCD4_gotoxy(2, 9);
	 LCD4_write_string("Ivan S.");
	 _delay_ms(500);
	 
	 
	while(1)
	{
	

		for (i = 0; i<8; i++) 
 		 {

			 
			LCD4_clear();

			uint8_t k = adc_read(ADC_PRESCALER_128, ADC_VREF_AVCC, i) / 4;

			itoa(k, num_buffer, 10);
		//	LCD4_write_string(num_buffer);

		//	LCD4_write(' ');
		//	LCD4_write('0' + i);

			arr[i] = k;

			_delay_ms(1000);
			

  }




			switch (button)
		{

				case 1:
				{
					LCD4_clear();
					LCD4_gotoxy(1,1);
					 LCD4_write_string("knopka1");
					_delay_ms(2000);

				button = 0;
				}
 		 
 		 
 			 break;
				case 2:
				{
					LCD4_clear();
					LCD4_gotoxy(1,1);
					itoa(arr[1], num_buffer, 10);
					 LCD4_write_string(num_buffer);
					_delay_ms(2000);

				

				button = 0;
				}
 		 
 		 
 		 	break;
		 		case 3:
				{
					LCD4_clear();
					LCD4_gotoxy(1,1);
					itoa(arr[2], num_buffer, 10);
					 LCD4_write_string(num_buffer);
					_delay_ms(2000);


				button = 0;
				}
 		 
 		 
 			 break;
		 		case 4:
				{


				button = 0;
				}
 		 
 		 
 			 break;
		 		case 5:
				{


				button = 0;
				}
 		 
 		
 			 break;
		 		case 6:
				{

				button = 0;
				}
 		 
 		
 			 break;
		 		case 7:
				{

				button = 0;
				}
 		 
 		 
 			 break;




			default:

			button = 0;
 	
 			break;
		}






	 

_delay_ms(300);




//	get_time();
//	write_time();



	}
	
	return 0;
}

void setup(void)
{
	
	sei();

	DDRA = 0x00;
	DDRD = 0x00;
	DDRB=(1<<DDB2) | (1<<DDB1) | (1<<DDB0); 

	
	PORTA = 0x00;
	PORTD = 0x00;


//	initADC(); //инициализация АЦП

	LCD4_init(); // Инициализация дисплея для работы 4-х битовом режиме
	
	I2CInit(); // Инициализация шины I2C

	int1_init(); //инициализация прерывания по нажатию кнопок

	

	// Запускаем ход часов
	uint8_t t_start;
	DS1307Read(0x00, &t_start);
	t_start &= ~(1 << 7); // обнуляем 7ой бит
	DS1307Write(0x00,t_start);

	get_time();


}




ISR(INT1_vect)
{
	
		//опрашиваем порты шифратора, узнаём, какая комбинация пришла на входы D0-D2	
	  	buttonA=(!(PIND & (1 << 2))) ; 
		buttonB=(!(PIND & (1 << 1)));
		buttonC=(!(PIND & (1 << 0))) ;
		button = buttonA | (buttonB << 1) | (buttonC << 2);


	//	LCD4_clear();
	//	LCD4_gotoxy(1,1);
	//	LCD4_write('0' + button);
	//	_delay_ms(2000);
	
}






void get_time(void) // Получить всё время сразу
{
	second = ds1307_get(0x00); // 00,01,02... - номера регистров, в которых хранятся данные о времени в микросхеме часов
	minute = ds1307_get(0x01);
	hour   = ds1307_get(0x02);
	
	week   = ds1307_get(0x03);
	
	day    = ds1307_get(0x04);
	month  = ds1307_get(0x05);
	year   = ds1307_get(0x06);
}



void write_time(void)
{
		
		_delay_ms(200);
		LCD4_clear();
		LCD4_gotoxy(1,1);
		LCD4_write('0' + hour / 10);
		LCD4_write('0' + hour % 10);
		LCD4_write(':');
	   	LCD4_write('0' + minute / 10);
		LCD4_write('0' + minute % 10);
	 	LCD4_write(':');
	 	LCD4_write('0' + second / 10);
		LCD4_write('0' + second % 10);
		_delay_ms(50);

}


void int1_init(void)
 {
 	
 	MCUCR |= (1 << ISC01)|(0 << ISC00);
 
 	GICR |= (1 << INT1);
}
