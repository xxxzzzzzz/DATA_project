#ifndef TYPE_H
#define TYPE_H

typedef unsigned char	INT8U;       /* 无符号8位整型变量            */
typedef signed   char	INT8S;       /* 有符号8位整型变量            */
typedef unsigned short	INT16U;      /* 无符号16位整型变量           */
typedef signed   short	INT16S;      /* 有符号16位整型变量           */
typedef unsigned int	INT32U;      /* 无符号32位整型变量           */
typedef signed   int	INT32S;      /* 有符号32位整型变量           */
typedef float		FP32;      /* 单精度浮点数（32位长度）       */
typedef double		FP64;      /* 双精度浮点数（64位长度）*/

#ifndef NULL
	#define NULL			0
#endif

#ifndef	BOOL
	#define BOOL			INT8U
#endif

#ifndef TRUE
	#define	TRUE			1
#endif

#ifndef FLASE
	#define FALSE			0
#endif

#endif

/*
*********************************************************************************************************
**                            End Of File
*********************************************************************************************************
*/
