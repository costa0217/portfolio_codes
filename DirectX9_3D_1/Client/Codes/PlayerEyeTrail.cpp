#include "stdafx.h"
#include "PlayerEyeTrail.h"

#include "Export_Function.h"
#include "EffectMgr.h"
#include "Player.h"
#include "RenderTargetMgr.h"

CPlayerEyeTrail::CPlayerEyeTrail(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex)
: CGameObject(pGraphicDev)
, m_vSwordStartPoint(0.f, 0.f, 0.f)
, m_vSwordEndPoint(0.f, 0.f, 0.f)
, m_iImageNum(0)
, m_vColor(0.f, 0.f, 0.f, 0.f)

, m_pVTXInfo(NULL)
, m_pOriVTXInfo(NULL)
, m_pvecStartPos(NULL)
, m_pvecEndPos(NULL)
//, m_vecEndPos(0.f, 0.f, 0.f)
, m_pmatWorldMonster(NULL)
, m_iIndex(iIndex)
, m_fTrailLength(0.f)
, m_fTimeDelta(0.f)
, m_matParentBone(NULL)
, m_pPlayer(CPlayer::GetInstance())
, m_fAccTime(0.f)
{
	//ZeroMemory(m_bTrailCheck, sizeof(_bool) * 2);
}

CPlayerEyeTrail::~CPlayerEyeTrail(void)
{

}

CPlayerEyeTrail* CPlayerEyeTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex)
{
	CPlayerEyeTrail*		pTrail = new CPlayerEyeTrail(pGraphicDev, iIndex);

	if(FAILED(pTrail->Initialize()))
	{
		MSG_BOX("pTrail Create Faild");
		::Safe_Release(pTrail);
	}
	return pTrail;
}

HRESULT CPlayerEyeTrail::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_PlayerSwordTrail");
	if(NULL == m_pEffect)		return E_FAIL;

	if(FAILED(Add_Component()))		return E_FAIL;

	m_pVTXInfo		= new Engine::VTXTEX[100];
	m_pOriVTXInfo 	= new Engine::VTXTEX[100];
	m_pBufferCom->Get_VtxInfo(m_pVTXInfo);

	return S_OK;
}

HRESULT CPlayerEyeTrail::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_EyeTrail");
	if(NULL == pComponent)		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));


	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerTrail");
	if(NULL == pComponent)		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

_int CPlayerEyeTrail::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;
	if(NULL == m_pmatWorldMonster)
	{
		m_pmatWorldMonster =  &(CPlayer::GetInstance()->Get_TransCom()->m_matWorld);
		m_pvecStartPos	= CPlayer::GetInstance()->Get_EyePos();
	}
	//m_vecEndPos		=	*m_pvecStartPos +  _vec3(100.f, 0.f, 0.f);

	Engine::CGameObject::Update(fTimeDelta);

	m_vSwordStartPoint		= *m_pvecStartPos + _vec3(0.f, -5.2f, 0.f);
	m_vSwordEndPoint		= *m_pvecStartPos + _vec3(0.f, -6.f, 0.f);

	D3DXVec3TransformCoord(&m_vSwordEndPoint, &m_vSwordEndPoint, m_pmatWorldMonster);
	D3DXVec3TransformCoord(&m_vSwordStartPoint, &m_vSwordStartPoint, m_pmatWorldMonster);

	memcpy(m_pOriVTXInfo, m_pVTXInfo, sizeof(Engine::VTXTEX) * 100);

	Move(fTimeDelta);
	Set_Point(m_vSwordStartPoint, m_vSwordEndPoint);

	if(CPlayer::GetInstance()->GetBerserkerCheck() || CPlayer::GetInstance()->GetPlayerAttackCombo() >= 5)
	{
		_matrix			matView;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXVec3TransformCoord(&m_vViewPos, &m_vSwordEndPoint, &(*m_pmatWorldMonster * matView));
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);
	}

	return 0;
}

void CPlayerEyeTrail::Render(void)
{
	if(NULL == m_pEffect)		return;

	m_pBufferCom->Set_VtxInfo(m_pOriVTXInfo);

	Set_ContantTable();
	LPDIRECT3DTEXTURE9 pDepthTexture = Engine::CRenderTargetMgr::GetInstance()->Get_TargetTexture(L"Target_Depth");
	m_pEffect->SetTexture("g_DepthTexture", pDepthTexture);

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(1);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_DepthTexture", NULL);

	m_pBufferCom->Set_VtxInfo(m_pVTXInfo);
}

void CPlayerEyeTrail::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	m_pEffect->SetVector("g_vColor", &m_vColor);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 6);	
}

