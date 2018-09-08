#include "stdafx.h"
#include "PlayerSkill.h"
#include "Export_Function.h"
#include "Player.h"
#include "DynamicCamera.h"
#include "SkillFailFont.h"

_float CPlayerSkill::m_fSkillUseValueBalance = 1.f;

CPlayerSkill::CPlayerSkill(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_fTimeDelta(0.f)
, m_fOverlapTime(0.f)
, m_pTransCom(NULL)
, m_pTextureCom(NULL)
, m_eChange(CHANGE_NULL)
, m_fIconTime(0.f)
, m_iCenterNum(2)
, m_bMove(false)
, m_bButtonDown(false)
, m_bButtonDownStart(true)
, m_fButtonTime(0.f)
, m_fMoveSillUI(0.f)
, m_eFirstFont(SF_OFF)
, m_iJudgementCnt(0)
{
	m_bJudgement[0] = false;
	m_bJudgement[1] = false;
}

CPlayerSkill::~CPlayerSkill(void)
{
	D3DXMatrixIdentity(&m_matOrtho);
}

HRESULT CPlayerSkill::Initialize(int iStageNum)
{
	m_iStageNum = iStageNum;

	m_pEffect = Engine::Get_EffectHandle(L"Shader_UI(Alpha)");
	
	if(NULL == m_pEffect)
		return E_FAIL;

	// ��ųâ �̹����� ũ�⸦ �˱� ���� ���� 
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSkillUseValueBalance = 3.f;

	const TCHAR*		wstrImagePath;

	wstrImagePath		= L"../Bin/Resources/Texture/StageScene/UIResource/Player/Player_Skill0.png";

	CImage*		pPngImage = new CImage;		
	pPngImage->Load(wstrImagePath);

	for(int j = 0; j < 2; ++j)		// ��Ÿ�� ��ųâ�� �׳� ��ųâ�� �ֱ� ���� 2�� �ݺ� 
	{
		for(int i = 0; i < SL_End; ++i)		// ��ų ����ü ����, ��ų ���ڸ�ŭ for���� ����
		{
			m_fTempYPos[i] = 0.f;

			SKILLUI*	pSkill = new SKILLUI;

			pSkill->m_eName = (SKILL_LIST)i;
			pSkill->m_vSize			= _vec2((float)pPngImage->GetWidth(), (float)pPngImage->GetHeight());	// �̹��� ũ�� ����
			pSkill->m_vOriginSize	= _vec2((float)pPngImage->GetWidth(), (float)pPngImage->GetHeight());
			pSkill->m_vStartUV		= _vec2(0.2f * i, 0.f);
			pSkill->m_vLastUV		= _vec2(0.2f * i + 0.2f, 1.f);	
			pSkill->m_pBufferCom	= m_pBufferCom[i];

			if(pSkill->m_eName == SL_Judgement)		// ��ų �̸��� ���� �ʱⰪ�� ���ݾ� �޶���, 
				// ��ų�� ������ 5���� Ȯ���ΰ� �����ϰ� �ϴ� �ʱ�ȭ
			{
				pSkill->m_fTransparent	 = 1.f;		// ����
				pSkill->m_iRenderRank	 = 5;		//���� �켱����
				pSkill->m_vSize			*= 0.75f;	
				pSkill->m_vOriginSize	*= 0.75f;
				pSkill->m_vPos			 = _vec3(WINSIZEX / (2.f ), WINSIZEY / (2.f - 0.3f), 0.f);	// ��ų�� ���Ҽ��� �ʱ� Y���� �ٸ�����
				pSkill->m_vOrizinPos	 = _vec3(WINSIZEX / (2.f ), WINSIZEY / (2.f - 0.3f), 0.f);
				pSkill->m_fCoolTime		 = 30.f / m_fSkillUseValueBalance;		// ��Ÿ�� ���̸� ��ų���� �ٸ��� ���� 
				pSkill->m_fCoolTimeBack	 = 30.f / m_fSkillUseValueBalance;
				pSkill->m_fNeedSp		 = 0.f / m_fSkillUseValueBalance;
			}
			else if(pSkill->m_eName == SL_Charge_Slash)
			{
				pSkill->m_fTransparent	 = 0.f;
				pSkill->m_iRenderRank	 = 3;
				pSkill->m_vSize			*= 0.75f;
				pSkill->m_vOriginSize	*= 0.75f;
				pSkill->m_vPos			 = _vec3(WINSIZEX / (2.f ), WINSIZEY / (2.f - 0.18f), 0.f);
				pSkill->m_vOrizinPos	 = _vec3(WINSIZEX / (2.f ), WINSIZEY / (2.f - 0.18f), 0.f);
				pSkill->m_fCoolTime		 = 0.f / m_fSkillUseValueBalance;
				pSkill->m_fCoolTimeBack	 = 0.f / m_fSkillUseValueBalance;
				pSkill->m_fNeedSp		 = 250.f / m_fSkillUseValueBalance;
			}
			else if(pSkill->m_eName == SL_Execution)
			{
				pSkill->m_fTransparent		= 0.f;

				pSkill->m_iRenderRank		= 1;			// ����°�� �߾�, m_iRenderRank�� ���� ���� ���� ���߿� ������ ��.

				pSkill->m_vSize				*= 1.25f;		// ����°�� ��� ��ų�̱� ������ �ʱ� ������ ���� �ٸ��ͺ��� ũ�� ���� 
				pSkill->m_vOriginSize		*= 1.25f;
				pSkill->m_vPos				= _vec3(WINSIZEX / (2.f ), WINSIZEY / 2.f, 0.f);
				pSkill->m_vOrizinPos		= _vec3(WINSIZEX / (2.f ), WINSIZEY / 2.f, 0.f);
				pSkill->m_fCoolTime			= 90.f / m_fSkillUseValueBalance;
				pSkill->m_fCoolTimeBack		= 90.f / m_fSkillUseValueBalance;
				pSkill->m_fNeedSp			= 500.f / m_fSkillUseValueBalance;
			}
			else if(pSkill->m_eName == SL_Berserker)
			{
				pSkill->m_fTransparent		= 0.f;
				pSkill->m_iRenderRank		= 2;
				pSkill->m_vSize				*= 0.75f;
				pSkill->m_vOriginSize		*= 0.75f;
				pSkill->m_vPos				= _vec3(WINSIZEX / (2.f ), WINSIZEY / (2.f + 0.22f), 0.f);
				pSkill->m_vOrizinPos		=  _vec3(WINSIZEX / (2.f ), WINSIZEY / (2.f + 0.22f), 0.f);
				pSkill->m_fCoolTime			= 45.f / m_fSkillUseValueBalance;
				pSkill->m_fCoolTimeBack		= 45.f / m_fSkillUseValueBalance;
				pSkill->m_fNeedSp			= 750.f / m_fSkillUseValueBalance;
			}
			else if(pSkill->m_eName == SL_Hell)
			{
				pSkill->m_fTransparent	= 1.f;
				pSkill->m_iRenderRank	= 4;
				pSkill->m_vSize			*= 0.75f;
				pSkill->m_vOriginSize	*= 0.75f;
				pSkill->m_vPos			= _vec3(WINSIZEX / (2.f ), WINSIZEY / (2.f + 0.405f), 0.f);
				pSkill->m_vOrizinPos	= _vec3(WINSIZEX / (2.f ), WINSIZEY / (2.f + 0.405f), 0.f);
				pSkill->m_fCoolTime		= 600.f / m_fSkillUseValueBalance;
				pSkill->m_fCoolTimeBack	= 600.f / m_fSkillUseValueBalance;
				pSkill->m_fNeedSp		= 1000.f / m_fSkillUseValueBalance;
			}

			if(j % 2 == 0)
			{
				m_vecSkillCool.push_back(*pSkill);		
				m_vecSkillCoolBack.push_back(pSkill);
			}
			else
			{
				m_vecSkillUI.push_back(*pSkill);
				m_vecSkillUIBack.push_back(pSkill);
			}
		}
	}

	Safe_Delete(pPngImage);		// ����ü �ʱ�ȭ�� ���� �� CImage*�� �����.

	for(int i = 0; i < SL_End; ++i)
	{
		m_vecSkillUIBack[i]->m_pVertex	= new Engine::VTXTEX[4];		// ���ؽ� �ʱ�ȭ
		m_vecSkillCoolBack[i]->m_pVertex = new Engine::VTXTEX[4];
		m_vecSkillCoolBack[i]->m_pOriginVertex = new Engine::VTXTEX[4];

		m_pBufferCom[i]->Get_VtxInfo(m_vecSkillUIBack[i]->m_pVertex);
		m_pBufferCoolCom[i]->Get_VtxInfo(m_vecSkillCoolBack[i]->m_pVertex);
		m_pBufferCoolCom[i]->Get_VtxInfo(m_vecSkillCoolBack[i]->m_pOriginVertex);
	}

	vector<SKILLUI*>::iterator	iterSkill		= m_vecSkillUIBack.begin();
	vector<SKILLUI*>::iterator	iterSkillEnd	= m_vecSkillUIBack.end();

	vector<SKILLUI*>::iterator	iterCoolBack		= m_vecSkillCoolBack.begin();
	vector<SKILLUI*>::iterator	iterCoolBackEnd		= m_vecSkillCoolBack.end();

	vector<SKILLUI>::iterator iterCool		= m_vecSkillCool.begin();
	vector<SKILLUI>::iterator iterCoolEnd	= m_vecSkillCool.end();

	for(int i = 0; iterSkill != iterSkillEnd; ++iterCoolBack ,++iterSkill, ++i)		// UV �ʱ�ȭ
	{
		m_vecSkillUIBack[i]->m_pVertex[0].vTexUV.x = (*iterSkill)->m_vStartUV.x;
		m_vecSkillUIBack[i]->m_pVertex[0].vTexUV.y = (*iterSkill)->m_vStartUV.y;

		m_vecSkillUIBack[i]->m_pVertex[1].vTexUV.x = (*iterSkill)->m_vLastUV.x;
		m_vecSkillUIBack[i]->m_pVertex[1].vTexUV.y = (*iterSkill)->m_vStartUV.y;

		m_vecSkillUIBack[i]->m_pVertex[2].vTexUV.x = (*iterSkill)->m_vLastUV.x;
		m_vecSkillUIBack[i]->m_pVertex[2].vTexUV.y = (*iterSkill)->m_vLastUV.y;

		m_vecSkillUIBack[i]->m_pVertex[3].vTexUV.x = (*iterSkill)->m_vStartUV.x;
		m_vecSkillUIBack[i]->m_pVertex[3].vTexUV.y = (*iterSkill)->m_vLastUV.y;

		m_vecSkillCoolBack[i]->m_pVertex[0].vTexUV.x = (*iterCoolBack)->m_vStartUV.x;
		m_vecSkillCoolBack[i]->m_pVertex[0].vTexUV.y = (*iterCoolBack)->m_vStartUV.y;

		m_vecSkillCoolBack[i]->m_pVertex[1].vTexUV.x = (*iterCoolBack)->m_vLastUV.x;
		m_vecSkillCoolBack[i]->m_pVertex[1].vTexUV.y = (*iterCoolBack)->m_vStartUV.y;

		m_vecSkillCoolBack[i]->m_pVertex[2].vTexUV.x = (*iterCoolBack)->m_vLastUV.x;
		m_vecSkillCoolBack[i]->m_pVertex[2].vTexUV.y = (*iterCoolBack)->m_vLastUV.y;

		m_vecSkillCoolBack[i]->m_pVertex[3].vTexUV.x = (*iterCoolBack)->m_vStartUV.x;
		m_vecSkillCoolBack[i]->m_pVertex[3].vTexUV.y = (*iterCoolBack)->m_vLastUV.y;

		m_pBufferCom[i]->Set_VtxInfo(m_vecSkillUIBack[i]->m_pVertex);
		m_pBufferCoolCom[i]->Set_VtxInfo(m_vecSkillCoolBack[i]->m_pVertex);
	}
	//// Font ����
	const TCHAR*		wstrImagePath2;

	wstrImagePath2		= L"../Bin/Resources/Texture/StageScene/UIResource/Player/Skill_Fail_Font0.png";

	CImage*		pPngImage2 = new CImage;		
	pPngImage2->Load(wstrImagePath2);

	for(int i = 0; i < SF_END; ++i)
	{
		m_vPosFont[i]		= _vec2(WINSIZEX / 2.f, WINSIZEY / 1.2f);
		m_vSizeFont[i]		= _vec2((float)pPngImage2->GetWidth(), (float)pPngImage2->GetHeight());
		m_fTransparentFont[i] = 1.f;
		m_bFailFont[i]		= false;
		m_pVertexFont[i]	= new Engine::VTXTEX[4];
		m_pOriginVertexFont[i]	= new Engine::VTXTEX[4];

		D3DXMatrixIdentity(&m_matViewFont[i]);

		m_pBufferComFont[i]->Get_VtxInfo(m_pVertexFont[i]);
		m_pBufferComFont[i]->Get_VtxInfo(m_pOriginVertexFont[i]);
	}
	m_vPosOriginFont = _vec2(WINSIZEX / 2.f, WINSIZEY / 1.2f);

	m_vStartUVFont[SF_COOLTIME]			= _vec2(0.f, 0.f);
	m_vLastUVFont[SF_COOLTIME]			= _vec2(1.f, 0.5f);

	m_vStartUVFont[SF_SP]		= _vec2(0.f, 0.5f);
	m_vLastUVFont[SF_SP]		= _vec2(1.f, 1.0f);

	Safe_Delete(pPngImage2);

	return S_OK;
}

HRESULT CPlayerSkill::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	// For.TransForm Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[SL_Judgement] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Judgement");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufJudgement", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[SL_Charge_Slash] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Charge_Slash");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufCharge_Slash", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[SL_Execution] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Execution");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufExecution", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[SL_Berserker] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Berserker");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufBerserker", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[SL_Hell] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Hell");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufHell", pComponent));

	// cooltime
	// For.VIBuffer Component
	pComponent = m_pBufferCoolCom[SL_Judgement] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_JudgementCool");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufSkillCool1", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCoolCom[SL_Charge_Slash] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Charge_SlashCool");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufSkillCool2", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCoolCom[SL_Execution] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_ExecutionCool");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufSkillCool3", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCoolCom[SL_Berserker] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_BerserkerCool");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufSkillCool4", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCoolCom[SL_Hell] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_HellCool");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufSkillCool5", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerSkill");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_PlayerSkill", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCoolCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerSkillCool");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_PlayerSkillCool", pComponent));

	// Font ����
	// For.VIBuffer Component
	pComponent = m_pBufferComFont[SF_SP] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_SP_SF");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufSkillFont", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferComFont[SF_COOLTIME] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Cool_SF");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_BufCoolFont", pComponent));

	// For.Texture Component
	pComponent = m_pTextureComFont = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_SkillFont");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TextureSkillFont", pComponent));

	return S_OK;
}

