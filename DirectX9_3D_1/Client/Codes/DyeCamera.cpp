#include "stdafx.h"
#include "DyeCamera.h"

#include "Export_Function.h"

CDyeCamera::CDyeCamera(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CCamera(pGraphicDev)
, m_bFixCamera(true)
, m_bFixEnable(false)
{
	Initialize();
	D3DXMatrixIdentity(&m_matWorld);
}

CDyeCamera::~CDyeCamera(void)
{

}

HRESULT CDyeCamera::Initialize(void)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	m_vEye	= D3DXVECTOR3(-10.f, 8.f, 0.f);
	m_vAt	= D3DXVECTOR3(0.f, 8.f, 0.f);
	m_vUp	= D3DXVECTOR3(0.f, 1.f, 0.f);
	Invalidate_ViewMatrix();

	m_fFovY = D3DXToRadian(45.f);
	m_fAspect = float(WINSIZEX) / WINSIZEY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	Invalidate_ProjMatrix();

	return S_OK;
}

_int CDyeCamera::Update(const _float& fTimeDelta)
{
	if(Engine::GetDIKeyState(DIK_ESCAPE) & 0x80)
	{
		if(!m_bFixEnable)
			m_bFixCamera ^= true;
		m_bFixEnable = true;
	}
	else
		m_bFixEnable = false;

	if(m_bFixCamera)
	{
		FixMouse();		
	}

	CameraMove();
	Engine::CCamera::Update(fTimeDelta);

	return 0;
}

void CDyeCamera::FixMouse(void)
{
	POINT		ptMouse = {1600 >> 1, 900 >> 1};
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CDyeCamera::CameraMove(void)
{
	if( Engine::GetDIKeyState(DIK_A) & 0x80 )
	{
		_matrix matCamera;
		_vec3	vRight;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamera);
		D3DXMatrixInverse(&matCamera, 0, &matCamera);

		memcpy(&vRight, &matCamera.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye	-= vRight * 2.f;
		m_vAt	-= vRight * 2.f;
	}
	if( Engine::GetDIKeyState(DIK_D) & 0x80 )
	{
		_matrix matCamera;
		_vec3	vRight;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamera);
		D3DXMatrixInverse(&matCamera, 0, &matCamera);

		memcpy(&vRight, &matCamera.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye	+= vRight * 2.f;
		m_vAt	+= vRight * 2.f;
	}
	if( Engine::GetDIKeyState(DIK_W) & 0x80 )
	{
		_vec3 vLook = m_vAt - m_vEye;

		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye += vLook * 2.f;
		//m_vAt += vLook * 0.05f;
	}
	if( Engine::GetDIKeyState(DIK_S) & 0x80 )
	{
		_vec3 vLook = m_vAt - m_vEye;

		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye -= vLook * 2.f;
	}
	if( m_bFixEnable == true )
	{
		FixMouse();

		int		iDistance = 0;

		if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_X) )
		{
			D3DXMATRIX		matAxis;
			D3DXMatrixRotationAxis(&matAxis, &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(iDistance / 10.f));

			D3DXVECTOR3		vDir;
			vDir = m_vAt - m_vEye;
			D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

			m_vEye = m_vAt - vDir;
		}

		if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Y) )
		{
			D3DXMATRIX		matViewInv;
			D3DXVECTOR3		vRight;
			D3DXMatrixInverse(&matViewInv, NULL, &m_matView);
			memcpy(&vRight, &matViewInv.m[0][0], sizeof(D3DXVECTOR3));

			D3DXMATRIX		matAxis;
			D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(iDistance / 10.f));

			D3DXVECTOR3		vDir;
			vDir = m_vAt - m_vEye;
			D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

			m_vAt = m_vEye + vDir;
		}
	}
}

CDyeCamera* CDyeCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDyeCamera*	pCamera = new CDyeCamera(pGraphicDev);

	if(FAILED(pCamera->Initialize()))
	{
		MSG_BOX("pScene Initialized Failed");
		::Safe_Release(pCamera);
	}

	return pCamera;
}

_ulong CDyeCamera::Release(void)
{
	_ulong dwRefCnt = Engine::CCamera::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}


