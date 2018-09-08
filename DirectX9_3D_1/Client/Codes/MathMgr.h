/*!
 * \file MathMgr.h
 * \date 2015/11/29 14:52
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 연산적으로 자주 사용할 수학 함수들을 모아 놓은 Sington class.
 *
 * \note
*/

#ifndef MathMgr_h__
#define MathMgr_h__

#include "Defines.h"

namespace Engine
{
	class CTransform;
}

class CMathMgr
{
	DECLARE_SINGLETON(CMathMgr)

private:
	explicit CMathMgr(void);
	virtual ~CMathMgr(void);

public:
	_float	ReturnDegreeAngle(_float DegreeAngle);	//디그리 각도 를 받아 0 ~ 359.999 사이로 보간해주는 함수
	_float	GetAngleBetweenTwoVectors(_vec3 LocalVecOne, _vec3 LocalVecTwo);	//방향을 기준으로 두 벡터 사이의 각도를 0 ~ 359.999 로 반환하는 함수
	DamageDirectionType DamageDirectionDecision(_vec3 vAttackCenterPos, Engine::CTransform* pDestCTransform);

private:
	_matrix m_matRotationDefault;	// -90도 회전하기 위한 회전행렬

public:
	_ulong Release(void);
};

#endif // MathMgr_h__