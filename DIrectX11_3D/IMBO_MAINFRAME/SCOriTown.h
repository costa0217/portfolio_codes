#pragma once

#include "Scene.h"
#include "ImageUI.h"

//object
#include "FreeCamera.h"
class CLesserGiant;
class CSCOriTown;
class CHpBar;
class CImageUI;

class CSCOriTown :public CScene{

public:
	CSCOriTown(SCENE_ID eID, CDirectXFramework* pFrameWork);
	~CSCOriTown();
public:
	//-----------------------------------scene--------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void Animate(float fTimeElapsed);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput(float fTimeElapsed);
	CGameObject* PickObjectPointedByCursor(int xClient, int yClient);
	
	//network
	void NetworkProc();
	VOID PROC_PT_RANGE_SKILL_INFO_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_BOSS_CLEAR_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_BOSS_HP_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_PLAYER_HP_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_FTOWN_READY_SC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	//주기적 위치 동기화
	VOID PROC_PT_FREQUENCY_MOVE_SC(DWORD dwProtocol, BYTE *Packet, DWORD dwPacketLength);
	VOID PROC_PT_BOSS_FREQUENCY_MOVE_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_MOUSE_LEFT_ATTACK_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_FTOWN_NPC_READY_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_FTOWN_NPC_READY_COMP_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_FTOWN_BOSS_ACTION_CAMERA_READY_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_FTOWN_BOSS_ACTION_CAMERA_READY_COMP_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_FTOWN_NPC2_READY_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_FTOWN_NPC2_READY_COMP_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	CGameObject* GetPlayer() { return m_ppPawn ? m_ppPawn[NETWORKMGR->GetSLOT_ID()] : nullptr; }
	//test
	//vector<XMFLOAT3> m_vPickPos;
	//CGameObject* m_pObject{ nullptr };
	CGameObject** m_ppPawn{ nullptr };
	//CPlayer* m_pPlayer{ nullptr };
private:
	void HPBarProc();
	//float m_fHPDownSpeed{ 1.f };

	CHpBar* m_pPlayerHPUI;
	CHpBar* m_pBossHPUI;
	CImageUI* m_pBossHPacc;

	CHpBar* m_pTeamNoHPUI[2];

	CLesserGiant*	m_pBoss{ nullptr };

	CImageUI*	m_pResult{ nullptr };
	bool		m_bResult{ false };
	float		m_fResultAccTime{ 0.f };
	string		m_strResultName{};


	void StartBoss1ActionCam();
	void CreateBoss1();
	void KillBoss1();
	void FirstTownFly();
	void ReadMapData();

	void LoadSkillObjects();
	void CreateUI();

	bool m_bGameStart{ false };
	bool m_bFinalProc{ false };
	bool m_bStartBossCam{ false };
private:
	
};