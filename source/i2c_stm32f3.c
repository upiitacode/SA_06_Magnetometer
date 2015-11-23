#include "i2c_stm32f3.h"
#include "stm32f30x.h"                  // Device header

#define I2C_STATUS_FAIL 0
#define I2C_STATUS_SUCCESS 1

#define PRESC 3
#define SCLL 0x13
#define SCLH 0xF
#define SDADEL 0x2
#define	SCLDEL 0x04

void i2c1_init(void){
	// Configure gpio pins for i2c operation PB9:SDA PB8:SCL
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

	// Configure I2C
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C_InitTypeDef myI2C;
	I2C_StructInit(&myI2C);
	myI2C.I2C_Ack = I2C_Ack_Enable;
	myI2C.I2C_Mode = I2C_Mode_I2C;
	myI2C.I2C_Timing = (PRESC << 28) | (SCLDEL << 20) | (SDADEL << 16) | (SCLH << 8) | (SCLL << 0);//Timing for 100KHz operation
	I2C_Init(I2C1,&myI2C);
	I2C_StretchClockCmd(I2C1,ENABLE);
	I2C_Cmd(I2C1, ENABLE);
}

int i2c1_write(char address7b, char regAddress, char* pData, int nData){
	return i2c1_writeRaw(address7b,regAddress,pData,nData,1);
}

int i2c1_writeRegister(char address7b, char regAddress, char data){
	char txData[1] = {data};
	return i2c1_writeRaw(address7b,regAddress,txData,1,1);
}

int i2c1_writeRaw(char address7b,char regAddress,char* pData, int nData,int useRegAddress){
	// Set i2c in tramission mode
	I2C_MasterRequestConfig(I2C1, I2C_Direction_Transmitter);
	I2C_10BitAddressingModeCmd(I2C1,DISABLE);
	I2C_SlaveAddressConfig(I2C1,address7b<<1);
	I2C_NumberOfBytesConfig(I2C1,nData+useRegAddress);
	I2C_ReloadCmd(I2C1,DISABLE);
	I2C_AutoEndCmd(I2C1, DISABLE);

	// Send start signal
	I2C_GenerateSTART(I2C1, ENABLE);

	//Send register address
	if(useRegAddress){
		while(1){
			if(I2C_GetFlagStatus(I2C1,I2C_FLAG_NACKF)){// If nack recivied exit with error
				return I2C_STATUS_FAIL;
			}
			else if(I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE)){
				I2C_SendData(I2C1,regAddress);
				break;
			}
		}
	}

	//Send data
	for(int i = 0; i < nData; i++){
		while(1){
			if(I2C_GetFlagStatus(I2C1,I2C_FLAG_NACKF)){// If nack recivied exit with error
				return I2C_STATUS_FAIL;
			}
			else if(I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE)){
				I2C_SendData(I2C1,pData[i]);
				break;
			}
		}
	}

	while(!I2C_GetFlagStatus(I2C1,I2C_FLAG_TC));//Wait until trasmission complete

	//Set stop signal
	I2C_GenerateSTOP(I2C1,ENABLE);
	return I2C_STATUS_SUCCESS;
}

int i2c1_read(char address7b,char regAddress, char* pData, int nData){
	// Wait while busy
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));

	//Configure bytes transmission
	I2C_MasterRequestConfig(I2C1, I2C_Direction_Transmitter);
	I2C_10BitAddressingModeCmd(I2C1,DISABLE);//7 Bit mode
	I2C_SlaveAddressConfig(I2C1,address7b<<1);
	I2C_NumberOfBytesConfig(I2C1,1);
	I2C_ReloadCmd(I2C1,DISABLE);
	I2C_AutoEndCmd(I2C1, DISABLE);

	// Send start signal
	I2C_GenerateSTART(I2C1, ENABLE);

	// Wait  until ready to transmittin
	while(1){
		if(I2C_GetFlagStatus(I2C1,I2C_FLAG_NACKF)){// If nack return fail
			return 0;
		}
		else if(I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE)){
			I2C_SendData(I2C1,regAddress);
			break;
		}
	}

	// Wait until transmission done
	while(!I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE));

	//Change to receiver mode
	I2C_MasterRequestConfig(I2C1, I2C_Direction_Receiver);
	I2C_NumberOfBytesConfig(I2C1,nData);
	I2C_10BitAddressingModeCmd(I2C1,DISABLE);

	// Send restart signal
	I2C_GenerateSTART(I2C1,ENABLE);

	// Wait until data is recived or nack is detected
	while(1){
		if(I2C_GetFlagStatus(I2C1,I2C_FLAG_NACKF)){// Device not found
			return I2C_STATUS_FAIL;
		}
		if(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE)){
			break;
		}
	}

	// read data
	for(int i = 0; i < nData; i++){
		while(!I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE));
			pData[i] = I2C_ReceiveData(I2C1);
	}

	//Send stop signal
	I2C_GenerateSTOP(I2C1,ENABLE);
	return I2C_STATUS_SUCCESS;
}