_int CPlayerSkill::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	if(!CDynamicCamera::GetInstance()->GetModeCheck())
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	m_fTimeDelta	 = fTimeDelta;
	m_fOverlapTime	+= fTimeDelta;

	if(Engine::GetDIKeyState(DIK_F) & 0x80 && 0.2f < m_fOverlapTime  && !(Engine::GetDIKeyState(DIK_Z) & 0x80))		// �ð����� �̿��� �ߺ�Ű üũ 
	{
		m_fOverlapTime = 0.f;		// Ű�� �������� ��� �ߺ�Ű üũ�� �ð��� �ʱ�ȭ

		m_eChange	= CHANGE_DOWN;	// Ű�� �������� ��ų�� ���°��� �ٲ� 
	}

	if(Engine::GetDIKeyState(DIK_R) & 0x80 && 0.2f < m_fOverlapTime && !(Engine::GetDIKeyState(DIK_Z) & 0x80))
	{
		m_fOverlapTime = 0.f;

		m_eChange	= CHANGE_UP;
	}

	if(!(Engine::GetDIKeyState(DIK_Z) & 0x80))
	{
		m_bButtonDown = false;
	}

	if(Engine::GetDIKeyState(DIK_K) & 0x80 && 0.2f < m_fOverlapTime)
	{
		m_fOverlapTime = 0.f;
		ReduceCoolTime(3, 10);
	}

	// ���� �ʱ�ȭ
	/*if(Engine::GetDIKeyState(DIK_Q) & 0x80 && 0.2f < m_fOverlapTime)
	{
		m_fOverlapTime = 0.f;
		Init_JudgementCoolTime();
	}*/

	if(Engine::GetDIKeyState(DIK_O) & 0x80 && 0.2f < m_fOverlapTime)
	{
		m_fOverlapTime = 0.f;
		m_bFailFont[SF_SP] = true;

		if(m_eFirstFont == SF_OFF)
			m_eFirstFont = SF_SP;
	}

	if(Engine::GetDIKeyState(DIK_P) & 0x80 && 0.2f < m_fOverlapTime)
	{
		m_fOverlapTime = 0.f;
		m_bFailFont[SF_COOLTIME] = true;

		if(m_eFirstFont == SF_OFF)
			m_eFirstFont = SF_COOLTIME;
	}

	// �� �Ʒ� �ִ� ���ʹ� ���� �ö󰡰� 
	// �װ� �ƴ� �༮���� �Ʒ��� ������.
	if(m_eChange == CHANGE_DOWN)
	{
		m_fIconTime += m_fTimeDelta;
		int	iTemp = 0;
		for(int i = 0; i < SL_End ; ++i)
		{
			if(m_vecSkillUI[i].m_iRenderRank == 1)		// m_iRenderRank�� 1�϶��� �߾�
				m_iCenterNum = i;						// m_iRenderRank�� 1�϶��� �ش��ϴ� ��ųâ�� ã���� 
		}
		SkillDownChange(m_iCenterNum);		// m_iRenderRank�� ã�� ��ų���� �߽����� ��ųâ ��ġ ü���� 
	}

	if(m_eChange == CHANGE_UP)
	{
		m_fIconTime += m_fTimeDelta;
		int	iTemp = 0;
		for(int i = 0; i < SL_End ; ++i)
		{
			if(m_vecSkillUI[i].m_iRenderRank == 1)
				m_iCenterNum = i;
		}
		SkillUpChange(m_iCenterNum);
	}

	SelectMove();
	SkillSelect();

	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);		// ���� ��Ʈ���� ����
	
	return 0;
}

void CPlayerSkill::Render(void)
{
	if(NULL == m_pEffect)
		return;

	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pEffect->SetMatrix("g_matWourld", &m_pTransCom->m_matWorld);

	for(int i = 0; i < SL_End; ++i)
	{
		if(m_vecSkillUIBack[i]->m_iRenderRank == 5)		// ������ũ�� �������� ���� ���� 
		{
			Set_ConTableSkillUi(i);
			m_pEffect->CommitChanges();
			m_vecSkillUIBack[i]->m_pBufferCom->Render(NULL);

			if(m_vecSkillCool[i].m_fCoolTime >= 0)		// ��Ÿ�� �ð��� 0���� Ŭ��
			{
				if(m_vecSkillCoolBack[i]->m_bCoolTime == true)		// ��Ÿ�� On�̸� ��Ÿ���� �����ϴ� ���ÿ� 
				{													// ��Ÿ�� ���� �Լ��� ������.
					CoolDown(i);
					Set_ConTableCool(i);
					m_pEffect->CommitChanges();
					m_pBufferCoolCom[i]->Render(NULL);
				}
			}
			else		// ��Ÿ���� 0���� ������ 
			{
				m_vecSkillCoolBack[i]->m_bCoolTime = false;
				m_vecSkillCoolBack[i]->m_fCoolTime = m_vecSkillCoolBack[i]->m_fCoolTimeBack;
				m_vecSkillCool[i].m_bCoolTime = false;
				m_vecSkillCool[i].m_fCoolTime = m_vecSkillCool[i].m_fCoolTime;
				m_vecSkillCoolBack[i]->m_bSkillStart = true;
				m_vecSkillCool[i].m_bSkillStart = true;
			}
		}
	}
	for(int i = 0; i < SL_End; ++i)
	{
		if(m_vecSkillUIBack[i]->m_iRenderRank == 4)
		{
			Set_ConTableSkillUi(i);
			m_pEffect->CommitChanges();
			m_vecSkillUIBack[i]->m_pBufferCom->Render(NULL);

			if(m_vecSkillCool[i].m_fCoolTime >= 0)
			{
				if(m_vecSkillCoolBack[i]->m_bCoolTime == true)
				{
					CoolDown(i);
					Set_ConTableCool(i);
					m_pEffect->CommitChanges();
					m_pBufferCoolCom[i]->Render(NULL);
				}
			}
			else
			{
				m_vecSkillCoolBack[i]->m_bCoolTime = false;
				m_vecSkillCoolBack[i]->m_fCoolTime = m_vecSkillCoolBack[i]->m_fCoolTimeBack;
				m_vecSkillCool[i].m_bCoolTime = false;
				m_vecSkillCool[i].m_fCoolTime = m_vecSkillCool[i].m_fCoolTime;
				m_vecSkillCoolBack[i]->m_bSkillStart = true;
				m_vecSkillCool[i].m_bSkillStart = true;
			}
		}
	}

	for(int i = 0; i < SL_End; ++i)
	{
		if(m_vecSkillUIBack[i]->m_iRenderRank == 3)
		{
			Set_ConTableSkillUi(i);
			m_pEffect->CommitChanges();
			m_vecSkillUIBack[i]->m_pBufferCom->Render(NULL);

			if(m_vecSkillCool[i].m_fCoolTime >= 0)
			{
				if(m_vecSkillCoolBack[i]->m_bCoolTime == true)
				{
					CoolDown(i);
					Set_ConTableCool(i);
					m_pEffect->CommitChanges();
					m_pBufferCoolCom[i]->Render(NULL);
				}
			}
			else
			{
				m_vecSkillCoolBack[i]->m_bCoolTime = false;
				m_vecSkillCoolBack[i]->m_fCoolTime = m_vecSkillCoolBack[i]->m_fCoolTimeBack;
				m_vecSkillCool[i].m_bCoolTime = false;
				m_vecSkillCool[i].m_fCoolTime = m_vecSkillCool[i].m_fCoolTime;
				m_vecSkillCoolBack[i]->m_bSkillStart = true;
				m_vecSkillCool[i].m_bSkillStart = true;
			}
		}
	}

	for(int i = 0; i < SL_End; ++i)
	{
		if(m_vecSkillUIBack[i]->m_iRenderRank == 2)
		{
			Set_ConTableSkillUi(i);
			m_pEffect->CommitChanges();
			m_vecSkillUIBack[i]->m_pBufferCom->Render(NULL);

			if(m_vecSkillCoolBack[i]->m_fCoolTime >= 0)
			{
				if(m_vecSkillCoolBack[i]->m_bCoolTime == true)
				{
					CoolDown(i);
					Set_ConTableCool(i);
					m_pEffect->CommitChanges();
					m_pBufferCoolCom[i]->Render(NULL);
				}
			}
			else
			{
				m_vecSkillCoolBack[i]->m_bCoolTime = false;
				m_vecSkillCoolBack[i]->m_fCoolTime = m_vecSkillCoolBack[i]->m_fCoolTimeBack;
				m_vecSkillCool[i].m_bCoolTime = false;
				m_vecSkillCool[i].m_fCoolTime = m_vecSkillCool[i].m_fCoolTime;
				m_vecSkillCoolBack[i]->m_bSkillStart = true;
				m_vecSkillCool[i].m_bSkillStart = true;
			}
		}
	}

	for(int i = 0; i < SL_End; ++i)
	{
		if(m_vecSkillUIBack[i]->m_iRenderRank == 1)
		{
			Set_ConTableSkillUi(i);
			m_pEffect->CommitChanges();
			m_vecSkillUIBack[i]->m_pBufferCom->Render(NULL);

			if(m_vecSkillCoolBack[i]->m_fCoolTime >= 0)		// ��Ÿ���� 0 ���ϰ� �ɶ����� ��Ÿ��â�� ���� 
			{
				if(m_vecSkillCoolBack[i]->m_bCoolTime == true)
				{
					CoolDown(i);
					//SelectMove();
					Set_ConTableCool(i);
					m_pEffect->CommitChanges();
					m_pBufferCoolCom[i]->Render(NULL);
				}
			}
			else		// ��Ÿ���� 0 ���Ϸ� ���� 
			{
				m_vecSkillCoolBack[i]->m_bCoolTime = false;
				m_vecSkillCoolBack[i]->m_fCoolTime = m_vecSkillCoolBack[i]->m_fCoolTimeBack;
				m_vecSkillCool[i].m_bCoolTime = false;
				m_vecSkillCool[i].m_fCoolTime = m_vecSkillCool[i].m_fCoolTime;
				m_vecSkillCoolBack[i]->m_bSkillStart = true;
				m_vecSkillCool[i].m_bSkillStart = true;
			}
		}
	}

	for(int i = 0; i < SF_END; ++i)
	{
		SkillFontControl(i);
		Set_ConTableSkillFont(i);
		m_pEffect->CommitChanges();
		m_pBufferComFont[i]->Render(NULL);
		m_pBufferComFont[i]->Set_VtxInfo(m_pOriginVertexFont);
	}

	m_pEffect->EndPass();
	m_pEffect->End();

	
}

