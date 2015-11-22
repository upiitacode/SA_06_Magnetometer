#ifndef _I2C_STM32F3_H
#define _I2C_STM32F3_H

#ifdef __cplusplus
extern "C" {
#endif

void i2c1_init(void);

int i2c1_write(char address7b,char* pData, int nData);

int i2c1_read(char address7b,char regAddress, char* pData, int nData);

#ifdef __cplusplus
}
#endif

#endif// _I2C_STM32F3_H
