#include "stdafx.h"
#include "PlayerBeShotUI.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CPlayerBeShotUI)

CPlayerBeShotUI::CPlayerBeShotUI(void)
: Engine::CGameObject(Engine::Get_GraphicDev())
{
	Initialize();
	ZeroMemory(m_matOrtho, sizeof(_matrix));
	ZeroMemory(m_bSelect, sizeof(_bool) * 15);
	ZeroMemory(m_fAlpha, sizeof(_float) * 15);
	ZeroMemory(m_fTime, sizeof(_float) * 15);
	ZeroMemory(m_fMaxTime, sizeof(_float) * 15);
	ZeroMemory(m_fMaxSize, sizeof(_float) * 15);
	ZeroMemory(m_fSize, sizeof(_float) * 15);
	ZeroMemory(m_fSpeed, sizeof(_float) * 15);
}

CPlayerBeShotUI::~CPlayerBeShotUI(void)
{

}

HRESULT CPlayerBeShotUI::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default(AlphaTest3)");

	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	for(int i = 0; i < 15; ++i)
	{
		m_fSizeX[i]		= 256.f;
		m_fSizeY[i]		= 256.f;

		m_fX[i]			= 800.f;
		m_fY[i]			= 450.f;
		
		m_pVTXInfo[i]	= new Engine::VTXTEX[4];
		m_pBufferCom[i]->Get_VtxInfo(m_pVTXInfo[i]);
	}

	return S_OK;
}

HRESULT CPlayerBeShotUI::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom[0] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood0");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood0", pComponent));

	pComponent = m_pBufferCom[1] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood1");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood1", pComponent));

	pComponent = m_pBufferCom[2] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood2");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood2", pComponent));

	pComponent = m_pBufferCom[3] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood3");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood3", pComponent));

	pComponent = m_pBufferCom[4] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood4");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood4", pComponent));

	pComponent = m_pBufferCom[5] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood5");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood5", pComponent));

	pComponent = m_pBufferCom[6] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood6");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood6", pComponent));

	pComponent = m_pBufferCom[7] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood7");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood7", pComponent));

	pComponent = m_pBufferCom[8] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood8");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood8", pComponent));

	pComponent = m_pBufferCom[9] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood9");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood9", pComponent));

	pComponent = m_pBufferCom[10] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood10");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood10", pComponent));

	pComponent = m_pBufferCom[11] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood11");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood11", pComponent));

	pComponent = m_pBufferCom[12] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood12");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood12", pComponent));

	pComponent = m_pBufferCom[13] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood13");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood13", pComponent));

	pComponent = m_pBufferCom[14] = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Blood14");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Blood14", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Blood");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_Blood", pComponent));

	return S_OK;
}

