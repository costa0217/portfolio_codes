#ifndef LandObject_h__
#define LandObject_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTransform;

	class CNaviMesh;
};

// ������ �¿��.
class CLandObject : public Engine::CGameObject
{
public:
	enum OBJTYPE { TYPE_NAVI, TYPE_TERRAIN, TYPE_END };

protected:
	explicit CLandObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLandObject(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void) {}

protected:
	Engine::CTransform*				m_pTransCom;
	Engine::VTXTEX*					m_pVertex;
	Engine::CVIBuffer*				m_pBufferCom;

	Engine::CNaviMesh*				m_pNaviMesh;

protected:
	//ANISTATE						m_eAniStat;
	OBJTYPE							m_eObjType;

	//_bool							m_bJumpFall;
	//float							m_fJumpPos;

	_ulong							m_dwNaviIndex;

private:
	void	Compute_Height(void);
	//_float	Compare_JumpingPoint(void);
public:
	virtual _ulong Release(void);
};


#endif // LandObject_h__
