#include "stdafx.h"
#include "DamageFont.h"
#include "Export_Function.h"
#include "DynamicCamera.h"

Engine::CLayer* CDamageFont::m_pLayer = NULL;

CDamageFont::CDamageFont(LPDIRECT3DDEVICE9 pGraphicDev) 
: Engine::CGameObject(pGraphicDev)
, m_eDfontType(DFONT_TYPE_DAMAGE)
, m_eDfontState(DFONT_OBJ_NULL)
, m_eDamage(DAMAGE_INIT)
, m_eOverkill(OVERKILL_INIT)
, m_fTimeDelta(0.f)
, m_fOverlapTime(0.f)
, m_bStart(true)
, m_pTransCom(NULL)
, m_iOne(0)
, m_iTen(0)
, m_iHundred(0)
, m_iThousand(0)
, m_fOverKillTime(0.f)
, m_fOverKillGapTime(0.f)
, m_bOverKill(true)
, m_bDelete(false)
{
	D3DXMatrixIdentity(&m_matViewFont);

	m_pMtrlComFont	= NULL;
	m_pBufferComFont	= NULL;
	m_pTextureComFont	= NULL;	
}

CDamageFont::~CDamageFont(void)
{
	D3DXMatrixIdentity(&m_matOrtho);
}

CDamageFont* CDamageFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, DFONT_FONT_TYPE eType, DFONT_OBJ_TYPE eState, _int iDamage, _vec3 vPos)
{
	CDamageFont*		pGameObject	= new CDamageFont(pGraphicDev);

	pGameObject->m_eDfontType	= eType;	//데미지, 오버킬
	pGameObject->m_eDfontState	= eState;	//Player, Monster
	pGameObject->m_vPos			= vPos;		//위치
	pGameObject->m_iDamage		= iDamage;	//데미지

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("DamageFont Created Failed");
	}
	
	m_pLayer->Ready_Object(L"DamageFont", pGameObject);

	return pGameObject;
}

