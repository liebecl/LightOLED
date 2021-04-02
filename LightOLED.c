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

}
