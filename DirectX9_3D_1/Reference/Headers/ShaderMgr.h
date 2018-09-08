/*!
 * \file ShaderMgr.h
 * \date 2015/11/07 1:18
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CShader를 Map 컨테이너를 통해 관리할 Singleton CShaderMgr class.
 *
 * \note
*/

#ifndef ShaderMgr_h__
#define ShaderMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CShader;
class ENGINE_DLL CShaderMgr
{
	DECLARE_SINGLETON(CShaderMgr)
private:
	explicit CShaderMgr(void);
	~CShaderMgr(void);

public: //getter
	LPD3DXEFFECT Get_EffectHandle(const TCHAR* pShaderTag);

public:
	HRESULT Ready_ShaderFromFiles(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFilePath);

private:
	CShader* Find_Shader(const TCHAR* pShaderTag);

private:
	boost::unordered_map<const TCHAR*, CShader*>			m_mapShaders;
	typedef boost::unordered_map<const TCHAR*, CShader*>	MAPSHADERS;

public:
	void Release(void);
};

END

#endif // ShaderMgr_h__