_int CPlayerBeShotUI::Update(const _float& fTimeDelta)
{
	//���⼭ ���� ���Ŀ� ���� �����δ�.
	for(int i = 0; i < 15; ++i)
	{
		if(m_bSelect[i])
		{
			//����, �ߴ� 
			if(m_fTime[i] < m_fMaxTime[i])
				m_fTime[i] += fTimeDelta;
			else
				m_bSelect[i] = false;

			//������� �˰���
			//���� ũ�� �Ϸ��� �ߴ� ũ�� ��ŭ Ŀ���� �Ѵ�.
			
			if(m_fSize[i] != m_fMaxSize[i])	//ũ�Ⱑ �ٸ��� ũ�Ⱑ ������������ ������ �Ѵ�.
			{
				m_fSize[i]	+= (m_fMaxSize[i] - m_fSize[i]) * fTimeDelta * 10.f;

				m_fAlpha[i] += fTimeDelta;
				if(m_fAlpha[i] > 1.f)
					m_fAlpha[i] = 1.f;

				if(abs(m_fMaxSize[i] - m_fSize[i]) < 0.05)
				{
					m_fSize[i] = m_fMaxSize[i];
					m_fAlpha[i] = 1.f;
				}

				m_pVTXInfo[i][0].vTexUV.y = 0.2f - (0.2f * (m_fTime[i] / m_fMaxTime[i]));
				m_pVTXInfo[i][1].vTexUV.y = 0.2f - (0.2f * (m_fTime[i] / m_fMaxTime[i]));
				m_pVTXInfo[i][2].vTexUV.y = 1.0f - (0.2f * (m_fTime[i] / m_fMaxTime[i]));
				m_pVTXInfo[i][3].vTexUV.y = 1.0f - (0.2f * (m_fTime[i] / m_fMaxTime[i]));
			}
			else//ũ�Ⱑ �������� uv�� �帣�� �Ѵ�.
			{
				m_pVTXInfo[i][0].vTexUV.y = 0.2f - (0.2f * (m_fTime[i] / m_fMaxTime[i]));
				m_pVTXInfo[i][1].vTexUV.y = 0.2f - (0.2f * (m_fTime[i] / m_fMaxTime[i]));
				m_pVTXInfo[i][2].vTexUV.y = 1.0f - (0.2f * (m_fTime[i] / m_fMaxTime[i]));
				m_pVTXInfo[i][3].vTexUV.y = 1.0f - (0.2f * (m_fTime[i] / m_fMaxTime[i]));

				m_fY[i] += fTimeDelta*m_fSpeed[i];
			}

			//�ð��� �� ������ ���� ���� ���� ��Ų��.
			if(m_fTime[i] > (m_fMaxTime[i] - (m_fMaxTime[i] * 0.5f)))
			{
				m_fAlpha[i] = (1.25f - (m_fTime[i] / m_fMaxTime[i])); //�ִ� 0.25f
			}
			
			m_fSizeX[i] = 200.f * m_fSize[i];
			m_fSizeY[i] = 200.f * m_fSize[i];

			m_pBufferCom[i]->Set_VtxInfo(m_pVTXInfo[i]);
		}
	}

	//������ �����ϸ� �׶��� ��� �ұ� �׸��� ����?
	/*if(!CDynamicCamera::GetInstance()->GetModeCheck())*/
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);
	// ������������� ������ִ� ����� �Լ���.
	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);
	Engine::CGameObject::Update(fTimeDelta);
	return 0;
}

void CPlayerBeShotUI::Render(void)
{
	if(NULL == m_pEffect)
		return;

	for(int i = 0; i < 15; ++i)
	{
		if(m_bSelect[i])
		{
			m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);
			_matrix matView;
			D3DXMatrixIdentity(&matView);

			//������
			matView._11 = m_fSizeX[i];
			matView._22 = m_fSizeY[i];
			matView._33 = 1.f;

			//��ġ(800, 450 �� ��� �̴�) ���� ������ 0,0 ������ �Ʒ��� 1600, 900 
			//m_fx = 800, m_fy = 450 �̸� 0,0�� �Ǿ� ����� ����.
			//������ ���� ������ -800 ���� �������� 800 �̴� 0 ~ 1600
			//�������� ���� ������ 450 �Ʒ��� -450 �̴� 0 ~ 900
			matView._41 = m_fX[i] - WINSIZEX * 0.5f;
			matView._42 = -m_fY[i] + WINSIZEY * 0.5f;

			m_pEffect->SetMatrix("g_matView", &matView);
			m_pEffect->SetMatrix("g_matProj", &m_matOrtho);

			m_pEffect->SetFloat("g_fAlpha", m_fAlpha[i]);

			m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", i);

			m_pEffect->Begin(NULL, 0);

			m_pEffect->BeginPass(1);

			m_pBufferCom[i]->Render(NULL);

			m_pEffect->EndPass();

			m_pEffect->End();	

			m_pEffect->SetTexture("g_BaseTexture", NULL);
		}
	}
}

