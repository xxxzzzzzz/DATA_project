#ifndef __DHT11_H
#define __DHT11_H

unsigned char DHT11_Init(int gpio_group,int gpio_pin);
unsigned char DHT11_Read_Data(unsigned char *temp,unsigned char *humi,int gpio_group,int gpio_pin);

#endif
