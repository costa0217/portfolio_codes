#ifndef HDRScene_h__
#define HDRScene_h__


#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderTarget;
class ENGINE_DLL CHDRScene
{
protected: //CBase의 생성자와 소멸자는 자식 클래스에서 호출 해야 되서 protected로 선언한다.
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


private:	// 버퍼 변수 
	LPDIRECT3DVERTEXBUFFER9		m_pVBrhw;
	LPDIRECT3DINDEXBUFFER9		m_pIBrhw; 
private:	// 버퍼 함수
	HRESULT	Create_Buffer();
	//void	Render_Buffer(_int iIdx, LPDIRECT3DVERTEXBUFFER9 vb);

public:
	_ulong Release(void);
};

END

#endif // HDRScene_h__
