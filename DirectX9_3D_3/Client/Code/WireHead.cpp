#include "stdafx.h"
#include "WireHead.h"

#include "Export_Function.h"
#include "Include.h"



#include "Pipeline.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "Stage.h"
#include "WireChest.h"
#include "..\..\Engine\Utility\Code\MathMgr.h"

CWireHead::CWireHead( LPDIRECT3DDEVICE9 pDevice )
: CWire(pDevice)
{
	
}

CWireHead::~CWireHead( void )
{
	Release();
}

HRESULT CWireHead::Initialize( void )
{
	FAILED_CHECK(AddComponent());
	//
	m_pInfo->m_vPos = D3DXVECTOR3(0.f,0.f,0.f);
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
	m_fStick_G  = 9.8f/15;
	m_fCenterX  = 30.f; 
	m_fCenterY  = 50.f;
	m_fStickLength = m_fStick_L*2;
	///////


	return S_OK;
}

int CWireHead::Update( void )
{

	Engine::CGameObject::Update();

	m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pParentMat);
	TransformMatrix();


	if(m_bWireMove)
	{
		m_bWireMove = false;
		m_fMoveTime = 0.f;
		m_WireChest->Set_T_and_V(0.f,m_fHeliSpeed);
		m_WireChest->SetbWireMove(true);
		m_WireChest->SetHeliSpeed(m_fHeliSpeed);

	}

	return 0;
}

void CWireHead::Render( void )
{
	/*m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_WireTex", m_pConvertVertex);

	m_pTexture->Render(0);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_WireTex", m_pVertex);*/
	

	//RECT		rc = {200, 200, 800, 600};
	//wsprintf(m_szFps, L"FPS : %d",int(m_fV*10000));
	//Engine::Get_Management()->GetRenderer()->GetFont()->DrawText(NULL, m_szFps, lstrlen(m_szFps), &rc, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 255, 0));
}

void CWireHead::TransformMatrix( void )
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);


	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);

		if(i == 2)
			m_WireChest->SetIndex_1(m_pConvertVertex[i].vPos + D3DXVECTOR3(m_fGap,-5.f,0.f));
		else if(i == 3)
			m_WireChest->SetIndex_0(m_pConvertVertex[i].vPos + D3DXVECTOR3(-m_fGap,-5.f,0.f));

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}

void CWireHead::Move()
{
	m_fT = m_fT + m_fH;

	SolceODERK4(m_fT,m_fX,m_fV);
	m_fX = m_Xdx;
	m_fV = m_Vdv;

	m_fUpdatedX = m_fCenterX - m_fStickLength*sin(m_fX) ;//* m_pTimeMgr->GetTime();
	m_fUpdatedY = m_fCenterY - m_fStickLength*cos(m_fX) ;//* m_pTimeMgr->GetTime();
}

CWireHead* CWireHead::Create( LPDIRECT3DDEVICE9 pDevice )
{
	CWireHead* pObj = new CWireHead(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

void CWireHead::SetHeliInfo( Engine::CTransform* pHeliInfo )
{
	m_pHeliInfo = pHeliInfo;
}

void CWireHead::CreateWireChest()
{
	m_WireChest = CWireChest::Create(m_pDevice);
	m_WireChestList->push_back(m_WireChest);
}
