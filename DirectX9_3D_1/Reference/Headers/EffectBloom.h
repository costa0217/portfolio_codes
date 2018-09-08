#ifndef	EffectBloom_h__
#define	EffectBloom_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderTarget;
class ENGINE_DLL CEffectBloom
{
protected: //CBase의 생성자와 소멸자는 자식 클래스에서 호출 해야 되서 protected로 선언한다.
	explicit CEffectBloom(LPDIRECT3DDEVICE9 pGraphicDev);
	~CEffectBloom(void);
public:
	HRESULT	Ready_CropBuffer();
	void	Perform_PostProcessing();		// 고휘도 반짝반짝

	void	Render_Debug();
private:
	float	ComputeGaussianValue(float x, float mean, float std_deviation );

public:
	static CEffectBloom* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	vector<Engine::CRenderTarget*>			vecCropBufferRT;
	typedef vector<Engine::CRenderTarget*>	VECCROPBUFFERS;

	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPD3DXEFFECT				m_pEffect;
	LPDIRECT3DTEXTURE9		m_BlendTexture; 

	D3DXHANDLE					m_OffsetHandle;
	D3DXHANDLE					m_DSOffsetHandle;

	D3DXHANDLE					m_HBloomOffset;
	D3DXHANDLE					m_HBloomWeight;

	D3DXHANDLE					m_VBloomOffset;
	D3DXHANDLE					m_VBloomWeight;

private:	// 버퍼 변수 
	LPDIRECT3DVERTEXBUFFER9	m_pVBrhw[4];
	LPDIRECT3DINDEXBUFFER9		m_pIBrhw; 
private:	// 버퍼 함수
	HRESULT	Create_Buffer();
	void		Render_Buffer(_int iIdx, LPDIRECT3DVERTEXBUFFER9 vb);

public:
	_ulong Release(void);
};

END


#endif // EffectBloom_h__
