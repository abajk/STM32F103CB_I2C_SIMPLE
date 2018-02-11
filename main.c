#include "stm32f10x.h"
#include "main.h"
#include "delay.h"
#include "i2c.h"
#include "tsl2561.h"

void TSL2561_Initialize(void);
void TSL2561_ReadData(void);

int main(void){
	SystemInit();
	
	SysTick_Config(SystemCoreClock/1000);
	
	I2C1_Initialize();
	
	TSL2561_Initialize();
	
	Delay(100);
	
	for(;;){
		TSL2561_ReadData();		
		Delay(200);
	}
}


void TSL2561_Initialize(void){

		uint8_t addr = TSL2561_ADDR_FLOAT;
		uint8_t cmd = TSL2561_COMMAND_BIT | TSL2561_REGISTER_CONTROL;
		uint8_t byte = TSL2561_CONTROL_POWERON;		
		I2C1_MemoryWrite(addr, cmd, byte);
	
		cmd = TSL2561_COMMAND_BIT | TSL2561_REGISTER_TIMING;
		byte = TSL2561_GAIN_1X | TSL2561_INTEGRATIONTIME_402MS;
		I2C1_MemoryWrite(addr, cmd, byte);
	
}

void TSL2561_ReadData(void){
		uint8_t tsl2561_raw_data[4]={0,0,0,0};
		uint8_t addr = TSL2561_ADDR_FLOAT;
		uint8_t cmd = TSL2561_COMMAND_BIT | TSL2561_CLEAR_BIT| TSL2561_REGISTER_CHAN0_LOW;
		tsl2561_raw_data[0] = I2C1_MemoryRead(addr, cmd);
		cmd = TSL2561_COMMAND_BIT | TSL2561_CLEAR_BIT| TSL2561_REGISTER_CHAN0_HIGH;
		tsl2561_raw_data[1] = I2C1_MemoryRead(addr, cmd);
		cmd = TSL2561_COMMAND_BIT | TSL2561_CLEAR_BIT| TSL2561_REGISTER_CHAN1_LOW;
		tsl2561_raw_data[2] = I2C1_MemoryRead(addr, cmd);
		cmd = TSL2561_COMMAND_BIT | TSL2561_CLEAR_BIT| TSL2561_REGISTER_CHAN1_HIGH;
		tsl2561_raw_data[3] = I2C1_MemoryRead(addr, cmd);

}