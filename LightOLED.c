#include <stdio.h>
#include <wiringPi.h>
#include "OLED.h"

#define I2C_ADDRESS 0x3C

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
  
  }
  return 0;
}

/*初始化OLED*/
void OLED_Init(int fd)
{
  WriteCmd(fd, 0xAE);  	/*Display off 关掉屏幕*/
  WriteCmd(fd, 0x20);	  /*Set Memory Addressing Mode:Horizontal Addressing Mode 水平刷写模式*/
  WriteCmd(fd, 0x10);	  /*Set Higher Column Start Address for Page Addressing Mode*/
  WriteCmd(fd, 0xB0);	  /*Set Page Start Address for Page Addressing Mode,PAGE0~PAGE7*/
  WriteCmd(fd, 0xC8);	  /*Set COM Output Scan Direction*/
  WriteCmd(fd, 0x00);   /*Set Lower Column Start Address for Page Addressing Mode 起始位置低位列0x00~0x0f */
  WriteCmd(fd, 0x40);   /*Set Display Start Line  起始行0x40~0x7F  正好是0~63*/
  WriteCmd(fd, 0x81); //--set contrast control register
  WriteCmd(fd, 0xff); //亮度调节 0x00~0xff
  WriteCmd(fd, 0xa1); //--set segment re-map 0 to 127
  WriteCmd(fd, 0xa6); //--set normal display
  WriteCmd(fd, 0xa8); //--set multiplex ratio(1 to 64)
  WriteCmd(fd, 0x3F); // /*设置页，0x3f为默认，有效值为0~14 */
  WriteCmd(fd, 0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  WriteCmd(fd, 0xd3); //-set display offset
  WriteCmd(fd, 0x00); //-not offset 0x00为默认
  WriteCmd(fd, 0xd5); //--set display clock divide ratio/oscillator frequency
  WriteCmd(fd, 0xf0); //--set divide ratio
  WriteCmd(fd, 0xd9); //--set pre-charge period
  WriteCmd(fd, 0x22); //
  WriteCmd(fd, 0xda); //--set com pins hardware configuration
  WriteCmd(fd, 0x12);
  WriteCmd(fd, 0xdb); //--set vcomh
  WriteCmd(fd, 0x20); //0x20,0.77xVcc 默认
  WriteCmd(fd, 0x8d); //--set DC-DC enable  
  WriteCmd(fd, 0x14); //  0x14 enable  0x10 disabled（sleep）  一般用于交流电设置
  WriteCmd(fd, 0xAF); //--turn on oled panel
}

/*写命令函数*/
void WriteCmd(int fd, unsigned char I2C_Command)
{
  wiringPiI2CWriteReg8(fd,0x00,I2C_Command);
}
