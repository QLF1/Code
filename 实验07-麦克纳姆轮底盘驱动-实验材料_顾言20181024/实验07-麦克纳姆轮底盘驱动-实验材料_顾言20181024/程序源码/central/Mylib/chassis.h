#ifndef __CHASSIS_H__
#define __CHASSIS_H__
#include <main.h>
void ChassisCal(void);
void ChassisPidInit(void);
void FollowPidInit(void); 
void Ymove(int vel);
void Xmove(int vel);
void Wmove(int vel);
void ALLmove(int velX, int velY, int velW);
void HoldOn(void);
void WOffset(float speed);
#endif
