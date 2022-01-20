#if 0
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "OLED.h"

#define I2C_ADDRESS 0x3C

/*全屏大小 8页 * 128列 * 64行  每页是8*128个格子，通过高低列组合出0~127的数据，寻找起始列进行从左向右填充*/
/*每一页有128个数据, 每个数据由一列8个格子组成, 大小为0x00-0xFF*/
unsigned char DataBuffer[8][128];
int i2cHand = 0;

int main(void)
{
  	/*初始化树莓派硬件*/
  	wiringPiSetup();
  	/*获取OLED设备句柄*/
  	i2cHand = wiringPiI2CSetup(I2C_ADDRESS);
  	/*初始化OLED*/
  	OLED_Init(i2cHand);
  	while(1)
  	{
    		OLED_Fill();
    		delay(1000);
 
    		OLED_CLS();
		delay(1000);
  	}
  	return 0;
}

/*初始化OLED*/
void OLED_Init(int fd)
{
  	WriteCmd(fd, 0xAE);   /*Display off 关掉屏幕*/
  	WriteCmd(fd, 0x20);   /*Set Memory Addressing Mode:Horizontal Addressing Mode 水平刷写模式*/
  	WriteCmd(fd, 0x10);   /*Set Higher Column Start Address for Page Addressing Mode*/
  	WriteCmd(fd, 0xB0);   /*Set Page Start Address for Page Addressing Mode,PAGE0~PAGE7*/
  	WriteCmd(fd, 0xC8);   /*Set COM Output Scan Direction*/
  	WriteCmd(fd, 0x00);   /*Set Lower Column Start Address for Page Addressing Mode 起始位置低位列0x00~0x0f */
  	WriteCmd(fd, 0x40);   /*Set Display Start Line  起始行0x40~0x7F  正好是0~63*/
  
  	WriteCmd(fd, 0x81);   /*Set Contrast Control 设置对比度*/
  	WriteCmd(fd, 0xFF);   /*亮度调节 0x00~0xFF*/
  
  	WriteCmd(fd, 0xA1);   /*Set Segment Re-map 段重定义设置,bit0:0,0->0;1,0->127*/
  	WriteCmd(fd, 0xA6);   /*Set Normal/Inverse Display 设置显示方式;bit0:1,反相显示;0,正常显示*/
  
  	WriteCmd(fd, 0xA8);   /*Set Multiplex Ratio(16 to 64)*/
  	WriteCmd(fd, 0x3F);   /*默认值是0x3F->63 63d, 64MUX*/
  
  	WriteCmd(fd, 0xA4);   /*Entire Display ON:0xA4,Output follows RAM content;0xA5,Output ignores RAM content*/
  
  	WriteCmd(fd, 0xD3);   /*Set Display Offset*/
  	WriteCmd(fd, 0x00);   /*not offset 0x00为默认*/
  
  	WriteCmd(fd, 0xD5);   /*Set Display Clock Divide Ratio/Oscillator Frequency 设置时钟分频因子,震荡频率*/
  	WriteCmd(fd, 0x80);   /*Set divide ratio [3:0],分频因子;[7:4],震荡频率*/
  
  	WriteCmd(fd, 0xD9);   /*Set Pre-charge Period 设置预充电周期*/
  	WriteCmd(fd, 0x22);   /*[3:0],PHASE 1;[7:4],PHASE 2*/
  
  	WriteCmd(fd, 0xDA);   /*Set COM Pins Hardware Configuration 设置COM硬件引脚配置*/
  	WriteCmd(fd, 0x12);   /*[5:4]配置*/
  
  	WriteCmd(fd, 0xDB);   /*设置VCOMH 电压倍率*/
  	WriteCmd(fd, 0x20);   /*[6:4] 000,0.65*vcc;010,0.77*vcc;011,0.83*vcc*/
	
	WriteCmd(fd, 0x8D);   /*允许电荷泵*/
  	WriteCmd(fd, 0x14);   
  
  	WriteCmd(fd, 0xAF);   /*Display on 打开屏幕*/
}

