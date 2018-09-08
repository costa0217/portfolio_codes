#ifndef Weapon_h__
#define Weapon_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourcesMgr;
	class CTimeMgr;
	class CInfoSubject;
	class CManagement;

	class CVIBuffer;
	class CTransform;

	class CCameraObserver;
	class CPlayerObserver;
}
class CPlayerObserver;
class CCameraObserver;
class CCollisionMgr;

class CWeapon
	: public Engine::CGameObject
{
private:
	explicit CWeapon(LPDIRECT3DDEVICE9 pDevice);

public:
	~CWeapon(void);
public:
	HRESULT		Initialize(void);
	int			Update(void);
	void		Render(void);

public:
	static CWeapon* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT		AddComponent(void);
	void		TransformMatrix(void);
	void		Move(void);
	void		Release();

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CManagement*	m_pManagement;
	CCollisionMgr*			m_pCollisionMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;

private:
	CCameraObserver*		m_pCameraObserver;
	CPlayerObserver*		m_pPlayerObserver;

private:
	Engine::VTXCOL*			m_pVertex;
	Engine::VTXCOL*			m_pConvertVertex;
	const Engine::VTXTEX*	m_pTerrainVertex;

	Engine::CGameObject*	m_pPlayer;

private:
	DWORD			m_dwVtxCnt;
	float			m_fAngleZ;
	float			m_fAngleX;
	float			m_fAngleY;

	D3DXMATRIX*		m_pParentMatrix;
	D3DXMATRIX*		m_pNoRotParMatrix;		// ȸ���� �ȸ��� �θ����

	bool			m_bDie;
	D3DXVECTOR3		m_vMyPos;
	
	WEAPON_TYPE		m_eType;		//�� Ÿ��

	float			m_fBulletTime;
	float			m_fFireSpeed;

	//�Ѿ˻��� ����
	typedef list<CGameObject*>				OBJECTLIST;
	typedef map<wstring, OBJECTLIST>		MAPOBJLIST;
	MAPOBJLIST*		m_pMapObjlist;

	//���� ����� �ϱ� ����
	wchar_t*		m_wstrBufferName;
	wchar_t*		m_CurrentBuffer;
	

public:
	//Ÿ�� ������
	Engine::CGameObject*	m_pTarget;
	Engine::CGameObject*	m_pBullet;


public:
	void SetParentMatrix(D3DXMATRIX* pParentMatrix){m_pParentMatrix = pParentMatrix;}
	void SetNoRotParMatrix(D3DXMATRIX* pNoRotParMatrix){m_pNoRotParMatrix = pNoRotParMatrix;}

	//void SetTarget(Engine::CGameObject* _pTarget){m_pTarget = _pTarget;}

	void SetPos(D3DXVECTOR3 vPos);
	void SetbDie(bool bDie){m_bDie = bDie;}

public:
	void SetType(WEAPON_TYPE _eType){ m_eType = _eType; }
	void SetMapObjlist(MAPOBJLIST* pMapObjlist){m_pMapObjlist = pMapObjlist;}
	void SetPlayer(Engine::CGameObject* _pPlayer){ m_pPlayer = _pPlayer; }
	void ChangeBuffer(wchar_t* BufferName);

private:
	void BasicBulletCreate(D3DXVECTOR3& vDir);		//������
	void MultiBulletCreate(D3DXVECTOR3& vDir);		// ��Ƽ
	void MissileCreate(D3DXVECTOR3& vDir);			//�̻���

	void FindTarget(D3DXVECTOR3& vDir);
 
};

#endif // Weapon_h__

