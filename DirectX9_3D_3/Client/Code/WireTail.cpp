#include "stdafx.h"
#include "WireTail.h"

#include "Export_Function.h"
#include "Include.h"

#include "Pipeline.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "Stage.h"

CWireTail::CWireTail( LPDIRECT3DDEVICE9 pDevice )
: CWire(pDevice)
{

}

CWireTail::~CWireTail( void )
{
	Release();
}

HRESULT CWireTail::Initialize( void )
{
	FAILED_CHECK(AddComponent());
	//
	m_pInfo->m_vPos = D3DXVECTOR3(0.f,0.f,115.f);
	//

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_WireTex", m_pVertex);
	/////////
	m_fH = 0.05f;
	m_fT = m_fV = 0;
	m_fX = 30*D3DX_PI/180;
	m_fStick_fm = 0.01f;
	m_fStick_M  = 0.1f;
	m_fStick_L  = 30*0.01f;
	m_fStick_J  = 0.02f;
	m_fStick_G  = 9.8f/8;
	m_fCenterX  = 30.f; 
	m_fCenterY  = 50.f;
	m_fStickLength = m_fStick_L*40;
	///////

	return S_OK;
}

int CWireTail::Update( void )
{
	m_pTerrainVertex = m_pManagement->GetTerrainVtx(Engine::LAYER_GAMELOGIC, L"Terrain");
	//m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);

	Engine::CGameObject::Update();

	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = *m_pCameraObserver->GetView();

	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	SetCenterXY(m_vIndex_0_Pos.x+m_fGap,m_vIndex_0_Pos.y);
	Move();
	TransformMatrix();
	

	if(m_bWireMove)
	{
		m_bWireMove = false;
		m_fMoveTime = 0.f;
	}

	//юс╫ц
	if(GetAsyncKeyState('3'))
	{
		m_fX = -2.f;
		//m_fV = -0.06f;
		m_fT = 0.f;
	}

	return 0;
}

void CWireTail::Render( void )
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_WireTex", m_pConvertVertex);

	m_pTexture->Render(0);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_WireTex", m_pVertex);
}

void CWireTail::TransformMatrix( void )
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);


	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		if(i == 0)
		{
			m_pInfo->m_vPos = m_vIndex_0_Pos;
			//m_pInfo->m_vPos.y = m_vIndex_0_Pos.y;
		}
		else if(i == 1)
		{
			m_pInfo->m_vPos = m_vIndex_1_Pos;
			//m_pInfo->m_vPos.y = m_vIndex_1_Pos.y;
		}
		else if(i == 2)
		{
			m_pInfo->m_vPos.x = m_fUpdatedX+m_fGap + fMoveValue;
			m_pInfo->m_vPos.y =  m_fUpdatedY;
			//m_pInfo->m_vPos.z = 115.f;
		}
		else if(i == 3)
		{
			m_pInfo->m_vPos.x = m_fUpdatedX-m_fGap + fMoveValue;
			m_pInfo->m_vPos.y =  m_fUpdatedY;
			//m_pInfo->m_vPos.z = 115.f;
		}


		MAPCOMPONENT::iterator	iter = m_mapComponent.find(L"Transform");
		iter->second->Update();


		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}

void CWireTail::Move()
{
	m_fT = m_fT + m_fH;

	SolceODERK4(m_fT,m_fX,m_fV);
	m_fX = m_Xdx;
	m_fV = m_Vdv;

	m_fUpdatedX = m_fCenterX - m_fStickLength*sin(m_fX) ;//* m_pTimeMgr->GetTime();
	m_fUpdatedY = m_fCenterY - m_fStickLength*cos(m_fX) ;//* m_pTimeMgr->GetTime();
}

CWireTail* CWireTail::Create( LPDIRECT3DDEVICE9 pDevice )
{
	CWireTail* pObj = new CWireTail(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}


