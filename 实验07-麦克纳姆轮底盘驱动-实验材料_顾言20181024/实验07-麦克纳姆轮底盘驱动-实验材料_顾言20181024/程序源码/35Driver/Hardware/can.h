#ifndef __CAN_H__
#define __CAN_H__

#define CAN_ID_SPEED_MODE_ALL        0x04 //speed mode
#define CAN_ID_SPEED_MODE_NUM01      0x14
#define CAN_ID_SPEED_MODE_NUM02      0x24
#define CAN_ID_SPEED_MODE_NUM03      0x34
#define CAN_ID_SPEED_MODE_NUM04      0x44
#define CAN_ID_CRASH 0x64

void CAN_Configuration(void);
void CANWriteData(unsigned char add,char *s);
void CANSendCrash(short state1, short state2);

#endif 
