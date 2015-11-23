#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "SerialStream_stm32f3.h"
#include "I2CBus_STM32F3.h"
#include "MainApp.h"

void tarea1(void const * arguments); //tarea 1
osThreadId  tarea1ID;	//identificador del hilo tarea 1
osThreadDef (tarea1,osPriorityNormal,1,0);// macro para definir tareas (aputandor de la funcion, prioridad,?,?)

void tarea1Init(void);//funcion que iniciliza la tarea1

void osInitAll(){
	osKernelInitialize();
	tarea1Init();
	osKernelStart();
}

int main(){
	SerialUSART2 serial(9600);
	I2CBus1 i2c;
	//User application
	MainApp::main(&serial, &i2c);
	return 0;
}

void tarea1Init(void){
	tarea1ID= osThreadCreate(osThread(tarea1),NULL);
}

void tarea1(void const * arguments){
	while(1){
		osDelay(1000);
	}
}
