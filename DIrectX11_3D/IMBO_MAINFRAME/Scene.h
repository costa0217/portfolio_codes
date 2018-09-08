#pragma once

#include "DXObject.h"
#include "GameObject.h"

class CDirectXFramework;


class CScene{

public:
	//begin end
	virtual bool Begin() { return true; };
	virtual bool Begin(string path);
	virtual bool End();
	//begin end
	//animate
	virtual void Animate(float fTimeElapsed);
	//animate
	//input
	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {};
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {};
	virtual void ProcessInput(float fTimeElapsed) {};
	//input
	//pick
	virtual CGameObject* PickObjectPointedByCursor(int xClient, int yClient) { return nullptr; };
	bool GetPickPositionByCursor(int xClient, int yClient, XMFLOAT3& output, UINT tag = TAG_STATIC_OBJECT | TAG_TERRAIN);
	//pick
	//create scene
	static CScene* CreateScene(string name, CDirectXFramework* pFramework, SCENE_ID eID);
	//create scene

	void LoadScene(string path);

	//load resource
	void CScene::LoadResource(wstring wsMeshRoot);

	//sel scene func
	virtual void SetSelSceneInfo(int id, int character, bool is_ready) {};
protected:
	string m_sName;

	//framework
	CDirectXFramework* m_pFrameWork{ nullptr };
	 CCamera* m_pCamera{ nullptr };
	CGameObject*	m_pPickingObject{ nullptr };

	SCENE_ID		m_eSceneID{ SCN_END };
	volatile bool			m_bIsLoading{ false };

	vector<CUIObject*>	m_vecUI;
public:
	bool		GetIsLoading() { return m_bIsLoading; };

	//flag 충돌 처리
	bool FlagCollision(CGameObject* pDest);
	bool m_bCollision{ false };
	XMFLOAT3 m_xmf3CollisionOffset;//충돌체가 있을 offset 위치
	float m_fRadius{ 0.f };
	void ResetCollisionValue(XMFLOAT3 xmf3CollisionOffset, float fRadius) {
		m_xmf3CollisionOffset = xmf3CollisionOffset;
		m_fRadius = fRadius;
	}
public:
	CScene::CScene(SCENE_ID eID){ m_eSceneID = eID; }
	~CScene() {}

	//set get
	void SetFrameWork(CDirectXFramework* pFramework) { m_pFrameWork = pFramework; }
	CDirectXFramework* GetFrameWork() { return m_pFrameWork; }
	//set get
	SCENE_ID	GetSceneID() { return m_eSceneID; }
};
