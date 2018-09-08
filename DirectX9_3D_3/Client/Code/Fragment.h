#ifndef Fragment_h__
#define Fragment_h__

#include "Effect.h"

class CFragment:
	public CEffect
{

private:
	explicit CFragment(LPDIRECT3DDEVICE9 pDevice);
public:
	~CFragment(void);
public:
	static CFragment* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	D3DXVECTOR3		m_vDir;
	D3DXVECTOR3		m_vScale;
	float	m_fTime;

	Engine::VTXCUBE*		m_pVertex_Frag;
	Engine::VTXCUBE*		m_pConvertVertex_Frag;

	int m_iFrameNum;
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Move(void);
	virtual HRESULT AddComponent(void);
public:
	void TransformFrag();
};


#endif // Fragment_h__
