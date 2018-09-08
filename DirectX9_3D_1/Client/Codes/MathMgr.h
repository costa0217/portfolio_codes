/*!
 * \file MathMgr.h
 * \date 2015/11/29 14:52
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: ���������� ���� ����� ���� �Լ����� ��� ���� Sington class.
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
	_float	ReturnDegreeAngle(_float DegreeAngle);	//��׸� ���� �� �޾� 0 ~ 359.999 ���̷� �������ִ� �Լ�
	_float	GetAngleBetweenTwoVectors(_vec3 LocalVecOne, _vec3 LocalVecTwo);	//������ �������� �� ���� ������ ������ 0 ~ 359.999 �� ��ȯ�ϴ� �Լ�
	DamageDirectionType DamageDirectionDecision(_vec3 vAttackCenterPos, Engine::CTransform* pDestCTransform);

private:
	_matrix m_matRotationDefault;	// -90�� ȸ���ϱ� ���� ȸ�����

public:
	_ulong Release(void);
};

#endif // MathMgr_h__