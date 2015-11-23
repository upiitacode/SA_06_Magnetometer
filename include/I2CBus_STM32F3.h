#ifndef _I2CBUS_STM32F3_H
#define _I2CBUS_STM32F3_H

#include "I2CBus.h"

class I2CBus1 : public I2CBus{
	private:
		int isBusy(void);
	public:
		I2CBus1(void);
		virtual void writeByte(unsigned char  address_7b, unsigned char data);
		virtual unsigned char readByte(unsigned char address_7b);

		virtual void writeByteAt(unsigned char  address_7b, unsigned char regAddress, unsigned char data);
		virtual unsigned char readByteAt(unsigned char address_7b, unsigned char regAddress);

		virtual void writePacket(unsigned char  address_7b, unsigned char  *pData, int nData);
		virtual void readPacket(unsigned char  address_7b, unsigned char *pData, int nData);

		virtual void writePacketAt(unsigned char  address_7b,unsigned char regAddress, unsigned char  *pData, int nData);
		virtual void readPacketAt(unsigned char  address_7b, unsigned char regAddress, unsigned char *pData, int nData);
};

class I2CBus2 : public I2CBus{
	private:
		int isBusy(void);
	public:
		I2CBus2(void);
		virtual void writeByte(unsigned char  address_7b, unsigned char data);
		virtual unsigned char readByte(unsigned char address_7b);

		virtual void writeByteAt(unsigned char  address_7b, unsigned char regAddress, unsigned char data);
		virtual unsigned char readByteAt(unsigned char address_7b, unsigned char regAddress);

		virtual void writePacket(unsigned char  address_7b, unsigned char  *pData, int nData);
		virtual void readPacket(unsigned char  address_7b, unsigned char *pData, int nData);

		virtual void writePacketAt(unsigned char  address_7b,unsigned char regAddress, unsigned char  *pData, int nData);
		virtual void readPacketAt(unsigned char  address_7b, unsigned char regAddress, unsigned char *pData, int nData);
};

class I2CBus3 : public I2CBus{
	private:
		int isBusy(void);
	public:
		I2CBus3(void);
		virtual void writeByte(unsigned char  address_7b, unsigned char data);
		virtual unsigned char readByte(unsigned char address_7b);

		virtual void writeByteAt(unsigned char  address_7b, unsigned char regAddress, unsigned char data);
		virtual unsigned char readByteAt(unsigned char address_7b, unsigned char regAddress);

		virtual void writePacket(unsigned char  address_7b, unsigned char  *pData, int nData);
		virtual void readPacket(unsigned char  address_7b, unsigned char *pData, int nData);

		virtual void writePacketAt(unsigned char  address_7b,unsigned char regAddress, unsigned char  *pData, int nData);
		virtual void readPacketAt(unsigned char  address_7b, unsigned char regAddress, unsigned char *pData, int nData);
};

#endif// _I2CBUS_STM32F3_H
