#pragma once
#include "Scene.h"

struct HEORSEL_INFO {
	//slot
	INT SLOT_ID{ -2 };
	INT CHARACTER{ -1 };
	BOOL READY{ false };
};

class CSCHeroSel :
	public CScene
{
public:
	CSCHeroSel(SCENE_ID eID, CDirectXFramework* pFrameWork);
	~CSCHeroSel();

private:
	vector<CUIObject*>	m_vecButtonUI;
	vector<CUIObject*>	m_vecReadyUI;
	vector<CUIObject*>	m_vecSelCharUI;
	//vector<CUIObject*>	m_vecSelectCharUI;

	CGameObject*	m_pObject;

	int	m_iHeroSelNum{ -1 };

	bool m_bSceneChange{ false };
	HEORSEL_INFO m_pHEROSEL_INFO[4];//index == slot_id
public:
	//-----------------------------------scene--------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void Animate(float fTimeElapsed);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput(float fTimeElapsed);

	virtual void SetSelSceneInfo(int slot_id, int input_slot_id, bool is_ready, int character);

	void UIProc();
	//network
	void NetworkProc();
	VOID PROC_PT_ENTER_SERVER_SUC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_ROOM_CREATE_SC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_ROOM_JOIN_SC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_ROOM_JOIN_SCC_SC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_ROOM_DATA_CHANGE_SC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_ROOM_OUT_SC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_CHANGE_SLOTID_SC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_FTOWN_START_SC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
private:
	bool	m_bReady{ false };


private:
	void	KeyInput();
	void	CheckCollisionButton();

	void	MouseMoveForCamera();
private:
	POINT	m_ptOldCursorPos;
	float	m_cxDelta{ 0 };
};

