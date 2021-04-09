void OLED_Init(int fd);
void WriteCmd(int fd, unsigned char I2C_Command);
void WriteData(int fd, unsigned char I2C_Data);
void OLED_SetPos(int fd,unsigned char x, unsigned char y);
void Write_DataBuffer(void);
void OLED_Fill(void);
void OLED_CLS(void);
