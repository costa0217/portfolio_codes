#pragma once
#include "Scene.h"
#include "FreeCamera.h"
#include "ImageUI.h"
#include "HpBar.h"

class CImageUI;
class CHpBar;
class CRoisa;
class CHpBar;
class CImageUI;
class CGameObject;

class CSCSarasen :
	public CScene
{
public:
	CSCSarasen(SCENE_ID eID, CDirectXFramework* pFrameWork);
	virtual ~CSCSarasen();


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
	VOID PROC_PT_BOSS_HP_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_BOSS_CLEAR_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_PLAYER_HP_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_SARASEN_READY_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);

	VOID PROC_PT_SARASEN_BOSS_START_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);

	VOID PROC_PT_SARASEN_BOSS_START_COMP_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);

	VOID PROC_PT_SARASEN_BOSS_ACTION_CAMERA_READY_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);

	VOID PROC_PT_SARASEN_BOSS_ACTION_CAMERA_READY_COMP_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);

	//주기적 위치 동기화
	VOID PROC_PT_FREQUENCY_MOVE_SC(DWORD dwProtocol, BYTE *Packet, DWORD dwPacketLength);
	VOID PROC_PT_BOSS_FREQUENCY_MOVE_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);
	VOID PROC_PT_MOUSE_LEFT_ATTACK_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength);

	CGameObject* GetPlayer() { return m_ppPawn ? m_ppPawn[NETWORKMGR->GetSLOT_ID()] : nullptr; }

private:
	void HPBarProc();

private:
	CHpBar* m_pPlayerHPUI{ nullptr };
	CHpBar* m_pBossHPUI{ nullptr };
	CImageUI* m_pBossHPacc{ nullptr };

	CHpBar* m_pTeamNoHPUI[2];

	CImageUI*	m_pResult{ nullptr };
	bool		m_bResult{ false };
	float		m_fResultAccTime{ 0.f };
	string		m_strResultName{};

	//cam
private:
	void ReadMapData();
	void LoadSkillObjects();
	void CreateUI();
	void StartBoss2ActionCam();
	void CreateBoss2();
	void KillBoss2();
	CRoisa* m_pBoss{ nullptr };

	bool m_bGameStart{ false };
	bool m_bStartBossCam{ false };
private:
	CGameObject** m_ppPawn{ nullptr };
};

