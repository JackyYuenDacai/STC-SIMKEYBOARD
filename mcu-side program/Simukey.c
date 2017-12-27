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
		COMSendString("CODE:\n",6);
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
		WaitRecv();
		COMSendString(RECVBUF,LASTLEN);
		COMSendString("CONN SUCCESS!\n",15);
}
bit CodeVerify(){ 
		if( RECVBUF[0]=='D'&&RECVBUF[1]=='a'&&RECVBUF[2]=='c'&&RECVBUF[3]=='a'&&RECVBUF[4]=='i'){
					COMSendString("Youre Allowed To Use Dc SIMUKEYB!\n",35);
					return 1;
			}else{
					COMSendString("WRONG CODE\n",12);
					return 0;
			}
}
void Delay1ms(unsigned int c){
		unsigned char a,b;
		for(;c>0;c--){
			for(b=38;b>0;b--){
					for(a=12;a>0;a--);
			}
		}
}
void PreSet(){
		COMSetup();
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
unsigned char getBtn(){
		if(BTN0==0){while(BTN0==0);return 1;}
		if(BTN1==0){while(BTN1==0);return 2;}
		if(BTN2==0){while(BTN2==0);return 3;}
		if(BTN3==0){while(BTN3==0);return 4;}
		if(BTN4==0){while(BTN4==0);return 5;}
		if(BTN5==0){while(BTN5==0);return 6;}
		return 0;	
}

void Verify(){
		unsigned char Ver=0;
		while(Ver==0){
			WaitRecv();
			LED0=~LED0;
			if(CodeVerify()){
					break;
			}else{
					RECVCOMP=0;
			}
		}
}

unsigned char KeyPed[16]={0};
unsigned char KeypLen=0;

int main(){
		unsigned char KeyP=0;  
		
		PreSet();
		Verify();
		while(1){
				
				KeyP=getBtn();
				if(KeyP!=0){
						StartTM();
						//COMSendByte(KeyP+'0');
						{
							KeyPed[KeypLen++]=KeyP+'0';
						}
						LED0=1;Delay1ms(50);LED0=0;Delay1ms(50);LED0=1;
				}else{
						Delay1ms(10);
				}
		}
}

void timer0() interrupt 1 using 1{
		static int ti=0;
		ti++;
		LED1=~LED1;
		if(ti>=21){
			LED1=~LED1;ti=0;
			COMSendByte(KeypLen+'0');
			COMSendString(KeyPed,KeypLen);
			COMSendByte('\n');
			KeypLen=0;
			TR0=0;
		}
}

void COMI() interrupt 4{
			unsigned char dat;
			dat=SBUF;
			if(dat=='\n'){
								RECVBUF[RECVLEN++]=dat;
								LASTLEN=RECVLEN;RECVLEN=0;RECVCOMP=1;
			}else{
								RECVBUF[RECVLEN++]=dat;
			}
			RI=0;TI=0;
}
