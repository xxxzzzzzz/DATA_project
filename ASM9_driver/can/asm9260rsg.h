#ifndef	SJA_RSG_H
#define SJA_RSG_H
#include "typedef.h"


typedef union tagRsgMod
{
	INT32U Byte;
	struct  
	{
		INT32U RM	:1;
		INT32U LOM	:1;
		INT32U STM	:1;
		INT32U AFM	:1;
		INT32U SM	:1;
		INT32U Rsv	:27;
	}Bits;
}uRsgMod;


typedef union tagRsgCmd
{
	INT32U Byte;
	struct  
	{
		INT32U TR	:1;
		INT32U AT	:1;
		INT32U RR	:1;
		INT32U CDO	:1;
		INT32U SRR	:1;
		INT32U Rsv	:27;
	}Bits;
}uRsgCmd;
#endif
/*
*********************************************************************************************************
**                            End Of File
*********************************************************************************************************
*/