HRESULT CDamageFont::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_UI(Alpha)");

	if(NULL == m_pEffect)
		return E_FAIL;

	srand(unsigned(time(NULL)));

	m_iRandPos = _vec3(float(rand() % 30), float(rand() % 30), 0.f);

	m_iThousand = m_iDamage / 1000;
	m_iHundred	= (m_iDamage - (m_iThousand * 1000)) / 100;
	m_iTen		= (m_iDamage - (m_iThousand * 1000) - (m_iHundred * 100)) / 10;
	m_iOne		= (m_iDamage - (m_iThousand * 1000) - (m_iHundred * 100) - (m_iTen * 10));

	const TCHAR*	wstrImagePath;

	if(m_eDfontType == DFONT_TYPE_DAMAGE)
	{
		wstrImagePath = L"..\\Bin\\Resources\\Texture\\StageScene\\UIResource\\Player\\MabiDamage0.png";

		CImage*		pPngImage = new CImage;
		pPngImage->Load(wstrImagePath);

		m_pOriginVertex		= NULL;

		for(int i = 0; i < 4; ++i)
		{
			m_vDamSize[i] = _vec2((float)pPngImage->GetWidth(), (float)pPngImage->GetHeight());

			m_fPosGap[i] = 0.07f;

			D3DXMatrixIdentity(&m_matView[i]);
			m_vDamPos[i]		= _vec3(m_vPos.x - (i * m_vDamSize[i].x * m_fPosGap[i]) + m_iRandPos.x, m_vPos.y + m_iRandPos.y + 50.f, 0.f);
			m_vDamStartUV[i]	= _vec2(0.f, 0.f);
			m_vDamLastUV[i]		= _vec2(0.1f, 1.f);

			m_pVertex[i]			= NULL;

			m_fDamTransparent[i]	= 1.f;
			
			m_fWaveTime[i] = 0.f;
		}
		m_pMtrlCom		= NULL;

		D3DXMatrixIdentity(&m_matOrtho);
		Safe_Delete(pPngImage);
	}

	if(m_eDfontType == DFONT_TYPE_OVERKILL)
	{
		wstrImagePath	= L"..\\Bin\\Resources\\Texture\\StageScene\\UIResource\\Player\\OverKill0.png";

		CImage*		pPngImage = new CImage;
		pPngImage->Load(wstrImagePath);

		m_pOriginVertex		= NULL;

		for(int i = 0; i < 4; ++i)
		{
			m_vOverSize[i] = _vec2((float)pPngImage->GetWidth(), (float)pPngImage->GetHeight());

			D3DXMatrixIdentity(&m_matView[i]);

			m_vOverStartUV[i]		= _vec2(0.f, 0.f);
			m_vOverLastUV[i]		= _vec2(0.076f, 1.f);

			m_vOriOverStartUV[i]	= _vec2(0.f, 0.f);
			m_vOriOverLastUV[i]		= _vec2(0.076f, 1.f);

			m_vOverPos[i]		= _vec3(m_vPos.x - (i * m_vOverSize[i].x * 0.056f) + m_iRandPos.x, m_vPos.y + m_iRandPos.y, 0.f);

			m_pVertex[i]			= NULL;

			m_fOverTransparent[i]	= 0.f;
		}
		m_pMtrlCom		= NULL;

		// font
		m_vSizeFont		= _vec2((float)pPngImage->GetWidth(), (float)pPngImage->GetHeight());
		m_pVertexFont	= new Engine::VTXTEX[4];
		m_vPosFont		= _vec3(m_vOverPos[3].x - 27.f, m_vOverPos[3].y + 1.f, 0.f);
		m_vStartUVFont	= _vec2(0.77f, 0.f);
		m_vLastUVFont	= _vec2(1.f, 1.f);
		m_fFontTransparent	= 0.f;

		D3DXMatrixIdentity(&m_matOrtho);
		Safe_Delete(pPngImage);
	}

	if(FAILED(Add_Component()))
		return	E_FAIL;

	m_pOriginVertex = new Engine::VTXTEX[4];
	for(int i = 0 ; i < 4; ++i)
	{
		m_pVertex[i] = new Engine::VTXTEX[4];

		m_pBufferCom[i]->Get_VtxInfo(m_pVertex[i]);
		m_pBufferCom[i]->Get_VtxInfo(m_pOriginVertex);
		m_iRandNum[i] = 0;

		if(m_eDfontType == DFONT_TYPE_OVERKILL)
			m_pBufferComFont->Get_VtxInfo(m_pVertexFont);

		m_bWaveDir[i] = false;
		m_iWaveCnt[i] = 0;
	}
	return S_OK;
}

HRESULT CDamageFont::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer 1
	pComponent = m_pBufferCom[0] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_DamageFont1");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufDamageFont1", pComponent));

	// For.VIBuffer 2
	pComponent = m_pBufferCom[1] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_DamageFont2");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufDamageFont2", pComponent));

	// For.VIBuffer 3
	pComponent = m_pBufferCom[2] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_DamageFont3");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufDamageFont3", pComponent));

	// For.VIBuffer 4
	pComponent = m_pBufferCom[3] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_DamageFont4");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufDamageFont4", pComponent));

	// For.VIBuffer Font
	pComponent = m_pBufferComFont = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_OverKillFont");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufferOverKill", pComponent));

	if(m_eDfontType == DFONT_TYPE_DAMAGE)
	{
		// For.Texture Component
		pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_DamageFont0");
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TextureDamage", pComponent));
	}
	else if(m_eDfontType == DFONT_TYPE_OVERKILL)
	{
		// For.Texture Component
		pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_OverKill0");
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TextureOverKill", pComponent));
	}

	return S_OK;
}

