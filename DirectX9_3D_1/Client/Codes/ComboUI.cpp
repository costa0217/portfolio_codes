#include "stdafx.h"
#include "ComboUI.h"
#include "Player.h"
#include "Export_Function.h"
#include "DynamicCamera.h"

CComboUI::CComboUI(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_bStart(false)
, m_pTransCom(NULL)
, m_pTextureCom(NULL)
, m_fTimeDelta(0.f)
, m_fOverlapTime(0.f)
, m_iCurComboCnt(0)
, m_iTemp(0)
, m_bComboPlayer(false)
{
	m_pEffect = NULL;

	m_iComboCnt[COMBO_ONE][RENDCOMBO_FRONT] = 1;
	m_iComboCnt[COMBO_ONE][RENDCOMBO_BACK]	= 2;
	m_iComboCnt[COMBO_TEN][RENDCOMBO_FRONT] = 1;
	m_iComboCnt[COMBO_TEN][RENDCOMBO_BACK] = 2;

	for(int i = 0; i < COMBO_END; ++i)
	{
		m_vOriginSize[i]	= _vec2(0.f, 0.f);
		m_pMtrlCom[i]		= NULL;

		for(int j = 0; j < RENDCOMBO_END; ++j)
		{
			m_vPos[i][j]			= _vec2(0.f, 0.f);
			m_vSize[i][j]			= _vec2(0.f, 0.f);
			m_vOriginPos[i][j]		= _vec2(0.f, 0.f);
			m_vStartUV[i][j]		= _vec2(0.f, 0.f);
			m_vLastUV[i][j]			= _vec2(0.1f, 0.2f);
			m_pBufferCom[i][j]		= NULL;
			m_pBufferCom[i][j]		= NULL;
			m_pVertex[i][j]			= NULL;
			m_pOriginVertex[i][j]	= NULL;

			m_bRenderStart[i][j]	= true;
			m_fTransparent[i][j]	= 0.f;
			
			m_eMoveState[i][j]		= COMBOMOVE_NULL;

			D3DXMatrixIdentity(&m_matView[i][j]);
		}
	}
	ZeroMemory(m_matOrtho, sizeof(_matrix));
	
	D3DXMatrixIdentity(&m_matViewFont);

	m_pTransComFont = NULL;
	m_pMtrlComFont	= NULL;
	m_pBufferComFont	= NULL;
	m_pTextureComFont	= NULL;	

	m_vPosFont = _vec2(WINSIZEX / 1.0715f, WINSIZEY / 5.3f);

	const TCHAR*	wstrImagePath;

	wstrImagePath = L"..\\bin\\Resources\\Texture\\StageScene\\UIResource\\Player\\ComboFont0.png";

	CImage*		pPngImage = new CImage;
	pPngImage->Load(wstrImagePath);

	m_vSizeFont		= _vec2((float)pPngImage->GetWidth(), (float)pPngImage->GetHeight());
	m_pVertexFont	= new Engine::VTXTEX[4];
	
	Safe_Delete(pPngImage);
}

CComboUI::~CComboUI(void)
{

}

HRESULT CComboUI::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_UI(Alpha)");

	if(NULL == m_pEffect)
		return E_FAIL;

	const TCHAR*	wstrImagePath;

	wstrImagePath = L"..\\bin\\Resources\\Texture\\StageScene\\UIResource\\Player\\Number_Combine0.png";

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_vPos[COMBO_ONE][RENDCOMBO_FRONT]			= _vec2(WINSIZEX / 1.025f, WINSIZEY / 4.0f);
	m_vPos[COMBO_ONE][RENDCOMBO_BACK]			= _vec2(WINSIZEX / 1.025f, WINSIZEY / 4.0f);

	m_vPos[COMBO_TEN][RENDCOMBO_FRONT]			= _vec2(WINSIZEX / 1.06f, WINSIZEY / 4.f);
	m_vPos[COMBO_TEN][RENDCOMBO_BACK]			= _vec2(WINSIZEX / 1.06f, WINSIZEY / 4.f);

	m_vOriginPos[COMBO_ONE][RENDCOMBO_FRONT]	= _vec2(WINSIZEX / 1.025f, WINSIZEY / 4.0f);
	m_vOriginPos[COMBO_ONE][RENDCOMBO_BACK]		= _vec2(WINSIZEX / 1.025f, WINSIZEY / 4.0f);

	m_vOriginPos[COMBO_TEN][RENDCOMBO_FRONT]	= _vec2(WINSIZEX / 1.06f, WINSIZEY / 4.f);
	m_vOriginPos[COMBO_TEN][RENDCOMBO_BACK]		= _vec2(WINSIZEX / 1.06f, WINSIZEY / 4.f);

	// 원본 사이즈 값 840 * 420
	CImage*		pPngImage = new CImage;
	pPngImage->Load(wstrImagePath);

	for(int i = 0; i < COMBO_END; ++i)
	{
		m_vOriginSize[i] = _vec2((float)pPngImage->GetWidth(), (float)pPngImage->GetHeight());

		for(int j = 0; j < RENDCOMBO_END; ++j)
		{
			m_vSize[i][j] = _vec2((float)pPngImage->GetWidth(), (float)pPngImage->GetHeight());
			m_pVertex[i][j]			= new Engine::VTXTEX[4];
			m_pOriginVertex[i][j]	= new Engine::VTXTEX[4];
		}
	}
	Safe_Delete(pPngImage);

	for( int i = 0; i < COMBO_END; ++i )
	{
		for(int j = 0 ; j < RENDCOMBO_END; ++j)
		{
			m_pBufferCom[i][j]->Get_VtxInfo(m_pVertex[i][j]);
			m_pBufferCom[i][j]->Get_VtxInfo(m_pOriginVertex[i][j]);
		}
	}
	m_pBufferComFont->Get_VtxInfo(m_pVertexFont);

	return S_OK;
}