/*写命令函数*/
void WriteCmd(int fd, unsigned char I2C_Command)
{
  	wiringPiI2CWriteReg8(fd,0x00,I2C_Command);
}

/*写数据函数*/
void WriteData(int fd, unsigned char I2C_Data)
{
  	wiringPiI2CWriteReg8(fd, 0x40, I2C_Data);
}

/*设置坐标起点*/
void OLED_SetPos(int fd,unsigned char x, unsigned char y)
{
	WriteCmd(fd, (unsigned char)(0xB0 + x));
	WriteCmd(fd,((y & 0x0F) | 0x00));       /* LOW */
	WriteCmd(fd,(((y & 0xF0) >> 4) | 0x10));/* HIGHT */
}

/*写缓存数据*/
void Write_DataBuffer(void) /*这个是将DataBuffer数组里面的值，全部写进屏里去*/
{
	unsigned char i, j;
 
	for (i = 0; i < 8; i++)
	{
		OLED_SetPos(i2cHand, i, 0); /*设置起始点坐标*/
		for (j = 0; j < 128; j++)
		{
			WriteData(i2cHand, DataBuffer[i][j]); /*写数据*/
		}
	}
}

/*全屏填充 0xFF*/
void OLED_Fill(void)
{
	int i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 128; j++)
		{
			DataBuffer[i][j] = 0xF0;
		}
	}
	Write_DataBuffer();
}

/*清屏 0x00*/
void OLED_CLS(void)
{
	unsigned char i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 128; j++)
		{
			DataBuffer[i][j] = 0x00;
		}
	}
	Write_DataBuffer();
}
#endif

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<assert.h>
#include<termios.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<sys/types.h>
#include<errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiI2C.h>
#include <unistd.h>

int fd;                                 //截至
unsigned char  yi[16]={"Angle of beam:"};
unsigned char  er[16]={"ming"};
unsigned char san[16]={"Distance:"};
unsigned char  si[16]={"okok"};//显示内容
const unsigned char zi[];

void init(void)//初始化
{
	wiringPiSetup();
        fd=wiringPiI2CSetup(0x3c);//i2c初始化
        wiringPiI2CWriteReg8(fd,0x00,0xa1);//图像反了修改成0xa0
        wiringPiI2CWriteReg8(fd,0x00,0xc8);//行输出反了修改成0xc0
        wiringPiI2CWriteReg8(fd,0x00,0x8d);//允许电荷泵
        wiringPiI2CWriteReg8(fd,0x00,0x14);
        wiringPiI2CWriteReg8(fd,0x00,0xa6);//想反相显示改成0xa7
        wiringPiI2CWriteReg8(fd,0x00,0xaf);//开显示
}

void qingping(void)//清屏
{
	char zt1,zt2;
	for(zt1=0;zt1<8;zt1++)
	{
		wiringPiI2CWriteReg8(fd,0x00,0xb0+zt1);
	        for(zt2=0;zt2<128;zt2++) wiringPiI2CWriteReg8(fd,0x40,0x00);
	}
}


void ascii(float Angle,float distance)//显示ASCII码8*16
{
    sprintf(er,"%f",Angle);  // float 到 char
	sprintf(si,"%f",distance); // double 到 char 
	int zt;
        char zt3,zt4;
	for(zt3=0;zt3<4;zt3++)
        {
		wiringPiI2CWriteReg8(fd,0x00,0xb0+(zt3*2));
                for(zt4=0;zt4<16;zt4++)
                {
			         for(zt=0;zt<8;zt++)
                        {
                        	if(zt3==0) wiringPiI2CWriteReg8(fd,0x40,zi[yi[zt4]*16+zt]);
                                else if(zt3==1)  wiringPiI2CWriteReg8(fd,0x40,zi[er[zt4]*16+zt]);
                                else if(zt3==2)  wiringPiI2CWriteReg8(fd,0x40,zi[san[zt4]*16+zt]);
                                else if(zt3==3)  wiringPiI2CWriteReg8(fd,0x40,zi[si[zt4]*16+zt]);
                        }
		}
                wiringPiI2CWriteReg8(fd,0x00,0xb0+(zt3*2)+1);
                for(zt4=0;zt4<16;zt4++)
                {
                	for(zt=0;zt<8;zt++)
                        {
                        	if(zt3==0) wiringPiI2CWriteReg8(fd,0x40,zi[yi[zt4]*16+zt+8]);
                                else if(zt3==1)  wiringPiI2CWriteReg8(fd,0x40,zi[er[zt4]*16+zt+8]);
                                else if(zt3==2)  wiringPiI2CWriteReg8(fd,0x40,zi[san[zt4]*16+zt+8]);
                                else if(zt3==3)  wiringPiI2CWriteReg8(fd,0x40,zi[si[zt4]*16+zt+8]);
                        }
                }
	}
}