void CPlayerEyeTrail::Set_Point(_vec3 vStart, _vec3 vEnd)
{
	m_fAccTime += m_fTimeDelta;
	_matrix		matRotY;
	_vec3		vPlayerDir(0.f, 0.f, -1.f);
	_vec3		vDir(0.f, 0.f, 0.f);
	float			fDist = 0.3f;

	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_pPlayer->m_fOriginalAngleY + 90.f));
	D3DXVec3TransformNormal(&vPlayerDir, &g_vLook, &matRotY);
	D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);

	D3DXVec3Cross(&vDir, &_vec3(0.f, 1.f, 0.f), &vPlayerDir);
	D3DXVec3Normalize(&vDir, &vDir);	

	m_pVTXInfo[0].vPos		= vStart + vPlayerDir * 0.2f - vDir * 0.2f;
	m_pVTXInfo[50].vPos	= vEnd	+ vPlayerDir * 0.2f - vDir * 0.2f;

	_vec3		vFstDir = (-vPlayerDir + vDir * 4.f);
	D3DXVec3Normalize(&vFstDir, &vFstDir);	


	if(D3DXVec3Length(&(m_pVTXInfo[9].vPos - m_pVTXInfo[49].vPos)) < 0.9f)
	{
		m_pVTXInfo[9].vPos		=m_pVTXInfo[0].vPos	;
		m_pVTXInfo[59].vPos	=m_pVTXInfo[50].vPos	;
	}
	else
	{
		m_pVTXInfo[9].vPos		= vStart + vFstDir * fDist;
		m_pVTXInfo[59].vPos	= vEnd	+ vFstDir * fDist;
	}	

	vDir = -vPlayerDir;
	if(m_fAccTime > 0.06f)
	{
		m_pVTXInfo[49].vPos	= m_pVTXInfo[39].vPos		/*+ _vec3(vDir.x, -vDir.x, vDir.z) * fDist*/;
		m_pVTXInfo[99].vPos	= m_pVTXInfo[89].vPos		/*+ _vec3(vDir.x, -vDir.x, vDir.z) * fDist*/;	

		m_pVTXInfo[39].vPos	= m_pVTXInfo[29].vPos		/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;
		m_pVTXInfo[89].vPos	= m_pVTXInfo[79].vPos		/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;

		m_pVTXInfo[29].vPos	= m_pVTXInfo[19].vPos		/*+ vDir * fDist*/;
		m_pVTXInfo[79].vPos	= m_pVTXInfo[69].vPos		/*+ vDir * fDist*/;

		m_pVTXInfo[19].vPos	= m_pVTXInfo[9].vPos		/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;
		m_pVTXInfo[69].vPos	= m_pVTXInfo[59].vPos		/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;

		m_fAccTime	 = 0.f;
	}

	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			if( 0 == i )
			{
				if(0 == j )		continue;
				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[0].vPos,	&m_pVTXInfo[0].vPos,
					&m_pVTXInfo[9].vPos,	&m_pVTXInfo[19].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 50)].vPos,
					&m_pVTXInfo[50].vPos,	&m_pVTXInfo[50].vPos,
					&m_pVTXInfo[9 + 50].vPos,	&m_pVTXInfo[19 + 50].vPos,
					(float)(j) * 0.125f);
			}
			else if( 1== i )
			{
				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[0].vPos,	&m_pVTXInfo[9].vPos,
					&m_pVTXInfo[19].vPos,	&m_pVTXInfo[29].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 50)].vPos,
					&m_pVTXInfo[0 + 50].vPos,	&m_pVTXInfo[9 + 50].vPos,
					&m_pVTXInfo[19 + 50].vPos,	&m_pVTXInfo[29 + 50].vPos,
					(float)(j) * 0.125f);
			}	
			else if( 2 == i)
			{
				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[9].vPos,	&m_pVTXInfo[19].vPos,
					&m_pVTXInfo[29].vPos,	&m_pVTXInfo[39].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 50)].vPos,
					&m_pVTXInfo[9 + 50].vPos,	&m_pVTXInfo[19 + 50].vPos,
					&m_pVTXInfo[29 + 50].vPos,	&m_pVTXInfo[39 + 50].vPos,
					(float)(j) * 0.125f);
			}	
			else if( 3 == i)
			{
				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[19].vPos,	&m_pVTXInfo[29].vPos,
					&m_pVTXInfo[39].vPos,	&m_pVTXInfo[49].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 50)].vPos,
					&m_pVTXInfo[19 + 50].vPos,	&m_pVTXInfo[29 + 50].vPos,
					&m_pVTXInfo[39 + 50].vPos,	&m_pVTXInfo[49 + 50].vPos,
					(float)(j) * 0.125f);
			}	
			else if( 4 == i)
			{
				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[29].vPos,	&m_pVTXInfo[39].vPos,
					&m_pVTXInfo[49].vPos,	&m_pVTXInfo[49].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 50)].vPos,
					&m_pVTXInfo[29 + 50].vPos,	&m_pVTXInfo[39 + 50].vPos,
					&m_pVTXInfo[49 + 50].vPos,	&m_pVTXInfo[49 + 50].vPos,
					(float)(j) * 0.125f);
			}	
		}		
	}
}

void CPlayerEyeTrail::Move(const _float& fTimeDelta)
{
	m_pBufferCom->Set_VtxInfo(m_pVTXInfo);
}

_ulong CPlayerEyeTrail::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		::Safe_Delete_Array(m_pVTXInfo);
		::Safe_Delete_Array(m_pOriVTXInfo);
		delete this;
	}
	return dwRefCnt;
}