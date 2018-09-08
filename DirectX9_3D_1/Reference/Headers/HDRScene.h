#ifndef HDRScene_h__
#define HDRScene_h__


#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderTarget;
class ENGINE_DLL CHDRScene
{
protected: //CBase�� �����ڿ� �Ҹ��ڴ� �ڽ� Ŭ�������� ȣ�� �ؾ� �Ǽ� protected�� �����Ѵ�.
	explicit CHDRScene(LPDIRECT3DDEVICE9 pGraphicDev);
	~CHDRScene(void);
public:
	HRESULT	Ready_HDRScene();
	void	Render_HDRScene();		

public:
	static CHDRScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPD3DXEFFECT				m_pEffect;

	LPDIRECT3DTEXTURE9			m_HDRTexture;
	LPDIRECT3DTEXTURE9			m_LumTexture; 
	LPDIRECT3DTEXTURE9			m_BloomTexture; 


private:	// ���� ���� 
	LPDIRECT3DVERTEXBUFFER9		m_pVBrhw;
	LPDIRECT3DINDEXBUFFER9		m_pIBrhw; 
private:	// ���� �Լ�
	HRESULT	Create_Buffer();
	//void	Render_Buffer(_int iIdx, LPDIRECT3DVERTEXBUFFER9 vb);

public:
	_ulong Release(void);
};

END

#endif // HDRScene_h__
