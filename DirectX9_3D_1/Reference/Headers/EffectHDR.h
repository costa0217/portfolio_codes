#ifndef EffectHDR_h__
#define EffectHDR_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderTarget;
class ENGINE_DLL CEffectHDR
{
protected: //CBase�� �����ڿ� �Ҹ��ڴ� �ڽ� Ŭ�������� ȣ�� �ؾ� �Ǽ� protected�� �����Ѵ�.
	explicit CEffectHDR(LPDIRECT3DDEVICE9 pGraphicDev);
	~CEffectHDR(void);
public:
	HRESULT		Ready_EffectHDR();
	void			Render_EffectHDR();		

public:
	static CEffectHDR* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	LPD3DXEFFECT					m_pEffect;

	LPDIRECT3DTEXTURE9			m_HDRTexture;
	LPDIRECT3DTEXTURE9			m_LumTexture; 
	LPDIRECT3DTEXTURE9			m_BloomTexture; 


private:	// ���� ���� 
	LPDIRECT3DVERTEXBUFFER9		m_pVBrhw;
	LPDIRECT3DINDEXBUFFER9			m_pIBrhw; 
private:	// ���� �Լ�
	HRESULT			Create_Buffer();
	//void	Render_Buffer(_int iIdx, LPDIRECT3DVERTEXBUFFER9 vb);

public:
	_ulong Release(void);
};

END

#endif // EffectHDR_h__
