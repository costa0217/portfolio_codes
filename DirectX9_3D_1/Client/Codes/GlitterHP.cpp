#include "stdafx.h"
#include "GlitterHP.h"
#include "Export_Function.h"
#include "Monster.h"
#include "DynamicCamera.h"

IMPLEMENT_SINGLETON(CGlitterHP)

CGlitterHP::CGlitterHP(void)
: Engine::CGameObject(Engine::Get_GraphicDev())
, m_iHpNumber(0)
, m_fDivision(0.f)
, m_bRender(false)
, m_bBarRender(false)
, m_fAlpha(0.f)
, m_bAlphaCheck(true)
, m_iNameNumber(1)
{
	Initialize();
	ZeroMemory(m_matOrtho, sizeof(_matrix));
	ZeroMemory(m_fBossMaxHp, sizeof(_float) * 6);
	ZeroMemory(m_fBossHp, sizeof(_float) * 6);
}

CGlitterHP::~CGlitterHP(void)
{

}

HRESULT CGlitterHP::Initialize(void)
{
	m_pEffect[0] = Engine::Get_EffectHandle(L"Shader_Default(AlphaTest2)");
	m_pEffect[1] = Engine::Get_EffectHandle(L"Shader_Default(AlphaTest3)");

	if(NULL == m_pEffect[0])
		return E_FAIL;

	if(NULL == m_pEffect[1])
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX[0]	= 613.f;
	m_fSizeY[0]	= 64.f;

	m_fSizeX[1]	= 650.f;
	m_fSizeY[1]	= 100.f;

	m_fX	= 800.f;
	m_fY	= 130.f;


	m_pVTXInfo = new Engine::VTXTEX[4];
	m_pBufferCom[HPFront]->Get_VtxInfo(m_pVTXInfo);

	return S_OK;
}

HRESULT CGlitterHP::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[HPFront] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_BossFrontGlitter");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_BossFrontGlitter", pComponent));

	pComponent = m_pBufferCom[HPBack] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_BossBack");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_BossBack", pComponent));

	pComponent = m_pBufferCom[HPBar] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_BossBar");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_BossBar", pComponent));

	pComponent = m_pNameBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Boss");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_BossName", pComponent));

	pComponent = m_pMulBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Boss");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_BossMul", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom[HPFront] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_BossHPFront");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_BossHPFront", pComponent));

	pComponent = m_pTextureCom[HPBack] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_BossHpBack");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_BossHpBack", pComponent));

	pComponent = m_pTextureCom[HPBar] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_BossHPBar");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_BossHPBar", pComponent));

	pComponent = m_pNameTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_BossName");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_BossName", pComponent));

	pComponent = m_pMulTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_UIMul");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_UIMul", pComponent));

	return S_OK;
}

_int CGlitterHP::Update(const _float& fTimeDelta)
{
	_float	m_fCurrentMonsterHp = m_pMonster->Get_MonsterInfo().fHP;

	//보스UI의 에너지가 현재 에네지랑 다르면 실행
	if(m_fBossHp[m_iHpNumber] != m_fCurrentMonsterHp)
	{
		//차이값을 이용한 보간으로 계산한다.
		m_fBossHp[m_iHpNumber] += (m_fCurrentMonsterHp - m_fBossHp[m_iHpNumber]) * fTimeDelta * 5;

		//해당 조건을 만족하면 다음 HP바로 넘어가게 한다.
		if(m_fBossHp[m_iHpNumber] <= (m_fBossMaxHp[m_iHpNumber] - m_fDivision))
		{
			m_iHpNumber += 1;
			if(m_iHpNumber > 6)
				m_iHpNumber = 6;
		}
	}

	//알파 값 변경으로 Bar를 반짝 반짝 아름다운 별
	if(abs(m_fCurrentMonsterHp - m_fBossHp[m_iHpNumber]) > 10)
		m_bBarRender = true;
	else
		m_bBarRender = false;

	if(m_bBarRender)
	{
		if(m_bAlphaCheck)
		{
			m_fAlpha += fTimeDelta * 2.5f;

			if(m_fAlpha > 0.4f)
			{
				m_fAlpha = 0.4f;
				m_bAlphaCheck = false;
			}
		}
		else
		{
			m_fAlpha -= fTimeDelta * 2.5f;;

			if(m_fAlpha < 0.0f)
			{
				m_fAlpha = 0.0f;
				m_bAlphaCheck = true;
			}
		}
	}
	else
		m_fAlpha = 1.f;


	//피가 최소 값까지 달아을때 [m_iHpNumber]를 변화시킨다.

	m_pVTXInfo[1].vPos.x = -0.5f + ((m_fDivision - (m_fBossMaxHp[m_iHpNumber] - m_fBossHp[m_iHpNumber])) / m_fDivision);
	m_pVTXInfo[2].vPos.x = -0.5f + ((m_fDivision - (m_fBossMaxHp[m_iHpNumber] - m_fBossHp[m_iHpNumber])) / m_fDivision);

	m_pVTXInfo[1].vTexUV.x = ((m_fDivision - (m_fBossMaxHp[m_iHpNumber] - m_fBossHp[m_iHpNumber])) / m_fDivision);
	m_pVTXInfo[2].vTexUV.x = ((m_fDivision - (m_fBossMaxHp[m_iHpNumber] - m_fBossHp[m_iHpNumber])) / m_fDivision);

	m_pBufferCom[HPFront]->Set_VtxInfo(m_pVTXInfo);

	if(!CDynamicCamera::GetInstance()->GetModeCheck() && m_bRender)
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);
	// 직교투영행렬을 만들어주는 기능의 함수다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);
	Engine::CGameObject::Update(fTimeDelta);

	return 0;
}

