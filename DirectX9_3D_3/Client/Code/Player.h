/*!
 * \file Player.h
 * \date 2015/07/09 12:45
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Player_h__
#define Player_h__


#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourcesMgr;
	class CTimeMgr;
	class CInfoSubject;
	class CManagement;

	class CTexture;
	class CVIBuffer;
	class CTransform;

	class CCameraObserver;
}

class CCameraObserver;
class CCollisionMgr;
class CMouseCol;
class CWireHead;
class CNemo;//0804

class CTerrainCol;
class CPropeller;
class CBasicBullet;

class CPlayer
	: public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPlayer(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void KeyCheck(void);
	void Move(void);
	void Release(void);

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CManagement*	m_pManagement;
	CCollisionMgr*			m_pCollisionMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;
	CTerrainCol*			m_pTerrainCol;
	CMouseCol*				m_pMouseCol;

private:
	CCameraObserver*		m_pCameraObserver;

private:
	Engine::VTXCOL*		m_pVertex;
	Engine::VTXCOL*		m_pConvertVertex;
	const Engine::VTXTEX*		m_pTerrainVertex;

	Engine::CGameObject* m_Dust;
	Engine::CGameObject* m_pTemp;
	Engine::CGameObject* m_pWire;

	//미사일변수
	Engine::CGameObject*	m_pBasicBullet;
	Engine::CGameObject*	m_pMissileBullet;
	Engine::CGameObject*	m_pMultiBullet;
	Engine::CGameObject*	m_pBomb;
	//
	//총구변수
	Engine::CGameObject*	m_pWeapon;

	
	//프로펠러변수
	Engine::CGameObject*	m_pPropeller;
	Engine::CGameObject*	m_pMiniPropeller;

	bool bPropellerSetPos;
	//

	CWireHead*	    m_pWireHead;
	
	//OBJ_STATE m_eState;
	D3DXVECTOR3 m_vDieMove;

	//
	float m_fCollisionSpeed;
	bool m_bCollisionDamage;
	//
	bool m_bFragmentCreate;

	//

	D3DXVECTOR3 m_vBombFragmentPos;

	//버튼을땟을때 와이어를 움직이기위한 불변수.
	bool	m_bLeftKeyDown;
	bool	m_bRightKeyDown;
	//

private:

	//
	wchar_t*		m_wstrBufferName;
	wchar_t*		m_CurrentBuffer;

	float			m_fMaxPosY;

	float			m_fBefore_AngZ_Limit;
	float			m_fAngleZ_Speed;
	float			m_fAngleZ_Stabilizing_Speed;
	float			m_fAngleY_Speed;
	float			m_fAngleX_Speed;
	float			m_fAngleZReturn_Speed;
	float			m_fX_Speed;
	float			m_fY_Speed;
	float			m_fWireX_Speed;
	float			m_fBeforeAngZ_Speed;
	float			m_fInertiaX_Speed;
	float			m_fAngZTime_Speed;
	float			m_fTime_Speed;
	//
	float			m_fAngZTime;
	bool			m_bAngZzero;
	bool			m_bBefore_AngZ_Decrease;
	float			m_fBefore_AngZ;

	bool			m_bLeftAngZ_Stabilizing;
	bool			m_bRightAngZ_Stabilizing;

	bool			m_bRightButtonUp;
	bool			m_bLeftButtonUp;

	D3DXVECTOR3     m_vUpDir;

	float			m_fRightSpeed;
	float			m_fLeftSpeed;
	bool			m_bWireMove;
	bool			m_bWireRightMove;
	bool			m_bWireLeftMove;

	bool			m_bBomb;
	float			m_BulletTime;
	float			m_fSmokeTime;
	float			m_fDustTime;


	//float			m_fHp;

	float			m_fBullet;
	bool			m_bJump;
	float			m_fTime;
	float			m_fAngleZ;
	float			m_fAngleY;
	float			m_fAngleX;
	
	float			m_fSpeed;
	DWORD			m_dwVtxCnt;
	typedef list<CGameObject*>				OBJECTLIST;
	typedef map<wstring, OBJECTLIST>		MAPOBJLIST;
	MAPOBJLIST*		m_pMapObjlist;
	MAPOBJLIST*		m_pMapObjlistUI;

	float m_fFragmentCreateTime;

	//
	//다리좌표
	D3DXVECTOR3  m_vLeftLegPos;
	D3DXVECTOR3  m_vRightLegPos;

	D3DXVECTOR3  m_vOriginLeftLegPos;
	D3DXVECTOR3  m_vOriginRightLegPos;

	bool		m_bHeliCol;

	float m_fAngleRevolveZ;
	float m_fOriginAngleRevolveZ;
	//

	//타겟 포인터
	Engine::CGameObject*	m_pTarget;

	//
	bool	m_bGravity;

	//
	bool  m_bStart;
	bool  m_bAlreadyStart;
	//
	//
	bool m_bTreeCollision;
	//
	float m_fDieAngleZ;
	float m_fDieAngleZ_Speed;

	//임시
	TCHAR			m_szFps[128];
	//

	//플레이어와 버튼 연결
	typedef map<wstring, Engine::CGameObject*>		BUTTONLIST;
	BUTTONLIST*		m_pButtonlist;

	bool	m_bOnce;


private:
	list<CNemo*>	m_NemoList; //0804

public:
	void SetScale(const D3DXVECTOR3& vScale);//0804
	void AddNemo(CNemo*	pNemo);//0804
	list<CNemo*>* GetNemoList(void);//0804
	
public:
	void SetSetMapObjlistUI(MAPOBJLIST* pMapObjlist){m_pMapObjlistUI = pMapObjlist;}
	void SetMapObjlist(MAPOBJLIST* pMapObjlist){m_pMapObjlist = pMapObjlist;}
	void JumpUpdate();
	void DustCreate();
	void SmokeCreate();

	void BombCreate();
	void PropellerCreate();


	void TreeCollision();

	/*void BasicBulletCreate();
	void MissileCreate();
	void MultiBulletCreate();*/

	/*void GunCreate();
	void GatlingGunCreate();
	void LauncherCreate();*/

	void WeaponCreate(wchar_t* BufferName, WEAPON_TYPE _eType);

	void FragmentCreate();

	void ChangeBuffer(wchar_t* BufferName);


public:
	//void FindTarget(D3DXVECTOR3& vDir);
	void SetHP(float rHP);
	void SetBulltCnt(float fBullet){m_fBullet = fBullet;}
	float GetBulltCnt(){return m_fBullet;}
public:
	//유아이 변수
	int		m_iBoardNum;			// 현재 탑승 인원
	int		m_iMAXBoardNum;			// 총 탑승 인원

public:
	void	SetBoardNum();

	int		GetBoardNum();
	int		GetMAXBoardNum(); 
	
};

#endif // Player_h__