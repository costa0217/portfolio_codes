/*!
 * \file Effect.cpp
 * \date 2015/12/16 15:20
 *
 * \author USER
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 이펙트들의 최상위 class로 이펙트 클래스들은 따로 메쉬나 텍스쳐를 갖는게 위치 값만을 갖고 있다.
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
	Engine::CTransform*		m_pTransCom;	//이펙트충돌 객체의 위치
	_float					m_fRadius;		//이펙트충돌 객체의 반지름
	_float					m_fTime;		//특정 이펙트를 재생하기 위해 임시로 선언
	_float					m_fDeleteTime;	//이펙트객체 삭제 시간

	_bool					m_bCollsion;

	_matrix					m_Matrix; 
	const _matrix*			m_pWorldMatrix; 
	const _matrix*			m_pBoneMatrix; 

	_float					m_fAttackValue; // 이펙트의 공격력.
	_float					m_fDownValue;	// 이펙트의 다운 게이지
	_bool					m_bDelete; // 특정프레임되면 지우기위해

#pragma region 디버그용 전용 변수와 함수들 과 확인용 임시코드
#ifdef _DEBUG
private:
	ID3DXMesh*				m_pSphereMesh;		// 경계구 그리기용 메쉬 & 버퍼
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