#ifndef __PCF8591_RW_H
#define __PCF8591_RW_H

void Start_I2c(void);
void Stop_I2c(void);
unsigned char IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(unsigned char txd);
unsigned char IIC_Read_Byte(unsigned char ack);
unsigned int bh1750(void);
unsigned char PCF8591_WR_Reg2(void);
#endif
