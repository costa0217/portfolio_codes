/*!
 * \file Effect.cpp
 * \date 2015/12/16 15:20
 *
 * \author USER
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: ����Ʈ���� �ֻ��� class�� ����Ʈ Ŭ�������� ���� �޽��� �ؽ��ĸ� ���°� ��ġ ������ ���� �ִ�.
 *
 * \note
*/

#ifndef Effect_h__
#define Effect_h__

#include "Defines.h"
#include "GameObject.h" 
#include "EffectMgr.h"
#include "SoundMgr.h"
#include "Export_Function.h"

namespace Engine
{
	class CTransform;
}

class CEffect : public Engine::CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect(void);

public:
	virtual _int Update(const _float& fTimeDelta) { return 0; }
	virtual void Render(void) { }

public: //getter setter
	void SetCollsion(_bool SetBool) { m_bCollsion = SetBool; }
	void Set_Delete(_bool bDelete){m_bDelete = bDelete;}

	_vec3	GetWorldPos(void) { return m_pTransCom->m_vPosition; }
	_float	GetRanius(void) { return m_fRadius; }
	_float  Get_AttackValue(){ return m_fAttackValue; }
	_float  Get_DownValue(){ return m_fDownValue; }

protected:
	Engine::CTransform*		m_pTransCom;	//����Ʈ�浹 ��ü�� ��ġ
	_float					m_fRadius;		//����Ʈ�浹 ��ü�� ������
	_float					m_fTime;		//Ư�� ����Ʈ�� ����ϱ� ���� �ӽ÷� ����
	_float					m_fDeleteTime;	//����Ʈ��ü ���� �ð�

	_bool					m_bCollsion;

	_matrix					m_Matrix; 
	const _matrix*			m_pWorldMatrix; 
	const _matrix*			m_pBoneMatrix; 

	_float					m_fAttackValue; // ����Ʈ�� ���ݷ�.
	_float					m_fDownValue;	// ����Ʈ�� �ٿ� ������
	_bool					m_bDelete; // Ư�������ӵǸ� ���������

#pragma region ����׿� ���� ������ �Լ��� �� Ȯ�ο� �ӽ��ڵ�
#ifdef _DEBUG
private:
	ID3DXMesh*				m_pSphereMesh;		// ��豸 �׸���� �޽� & ����
	LPD3DXBUFFER			m_pSphereBuffer;

protected:
	void	Render_BoudingSphere();
#endif
#pragma endregion

private:
	virtual HRESULT Initialize(void)		{ return S_OK; }
	virtual HRESULT Add_Component(void)		{ return S_OK; }
	virtual void Set_ConstantTable(void)	{ }

public:
	virtual _ulong Release(void);
};

#endif // Effect_h__