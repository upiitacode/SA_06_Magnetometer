#include "MainApp.h"
#include "delay.h"

#define MPU6050_ADDRESS 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_AXEL_XOUT 0x3B

void MainApp::main(SerialStream* serial, I2CBus* i2c_bus){
	serial->printf("\nStarting main application\n");
	unsigned char rxData[4];
	i2c_bus->writeByteAt(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1,0x00);
	while(1){
		i2c_bus->readPacketAt(MPU6050_ADDRESS, MPU6050_AXEL_XOUT, rxData, 2);
		serial->printf("x = %d\n", rxData[0]);
		delay_ms(500);
	}
}
