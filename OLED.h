
typedef unsigned char uint8;
#define I2C_ADDRESS 0x3C
#define MAX_CHARACTER 16

void initOLED(void);
void clearDisplay(void);
void displayData(float data1,float data2);
void getTime(void);
void getIp(char *ip_buf);




