#ifndef EffectMgr_h__
#define EffectMgr_h__

#include "Defines.h"
namespace Engine
{
	class	CGameObject;
	class	CLayer;
}

class CMyEffect;
class CEffectMgr
{
	DECLARE_SINGLETON(CEffectMgr)

private:
	explicit CEffectMgr(void);
	virtual ~CEffectMgr(void);

public:
	//HRESULT Reserve_ContainerSize(const _ushort& wSize); //몇개의 리소스 맵 컨터이너를 만들지 결정한다. 

private:
	boost::unordered_map<const TCHAR*,vector<Engine::CGameObject*>>				m_mapEffect;
	typedef boost::unordered_map<const TCHAR*, vector<Engine::CGameObject*>>	MAPEFFECT;


private:	
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	Engine::CLayer*				m_pLayer;		// 이펙트

public:
	void		Initialize(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer*	pLayer);
public:
	void		Load_EffectData(TCHAR* pName, TCHAR* pKey);
	void		Play_Effect(TCHAR* pName, _vec3 vPos, _bool bReStart = false, _bool bHDR = true
								, _vec3 vSize = _vec3(0.f, 0.f, 0.f), _vec3 Angle = _vec3(0.f, 0.f, 0.f), _float fRevolAngle = 0.f
								, _bool bRefeat = false, _float fRefeatTime = 0.f, _vec3* pvecParent = NULL
								, _bool bNoTesting = false, _float fColorDown = 0.f, _float fStartTime = 0.f, _bool bDistort = false);

	void		Create_Effect(TCHAR* pName, _vec3 vPos, _bool bReStart, _vec3 vSize, _vec3 vAngle
		, _float fRevolAngle,  _bool bRefeat = false, _float fRefeatTime = 0.f, _vec3* pvecParent = NULL
		, bool bHDR = false, _bool bNoTesting = false, _float fColorDown = 0.f, _float fStartTime = 0.f, _bool bDistort = false);

	void		Stop_Effect(TCHAR* pName);
	void		Move_Effect(TCHAR* pName,_vec3 vOriPos, _vec3 vMovePos);

private:
	void		Compute_Key_InStr(TCHAR* pOut, TCHAR* pName);	// 경로에서 맵에 넣을 키 값 구하는 함수
	vector<Engine::CGameObject*>*	Find_EffectList(TCHAR* pName);


public:
	_ulong Release(void);
};

#endif // EffectMgr_h__