HRESULT CComboUI::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[COMBO_ONE][RENDCOMBO_FRONT] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_ComboOneFront");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufComboOneFront", pComponent));

	pComponent = m_pBufferCom[COMBO_ONE][RENDCOMBO_BACK] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_ComboOneBack");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufComboOneBack", pComponent));

	pComponent = m_pBufferCom[COMBO_TEN][RENDCOMBO_FRONT] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_ComboTenFront");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_BufComboTenFront", pComponent));

	pComponent = m_pBufferCom[COMBO_TEN][RENDCOMBO_BACK] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_ComboTenBack");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufComboTenBack", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Number_Combine0");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TextureCombo", pComponent));
		

	// **Font 컴포넌트 추가 **
	// TransCom
	pComponent = m_pTransComFont = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TransformFont", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferComFont = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_ComboFont");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_ComboFont", pComponent));

	// For.Texture Component
	pComponent = m_pTextureComFont = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_ComboFont0");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TextureComboFont", pComponent));

	return S_OK;
}

_int CComboUI::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	if(!CDynamicCamera::GetInstance()->GetModeCheck())
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	m_fTimeDelta	= fTimeDelta;

	m_fOverlapTime += fTimeDelta;

	Set_Combo();

	if(m_bComboPlayer == true)
	{
		m_fOverlapTime = 0.f;

		if(m_eMoveState[COMBO_ONE][RENDCOMBO_FRONT] == COMBOMOVE_NULL)
		{
			m_eMoveState[COMBO_ONE][RENDCOMBO_FRONT] = COMBOMOVE_START;
			m_fTransparent[COMBO_ONE][RENDCOMBO_FRONT]	= 0.f;
		}
		else if(m_eMoveState[COMBO_ONE][RENDCOMBO_FRONT] == COMBOMOVE_STOP)
		{
			m_eMoveState[COMBO_ONE][RENDCOMBO_FRONT] = COMBOMOVE_MOVE;
		}

		if(m_iComboCnt[COMBO_ONE][RENDCOMBO_BACK] == 0/* || m_iComboCnt[COMBO_ONE][RENDCOMBO_FRONT] > 9*/) // 데미지를 직접 받는다면 
																											// 9 보다 데미지가 커질리가 없다.
		{
			if(m_eMoveState[COMBO_TEN][RENDCOMBO_FRONT] == COMBOMOVE_NULL)
			{
				m_eMoveState[COMBO_TEN][RENDCOMBO_FRONT] = COMBOMOVE_START;
				m_fTransparent[COMBO_TEN][RENDCOMBO_FRONT]	= 0.f;
			}
			else if(m_eMoveState[COMBO_TEN][RENDCOMBO_FRONT] == COMBOMOVE_STOP)
			{
				m_eMoveState[COMBO_TEN][RENDCOMBO_FRONT] = COMBOMOVE_MOVE;
			}

			if(m_eMoveState[COMBO_TEN][RENDCOMBO_FRONT] == COMBOMOVE_MOVE)
			{
				m_eMoveState[COMBO_TEN][RENDCOMBO_BACK] = COMBOMOVE_START;
				m_fTransparent[COMBO_TEN][RENDCOMBO_BACK]	= 0.f;
			}
			else if(m_eMoveState[COMBO_TEN][RENDCOMBO_BACK] == COMBOMOVE_STOP)
			{
				m_eMoveState[COMBO_TEN][RENDCOMBO_BACK] = COMBOMOVE_MOVE;
			}
		}
		if(m_eMoveState[COMBO_ONE][RENDCOMBO_FRONT] == COMBOMOVE_MOVE)
		{
			m_eMoveState[COMBO_ONE][RENDCOMBO_BACK] = COMBOMOVE_START;
			m_fTransparent[COMBO_ONE][RENDCOMBO_BACK]	= 0.f;
		}
		else if(m_eMoveState[COMBO_ONE][RENDCOMBO_BACK] == COMBOMOVE_STOP)
		{
			m_eMoveState[COMBO_ONE][RENDCOMBO_BACK] = COMBOMOVE_MOVE;
		}
		if(m_iComboCnt[COMBO_ONE][RENDCOMBO_BACK] > 9)
		{
			m_iComboCnt[COMBO_ONE][RENDCOMBO_BACK] = 0;
		}
		if(m_iComboCnt[COMBO_TEN][RENDCOMBO_BACK] > 9)
		{
			m_iComboCnt[COMBO_TEN][RENDCOMBO_BACK] = 0;
		}
		m_bComboPlayer = false;
	}
	if(CPlayer::GetInstance()->GetPlayerAttackCombo() == 0)
	{
		m_bComboPlayer = false;
		Set_Initialize();

		m_fOverlapTime = 0;
	}

	ComboControl();

	// 직교투영 생성
	// 직교투영을 할 경우 정중앙의 좌표가 0, 0이 된다.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);

	//m_iCurComboCnt = m_iComboCnt[COMBO_ONE][RENDCOMBO_FRONT] + m_iComboCnt[COMBO_TEN][RENDCOMBO_FRONT] * 10;

	//printf("%d", m_iCurComboCnt);

	return 0;
}

