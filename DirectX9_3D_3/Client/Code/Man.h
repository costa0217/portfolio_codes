#ifndef Man_h__
#define Man_h__


#include "GameObject.h"
#include "ResourcesMgr.h"
#include "Include.h"

namespace Engine
{
	class CResourcesMgr;
	class CManagement;
	class CTimeMgr;
	class CInfoSubject;

	class CTexture;
	class CVIBuffer;
	class CTransform;

	class CCameraObserver;
}
enum PART_TYPE	{ PART_HART, PART_BODY, PART_HEAD, PART_LEFTARM, PART_RIGHTARM,
PART_LOWER, PART_LEFTLEG, PART_RIGHTLEG, PART_END };

enum SCALE_STATE { SCALE_NO, SCALE_YES, SCALE_END };

class CPlayerObserver;
class CCameraObserver;
class CCollisionMgr;
class CMouseCol;
class CTerrainCol;

class CMan
	: public Engine::CGameObject
{
private:
	explicit CMan(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CMan(void);

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CManagement*	m_pManagement;
	CCollisionMgr*			m_pCollisionMgr;
private:
	DWORD					m_dwVtxCnt;

private:
	Engine::VTXCUBE*		m_pVertex[PART_END];
	Engine::VTXCUBE*		m_pConvertVertex[PART_END];
	const Engine::VTXTEX*	m_pTerrainVertex;

	Engine::CVIBuffer*		m_pBuffer[PART_END];
	Engine::CTexture*		m_pTexture[PART_END];

	Engine::CTransform*		m_pInfo[PART_END];

public:
	CTerrainCol*			m_pTerrainCol;

private:
	CCameraObserver*		m_pCameraObserver;
	CPlayerObserver*		m_pPlayerObserver;

	Engine::CGameObject*	m_pPlayer;

public:
	static CMan* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual HRESULT		Initialize(void);
	virtual int			Update(void);
	virtual void		Render(void);

public:
	void		SetPos(D3DXVECTOR3& rPos);
	void		SetPlayer(Engine::CGameObject*	_pPlayer){ m_pPlayer = _pPlayer; }

private:
	void		TransformMatrix(void);
	void		CombineParts(void);
	void		SetParts(void);
	void		SetAnimation(void);

	void		Release(void);
private:
	void		SetDirMove(void);
	void		FrameMove(void);

private:
	HRESULT		AddComponent(void);

	vector<LERPSAVE*>		m_LerpVec[PART_END];

	int			m_iRotDir;
	float		m_fAngle;
	float		m_fCircleAngle;
	float		m_fTime;
	int			m_iFrame[PART_END];
	float		m_iFrameCnt[PART_END];

	bool		m_bArrive;
	float		m_fSpeed;

	bool		m_bBoard;
	bool		m_bGoal;
	D3DXVECTOR3	m_vMyPos;

	float		m_fDeadTime;

	int			m_iIndexNum;
	wchar_t*	m_wstrBufferName;

	float m_fFlareTime;
	bool m_bFlareCreate;

	typedef list<CGameObject*>				OBJECTLIST;
	typedef map<wstring, OBJECTLIST>		MAPOBJLIST;
	MAPOBJLIST*		m_pMapObjlist;
public:
	void SetMistMap(MAPOBJLIST* pMapObjlist){m_pMapObjlist = pMapObjlist;}
};

#endif // Man_h__
