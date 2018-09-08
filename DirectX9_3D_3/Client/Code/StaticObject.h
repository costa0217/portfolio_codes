#ifndef StaticObject_h__
#define StaticObject_h__

#include "GameObject.h"

namespace Engine
{
	class CResourcesMgr;
	class CVIBuffer;
	class CResources;
}
class CNemo;

class CStaticObject
	: public Engine::CGameObject
{
private:
	explicit CStaticObject(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStaticObject(void);

public:
	void		SetPos(const D3DXVECTOR3& rPos);
	void		SetScale(const D3DXVECTOR3& rScale);
	void		SetAngle(float fAngle[3]);
	void		SetWorldMatrix(const D3DXMATRIX& rMatrix);
	void		SetName(wstring wstrName);

public:
	void		GetPos(D3DXVECTOR3* pPos);
	void		GetScale(D3DXVECTOR3* pScale);
	void		GetAngle(float* fAngle);
	D3DXMATRIX	GetWorldMatrix(void);
	wstring		GetName(void);

public:
	void		AddNemo(CNemo* pNemo);
	list<CNemo*>*	GetNemoList(void);

public:
	virtual		HRESULT Initialize(void);
	virtual		int Update(void);
	virtual		void Render(void);

public:
	static CStaticObject* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void		Release(void);

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CResources*		m_pResource;

private:
	D3DXMATRIX				m_matWorld;
	D3DXVECTOR3				m_vPos;		// 위치
	float					m_fAngle[Engine::ANGLE_END];	// 각도
	D3DXVECTOR3				m_vScale;	// 스케일 
	list<CNemo*>			m_NemoList;
	wstring					m_wstrName;
};
#endif // StaticObject_h__
