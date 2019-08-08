1. 测试程序分两个rcv.c和send.c，分别为接收测试程序和发送测试程序。

2. 演示中的波特率对应为10kbps(接收程序)，100kbps(发送程序)。
         其中接收程序的运行命令：canrcv-test
                          相应的发送端如果有数据发送过来，接收端接收成功的话就会将接收到的在串口中打印出来。
                             （按Ctrl+C键退出接收）
                            
  
              发送程序的运行命令:cansend-test回车可以看见串口中打印发送的数据
                           CAN Data[0x00] = 0x00.
                           CAN Data[0x01] = 0x11.
                           CAN Data[0x02] = 0x22.
                           CAN Data[0x03] = 0x33.
                           CAN Data[0x04] = 0x44.
                           CAN Data[0x05] = 0x55.
                           CAN Data[0x06] = 0x66.
                           CAN Data[0x07] = 0x77.
                        相应的接收端接收到数据   
                                  
                      
  