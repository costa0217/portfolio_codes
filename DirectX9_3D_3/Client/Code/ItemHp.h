#ifndef ItemHp_h__
#define ItemHp_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourcesMgr;
	class CVIBuffer;
	class CResources;
	class CInfoSubject;
	class CTexture;
}
class CCameraObserver;
class CPlayerObserver;

class CItemHp
	: public Engine::CGameObject
{
private:
	explicit CItemHp(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CItemHp(void);

public:
	void		SetPos(const D3DXVECTOR3& rPos);
	void		SetScale(const D3DXVECTOR3& rScale);
	void		SetAngle(float fAngle[3]);
	void		SetWorldMatrix(const D3DXMATRIX& rMatrix);

public:
	void		GetPos(D3DXVECTOR3* pPos);
	void		GetScale(D3DXVECTOR3* pScale);
	void		GetAngle(float* fAngle);
	D3DXMATRIX	GetWorldMatrix(void);

public:
	void		AddComponent(void);

public:
	virtual		HRESULT Initialize(void);
	virtual		int Update(void);
	virtual		void Render(void);

public:
	static CItemHp* Create(LPDIRECT3DDEVICE9 pDevice);
	void SetPlayer(Engine::CGameObject* pPlayer);

private:
	CCameraObserver*		m_pCameraObserver;
	Engine::CInfoSubject*	m_pInfoSubject;
	CPlayerObserver*		m_pPlayerObserver;

private:
	void		Release(void);
	void		TransformMatrix(void);

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CResources*		m_pResource;

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*			m_pTexture;

private:
	Engine::VTXCUBE*			m_pVertex;
	Engine::VTXCUBE*			m_pConvertVertex;

private:
	DWORD					m_dwVtxCnt;
	D3DXMATRIX				m_matWorld;

	D3DXVECTOR3				m_vPos;		// 위치
	D3DXVECTOR3				m_vScale;	// 스케일 
	float					m_fAngle[Engine::ANGLE_END];	// 각도

private:
	int			m_iTextureNum;
	Engine::CGameObject*	m_pPlayer;

private:
	float		 m_fPlayerHp;
	D3DXVECTOR3  m_vPlayerPos;

};
#endif // ItemHp_h__
