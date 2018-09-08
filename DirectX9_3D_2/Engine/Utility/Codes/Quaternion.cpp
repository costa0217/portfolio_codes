#include "Quaternion.h"

USING(Engine)

Engine::CQuaternion::CQuaternion(void)
{

}

Engine::CQuaternion::~CQuaternion(void)
{

}


_int Engine::CQuaternion::Update(const _float& fTimeDelta)
{
	_matrix			matScale, matRot, matTrans;
	_matrix			matRotPlus;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	D3DXQUATERNION quaternion;
	D3DXQuaternionIdentity(&quaternion);

	D3DXQuaternionRotationAxis(&quaternion, &m_vCross, m_fQuatAngle);
	D3DXMatrixRotationQuaternion(&matRot, &quaternion);

	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_matWorld = matScale * matRot * matTrans;

	return 0;
}

HRESULT Engine::CQuaternion::Initialize(void)
{
	m_vScale = _vec3(1.f, 1.f, 1.f);

	m_vPosition = _vec3(0.0f, 0.0f, 0.f);
	m_vDir		= _vec3(1.f, 0.f, 0.f);

	m_vCross	= _vec3(0.f, 0.f, 0.f);
	m_fQuatAngle = 0.f;

	ZeroMemory(m_fAngle, sizeof(_float) * ANGLE_END);

	D3DXMatrixIdentity(&m_matWorld);

	return S_OK;
}

CQuaternion* Engine::CQuaternion::Create(void)
{
	CQuaternion*	pTransform = new CQuaternion;

	if(FAILED(pTransform->Initialize()))
	{
		MSG_BOX("pTransform Create Failed");
		Engine::Safe_Release(pTransform);
	}
	return pTransform;
}

_ulong Engine::CQuaternion::Release()
{
	_ulong dwRefCnt = Engine::CComponent::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}
	return dwRefCnt;
}