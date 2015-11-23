#ifndef _I2CBUS_H
#define _I2CBUS_H

class I2CBus{
	public:
		virtual ~I2CBus(){};
		virtual void writeByte(unsigned char  address_7b, unsigned char data) = 0;
		virtual unsigned char readByte(unsigned char address_7b) = 0;

		virtual void writeByteAt(unsigned char  address_7b, unsigned char regAddress, unsigned char data) = 0;
		virtual unsigned char readByteAt(unsigned char address_7b, unsigned char regAddress) = 0;

		virtual void writePacket(unsigned char  address_7b, unsigned char  *pData, int nData) = 0;
		virtual void readPacket(unsigned char  address_7b, unsigned char *pData, int nData) = 0;

		virtual void writePacketAt(unsigned char  address_7b,unsigned char regAddress, unsigned char  *pData, int nData) = 0;
		virtual void readPacketAt(unsigned char  address_7b, unsigned char regAddress, unsigned char *pData, int nData) = 0;
};

#endif// _I2CBUS_H
