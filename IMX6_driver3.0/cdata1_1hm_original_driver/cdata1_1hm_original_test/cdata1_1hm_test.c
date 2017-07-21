
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h> 
#include<string.h>
#include<malloc.h>

void read_weight1();
void read_weight2();
void read_weight3();
void read_weight4();
void read_weight_zong();

int main(int arc,char *argv[]){

	if(arc!=2){
	printf("input erro\n");
	return -1;
	}



switch(atoi(argv[1]))
{
	case 1:read_weight1();break;
	case 2:read_weight2();break;
	case 3:read_weight3();break;
	case 4:read_weight4();break;
	default:
	       read_weight_zong();
	       break;
}
return 0;
}





///////////////////////////////////////read weight1//////////////////////////


void read_weight1(){
int original1;
int weight1;
unsigned int data1=0;


/*weight1 open sensor*/
if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight1_class failed\n");}
else {printf("APP open weight1_class success\n");}

read(weight1, &original1, sizeof(&original1));//read weight1 value

while(1){

	read(weight1, &data1, sizeof(&data1));//read weight1 value
	data1=data1-original1;
	printf("weight1 : %d\n",data1);
	

  }
close(weight1);

}



///////////////////////////////////////read weight2//////////////////////////
void read_weight2(){
int original2;
int weight2;
unsigned int data2=0;


/*weight2 open sensor*/
if((weight2=open("/dev/threesboy_weight2", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight2_class failed\n");}
else {printf("APP open weight2_class success\n");}

read(weight2, &original2, sizeof(&original2));//read weight1 value

while(1){

	read(weight2, &data2, sizeof(&data2));//read weight1 value
	data2=data2-original2;
	printf("weight2 : %d\n",data2);
	

  }
close(weight2);

}


///////////////////////////////////////read weight3//////////////////////////
void read_weight3(){
int original3;
int weight3;
unsigned int data3=0;


/*weight3 open sensor*/
if((weight3=open("/dev/threesboy_weight3", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight3_class failed\n");}
else {printf("APP open weight3_class success\n");}

read(weight3, &original3, sizeof(&original3));//read weight1 value

while(1){

	read(weight3, &data3, sizeof(&data3));//read weight1 value
	data3=data3-original3;
	printf("weight3 : %d\n",data3);
	

  }
close(weight3);

}


///////////////////////////////////////read weight4//////////////////////////
void read_weight4(){
int original4;
int weight4;
unsigned int data4=0;


/*weight4 open sensor*/
if((weight4=open("/dev/threesboy_weight4", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight4_class failed\n");}
else {printf("APP open weight4_class success\n");}

read(weight4, &original4, sizeof(&original4));//read weight1 value

while(1){

	read(weight4, &data4, sizeof(&data4));//read weight1 value
	data4=data4-original4;
	printf("weight4 : %d\n",data4);
	

  }
close(weight4);

}



void read_weight_zong(){
int original1,original2,original3,original4;
int weight1,weight2,weight3,weight4,chanshu;
unsigned int data1=0,data2=0,data3=0,data4=0;
unsigned int totalweight;




/*weight2 open sensor*/
if((weight2=open("/dev/threesboy_weight2", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight2_class failed\n");}
else {printf("APP open weight2_class success\n");}


/*weight3 open sensor*/

if((weight3=open("/dev/threesboy_weight3", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight3_class failed\n");}
else {printf("APP open weight3_class success\n");}


/*weight4 open sensor*/
if((weight4=open("/dev/threesboy_weight4", O_RDWR | O_NDELAY))<0)
  {printf("APP open weight4_class failed\n");}
else {printf("APP open weight4_class success\n");}

while(1){
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value
 
 data1=data1-original1;
 data2=data2-original2;
 data3=data3-original3;
 data4=data4-original4;
 totalweight=(data1+data2+data3+data4);

printf("weight1 : %d weight2 : %d weight3 : %d weight4 : %d total_weight : %d\n",data1,data2,data3,data4,totalweight);
}

close(weight1);
close(weight2);
close(weight3);
close(weight4);

}


