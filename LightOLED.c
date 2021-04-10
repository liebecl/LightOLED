#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "OLED.h"

#define I2C_ADDRESS 0x3C

/*全屏大小 8page页 * 128col列 * row64行  每页是8*128个格子，通过高低列组合出0~127的数据，寻找起始列进行从左向右填充*/
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
			DataBuffer[i][j] = 0xFF;
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
