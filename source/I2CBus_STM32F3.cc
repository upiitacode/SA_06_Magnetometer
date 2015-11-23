#include "I2CBus_STM32F3.h"
#include "i2c_stm32f3.h"                    // Device header

I2CBus1::I2CBus1(void){
	i2c1_init();
}

void I2CBus1::writeByte(unsigned char  address_7b, unsigned char data){
	unsigned char txData[1];
	i2c1_writeRaw(address_7b,DONT_CARE,txData,1,IGNORE_REGISTER_ADDRESS);
}

unsigned char I2CBus1::readByte(unsigned char address_7b){
	return 0;
}

void I2CBus1::writeByteAt(unsigned char  address_7b, unsigned char regAddress, unsigned char data){
	i2c1_writeRegister(address_7b, regAddress, data);
}

unsigned char I2CBus1::readByteAt(unsigned char address_7b, unsigned char regAddress){
	unsigned char rxData[1];
	i2c1_read(address_7b, regAddress, rxData, 1);
	return rxData[0];
}

void I2CBus1::writePacket(unsigned char  address_7b, unsigned char  *pData, int nData){
	i2c1_writeRaw(address_7b, DONT_CARE, pData, nData,IGNORE_REGISTER_ADDRESS);
}

void I2CBus1::readPacket(unsigned char  address_7b, unsigned char *pData, int nData){
}

void I2CBus1::writePacketAt(unsigned char  address_7b,unsigned char regAddress, unsigned char  *pData, int nData){
	i2c1_write(address_7b, regAddress, pData, nData);
}
void I2CBus1::readPacketAt(unsigned char  address_7b, unsigned char regAddress, unsigned char *pData, int nData){
	i2c1_read(address_7b, regAddress, pData, nData);
}
