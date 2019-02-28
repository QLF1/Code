#include "main.h"

PID WheelSpeed_Pid;
/***************************PID初始参数****************************************/
void PID_Init(void)
{
	WheelSpeed_Pid.P = 4.0f;					
	WheelSpeed_Pid.I = 1.0f;					
	WheelSpeed_Pid.D = 0.5f;					
	WheelSpeed_Pid.IMax = 1500.0f;
	WheelSpeed_Pid.SetPoint = 0.0f;
}

/*******************************PID计算****************************************/
float PIDCalc(PID *P, float NextPoint)
{
		float dError,Error;
		Error = P->SetPoint - NextPoint;
		
		P->SumError += Error;
		dError = Error-P->LastError;
		P->PreError = P->LastError;
		P->LastError = Error;
		
		if(P->SumError >= P->IMax)
			P->SumError = P->IMax;
		else if(P->SumError <= -P->IMax)
			P->SumError = -P->IMax;
		
		P->POut = P->P*Error;
		P->IOut = P->I*P->SumError;
		P->DOut = P->D*dError;
		
		return P->POut+P->IOut+P->DOut;
}