void i2c2_init(void){
	// Configure gpio pins for i2c operation PA10:SDA PA9:SCL
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Mode = GPIO_Mode_AF;
	myGPIO.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9;
	myGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOA, &myGPIO);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_4);

	// Configure I2C
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	I2C_InitTypeDef myI2C;
	I2C_StructInit(&myI2C);
	myI2C.I2C_Ack = I2C_Ack_Enable;
	myI2C.I2C_Mode = I2C_Mode_I2C;
	myI2C.I2C_Timing = (PRESC << 28) | (SCLDEL << 20) | (SDADEL << 16) | (SCLH << 8) | (SCLL << 0);//Timing for 100KHz operation
	I2C_Init(I2C2,&myI2C);
	I2C_StretchClockCmd(I2C2,ENABLE);
	I2C_Cmd(I2C2, ENABLE);
}

int i2c2_write(char address7b, char regAddress, char* pData, int nData){
	return i2c2_writeRaw(address7b,regAddress,pData,nData,1);
}

int i2c2_writeRegister(char address7b, char regAddress, char data){
	char txData[1] = {data};
	return i2c2_writeRaw(address7b,regAddress,txData,1,1);
}

int i2c2_writeRaw(char address7b,char regAddress,char* pData, int nData,int useRegAddress){
	// Set i2c in tramission mode
	I2C_MasterRequestConfig(I2C2, I2C_Direction_Transmitter);
	I2C_10BitAddressingModeCmd(I2C2,DISABLE);
	I2C_SlaveAddressConfig(I2C2,address7b<<1);
	I2C_NumberOfBytesConfig(I2C2,nData+useRegAddress);
	I2C_ReloadCmd(I2C2,DISABLE);
	I2C_AutoEndCmd(I2C2, DISABLE);

	// Send start signal
	I2C_GenerateSTART(I2C2, ENABLE);

	//Send register address
	if(useRegAddress){
		while(1){
			if(I2C_GetFlagStatus(I2C2,I2C_FLAG_NACKF)){// If nack recivied exit with error
				return I2C_STATUS_FAIL;
			}
			else if(I2C_GetFlagStatus(I2C2,I2C_FLAG_TXE)){
				I2C_SendData(I2C2,regAddress);
				break;
			}
		}
	}

	//Send data
	for(int i = 0; i < nData; i++){
		while(1){
			if(I2C_GetFlagStatus(I2C2,I2C_FLAG_NACKF)){// If nack recivied exit with error
				return I2C_STATUS_FAIL;
			}
			else if(I2C_GetFlagStatus(I2C2,I2C_FLAG_TXE)){
				I2C_SendData(I2C2,pData[i]);
				break;
			}
		}
	}

	while(!I2C_GetFlagStatus(I2C2,I2C_FLAG_TC));//Wait until trasmission complete

	//Set stop signal
	I2C_GenerateSTOP(I2C2,ENABLE);
	return I2C_STATUS_SUCCESS;
}

int i2c2_read(char address7b,char regAddress, char* pData, int nData){
	// Wait while busy
	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));

	//Configure bytes transmission
	I2C_MasterRequestConfig(I2C2, I2C_Direction_Transmitter);
	I2C_10BitAddressingModeCmd(I2C2,DISABLE);//7 Bit mode
	I2C_SlaveAddressConfig(I2C2,address7b<<1);
	I2C_NumberOfBytesConfig(I2C2,1);
	I2C_ReloadCmd(I2C2,DISABLE);
	I2C_AutoEndCmd(I2C2, DISABLE);

	// Send start signal
	I2C_GenerateSTART(I2C2, ENABLE);

	// Wait  until ready to transmittin
	while(1){
		if(I2C_GetFlagStatus(I2C2,I2C_FLAG_NACKF)){// If nack return fail
			return 0;
		}
		else if(I2C_GetFlagStatus(I2C2,I2C_FLAG_TXE)){
			I2C_SendData(I2C2,regAddress);
			break;
		}
	}

	// Wait until transmission done
	while(!I2C_GetFlagStatus(I2C2,I2C_FLAG_TXE));

	//Change to receiver mode
	I2C_MasterRequestConfig(I2C2, I2C_Direction_Receiver);
	I2C_NumberOfBytesConfig(I2C2,nData);
	I2C_10BitAddressingModeCmd(I2C2,DISABLE);

	// Send restart signal
	I2C_GenerateSTART(I2C2,ENABLE);

	// Wait until data is recived or nack is detected
	while(1){
		if(I2C_GetFlagStatus(I2C2,I2C_FLAG_NACKF)){// Device not found
			return I2C_STATUS_FAIL;
		}
		if(I2C_GetFlagStatus(I2C2,I2C_FLAG_RXNE)){
			break;
		}
	}

	// read data
	for(int i = 0; i < nData; i++){
		while(!I2C_GetFlagStatus(I2C2,I2C_FLAG_RXNE));
			pData[i] = I2C_ReceiveData(I2C2);
	}

	//Send stop signal
	I2C_GenerateSTOP(I2C2,ENABLE);
	return I2C_STATUS_SUCCESS;
}

