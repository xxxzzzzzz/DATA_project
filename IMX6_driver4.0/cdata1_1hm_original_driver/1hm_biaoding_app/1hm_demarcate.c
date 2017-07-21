
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h> 
#include<string.h>
#include<malloc.h>
#include<1hm_original_creat.h>

void biaoding();
void read_weight();
void test_1hm();

int main(int arc,char *argv[]){

	if(arc!=2){
	printf("input erro\n");
	return -1;
	}



switch(atoi(argv[1]))
{
	case 1:test_1hm();break;
	case 2:biaoding();break;
	case 3:read_weight();break;

	default:
	       break;
}
return 0;
}

/*get dermarcate oefficient and original*/
void biaoding(){
get_original_oefficient();
}




///////////////////////////////////////read weight//////////////////////////

void read_weight(){
int original1,original2,original3,original4;
int weight1,weight2,weight3,weight4,chanshu;
unsigned int data1=0,data2=0,data3=0,data4=0;
float oefficient;
unsigned int totalweight;
read_original_file(&original1,&original2,&original3,&original4,&oefficient);


/*weight1 open sensor*/
if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight1_class failed\n");}
else {printf("APP open weight1_class success\n");}


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
 totalweight=(data1+data2+data3+data4)*oefficient;

printf("weight1 : %d weight2 : %d weight3 : %d weight4 : %d total_weight : %d\n",data1,data2,data3,data4,totalweight);
}

close(weight1);
close(weight2);
close(weight3);
close(weight4);

}


void test_1hm(){
	 
unsigned int original1,original2,original3,original4;
int weight1,weight2,weight3,weight4,chanshu;
unsigned int data1=0,data2=0,data3=0,data4=0;
unsigned int totalweight;
 
/*weight1 open sensor*/
if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0)
	{printf("APP open weight1_class failed\n");}
else {printf("APP open weight1_class success\n");}


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

 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value
 original1=data1;
 original2=data2;
 original3=data3;
 original4=data4;
 
 do{ 
 
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value

 data1=data1-original1;
 data2=data2-original2;
 data3=data3-original3;
 data4=data4-original4;
 totalweight=(data1+data2+data3+data4);
 totalweight=abs(totalweight);
 printf("1hm_test totalweight = %d\n",totalweight);
}while(totalweight<2000);

printf("run close\n");


close(weight1);
close(weight2);
close(weight3);
close(weight4);

}
