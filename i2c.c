#include "stm32f10x.h"
#include "i2c.h"

void I2C1_Initialize(void){
	RCC -> APB2ENR |= RCC_APB2ENR_IOPBEN |
										RCC_APB2ENR_AFIOEN;
	RCC -> APB1ENR |= RCC_APB1ENR_I2C1EN;
	
  // I2C GPIO CONFIG
   GPIOB->CRL |= GPIO_CRL_CNF6 | GPIO_CRL_CNF7 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7;

   // Software reset I2C
   I2C1->CR1 |= I2C_CR1_SWRST;
   I2C1->CR1 &= ~I2C_CR1_SWRST;

   I2C1->CR1 &= ~I2C_CR1_PE;
   I2C1->CR2 = I2C_CR2_FREQ_5 | I2C_CR2_FREQ_2;			//I2C1 Clock 36Mhz
	 I2C1->TRISE = 37;
   I2C1->CCR = 180;																	//I2C1 Clk = 36Mhz/180/2 = 100kHz

   // Wlaczenie I2C
   I2C1->CR1 |= I2C_CR1_PE | I2C_CR1_ACK;
   I2C1->OAR1 = (1 << 14);
}


void I2C1_Start(void)
{
   I2C1->CR1 |= I2C_CR1_START;
   while( !(I2C1->SR1 & I2C_SR1_SB) );
   uint32_t tmp = I2C1->SR1;
}

void I2C1_Stop(void)
{
   while( !(I2C1->SR1 & I2C_SR1_BTF) );
   I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C1_SendAddr(uint8_t address)
{
   I2C1->DR = address;
   while( !(I2C1->SR1 & I2C_SR1_ADDR) );
   uint32_t dummy = I2C1->SR1;
   dummy = I2C1->SR2;
}

void I2C1_SendByte(uint8_t byte)
{
   while( !(I2C1->SR1 & I2C_SR1_TXE) );
   I2C1->DR = byte;
}

uint8_t I2C1_MemoryWrite(uint8_t addr, uint8_t cmd, uint8_t byte){
	I2C1_Start();
	I2C1_SendAddr(addr);
	I2C1_SendByte(cmd);
	I2C1_SendByte(byte);
	I2C1_Stop();
	return 0;
}

uint8_t I2C1_MemoryRead(uint8_t addr, uint8_t cmd){
	I2C1_Start();
	I2C1_SendAddr(addr);
	I2C1_SendByte(cmd);
	while(!(I2C1->SR1 & I2C_SR1_BTF));
	I2C1_Start();		
	I2C1->DR = addr | 0x01;
	I2C1->CR1 &= ~I2C_CR1_ACK;							//NACK
	while( !(I2C1->SR1 & I2C_SR1_ADDR) );
	uint32_t dummy = I2C1->SR1;
	dummy = I2C1->SR2;
	I2C1_Stop();
	while(!(I2C1->SR1 & I2C_SR1_RXNE));	
	return I2C1->DR;
}