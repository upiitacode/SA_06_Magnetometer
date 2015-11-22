#include "i2c_stm32f3.h"
#include "stm32f30x.h"                  // Device header

void i2c1_init(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Mode = GPIO_Mode_AF;
	myGPIO.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;  
	myGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOB, &myGPIO);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9, GPIO_AF_4);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C_InitTypeDef myI2C;
	I2C_StructInit(&myI2C);

	#define PRESC 3
	#define SCLL 0x13
	#define SCLH 0xF
	#define SDADEL 0x2
	#define	SCLDEL 0x04 

	myI2C.I2C_Ack = I2C_Ack_Enable;
	myI2C.I2C_Mode = I2C_Mode_I2C;
	myI2C.I2C_Timing = (PRESC << 28) | (SCLDEL << 20) | (SDADEL << 16) | (SCLH << 8) | (SCLL << 0);
	I2C_Init(I2C1,&myI2C);
	I2C_StretchClockCmd(I2C1,ENABLE);
	I2C_Cmd(I2C1, ENABLE);
}

int i2c1_write(char address7b,char* pData, int nData){
	I2C_MasterRequestConfig(I2C1, I2C_Direction_Transmitter);
	I2C_10BitAddressingModeCmd(I2C1,DISABLE);
	I2C_SlaveAddressConfig(I2C1,address7b<<1);
	I2C_NumberOfBytesConfig(I2C1,nData);
	I2C_ReloadCmd(I2C1,DISABLE);
	I2C_AutoEndCmd(I2C1, DISABLE);
	I2C_GenerateSTART(I2C1, ENABLE);
	//I2C_TransferHandling(I2C1,address7b<<1,nData,I2C_AutoEnd_Mode,I2C_Generate_Start_Write);
	for(int i = 0; i < nData; i++){
		while(1){
			if(I2C_GetFlagStatus(I2C1,I2C_FLAG_NACKF)){
				return 0;
			}
			else if(I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE)){
				I2C_SendData(I2C1,pData[i]);
				break;
			}
		}
	}
	//while(!I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE));
	while(!I2C_GetFlagStatus(I2C1,I2C_FLAG_TC));
	I2C_GenerateSTOP(I2C1,ENABLE);
	return 1;
}

int i2c1_read(char address7b,char regAddress, char* pData, int nData){
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
	
	I2C_MasterRequestConfig(I2C1, I2C_Direction_Transmitter);
	I2C_10BitAddressingModeCmd(I2C1,DISABLE);
	I2C_SlaveAddressConfig(I2C1,address7b<<1);
	I2C_NumberOfBytesConfig(I2C1,1);
	I2C_ReloadCmd(I2C1,DISABLE);
	I2C_AutoEndCmd(I2C1, DISABLE);
	I2C_GenerateSTART(I2C1, ENABLE);
	
	while(1){
		if(I2C_GetFlagStatus(I2C1,I2C_FLAG_NACKF)){
			return 0;
		}
		else if(I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE)){
			I2C_SendData(I2C1,regAddress);
			break;
		}
	}
	
	while(!I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE));
	
	I2C_MasterRequestConfig(I2C1, I2C_Direction_Receiver);
	I2C_NumberOfBytesConfig(I2C1,nData);
	I2C_10BitAddressingModeCmd(I2C1,DISABLE);
	I2C_GenerateSTART(I2C1,ENABLE);
	
	while(1){
		if(I2C_GetFlagStatus(I2C1,I2C_FLAG_NACKF)){
			return 0;
		}
		if(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE)){
			break;
		}
	}
	
	for(int i = 0; i < nData; i++){
		while(!I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE));
			pData[i] = I2C_ReceiveData(I2C1);
	}
	I2C_GenerateSTOP(I2C1,ENABLE);
	return 1;
}
