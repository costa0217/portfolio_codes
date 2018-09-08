#ifndef TreeRect_h__
#define TreeRect_h__

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

class CTreeRect
	: public Engine::CGameObject
{
private:
	explicit CTreeRect(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTreeRect(void);

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
	static CTreeRect* Create(LPDIRECT3DDEVICE9 pDevice, TERRAINTYPE _eTerrainType);
	void SetTerrainType(TERRAINTYPE  _eTerrainType);

private:
	CCameraObserver*		m_pCameraObserver;
	Engine::CInfoSubject*	m_pInfoSubject;

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
	Engine::VTXTEX*			m_pVertex;
	Engine::VTXTEX*			m_pConvertVertex;

private:
	DWORD					m_dwVtxCnt;
	D3DXMATRIX				m_matWorld;

	D3DXVECTOR3				m_vPos;		// 위치
	D3DXVECTOR3				m_vScale;	// 스케일 
	float					m_fAngle[Engine::ANGLE_END];	// 각도

	int			m_iTextureNum;

	TERRAINTYPE					m_eTerrainType;
};
#endif // TreeRect_h__