int main(void)
{
    float Angle = 2.98754546;
    float distance = 5.754644545;

	init();
	delay(10);
	qingping();
	while(1)
	{
		ascii(Angle,distance);
		delay(10);
	}
}


const unsigned char zi[] =
{
/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//0

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//1

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//2

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//3

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//4

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//5

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//6

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//7

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//8

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//9

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//10

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//11

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//12

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//13

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//14

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//15

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//16

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//17

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//18

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//19

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//20

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//21

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//22

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//23

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//24

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//25

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//26

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//27

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//28

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//29

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//30

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//31

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//32

/*--  文字:  !  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x00,0x00,0x00,0x00,//33

/*--  文字:  "  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x10,0x0C,0x02,0x10,0x0C,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//34

/*--  文字:  #  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x40,0xC0,0x78,0x40,0xC0,0x78,0x00,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x00,//35

/*--  文字:  $  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x88,0x88,0xFC,0x08,0x30,0x00,0x00,0x18,0x20,0x20,0xFF,0x21,0x1E,0x00,//36

/*--  文字:  %  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xF0,0x08,0xF0,0x80,0x60,0x18,0x00,0x00,0x00,0x31,0x0C,0x03,0x1E,0x21,0x1E,0x00,//37

/*--  文字:  &  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x2C,0x19,0x27,0x21,0x10,//38

/*--  文字:  '  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x12,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//39

/*--  文字:  (  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//40

/*--  文字:  )  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//41

/*--  文字:  *  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//42

/*--  文字:  +  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x0F,0x01,0x01,0x01,//43

/*--  文字:  ,  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,0x70,0x00,0x00,0x00,0x00,0x00,//44

/*--  文字:  -  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,//45

/*--  文字:  .  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//46

/*--  文字:  /  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0xC0,0x38,0x04,0x00,0x00,0x60,0x18,0x07,0x00,0x00,0x00,0x00,//47

/*--  文字:  0  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//48

/*--  文字:  1  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,//49

/*--  文字:  2  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x08,0x08,0x08,0x08,0xF0,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//50

/*--  文字:  3  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x20,0x21,0x21,0x22,0x1C,0x00,//51

/*--  文字:  4  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x40,0x30,0xF8,0x00,0x00,0x00,0x06,0x05,0x24,0x24,0x3F,0x24,0x24,//52

/*--  文字:  5  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x20,0x20,0x20,0x11,0x0E,0x00,//53

/*--  文字:  6  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xE0,0x10,0x88,0x88,0x90,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x20,0x1F,0x00,//54

/*--  文字:  7  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x18,0x08,0x08,0x88,0x68,0x18,0x00,0x00,0x00,0x00,0x3E,0x01,0x00,0x00,0x00,//55

/*--  文字:  8  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//56

/*--  文字:  9  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xF0,0x08,0x08,0x08,0x10,0xE0,0x00,0x00,0x01,0x12,0x22,0x22,0x11,0x0F,0x00,//57

/*--  文字:  :  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//58

/*--  文字:  ;  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0x00,//59

/*--  文字:  <  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//60

/*--  文字:  =  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,0x02,0x02,0x02,0x02,0x02,0x02,0x00,//61

/*--  文字:  >  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//62

/*--  文字:  ?  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x48,0x08,0x08,0x88,0x70,0x00,0x00,0x00,0x00,0x30,0x37,0x00,0x00,0x00,//63

/*--  文字:  @  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x28,0x2F,0x28,0x17,0x00,//64

/*--  文字:  A  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//65

/*--  文字:  B  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//66

/*--  文字:  C  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//67

/*--  文字:  D  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//68

/*--  文字:  E  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//69

/*--  文字:  F  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//70

/*--  文字:  G  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//71

/*--  文字:  H  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//72

/*--  文字:  I  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//73

/*--  文字:  G  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//74

/*--  文字:  K  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//75

/*--  文字:  L  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//76

/*--  文字:  M  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x01,0x3E,0x01,0x3F,0x20,0x00,//77

/*--  文字:  N  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//78

/*--  文字:  O  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//79

/*--  文字:  P  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//80

/*--  文字:  Q  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x28,0x28,0x30,0x50,0x4F,0x00,//81

/*--  文字:  R  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//82

/*--  文字:  S  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//83

/*--  文字:  T  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//84

/*--  文字:  U  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//85

/*--  文字:  V  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//86

/*--  文字:  W  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x00,0xF8,0x00,0xF8,0x08,0x00,0x00,0x03,0x3E,0x01,0x3E,0x03,0x00,0x00,//87

/*--  文字:  X  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//88

/*--  文字:  Y  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//89

/*--  文字:  Z  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//90

/*--  文字:  [  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//91

/*--  文字:  \  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x04,0x38,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//92

/*--  文字:  ]  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//93

/*--  文字:  ^  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x04,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//94

/*--  文字:  _  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//95

/*--  文字:  `  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//96

/*--  文字:  a  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x19,0x24,0x24,0x12,0x3F,0x20,0x00,//97

/*--  文字:  b  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x10,0xF0,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//98

/*--  文字:  c  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//99

/*--  文字:  d  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x90,0xF0,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//100

/*--  文字:  e  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x24,0x24,0x24,0x24,0x17,0x00,//101

/*--  文字:  f  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x80,0x80,0xE0,0x90,0x90,0x20,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//102

/*--  文字:  g  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//103

/*--  文字:  h  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x10,0xF0,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//104

/*--  文字:  i  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//105

/*--  文字:  g  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//106

/*--  文字:  k  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x10,0xF0,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x06,0x29,0x30,0x20,0x00,//107

/*--  文字:  l  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//108

/*--  文字:  m  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//109

/*--  文字:  n  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//110

/*--  文字:  o  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//111

/*--  文字:  p  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0x91,0x20,0x20,0x11,0x0E,0x00,//112

/*--  文字:  q  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x80,0x80,0x00,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0x91,0xFF,0x80,//113

/*--  文字:  r  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//114

/*--  文字:  s  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//115

/*--  文字:  t  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x10,0x00,//116

/*--  文字:  u  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//117

/*--  文字:  v  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x03,0x0C,0x30,0x0C,0x03,0x00,0x00,//118

/*--  文字:  w  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x00,0x80,0x80,0x00,0x80,0x80,0x01,0x0E,0x30,0x0C,0x07,0x38,0x06,0x01,//119

/*--  文字:  x  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x80,0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x20,0x31,0x0E,0x2E,0x31,0x20,0x00,//120

/*--  文字:  y  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x81,0x86,0x78,0x18,0x06,0x01,0x00,//121

/*--  文字:  z  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//122

/*--  文字:  {  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0x00,0xFC,0x02,0x02,0x00,0x00,0x00,0x00,0x01,0x3E,0x40,0x40,//122

/*--  文字:  |  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//124

/*--  文字:  }  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x02,0x02,0xFC,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x3E,0x01,0x00,0x00,0x00,0x00,//125

/*--  文字:  ~  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x02,0x01,0x02,0x02,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//126
};

#if 0 
void shijian(void)//当前时间
{
        struct tm *ptr;
        time_t lt;
        lt=time(&lt);
        ptr=localtime(&lt);
        strftime(yi[1],16,"%m/%d %a",ptr); //月/日 周几
    strftime(yi[2],16,"%R %p",ptr);//时:分 am或pm
}
#endif