_int CDamageFont::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	m_fTimeDelta	 = fTimeDelta;
	m_fOverlapTime	+= fTimeDelta;
	m_fOverKillTime += m_fTimeDelta*3.f;
	m_fDelTime		+= fTimeDelta;

	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);

	if(m_eDfontType == DFONT_TYPE_DAMAGE)
	{
		for(int i = 0; i < 4; ++i)
		{
			if(m_eDamage == DAMAGE_INIT)
			{
				m_fDamTransparent[i] -= fTimeDelta * 10;
				if(m_fDamTransparent[i] >= 0.1f)
				{
					for(int i = 0; i < 4; ++i)
						m_vDamPos[i].y += fTimeDelta * 200.f;
				}
				else
				{
					m_eDamage = DAMAGE_WAVE;
				}
			}
		}

		if(m_eDamage == DAMAGE_WAVE)		// 웨이브 상태일때
		{
			for(int i = 0; i < 4; ++i)
			{	
				if(m_iWaveCnt[i] <= 4)				// 두번의 웨이브를 한다.
				{
					if(m_fWaveTime[i] < 0.06f)		
					{
						m_fWaveTime[i] += fTimeDelta;
						if(m_bWaveDir[i] == false)
						{
							m_vDamPos[i].y -= fTimeDelta * 300.f;
						}
						else
						{
							m_vDamPos[i].y += fTimeDelta * 300.f;
						}
					}
					else
					{
						if(m_bWaveDir[i] == true)
							m_bWaveDir[i] = false;
						else
							m_bWaveDir[i] = true;

						m_fWaveTime[i] = 0.f;
						++m_iWaveCnt[i];
					}
				}
				else
				{
					m_eDamage = DAMAGE_DEL;
				}
			}
		}

		for(int i = 0; i < 4; ++i)
		{
			if(m_eDamage == DAMAGE_DEL)
			{
				if(m_fDamTransparent[i] <= 1.f)
				{
					m_fDamTransparent[i] += m_fTimeDelta * 2.f;
					m_vDamPos[i].y		 -= m_fTimeDelta * 1000.f;
					m_vDamSize[i].x		 -= m_fTimeDelta * 500.f ;
					m_vDamSize[i].y		 -= m_fTimeDelta * 100.f;
				}
				m_vDamPos[0].x -= m_fTimeDelta * 15.f;
				m_vDamPos[1].x -= m_fTimeDelta * 5.5f;
				m_vDamPos[2].x += m_fTimeDelta * 5.5f;
				m_vDamPos[3].x += m_fTimeDelta * 15.f;
			}
		}
	}

	if(m_eDfontType == DFONT_TYPE_OVERKILL)
	{
		if(m_fOverKillTime > 5.f)
		{
			m_vSizeFont.x += m_fTimeDelta * 100.f;
			m_vSizeFont.y += m_fTimeDelta * 50.f;
			m_fFontTransparent += m_fTimeDelta * 1.5f;

			for(int i = 0; i < 4; ++i)
			{
				m_vOverSize[i].x += m_fTimeDelta * 100.f;
				m_vOverSize[i].y += m_fTimeDelta * 50.f;
				m_fOverTransparent[i] += m_fTimeDelta * 1.5f;

				// 아직 지워지는 코드가 완전하지 않아서 남겨둠 
				if(m_fOverTransparent[i] > 1.f)
					return -1;
			}
		}
	}

	if(!CDynamicCamera::GetInstance()->GetModeCheck())
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	return 0;
}