CPlayerSkill* CPlayerSkill::Create(LPDIRECT3DDEVICE9 pGraphicDev, int iStageNum)
{
	CPlayerSkill*		pGameObject = new CPlayerSkill(pGraphicDev);

	if(FAILED(pGameObject->Initialize(iStageNum)))
	{
		MSG_BOX("CSkillList Created Failed");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CPlayerSkill::Set_ConTableSkillUi(int iSkillIndex)
{
	m_vecSkillUIBack[iSkillIndex]->m_matView._11 = m_vecSkillUIBack[iSkillIndex]->m_vSize.x * 0.2f;	// UV�� �߶��� ������ ����� �׸�ŭ �ٿ��ش�.
	m_vecSkillUIBack[iSkillIndex]->m_matView._22 = m_vecSkillUIBack[iSkillIndex]->m_vSize.y;	// ��ų�� �����̱� ������ Y���� �����ʿ�� ���� 
	m_vecSkillUIBack[iSkillIndex]->m_matView._33 = 1.f;

	m_vecSkillUIBack[iSkillIndex]->m_matView._41 = m_vecSkillUIBack[iSkillIndex]->m_vPos.x - WINSIZEX * 0.5f + 500.f + (200.f * (1.f - CDynamicCamera::GetInstance()->GetCurrentDistance() / 25.f));
	m_vecSkillUIBack[iSkillIndex]->m_matView._42 = m_vecSkillUIBack[iSkillIndex]->m_vPos.y - WINSIZEY * 0.5f;

	m_pEffect->SetMatrix("g_matView", &m_vecSkillUIBack[iSkillIndex]->m_matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	// m_fTransparent ������ ���� ������ ��ȭ��Ŵ . (�ַ� �������� ����� ����)
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4(m_vecSkillUIBack[iSkillIndex]->m_fTransparent, m_vecSkillUIBack[iSkillIndex]->m_fTransparent, m_vecSkillUIBack[iSkillIndex]->m_fTransparent, m_vecSkillUIBack[iSkillIndex]->m_fTransparent));

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");
}

void CPlayerSkill::Set_ConTableCool(int iSkillIndex)
{
	float fTempX = m_vecSkillCoolBack[iSkillIndex]->m_vSize.x * 0.2f;
	float fTempY = m_vecSkillCoolBack[iSkillIndex]->m_vSize.y;

	m_vecSkillCoolBack[iSkillIndex]->m_matView._11 = m_vecSkillCoolBack[iSkillIndex]->m_vSize.x * 0.2f;
	m_vecSkillCoolBack[iSkillIndex]->m_matView._22 = m_vecSkillCoolBack[iSkillIndex]->m_vSize.y;
	m_vecSkillCoolBack[iSkillIndex]->m_matView._33 = 1.f;

	m_vecSkillCoolBack[iSkillIndex]->m_matView._41 = m_vecSkillCoolBack[iSkillIndex]->m_vPos.x - WINSIZEX * 0.5f + 500.f + (200.f * (1.f - CDynamicCamera::GetInstance()->GetCurrentDistance() / 25.f));
	m_vecSkillCoolBack[iSkillIndex]->m_matView._42 = m_vecSkillCoolBack[iSkillIndex]->m_vPos.y - WINSIZEY * 0.5f;

	m_pEffect->SetMatrix("g_matView", &m_vecSkillCoolBack[iSkillIndex]->m_matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	m_pEffect->SetVector("g_vLightDiffuse", &_vec4(m_vecSkillCoolBack[iSkillIndex]->m_fTransparent, m_vecSkillCoolBack[iSkillIndex]->m_fTransparent, m_vecSkillCoolBack[iSkillIndex]->m_fTransparent, m_vecSkillCoolBack[iSkillIndex]->m_fTransparent));

	m_pTextureCoolCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");
}

void CPlayerSkill::SkillFontControl(int iNum)
{
	if(m_bFailFont[SF_COOLTIME] == true)
	{
		if(m_eFirstFont == SF_SP)
		{
			m_vPosFont[SF_COOLTIME].y = m_vPosOriginFont.y - 100.f;
		}
		else
		{
			m_vPosFont[SF_COOLTIME].y = m_vPosOriginFont.y;
			m_eFirstFont = SF_COOLTIME;
		}
		
		printf("m_bFailFont[SF_COOLTIME] == true, %d \n", m_fTransparentFont[SF_COOLTIME]);
		

		if(m_fTransparentFont[SF_COOLTIME] >= 0.f)
		{
			m_fTransparentFont[SF_COOLTIME] -= m_fTimeDelta * 0.05f;
		}
		else
		{
			m_bFailFont[SF_COOLTIME] = false;
		}

		//// ���� ��
		//m_pVertexFont[SF_COOLTIME][0].vTexUV.x = m_vStartUVFont[SF_COOLTIME].x;
		//m_pVertexFont[SF_COOLTIME][0].vTexUV.y = m_vStartUVFont[SF_COOLTIME].y;

		//// ������ ��
		//m_pVertexFont[SF_COOLTIME][1].vTexUV.x = m_vLastUVFont[SF_COOLTIME].x;
		//m_pVertexFont[SF_COOLTIME][1].vTexUV.y = m_vStartUVFont[SF_COOLTIME].y;

		//// ������ �Ʒ�
		//m_pVertexFont[SF_COOLTIME][2].vTexUV.x = m_vLastUVFont[SF_COOLTIME].x;
		//m_pVertexFont[SF_COOLTIME][2].vTexUV.y = m_vLastUVFont[SF_COOLTIME].y;

		//// ������ ��
		//m_pVertexFont[SF_COOLTIME][3].vTexUV.x = m_vStartUVFont[SF_COOLTIME].x;
		//m_pVertexFont[SF_COOLTIME][3].vTexUV.y = m_vLastUVFont[SF_COOLTIME].y;

		//m_pBufferComFont[SF_COOLTIME]->Set_VtxInfo(m_pVertexFont[SF_COOLTIME]);
	}
	else
	{
		if(m_fTransparentFont[SF_COOLTIME] <= 1.f)
		{
			m_fTransparentFont[SF_COOLTIME] += m_fTimeDelta * 0.05f;
		}
	}
		
	if(m_bFailFont[SF_SP] == true)
	{
		if(m_eFirstFont == SF_COOLTIME)
		{
			m_vPosFont[SF_SP].y = m_vPosOriginFont.y - 100.f;
		}
		else
		{
			m_vPosFont[SF_SP].y = m_vPosOriginFont.y;
			m_eFirstFont = SF_SP;
		}

		if(m_fTransparentFont[SF_SP] >= 0.f)
		{
			m_fTransparentFont[SF_SP] -= m_fTimeDelta * 0.05f;
		}
		else
		{
			m_bFailFont[SF_SP] = false;
		}

		//// ���� ��
		//m_pVertexFont[SF_SP][0].vTexUV.x = m_vStartUVFont[SF_SP].x;
		//m_pVertexFont[SF_SP][0].vTexUV.y = m_vStartUVFont[SF_SP].y;

		//// ������ ��
		//m_pVertexFont[SF_SP][1].vTexUV.x = m_vLastUVFont[SF_SP].x;
		//m_pVertexFont[SF_SP][1].vTexUV.y = m_vStartUVFont[SF_SP].y;

		//// ������ �Ʒ�
		//m_pVertexFont[SF_SP][2].vTexUV.x = m_vLastUVFont[SF_SP].x;
		//m_pVertexFont[SF_SP][2].vTexUV.y = m_vLastUVFont[SF_SP].y;

		//// ������ ��
		//m_pVertexFont[SF_SP][3].vTexUV.x = m_vStartUVFont[SF_SP].x;
		//m_pVertexFont[SF_SP][3].vTexUV.y = m_vLastUVFont[SF_SP].y;

		//m_pBufferComFont[SF_SP]->Set_VtxInfo(m_pVertexFont[SF_SP]);
	}
	else
	{
		if(m_fTransparentFont[SF_SP] <= 1.f)
		{
			m_fTransparentFont[SF_SP] += m_fTimeDelta * 0.05f;
		}
	}

	if(m_bFailFont[SF_COOLTIME] == false && m_bFailFont[SF_SP] == false)
		m_eFirstFont = SF_OFF;
}

void CPlayerSkill::Set_ConTableSkillFont(int iFontIndex)
{
	m_matViewFont[iFontIndex]._11 = m_vSizeFont[iFontIndex].x * 0.7f;
	m_matViewFont[iFontIndex]._22 = m_vSizeFont[iFontIndex].y * 0.5f * 0.7f;
	m_matViewFont[iFontIndex]._33 = 1.f;

	m_matViewFont[iFontIndex]._41 = 0.f;
	m_matViewFont[iFontIndex]._42 = m_vPosFont[iFontIndex].y - WINSIZEY * 0.5f - 15.f; //�ϴ� �ӽ÷� �ؼ� ���Ⱦ� ������

	m_pEffect->SetMatrix("g_matView", &m_matViewFont[iFontIndex]);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	m_pEffect->SetVector("g_vLightDiffuse", &_vec4(0.f, 0.f, 0.f, m_fTransparentFont[iFontIndex]));

	m_pTextureComFont->Set_ConstantTable(m_pEffect, "g_BaseTexture");
}

_ulong CPlayerSkill::Release(void)
{
	_ulong		dwRefCnt	= Engine::CGameObject::Release();

	if( 0 == dwRefCnt )
	{
		vector<SKILLUI*>::iterator	iterSkill		= m_vecSkillUIBack.begin();
		vector<SKILLUI*>::iterator	iterSkillEnd	= m_vecSkillUIBack.end();

		vector<SKILLUI*>::iterator	iterCoolBack		= m_vecSkillCoolBack.begin();
		vector<SKILLUI*>::iterator	iterCoolEndBack		= m_vecSkillCoolBack.end();

		vector<SKILLUI>::iterator iterCool		= m_vecSkillCool.begin();
		vector<SKILLUI>::iterator iterCoolEnd	= m_vecSkillCool.end();

		for(int i = 0 ; i < 5 ; ++i, ++iterCoolBack, ++iterCool, ++iterSkill)
		{
			Engine::Safe_Delete_Array((*iterSkill)->m_pVertex);
			Engine::Safe_Delete_Array((*iterCoolBack)->m_pVertex);
			Engine::Safe_Delete_Array((*iterCoolBack)->m_pOriginVertex);

			Safe_Delete(*iterSkill);
			Safe_Delete(*iterCoolBack);
		}
		for(int i = 0; i < SF_END; ++i)
		{
			Engine::Safe_Delete_Array(m_pVertexFont[i]);
			Engine::Safe_Delete_Array(m_pOriginVertexFont[i]);
		}

		delete this;
	}
	return dwRefCnt;
}


bool CPlayerSkill::SkillSelect()
{
	if(Engine::GetDIKeyState(DIK_Z) & 0x80 && !m_bButtonDown)		// �ð����� �̿��� �ߺ�Ű üũ 
	{
		for(int i = 0; i < SL_End; ++i)
		{
			if(m_vecSkillCoolBack[i]->m_iRenderRank == 1)
			{
				m_bButtonDown = true;		// �����̿� ��ư
				m_iCenterNum = i;			// �����Ҷ��� �߾� ��ų�� ������ �˱����� ���� ���� ��ų�� ����
				if(m_vecSkillCoolBack[i]->m_bCoolTime == true)	// �̹� ��Ÿ���� ���� ������
				{
					// Ư�� ���� ������ 
					m_bFailFont[SF_COOLTIME] = true;

					if(m_eFirstFont == SF_OFF)
					{
						m_eFirstFont = SF_COOLTIME;
						CSkillFailFont::GetInstance()->m_bRender = true;
						CSkillFailFont::GetInstance()->m_iTextureNumber = 0;
					}

					return false;
				}
				else if(m_vecSkillCoolBack[i]->m_fNeedSp > CPlayer::GetInstance()->GetPlayerSP()) // SP�� �����Ҷ�
				{
					// Ư�� ���� ������ 
					m_bFailFont[SF_SP] = true;
					
					if(m_eFirstFont == SF_OFF)
					{
						m_eFirstFont = SF_SP;
						CSkillFailFont::GetInstance()->m_bRender = true;
						CSkillFailFont::GetInstance()->m_iTextureNumber = 1;
					}

					

					return false;
				}

				if(CPlayer::GetInstance()->PlayerSkill_Input(m_iCenterNum))
				{
					if(i != SL_Judgement)
					{
						m_vecSkillCoolBack[i]->m_bCoolTime = true;		// ��Ÿ��â ������ ���� (���)
						m_vecSkillCool[i].m_bCoolTime = true;			// ��Ÿ��â ������ ����
					}
					else
					{
						//CPlayer::GetInstance()->PlayerSkill_Input(m_iCenterNum) �� true�� ��ȯ�ϸ� ��ų�� ���� �ߴٴ� ���̴�.
						
						if(m_iJudgementCnt < 1)
						{
							if(m_bJudgement[0] == false)
							{
								m_bJudgement[0] = true;
								m_iJudgementCnt += 1;
							}
							else if(m_bJudgement[1] == false)
							{
								m_bJudgement[1] = true;
								m_iJudgementCnt += 1;
							}
						}
						else
						{
							m_vecSkillCoolBack[SL_Judgement]->m_bCoolTime = true;
							m_vecSkillCool[SL_Judgement].m_bCoolTime = true;
							m_iJudgementCnt = 0;
						}
					}
				}
			}
		}
	}

	//Judgement_l�� ����� Judgement_r�� �ƴ� �ٸ� ������� �ٲ�ų�
	//JUdgement_r�� �ִϸ��̼��� ������ �ǰ� ���� �ٸ� ������� �ٲ�� ��Ÿ���� ����.
	if(m_bJudgement[0] == true && (CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() != Player::SP_attack_judgement_l && CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() != Player::SP_attack_judgement_r))
	{
		m_vecSkillCoolBack[SL_Judgement]->m_bCoolTime = true;
		m_vecSkillCool[SL_Judgement].m_bCoolTime = true;

		m_bJudgement[0] = false;
		m_bJudgement[1] = false;
	}	else if(m_bJudgement[1] == true && (CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() != Player::SP_attack_judgement_l && CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() != Player::SP_attack_judgement_r))
	{
		m_vecSkillCoolBack[SL_Judgement]->m_bCoolTime = true;
		m_vecSkillCool[SL_Judgement].m_bCoolTime = true;

		m_bJudgement[0] = false;
		m_bJudgement[1] = false;
	}
	return true;
}

void CPlayerSkill::Init_JudgementCoolTime()
{
	m_vecSkillCool[SL_Judgement].m_fCoolTime = 0;
	m_vecSkillCoolBack[SL_Judgement]->m_fCoolTime = 0;
}

void CPlayerSkill::CoolDown(int i)		// HP ����� �Լ��� ���� 
{
	m_vecSkillCoolBack[i]->m_fCoolTime -= m_fTimeDelta;		

	m_vecSkillCoolBack[i]->m_fPer = (m_vecSkillCoolBack[i]->m_fCoolTime / m_vecSkillCoolBack[i]->m_fCoolTimeBack); // ���� ��Ÿ�� ����
	m_vecSkillCoolBack[i]->m_pVertex[2].vTexUV.y = m_vecSkillCoolBack[i]->m_fPer;
	m_vecSkillCoolBack[i]->m_pVertex[3].vTexUV.y = m_vecSkillCoolBack[i]->m_fPer;

	m_vecSkillCool[i].m_fPer = (m_vecSkillCool[i].m_fCoolTime / m_vecSkillCool[i].m_fCoolTimeBack);

	if(m_bMove == false)	// ��Ÿ���� ���� ��ġ���� ���÷� �ٲ���� �Ǵµ� ��Ÿ���� ���� ���� 
		// ��ư ��ü�� �� Ȥ�� �Ʒ��� ���� ��Ÿ��â�� ��ġ�� �̻��� ���� �װ� �ִ��� �������� 
		// ��ư�� �̵��ϴ� ���ȿ��� ��Ÿ��â�� ��ġ �̵��� ����� ��
	{	
		if(i == m_iCenterNum)
		{
			m_vecSkillCoolBack[i]->m_vPos.y		= m_vecSkillCoolBack[i]->m_vOrizinPos.y + ((m_vecSkillCoolBack[i]->m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[i]->m_fPer)) + m_fTempYPos[m_iCenterNum];
			m_vecSkillCoolBack[i]->m_vSize.y	= (m_vecSkillCoolBack[i]->m_vOriginSize.y * m_vecSkillCoolBack[i]->m_fPer);
		}
		else
		{
			m_vecSkillCoolBack[i]->m_vPos.y		= m_vecSkillCoolBack[i]->m_vOrizinPos.y + ((m_vecSkillCoolBack[i]->m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[i]->m_fPer));
			m_vecSkillCoolBack[i]->m_vSize.y	= (m_vecSkillCoolBack[i]->m_vOriginSize.y * m_vecSkillCoolBack[i]->m_fPer);
		}
	}
	m_pBufferCoolCom[i]->Set_VtxInfo(m_vecSkillCoolBack[i]->m_pVertex);
}

// ó������ ��ų �����ܵ��� 0.3�ʰ� �̵���Ű�� �ð��� ������

// ����� ����				0 1 2 3 4 �� 
// ���� �ٲ� ���� ����		1 2 3 4 0 �� ����ִ´�.
// ���⼭ ����� ������ ���� ������ �����ϴ� ����ε� 
// �Լ��� ������ ���� ���� ���� 0 1 2 3 4�� 
// ����� ����					0 1 2 3 4�� ����־� ���� ������ ���� �������ش�.

void CPlayerSkill::SelectMove()
{
	if(m_bButtonDown == true)
	{
		m_fButtonTime += m_fTimeDelta;
		m_fMoveSillUI += 150 * m_fTimeDelta;
		if(m_fMoveSillUI > 10.f)
			m_fMoveSillUI = 10.f;
		//�ð����� �ؼ� �����µ��� ������������ �ѹ��� ġ�� �������� �̵� �Ҽ� �ְ� �Ѵ�.
		m_vecSkillCoolBack[m_iCenterNum]->m_vPos.x  = m_vecSkillCoolBack[m_iCenterNum]->m_vOrizinPos.x + m_fMoveSillUI;
		//m_vecSkillCoolBack[m_iCenterNum]->m_vPos.y  = m_vecSkillCoolBack[m_iCenterNum]->m_vOrizinPos.y + ((m_vecSkillCoolBack[m_iCenterNum]->m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[m_iCenterNum]->m_fPer)) - m_fMoveSillUI * 0.5f;
		m_vecSkillUIBack[m_iCenterNum]->m_vPos.x	= m_vecSkillCoolBack[m_iCenterNum]->m_vOrizinPos.x + m_fMoveSillUI;
		m_vecSkillUIBack[m_iCenterNum]->m_vPos.y	= m_vecSkillCoolBack[m_iCenterNum]->m_vOrizinPos.y - m_fMoveSillUI*0.5f;

		m_fTempYPos[m_iCenterNum] =  - m_fMoveSillUI*0.5f;
	}
	else
	{
		m_fButtonTime = 0.f;
		m_fMoveSillUI = 0.f;
		m_fTempYPos[m_iCenterNum] = 0.f;
		m_vecSkillCoolBack[m_iCenterNum]->m_vPos  = m_vecSkillCoolBack[m_iCenterNum]->m_vOrizinPos;
		m_vecSkillUIBack[m_iCenterNum]->m_vPos	= m_vecSkillCoolBack[m_iCenterNum]->m_vOrizinPos;
	}
}

void CPlayerSkill::ReduceCoolTime(int iSkillNum, int iReduceTime)
{
	for(int i = 0; i < SL_End; ++i)
	{
		if(m_vecSkillCoolBack[i]->m_eName == (SKILL_LIST)iSkillNum)
		{
			if(m_vecSkillCoolBack[i]->m_bCoolTime == true)
			{
				m_vecSkillCoolBack[i]->m_fCoolTime  -= iReduceTime;
				m_vecSkillCool[i].m_fCoolTime		-= iReduceTime;

			/*	if(m_vecSkillCoolBack[i]->m_fCoolTime < 0.f)
				{
					m_vecSkillCoolBack[i]->m_fCoolTime = 0.f;
					m_vecSkillCool[i].m_fCoolTime	   = 0.f;
				}*/
			}
		}
	}
}


void CPlayerSkill::SkillDownChange(int iTemp)
{
	_float fSizeControl = 80.f; // ������ ��Ʈ���� �ѹ��� �ϱ� ���� �ӽ÷� ���� ����

	if(iTemp == 2)	// ��ų �߾Ӱ��� �ش��ϴ� ��ų���� 2�϶�
	{
		if(m_fIconTime < 0.3f)		// ������(��ųâ)�� �̵��ϴ� �ð� 
		{
			m_bMove = true;

			// ���� ������â�� �ٲܶ�
			m_vecSkillUIBack[0]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[1].m_vSize;

			m_vecSkillUIBack[1]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;		
			m_vecSkillUIBack[1]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillUIBack[1]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;	// �߾����� ���� ��ų�̱� ������ ũ�⸦ ���� Ű��

			m_vecSkillUIBack[2]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillUIBack[2]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;	// �߾ӿ��� �������� ��ų�̱� ������ ũ�⸦ ���� ����

			m_vecSkillUIBack[3]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 0.f;

			m_vecSkillUIBack[4]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[0].m_vSize;

			// ��ų ��Ÿ��â�� �ٲܶ�
			m_vecSkillCoolBack[0]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vSize = m_vecSkillCool[1].m_vSize;

			m_vecSkillCoolBack[1]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillCoolBack[1]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillCoolBack[2]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillCoolBack[2]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillCoolBack[3]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vSize = m_vecSkillCool[4].m_vSize;
			m_vecSkillCoolBack[3]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[4]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vSize = m_vecSkillCool[0].m_vSize;
		}
		else		// ������ ��ų�� �̵��� ��ġ�� ��ĭ �Ʒ��� �ε������� �����ϴ� ����
		{
			m_fIconTime = 0.f;

			m_vecSkillUIBack[0]->m_vPos = m_vecSkillUI[1].m_vPos;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[1].m_vSize;
			m_vecSkillUIBack[0]->m_fTransparent = 0.f;

			m_vecSkillUIBack[1]->m_vPos = m_vecSkillUI[2].m_vPos;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[2].m_vSize;
			m_vecSkillUIBack[1]->m_fTransparent = 0.f;

			m_vecSkillUIBack[2]->m_vPos = m_vecSkillUI[3].m_vPos;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[3].m_vSize;
			m_vecSkillUIBack[2]->m_fTransparent = 0.f;

			m_vecSkillUIBack[3]->m_vPos = m_vecSkillUI[4].m_vPos;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 1.f;

			m_vecSkillUIBack[4]->m_vPos = m_vecSkillUI[0].m_vPos;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[0].m_vSize;
			m_vecSkillUIBack[4]->m_fTransparent = 1.f;

			m_vecSkillUI[0] = *m_vecSkillUIBack[0];		// ������͸� �̿��� ���� ��ųâ ���͵� �ٲ۴�.
			m_vecSkillUI[1] = *m_vecSkillUIBack[1];
			m_vecSkillUI[2] = *m_vecSkillUIBack[2];
			m_vecSkillUI[3] = *m_vecSkillUIBack[3];
			m_vecSkillUI[4] = *m_vecSkillUIBack[4];


			// ���� ��ũ�� ��ġ�� ���� �״�ο��� �ϴ°͸� �ٲ���� �Ѵ�.

			m_vecSkillCoolBack[0]->m_vPos = _vec3(m_vecSkillCool[1].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y - ((m_vecSkillCool[1].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[0]->m_fPer)), 0.f);
			// ���� ��ų���� ��ġ���� ��Ÿ���� �����Ų ��ġ���� �Բ� ���� ��Ű�鼭 ������ش�.
			m_vecSkillCoolBack[0]->m_vOrizinPos = m_vecSkillCool[1].m_vOrizinPos;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vOriginSize = m_vecSkillCool[1].m_vOriginSize;
			m_vecSkillCoolBack[0]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[1]->m_vPos = _vec3(m_vecSkillCool[2].m_vPos.x ,m_vecSkillCool[2].m_vOrizinPos.y - ((m_vecSkillCool[2].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[1]->m_fPer)), 0.f);
			m_vecSkillCoolBack[1]->m_vOrizinPos = m_vecSkillCool[2].m_vOrizinPos;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vOriginSize = m_vecSkillCool[2].m_vOriginSize;
			m_vecSkillCoolBack[1]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[2]->m_vPos = _vec3(m_vecSkillCool[3].m_vPos.x ,m_vecSkillCool[3].m_vOrizinPos.y - ((m_vecSkillCool[3].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[2]->m_fPer)), 0.f);
			m_vecSkillCoolBack[2]->m_vOrizinPos = m_vecSkillCool[3].m_vOrizinPos;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vOriginSize = m_vecSkillCool[3].m_vOriginSize;
			m_vecSkillCoolBack[2]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[3]->m_vPos = _vec3(m_vecSkillCool[4].m_vPos.x ,m_vecSkillCool[4].m_vOrizinPos.y - ((m_vecSkillCool[4].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[3]->m_fPer)), 0.f);
			m_vecSkillCoolBack[3]->m_vOrizinPos = m_vecSkillCool[4].m_vOrizinPos;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vOriginSize = m_vecSkillCool[4].m_vOriginSize;
			m_vecSkillCoolBack[3]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[4]->m_vPos = _vec3(m_vecSkillCool[0].m_vPos.x ,m_vecSkillCool[0].m_vOrizinPos.y - ((m_vecSkillCool[0].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[4]->m_fPer)), 0.f);
			m_vecSkillCoolBack[4]->m_vOrizinPos = m_vecSkillCool[0].m_vOrizinPos;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vOriginSize = m_vecSkillCool[0].m_vOriginSize;
			m_vecSkillCoolBack[4]->m_fTransparent = 1.f;

			m_vecSkillCool[0] = *m_vecSkillCoolBack[0];
			m_vecSkillCool[1] = *m_vecSkillCoolBack[1];
			m_vecSkillCool[2] = *m_vecSkillCoolBack[2];
			m_vecSkillCool[3] = *m_vecSkillCoolBack[3];
			m_vecSkillCool[4] = *m_vecSkillCoolBack[4];

			m_eChange = CHANGE_NULL;		// �� ������ ������ ���� enum���� �ٲ㼭 �� �Լ��� �ٽ� ������ ���ϰ� �� .
			m_bMove = false;				// ��Ÿ�� �������� �̵��Ҷ� ��ġ���� ���̴°� �����ϱ� ���� ���� 
		}
	}

	if(iTemp == 3)
	{
		if(m_fIconTime < 0.3f)
		{
			m_bMove = true;
			m_vecSkillUIBack[1]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[2].m_vSize;

			m_vecSkillUIBack[2]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillUIBack[2]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillUIBack[3]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillUIBack[3]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillUIBack[4]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[0].m_vSize;
			m_vecSkillUIBack[4]->m_fTransparent = 0.f;

			m_vecSkillUIBack[0]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[1].m_vSize;

			m_vecSkillCoolBack[1]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vSize = m_vecSkillCool[2].m_vSize;

			m_vecSkillCoolBack[2]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillCoolBack[2]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillCoolBack[3]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillCoolBack[3]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillCoolBack[4]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vSize = m_vecSkillCool[0].m_vSize;
			m_vecSkillCoolBack[4]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[0]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vSize = m_vecSkillCool[1].m_vSize;
		}
		else
		{
			m_fIconTime = 0.f;
			m_vecSkillUIBack[1]->m_vPos = m_vecSkillUI[2].m_vPos;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[2].m_vSize;
			m_vecSkillUIBack[1]->m_fTransparent = 0.f;

			m_vecSkillUIBack[2]->m_vPos = m_vecSkillUI[3].m_vPos;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[3].m_vSize;
			m_vecSkillUIBack[2]->m_fTransparent = 0.f;

			m_vecSkillUIBack[3]->m_vPos = m_vecSkillUI[4].m_vPos;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 0.f;

			m_vecSkillUIBack[4]->m_vPos = m_vecSkillUI[0].m_vPos;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[0].m_vSize;
			m_vecSkillUIBack[4]->m_fTransparent = 1.f;

			m_vecSkillUIBack[0]->m_vPos = m_vecSkillUI[1].m_vPos;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[1].m_vSize;
			m_vecSkillUIBack[0]->m_fTransparent = 1.f;

			m_vecSkillUI[0] = *m_vecSkillUIBack[0];
			m_vecSkillUI[1] = *m_vecSkillUIBack[1];
			m_vecSkillUI[2] = *m_vecSkillUIBack[2];
			m_vecSkillUI[3] = *m_vecSkillUIBack[3];
			m_vecSkillUI[4] = *m_vecSkillUIBack[4];

			m_vecSkillCoolBack[1]->m_vPos = _vec3(m_vecSkillCool[2].m_vPos.x ,m_vecSkillCool[2].m_vOrizinPos.y + ((m_vecSkillCool[2].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[1]->m_fPer)), 0.f);
			m_vecSkillCoolBack[1]->m_vOrizinPos = m_vecSkillCool[2].m_vOrizinPos;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vOriginSize = m_vecSkillCool[2].m_vOriginSize;
			m_vecSkillCoolBack[1]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[2]->m_vPos = _vec3(m_vecSkillCool[3].m_vPos.x ,m_vecSkillCool[3].m_vOrizinPos.y + ((m_vecSkillCool[3].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[2]->m_fPer)), 0.f);
			m_vecSkillCoolBack[2]->m_vOrizinPos = m_vecSkillCool[3].m_vOrizinPos;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vOriginSize = m_vecSkillCool[3].m_vOriginSize;
			m_vecSkillCoolBack[2]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[3]->m_vPos = _vec3(m_vecSkillCool[4].m_vPos.x ,m_vecSkillCool[4].m_vOrizinPos.y + ((m_vecSkillCool[4].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[3]->m_fPer)), 0.f);
			m_vecSkillCoolBack[3]->m_vOrizinPos = m_vecSkillCool[4].m_vOrizinPos;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vOriginSize = m_vecSkillCool[4].m_vOriginSize;
			m_vecSkillCoolBack[3]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[4]->m_vPos = _vec3(m_vecSkillCool[0].m_vPos.x ,m_vecSkillCool[0].m_vOrizinPos.y + ((m_vecSkillCool[0].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[4]->m_fPer)), 0.f);
			m_vecSkillCoolBack[4]->m_vOrizinPos = m_vecSkillCool[0].m_vOrizinPos;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vOriginSize = m_vecSkillCool[0].m_vOriginSize;
			m_vecSkillCoolBack[4]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[0]->m_vPos = _vec3(m_vecSkillCool[1].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y + ((m_vecSkillCool[1].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[0]->m_fPer)), 0.f);
			m_vecSkillCoolBack[0]->m_vOrizinPos = m_vecSkillCool[1].m_vOrizinPos;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vOriginSize = m_vecSkillCool[1].m_vOriginSize;
			m_vecSkillCoolBack[0]->m_fTransparent = 1.f;

			m_vecSkillCool[0] = *m_vecSkillCoolBack[0];
			m_vecSkillCool[1] = *m_vecSkillCoolBack[1];
			m_vecSkillCool[2] = *m_vecSkillCoolBack[2];
			m_vecSkillCool[3] = *m_vecSkillCoolBack[3];
			m_vecSkillCool[4] = *m_vecSkillCoolBack[4];

			m_eChange = CHANGE_NULL;
			m_bMove = false;
		}
	}

	if(iTemp == 4)
	{
		if(m_fIconTime < 0.3f)
		{
			m_bMove = true;
			m_vecSkillUIBack[2]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[3].m_vSize;

			m_vecSkillUIBack[3]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillUIBack[3]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillUIBack[4]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillUIBack[4]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillUIBack[0]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[1].m_vSize;
			m_vecSkillUIBack[0]->m_fTransparent = 0.f;

			m_vecSkillUIBack[1]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[2].m_vSize;

			m_vecSkillCoolBack[2]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vSize = m_vecSkillCool[3].m_vSize;

			m_vecSkillCoolBack[3]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillCoolBack[3]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillCoolBack[4]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillCoolBack[4]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillCoolBack[0]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vSize = m_vecSkillCool[1].m_vSize;
			m_vecSkillCoolBack[0]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[1]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vSize = m_vecSkillCool[2].m_vSize;
		}
		else
		{
			m_fIconTime = 0.f;
			m_vecSkillUIBack[2]->m_vPos = m_vecSkillUI[3].m_vPos;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[3].m_vSize;
			m_vecSkillUIBack[2]->m_fTransparent = 0.f;

			m_vecSkillUIBack[3]->m_vPos = m_vecSkillUI[4].m_vPos;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 0.f;

			m_vecSkillUIBack[4]->m_vPos = m_vecSkillUI[0].m_vPos;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[0].m_vSize;
			m_vecSkillUIBack[4]->m_fTransparent = 0.f;

			m_vecSkillUIBack[0]->m_vPos = m_vecSkillUI[1].m_vPos;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[1].m_vSize;
			m_vecSkillUIBack[0]->m_fTransparent = 1.f;

			m_vecSkillUIBack[1]->m_vPos = m_vecSkillUI[2].m_vPos;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[2].m_vSize;
			m_vecSkillUIBack[1]->m_fTransparent = 1.f;

			m_vecSkillUI[0] = *m_vecSkillUIBack[0];
			m_vecSkillUI[1] = *m_vecSkillUIBack[1];
			m_vecSkillUI[2] = *m_vecSkillUIBack[2];
			m_vecSkillUI[3] = *m_vecSkillUIBack[3];
			m_vecSkillUI[4] = *m_vecSkillUIBack[4];

			m_vecSkillCoolBack[2]->m_vPos = _vec3(m_vecSkillCool[3].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y + ((m_vecSkillCool[3].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[2]->m_fPer)), 0.f);
			m_vecSkillCoolBack[2]->m_vOrizinPos = m_vecSkillCool[3].m_vOrizinPos;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vOriginSize = m_vecSkillCool[3].m_vOriginSize;
			m_vecSkillCoolBack[2]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[3]->m_vPos = _vec3(m_vecSkillCool[4].m_vPos.x ,m_vecSkillCool[4].m_vOrizinPos.y + ((m_vecSkillCool[4].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[3]->m_fPer)), 0.f);
			m_vecSkillCoolBack[3]->m_vOrizinPos = m_vecSkillCool[4].m_vOrizinPos;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vOriginSize = m_vecSkillCool[4].m_vOriginSize;
			m_vecSkillCoolBack[3]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[4]->m_vPos = _vec3(m_vecSkillCool[0].m_vPos.x ,m_vecSkillCool[0].m_vOrizinPos.y + ((m_vecSkillCool[0].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[4]->m_fPer)), 0.f);
			m_vecSkillCoolBack[4]->m_vOrizinPos = m_vecSkillCool[0].m_vOrizinPos;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vOriginSize = m_vecSkillCool[0].m_vOriginSize;
			m_vecSkillCoolBack[4]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[0]->m_vPos = _vec3(m_vecSkillCool[1].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y + ((m_vecSkillCool[1].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[0]->m_fPer)), 0.f);
			m_vecSkillCoolBack[0]->m_vOrizinPos = m_vecSkillCool[1].m_vOrizinPos;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vOriginSize = m_vecSkillCool[1].m_vOriginSize;
			m_vecSkillCoolBack[0]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[1]->m_vPos = _vec3(m_vecSkillCool[2].m_vPos.x ,m_vecSkillCool[2].m_vOrizinPos.y + ((m_vecSkillCool[2].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[1]->m_fPer)), 0.f);
			m_vecSkillCoolBack[1]->m_vOrizinPos = m_vecSkillCool[2].m_vOrizinPos;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vOriginSize = m_vecSkillCool[2].m_vOriginSize;
			m_vecSkillCoolBack[1]->m_fTransparent = 1.f;

			m_vecSkillCool[0] = *m_vecSkillCoolBack[0];
			m_vecSkillCool[1] = *m_vecSkillCoolBack[1];
			m_vecSkillCool[2] = *m_vecSkillCoolBack[2];
			m_vecSkillCool[3] = *m_vecSkillCoolBack[3];
			m_vecSkillCool[4] = *m_vecSkillCoolBack[4];

			m_eChange = CHANGE_NULL;
			m_bMove = false;
		}
	}

	if(iTemp == 0)
	{
		if(m_fIconTime < 0.3f)
		{
			m_bMove = true;
			m_vecSkillUIBack[0]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillUIBack[0]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillUIBack[1]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[2].m_vSize;
			m_vecSkillUIBack[1]->m_fTransparent = 0.f;

			m_vecSkillUIBack[3]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[4].m_vSize;

			m_vecSkillUIBack[4]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillUIBack[4]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillUIBack[2]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[3].m_vSize;

			m_vecSkillCoolBack[0]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillCoolBack[0]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillCoolBack[1]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vSize = m_vecSkillCool[2].m_vSize;
			m_vecSkillCoolBack[1]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[3]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vSize = m_vecSkillCool[4].m_vSize;

			m_vecSkillCoolBack[4]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillCoolBack[4]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillCoolBack[2]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vSize = m_vecSkillCool[3].m_vSize;
		}
		else
		{
			m_fIconTime = 0.f;
			m_vecSkillUIBack[3]->m_vPos = m_vecSkillUI[4].m_vPos;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 0.f;

			m_vecSkillUIBack[4]->m_vPos = m_vecSkillUI[0].m_vPos;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[0].m_vSize;
			m_vecSkillUIBack[4]->m_fTransparent = 0.f;

			m_vecSkillUIBack[0]->m_vPos = m_vecSkillUI[1].m_vPos;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[1].m_vSize;
			m_vecSkillUIBack[0]->m_fTransparent = 0.f;

			m_vecSkillUIBack[1]->m_vPos = m_vecSkillUI[2].m_vPos;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[2].m_vSize;
			m_vecSkillUIBack[1]->m_fTransparent = 1.f;

			m_vecSkillUIBack[2]->m_vPos = m_vecSkillUI[3].m_vPos;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[3].m_vSize;
			m_vecSkillUIBack[2]->m_fTransparent = 1.f;

			m_vecSkillUI[0] = *m_vecSkillUIBack[0];
			m_vecSkillUI[1] = *m_vecSkillUIBack[1];
			m_vecSkillUI[2] = *m_vecSkillUIBack[2];
			m_vecSkillUI[3] = *m_vecSkillUIBack[3];
			m_vecSkillUI[4] = *m_vecSkillUIBack[4];

			m_vecSkillCoolBack[3]->m_vPos = _vec3(m_vecSkillCool[4].m_vPos.x ,m_vecSkillCool[4].m_vOrizinPos.y + ((m_vecSkillCool[4].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[3]->m_fPer)), 0.f);
			m_vecSkillCoolBack[3]->m_vOrizinPos = m_vecSkillCool[4].m_vOrizinPos;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vOriginSize = m_vecSkillCool[4].m_vOriginSize;
			m_vecSkillCoolBack[3]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[4]->m_vPos = _vec3(m_vecSkillCool[0].m_vPos.x ,m_vecSkillCool[0].m_vOrizinPos.y + ((m_vecSkillCool[0].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[4]->m_fPer)), 0.f);
			m_vecSkillCoolBack[4]->m_vOrizinPos = m_vecSkillCool[0].m_vOrizinPos;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vOriginSize = m_vecSkillCool[0].m_vOriginSize;
			m_vecSkillCoolBack[4]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[0]->m_vPos = _vec3(m_vecSkillCool[1].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y + ((m_vecSkillCool[1].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[0]->m_fPer)), 0.f);
			m_vecSkillCoolBack[0]->m_vOrizinPos = m_vecSkillCool[1].m_vOrizinPos;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vOriginSize = m_vecSkillCool[1].m_vOriginSize;
			m_vecSkillCoolBack[0]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[1]->m_vPos = _vec3(m_vecSkillCool[2].m_vPos.x ,m_vecSkillCool[2].m_vOrizinPos.y + ((m_vecSkillCool[2].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[1]->m_fPer)), 0.f);
			m_vecSkillCoolBack[1]->m_vOrizinPos = m_vecSkillCool[2].m_vOrizinPos;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vOriginSize = m_vecSkillCool[2].m_vOriginSize;
			m_vecSkillCoolBack[1]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[2]->m_vPos = _vec3(m_vecSkillCool[3].m_vPos.x ,m_vecSkillCool[3].m_vOrizinPos.y + ((m_vecSkillCool[3].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[2]->m_fPer)), 0.f);
			m_vecSkillCoolBack[2]->m_vOrizinPos = m_vecSkillCool[3].m_vOrizinPos;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vOriginSize = m_vecSkillCool[3].m_vOriginSize;
			m_vecSkillCoolBack[2]->m_fTransparent = 1.f;

			m_vecSkillCool[0] = *m_vecSkillCoolBack[0];
			m_vecSkillCool[1] = *m_vecSkillCoolBack[1];
			m_vecSkillCool[2] = *m_vecSkillCoolBack[2];
			m_vecSkillCool[3] = *m_vecSkillCoolBack[3];
			m_vecSkillCool[4] = *m_vecSkillCoolBack[4];

			m_eChange = CHANGE_NULL;
			m_bMove = false;
		}
	}

	if(iTemp == 1)
	{
		if(m_fIconTime < 0.3f)
		{
			m_bMove = true;
			m_vecSkillUIBack[4]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[0].m_vSize;

			m_vecSkillUIBack[0]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillUIBack[0]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillUIBack[1]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillUIBack[1]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillUIBack[2]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[3].m_vSize;
			m_vecSkillUIBack[2]->m_fTransparent = 0.f;

			m_vecSkillUIBack[3]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[4].m_vSize;

			m_vecSkillCoolBack[4]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vSize = m_vecSkillCool[0].m_vSize;

			m_vecSkillCoolBack[0]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillCoolBack[0]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillCoolBack[1]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillCoolBack[1]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillCoolBack[2]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vSize = m_vecSkillCool[3].m_vSize;
			m_vecSkillCoolBack[2]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[3]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vSize = m_vecSkillCool[4].m_vSize;
		}
		else
		{
			m_fIconTime = 0.f;
			m_vecSkillUIBack[4]->m_vPos = m_vecSkillUI[0].m_vPos;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[0].m_vSize;
			m_vecSkillUIBack[4]->m_fTransparent = 0.f;

			m_vecSkillUIBack[0]->m_vPos = m_vecSkillUI[1].m_vPos;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[1].m_vSize;
			m_vecSkillUIBack[0]->m_fTransparent = 0.f;

			m_vecSkillUIBack[1]->m_vPos = m_vecSkillUI[2].m_vPos;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[2].m_vSize;
			m_vecSkillUIBack[1]->m_fTransparent = 0.f;

			m_vecSkillUIBack[2]->m_vPos = m_vecSkillUI[3].m_vPos;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[3].m_vSize;
			m_vecSkillUIBack[2]->m_fTransparent = 1.f;

			m_vecSkillUIBack[3]->m_vPos = m_vecSkillUI[4].m_vPos;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 1.f;

			m_vecSkillUI[0] = *m_vecSkillUIBack[0];
			m_vecSkillUI[1] = *m_vecSkillUIBack[1];
			m_vecSkillUI[2] = *m_vecSkillUIBack[2];
			m_vecSkillUI[3] = *m_vecSkillUIBack[3];
			m_vecSkillUI[4] = *m_vecSkillUIBack[4];

			m_vecSkillCoolBack[4]->m_vPos = _vec3(m_vecSkillCool[0].m_vPos.x ,m_vecSkillCool[0].m_vOrizinPos.y + ((m_vecSkillCool[0].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[4]->m_fPer)), 0.f);
			m_vecSkillCoolBack[4]->m_vOrizinPos = m_vecSkillCool[0].m_vOrizinPos;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vOriginSize = m_vecSkillCool[0].m_vOriginSize;
			m_vecSkillCoolBack[4]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[0]->m_vPos = _vec3(m_vecSkillCool[1].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y + ((m_vecSkillCool[1].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[0]->m_fPer)), 0.f);
			m_vecSkillCoolBack[0]->m_vOrizinPos = m_vecSkillCool[1].m_vOrizinPos;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vOriginSize = m_vecSkillCool[1].m_vOriginSize;
			m_vecSkillCoolBack[0]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[1]->m_vPos = _vec3(m_vecSkillCool[2].m_vPos.x ,m_vecSkillCool[2].m_vOrizinPos.y + ((m_vecSkillCool[2].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[1]->m_fPer)), 0.f);
			m_vecSkillCoolBack[1]->m_vOrizinPos = m_vecSkillCool[2].m_vOrizinPos;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vOriginSize = m_vecSkillCool[2].m_vOriginSize;
			m_vecSkillCoolBack[1]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[2]->m_vPos = _vec3(m_vecSkillCool[3].m_vPos.x ,m_vecSkillCool[3].m_vOrizinPos.y + ((m_vecSkillCool[3].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[2]->m_fPer)), 0.f);
			m_vecSkillCoolBack[2]->m_vOrizinPos = m_vecSkillCool[3].m_vOrizinPos;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vOriginSize = m_vecSkillCool[3].m_vOriginSize;
			m_vecSkillCoolBack[2]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[3]->m_vPos = _vec3(m_vecSkillCool[4].m_vPos.x ,m_vecSkillCool[4].m_vOrizinPos.y + ((m_vecSkillCool[4].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[3]->m_fPer)), 0.f);
			m_vecSkillCoolBack[3]->m_vOrizinPos = m_vecSkillCool[4].m_vOrizinPos;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vOriginSize = m_vecSkillCool[4].m_vOriginSize;
			m_vecSkillCoolBack[3]->m_fTransparent = 1.f;

			m_vecSkillCool[0] = *m_vecSkillCoolBack[0];
			m_vecSkillCool[1] = *m_vecSkillCoolBack[1];
			m_vecSkillCool[2] = *m_vecSkillCoolBack[2];
			m_vecSkillCool[3] = *m_vecSkillCoolBack[3];
			m_vecSkillCool[4] = *m_vecSkillCoolBack[4];

			m_eChange = CHANGE_NULL;
			m_bMove = false;
		}
	}
}

void CPlayerSkill::SkillUpChange(int iTemp)
{
	_float fSizeControl = 80.f;

	if(iTemp == 2)
	{
		if(m_fIconTime < 0.3f)
		{
			m_bMove = true;
			m_vecSkillUIBack[0]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[4].m_vSize;

			m_vecSkillUIBack[1]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[1]->m_fTransparent = 0.f;

			m_vecSkillUIBack[2]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillUIBack[2]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillUIBack[3]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillUIBack[3]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillUIBack[4]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[3].m_vSize;

			m_vecSkillCoolBack[0]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vSize = m_vecSkillCool[4].m_vSize;

			m_vecSkillCoolBack[1]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[2]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillCoolBack[2]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillCoolBack[3]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillCoolBack[3]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillCoolBack[4]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vSize = m_vecSkillCool[3].m_vSize;
		}
		else
		{
			m_fIconTime = 0.f;
			m_vecSkillUIBack[0]->m_vPos			= m_vecSkillUI[4].m_vPos;
			m_vecSkillUIBack[0]->m_iRenderRank	= m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize		= m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[0]->m_fTransparent = 1.f;

			m_vecSkillUIBack[1]->m_vPos			= m_vecSkillUI[0].m_vPos;
			m_vecSkillUIBack[1]->m_iRenderRank	= m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize		= m_vecSkillUI[0].m_vSize;
			m_vecSkillUIBack[1]->m_fTransparent = 1.f;	

			m_vecSkillUIBack[2]->m_vPos			= m_vecSkillUI[1].m_vPos;
			m_vecSkillUIBack[2]->m_iRenderRank	= m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize		= m_vecSkillUI[1].m_vSize;
			m_vecSkillUIBack[2]->m_fTransparent = 0.f;

			m_vecSkillUIBack[3]->m_vPos			= m_vecSkillUI[2].m_vPos;
			m_vecSkillUIBack[3]->m_iRenderRank	= m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize		= m_vecSkillUI[2].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 0.f;


			m_vecSkillUIBack[4]->m_vPos			= m_vecSkillUI[3].m_vPos;
			m_vecSkillUIBack[4]->m_iRenderRank	= m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize		= m_vecSkillUI[3].m_vSize;
			m_vecSkillUIBack[4]->m_fTransparent = 0.f;


			m_vecSkillUI[0] = *m_vecSkillUIBack[0];
			m_vecSkillUI[1] = *m_vecSkillUIBack[1];
			m_vecSkillUI[2] = *m_vecSkillUIBack[2];
			m_vecSkillUI[3] = *m_vecSkillUIBack[3];
			m_vecSkillUI[4] = *m_vecSkillUIBack[4];

			//m_vecSkillCoolBack[0]->m_vPos			= m_vecSkillCool[4].m_vPos;
			m_vecSkillCoolBack[0]->m_vPos = _vec3(m_vecSkillCool[4].m_vPos.x ,m_vecSkillCool[4].m_vOrizinPos.y - ((m_vecSkillCool[4].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[0]->m_fPer)), 0.f);
			m_vecSkillCoolBack[0]->m_vOrizinPos		= m_vecSkillCool[4].m_vOrizinPos;
			m_vecSkillCoolBack[0]->m_iRenderRank	= m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vOriginSize	= m_vecSkillCool[4].m_vOriginSize;
			m_vecSkillCoolBack[0]->m_fTransparent	= 1.f;

			//m_vecSkillCoolBack[1]->m_vPos			= m_vecSkillCool[0].m_vPos;
			m_vecSkillCoolBack[1]->m_vPos = _vec3(m_vecSkillCool[0].m_vPos.x ,m_vecSkillCool[0].m_vOrizinPos.y - ((m_vecSkillCool[0].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[1]->m_fPer)), 0.f);
			m_vecSkillCoolBack[1]->m_vOrizinPos		= m_vecSkillCool[0].m_vOrizinPos;
			m_vecSkillCoolBack[1]->m_iRenderRank	= m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vOriginSize	= m_vecSkillCool[0].m_vOriginSize;
			m_vecSkillCoolBack[1]->m_fTransparent	= 1.f;

			//m_vecSkillCoolBack[2]->m_vPos			= m_vecSkillCool[1].m_vPos;
			m_vecSkillCoolBack[2]->m_vPos = _vec3(m_vecSkillCool[1].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y - ((m_vecSkillCool[1].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[2]->m_fPer)), 0.f);
			m_vecSkillCoolBack[2]->m_vOrizinPos = m_vecSkillCool[1].m_vOrizinPos;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vOriginSize = m_vecSkillCool[1].m_vOriginSize;
			m_vecSkillCoolBack[2]->m_fTransparent = 0.f;

			//m_vecSkillCoolBack[3]->m_vPos = m_vecSkillCool[2].m_vPos;
			m_vecSkillCoolBack[3]->m_vPos = _vec3(m_vecSkillCool[2].m_vPos.x ,m_vecSkillCool[2].m_vOrizinPos.y - ((m_vecSkillCool[2].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[3]->m_fPer)), 0.f);
			m_vecSkillCoolBack[3]->m_vOrizinPos = m_vecSkillCool[2].m_vOrizinPos;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vOriginSize = m_vecSkillCool[2].m_vOriginSize;
			m_vecSkillCoolBack[3]->m_fTransparent = 0.f;

			//m_vecSkillCoolBack[4]->m_vPos = m_vecSkillCool[3].m_vPos;
			m_vecSkillCoolBack[4]->m_vPos = _vec3(m_vecSkillCool[3].m_vPos.x ,m_vecSkillCool[3].m_vOrizinPos.y - ((m_vecSkillCool[3].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[4]->m_fPer)), 0.f);
			m_vecSkillCoolBack[4]->m_vOrizinPos = m_vecSkillCool[3].m_vOrizinPos;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vOriginSize = m_vecSkillCool[3].m_vOriginSize;
			m_vecSkillCoolBack[4]->m_fTransparent = 0.f;

			m_vecSkillCool[0] = *m_vecSkillCoolBack[0];
			m_vecSkillCool[1] = *m_vecSkillCoolBack[1];
			m_vecSkillCool[2] = *m_vecSkillCoolBack[2];
			m_vecSkillCool[3] = *m_vecSkillCoolBack[3];
			m_vecSkillCool[4] = *m_vecSkillCoolBack[4];

			m_bMove = false;
			m_eChange = CHANGE_NULL;
		}
	}

	if(iTemp == 3)
	{
		if(m_fIconTime < 0.3f)
		{
			m_bMove = true;
			m_vecSkillUIBack[1]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[0].m_vSize;

			m_vecSkillUIBack[2]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[2]->m_fTransparent = 0.f;

			m_vecSkillUIBack[3]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillUIBack[3]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillUIBack[4]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillUIBack[4]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillUIBack[0]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[4].m_vSize;

			m_vecSkillCoolBack[1]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vSize = m_vecSkillCool[0].m_vSize;

			m_vecSkillCoolBack[2]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[3]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillCoolBack[3]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillCoolBack[4]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillCoolBack[4]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillCoolBack[0]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vSize = m_vecSkillCool[4].m_vSize;
		}
		else
		{
			m_fIconTime = 0.f;

			m_vecSkillUIBack[1]->m_vPos = m_vecSkillUI[0].m_vPos;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[0].m_vSize;
			m_vecSkillUIBack[1]->m_fTransparent = 1.f;

			m_vecSkillUIBack[2]->m_vPos = m_vecSkillUI[1].m_vPos;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[1].m_vSize;
			m_vecSkillUIBack[2]->m_fTransparent = 1.f;	

			m_vecSkillUIBack[3]->m_vPos = m_vecSkillUI[2].m_vPos;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[2].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 0.f;

			m_vecSkillUIBack[4]->m_vPos = m_vecSkillUI[3].m_vPos;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[3].m_vSize;
			m_vecSkillUIBack[4]->m_fTransparent = 0.f;


			m_vecSkillUIBack[0]->m_vPos = m_vecSkillUI[4].m_vPos;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[0]->m_fTransparent = 0.f;


			m_vecSkillUI[0] = *m_vecSkillUIBack[0];
			m_vecSkillUI[1] = *m_vecSkillUIBack[1];
			m_vecSkillUI[2] = *m_vecSkillUIBack[2];
			m_vecSkillUI[3] = *m_vecSkillUIBack[3];
			m_vecSkillUI[4] = *m_vecSkillUIBack[4];

			m_vecSkillCoolBack[1]->m_vPos = _vec3(m_vecSkillCool[0].m_vPos.x ,m_vecSkillCool[0].m_vOrizinPos.y - ((m_vecSkillCool[0].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[1]->m_fPer)), 0.f);
			m_vecSkillCoolBack[1]->m_vOrizinPos = m_vecSkillCool[0].m_vOrizinPos;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vOriginSize = m_vecSkillCool[0].m_vOriginSize;
			m_vecSkillCoolBack[1]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[2]->m_vPos = _vec3(m_vecSkillCool[1].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y - ((m_vecSkillCool[1].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[2]->m_fPer)), 0.f);
			m_vecSkillCoolBack[2]->m_vOrizinPos = m_vecSkillCool[1].m_vOrizinPos;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vOriginSize = m_vecSkillCool[1].m_vOriginSize;
			m_vecSkillCoolBack[2]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[3]->m_vPos = _vec3(m_vecSkillCool[2].m_vPos.x ,m_vecSkillCool[2].m_vOrizinPos.y - ((m_vecSkillCool[2].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[3]->m_fPer)), 0.f);
			m_vecSkillCoolBack[3]->m_vOrizinPos = m_vecSkillCool[2].m_vOrizinPos;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vOriginSize = m_vecSkillCool[2].m_vOriginSize;
			m_vecSkillCoolBack[3]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[4]->m_vPos = _vec3(m_vecSkillCool[3].m_vPos.x ,m_vecSkillCool[3].m_vOrizinPos.y - ((m_vecSkillCool[3].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[4]->m_fPer)), 0.f);
			m_vecSkillCoolBack[4]->m_vOrizinPos = m_vecSkillCool[3].m_vOrizinPos;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vOriginSize = m_vecSkillCool[3].m_vOriginSize;
			m_vecSkillCoolBack[4]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[0]->m_vPos = _vec3(m_vecSkillCool[4].m_vPos.x ,m_vecSkillCool[4].m_vOrizinPos.y - ((m_vecSkillCool[4].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[0]->m_fPer)), 0.f);
			m_vecSkillCoolBack[0]->m_vOrizinPos = m_vecSkillCool[4].m_vOrizinPos;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vOriginSize = m_vecSkillCool[4].m_vOriginSize;
			m_vecSkillCoolBack[0]->m_fTransparent = 0.f;

			m_vecSkillCool[0] = *m_vecSkillCoolBack[0];
			m_vecSkillCool[1] = *m_vecSkillCoolBack[1];
			m_vecSkillCool[2] = *m_vecSkillCoolBack[2];
			m_vecSkillCool[3] = *m_vecSkillCoolBack[3];
			m_vecSkillCool[4] = *m_vecSkillCoolBack[4];

			m_bMove = false;
			m_eChange = CHANGE_NULL;
		}
	}

	if(iTemp == 4)
	{
		if(m_fIconTime < 0.3f)
		{
			m_bMove = true;
			m_vecSkillUIBack[2]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[1].m_vSize;

			m_vecSkillUIBack[3]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[3]->m_fTransparent = 0.f;

			m_vecSkillUIBack[4]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillUIBack[4]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillUIBack[0]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillUIBack[0]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillUIBack[1]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[0].m_vSize;

			m_vecSkillCoolBack[2]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vSize = m_vecSkillCool[1].m_vSize;

			m_vecSkillCoolBack[3]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[4]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillCoolBack[4]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillCoolBack[0]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillCoolBack[0]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillCoolBack[1]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vSize = m_vecSkillCool[0].m_vSize;
		}
		else
		{
			m_fIconTime = 0.f;

			m_vecSkillUIBack[2]->m_vPos = m_vecSkillUI[1].m_vPos;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[1].m_vSize;
			m_vecSkillUIBack[2]->m_fTransparent = 1.f;

			m_vecSkillUIBack[3]->m_vPos = m_vecSkillUI[2].m_vPos;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[2].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 1.f;	

			m_vecSkillUIBack[4]->m_vPos = m_vecSkillUI[3].m_vPos;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[3].m_vSize;
			m_vecSkillUIBack[4]->m_fTransparent = 0.f;

			m_vecSkillUIBack[0]->m_vPos = m_vecSkillUI[4].m_vPos;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[0]->m_fTransparent = 0.f;


			m_vecSkillUIBack[1]->m_vPos = m_vecSkillUI[0].m_vPos;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[0].m_vSize;
			m_vecSkillUIBack[1]->m_fTransparent = 0.f;


			m_vecSkillUI[0] = *m_vecSkillUIBack[0];
			m_vecSkillUI[1] = *m_vecSkillUIBack[1];
			m_vecSkillUI[2] = *m_vecSkillUIBack[2];
			m_vecSkillUI[3] = *m_vecSkillUIBack[3];
			m_vecSkillUI[4] = *m_vecSkillUIBack[4];

			m_vecSkillCoolBack[2]->m_vPos = _vec3(m_vecSkillCool[1].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y - ((m_vecSkillCool[1].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[2]->m_fPer)), 0.f);
			m_vecSkillCoolBack[2]->m_vOrizinPos = m_vecSkillCool[1].m_vOrizinPos;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vOriginSize = m_vecSkillCool[1].m_vOriginSize;
			m_vecSkillCoolBack[2]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[3]->m_vPos = _vec3(m_vecSkillCool[2].m_vPos.x ,m_vecSkillCool[2].m_vOrizinPos.y - ((m_vecSkillCool[2].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[3]->m_fPer)), 0.f);
			m_vecSkillCoolBack[3]->m_vOrizinPos = m_vecSkillCool[2].m_vOrizinPos;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vOriginSize = m_vecSkillCool[2].m_vOriginSize;
			m_vecSkillCoolBack[3]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[4]->m_vPos = _vec3(m_vecSkillCool[3].m_vPos.x ,m_vecSkillCool[3].m_vOrizinPos.y - ((m_vecSkillCool[3].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[4]->m_fPer)), 0.f);
			m_vecSkillCoolBack[4]->m_vOrizinPos = m_vecSkillCool[3].m_vOrizinPos;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vOriginSize = m_vecSkillCool[3].m_vOriginSize;
			m_vecSkillCoolBack[4]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[0]->m_vPos = _vec3(m_vecSkillCool[4].m_vPos.x ,m_vecSkillCool[4].m_vOrizinPos.y - ((m_vecSkillCool[4].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[0]->m_fPer)), 0.f);
			m_vecSkillCoolBack[0]->m_vOrizinPos = m_vecSkillCool[4].m_vOrizinPos;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vOriginSize = m_vecSkillCool[4].m_vOriginSize;
			m_vecSkillCoolBack[0]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[1]->m_vPos = _vec3(m_vecSkillCool[0].m_vPos.x ,m_vecSkillCool[0].m_vOrizinPos.y - ((m_vecSkillCool[0].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[1]->m_fPer)), 0.f);
			m_vecSkillCoolBack[1]->m_vOrizinPos = m_vecSkillCool[0].m_vOrizinPos;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vOriginSize = m_vecSkillCool[0].m_vOriginSize;
			m_vecSkillCoolBack[1]->m_fTransparent = 0.f;

			m_vecSkillCool[0] = *m_vecSkillCoolBack[0];
			m_vecSkillCool[1] = *m_vecSkillCoolBack[1];
			m_vecSkillCool[2] = *m_vecSkillCoolBack[2];
			m_vecSkillCool[3] = *m_vecSkillCoolBack[3];
			m_vecSkillCool[4] = *m_vecSkillCoolBack[4];

			m_bMove = false;
			m_eChange = CHANGE_NULL;
		}
	}

	if(iTemp == 0)
	{
		if(m_fIconTime < 0.3f)
		{
			m_bMove = true;
			m_vecSkillUIBack[3]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[2].m_vSize;

			m_vecSkillUIBack[4]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[4]->m_fTransparent = 0.f;

			m_vecSkillUIBack[0]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillUIBack[0]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillUIBack[1]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillUIBack[1]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillUIBack[2]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[1].m_vSize;

			m_vecSkillCoolBack[3]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vSize = m_vecSkillCool[2].m_vSize;

			m_vecSkillCoolBack[4]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[0]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillCoolBack[0]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;

			m_vecSkillCoolBack[1]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillCoolBack[1]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;

			m_vecSkillCoolBack[2]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vSize = m_vecSkillCool[1].m_vSize;
		}
		else
		{
			m_fIconTime = 0.f;

			m_vecSkillUIBack[3]->m_vPos = m_vecSkillUI[2].m_vPos;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[2].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 1.f;

			m_vecSkillUIBack[4]->m_vPos = m_vecSkillUI[3].m_vPos;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[3].m_vSize;
			m_vecSkillUIBack[4]->m_fTransparent = 1.f;	

			m_vecSkillUIBack[0]->m_vPos = m_vecSkillUI[4].m_vPos;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[0]->m_fTransparent = 0.f;

			m_vecSkillUIBack[1]->m_vPos = m_vecSkillUI[0].m_vPos;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[0].m_vSize;
			m_vecSkillUIBack[1]->m_fTransparent = 0.f;


			m_vecSkillUIBack[2]->m_vPos = m_vecSkillUI[1].m_vPos;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[1].m_vSize;
			m_vecSkillUIBack[2]->m_fTransparent = 0.f;


			m_vecSkillUI[0] = *m_vecSkillUIBack[0];
			m_vecSkillUI[1] = *m_vecSkillUIBack[1];
			m_vecSkillUI[2] = *m_vecSkillUIBack[2];
			m_vecSkillUI[3] = *m_vecSkillUIBack[3];
			m_vecSkillUI[4] = *m_vecSkillUIBack[4];


			m_vecSkillCoolBack[3]->m_vPos = _vec3(m_vecSkillCool[2].m_vPos.x ,m_vecSkillCool[2].m_vOrizinPos.y - ((m_vecSkillCool[2].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[3]->m_fPer)), 0.f);
			m_vecSkillCoolBack[3]->m_vOrizinPos = m_vecSkillCool[2].m_vOrizinPos;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vOriginSize = m_vecSkillCool[2].m_vOriginSize;
			m_vecSkillCoolBack[3]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[4]->m_vPos = _vec3(m_vecSkillCool[3].m_vPos.x ,m_vecSkillCool[3].m_vOrizinPos.y - ((m_vecSkillCool[3].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[4]->m_fPer)), 0.f);
			m_vecSkillCoolBack[4]->m_vOrizinPos = m_vecSkillCool[3].m_vOrizinPos;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vOriginSize = m_vecSkillCool[3].m_vOriginSize;
			m_vecSkillCoolBack[4]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[0]->m_vPos = _vec3(m_vecSkillCool[4].m_vPos.x ,m_vecSkillCool[4].m_vOrizinPos.y - ((m_vecSkillCool[4].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[0]->m_fPer)), 0.f);
			m_vecSkillCoolBack[0]->m_vOrizinPos = m_vecSkillCool[4].m_vOrizinPos;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vOriginSize = m_vecSkillCool[4].m_vOriginSize;
			m_vecSkillCoolBack[0]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[1]->m_vPos = _vec3(m_vecSkillCool[0].m_vPos.x ,m_vecSkillCool[0].m_vOrizinPos.y - ((m_vecSkillCool[0].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[1]->m_fPer)), 0.f);
			m_vecSkillCoolBack[1]->m_vOrizinPos = m_vecSkillCool[0].m_vOrizinPos;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vOriginSize = m_vecSkillCool[0].m_vOriginSize;
			m_vecSkillCoolBack[1]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[2]->m_vPos = _vec3(m_vecSkillCool[1].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y - ((m_vecSkillCool[1].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[2]->m_fPer)), 0.f);
			m_vecSkillCoolBack[2]->m_vOrizinPos = m_vecSkillCool[1].m_vOrizinPos;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vOriginSize = m_vecSkillCool[1].m_vOriginSize;
			m_vecSkillCoolBack[2]->m_fTransparent = 0.f;

			m_vecSkillCool[0] = *m_vecSkillCoolBack[0];
			m_vecSkillCool[1] = *m_vecSkillCoolBack[1];
			m_vecSkillCool[2] = *m_vecSkillCoolBack[2];
			m_vecSkillCool[3] = *m_vecSkillCoolBack[3];
			m_vecSkillCool[4] = *m_vecSkillCoolBack[4];

			m_bMove = false;
			m_eChange = CHANGE_NULL;
		}
	}

	if(iTemp == 1)
	{
		if(m_fIconTime < 0.3f)
		{
			m_bMove = true;
			m_vecSkillUIBack[4]->m_vPos.y	   -= m_fTimeDelta * 140;
			m_vecSkillUIBack[4]->m_iRenderRank	= m_vecSkillUI[3].m_iRenderRank;

			m_vecSkillUIBack[0]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;

			m_vecSkillUIBack[1]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillUIBack[1]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f;

			m_vecSkillUIBack[2]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillUIBack[2]->m_vSize.y += m_fTimeDelta * fSizeControl  * 0.8f;

			m_vecSkillUIBack[3]->m_vPos.y	   -= m_fTimeDelta * 140;
			m_vecSkillUIBack[3]->m_iRenderRank	= m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize		= m_vecSkillUI[2].m_vSize;

			m_vecSkillCoolBack[4]->m_vPos.y	   -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[4]->m_iRenderRank	= m_vecSkillCool[3].m_iRenderRank;

			m_vecSkillCoolBack[0]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;

			m_vecSkillCoolBack[1]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillCoolBack[1]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f;

			m_vecSkillCoolBack[2]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillCoolBack[2]->m_vSize.y += m_fTimeDelta * fSizeControl  * 0.8f;

			m_vecSkillCoolBack[3]->m_vPos.y	   -= m_fTimeDelta * 140;
			m_vecSkillCoolBack[3]->m_iRenderRank	= m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vSize		= m_vecSkillCool[2].m_vSize;
		}
		else
		{
			m_fIconTime = 0.f;

			m_vecSkillUIBack[4]->m_vPos = m_vecSkillUI[3].m_vPos;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[3].m_vSize;
			m_vecSkillUIBack[4]->m_fTransparent = 1.f;

			m_vecSkillUIBack[3]->m_vPos = m_vecSkillUI[2].m_vPos;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[2].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 0.f;	

			m_vecSkillUIBack[2]->m_vPos = m_vecSkillUI[1].m_vPos;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize = m_vecSkillUI[1].m_vSize;
			m_vecSkillUIBack[2]->m_fTransparent = 0.f;

			m_vecSkillUIBack[1]->m_vPos = m_vecSkillUI[0].m_vPos;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[1]->m_vSize = m_vecSkillUI[0].m_vSize;
			m_vecSkillUIBack[1]->m_fTransparent = 0.f;

			m_vecSkillUIBack[0]->m_vPos = m_vecSkillUI[4].m_vPos;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[0]->m_fTransparent = 1.f;

			m_vecSkillUI[0] = *m_vecSkillUIBack[0];
			m_vecSkillUI[1] = *m_vecSkillUIBack[1];
			m_vecSkillUI[2] = *m_vecSkillUIBack[2];
			m_vecSkillUI[3] = *m_vecSkillUIBack[3];
			m_vecSkillUI[4] = *m_vecSkillUIBack[4];


			m_vecSkillCoolBack[4]->m_vPos = _vec3(m_vecSkillCool[3].m_vPos.x ,m_vecSkillCool[3].m_vOrizinPos.y - ((m_vecSkillCool[3].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[4]->m_fPer)), 0.f);
			m_vecSkillCoolBack[4]->m_vOrizinPos = m_vecSkillCool[3].m_vOrizinPos;
			m_vecSkillCoolBack[4]->m_iRenderRank = m_vecSkillCool[3].m_iRenderRank;
			m_vecSkillCoolBack[4]->m_vOriginSize = m_vecSkillCool[3].m_vOriginSize;
			m_vecSkillCoolBack[4]->m_fTransparent = 1.f;

			m_vecSkillCoolBack[3]->m_vPos = _vec3(m_vecSkillCool[2].m_vPos.x ,m_vecSkillCool[2].m_vOrizinPos.y - ((m_vecSkillCool[2].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[3]->m_fPer)), 0.f);
			m_vecSkillCoolBack[3]->m_vOrizinPos = m_vecSkillCool[2].m_vOrizinPos;
			m_vecSkillCoolBack[3]->m_iRenderRank = m_vecSkillCool[2].m_iRenderRank;
			m_vecSkillCoolBack[3]->m_vOriginSize = m_vecSkillCool[2].m_vOriginSize;
			m_vecSkillCoolBack[3]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[2]->m_vPos = _vec3(m_vecSkillCool[1].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y - ((m_vecSkillCool[1].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[2]->m_fPer)), 0.f);
			m_vecSkillCoolBack[2]->m_vOrizinPos = m_vecSkillCool[1].m_vOrizinPos;
			m_vecSkillCoolBack[2]->m_iRenderRank = m_vecSkillCool[1].m_iRenderRank;
			m_vecSkillCoolBack[2]->m_vOriginSize = m_vecSkillCool[1].m_vOriginSize;
			m_vecSkillCoolBack[2]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[1]->m_vPos = _vec3(m_vecSkillCool[0].m_vPos.x ,m_vecSkillCool[0].m_vOrizinPos.y - ((m_vecSkillCool[0].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[1]->m_fPer)), 0.f);
			m_vecSkillCoolBack[1]->m_vOrizinPos = m_vecSkillCool[0].m_vOrizinPos;
			m_vecSkillCoolBack[1]->m_iRenderRank = m_vecSkillCool[0].m_iRenderRank;
			m_vecSkillCoolBack[1]->m_vOriginSize = m_vecSkillCool[0].m_vOriginSize;
			m_vecSkillCoolBack[1]->m_fTransparent = 0.f;

			m_vecSkillCoolBack[0]->m_vPos = _vec3(m_vecSkillCool[4].m_vPos.x ,m_vecSkillCool[4].m_vOrizinPos.y - ((m_vecSkillCool[4].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[0]->m_fPer)), 0.f);
			m_vecSkillCoolBack[0]->m_vOrizinPos = m_vecSkillCool[4].m_vOrizinPos;
			m_vecSkillCoolBack[0]->m_iRenderRank = m_vecSkillCool[4].m_iRenderRank;
			m_vecSkillCoolBack[0]->m_vOriginSize = m_vecSkillCool[4].m_vOriginSize;
			m_vecSkillCoolBack[0]->m_fTransparent = 1.f;

			m_vecSkillCool[0] = *m_vecSkillCoolBack[0];
			m_vecSkillCool[1] = *m_vecSkillCoolBack[1];
			m_vecSkillCool[2] = *m_vecSkillCoolBack[2];
			m_vecSkillCool[3] = *m_vecSkillCoolBack[3];
			m_vecSkillCool[4] = *m_vecSkillCoolBack[4];

			m_bMove = false;
			m_eChange = CHANGE_NULL;
		}
	}
}