void CPlayerBeShotUI::PlayerBeShotUI(void)
{
	//15���� �ϳ��� �����Ų��.]
	_int iSelect = rand()%15;

	//���� �������� �ƴ϶�� �����Ѵ�.
	if(!m_bSelect[iSelect])
	{
		//���⼭ ������ ��ȣ�� ��ġ�� ������� �׸����� ������� �Ѵ�.
		m_bSelect[iSelect]	= true;
		m_fAlpha[iSelect]	= 0.f; 

		//ũ�� �ʱ�ȭ
		m_fSizeX[iSelect]	= 200.f;
		m_fSizeY[iSelect]	= 200.f;

		_int	iRandom		= rand()%150;
		_int	iRandom2	= rand()%50;
		m_fMaxSize[iSelect] = (1 + iRandom / 100.f);	//�ִ� 2.5
		m_fSize[iSelect]	= (iRandom2 + 50) / 100.f;	//�ִ� 1
		//�ӵ� �ʱ�ȭ
		m_fSpeed[iSelect]	= 2 + ((iRandom*2.f)/100.f);

		//�ð� �ʱ�ȭ
		_int	iRandom3	= rand()%150;
		m_fTime[iSelect]	= 0.f;
		m_fMaxTime[iSelect]	= 3.5f + (iRandom3/ 150.f);	//�ִ� 5��

		//��ġ �ʱ�ȭ ������ �ۼ��Ͽ��� �Ѵ�.
		_int	iSelectRandom	= rand()%3;

		_float	fX = 0.f;
		_float	fY = 0.f;

		switch(iSelectRandom)
		{
		case 0:
			fX = 200.f + rand()%300;
			fY = 200.f + rand()%550;
			break;
		case 1:
			fX = 500.f + rand()%700;
			fY = 600.f + rand()%150;
			break;
		case 2:
			fX = 1100.f + rand()%300;
			fY = 200.f + rand()%550;
			break;
		}

		m_fX[iSelect]		= fX;
		m_fY[iSelect]		= fY;

		//���İ� �ʱ�ȭ
		m_fAlpha[iSelect]	= 0.5f;
	}
	else
	{
		for(int i = 0; i < 15; ++i)
		{
			if(!m_bSelect[i])
			{
				//���⼭ ������ ��ȣ�� ��ġ�� ������� �׸����� ������� �Ѵ�.
				m_bSelect[i]	= true;
				m_fAlpha[i]	= 0.f; 

				//ũ�� �ʱ�ȭ
				m_fSizeX[i]	= 200.f;
				m_fSizeY[i]	= 200.f;

				_int	iRandom		= rand()%150;
				_int	iRandom2	= rand()%50;
				m_fMaxSize[i] = (1 + iRandom / 100.f);	//�ִ� 2.5
				m_fSize[i]	= (iRandom2 + 50) / 100.f;	//�ִ� 1
				//�ӵ� �ʱ�ȭ
				m_fSpeed[i]	= 2 + ((iRandom*2.f)/100.f);

				//�ð� �ʱ�ȭ
				_int	iRandom3	= rand()%150;
				m_fTime[i]	= 0.f;
				m_fMaxTime[i]	= 3.5f + (iRandom3/ 150.f);	//�ִ� 5��

				//��ġ �ʱ�ȭ ������ �ۼ��Ͽ��� �Ѵ�.
				_int	iSelectRandom	= rand()%3;

				_float	fX = 0.f;
				_float	fY = 0.f;

				switch(iSelectRandom)
				{
				case 0:
					fX = 200.f + rand()%300;
					fY = 200.f + rand()%550;
					break;
				case 1:
					fX = 500.f + rand()%700;
					fY = 600.f + rand()%150;
					break;
				case 2:
					fX = 1100.f + rand()%300;
					fY = 200.f + rand()%550;
					break;
				}

				m_fX[i]		= fX;
				m_fY[i]		= fY;

				//���İ� �ʱ�ȭ
				m_fAlpha[i]	= 0.5f;
				break;
			}
		}
	}
}

_ulong CPlayerBeShotUI::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		for(int i = 0; i < 15; ++i)
		{
			::Safe_Delete_Array(m_pVTXInfo[i]);
		}
		delete this;
	}

	return dwRefCnt;
}