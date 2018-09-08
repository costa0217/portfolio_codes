#ifndef Nemo_h__
#define Nemo_h__

#include "GameObject.h"

namespace Engine
{
	class CResourcesMgr;
	class CVIBuffer;
	class CResources;
	class CInfoSubject;
}
class CCameraObserver;

class CNemo
	: public Engine::CGameObject
{
private:
	explicit CNemo(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CNemo(void);

public:
	void		SetPos(const D3DXVECTOR3& rPos);
	void		SetRGB(int _iRed, int _iGreen, int _iBlue);
	void		SetScale(const D3DXVECTOR3& rScale);
	void		SetAngle(float fAngle[3]);
	void		SetWorldMatrix(const D3DXMATRIX& rMatrix);

public:
	void		GetPos(D3DXVECTOR3* pPos);
	void		GetRGB(int* _iRed, int* _iGreen, int* _iBlue);
	void		GetScale(D3DXVECTOR3* pScale);
	void		GetAngle(float* fAngle);
	D3DXMATRIX	GetWorldMatrix(void);

public:
	void		SetColor(void);
	void		AddBuffer(void);

public:
	virtual		HRESULT Initialize(void);
	virtual		int Update(void);
	virtual		void Render(void);

public:
	static CNemo* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	CCameraObserver*		m_pCameraObserver;
	Engine::CInfoSubject*	m_pInfoSubject;

private:
	void		Release(void);

	void TransformMatrix(void); // 0803


private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CResources*		m_pResource;

private:
	Engine::CVIBuffer*		m_pBuffer;

private:
	Engine::VTXCOL*			m_pVertex;
	Engine::VTXCOL*			m_pConvertVertex;

private:
	DWORD					m_dwVtxCnt;
	D3DXMATRIX				m_matWorld;

	D3DXVECTOR3				m_vPos;		// 위치

	float					m_fAngle[Engine::ANGLE_END];	// 각도

	D3DXVECTOR3				m_vScale;	// 스케일 

	int						m_iRed;		// RGB
	int						m_iGreen;
	int						m_iBlue;
};
#endif // Nemo_h__
