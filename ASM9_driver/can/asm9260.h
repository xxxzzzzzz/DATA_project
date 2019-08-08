#ifndef ASM9260_H
#define ASM9260_H
#include "typedef.h"
#include "asm9260rsg.h"
#include <linux/ioctl.h>


extern void *asm9260_ale;
extern void *asm9260_dat;
/*
***********************************
	¸ÃÐÐÃªlinux ioctlº¯Ê
***********************************
*/
#define CAN_BUS_TYPE	0x40

#define IOCTL_BAUD	_IOWR(CAN_BUS_TYPE, 0x01, INT32U)
#define IOCTL_ACR	_IOWR(CAN_BUS_TYPE, 0x02, INT32U)
#define IOCTL_AMR	_IOWR(CAN_BUS_TYPE, 0x03, INT32U)

void Write_ASM9260(INT8U Val, INT8U OffSet);

INT8U Read_ASM9260(INT8U OffSet);

/*
*****************************
*/
#define MOD_LOM		0x01
#define MOD_STM		0x02
#define MOD_AFM_SIG	0x04
#define MOD_AFM_DOB	0x08
/*
*****************************
*/
#define CMR_NOM_SD	0x01
#define CMR_SLF_SD	0x02
#define CMR_SIG_SD	0x03
#define CMR_CL_BUF	0x05
#define CMR_CL_DATA	0x06
/*
*****************************
*/
#define SR_RCV_VALI	0x01
#define SR_DAT_OVER 0x02
#define SR_TBF_VALI	0x04
#define SR_TRM_COMP 0x08
#define SR_RCV_PROC	0x10
#define SR_TRM_PROC	0x20
#define SR_ERR_OCCR	0x40
#define SR_BUS_ERR	0x80
/*
*****************************
*/
#define	IR_RC	0x01
#define	IR_TR	0x02
#define IR_ER	0x04
#define	IR_DO	0x08
#define	IR_WU	0x10
#define	IR_EP	0x20
#define	IR_AL	0x40
#define	IR_BE	0x80
/*
***************************** 
*/
#define	IER_RC	0x01
#define	IER_TR	0x02
#define IER_ER	0x04
#define	IER_DO	0x08
#define	IER_WU	0x10
#define	IER_EP	0x20
#define	IER_AL	0x40
#define	IER_BE	0x80
#define IER_ALL	0xFF


#define BAIS_CAN	0x00
#define PELI_CAN	0x01

#define	MOD		0
#define CMR		1
#define SR		2
#define IR		3
#define IER		4
#define	BTR0	5
#define	BTR1	6
#define	BTR2	7
#define	OCR		8
#define	TEST	9
#define	ALC		11
#define	ECC		12
#define	EWL		13
#define RXERR	14
#define TXERR	15
#define TXBUF	16
#define RXBUF	16
#define ACR0	16
#define AMR0	20
#define CDR		31



typedef union tagFramInfo
{
	INT32U Byte;
	struct
	{
		INT32U	DLC		:4;
		INT32U	Resv1	:2;
		INT32U	RTR		:1;
		INT32U	FF		:1;
		INT32U	Resv2	:24;
	}Bits;
}FramInfo;
typedef struct tagASM9260_BufInfo
{
	INT32U		FrID;
	FramInfo	FrIf;
	INT8U		DA[8];
}stcASM9260_BufInfo;

BOOL asm_SoftRst(BOOL bIsRst);

BOOL SetModeReg(INT8U Val, BOOL bIsEn);

BOOL EnterSleepMod(BOOL IsEn);

void SetCommand(INT8U Cmd);

BOOL SetInterrupt(INT8U Val, BOOL IsEn);

BOOL SetBaudRate(INT32U BaudVal);

BOOL SetOutPutMod(INT8U Val);

BOOL SetClkDiv(INT8U CAN_Mod,BOOL bRxINTEn,BOOL bClkOFF,INT8U Div);

BOOL ASM9260Init(INT8U CanMode, INT32U BaudRate,INT32U ACRCode, INT32U AMRCode);

void SetTxBuf(stcASM9260_BufInfo *pTxInfo);

BOOL ReadRcvBuf(stcASM9260_BufInfo *pRcvBuf);

BOOL SetFliter(INT32U ACRCode, INT32U AMRCode);

BOOL ACRCode(INT32U ACRCode);

BOOL AMRCode(INT32U AMRCode);

void IntEntry(INT8U IRTemp);

#endif
/*
*********************************************************************************************************
**                            End Of File
*********************************************************************************************************
*/
