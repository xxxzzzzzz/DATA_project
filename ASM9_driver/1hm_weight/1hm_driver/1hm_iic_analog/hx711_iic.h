#ifndef __1HM_IIC_ANALOG 
#define __1HM_IIC_ANALOG

unsigned long Read_HX711(int clk_GPIOgroup,int clk_pin,int data_GPIOgroup,int data_pin);
unsigned long get_weight(int clk_GPIOgroup,int clk_pin,int data_GPIOgroup,int data_pin);
#endif
