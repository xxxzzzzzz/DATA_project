#ifndef TYPE_H
#define TYPE_H

typedef unsigned char	INT8U;       /* �޷���8λ���ͱ���            */
typedef signed   char	INT8S;       /* �з���8λ���ͱ���            */
typedef unsigned short	INT16U;      /* �޷���16λ���ͱ���           */
typedef signed   short	INT16S;      /* �з���16λ���ͱ���           */
typedef unsigned int	INT32U;      /* �޷���32λ���ͱ���           */
typedef signed   int	INT32S;      /* �з���32λ���ͱ���           */
typedef float		FP32;      /* �����ȸ�������32λ���ȣ�       */
typedef double		FP64;      /* ˫���ȸ�������64λ���ȣ�*/

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
