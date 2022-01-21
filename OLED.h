
typedef unsigned char uint8;
#define I2C_ADDRESS 0x3C
#define MAX_CHARACTER 16

void initOLED(int fd);
void clearDisplay(void);
void displayData(float data1,float data2);


