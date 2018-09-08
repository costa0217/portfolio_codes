#pragma once

//#include "GameFramework.h"
class CScene;

#define READ_PACKET(PROTOCOL)\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(Packet, Data);

// 클라이언트 통신을 담당할 CTestClientSession 개체
// CClientSession을 상속받아서 사용합니다.
class CLabClientSession : public CClientSession
{
public:

protected:
	// 클라이언트가 접속 성공되었을때 호출되는 가상함수
	VOID OnIoConnected(VOID);

	// 클라이언트가 접속 종료되었을때 호출되는 가상함수
	VOID OnIoDisconnected(VOID);

	//VOID OnIoRead(VOID);

private:
	
};
