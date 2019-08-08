#ifndef __RX8025SA_RW_H
#define __RX8025SA_RW_H

struct rx8025_GPIO_definit
{
	int rx8025_IO_GPIOgroup; //RX8025 data
	int rx8025_IO_pin; //RX8025 data
	int rx8025_SCLK_GPIOgroup; //RX8025 data sclk
	int rx8025_SCLK_pin; //RX8025 data sclk
};



void RX8025_Time_set(struct rtc_time *tm ,struct rx8025_GPIO_definit*  plat_gpio_data);
void RX8025_get_Time(struct rtc_time *tm ,struct rx8025_GPIO_definit* plat_gpio_data);

void RX8025_regTime_test(struct rx8025_GPIO_definit  * plat_gpio_data);

void set_INTA_out_Mode(unsigned char CT,struct rx8025_GPIO_definit* plat_gpio_data);

/*software INTA Out leve Time
 * *tm : value set start time sec min hours week day month year 
 * setmin : set time out INTA Out level minture
 * sethour: set time out INTA Out level hours
 **/                    
void RX8025_INTA_OutIrq_set(unsigned char setmin,unsigned char sethour,struct rx8025_GPIO_definit* plat_gpio_data);


void RX8025_INTA_REG_CONTROL2_get_set_CTFG(struct rx8025_GPIO_definit* plat_gpio_data);

int RX8025_Read_CONTROL2_CTFG(struct rx8025_GPIO_definit* plat_gpio_data);//query CONTROL2 reg  low 3 bit status
void RX8025_SET_CONTROL2_CTFG(struct rx8025_GPIO_definit* plat_gpio_data);///*reset CONTROL2 reg low 3 bit*/
#endif
