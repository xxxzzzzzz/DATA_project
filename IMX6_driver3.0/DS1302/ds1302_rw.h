#ifndef __DS1302_RW_H
#define __DS1302_RW_H
void DS1302_writeByte(unsigned char data);  /*write ds1302 register 1  byte */
unsigned char DS1302_ReadByte(void);        /*read ds1302 register 1  byte */
void write_DS1302(unsigned char addr,unsigned char DS_dat);  /*write DS1302 addr the data*/
unsigned char read_DS1302(unsigned char addr); /*read DS1302 data*/



void init_ds1302(unsigned char sec, unsigned char min, unsigned char hour,unsigned char day,
unsigned char month,unsigned char year);


#define WRITE_SECOND 0X80
#define READ_SECOND  0X81
#define WRITE_MINUTE 0X82
#define READ_MINUTE  0X83
#define WRITE_HOUR   0X84
#define READ_HOUR    0X85
#define WRITE_DAY    0X86
#define READ_DAY     0X87
#define WRITE_MONTH  0X88
#define READ_MONTH   0X89
#define WRITE_YEARS  0X8C
#define READ_YEARS   0X8D

#define charger      0X90
#define WRITE_PROTECT 0X8E //write baohu





#endif
