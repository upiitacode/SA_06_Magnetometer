#ifndef _I2C_STM32F3_H
#define _I2C_STM32F3_H

#ifdef __cplusplus
extern "C" {
#endif

#define USE_REGISTER_ADDRESS 1
#define IGNORE_REGISTER_ADDRESS 0

#define DONT_CARE 0

void i2c1_init(void);

int i2c1_writeRaw(unsigned char address7b,unsigned char regAddress, unsigned char* pData, int nData, int useRegAddress);

int i2c1_write(unsigned char address7b,unsigned char regAddress, unsigned char* pData, int nData);

int i2c1_writeRegister(unsigned char address7b, unsigned char regAddress, unsigned char data);

int i2c1_read(unsigned char address7b,unsigned char regAddress, unsigned char* pData, int nData);

void i2c2_init(void);

int i2c2_writeRaw(unsigned char address7b,unsigned char regAddress, unsigned char* pData, int nData, int useRegAddress);

int i2c2_write(unsigned char address7b,unsigned char regAddress, unsigned char* pData, int nData);

int i2c2_writeRegister(unsigned char address7b, unsigned char regAddress, unsigned char data);

int i2c2_read(unsigned char address7b,unsigned char regAddress, unsigned char* pData, int nData);

void i2c3_init(void);

int i2c3_writeRaw(unsigned char address7b,unsigned char regAddress, unsigned char* pData, int nData, int useRegAddress);

int i2c3_write(unsigned char address7b,unsigned char regAddress, unsigned char* pData, int nData);

int i2c3_writeRegister(unsigned char address7b, unsigned char regAddress, unsigned char data);

int i2c3_read(unsigned char address7b,unsigned char regAddress, unsigned char* pData, int nData);

#ifdef __cplusplus
}
#endif

#endif// _I2C_STM32F3_H
