#pragma once
#include "Scene.h"

struct tRoomData
{
	int iRoomNum;
	int	iJoinNum;
};

enum BUTTONOPT
{
	BUTT_ROOM1,
	BUTT_ROOM2,
	BUTT_LEFT,
	BUTT_RIGHT,
	BUTT_JOIN,
	BUTT_CREATE,
	BUTT_END,
};

class CSCLobby :
	public CScene
{
public:
	CSCLobby(SCENE_ID eID, CDirectXFramework* pFrameWork);
	~CSCLobby();
public:
	//-----------------------------------scene--------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void Animate(float fTimeElapsed);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput(float fTimeElapsed);

	virtual void SetSelSceneInfo(int slot_id, int input_slot_id, bool is_ready, int character);

	void NetworkProc();

	VOID	PROC_PT_ROOM_LIST_COUNT_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID	PROC_PT_ROOM_LIST_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);

private:
	void	PrepareUIObject();
	
	void	CheckCollisionButton();

private:
	vector<CUIObject*>	m_vecButtonUI;

	CUIObject*	m_pSelectUI;
	CUIObject*	m_pRoomListNum;
	/////////////////////
	CUIObject*	m_pRoom1Num10;
	CUIObject*	m_pRoom1Num1;
	CUIObject*	m_pRoom2Num10;
	CUIObject*	m_pRoom2Num1;
	CUIObject*	m_pRoom1Opt;
	CUIObject*	m_pRoom2Opt;
	/////////////////////
	CUIObject*	m_pRoom1Capa;
	CUIObject*	m_pRoom2Capa;



	int		m_iClickRoomNum{ -1 };
	int		m_iRoomListNum{ 0 };

private:
	bool	m_bSceneChange{ false };

private:
	list<tRoomData*>	m_pRoomList;
};

