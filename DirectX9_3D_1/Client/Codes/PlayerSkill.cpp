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

	// 스킬창 이미지의 크기를 알기 위한 과정 
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSkillUseValueBalance = 3.f;

	const TCHAR*		wstrImagePath;

	wstrImagePath		= L"../Bin/Resources/Texture/StageScene/UIResource/Player/Player_Skill0.png";

	CImage*		pPngImage = new CImage;		
	pPngImage->Load(wstrImagePath);

	for(int j = 0; j < 2; ++j)		// 쿨타임 스킬창과 그냥 스킬창을 넣기 위해 2번 반복 
	{
		for(int i = 0; i < SL_End; ++i)		// 스킬 구조체 설정, 스킬 숫자만큼 for문을 돌림
		{
			m_fTempYPos[i] = 0.f;

			SKILLUI*	pSkill = new SKILLUI;

			pSkill->m_eName = (SKILL_LIST)i;
			pSkill->m_vSize			= _vec2((float)pPngImage->GetWidth(), (float)pPngImage->GetHeight());	// 이미지 크기 저장
			pSkill->m_vOriginSize	= _vec2((float)pPngImage->GetWidth(), (float)pPngImage->GetHeight());
			pSkill->m_vStartUV		= _vec2(0.2f * i, 0.f);
			pSkill->m_vLastUV		= _vec2(0.2f * i + 0.2f, 1.f);	
			pSkill->m_pBufferCom	= m_pBufferCom[i];

			if(pSkill->m_eName == SL_Judgement)		// 스킬 이름에 따라 초기값이 조금씩 달라짐, 
				// 스킬의 갯수를 5개가 확정인걸 가정하고 하는 초기화
			{
				pSkill->m_fTransparent	 = 1.f;		// 투명도
				pSkill->m_iRenderRank	 = 5;		//랜더 우선순위
				pSkill->m_vSize			*= 0.75f;	
				pSkill->m_vOriginSize	*= 0.75f;
				pSkill->m_vPos			 = _vec3(WINSIZEX / (2.f ), WINSIZEY / (2.f - 0.3f), 0.f);	// 스킬이 변할수록 초기 Y값을 다르게함
				pSkill->m_vOrizinPos	 = _vec3(WINSIZEX / (2.f ), WINSIZEY / (2.f - 0.3f), 0.f);
				pSkill->m_fCoolTime		 = 30.f / m_fSkillUseValueBalance;		// 쿨타임 길이를 스킬마다 다르게 넣음 
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

				pSkill->m_iRenderRank		= 1;			// 세번째가 중앙, m_iRenderRank가 낮을 수록 가장 나중에 랜더가 됨.

				pSkill->m_vSize				*= 1.25f;		// 세번째는 가운데 스킬이기 때문에 초기 사이즈 값을 다른것보다 크게 만듬 
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

	Safe_Delete(pPngImage);		// 구조체 초기화를 끝낸 후 CImage*를 지운다.

	for(int i = 0; i < SL_End; ++i)
	{
		m_vecSkillUIBack[i]->m_pVertex	= new Engine::VTXTEX[4];		// 버텍스 초기화
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

	for(int i = 0; iterSkill != iterSkillEnd; ++iterCoolBack ,++iterSkill, ++i)		// UV 초기화
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
	//// Font 관련
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

	// Font 관련
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

	if(Engine::GetDIKeyState(DIK_F) & 0x80 && 0.2f < m_fOverlapTime  && !(Engine::GetDIKeyState(DIK_Z) & 0x80))		// 시간값을 이용해 중복키 체크 
	{
		m_fOverlapTime = 0.f;		// 키가 눌려졌을 경우 중복키 체크용 시간을 초기화

		m_eChange	= CHANGE_DOWN;	// 키가 눌렸을때 스킬의 상태값을 바꿈 
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

	// 심판 초기화
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

	// 맨 아래 있는 벡터는 위로 올라가고 
	// 그게 아닌 녀석들은 아래로 내린다.
	if(m_eChange == CHANGE_DOWN)
	{
		m_fIconTime += m_fTimeDelta;
		int	iTemp = 0;
		for(int i = 0; i < SL_End ; ++i)
		{
			if(m_vecSkillUI[i].m_iRenderRank == 1)		// m_iRenderRank가 1일때는 중앙
				m_iCenterNum = i;						// m_iRenderRank가 1일때에 해당하는 스킬창을 찾아줌 
		}
		SkillDownChange(m_iCenterNum);		// m_iRenderRank로 찾은 스킬값을 중심으로 스킬창 위치 체인지 
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

	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);		// 직교 매트릭스 생성
	
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
		if(m_vecSkillUIBack[i]->m_iRenderRank == 5)		// 랜더랭크가 높을수록 먼저 랜더 
		{
			Set_ConTableSkillUi(i);
			m_pEffect->CommitChanges();
			m_vecSkillUIBack[i]->m_pBufferCom->Render(NULL);

			if(m_vecSkillCool[i].m_fCoolTime >= 0)		// 쿨타임 시간이 0보다 클때
			{
				if(m_vecSkillCoolBack[i]->m_bCoolTime == true)		// 쿨타임 On이면 쿨타임을 랜더하는 동시에 
				{													// 쿨타임 관련 함수를 돌린다.
					CoolDown(i);
					Set_ConTableCool(i);
					m_pEffect->CommitChanges();
					m_pBufferCoolCom[i]->Render(NULL);
				}
			}
			else		// 쿨타임이 0보다 작을때 
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

			if(m_vecSkillCoolBack[i]->m_fCoolTime >= 0)		// 쿨타임이 0 이하가 될때까지 쿨타임창을 돌림 
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
			else		// 쿨타임이 0 이하로 갈시 
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
	m_vecSkillUIBack[iSkillIndex]->m_matView._11 = m_vecSkillUIBack[iSkillIndex]->m_vSize.x * 0.2f;	// UV로 잘랐기 때문에 사이즈도 그만큼 줄여준다.
	m_vecSkillUIBack[iSkillIndex]->m_matView._22 = m_vecSkillUIBack[iSkillIndex]->m_vSize.y;	// 스킬이 한줄이기 때문에 Y값을 줄일필요는 없음 
	m_vecSkillUIBack[iSkillIndex]->m_matView._33 = 1.f;

	m_vecSkillUIBack[iSkillIndex]->m_matView._41 = m_vecSkillUIBack[iSkillIndex]->m_vPos.x - WINSIZEX * 0.5f + 500.f + (200.f * (1.f - CDynamicCamera::GetInstance()->GetCurrentDistance() / 25.f));
	m_vecSkillUIBack[iSkillIndex]->m_matView._42 = m_vecSkillUIBack[iSkillIndex]->m_vPos.y - WINSIZEY * 0.5f;

	m_pEffect->SetMatrix("g_matView", &m_vecSkillUIBack[iSkillIndex]->m_matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

	// m_fTransparent 변수에 따라 색깔을 변화시킴 . (주로 투명으로 만들기 위함)
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

		//// 왼쪽 위
		//m_pVertexFont[SF_COOLTIME][0].vTexUV.x = m_vStartUVFont[SF_COOLTIME].x;
		//m_pVertexFont[SF_COOLTIME][0].vTexUV.y = m_vStartUVFont[SF_COOLTIME].y;

		//// 오른쪽 위
		//m_pVertexFont[SF_COOLTIME][1].vTexUV.x = m_vLastUVFont[SF_COOLTIME].x;
		//m_pVertexFont[SF_COOLTIME][1].vTexUV.y = m_vStartUVFont[SF_COOLTIME].y;

		//// 오른쪽 아래
		//m_pVertexFont[SF_COOLTIME][2].vTexUV.x = m_vLastUVFont[SF_COOLTIME].x;
		//m_pVertexFont[SF_COOLTIME][2].vTexUV.y = m_vLastUVFont[SF_COOLTIME].y;

		//// 오른쪽 위
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

		//// 왼쪽 위
		//m_pVertexFont[SF_SP][0].vTexUV.x = m_vStartUVFont[SF_SP].x;
		//m_pVertexFont[SF_SP][0].vTexUV.y = m_vStartUVFont[SF_SP].y;

		//// 오른쪽 위
		//m_pVertexFont[SF_SP][1].vTexUV.x = m_vLastUVFont[SF_SP].x;
		//m_pVertexFont[SF_SP][1].vTexUV.y = m_vStartUVFont[SF_SP].y;

		//// 오른쪽 아래
		//m_pVertexFont[SF_SP][2].vTexUV.x = m_vLastUVFont[SF_SP].x;
		//m_pVertexFont[SF_SP][2].vTexUV.y = m_vLastUVFont[SF_SP].y;

		//// 오른쪽 위
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
	m_matViewFont[iFontIndex]._42 = m_vPosFont[iFontIndex].y - WINSIZEY * 0.5f - 15.f; //일단 임시로 해서 내렸어 성원아

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
	if(Engine::GetDIKeyState(DIK_Z) & 0x80 && !m_bButtonDown)		// 시간값을 이용해 중복키 체크 
	{
		for(int i = 0; i < SL_End; ++i)
		{
			if(m_vecSkillCoolBack[i]->m_iRenderRank == 1)
			{
				m_bButtonDown = true;		// 딸각이용 버튼
				m_iCenterNum = i;			// 딸각할때를 중앙 스킬이 뭔지를 알기위해 내가 누른 스킬값 저장
				if(m_vecSkillCoolBack[i]->m_bCoolTime == true)	// 이미 쿨타임이 돌고 있을때
				{
					// 특정 문구 나오게 
					m_bFailFont[SF_COOLTIME] = true;

					if(m_eFirstFont == SF_OFF)
					{
						m_eFirstFont = SF_COOLTIME;
						CSkillFailFont::GetInstance()->m_bRender = true;
						CSkillFailFont::GetInstance()->m_iTextureNumber = 0;
					}

					return false;
				}
				else if(m_vecSkillCoolBack[i]->m_fNeedSp > CPlayer::GetInstance()->GetPlayerSP()) // SP가 부족할때
				{
					// 특정 문구 나오게 
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
						m_vecSkillCoolBack[i]->m_bCoolTime = true;		// 쿨타임창 돌리기 시작 (백업)
						m_vecSkillCool[i].m_bCoolTime = true;			// 쿨타임창 돌리기 시작
					}
					else
					{
						//CPlayer::GetInstance()->PlayerSkill_Input(m_iCenterNum) 가 true을 반환하면 스킬을 실행 했다는 뜻이다.
						
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

	//Judgement_l의 모션중 Judgement_r이 아닌 다른 모션으로 바뀌거나
	//JUdgement_r의 애니메이션이 실행이 되고 난후 다른 모션으로 바뀌면 쿨타임을 돈다.
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

void CPlayerSkill::CoolDown(int i)		// HP 깎듯이 함수를 만듬 
{
	m_vecSkillCoolBack[i]->m_fCoolTime -= m_fTimeDelta;		

	m_vecSkillCoolBack[i]->m_fPer = (m_vecSkillCoolBack[i]->m_fCoolTime / m_vecSkillCoolBack[i]->m_fCoolTimeBack); // 남은 쿨타임 비율
	m_vecSkillCoolBack[i]->m_pVertex[2].vTexUV.y = m_vecSkillCoolBack[i]->m_fPer;
	m_vecSkillCoolBack[i]->m_pVertex[3].vTexUV.y = m_vecSkillCoolBack[i]->m_fPer;

	m_vecSkillCool[i].m_fPer = (m_vecSkillCool[i].m_fCoolTime / m_vecSkillCool[i].m_fCoolTimeBack);

	if(m_bMove == false)	// 쿨타임이 돌때 위치값도 수시로 바꿔줘야 되는데 쿨타임이 도는 동안 
		// 버튼 자체가 위 혹은 아래로 가면 쿨타임창의 위치가 이상해 져서 그걸 최대한 막기위해 
		// 버튼이 이동하는 동안에는 쿨타임창의 위치 이동을 막기로 함
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

// 처음에는 스킬 아이콘들을 0.3초간 이동시키다 시간이 지나면

// 백업용 벡터				0 1 2 3 4 에 
// 값을 바꾼 보통 벡터		1 2 3 4 0 을 집어넣는다.
// 여기서 백업용 벡터의 값을 가지고 랜더하는 방식인데 
// 함수를 끝내기 전에 보통 벡터 0 1 2 3 4에 
// 백업용 벡터					0 1 2 3 4를 집어넣어 보통 벡터의 값도 조정해준다.

void CPlayerSkill::SelectMove()
{
	if(m_bButtonDown == true)
	{
		m_fButtonTime += m_fTimeDelta;
		m_fMoveSillUI += 150 * m_fTimeDelta;
		if(m_fMoveSillUI > 10.f)
			m_fMoveSillUI = 10.f;
		//시간으로 해서 누르는동안 선형보간같이 한번에 치대 값까지만 이동 할수 있게 한다.
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
	_float fSizeControl = 80.f; // 사이즈 컨트롤을 한번에 하기 위해 임시로 만든 변수

	if(iTemp == 2)	// 스킬 중앙값에 해당하는 스킬값이 2일때
	{
		if(m_fIconTime < 0.3f)		// 아이콘(스킬창)이 이동하는 시간 
		{
			m_bMove = true;

			// 보통 아이콘창을 바꿀때
			m_vecSkillUIBack[0]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[0]->m_iRenderRank = m_vecSkillUI[1].m_iRenderRank;
			m_vecSkillUIBack[0]->m_vSize = m_vecSkillUI[1].m_vSize;

			m_vecSkillUIBack[1]->m_vPos.y -= m_fTimeDelta * 140;
			m_vecSkillUIBack[1]->m_iRenderRank = m_vecSkillUI[2].m_iRenderRank;		
			m_vecSkillUIBack[1]->m_vSize.x += m_fTimeDelta * fSizeControl * 4.f;
			m_vecSkillUIBack[1]->m_vSize.y += m_fTimeDelta * fSizeControl * 0.8f;	// 중앙으로 가는 스킬이기 때문에 크기를 점점 키움

			m_vecSkillUIBack[2]->m_vPos.y  -= m_fTimeDelta * 140;
			m_vecSkillUIBack[2]->m_iRenderRank = m_vecSkillUI[3].m_iRenderRank;
			m_vecSkillUIBack[2]->m_vSize.x -= m_fTimeDelta * fSizeControl * 4.0f;
			m_vecSkillUIBack[2]->m_vSize.y -= m_fTimeDelta * fSizeControl / 1.3f ;	// 중앙에서 물러나는 스킬이기 때문에 크기를 점점 줄임

			m_vecSkillUIBack[3]->m_vPos.y  += m_fTimeDelta * 140;
			m_vecSkillUIBack[3]->m_iRenderRank = m_vecSkillUI[4].m_iRenderRank;
			m_vecSkillUIBack[3]->m_vSize = m_vecSkillUI[4].m_vSize;
			m_vecSkillUIBack[3]->m_fTransparent = 0.f;

			m_vecSkillUIBack[4]->m_vPos.y += m_fTimeDelta * 140;
			m_vecSkillUIBack[4]->m_iRenderRank = m_vecSkillUI[0].m_iRenderRank;
			m_vecSkillUIBack[4]->m_vSize = m_vecSkillUI[0].m_vSize;

			// 스킬 쿨타임창을 바꿀때
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
		else		// 아이콘 스킬이 이동을 마치고 한칸 아래의 인덱스값을 적용하는 과정
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

			m_vecSkillUI[0] = *m_vecSkillUIBack[0];		// 백업벡터를 이용해 보통 스킬창 벡터도 바꾼다.
			m_vecSkillUI[1] = *m_vecSkillUIBack[1];
			m_vecSkillUI[2] = *m_vecSkillUIBack[2];
			m_vecSkillUI[3] = *m_vecSkillUIBack[3];
			m_vecSkillUI[4] = *m_vecSkillUIBack[4];


			// 랜더 랭크의 위치에 값이 그대로여야 하는것만 바꿔줘야 한다.

			m_vecSkillCoolBack[0]->m_vPos = _vec3(m_vecSkillCool[1].m_vPos.x ,m_vecSkillCool[1].m_vOrizinPos.y - ((m_vecSkillCool[1].m_vOriginSize.y / 2.f) * (1.f - m_vecSkillCoolBack[0]->m_fPer)), 0.f);
			// 기존 스킬값의 위치값에 쿨타임을 적용시킨 위치값을 함께 적용 시키면서 계산해준다.
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

			m_eChange = CHANGE_NULL;		// 값 조정이 끝나면 상태 enum값을 바꿔서 이 함수로 다시 들어오지 못하게 함 .
			m_bMove = false;				// 쿨타임 아이콘이 이동할때 위치값이 꼬이는걸 방지하기 위한 변수 
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

