#ifndef _MAINAPP_H
#define _MAINAPP_H

#include "SerialStream.h"
#include "I2CBus.h"

class MainApp{
	public:
		static void main(SerialStream* serial, I2CBus* i2c_bus);
};

#endif// _MAINAPP_H