void CComboUI::Render(void)
{
	if(NULL == m_pEffect)
		return;

	// 뒤
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	if(m_eMoveState[COMBO_ONE][RENDCOMBO_FRONT] != COMBOMOVE_NULL || m_eMoveState[COMBO_ONE][RENDCOMBO_BACK] != COMBOMOVE_NULL)
	{
		Set_ConTableComboFont();
		m_pEffect->CommitChanges();
		m_pBufferComFont->Render(NULL);
	}

	if(m_eMoveState[COMBO_ONE][RENDCOMBO_BACK] != COMBOMOVE_NULL)
	{
		Set_ConTableComboNum(COMBO_ONE, RENDCOMBO_BACK);
		m_pEffect->CommitChanges();
		m_pBufferCom[COMBO_ONE][RENDCOMBO_BACK]->Render(NULL);
	}

	if(m_eMoveState[COMBO_TEN][RENDCOMBO_BACK] != COMBOMOVE_NULL)
	{
		//if(m_iComboCnt[COMBO_TEN][RENDCOMBO_BACK] != 0)
		//{
			Set_ConTableComboNum(COMBO_TEN, RENDCOMBO_BACK);
			m_pEffect->CommitChanges();
			m_pBufferCom[COMBO_TEN][RENDCOMBO_BACK]->Render(NULL);
		//}
	}

	m_pEffect->EndPass();
	m_pEffect->End();

	m_pEffect->SetTexture("g_BaseTexture", NULL);

	// 앞
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	if(m_eMoveState[COMBO_ONE][RENDCOMBO_FRONT] != COMBOMOVE_NULL)
	{
		Set_ConTableComboNum(COMBO_ONE, RENDCOMBO_FRONT);
		m_pEffect->CommitChanges();
		m_pBufferCom[COMBO_ONE][RENDCOMBO_FRONT]->Render(NULL);
	}
	
	if(m_eMoveState[COMBO_TEN][RENDCOMBO_FRONT] != COMBOMOVE_NULL)
	{
		//if(m_iComboCnt[COMBO_TEN][RENDCOMBO_FRONT] != 0)
		//{
			Set_ConTableComboNum(COMBO_TEN, RENDCOMBO_FRONT);
			m_pEffect->CommitChanges();
			m_pBufferCom[COMBO_TEN][RENDCOMBO_FRONT]->Render(NULL);
		//}
	}
	m_pEffect->EndPass();
	m_pEffect->End();

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CComboUI* CComboUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CComboUI*		pGameObject = new CComboUI(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("ComboUI Created Failed");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CComboUI::ComboControl()
{
	// *** 한자리수 앞 ***
	for(int i = 0; i < COMBO_END; ++i)
	{
		for(int j = 0; j <RENDCOMBO_END; ++ j)
		{
			if(m_eMoveState[i][j] != COMBOMOVE_NULL)
			{
				// 왼쪽 위
				m_pVertex[i][j][0].vTexUV.x	= m_vStartUV[i][j].x + (0.1f * m_iComboCnt[i][j]);
				m_pVertex[i][j][0].vTexUV.y	= m_vStartUV[i][j].y ;

				// 오른쪽 위
				m_pVertex[i][j][1].vTexUV.x	= m_vLastUV[i][j].x  + (0.1f * m_iComboCnt[i][j]);
				m_pVertex[i][j][1].vTexUV.y	= m_vStartUV[i][j].y ;

				// 오른쪽 아래
				m_pVertex[i][j][2].vTexUV.x	= m_vLastUV[i][j].x  + (0.1f * m_iComboCnt[i][j]);
				m_pVertex[i][j][2].vTexUV.y	= m_vLastUV[i][j].y  ;

				// 왼쪽 아래
				m_pVertex[i][j][3].vTexUV.x	= m_vStartUV[i][j].x  + (0.1f * m_iComboCnt[i][j]);
				m_pVertex[i][j][3].vTexUV.y	= m_vLastUV[i][j].y ;

				m_pBufferCom[i][j]->Set_VtxInfo(m_pVertex[i][j]);
			}
		}
	}
}

void CComboUI::Set_ConTableComboNum(COMBO_STATE _eCombo, RENDCOMBO_STATE _eComboRend)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);
	
	//0.5 * 600 + 1.f * -m_fY + WINSIZEY * 0.5f == 300

	// 목표 지점을 미리 설정 해주고 목표 지점에 올라가기 까지의 비율을 따져서 투명도를 올려줘야 한다.

	// 콤보 랜더 시작
	if(m_eMoveState[_eCombo][_eComboRend] == COMBOMOVE_START)	
	{
		float Temp = m_vSize[_eCombo][_eComboRend].x * 2.f;
		if(Temp >= m_vOriginSize[_eCombo].x)
		{
			m_vSize[_eCombo][_eComboRend].x -= m_fTimeDelta * 5000;
			m_vSize[_eCombo][_eComboRend].y -= m_fTimeDelta * 2500;
		}
		else
		{
			m_eMoveState[_eCombo][_eComboRend] = COMBOMOVE_STOP;
		}
		// 한자리수 앞
		m_matView[_eCombo][_eComboRend]._11 = m_vSize[_eCombo][_eComboRend].x * 0.2f;
		m_matView[_eCombo][_eComboRend]._22 = m_vSize[_eCombo][_eComboRend].y * 0.4f;
		m_matView[_eCombo][_eComboRend]._33 = 1.f;

		m_matView[_eCombo][_eComboRend]._41 = m_vPos[_eCombo][_eComboRend].x - WINSIZEX * 0.5f;
		m_matView[_eCombo][_eComboRend]._42 = -m_vPos[_eCombo][_eComboRend].y + WINSIZEY * 0.5f;

		m_pEffect->SetMatrix("g_matView", &m_matView[_eCombo][_eComboRend]);
		m_pEffect->SetMatrix("g_matProj", &m_matOrtho);
		m_pEffect->SetVector("g_vLightDiffuse", &_vec4(0.f, 0.f, 0.f, m_fTransparent[_eCombo][_eComboRend] ));
		m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");
	}
	else if(m_eMoveState[_eCombo][_eComboRend] == COMBOMOVE_STOP)
	{
		m_matView[_eCombo][_eComboRend]._11 = m_vOriginSize[_eCombo].x * 0.1f;
		m_matView[_eCombo][_eComboRend]._22 = m_vOriginSize[_eCombo].y * 0.2f;
		m_matView[_eCombo][_eComboRend]._33 = 1.f;

		m_matView[_eCombo][_eComboRend]._41 = m_vOriginPos[_eCombo][_eComboRend].x - WINSIZEX * 0.5f;
		m_matView[_eCombo][_eComboRend]._42 = -m_vOriginPos[_eCombo][_eComboRend].y + WINSIZEY * 0.5f;

		m_pEffect->SetMatrix("g_matView", &m_matView[_eCombo][_eComboRend]);
		m_pEffect->SetMatrix("g_matProj", &m_matOrtho);
		m_pEffect->SetVector("g_vLightDiffuse", &_vec4(0.f, 0.f, 0.f, m_fTransparent[_eCombo][_eComboRend] ));
		m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");
	}
	else if(m_eMoveState[_eCombo][_eComboRend] == COMBOMOVE_MOVE)
	{
		// 한자리수 앞
		m_matView[_eCombo][_eComboRend]._11 = m_vOriginSize[_eCombo].x * 0.1f;
		m_matView[_eCombo][_eComboRend]._22 = m_vOriginSize[_eCombo].y * 0.2f;
		m_matView[_eCombo][_eComboRend]._33 = 1.f;

		if(m_vPos[_eCombo][_eComboRend].y >= m_vOriginPos[_eCombo][_eComboRend].y - 100.f)	
		{
			m_vPos[_eCombo][_eComboRend].y		-= m_fTimeDelta * 1000.f;
			m_fTransparent[_eCombo][_eComboRend]	+= m_fTimeDelta * 10.f;
			m_matView[_eCombo][_eComboRend]._41	= m_vPos[_eCombo][_eComboRend].x - WINSIZEX * 0.5f;
			m_matView[_eCombo][_eComboRend]._42	= -m_vPos[_eCombo][_eComboRend].y + WINSIZEY * 0.5f;
		}
		else
		{
			m_eMoveState[_eCombo][_eComboRend]		= COMBOMOVE_NULL;
			m_matView[_eCombo][_eComboRend]._41		= m_vOriginPos[_eCombo][_eComboRend].x - WINSIZEX * 0.5f;
			m_matView[_eCombo][_eComboRend]._42		= -m_vOriginPos[_eCombo][_eComboRend].y + WINSIZEY * 0.5f;
			m_vSize[_eCombo][_eComboRend].x			= m_vOriginSize[_eCombo].x;
			m_vSize[_eCombo][_eComboRend].y			= m_vOriginSize[_eCombo].y;
			m_vPos[_eCombo][_eComboRend].x			= m_vOriginPos[_eCombo][_eComboRend].x;
			m_vPos[_eCombo][_eComboRend].y			= m_vOriginPos[_eCombo][_eComboRend].y;
		}

		m_pEffect->SetMatrix("g_matView", &m_matView[_eCombo][_eComboRend]);
		m_pEffect->SetMatrix("g_matProj", &m_matOrtho);		
		m_pEffect->SetVector("g_vLightDiffuse", &_vec4(0.f, 0.f, 0.f, m_fTransparent[_eCombo][_eComboRend] ));

		m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");
	}
}

