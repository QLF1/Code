#ifndef __USART5_H__
#define __USART5_H__

typedef struct{
	struct
	{
		unsigned short ch0;
		unsigned short ch1;
		unsigned short ch2;
		unsigned short ch3;
		unsigned char s1;
		unsigned char s2;
	}rc;
	struct
	{
		short x;
		short y;
		short z;
		unsigned char press_l;
		unsigned char press_r;
	}mouse;
	struct
	{
		unsigned short w,s,a,d,q,e,r,f,g,z,x,c,v,b,shift,ctrl;
	}key;
    
}RC_Ctl_t;

void UART5_Configuration(void);
void RC_Rst(void);
void RemoteReceive(volatile unsigned char rx_buffer[]);

#define RX_UART5_BUFFER 30
#endif

