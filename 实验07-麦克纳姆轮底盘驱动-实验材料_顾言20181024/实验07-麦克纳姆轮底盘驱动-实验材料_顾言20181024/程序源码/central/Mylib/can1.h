#ifndef __CAN1_H__
#define __CAN1_H__

#define RM820R_0x204_1 0x200
#define RM820R_0x207_5 0x1FF
void CAN1_Configration(void);
void ChassisSend(int a, int b, int c, int d);
void ChassisModeSend(int mode);

#endif 