void i2c3_init(void){
	// Configure gpio pins for i2c operation PC9:SDA PA8:SCL
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Mode = GPIO_Mode_AF;
	myGPIO.GPIO_Pin = GPIO_Pin_9;
	myGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOC, &myGPIO);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	myGPIO.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &myGPIO);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_4);

	// Configure I2C
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
	I2C_InitTypeDef myI2C;
	I2C_StructInit(&myI2C);
	myI2C.I2C_Ack = I2C_Ack_Enable;
	myI2C.I2C_Mode = I2C_Mode_I2C;
	myI2C.I2C_Timing = (PRESC << 28) | (SCLDEL << 20) | (SDADEL << 16) | (SCLH << 8) | (SCLL << 0);//Timing for 100KHz operation
	I2C_Init(I2C3,&myI2C);
	I2C_StretchClockCmd(I2C3,ENABLE);
	I2C_Cmd(I2C3, ENABLE);
}

int i2c3_write(char address7b, char regAddress, char* pData, int nData){
	return i2c3_writeRaw(address7b,regAddress,pData,nData,1);
}

int i2c3_writeRegister(char address7b, char regAddress, char data){
	char txData[1] = {data};
	return i2c3_writeRaw(address7b,regAddress,txData,1,1);
}

int i2c3_writeRaw(char address7b,char regAddress,char* pData, int nData,int useRegAddress){
	// Set i2c in tramission mode
	I2C_MasterRequestConfig(I2C3, I2C_Direction_Transmitter);
	I2C_10BitAddressingModeCmd(I2C3,DISABLE);
	I2C_SlaveAddressConfig(I2C3,address7b<<1);
	I2C_NumberOfBytesConfig(I2C3,nData+useRegAddress);
	I2C_ReloadCmd(I2C3,DISABLE);
	I2C_AutoEndCmd(I2C3, DISABLE);

	// Send start signal
	I2C_GenerateSTART(I2C3, ENABLE);

	//Send register address
	if(useRegAddress){
		while(1){
			if(I2C_GetFlagStatus(I2C3,I2C_FLAG_NACKF)){// If nack recivied exit with error
				return I2C_STATUS_FAIL;
			}
			else if(I2C_GetFlagStatus(I2C3,I2C_FLAG_TXE)){
				I2C_SendData(I2C3,regAddress);
				break;
			}
		}
	}

	//Send data
	for(int i = 0; i < nData; i++){
		while(1){
			if(I2C_GetFlagStatus(I2C3,I2C_FLAG_NACKF)){// If nack recivied exit with error
				return I2C_STATUS_FAIL;
			}
			else if(I2C_GetFlagStatus(I2C3,I2C_FLAG_TXE)){
				I2C_SendData(I2C3,pData[i]);
				break;
			}
		}
	}

	while(!I2C_GetFlagStatus(I2C3,I2C_FLAG_TC));//Wait until trasmission complete

	//Set stop signal
	I2C_GenerateSTOP(I2C3,ENABLE);
	return I2C_STATUS_SUCCESS;
}

int i2c3_read(char address7b,char regAddress, char* pData, int nData){
	// Wait while busy
	while(I2C_GetFlagStatus(I2C3,I2C_FLAG_BUSY));

	//Configure bytes transmission
	I2C_MasterRequestConfig(I2C3, I2C_Direction_Transmitter);
	I2C_10BitAddressingModeCmd(I2C3,DISABLE);//7 Bit mode
	I2C_SlaveAddressConfig(I2C3,address7b<<1);
	I2C_NumberOfBytesConfig(I2C3,1);
	I2C_ReloadCmd(I2C3,DISABLE);
	I2C_AutoEndCmd(I2C3, DISABLE);

	// Send start signal
	I2C_GenerateSTART(I2C3, ENABLE);

	// Wait  until ready to transmittin
	while(1){
		if(I2C_GetFlagStatus(I2C3,I2C_FLAG_NACKF)){// If nack return fail
			return 0;
		}
		else if(I2C_GetFlagStatus(I2C3,I2C_FLAG_TXE)){
			I2C_SendData(I2C3,regAddress);
			break;
		}
	}

	// Wait until transmission done
	while(!I2C_GetFlagStatus(I2C3,I2C_FLAG_TXE));

	//Change to receiver mode
	I2C_MasterRequestConfig(I2C3, I2C_Direction_Receiver);
	I2C_NumberOfBytesConfig(I2C3,nData);
	I2C_10BitAddressingModeCmd(I2C3,DISABLE);

	// Send restart signal
	I2C_GenerateSTART(I2C3,ENABLE);

	// Wait until data is recived or nack is detected
	while(1){
		if(I2C_GetFlagStatus(I2C3,I2C_FLAG_NACKF)){// Device not found
			return I2C_STATUS_FAIL;
		}
		if(I2C_GetFlagStatus(I2C3,I2C_FLAG_RXNE)){
			break;
		}
	}

	// read data
	for(int i = 0; i < nData; i++){
		while(!I2C_GetFlagStatus(I2C3,I2C_FLAG_RXNE));
			pData[i] = I2C_ReceiveData(I2C3);
	}

	//Send stop signal
	I2C_GenerateSTOP(I2C3,ENABLE);
	return I2C_STATUS_SUCCESS;
}