void CComboUI::Set_ConTableComboFont(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransComFont->m_matWorld);
	
	m_matViewFont._11	= m_vSizeFont.x;
	m_matViewFont._22	= m_vSizeFont.y;
	m_matViewFont._33	= 1.f;

	m_matViewFont._41	= m_vPosFont.x - WINSIZEX * 0.5f;
	m_matViewFont._42	= -m_vPosFont.y + WINSIZEY * 0.5f;

	m_pEffect->SetMatrix("g_matView", &m_matViewFont);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);		
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4(0.f, 0.f, 0.f, 0.f ));

	m_pTextureComFont->Set_ConstantTable(m_pEffect, "g_BaseTexture");

	m_pBufferComFont->Set_VtxInfo(m_pVertexFont);
}

void CComboUI::Set_Initialize()
{
	for( int i = 0; i < COMBO_END; ++i)
	{
		for(int j = 0; j < RENDCOMBO_END; ++j)
		{
			m_eMoveState[i][j]		= COMBOMOVE_NULL;
			m_matView[i][j]._41		= m_vOriginPos[i][j].x - WINSIZEX * 0.5f;
			m_matView[i][j]._42		= -m_vOriginPos[i][j].y + WINSIZEY * 0.5f;
			m_vSize[i][j].x			= m_vOriginSize[i].x;
			m_vSize[i][j].y			= m_vOriginSize[i].y;
			m_vPos[i][j].x			= m_vOriginPos[i][j].x;
			m_vPos[i][j].y			= m_vOriginPos[i][j].y;
		}
	}
	m_iComboCnt[COMBO_ONE][RENDCOMBO_FRONT] = 1;
	m_iComboCnt[COMBO_ONE][RENDCOMBO_BACK]	= 2;
	m_iComboCnt[COMBO_TEN][RENDCOMBO_FRONT] = 1;
	m_iComboCnt[COMBO_TEN][RENDCOMBO_BACK] = 2;
}

