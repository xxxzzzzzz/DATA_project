#ifndef __DHT11_H
#define __DHT11_H
unsigned char ds2401_resetAck(int gpio_group,int gpio_pin);
void ds2401_sendcommand(unsigned char command,int gpio_group,int gpio_pin);
unsigned char ds2401_read(unsigned char *romdata,int gpio_group,int gpio_pin);
unsigned char ds2401_readID(unsigned char *romdata,int gpio_group,int gpio_pin);

#endif
