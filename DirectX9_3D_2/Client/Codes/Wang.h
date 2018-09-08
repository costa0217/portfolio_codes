#ifndef Wang_h__
#define Wang_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;

	class CFrame;
}

class CWang	: public Engine::CGameObject
{
private:
	explicit CWang(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWang(void);

//public: //Get
//	void	Get_Switch(void);
//public: //Set
//	void	Set_Switch(_bool bSwitch) { m_bSwitch = bSwitch; }
//	void	Set_Pos(_vec3 vPos);
//	void	Set_DataInit(void);

public:
	virtual HRESULT	Initialize(_vec3 vPos);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CWang* Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vPos);

private:	
	Engine::CFrame*				m_pFrame;

	Engine::CTransform*			m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_bool						m_bSwitch;

private: 
	_float						m_fAccTime;

	_vec4						m_vColor;
	_float						m_fAlpha;

	_bool						m_bSound;
private:
	void	Set_ContantTable(void);
	void	Set_SettingData(void);
public:
	virtual _ulong Release(void);

};
#endif // Wang_h__
