#include"func.h"

void PreSet(){
		BlinkLED();
		COMSetup();
		
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
						ET0=0;
						TR0=0;
						StartTM();
						 
						{
							KeyPed[KeypLen++]=KeyP+'0';
						}
						LED0=1;Delay1ms(100);LED0=0;Delay1ms(100);LED0=1;
				}else{
						Delay1ms(10);
				}
		}
}

void timer0() interrupt 1 using 1{
		static int ti=0;
		ti++;
		LED1=~LED1;
		if(ti>=15){
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
			}else if(dat==0xA5){
								COMCMDStart();RECVBUF[RECVLEN++]=dat;
				
			}else if(dat==0xEF){
								COMCMDEnd();RECVBUF[RECVLEN++]=dat;
								LASTLEN=RECVLEN;RECVLEN=0;RECVCOMP=1;
					
			}else{
								RECVBUF[RECVLEN++]=dat;
			}
			RI=0;TI=0;
}
