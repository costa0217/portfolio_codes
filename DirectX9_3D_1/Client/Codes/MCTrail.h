#ifndef MCTrail_h__
#define MCTrail_h__


#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CMCTrail : public Engine::CGameObject
{
private:
	explicit CMCTrail(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);
	virtual ~CMCTrail(void);

public:
	void Set_MoonChop(Engine::CGameObject* pMonster){ m_pMonster = pMonster; }

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CMCTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);

private: //변수들
	LPD3DXEFFECT	m_pEffect;

	Engine::CGameObject*		m_pMonster;

	Engine::CTransform*			m_pTransCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;

	Engine::VTXTEX*				m_pVTXInfo;
	Engine::VTXTEX*				m_pOriVTXInfo;

	const _matrix*				m_pmatWorldMonster;

	_vec3						m_vViewPos;	//알파 정리를 하기위해 선언
	/*_bool						m_bOnce;
	_float							m_fTimeDelta;*/

	//TCHAR						m_wstrStartPosNamBone[MAX_PATH];

	const	_vec3*				m_pvecStartPos;
	const _vec3*				m_pvecEndPos;
private:
	_vec3						m_vSwordStartPoint;
	_vec3						m_vSwordEndPoint;

	_int							m_iImageNum;
	_vec4						m_vColor;

	_int							m_iIndex;

	_float							m_fTrailLength;
private:
	virtual HRESULT Initialize(void);
	virtual HRESULT Add_Component(void);

	void Set_ContantTable(void);

	void Set_Point(_vec3 vStart, _vec3 vEnd);
	void Move(const _float& fTimeDelta);

public:
	virtual _ulong Release(void);
};
#endif // MCTrail_h__
