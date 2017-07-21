#ifndef __TEMPHUMI_H
#define __TEMPHUMI_H
unsigned char DHT11_Init(void);//初始化DHT11
unsigned char DHT11_Read_Data(u8 *temp,u8 *humi);//读取温湿度
unsigned char DHT11_Read_Byte(void);//读出一个字节
unsigned char DHT11_Read_Bit(void);//读出一个位
unsigned char DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    


#endif
