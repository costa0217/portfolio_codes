/*!
 * \file Shader.h
 * \date 2015/11/07 1:18
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: ShaderFiles�� �ִ� hpp Ȥ�� fx�� ���ǵǾ� �ִ� HLSL�� �����Ͽ� �����Ѵ�.
 *
 * \note
*/

#include "Base.h"

#ifndef Shader_h__
#define Shader_h__

BEGIN(Engine)

class ENGINE_DLL CShader : public Engine::CBase
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShader(void);

public:
	LPD3DXEFFECT Get_EffectHandle(void) { return m_pEffect == NULL ? NULL : m_pEffect; } //m_pEffect �� NULL�̸� NULL�� ��ȯ�ϰ� �ƴ϶�� m_pEffect�� ��ȯ�Ѵ�.

public:
	HRESULT	Init_Shader(const TCHAR* pFilePath);
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFilePath);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPD3DXEFFECT		m_pEffect;		//typedef interface ID3DXEffect *
	LPD3DXBUFFER		m_pErrBuffer;
public:
	virtual _ulong Release(void);
};

END

#endif // Shader_h__