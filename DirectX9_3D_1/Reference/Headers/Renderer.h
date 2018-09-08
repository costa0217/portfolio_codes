/*!
 * \file Renderer.h
 * \date 2015/11/07 1:09
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 씬의 랜더링을 수행하는 class.
 *
 * \note
*/

#ifndef Renderer_h__
#define Renderer_h__

#include "Base.h"

BEGIN(Engine)

class CEffectHDR;
class CEffectBloom;

class CCrossFillter;
class CHDRScene;
class CBloom;
class CLuminance;

class CSceneNumMgr;
class CScene;
class CGameObject;
class ENGINE_DLL CRenderer : public Engine::CBase
{
public:
	//랜더타입 구조체
	enum RENDERTYPE { RENDER_PRIORITY, RENDER_ZSORT, RENDER_SHADOW
		, RENDER_UI, RENDER_OBJALPHABLEND, RENDER_ALPHA, RENDER_DISTORTION, RENDER_VELOCITY, RENDER_END};

private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDev);
	~CRenderer(void);

public:
	void				Add_RenderGroup(RENDERTYPE eType, Engine::CGameObject* pGameObject, float fViewZ);
	void				SetCurrentScene(CScene* pScene);

public:
	HRESULT			Ready_Renderer(void);
	void				Render(const _float& fTimeDelta);
	void				Clear_RenderGroup(void);

private:				// 포워드
	void				Render_Priority(void);
	void				Render_ZSort(void);
	void				Render_ObjAlpha(void);
	void				Render_Alpha(void);
	void				Render_Distortion(void);		// 왜곡
	void				Render_UI(void);

private:				// 디퍼드
	void				Render_Shadow(void);
	void				Render_Velocity(void);

	void				Render_GBuffer(void);
	void				Render_Lighting(void);
	void				Render_Blend(void);					// GBuffer - Lighting
	void				Render_BlendFinal(void);			//	HDR	-	Effect


private:
	HRESULT			Create_TargetBuffer(void);	// 타겟을 그리기위한 버퍼
	void				Render_Buffer(LPDIRECT3DVERTEXBUFFER9 vb, LPDIRECT3DINDEXBUFFER9 ib, const DWORD vtxFVF, _byte byIndex);
	void				Render_Debug(void);
	void				Compute_Frame(const _float& fTimeDelta);

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	static _bool			m_bMotionBlurRender;	//공격 박스가 있을때
	static _bool			m_bRadialBlurRender;	//스매쉬 공격, 대쉬 같은 주변

	static _float		m_sBrightPassThreshold;
	static _float		m_sGausianstd;
	static _float		m_sGausianpow;
	static _float		m_sExposure;
	static _float		m_sLightPow;

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	CScene*								m_pScene;

	list<Engine::CGameObject*>				m_RenderGroup[RENDER_END];
	typedef list<Engine::CGameObject*>	RENDERLIST;

	multimap<float, Engine::CGameObject*, greater<float>>				m_mapAlpha;
	typedef multimap<float, Engine::CGameObject*, greater<float>>	MAPALPHA;

	multimap<float, Engine::CGameObject*, greater<float>>				m_mapDistort;
	typedef multimap<float, Engine::CGameObject*, greater<float>>	MAPDISTORTION;

	multimap<float, Engine::CGameObject*, greater<float>>				m_mapObjAlpha;
	typedef multimap<float, Engine::CGameObject*, greater<float>>	MAPOBJALPHA;

	//FPS를 띄우기 위한 변수들
	float								m_fTime;
	int									m_iFrameCnt;
	TCHAR							m_szFps[128];

	//메모리 관련 변수들
	TCHAR							m_szMemory[128];
	MEMORYSTATUSEX			memoryStatus;

	//임시
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;
	
	LPDIRECT3DVERTEXBUFFER9			m_pVBXYZ;
	LPDIRECT3DINDEXBUFFER9				m_pIBXYZ;

	bool		m_bDebugClick;
	bool		m_bDebugShow;
private:
	CSceneNumMgr*		m_pSceneNum;
private:
	CLuminance*			m_pLuminance;
	CBloom*					m_pBloom;
	CHDRScene*			m_pHDRScene;
	CCrossFillter*			m_pCrossFillter;

	CEffectBloom*			m_pEffectBloom;
	CEffectHDR*			m_pEffectHDR;

private: //Rander Option
	bool			m_bShadow, m_bShadowSwitch;		// 그림자 끄기
	bool			m_bHDR, m_bHDRSwitch;				// HDR 끄기
	//bool			m_bE

private:	//UV
	float						m_fWidthTexel;
	float						m_fHeightTexel;
	_matrix					m_prematView;

public:
	bool			Get_ShadowState(void) { return m_bShadow; }

private:
	void			Set_Data(void);

public:
	virtual _ulong Release(void);
};

END

#endif // Renderer_h___