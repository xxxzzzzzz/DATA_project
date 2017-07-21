#include<stdio.h>
#include <fcntl.h>
#include <stdlib.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>
#include<malloc.h>


char *readline(char *filename,int lineee);


int main(){
int fd,out1,out2,out3,out4;
float office;
char zifu1[1024];
char *zizzz;

zizzz=zifu1;
zizzz=readline("/biaoding.txt",0);
out1=atoi(zizzz);
printf("read out1 = %d\n",out1);

zizzz=readline("/biaoding.txt",1);
out2=atoi(zizzz);
printf("read out2 = %d\n",out2);
zizzz=readline("/biaoding.txt",2);
out3=atoi(zizzz);
printf("read out3 = %d\n",out3);

zizzz=readline("/biaoding.txt",3);
out4=atoi(zizzz);
printf("read out4 = %d\n",out4);

zizzz=readline("/biaoding.txt",4);
office=atof(zizzz);
printf("read xishu = %f\n",office);
return 0;
}







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
return NULL;

}
