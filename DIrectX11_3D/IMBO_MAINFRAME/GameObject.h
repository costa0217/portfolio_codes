#pragma once

#include "Component.h"
#include "Camera.h"
#include "Object.h"
#include "PositionInfoManager.h"
#include "EffectMgr.h"

class CTerrainContainer;
class CAnimater;

//rendercontainer 더미 클래스
class CRenderContainer;
//class CRenderContainerSeller;

typedef std::pair<component_id, CComponent*> pairComponent;
typedef std::map<component_id, CComponent*> mapComponent;


struct VS_VB_INSTANCE {
	XMMATRIX m_xmmtxWorld;
};
class CMesh;
struct StructLoadTextureFile {
	 CMesh* m_pMesh;
	string m_sName;
};

class CGameObject : public CObject {
public:

	bool	m_bDamaged{ false };

	//action move
	void ActionMoveStart(string sName);
	void ActionMoveEnd();
	void ActionMoveProc();
	bool m_bActionMove{ false };
	UINT m_CurPositionInfoIndex{ 0 };
	float m_fProgress{ 0.f };
	string m_sActionCamName;
	//----------------------------object-----------------------------
	bool Begin();
	virtual void Initialize() {};
	virtual bool End();
	//----------------------------object-----------------------------

	//object_id GetObjectID() { return m_objectID; }

	virtual void SetActive(bool b) { m_bActive = b; }
	bool& GetActive() { return m_bActive; }
	virtual void DisappearSkill() {};

	virtual bool IsCollision(CGameObject* pOther);
	virtual bool IsCollisionMeshToMesh(CGameObject* pOther);
	virtual bool IsCollisionAnimToAnim(CGameObject* pOther);
	virtual bool IsCollisionAnimToMesh(CGameObject* pOther);


	XMVECTOR GetRight();
	XMVECTOR GetUp();
	XMVECTOR GetLook();
	void SetRight(XMVECTOR xmvRight);
	void SetUp(XMVECTOR xmvUp);
	void SetLook(XMVECTOR xmvLook);

	void SetWorldMtx(XMMATRIX mtxWorld);
	virtual XMMATRIX GetWorldMtx();
	virtual void SetPosition(XMVECTOR pos);
	void SetPositionServer(XMVECTOR pos);
	void SetPositionX(const float pos);
	void SetPositionY(const float pos);
	void SetPositionZ(const float pos);
	float GetPositionX() { return m_xmf4x4World._41; };
	float GetPositionY() { return m_xmf4x4World._42; };
	float GetPositionZ() { return m_xmf4x4World._43; };
	XMVECTOR GetPosition();
	XMVECTOR GetQuaternion() { return XMLoadFloat4(&m_xmf4Quaternion); }
	void SetQuaternion(XMVECTOR xmv) { XMStoreFloat4(&m_xmf4Quaternion, xmv); }
	XMVECTOR GetScale() { return XMVectorSet(m_xmf4Scale.x*m_xmf4Scale.w, m_xmf4Scale.y*m_xmf4Scale.w, m_xmf4Scale.z*m_xmf4Scale.w, 1.0f); }
	void SetScale(XMVECTOR xmv);

	void SetRotation(XMMATRIX mtxRotation);

	//----------------------------component------------------------
	//test animate func
	virtual void Move(XMVECTOR xmvDir, float fDistance);
	virtual void Rotate(XMMATRIX xmMtx);
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void RotateWorldAxis(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	float& GetRotateX() { return m_xmf3Rotate.x; };
	float& GetRotateY() { return m_xmf3Rotate.y; };
	float& GetRotateZ() { return m_xmf3Rotate.z; };

	//자신의 component 실행 + a
	virtual void Animate(float fTimeElapsed);
	bool SetComponent(CComponent* pComponenet);
	CComponent* GetComponenet(const component_id& componenetID);
	const CComponent* GetComponenet(const component_id& componenetID)const;
	void ClearComponents();
	//----------------------------component------------------------

	//instance buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance,  CCamera* pCamera);
	
	//render container controll
	virtual void RegistToContainer();
	CRenderContainer* GetRenderContainer() { return m_pRenderContainer; }

	//debuger container controll
	virtual void RegistToDebuger();

	//space
	void SetSpaceIndex(int index) { m_spaceIndex = index; }
	int GetSpaceIndex() { return m_spaceIndex;	}

	//terrain 
	float GetTerrainHeight();
	void SetTerrainContainer(CTerrainContainer* pTerrainContainer) { m_pTerrainContainer = pTerrainContainer; };

	//aabb
	virtual bool IsVisible( CCamera* pCamera);//계층구조의 녀석들은 다시 만들어줄 필요가 있음
	void GetMainBoundingBox(BoundingBox& out);
	//ray picking 
	virtual bool CheckPickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distance);
	virtual void PickingProc();

	//Collision
	virtual void PhisicsLogic(map<utag, list<CGameObject*>>& mlpObject, float fDeltaTime) {};

