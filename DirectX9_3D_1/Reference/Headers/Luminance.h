#ifndef Luminance_h__
#define Luminance_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderTarget;
class ENGINE_DLL CLuminance
{
protected: //CBase�� �����ڿ� �Ҹ��ڴ� �ڽ� Ŭ�������� ȣ�� �ؾ� �Ǽ� protected�� �����Ѵ�.
	explicit CLuminance(LPDIRECT3DDEVICE9 pGraphicDev);
	~CLuminance(void);
public:
	HRESULT	Ready_CropBuffer();
	void	Measure_Luminance();

	void	Render_Debug();	
public:
	static CLuminance* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	vector<Engine::CRenderTarget*>			vecCropBufferRT;
	typedef vector<Engine::CRenderTarget*>	VECCROPBUFFERS;

	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPD3DXEFFECT				m_pEffect;
	LPDIRECT3DTEXTURE9			m_BlendTexture; 

	D3DXHANDLE					m_OffsetHandle;
	D3DXHANDLE					m_DSOffsetHandle;

private:	// ���� ���� 
	LPDIRECT3DVERTEXBUFFER9		m_pVBrhw[6];
	LPDIRECT3DINDEXBUFFER9		m_pIBrhw; 
private:	// ���� �Լ�
	HRESULT	Create_Buffer();


public:
	_ulong Release(void);
};

END

#endif // Luminance_h__
