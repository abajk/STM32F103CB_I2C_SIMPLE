#ifndef I2C_H
#define I2C_H

void I2C1_Initialize(void);
uint8_t I2C1_RecvByte(void);
uint8_t I2C1_MemoryWrite(uint8_t addr, uint8_t cmd, uint8_t byte);
uint8_t I2C1_MemoryRead(uint8_t addr, uint8_t cmd);
#endif