#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

// i2c

void I2CInit(void);
void I2CStart(void);
void I2CStop(void);
uint8_t I2CWriteByte(uint8_t data);
uint8_t I2CReadByte(uint8_t *data,uint8_t ack);
uint8_t DS1307Read(uint8_t address,uint8_t *data);
uint8_t DS1307Write(uint8_t address,uint8_t data);


// rtc

uint8_t ds1307_get(uint8_t reg);
void    time_change(uint8_t time, uint8_t reg);
