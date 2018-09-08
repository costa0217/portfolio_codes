#ifndef EffectHDR_h__
#define EffectHDR_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderTarget;
class ENGINE_DLL CEffectHDR
{
protected: //CBase의 생성자와 소멸자는 자식 클래스에서 호출 해야 되서 protected로 선언한다.
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


private:	// 버퍼 변수 
	LPDIRECT3DVERTEXBUFFER9		m_pVBrhw;
	LPDIRECT3DINDEXBUFFER9			m_pIBrhw; 
private:	// 버퍼 함수
	HRESULT			Create_Buffer();
	//void	Render_Buffer(_int iIdx, LPDIRECT3DVERTEXBUFFER9 vb);

public:
	_ulong Release(void);
};

END

#endif // EffectHDR_h__
