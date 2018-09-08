#ifndef Wire_h__
#define Wire_h__

#include "GameObject.h"

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

class CTerrainCol;

class CWire
	: public Engine::CGameObject
{
protected:
	explicit CWire(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWire(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	virtual void TransformMatrix(void);
	virtual void Move();
public:
	static CWire* Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	HRESULT AddComponent(void);
	void Release();
	
protected:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CManagement*	m_pManagement;
	CCollisionMgr*			m_pCollisionMgr;

protected:
	Engine::CTransform*		m_pInfo;
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;
	CTerrainCol*			m_pTerrainCol;
	

protected:
	CCameraObserver*		m_pCameraObserver;
protected:
	Engine::VTXTEX*		m_pVertex;
	Engine::VTXTEX*		m_pConvertVertex;
	const Engine::VTXTEX*		m_pTerrainVertex;
public:
	Engine::VTXTEX*	GetpVertex(){return m_pVertex;}
protected:
	DWORD			m_dwVtxCnt;
	////////////
protected:
	float	m_fH;
	float	m_fT;
	float	m_fV;
	float	m_fX;
	float	m_fStick_fm;
	float	m_fStick_M;
	float	m_fStick_L;
	float	m_fStick_J;
	float	m_fStick_G;
	float	m_fCenterX;
	float   m_fCenterY;
	float   m_fStickLength;
	float	m_Xdx;
	float   m_Vdv;

	float  m_fUpdatedX;
	float  m_fUpdatedY;

	float   m_fMoveTime;
	float	m_fAngleZ;
	bool	m_bWireMove;
	float   m_fHeliSpeed;

	float   m_fGap;
	//юс╫ц
	TCHAR			m_szFps[128];
	//
public:
	static float fMoveValue;
public:
	float CalcODE(float t,float x,float v);
	void  SolceODERK4(float t, float x, float v);
public:
	void SetCenterXY(float CenterX,float CenterY);
	void Set_T_and_V(float T, float X){m_fT = T; m_fV = X;}
	void SetbWireMove(bool bWireMove){m_bWireMove = bWireMove;}
	void SetHeliSpeed(float fHeliSpeed){m_fHeliSpeed = fHeliSpeed;}

	Engine::CTransform* GetTransform(){return m_pInfo;}
	
	///////////////////


};
#endif // Wire_h__
