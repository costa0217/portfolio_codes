#ifndef SmokeSphere_h__
#define SmokeSphere_h__
#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;

	class CFrame;
}

class CSmokeSphere	: public Engine::CGameObject
{
private:
	explicit CSmokeSphere(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSmokeSphere(void);

public: //Get
	void	Get_Switch(void);
public: //Set
	void	Set_Switch(_bool bSwitch) { m_bSwitch = bSwitch; }
	void	Set_Pos(_vec3 vPos);
	void	Set_DataInit(void);

public:
	virtual HRESULT	Initialize(_vec3 vPos);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CSmokeSphere* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:	
	Engine::CFrame*				m_pFrame;

	Engine::CTransform*			m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;
private:
	LPD3DXEFFECT				m_pEffect;

private:
	_bool						m_bSwitch;

	_vec3						m_vViewPos;


private:
	_float						m_fAccTime;
	_float						m_fAlpha;

	_int						m_iRand;

private:
	void	Set_ContantTable(void);
	void	Set_SettingData(void);
public:
	virtual _ulong Release(void);

};

#endif // SmokeSphere_h__
