#include<reg51.h>

sbit BTN5=P1^1;
sbit BTN4=P1^0;
sbit BTN3=P1^3;
sbit BTN2=P1^2;
sbit BTN1=P1^5;
sbit BTN0=P1^4;

sbit LED0=P1^6;
sbit LED1=P1^7;

unsigned char  RECVBUF[64]={0};
unsigned int  RECVLEN=0,LASTLEN=0;
bit RECVCOMP=0;
 


void COMSendString(unsigned char*,int);
void COMSetup();
void Delay1ms(unsigned int);
void COMSendByte(unsigned char );
void WaitRecv();
void ConnTest();
bit CodeVerify();
void COMCMDStart();
void COMCMDEnd();

void COMSetup(){
		SCON=0x50;
	  TMOD=0x20;
	  PCON=0x80;
		TH1=0XFC;
		TL1=0XFC;
		EA=1;
	  ES=1; 
	  TR1=1;
		ConnTest();
		RECVCOMP=0;
}

unsigned char tCount=0;
void StartTM(){
		TMOD=TMOD|1;
		TF0=0;
		ET0 = 1;
		TH0=0x4C;
		TL0=0x00;
		TR0=0;
		TR0=1;
		tCount=0;
}
void COMSendByte(unsigned char dat){
		SBUF=dat;
		while(!TI);
		TI=0;
}
void COMSendString(unsigned char* dat,int len){
	  int lb=0;
		for(lb=0;lb<len;lb++){
					COMSendByte(dat[lb]);
		}
		Delay1ms(100);
}
void WaitRecv(){
		while(!RECVCOMP);
		RECVCOMP=0;
}
void ConnTest(){
		COMSendString("DC\n",4);
		WaitRecv();
		COMSendString(RECVBUF,LASTLEN);
		COMSendString("DCSKB!\n",7);
		LED0=0;
}
void BlinkLED(){
		LED0=0;
		LED1=1;
		Delay1ms(500);
		LED0=~LED0;
		LED1=~LED1;
		Delay1ms(600);
		LED0=~LED0;
		LED1=~LED1;
		Delay1ms(1000);
		LED0=0;
		LED1=0;
		Delay1ms(3200);
		LED0=1;
		LED1=1;
}
bit CodeVerify(){ 
		if( RECVBUF[0]=='D'&&RECVBUF[1]=='a'&&RECVBUF[2]=='c'&&RECVBUF[3]=='a'&&RECVBUF[4]=='i'){
					COMSendString("DCSKB STARTED!\n",15);LED1=0;
					Delay1ms(2000);
					BlinkLED();
					return 1;
			}else{
					COMSendString("WRONG CODE\n",11);
					return 0;
			}
}
void COMCMDStart(){
}
void COMCMDEnd(){
		
}
void Delay1ms(unsigned int c){
		unsigned char a,b;
		for(;c>0;c--){
			for(b=38;b>0;b--){
					for(a=12;a>0;a--);
			}
		}
}