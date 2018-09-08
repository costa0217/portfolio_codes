#include "stdafx.h"
#include "MathMgr.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CMathMgr)

CMathMgr::CMathMgr(void)
{
	D3DXMatrixRotationY(&m_matRotationDefault, D3DXToRadian(-90.f));
}

CMathMgr::~CMathMgr(void)
{

}

_ulong CMathMgr::Release(void)
{
	delete this;
	return 0;
}

_float CMathMgr::ReturnDegreeAngle(_float DegreeAngle)
{
	_float fReturnAngle = 0.f;

	fReturnAngle = fmodf(DegreeAngle, 360.f);

	if(fReturnAngle < 0.f)
		fReturnAngle += 360.f;

	return fReturnAngle;
}

_float CMathMgr::GetAngleBetweenTwoVectors(_vec3 LocalVecOne, _vec3 LocalVecTwo)
{
	//�� ������ �� ���ϱ�
	//http://neoplanetz.tistory.com/entry/C-%EB%91%90-%EB%B2%A1%ED%84%B0%EC%9D%98-%EA%B0%81-%EA%B5%AC%ED%95%98%EA%B8%B0Calculate-degree-between-two-vectors
	_float fAngleOne = D3DXToDegree( atan2( LocalVecOne.z, LocalVecOne.x ) );
	_float fAngleTwo = D3DXToDegree( atan2( LocalVecTwo.z, LocalVecTwo.x ) );

	_float fDiffAngles = fAngleOne - fAngleTwo;

	if ( fDiffAngles < 0 )
		fDiffAngles = 360 + fDiffAngles;

	return fDiffAngles;
}

DamageDirectionType CMathMgr::DamageDirectionDecision(_vec3 vAttackCenterPos, Engine::CTransform* pDestCTransform)
{
	// vAttackCenterPos(���� �ڽ��� �߽� ��ġ)
	// pDestCtransform(�ǰ� ����� CTransfomr)

	// ���� �ڽ��� ��ġ�� �ǰ� ����� ��ġ, �ǰ� ����� ����Ʈ, ���� ���ؼ� �ǰ� ������ ��ȯ���ش�.

	//http://ajudaegal.tistory.com/entry/%EB%91%90-%EB%B2%A1%ED%84%B0%EC%9D%98-%EB%82%B4%EC%A0%81%EA%B0%92%EC%9D%84-0360%EB%8F%84%EB%A1%9C-%ED%91%9C%ED%98%84%ED%95%98%EA%B8%B0 �� ������
	_vec3 vDir = vAttackCenterPos - pDestCTransform->m_vPosition;
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vLook;
	memcpy( &vLook, &(pDestCTransform->m_matWorld.m[2][0]), sizeof(_vec3) );	// �躤�� ( ��, ��, �� )
	D3DXVec3Normalize(&vLook, &vLook);											// ���⺤���̹Ƿγ�ֶ�����
	D3DXVec3TransformNormal(&vLook, &vLook, &m_matRotationDefault);	//���� �ǰ� ��ü�� ������ ������ ����ġ ������ �� ������ ���� ����ó���� �Ѵ�.

	_vec3 vRight;
	memcpy( &vRight, &(pDestCTransform->m_matWorld.m[0][0]), sizeof(_vec3) );	// ����Ʈ���� ( ��, ��, �� )
	D3DXVec3Normalize(&vRight, &vRight);										// ���⺤���̹Ƿ� ��ֶ�����
	D3DXVec3TransformNormal(&vRight, &vRight, &m_matRotationDefault);

	_float fRadian_Look		= acosf( D3DXVec3Dot(&vLook, &vDir) );		// �ǰ� ��ü�� �� ���Ϳ� vDir�� ����Ͽ� ������ ���Ѵ�.
	_float fDegree_Look		= D3DXToDegree(fRadian_Look);				

	_float fRadian_Right	= acosf( D3DXVec3Dot(&vRight, &vDir) );		// �ǰ� ��ü�� ����Ʈ ���� �� vDir�� ����Ͽ� ������ ���Ѵ�.
	_float fDegree_Right	= D3DXToDegree(fRadian_Right);				

	_float fDegree = 0.f;
	if(fDegree_Right > 90)
	{
		fDegree = 360.f - fDegree_Look;
	}
	else
	{
		fDegree = fDegree_Look;
	}

	if( (fDegree>= 0.f && fDegree < 60.f) || (fDegree >= 300.f && fDegree <= 360.f) )
	{
		//printf("DDT_FRONT\n");
		return DDT_FRONT;
	}
	else if(fDegree >= 60.f && fDegree < 90.f)
	{
		//printf("DDT_FRONT_RIGHT\n");
		return DDT_FRONT_RIGHT;
	}
	else if(fDegree >= 90.f && fDegree < 120)
	{
		//printf("DDT_BACK_RIGHT\n");
		return DDT_BACK_RIGHT;
	}
	else if((fDegree >= 120.f && fDegree <= 180.f) || (fDegree >= 180.f && fDegree <= 240.f))
	{
		//printf("DDT_BACK\n");
		return DDT_BACK;
	}
	else if(fDegree >= 240.f && fDegree <= 270.f)
	{
		//printf("DDT_BACK_LEFT\n");
		return DDT_BACK_LEFT;
	}
	else
	{
		//printf("DDT_FRONT_LEFT\n");
		return DDT_FRONT_LEFT;
	}
}