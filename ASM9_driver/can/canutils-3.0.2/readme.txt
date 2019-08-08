1. 用线将两台S3C2440的CAN口连接起来。
2. 然后一个板子用于发送，另一个板子接收。如图2是接收的结果。发送命令具体看man帮助。


CAN通信测试工具.
canconfig.c——can配置工具；
candump.c——can接收工具；
cansend.c——can发送工具；