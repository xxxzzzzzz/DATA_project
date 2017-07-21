#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h> 
#include<string.h>
#include<malloc.h>
#include<1hm_original_creat.h>

char *readline(char *filename,int line);

int get_original_oefficient(){
static int i=5;	
static int weight1,weight2,weight3,weight4,chanshu,weight_oefficient_file;
static unsigned int original1,original2,original3,original4;
static unsigned int data1=0,data2=0,data3=0,data4=0;
static unsigned int totalweight,length;
static unsigned int weight_f=5000;
static float medain,weight_oefficient;
char original_buffer[20];


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

sleep(1);
/*original save variable*/ 
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value
 original1=data1;
 original2=data2;
 original3=data3;
 original4=data4;
 
 printf("original1 = %d original2 = %d original3 = %d original4 = %d\n",data1,data2,data3,data4);
 sleep(1);
 
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value
 data1=data1-original1;
 data2=data2-original2;
 data3=data3-original3;
 data4=data4-original4;
 totalweight=(data1+data2+data3+data4);
 printf("out totalweight original =%d\n",totalweight);
 sleep(1);
 
 printf("plase put in demarcate goods\n"); //get demarcate value sever hint 

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
// printf("out totalweight original =%d\n",totalweight);
// sleep(1);
 }while(abs(totalweight)<2000);
 /*dleay 1s after get stabilization value*/
 sleep(1);
 read(weight1, &data1, sizeof(&data1));//read weight1 value
 read(weight2, &data2, sizeof(&data2));//read weight2 value
 read(weight3, &data3, sizeof(&data3));//read weight3 value
 read(weight4, &data4, sizeof(&data4));//read weight4 value
 data1=data1-original1;
 data2=data2-original2;
 data3=data3-original3;
 data4=data4-original4;

 totalweight=(data1+data2+data3+data4);

// printf("enter calputer weight oefficiebnt\n");
 medain=(float)(weight_f-totalweight)/(float)totalweight;  //demarcate calculate
 printf("medain out = %f\n",medain);
 weight_oefficient=1+medain;//demarcate calculat
 printf("write weight_oefficient =  %1.6f\n",weight_oefficient);


/////////////////////////////////////////////////////////////save in file////////////////////////////
 
if((weight_oefficient_file = creat("/weight_oefficient.txt",0600))<0)
	{printf("weight_oefficient_file creat failed\n");}
	else 
	printf("weight_oefficient_file creat success\n");
if((weight_oefficient_file =open("/weight_oefficient.txt",O_RDWR,0600))<0)
	{printf("weight_oefficient_file creat failed\n");}
	else 
	printf("weight_oefficient_file creat success\n");

/*original1 save file*/
sprintf(original_buffer,"%d",original1);
length=write(weight_oefficient_file,original_buffer,strlen(original_buffer));
if(length==-1)
 printf("txt write failed\n");
else
 printf("txt write success\n");

/*original2 save file*/
length=write(weight_oefficient_file,"\n",strlen("\n")); //line feed

sprintf(original_buffer,"%d",original2);        
length=write(weight_oefficient_file,original_buffer,strlen(original_buffer));
if(length==-1)
 printf("txt write failed\n");
else
 printf("txt write success\n");

/*original3 save file*/
length=write(weight_oefficient_file,"\n",strlen("\n"));
sprintf(original_buffer,"%d",original3);
length=write(weight_oefficient_file,original_buffer,strlen(original_buffer));
if(length==-1)
 printf("txt write failed\n");
else
 printf("txt write success\n");

/*original4 save file*/
length=write(weight_oefficient_file,"\n",strlen("\n"));
sprintf(original_buffer,"%d",original4);
length=write(weight_oefficient_file,original_buffer,strlen(original_buffer));
if(length==-1)
 printf("txt write failed\n");
else
 printf("txt write success\n");

/*demarcate value save file*/
length=write(weight_oefficient_file,"\n",strlen("\n"));
sprintf(original_buffer,"%1.6f",weight_oefficient);
length=write(weight_oefficient_file,original_buffer,strlen(original_buffer));
if(length==-1)
 printf("txt write failed\n");
else
 printf("txt write success\n");

close(weight1);
close(weight2);
close(weight3);
close(weight4);
close(weight_oefficient_file);
return 0;
}
 
 
 
 
	




/*according file rules get original or dermarcate value*/
void read_original_file(int *weight1,int *weight2,int *weight3,int *weight4,float *oefficient){
char *data;

data=(char *)malloc(sizeof(char)*8);
data=readline("/weight_oefficient.txt",0);	
*weight1=atoi(data);
printf("out original weight1 = %d",*weight1);


data=readline("/weight_oefficient.txt",1);	
*weight2=atoi(data);
printf("out original weight2 = %d",*weight2);


data=readline("/weight_oefficient.txt",2);	
*weight3=atoi(data);
printf("out original weight3 = %d",*weight3);


data=readline("/weight_oefficient.txt",3);	
*weight4=atoi(data);
printf("out original weight4 = %d",*weight4);


data=readline("/weight_oefficient.txt",4);	
*oefficient=atof(data);
printf("out original oefficient = %f",*oefficient);

}


/////////////////////////////////////////////get line function ////////////////////////////////////////////////////
char *readline(char *filename,int line){
FILE *fp;
int linenumber;
int CurrentIndex=0;
char *strLine;


strLine=malloc(sizeof(char)*1024);
linenumber=line;
if((fp = fopen(filename,"r")) == NULL)
	{printf("txt file open failed\n");}
else
	printf("txt file open success\n");

while(!feof(fp)){
memset(strLine,0,sizeof(strLine));
        if(CurrentIndex==linenumber) 
	{
	             fgets(strLine,1024,fp);  //读取一行
	             return strLine;
	}
	         
fgets(strLine,1024,fp);  //读取一行,并定位到下一行
CurrentIndex++;
}

fclose(fp);
}

