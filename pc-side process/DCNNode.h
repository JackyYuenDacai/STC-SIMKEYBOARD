#include<cstdlib>
#include<cstring>
#ifndef DCNode_H

#define DCNode_H
#define abs(value) (value<0?-value:value)

typedef unsigned char byte ;
class DCNStr
{
  	struct NodeLink
  	{
	  	unsigned char c;
		NodeLink*next,*unext;
	};
	NodeLink*head,*nail;
	NodeLink*pointer;
	struct TurnLog
	{
		void*Addr;
		TurnLog*next;
	};
	TurnLog*LogHead,*LogNow;
	long long pointNum;
	long long NodeCount;
	bool AutoFree;
	bool Mutex;
public:
	void Alloc(){
		head=(NodeLink*)malloc(sizeof(NodeLink));
		nail=head;
		head->unext=NULL;
		NodeCount=0;
		pointer=nail;
		pointNum=-1;
		LogHead=(TurnLog*)malloc(sizeof(TurnLog));
		LogNow=LogHead;LogNow->Addr=0;
		AutoFree=true;
		Mutex=false;
	}
	DCNStr(){
		Alloc();
	}
	DCNStr(long long len){
		Alloc();
		AddNode(len);
	}
	DCNStr(char let){
		Alloc();
		AddNode(1);
		head->next->c=let;
	}
	DCNStr(const char* str){
		Alloc();
		this->AddString((char*)str,strlen((char*)str));
	}
	void Settle(long long int Length=0)
	{
                if(head!=NULL)return;
		Alloc();
		AddNode(Length);
	}
	void AddNode(long long NCount){
		for(;NCount>0;NCount--){
			NodeCount++;
			nail->next=(NodeLink*)malloc(sizeof(NodeLink));
			nail->next->c=0;
                        (nail->next)->unext=nail;
                        nail=nail->next;
		}
	}
	void PushTop(long long count=1){
		for(int i=0;i<count;i++){
			NodeCount++;
			head->unext=(NodeLink*)malloc(sizeof(NodeLink));
			head->unext->next=head;
			head=head->unext;
			pointNum++;
		}
	}
	unsigned char& Top(){
		return (head->next->c);
	}
	void AddArrayLog(void*Addr){
		LogNow->Addr=Addr;
		LogNow->next=(TurnLog*)malloc(sizeof(TurnLog));
		LogNow=LogNow->next;LogNow->Addr=0;
	}
	void IsAutoFree(bool isn){
		AutoFree=isn;
	}
	void ClearArrayLog(){
		TurnLog*Enum;
		LogNow=LogHead;
		if(LogHead->Addr==0)
			return;
		free(LogHead->Addr),LogNow=LogHead->next;
		while(LogNow->Addr!=0)
		{
			free(LogNow->Addr);
                        Enum=LogNow;
			LogNow=LogNow->next;
                        free(Enum);
		}
		free(LogNow);
		LogNow=LogHead,LogNow->Addr=0;
	}
	NodeLink* NodeSet(long long index){
		while(Mutex==true);
		Mutex=true;
		if(index<0){
			Mutex=false;
			Insert(0,-index);
			return head->next;
		}
		if(index+1>NodeCount){
				Mutex=false;
       			return (AddNode((index+1)-NodeCount),nail);
       		}
       		if(abs(NodeCount/2-index)<abs(index-pointNum)){
       		NodeLink*retNode;
       		switch((bool)(index>=NodeCount/2)){
				case false:
					retNode=head->next;
					for(long long i=0;i<index;i++)
						retNode=retNode->next;
					pointNum=index;
					pointer=retNode;
					Mutex=false;
					return retNode;
				case true:
					retNode=nail;
					for(long long i=0;i<(NodeCount-index-1);i++)
						retNode=retNode->unext;
					pointNum=index;
					pointer=retNode;
					Mutex=false;
					return retNode;
			}
       		}
       		else{
       			switch((bool)(index>pointNum)){
       				case false:
       					for(long long i=pointNum;i>index;i--)
       						pointer=pointer->unext,
							pointNum--;
       					Mutex=false;
       					return pointer;
       				case true:
       					for(long long i=pointNum;i<index;i++)
       						pointer=pointer->next,
							pointNum++;
       					Mutex=false;
       					return pointer;
       			}
       			
       		}
       		return NULL;
	}
	void Insert(long long index,long long count=1){
		NodeLink*linkbuf=NodeSet(index)->unext;
		NodeLink*linkto=linkbuf->next;
		for(;count>0;count--){
			NodeCount++;
			linkbuf->next=(NodeLink*)malloc(sizeof(NodeLink));
                        linkbuf->next->unext=linkbuf;
                        linkbuf=linkbuf->next;
		}
		pointer=linkbuf;
		pointNum=index+count;
		linkto->unext=linkbuf;
		linkbuf->next=linkto;
	}
	void DelNode(long long index){	
		if(index>=NodeCount||index<0)
		return;
		if(index==NodeCount-1)
		{
			nail=(nail->unext);free(nail->next);
			nail->next=NULL;NodeCount--;
			pointer=nail;
			pointNum=NodeCount-2;
			return;
		}
		NodeLink*DelN=NodeSet(index);
		pointer=DelN->unext;
		pointNum=index-1;
		DelN->unext->next=DelN->next;
		DelN->next->unext=DelN->unext;
		free(DelN);
		NodeCount--;
		
	}
	void DelAll(){
		for(long long i=0;i<NodeCount;i++)
			nail=nail->unext,
			free(nail->next);
		NodeCount=0;
		head=nail;head->next=nail;
		nail->unext=head;
		pointer=head;
		pointNum=-1;
	}
	char*ToCharStr(long length=0){
		NodeLink*save=head->next;
		char*retArray;
		if(length!=0){		
        		retArray=(char*)malloc(size_t(sizeof(char)*(length+1)));
       			for(long long i=0;i<length;i++)
				retArray[i]=char(save->c),
				save=save->next;
			retArray[length]='\0';
			AddArrayLog(retArray);
			return retArray;
		}
        	retArray=(char*)malloc(size_t(sizeof(char)*(NodeCount+1)));
       		for(long long i=0;i<NodeCount;i++)
			retArray[i]=char(save->c),
			save=save->next;
		retArray[NodeCount]='\0';
		AddArrayLog(retArray);
		return retArray;
	}
	short*ToShortInt(){
		NodeLink*save=head->next;
        	short*retArray=(short*)malloc(size_t(sizeof(short)*(NodeCount+1)));
       		for(long long i=0;i<NodeCount;i++)
			retArray[i]=save->c,
			save=save->next;
		retArray[NodeCount]=0;
		AddArrayLog(retArray);
		return retArray;
	}
	operator char*(){
		return ToCharStr();
	}
	operator short*(){
		return ToShortInt();
	}
	operator const char*(){
		return (const char*)ToCharStr();
	}
	long long GetLength(){
		return NodeCount;
	}
	void AddString(char*Str,int length){
		NodeLink*save=nail;
		AddNode(length);save=save->next;
		for(long long i=0;i<length;i++)
		{
			save->c=Str[i];
			save=save->next;
		}
	}
	unsigned char& operator [](long long index){
		return (NodeSet(index)->c);
	}
	unsigned char& operator [](long index){
		return (NodeSet(index)->c);
	}	
	unsigned char& operator [](int index){
		return (NodeSet(index)->c);
	}
	unsigned char& operator [](short index){
		return (NodeSet(index)->c);
	}
	void operator =(char* Str){
		this->DelAll();
		this->AddString(Str,strlen(Str));
	}
	void operator =(const char* Str){
		this->DelAll();
		this->AddString((char*)Str,strlen((char*)Str));
	}
	void operator += (char Lett){
		NodeSet(NodeCount+1)->c=Lett;
		
	}
	void operator += (char* Str){
		AddString(Str,strlen(Str));
	}
	void operator += (unsigned char Lett){
		NodeSet(NodeCount+1)->c=(char)Lett;
	}
	void operator += (const char* Str){
		AddString((char*)Str,strlen((char*)Str));
	}
	void operator = (DCNStr&dcnode){
		DelAll();
		AddString(dcnode.ToCharStr(),(int)dcnode.GetLength());
	}
	friend DCNStr& operator + (DCNStr& arg0,char* arg1){
		static DCNStr adbuf;
		adbuf=arg0;
		adbuf+=arg1;
		return adbuf;
	}
	~DCNStr(){
		DelAll();
		if(AutoFree==true)ClearArrayLog();
		free(LogHead);
		free(head);
	}
};
template <class ANType>
class DCNode{
  	struct NodeLink
  	{
	  	void* data;
		NodeLink*next,*unext;
	};
	NodeLink*head,*nail;
	NodeLink*pointer;
	struct TurnLog
	{
		void*Addr;
		TurnLog*next;
	};
	TurnLog*LogHead,*LogNow;
	long long pointNum;
	long long NodeCount;
	size_t IndivSize;
	bool Mutex;
public:
	void Alloc(){
		head=(NodeLink*)malloc(sizeof(NodeLink));
		nail=head;
		IndivSize=sizeof(ANType);
		head->data=malloc(IndivSize);
		head->unext=NULL;
		NodeCount=0;
		pointer=nail;
		pointNum=-1;
		Mutex=false;
		LogHead=(TurnLog*)malloc(sizeof(TurnLog));
		LogNow=LogHead;LogNow->Addr=0;
	}
	DCNode(long long len){
		Alloc();
		AddNode(len);
	}
	DCNode(){
		Alloc();
	}
	void Settle(long long int Length=0)
	{
                if(head!=NULL)return;
		Alloc();
		AddNode(Length);
	}
	void PushTop(){
		NodeCount++;
		head->unext=(NodeLink*)malloc(sizeof(NodeLink));
		head->unext->next=head;
		head=head->unext;
		(head->next)->data=(void*)new ANType;
		if(pointNum!=-1)pointNum++;
	}
	void AddNode(long long NCount){
		for(;NCount>0;NCount--){
			NodeCount++;
			nail->next=(NodeLink*)malloc(sizeof(NodeLink));
			nail->next->data=(void*)new ANType;
                        (nail->next)->unext=nail;
                        nail=nail->next;
		}
	}
	ANType& New(){
		AddNode(1);
		return *(ANType*)(nail->data);
	}
	ANType& Last(){
		return *(ANType*)(nail->data);
	}
	ANType& Top(){
		return *(ANType*)(head->next->data);
	}
	void AddArrayLog(void*Addr){
		LogNow->Addr=Addr;
		LogNow->next=(TurnLog*)malloc(sizeof(TurnLog));
		LogNow=LogNow->next;LogNow->Addr=0;
	}
	void ClearArrayLog(){
		TurnLog*Enum;
		LogNow=LogHead;
		if(LogHead->Addr==0)
			return;
		free(LogHead->Addr),LogNow=LogHead->next;
		while(LogNow->Addr!=0)
		{
			free(LogNow->Addr);
                        Enum=LogNow;
			LogNow=LogNow->next;
                        free(Enum);
		}
		free(LogNow);
		LogNow=LogHead,LogNow->Addr=0;
	}
	NodeLink* NodeSet(long long index){
		while(Mutex==true);
		Mutex=true;
		if(index<0){
			Mutex=false;
			Insert(0,-index);
			return head->next;
		}
		if(index+1>NodeCount){
				Mutex=false;
       			return (AddNode((index+1)-NodeCount),nail);
       	}
       	if(abs(NodeCount/2-index)<abs(index-pointNum)){
       		NodeLink*retNode;
       		switch(index>=NodeCount/2){
				case false:
					retNode=head->next;
					for(long long i=0;i<index;i++)
						retNode=retNode->next;
					pointNum=index;
					pointer=retNode;
					Mutex=false;
					return retNode;
				case true:
					retNode=nail;
					for(long long i=0;i<(NodeCount-index-1);i++)
						retNode=retNode->unext;
					pointNum=index;
					pointer=retNode;
					Mutex=false;
					return retNode;
			}
       	}
       	else{
       			switch(index>pointNum){
       				case false:
       					for(long long i=pointNum;i>index;i--)
       						pointer=pointer->unext,
							pointNum--;
       					Mutex=false;
       					return pointer;
       				case true:
       					for(long long i=pointNum;i<index;i++)
       						pointer=pointer->next,
							pointNum++;
       					Mutex=false;
       					return pointer;
       			}
       			
       		}
       	return NULL;
	}
	void Insert(long long index,long long count=1){
		NodeLink*linkbuf=NodeSet(index)->unext;
		NodeLink*linkto=linkbuf->next;
		for(;count>0;count--){
			NodeCount++;
			linkbuf->next=(NodeLink*)malloc(sizeof(NodeLink));
			linkbuf->next->data=(void*)new ANType;
                        linkbuf->next->unext=linkbuf;
                        linkbuf=linkbuf->next;
		}
		pointer=linkbuf;
		pointNum=index+count;
		linkto->unext=linkbuf;
		linkbuf->next=linkto;
	}
	void Swap(long long a,long long b){
		NodeLink*FLink=NodeSet(a);
		NodeLink*SLink=NodeSet(b);
		void*data=FLink->data;
		FLink->data=SLink->data;
		SLink->data=data;
	}
	void DelNode(long long index){
		if(index>=NodeCount||index<0)
		return;
		if(index==NodeCount-1)
		{
			nail=(nail->unext);free(nail->next);
			nail->next=NULL;NodeCount--;
			return;
		}
		if(index<=pointNum){
			pointNum--;
			pointer=pointer->unext;
		}
		NodeLink*DelN=NodeSet(index);
		DelN->unext->next=DelN->next;
		DelN->next->unext=DelN->unext;
		delete DelN->data;
		free(DelN);
		NodeCount--;
	}
	void DelAll(){
		for(long long i=0;i<NodeCount;i++){
			nail=nail->unext;
			delete nail->next->data;
			free(nail->next);
		}
		NodeCount=0;
		head=nail;head->next=nail;
		nail->unext=head;
		pointer=head;
		pointNum=-1;
	}
	ANType*ToAnyType(){
		NodeLink*save=head->next;
        	ANType*retArray=(ANType*)malloc(size_t(IndivSize*(NodeCount)));
       		for(long long i=0;i<NodeCount;i++)
			retArray[i]=*(ANType*)(save->data),
			save=save->next;
		AddArrayLog(retArray);
		return retArray;
	}
	operator ANType*(){
		return ToAnyType();
	}
	operator const ANType*(){
		return (const ANType*)ToAnyType();
	}
	long long GetLength(){
		return NodeCount;
	}
	void AddArray(ANType*Array,int length){
		NodeLink*save=nail;
		AddNode(length);
		save=save->next;
		for(long long i=0;i<length;i++)
		{
			save->data=(void*)Array[i];
			save=save->next;
		}
		
	}
	ANType& operator [](long long index){
		return *(ANType*)NodeSet(index)->data;
	}
	ANType& operator [](long index){
		return *(ANType*)NodeSet(index)->data;
	}	
	ANType& operator [](int index){
		return *(ANType*)NodeSet(index)->data;
	}
	//ANType& operator [](short index){
	//	return *(ANType*)(NodeSet(index)->data);
	//}
	void operator +=(ANType Lett){
		*((ANType*)(NodeSet(NodeCount+1)->data))=Lett;
	}
	void operator=(DCNode&dcnode){
		DelAll();
		AddArray(dcnode.ToAnyType(),dcnode.GetLength());
	}
	~DCNode(){
		DelAll();
		ClearArrayLog();
		free(LogHead);
		free(head);
	}
};

#endif
