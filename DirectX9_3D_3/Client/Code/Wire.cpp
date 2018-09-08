#include "stdafx.h"
#include "Wire.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
//#include "Pipeline.h"
//#include "TerrainCol.h"
#include "CollisionMgr.h"
#include "Stage.h"
#include "MathMgr.h"

float CWire::fMoveValue = 0.f;

CWire::CWire( LPDIRECT3DDEVICE9 pDevice )
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pManagement(Engine::Get_Management())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pInfo(NULL)
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pTerrainCol(NULL)
, m_pCameraObserver(NULL)
, m_pTerrainVertex(NULL)
, m_dwVtxCnt(0)
, m_fMoveTime(0.f)
, m_fAngleZ(0.f)
, m_bWireMove(false)
, m_fGap(1.2f)
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * 128);
}

CWire::~CWire( void )
{
	Release();
}

HRESULT CWire::Initialize( void )
{
	return S_OK;
}

int CWire::Update( void )
{

	return 0;
}

void CWire::Render( void )
{
	
}

CWire* CWire::Create( LPDIRECT3DDEVICE9 pDevice )
{
	CWire* pObj = new CWire(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CWire::AddComponent( void )
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(3.f,5.f,1.f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Wire");
	m_pTexture = static_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	
	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_WireTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));
	

	return S_OK;
}

void CWire::TransformMatrix( void )
{
	
}

void CWire::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

void CWire::Move()
{

}

float CWire::CalcODE( float t,float x,float v )
{
	return -m_fStick_fm/(m_fStick_M*m_fStick_L*m_fStick_L+m_fStick_J)*v-m_fStick_M*m_fStick_G*m_fStick_L/
		(m_fStick_M*m_fStick_L*m_fStick_L+m_fStick_J)*x;
}

void CWire::SolceODERK4( float t, float x, float v )
{
	float kx1 = v;
	float kv1 = CalcODE(t,x,v);

	float kx2 = v + m_fH*kv1/2;
	float kv2 = CalcODE(t+m_fH/2, x+m_fH*kx1/2, v+m_fH*kv1/2);

	float kx3 = v + m_fH*kv2/2;
	float kv3 = CalcODE(t+m_fH/2, x+m_fH*kx2/2, v+m_fH*kv2/2);

	float kx4 = v + m_fH*kv3;
	float kv4 = CalcODE(t+m_fH, x+m_fH*kx3, v+m_fH*kv3);

	float dx = m_fH*(kx1 + 2*kx2 + 2*kx3 + kx4)/6;
	float dv = m_fH*(kv1 + 2*kv2 + 2*kv3 + kv4)/6;

	m_Xdx = x + dx;
	m_Vdv = v + dv;

}

void CWire::SetCenterXY(float CenterX,float CenterY)
{
	m_fCenterX = CenterX;
	m_fCenterY = CenterY;
}



