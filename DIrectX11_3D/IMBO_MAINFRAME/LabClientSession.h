#pragma once

//#include "GameFramework.h"
class CScene;

#define READ_PACKET(PROTOCOL)\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(Packet, Data);

// Ŭ���̾�Ʈ ����� ����� CTestClientSession ��ü
// CClientSession�� ��ӹ޾Ƽ� ����մϴ�.
class CLabClientSession : public CClientSession
{
public:

protected:
	// Ŭ���̾�Ʈ�� ���� �����Ǿ����� ȣ��Ǵ� �����Լ�
	VOID OnIoConnected(VOID);

	// Ŭ���̾�Ʈ�� ���� ����Ǿ����� ȣ��Ǵ� �����Լ�
	VOID OnIoDisconnected(VOID);

	//VOID OnIoRead(VOID);

private:
	
};
