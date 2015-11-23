#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "delay.h"
#include "SerialStream_stm32f3.h"
#include "I2CBus_STM32F3.h"

extern uint32_t SystemCoreClock;
void tarea1(void const * arguments); //tarea 1
osThreadId  tarea1ID;	//identificador del hilo tarea 1
osThreadDef (tarea1,osPriorityNormal,1,0);// macro para definir tareas (aputandor de la funcion, prioridad,?,?)

void tarea2(void const * arguments); //tarea 2
osThreadId  tarea2ID;	//identificador del hilo  tarea 2
osThreadDef (tarea2,osPriorityNormal,1,0);// macro para definir tareas (aputandor de la funcion, prioridad,?,?)

void tarea1Init(void);//funcion que iniciliza la tarea1
void tarea2Init(void);//funcion que iniciliza la tarea1

void osInitAll(){
	osKernelInitialize();
	tarea1Init();
	tarea2Init();
	osKernelStart();
}

#define MPU6050_ADDRESS 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_AXEL_XOUT 0x3B

int main(){
	unsigned char rxData[4];
	SerialUSART2 serial(9600);
	serial.printf("\nEl dinero es dinero nano desu\n");
	serial.printf("core speed %u\n",(unsigned int) SystemCoreClock);
	I2CBus1 i2c;
	serial.printf("i2c intialized\n");
	//User application
	i2c.writeByteAt(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1,0x00);
	while(1){
		i2c.readPacketAt(MPU6050_ADDRESS, MPU6050_AXEL_XOUT, rxData, 2);
		serial.printf("x = %d\n", rxData[0]);
		delay_ms(500);
	}
}

void tarea1Init(void){
	tarea1ID= osThreadCreate(osThread(tarea1),NULL);
}

void tarea2Init(void){
	tarea2ID= osThreadCreate(osThread(tarea2),NULL);
}

void tarea1(void const * arguments){
	while(1){
		osDelay(1000);
	}
}

void tarea2(void const * arguments){
	while(1){
		osDelay(1000);
	}
}