void CGlitterHP::Render(void)
{
	if(NULL == m_pEffect[0])
		return;

	if(NULL == m_pEffect[1])
		return;

	//그리는 로직 관련
	{
		m_pEffect[1]->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);
		_matrix matView;
		D3DXMatrixIdentity(&matView);

		//사이즈
		matView._11 = m_fSizeX[1];
		matView._22 = m_fSizeY[1];
		matView._33 = 1.f;

		//위치
		matView._41 = m_fX - WINSIZEX * 0.5f;
		matView._42 = -m_fY + WINSIZEY * 0.5f;

		m_pEffect[1]->SetMatrix("g_matView", &matView);
		m_pEffect[1]->SetMatrix("g_matProj", &m_matOrtho);

		m_pEffect[1]->SetFloat("g_fAlpha", m_fAlpha);

		m_pTextureCom[HPBar]->Set_ConstantTable(m_pEffect[1], "g_BaseTexture");

		m_pEffect[1]->Begin(NULL, 0);

		m_pEffect[1]->BeginPass(0);

		m_pBufferCom[HPBar]->Render(NULL);

		m_pEffect[1]->EndPass();

		m_pEffect[1]->End();	

		m_pEffect[1]->SetTexture("g_BaseTexture", NULL);
	}

	m_pEffect[0]->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix matView;
	D3DXMatrixIdentity(&matView);

	//사이즈
	matView._11 = m_fSizeX[0];
	matView._22 = m_fSizeY[0];
	matView._33 = 1.f;

	//위치
	matView._41 = m_fX - WINSIZEX * 0.5f;
	matView._42 = -m_fY + WINSIZEY * 0.5f;

	m_pEffect[0]->SetMatrix("g_matView", &matView);
	m_pEffect[0]->SetMatrix("g_matProj", &m_matOrtho);

	{
		m_pTextureCom[HPBack]->Set_ConstantTable(m_pEffect[0], "g_BaseTexture", m_iHpNumber);

		m_pEffect[0]->Begin(NULL, 0);

		m_pEffect[0]->BeginPass(0);

		m_pBufferCom[HPBack]->Render(NULL);

		m_pEffect[0]->EndPass();

		m_pEffect[0]->End();

		m_pEffect[0]->SetTexture("g_BaseTexture", NULL);
	}

	{
		m_pTextureCom[HPFront]->Set_ConstantTable(m_pEffect[0], "g_BaseTexture", m_iHpNumber);

		m_pEffect[0]->Begin(NULL, 0);

		m_pEffect[0]->BeginPass(0);

		m_pBufferCom[HPFront]->Render(NULL);

		m_pEffect[0]->EndPass();

		m_pEffect[0]->End();	

		m_pEffect[0]->SetTexture("g_BaseTexture", NULL);
	}

	{
		m_pEffect[1]->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);
		_matrix matView;
		D3DXMatrixIdentity(&matView);

		//사이즈
		matView._11 = m_fSizeX[0];
		matView._22 = m_fSizeY[0];
		matView._33 = 1.f;

		//위치
		matView._41 = m_fX - WINSIZEX * 0.5f;
		matView._42 = -m_fY + WINSIZEY * 0.5f;

		m_pEffect[1]->SetMatrix("g_matView", &matView);
		m_pEffect[1]->SetMatrix("g_matProj", &m_matOrtho);

		m_pEffect[1]->SetFloat("g_fAlpha", 0.f);

		m_pNameTextureCom->Set_ConstantTable(m_pEffect[1], "g_BaseTexture", m_iNameNumber);

		m_pEffect[1]->Begin(NULL, 0);

		m_pEffect[1]->BeginPass(0);

		m_pNameBufferCom->Render(NULL);

		m_pEffect[1]->EndPass();

		m_pEffect[1]->End();	

		m_pEffect[1]->SetTexture("g_BaseTexture", NULL);
	}

	{
		m_pEffect[1]->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);
		_matrix matView;
		D3DXMatrixIdentity(&matView);

		//사이즈
		matView._11 = m_fSizeX[0];
		matView._22 = m_fSizeY[0];
		matView._33 = 1.f;

		//위치
		matView._41 = m_fX - WINSIZEX * 0.5f;
		matView._42 = -m_fY + WINSIZEY * 0.5f;

		m_pEffect[1]->SetMatrix("g_matView", &matView);
		m_pEffect[1]->SetMatrix("g_matProj", &m_matOrtho);

		m_pEffect[1]->SetFloat("g_fAlpha", 0.f);

		m_pMulTextureCom->Set_ConstantTable(m_pEffect[1], "g_BaseTexture", (6 - m_iHpNumber));

		m_pEffect[1]->Begin(NULL, 0);

		m_pEffect[1]->BeginPass(0);

		m_pMulBufferCom->Render(NULL);

		m_pEffect[1]->EndPass();

		m_pEffect[1]->End();	

		m_pEffect[1]->SetTexture("g_BaseTexture", NULL);
	}
}

void CGlitterHP::SetpMonster(CMonster* pMonster)
{
	m_pMonster	= pMonster;
	_float	m_fReallyBossMaxHp = m_pMonster->Get_MonsterInfo().fMaxHP;

	m_fDivision	= m_fReallyBossMaxHp / 7.f;

	//0이처음 5가 마지막
	for(int i = 0; i < 7; ++i)
	{
		m_fBossMaxHp[i]	= m_fReallyBossMaxHp - m_fDivision * i;
		m_fBossHp[i]	= m_fReallyBossMaxHp - m_fDivision * i;
	}
}

_ulong CGlitterHP::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		::Safe_Delete_Array(m_pVTXInfo);
		delete this;
	}

	return dwRefCnt;
}