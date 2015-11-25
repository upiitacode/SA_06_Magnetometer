#include "MainApp.h"
#include "delay.h"

#define MAG_ADDRESS 0x1E

#define MAG_CONF_H 0x00
#define MAG_CONF_L 0x01
#define MAG_MODE 0x02
#define MAG_OUT_X_H 0x03
#define MAG_OUT_X_L 0x04
#define MAG_OUT_Z_H 0x05
#define MAG_OUT_Z_L 0x06
#define MAG_OUT_Y_H 0x07
#define MAG_OUT_Y_L 0x08


void magnetometer_init(I2CBus* i2c_bus){
	i2c_bus->writeByteAt(MAG_ADDRESS,MAG_MODE,0x00);
}

int16_t magnetometerData[3];

void magnetometerRead(I2CBus* i2c_bus){
	i2c_bus->readPacketAt(MAG_ADDRESS,MAG_OUT_X_H,(uint8_t *) magnetometerData,6);
	//obtener datos, calcular arco tangente
}

void compass_read(void){
}

void MainApp::main(SerialStream* serial, I2CBus* i2c_bus){
	serial->printf("\nStarting main application\n");
	magnetometer_init(i2c_bus);
	while(1){
		magnetometerRead(i2c_bus);
		serial->printf("x=%6d y=%6d z=%6d\n",magnetometerData[0],magnetometerData[2],magnetometerData[1]);
		delay_ms(100);
	}
}