void CComboUI::Set_Combo()
{
	if(m_iCurComboCnt != CPlayer::GetInstance()->GetPlayerAttackCombo())
	{
		m_iCurComboCnt = CPlayer::GetInstance()->GetPlayerAttackCombo();
		
		m_bComboPlayer = true;

		m_iComboCnt[COMBO_TEN][RENDCOMBO_FRONT] = m_iCurComboCnt / 10;
		m_iComboCnt[COMBO_TEN][RENDCOMBO_BACK]	=  m_iCurComboCnt / 10;
		m_iComboCnt[COMBO_ONE][RENDCOMBO_FRONT] = m_iCurComboCnt - ((m_iCurComboCnt / 10) * 10);
		m_iComboCnt[COMBO_ONE][RENDCOMBO_BACK]	= m_iCurComboCnt - ((m_iCurComboCnt / 10) * 10);
	}
}

_ulong CComboUI::Release(void)
{
	_ulong	dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		for(int i = 0; i < COMBO_END; ++i)
		{
			for(int j = 0; j < RENDCOMBO_END; ++j)
			{
				Engine::Safe_Delete_Array(m_pVertex[i][j]);
				Engine::Safe_Delete_Array(m_pOriginVertex[i][j]);
			}
		}
		Engine::Safe_Delete_Array(m_pVertexFont);
		delete this;
	}
	return dwRefCnt;
}




