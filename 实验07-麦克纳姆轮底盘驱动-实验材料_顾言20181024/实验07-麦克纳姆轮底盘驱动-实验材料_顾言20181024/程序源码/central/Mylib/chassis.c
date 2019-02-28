/************************************************************************************
底盘控制，包括遥控器模式下底盘计算，以及封装好的X,Y,W方向旋转函数
电机发送正值向上，发送负值向下
***************************************************************************************/

#include "stm32f4xx.h"
#include "main.h"
#include "chassis.h"
#include "can1.h"
#include "uart5.h"

float carSpeed[4];
int carSpeedX, carSpeedY, carSpeedW;
extern RC_Ctl_t RC_Ctl; 
int ctrl_mode;
int tick;
/**
  * @brief  底盘速度计算
  * @param  None
  * @retval None
  */
void ChassisCal(void)
{
	if(RC_Ctl.rc.s1 == 3)
	{
		if(RC_Ctl.rc.ch1 > 1044 || RC_Ctl.rc.ch1 < 1004)
			carSpeedY = (1024 - RC_Ctl.rc.ch1) / 3.0f;
		else 
			carSpeedY = 0;
		if(RC_Ctl.rc.ch0 > 1044 || RC_Ctl.rc.ch0 < 1004)
			carSpeedX = (1024 - RC_Ctl.rc.ch0) / 3.0f;
		else 
			carSpeedX = 0;
		if(RC_Ctl.rc.ch2 > 1044 || RC_Ctl.rc.ch2 < 1004)
			carSpeedW = (1024 - RC_Ctl.rc.ch2) / 3.0f;
		else
			carSpeedW = 0;
		tick = 0;
	}
	else if(RC_Ctl.rc.s1 == 1)
	{
		tick++;
		/***************在此部分设定自选动作******************/
		if(tick <= 200)
		{
			carSpeedX = -50;
			carSpeedY = 0;
			carSpeedW = 0;
		}
		if(tick > 200 && tick <= 400)
		{
			carSpeedX = 0;
			carSpeedY = 50;
			carSpeedW = 0;
		}
		if(tick > 400 && tick <= 600)
		{
			carSpeedX = 50;
			carSpeedY = 0;
			carSpeedW = 0;
		}
		if(tick > 600 && tick <= 800)
		{
			carSpeedX = 0;
			carSpeedY = -50;
			carSpeedW = 0;
		}
		if(tick > 800)
		{
			carSpeedX = 0;
			carSpeedY = 0;
			carSpeedW = 0;
		}
		/***************************************************/
	}
	else 
	{
		tick = 0;
		carSpeedX = 0;
		carSpeedY = 0;
		carSpeedW = 0;
	}
/*********************此处做麦克纳姆轮速度分解******************/
//	carSpeed[0] = + carSpeedX + carSpeedY + carSpeedW;
//	carSpeed[1] = + carSpeedX + carSpeedY + carSpeedW;    
//	carSpeed[2] = + carSpeedX + carSpeedY + carSpeedW;
//	carSpeed[3] = + carSpeedX + carSpeedY + carSpeedW; 
/*************此处做直流电机负反馈实验，只控制一个电机**********/
	carSpeed[0] = -carSpeedY;
	carSpeed[1] = -carSpeedY;
	carSpeed[2] = +carSpeedY;
	carSpeed[3] = +carSpeedY;
	
	if(RC_Ctl.rc.s2 == 3)
		ctrl_mode = 1;
	else if(RC_Ctl.rc.s2 == 2)
		ctrl_mode = 0;
	ChassisModeSend(ctrl_mode);

	ChassisSend(carSpeed[0], carSpeed[1], carSpeed[2], carSpeed[3]);
}