void CDamageFont::Render(void)
{
	if(m_fFontTransparent > 1.f)
		return;

	if(NULL == m_pEffect)
		return;

	if(m_eDfontType == DFONT_TYPE_OVERKILL)
		OverKillFontControl();

	else if(m_eDfontType == DFONT_TYPE_DAMAGE)
		DamageFontControl();

	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	int iNum = 0;

	if(m_iThousand == 0)
	{
		iNum = 3;
	}
	else
	{
		iNum = 4;
	}

	for(int i = 0; i < iNum; ++i)
	{
		if(m_eDfontType == DFONT_TYPE_DAMAGE)
		{
			Set_ConTableDamage(i);
			m_pEffect->CommitChanges();
			m_pBufferCom[i]->Render(NULL);
		}
		else if(m_eDfontType == DFONT_TYPE_OVERKILL)
		{
			Set_ConTableOverKillFont();
			m_pEffect->CommitChanges();
			m_pBufferComFont->Render(NULL);

			Set_ConTableOverKill(i);
			m_pEffect->CommitChanges();
			m_pBufferCom[i]->Render(NULL);
		}
	}
	m_pEffect->EndPass();
	m_pEffect->End();

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

void CDamageFont::Set_ConTableDamage(int iNum)
{
	m_pEffect->SetMatrix("g_matWould", &m_pTransCom->m_matWorld);

	m_matView[iNum]._11 = m_vDamSize[iNum].x * 0.076f;
	m_matView[iNum]._22 = m_vDamSize[iNum].y;
	m_matView[iNum]._33 = 1.f;

	m_matView[iNum]._41 = m_vDamPos[iNum].x - WINSIZEX * 0.1f;
	m_matView[iNum]._42 = -m_vDamPos[iNum].y + WINSIZEY * 0.23f;

	m_pEffect->SetMatrix("g_matView", &m_matView[iNum]);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	if(m_eDfontType == DFONT_TYPE_OVERKILL)
	{
		m_pEffect->SetVector("g_vLightDiffuse", &_vec4(0.f, 0.f, 0.f, m_fDamTransparent[iNum]));
		//printf("DFONT_TYPE_OVERKILL\n");
	}
	else if(m_eDfontType == DFONT_TYPE_DAMAGE)
	{
		if(m_eDfontState == DFONT_OBJ_MONSTER) // 빨강
		{
			m_pEffect->SetVector("g_vLightDiffuse", &_vec4(0.f, 1.0f, 1.f, m_fDamTransparent[iNum]));
			//printf("DFONT_OBJ_MONSTER\n");
		}
		else if(m_eDfontState == DFONT_OBJ_PLAYER) // 자주색
		{
			m_pEffect->SetVector("g_vLightDiffuse", &_vec4(0.f, 1.f, 0.1f, m_fDamTransparent[iNum]));
			//printf("DFONT_OBJ_PLAYER\n");
		}
	}

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");
}

void CDamageFont::DamageFontControl(void)
{
	for(int i = 0; i < 4; ++i)
	{
		int iTemp;

		if(i == 0) 
			iTemp = m_iOne;
		if(i == 1) 
			iTemp = m_iTen;
		if(i == 2) 
			iTemp = m_iHundred;
		if(i == 3)
			iTemp = m_iThousand;

		// 왼쪽 위
		m_pVertex[i][0].vTexUV.x = m_vDamStartUV[i].x + (0.1f * iTemp - 1);
		m_pVertex[i][0].vTexUV.y = m_vDamStartUV[i].y;

		// 오른쪽 위
		m_pVertex[i][1].vTexUV.x = m_vDamLastUV[i].x + (0.1f * iTemp - 1);
		m_pVertex[i][1].vTexUV.y = m_vDamStartUV[i].y;

		// 오른쪽 아래
		m_pVertex[i][2].vTexUV.x = m_vDamLastUV[i].x + (0.1f * iTemp - 1);
		m_pVertex[i][2].vTexUV.y = m_vDamLastUV[i].y;

		// 오른쪽 위
		m_pVertex[i][3].vTexUV.x = m_vDamStartUV[i].x + (0.1f * iTemp - 1);
		m_pVertex[i][3].vTexUV.y = m_vDamLastUV[i].y;

		m_pBufferCom[i]->Set_VtxInfo(m_pVertex[i]);
	}
}

void CDamageFont::Set_ConTableOverKill(int iNum)
{
	m_pEffect->SetMatrix("g_matWould", &m_pTransCom->m_matWorld);

	m_matView[iNum]._11 = m_vOverSize[iNum].x * 0.076f * 1.5f;
	m_matView[iNum]._22 = m_vOverSize[iNum].y * 1.5f;;
	m_matView[iNum]._33 = 1.f;

	if(m_iThousand == 0)	//천 자리 없을때
	{
		printf("세자리임\n");
		if(iNum == 0)
			m_matView[iNum]._41 = m_vOverPos[iNum].x - WINSIZEX * 0.1f + 15.f;
		else if(iNum == 1)
			m_matView[iNum]._41 = m_vOverPos[iNum].x - WINSIZEX * 0.1f + 10.f;
		else if(iNum == 2)
			m_matView[iNum]._41 = m_vOverPos[iNum].x - WINSIZEX * 0.1f + 5.f;
		/*else if(iNum == 3) // 천자리 일때는 어차피 iNum 가 3 일때가 없음
			m_matView[iNum]._41 = m_vOverPos[iNum].x - WINSIZEX * 0.1f + 15.f;*/
	}
	else					//천 자리 있을때
	{
		if(iNum == 0)
			m_matView[iNum]._41 = m_vOverPos[iNum].x - WINSIZEX * 0.1f + 30.f;
		else if(iNum == 1)
			m_matView[iNum]._41 = m_vOverPos[iNum].x - WINSIZEX * 0.1f + 25.f;
		else if(iNum == 2)
			m_matView[iNum]._41 = m_vOverPos[iNum].x - WINSIZEX * 0.1f + 20.f;
		else if(iNum == 3)
			m_matView[iNum]._41 = m_vOverPos[iNum].x - WINSIZEX * 0.1f + 15.f;
	}

	m_matView[iNum]._42 = -m_vOverPos[iNum].y + WINSIZEY * 0.23f;
																 
	m_pEffect->SetMatrix("g_matView", &m_matView[iNum]);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	m_pEffect->SetVector("g_vLightDiffuse", &_vec4(0.f, 0.f, 0.f, m_fOverTransparent[iNum]));

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");
}

void CDamageFont::Set_ConTableOverKillFont(void)
{
	// 왼쪽 위
	m_pVertexFont[0].vTexUV.x = m_vStartUVFont.x;
	m_pVertexFont[0].vTexUV.y = m_vStartUVFont.y;

	// 오른쪽 위
	m_pVertexFont[1].vTexUV.x = m_vLastUVFont.x;
	m_pVertexFont[1].vTexUV.y = m_vStartUVFont.y;

	// 오른쪽 아래
	m_pVertexFont[2].vTexUV.x = m_vLastUVFont.x;
	m_pVertexFont[2].vTexUV.y = m_vLastUVFont.y;

	// 오른쪽 위
	m_pVertexFont[3].vTexUV.x = m_vStartUVFont.x;
	m_pVertexFont[3].vTexUV.y = m_vLastUVFont.y;

	m_pBufferComFont->Set_VtxInfo(m_pVertexFont);

	m_pEffect->SetMatrix("g_matWould", &m_pTransCom->m_matWorld);

	m_matViewFont._11 = m_vSizeFont.x * 0.23f * 1.5f;
	m_matViewFont._22 = m_vSizeFont.y * 1.5f;
	m_matViewFont._33 = 1.f;

	m_matViewFont._41 = m_vPosFont.x - WINSIZEX * 0.1f;
	m_matViewFont._42 = -m_vPosFont.y + WINSIZEY * 0.23f;

	m_pEffect->SetMatrix("g_matView", &m_matViewFont);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	m_pEffect->SetVector("g_vLightDiffuse", &_vec4(0.f, 0.f, 0.f, m_fFontTransparent));

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");
}

void CDamageFont::OverKillFontControl(void)
{
	if(m_fOverKillTime < 2.f)
	{
		if(m_bOverKill == true)
		{
			m_fOverKillGapTime += m_fTimeDelta;
			if(m_fOverKillGapTime > 0.05f)
			{
				m_iRandNum[0] = rand() % 9;
				m_iRandNum[1] = rand() % 9;
				m_iRandNum[2] = rand() % 9;
				m_iRandNum[3] = rand() % 9;

				m_fOverKillGapTime = 0.f;

				for(int i = 0; i < 4; ++i)
				{
					// 왼쪽 위
					m_pVertex[i][0].vTexUV.x = m_vOverStartUV[i].x + (0.076f * (float)m_iRandNum[i]);
					m_pVertex[i][0].vTexUV.y = m_vOverStartUV[i].y;

					// 오른쪽 위
					m_pVertex[i][1].vTexUV.x = m_vOverLastUV[i].x + (0.076f * (float)m_iRandNum[i]);
					m_pVertex[i][1].vTexUV.y = m_vOverStartUV[i].y;

					// 오른쪽 아래
					m_pVertex[i][2].vTexUV.x = m_vOverLastUV[i].x + (0.076f * (float)m_iRandNum[i]);
					m_pVertex[i][2].vTexUV.y = m_vOverLastUV[i].y;

					// 오른쪽 위
					m_pVertex[i][3].vTexUV.x = m_vOverStartUV[i].x + (0.076f * (float)m_iRandNum[i]);
					m_pVertex[i][3].vTexUV.y = m_vOverLastUV[i].y;

					m_pBufferCom[i]->Set_VtxInfo(m_pVertex[i]);
				}
			}
			else
			{
				for(int i = 0; i < 4; ++i)
				{
					// 왼쪽 위
					m_pVertex[i][0].vTexUV.x = m_vOverStartUV[i].x + (0.076f * (float)m_iRandNum[i]);
					m_pVertex[i][0].vTexUV.y = m_vOverStartUV[i].y;

					// 오른쪽 위
					m_pVertex[i][1].vTexUV.x = m_vOverLastUV[i].x + (0.076f * (float)m_iRandNum[i]);
					m_pVertex[i][1].vTexUV.y = m_vOverStartUV[i].y;

					// 오른쪽 아래
					m_pVertex[i][2].vTexUV.x = m_vOverLastUV[i].x + (0.076f * (float)m_iRandNum[i]);
					m_pVertex[i][2].vTexUV.y = m_vOverLastUV[i].y;

					// 오른쪽 위
					m_pVertex[i][3].vTexUV.x = m_vOverStartUV[i].x + (0.076f * (float)m_iRandNum[i]);
					m_pVertex[i][3].vTexUV.y = m_vOverLastUV[i].y;

					m_pBufferCom[i]->Set_VtxInfo(m_pVertex[i]);
				}
			}
		}
	}
	else
	{
		m_bOverKill = false;
		for(int i = 0; i < 4; ++i)
		{
			int iTemp;

			if(i == 0) 
				iTemp = m_iOne;
			if(i == 1) 
				iTemp = m_iTen;
			if(i == 2) 
				iTemp = m_iHundred;
			if(i == 3)
				iTemp = m_iThousand;

			// 왼쪽 위
			m_pVertex[i][0].vTexUV.x = m_vOverStartUV[i].x + (0.076f * iTemp - 1);
			m_pVertex[i][0].vTexUV.y = m_vOverStartUV[i].y;

			// 오른쪽 위
			m_pVertex[i][1].vTexUV.x = m_vOverLastUV[i].x + (0.076f * iTemp - 1);
			m_pVertex[i][1].vTexUV.y = m_vOverStartUV[i].y;

			// 오른쪽 아래
			m_pVertex[i][2].vTexUV.x = m_vOverLastUV[i].x + (0.076f * iTemp - 1);
			m_pVertex[i][2].vTexUV.y = m_vOverLastUV[i].y;

			// 오른쪽 위
			m_pVertex[i][3].vTexUV.x = m_vOverStartUV[i].x + (0.076f * iTemp - 1);
			m_pVertex[i][3].vTexUV.y = m_vOverLastUV[i].y;

			m_pBufferCom[i]->Set_VtxInfo(m_pVertex[i]);
		}
	}
}

_ulong CDamageFont::Release(void)
{
	_ulong	dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		for(int i = 0 ; i < 4; ++i)
		{
			Engine::Safe_Delete_Array(m_pVertex[i]);
		}
		Engine::Safe_Delete_Array(m_pOriginVertex);

		if(m_eDfontType == DFONT_TYPE_OVERKILL)
			Engine::Safe_Delete_Array(m_pVertexFont);

		delete this;
	}
	return dwRefCnt;
}