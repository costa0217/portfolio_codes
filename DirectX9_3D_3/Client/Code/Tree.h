#ifndef Tree_h__
#define Tree_h__

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
}


class CCameraObserver;

class CTree :
	public Engine::CGameObject
{
protected:
	explicit CTree(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CTree(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual HRESULT AddComponent(void);

public:
	//void		SetPos(const D3DXVECTOR3& rPos); // 0812
	void		SetScale(const D3DXVECTOR3& rScale);
	void		SetAngle(float fAngle[3]);
	void		SetWorldMatrix(const D3DXMATRIX& rMatrix);

public:
	void		GetPos(D3DXVECTOR3* pPos);
	void		GetScale(D3DXVECTOR3* pScale);
	void		GetAngle(float* fAngle);
	D3DXMATRIX	GetWorldMatrix(void);

public:
	static CTree* Create(LPDIRECT3DDEVICE9 pDevice, wstring wstrBufferName);

protected:
	void TransformMatrix(void);
	void Release(void);

protected:
	Engine::CTransform*		m_pInfo;
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;

protected:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CTimeMgr*		m_pTimeMgr;

private:
	CCameraObserver*		m_pCameraObserver;
	wstring m_wstrBufferName;
	
private:
	DWORD			m_dwVtxCnt;

protected:
	Engine::VTXCOL*		m_pVertex;
	Engine::VTXCOL*		m_pConvertVertex;
	const Engine::VTXTEX*		m_pTerrainVertex;

	D3DXMATRIX				m_matWorld;

	D3DXVECTOR3				m_vPos;		// 위치
	D3DXVECTOR3				m_vScale;	// 스케일 
	float					m_fAngle[Engine::ANGLE_END];	// 각도

public:
	void SetPos( D3DXVECTOR3 vPos ); 
	void SetBufferName(wstring wstrBufferName){m_wstrBufferName = wstrBufferName;}
	D3DXVECTOR3 GetPos(void);

};

#endif // Tree_h__
