#ifndef	EffectBloom_h__
#define	EffectBloom_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderTarget;
class ENGINE_DLL CEffectBloom
{
protected: //CBase�� �����ڿ� �Ҹ��ڴ� �ڽ� Ŭ�������� ȣ�� �ؾ� �Ǽ� protected�� �����Ѵ�.
	explicit CEffectBloom(LPDIRECT3DDEVICE9 pGraphicDev);
	~CEffectBloom(void);
public:
	HRESULT	Ready_CropBuffer();
	void	Perform_PostProcessing();		// ���ֵ� ��¦��¦

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

private:	// ���� ���� 
	LPDIRECT3DVERTEXBUFFER9	m_pVBrhw[4];
	LPDIRECT3DINDEXBUFFER9		m_pIBrhw; 
private:	// ���� �Լ�
	HRESULT	Create_Buffer();
	void		Render_Buffer(_int iIdx, LPDIRECT3DVERTEXBUFFER9 vb);

public:
	_ulong Release(void);
};

END


#endif // EffectBloom_h__
