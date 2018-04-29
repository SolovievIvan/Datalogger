#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "rtc.h"


// ������� ������������� ���� TWI
void I2CInit(void)
{
	// ��������� TWI ������
	TWBR = 2;
	TWSR = (1 << TWPS1)|(1 << TWPS0); // ������������ �� 64
	TWCR |= (1 << TWEN); // ��������� ������ TWI
}

void I2CStart(void)
{
	// �������� ������� �����
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTA);
	// �������� ��������� ����� TWINT
	while(!(TWCR & (1 << TWINT)));
}

void I2CStop(void)
{
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO); // �������� ������� ����
	while(TWCR & (1 << TWSTO)); // �������� ���������� �������� ������� ����
}

// ������� ������ ������ �� ����
uint8_t I2CWriteByte(uint8_t data)
{
	TWDR = data; // �������� ������ � TWDR 
	TWCR = (1 << TWEN)|(1 << TWINT); // ����� ����� TWINT ��� ������ �������� ������
	while(!(TWCR & (1 << TWINT))); // �������� ���������� ��������
	// �������� �������
	if((TWSR & 0xF8) == 0x18 || (TWSR & 0xF8) == 0x28 || (TWSR & 0xF8) == 0x40)
	{
		// ���� ����� DS1307, ���� R/W � ������ ��������
		// � �������� �������������
		return 1;
	}
	else
		return 0; // ������
}

// ������� ������ ������ �� ����
uint8_t I2CReadByte(uint8_t *data,uint8_t ack)
{
	if(ack) // ������������� �������������
	{
		// ���������� ������������� ����� ������
		TWCR |= (1 << TWEA);
	}
	else
	{
		// ���������� ��������������� ����� ������
		// ������� ���������� �� �������� ������ ������
		// ������ ������������ ��� ������������� ���������� �����
		TWCR &= ~(1 << TWEA);
	}
	// ���������� ������ ������ ����� ������ TWINT
	TWCR |= (1 << TWINT);
	while(!(TWCR & (1 << TWINT))); // �������� ��������� ����� TWINT
	// �������� �������
	if((TWSR & 0xF8) == 0x58 || (TWSR & 0xF8) == 0x50)
	{
		// ����� ������ � ����������� �������������
		//  ���
		// ����� ������ � ����������� ���������������
		*data = TWDR; // ������ ������
		return 1;
	}
	else
		return 0; // ������
}
// ������� ������ ������ �� DS1307
uint8_t DS1307Read(uint8_t address,uint8_t *data)
{
	uint8_t res; // ���������
	I2CStart(); // �����
	res = I2CWriteByte(0b11010000); // ����� DS1307 + ��� W
	if(!res)    return 0; // ������
	// �������� ������ ������������ ��������
	res = I2CWriteByte(address);
	if(!res)    return 0; // ������
	I2CStart(); // ��������� ����� 
	res = I2CWriteByte(0b11010001); // ����� DS1307 + ��� R
	if(!res)    return 0; // ������
	// ������ ������ � ����������������
	res = I2CReadByte(data,0);
	if(!res)    return 0; // ������
	I2CStop(); // ����
	return 1;
}
// ������� ������ ������ � DS1307

uint8_t DS1307Write(uint8_t address,uint8_t data)
{
	uint8_t res; // ���������
	I2CStart(); // �����
	res = I2CWriteByte(0b11010000); // ����� DS1307 + ��� W
	if(!res)    return 0; // ������
	// �������� ������ ������������ ��������
	res = I2CWriteByte(address);
	if(!res)    return 0; // ������
	res = I2CWriteByte(data); // ������ ������
	if(!res)    return 0; // ������
	I2CStop(); // ����
	return 1;
}
