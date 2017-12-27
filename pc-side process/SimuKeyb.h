#include<Windows.h>
#include<WinUser.h>
#include <mmsystem.h> 
#include<cstdio>
#include<string>
#include<map>
#include<time.h>
#include"config.h"
#include"resource.h"
#include <tchar.h>
#include <strsafe.h>
#pragma comment(lib,"winmm.lib")

using namespace std;

#define TOBP(a) ((byte*)a)
#define CONFIG ".simukb.conf"
bool VerifyCOM();
bool SetupCOM();
void ConHardware();
void DataRecv(DWORD);
bool RecvData();
void DataAna(FILE*fp);
void RecvError();
 
void LoadConfig();void SendData(byte*,int );void WaitRecv();
HANDLE hCom=(HANDLE)-1;
byte RecvBuf[512]={0};
DWORD RecvLen=0;
byte SendBuf[1024]={0};
DWORD SendLen=0;
map<long, DWORD> KeyBind;
short RecvSignal=0;
bool EnterUser=false;
void COMError();
DWORD WINAPI SimuMain(){
	LoadConfig();
	ConHardware();

	EnterUser=true;
	while(1){
			RecvData();

	}
	return TRUE;
}
void ConHardware(){
	while(SetupCOM()==false);
	printf("COM SETUP!\n");	
	SendData(TOBP("Dacai\n\n"),7);
	PlaySound(MAKEINTRESOURCE(IDR_WAVE1 ),NULL,SND_ASYNC|SND_RESOURCE);
	RecvData();
}
bool VerifyCOM(){
	
	Sleep(100);
	RecvData();
	if(RecvBuf[0]=='D'||RecvBuf[1]=='C'){
		  SendData(TOBP("Hello World\n"),12);RecvData();
		printf("VERIFY SUCCESS!\n");
		return true;
	}
	printf("VERIFY FAILED!\n");
	return false;

}
bool SetupCOM(){
	char COMName[15]={0};
	DWORD len=0;
	_DCB ComState={0};
	COMMTIMEOUTS ComTimeOuts;
	for(int i=3;i<10;i++){
		sprintf_s(COMName,"COM%d",i);
		printf("Try COM%d\n",i);
		hCom=CreateFile((char*)COMName,GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							0,
							NULL);
		if(hCom==(HANDLE)-1){
			Sleep(100);
			continue;
		}
		
		GetCommState(hCom,&ComState);
		ComState.BaudRate=14400;
		ComState.StopBits=2;
		ComState.Parity=NOPARITY;
		ComState.ByteSize=8;
		SetCommState(hCom,&ComState);
		ComTimeOuts.ReadIntervalTimeout=150;
		ComTimeOuts.ReadTotalTimeoutConstant=150;
		ComTimeOuts.ReadTotalTimeoutMultiplier=150;
		ComTimeOuts.WriteTotalTimeoutConstant=100;
		ComTimeOuts.WriteTotalTimeoutMultiplier=100;
		SetCommTimeouts(hCom,&ComTimeOuts);
		SetupComm(hCom,512,512);
		PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);
		

		while(!VerifyCOM());
		break;
	 }
	if(hCom==(HANDLE)-1)return false;
	return true;
}
void LoadConfig(){
	FILE* fp=fopen(CONFIG,"r");
	if(fp==NULL){
			fp=fopen(CONFIG,"w+");
			fwrite(OriConfig,1,strlen(OriConfig),fp);
	}else{
			printf("FILE EXIST!\nKEY MAP:\n");
	}
	DataAna(fp);
	fclose(fp);
}
bool RecvData(){
	DWORD Len;
	 if(!ReadFile(hCom,RecvBuf,512,&Len,NULL)){
			COMError();
			return 0;
	}
	 	
	 if(Len>1)
	 DataRecv(Len);
	 return   1;
}
void DataRecv(DWORD Len){
	RecvBuf[Len]=0;	
	printf("RECV:%s\n",RecvBuf);
	if(EnterUser==true&&RecvBuf[0]=='D'&&RecvBuf[1]=='C'&&Len<=5){
		SendData(TOBP("Hello World\n"),12);RecvData();
		SendData(TOBP("Dacai\n\n"),7);
		EnterUser=true;
		PlaySound(MAKEINTRESOURCE(IDR_WAVE1 ),NULL,SND_ASYNC|SND_RESOURCE);return;
	}
	if(EnterUser==false){
			return;
	}
	long TKeys=atoi((char*)RecvBuf+1);
	if((KeyBind.find(TKeys)!=KeyBind.end())){
		printf("K:%d\n",KeyBind[TKeys]);
		  INPUT input[2]; 
		  memset(input, 0, 2 * sizeof(INPUT)); 

		  input[0].type = INPUT_KEYBOARD; 
		  input[0].ki.wVk = KeyBind[TKeys]; 
		  SendInput(1, input, sizeof(INPUT)); 

		  input[1].type = INPUT_KEYBOARD; 
		  input[1].ki.wVk =KeyBind[TKeys]; 
		  input[1].ki.dwFlags = KEYEVENTF_KEYUP; 

		  SendInput(1, input + 1, sizeof(INPUT));

	}
}
void SendData(byte*dat,int len){
		sprintf((char*)SendBuf,"%s",(char*)dat);
		printf("SEND:%s\n",SendBuf);
		SendLen=len;
		DWORD Len=0;
		PurgeComm(hCom,PURGE_TXCLEAR);
		if(!WriteFile(hCom,dat,len,&Len,NULL)){
				COMError();
		}
}
 
void DataAna(FILE*fp){
	char fBuf[512]={0};
	int CataCount=0;
	int CataNo=0,BindCount=0;
	int SIMKey=0,TKeys=0;
	fscanf(fp,"%d",&CataCount);
	fgets(fBuf,512,fp);
	for(int i=0;i<CataCount;i++){
			fscanf(fp,"*%d %d",&CataNo,&BindCount);fgets(fBuf,512,fp);
			for(int b=0;b<BindCount;b++){
					fscanf(fp,"%d %d",&TKeys,&SIMKey);
					fgets(fBuf,512,fp);
					KeyBind[TKeys]=SIMKey;
					printf("%d %d\n",TKeys,SIMKey);
			}
	}
	
}
void COMError(){
	printf("ERROR!\n");
	 CloseHandle(hCom);
	 hCom=(HANDLE)-1;
	ConHardware();
}
DWORD WINAPI FuncLoop(void*n){
		DWORD Len;
	while(1){
			
	 if(!ReadFile(hCom,RecvBuf,512,&Len,NULL)){
			COMError();
			return 0;
	}
	 	
	 if(Len>1)
	 DataRecv(Len);
	}
}