	//world
	XMFLOAT4X4 m_xmf4x4World;
	XMFLOAT3 m_xmf3Position{ 0.f, 0.f, 0.f };
	XMFLOAT4 m_xmf4Quaternion;
	XMFLOAT4 m_xmf4Scale{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT3 m_xmf3Rotate{ 0.f,0.f, 0.f };

	//texture select ui를 위한 함수
	int& GetSelectMeshIndex() { return m_indexSelectMesh; }
	void SetSelectMeshIndex(float& index) { 
		m_indexSelectMesh = (int)index; 
	}

	 CAnimater* GetAnimater() { return m_pAnimater; }
	vector<BoundingOrientedBox>& GetObjectActiveOBBs() { return m_vObjectActiveOBBs; }

	virtual void LoadInfo();

	static CGameObject* CreateObject(string name, tag t, XMMATRIX xmmtxWorld);

	//navi mesh index
	void SetNaviMeshIndex(int index);
	void SetNaviMeshIndex();

	//demage proc
	
	virtual bool GetDemaged(int iDemage) { 
		m_iCurHP = MAX(0, m_iCurHP - iDemage);

		return true; 
	};
	virtual bool GetHeal(int iHeal) { 
		m_iCurHP = MIN(m_iMaxHP, m_iCurHP + iHeal);
		return true; 
	}
	int SetHP(int hp) { m_iCurHP = hp; return m_iCurHP; }
	bool GetbCollision() { return m_bCollision; }
	void SetbCollision(bool b) { m_bCollision = b; }
public:
	BoundingBox*	GetBBox() { return &m_OriBoundingBox; }


	bool SkillCollision(CGameObject* pDest, bool bRelative = true);
	//충돌 처리할 때 
	float m_fAnimTime{ 0.f };//현재 진행중인 Animation time
	float m_fCollisionTime{ 0.f };//collision 주기
	XMFLOAT3 m_xmf3CollisionOffset;//충돌체가 있을 offset 위치
	float m_fMinCollisionOffsetTime{ 0.f };//충돌체가 생성될 시간
	float m_fMaxCollisionOffsetTime{ 0.f };//충돌체가 생성될 시간
	bool m_bCollision{ false };//충돌은 한번만 할꺼니까. .
	float m_fRadius{ 0.f };
	void ResetCollisionValue(XMFLOAT3 xmf3CollisionOffset, float fMin, float fMax, float fRadius) {
		m_fCollisionTime = 0.f;
		m_xmf3CollisionOffset = xmf3CollisionOffset;
		m_fMinCollisionOffsetTime = fMin;
		m_fMaxCollisionOffsetTime = fMax;
		m_fRadius = fRadius;
		m_fAnimTime = 0.f;
		m_bCollision = false;
	}

	//maxhp가 set되면 cur hp도 maxhp로 변한다. 
	void ResetHPValues(int fMaxHP, int fCurHP) {
		m_iMaxHP = fMaxHP;
		m_iCurHP = fCurHP;
	}
	void SetMaxHP(int fMaxHP) {
		m_iMaxHP = fMaxHP;
	};
	int GetCurHp() { return m_iCurHP; }
	int GetMaxHp() { return m_iMaxHP; }
	void SetCurHP(int hp){ 
		if (hp < 0) m_iCurHP = 0;
		else if (hp > m_iMaxHP)m_iCurHP = m_iMaxHP;
		else m_iCurHP = hp;
	}
	void GetDefenceValue(float& fDefence, float& fCurDefence) {
		fDefence = m_fDefence;
		fCurDefence = m_fCurDefence;
	}
	void SetDefenceValue(float fDefence, float fCurDefence) {
		float m_fCurDefence{ 1.f };//디펜스는 퍼센트로 대미지 감소시킴 
		float m_fDefence{ 1.f };
	}
	virtual void GetSkilled(int nSkill);
	virtual int GetSlotID() { return 0; }

protected:
	//navi mesh index
	int m_indexNaviMesh{ -1 };

	int m_indexSelectMesh{ 0 };

	BoundingBox m_OriBoundingBox;

	bool m_bIsVisible{ true }; 
	bool m_bActive{ true };
	

	//terrain container
	CTerrainContainer*	m_pTerrainContainer{ nullptr };

	//component
	mapComponent		m_mapComponents;
	CRenderContainer*	m_pRenderContainer{ nullptr };

	//space
	int m_spaceIndex{ 0 };
	//animater
	 CAnimater* m_pAnimater{ nullptr };
	vector<BoundingOrientedBox> m_vObjectActiveOBBs;

protected:
	bool		m_bStrong{ false };
	XMFLOAT4	m_xmf4RimColor;
	CBuffer*	m_pRimCBuffer{ nullptr };
	bool		m_bRimSwitch{ false };
	float		m_fRimAccTime{ 0.f };
public:
	void	SetStrong(bool bStrong) { m_bStrong = bStrong; };
	void	MappingRimLight(float fDeltaTime);
	void	SetRimLight(){
		m_fRimAccTime = 0.f;
		m_bRimSwitch = true;

		CEffectMgr::GetInstance()->Play_Effect(L"blood2", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 3.f, m_xmf3Position.z, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
	}


protected:
	// 버프 지속시간 이딴거 해야함
	//hp변수
	int m_iCurHP{ 1000 };
	int m_iMaxHP{ 1000 };
	//데미지 변수
	float m_fCurDefence{ 1.f };//디펜스는 퍼센트로 대미지 감소시킴 
	float m_fDefence{ 1.f };
	int m_iCurAttack{ 100 };
	int m_iAttack{ 100 };

	bool		m_bIdle{ false };
	CCamera*	m_pCamera{ nullptr };
	bool		m_bJump{ false };
	bool		m_bStay{ false };

public:
	//stay flag
	void SetbStay(bool b) { m_bStay = b; }
	bool GetbStay() { return m_bStay; }

	void SetIdleState(bool bIdle) { m_bIdle = bIdle; }
	bool GetIdleState() { return m_bIdle; }

	void SetCamera(CCamera* pCam) { m_pCamera = pCam; }

public:
	CGameObject(string name, tag t = tag::TAG_DEFAULT);
	virtual ~CGameObject();
};