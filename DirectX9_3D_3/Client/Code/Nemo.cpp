#include "stdafx.h"
#include "Nemo.h"

#include "Export_Function.h"
#include "Include.h"

#include "Pipeline.h"
#include "MathMgr.h"
#include "CameraObserver.h"


CNemo::CNemo(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pBuffer(NULL)
, m_pResource(NULL)
, m_dwVtxCnt(0)
, m_vPos(D3DXVECTOR3(0.f, 0.f, 0.f))
, m_vScale(D3DXVECTOR3(1.f, 1.f, 1.f))
, m_iRed(0)
, m_iGreen(0)
, m_iBlue(0)
, m_pCameraObserver(NULL) // 0804
, m_pInfoSubject(Engine::Get_InfoSubject())
{
	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(m_fAngle, sizeof(float) * Engine::ANGLE_END);
}

CNemo::~CNemo(void)
{
	Release();
}



HRESULT CNemo::Initialize(void)
{
	AddBuffer();

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcCol", m_pVertex);

	m_pCameraObserver = CCameraObserver::Create(); // 0804
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	return S_OK;
}
int CNemo::Update(void) // 0804
{
	//SetColor(); // 0804
	D3DXMatrixIdentity(&m_matWorld);

	Engine::CPipeline::MakeWorldMatrix(&m_matWorld, &m_vScale
		, m_fAngle, &m_vPos);

	return 0;
}
void CNemo::Render(void)
{
	//m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	TransformMatrix(); // 0804

	m_pBuffer->SetVtxInfo(m_pConvertVertex); // 0804
	m_pBuffer->Render();

}	

void CNemo::TransformMatrix(void) // 0804
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];
		m_pConvertVertex[i].dwColor = D3DCOLOR_ARGB(255, m_iRed, m_iGreen, m_iBlue); // 0804

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}

CNemo* CNemo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CNemo* pObj = new CNemo(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;	
}
void CNemo::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete(m_pCameraObserver); // 0804

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pBuffer);
}

void CNemo::AddBuffer(void)
{
	m_pResource	= m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_RcCol");	
	m_pBuffer	= dynamic_cast<Engine::CVIBuffer*>(m_pResource);
}

void CNemo::SetColor(void)
{
	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];
		m_pConvertVertex[i].dwColor = D3DCOLOR_ARGB(255, m_iRed, m_iGreen, m_iBlue);
	}	
	// 0804
	//m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcCol", m_pConvertVertex);
}


void CNemo::SetPos(const D3DXVECTOR3& rPos)
{
	m_vPos	= rPos;	
}

void CNemo::SetRGB(int _iRed, int _iGreen, int _iBlue)
{
	m_iRed		= _iRed;
	m_iGreen	= _iGreen;
	m_iBlue		= _iBlue;
}

void CNemo::SetScale(const D3DXVECTOR3& rScale)
{
	m_vScale	= rScale;
}

void CNemo::SetAngle(float fAngle[3])
{
	for(int i = 0; i < Engine::ANGLE_END; ++i)
		m_fAngle[i] = fAngle[i];
}

void CNemo::GetPos(D3DXVECTOR3* pPos)
{
	*pPos = m_vPos;
}

void CNemo::GetRGB(int* _iRed, int* _iGreen, int* _iBlue)
{
	*_iRed		= m_iRed;
	*_iGreen	= m_iGreen;
	*_iBlue		= m_iBlue;
}

void CNemo::GetScale(D3DXVECTOR3* pScale)
{
	*pScale = m_vScale;
}

void CNemo::GetAngle(float* fAngle)
{
	for(int i = 0; i < Engine::ANGLE_END; ++i)
		fAngle[i] = m_fAngle[i];
}

void CNemo::SetWorldMatrix(const D3DXMATRIX& rMatrix)
{
	m_matWorld = rMatrix;
}

D3DXMATRIX CNemo::GetWorldMatrix(void)
{
	return m_matWorld;
}
