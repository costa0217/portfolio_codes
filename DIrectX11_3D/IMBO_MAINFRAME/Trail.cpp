#include "stdafx.h"
#include "Trail.h"

CTrail::CTrail(XMVECTOR xmf4Color, int mode, float followT)
{
	ZeroMemory(&m_tTrailInfo, sizeof(TrailInfo));
	XMStoreFloat4(&m_tTrailInfo.m_xmf4Color, xmf4Color);
	m_iMode = mode;
	m_fFollowTime = followT;
}

CTrail::~CTrail()
{
	Release();
}
void CTrail::SetInitPos(XMVECTOR xmPos1, XMVECTOR xmPos2)
{
	for (int i = 0; i < 80; ++i)
	{
		if (i < 40)
		{
			XMStoreFloat4(&m_tTrailInfo.m_xmf4Vtx[i], xmPos1);
		}
		else
		{
			XMStoreFloat4(&m_tTrailInfo.m_xmf4Vtx[i], xmPos2);
		}
	}
}
void CTrail::SetTexName(CString name)
{
	_tcscpy_s(m_szTexture, _countof(m_szTexture), name);
}

HRESULT CTrail::Initialize()
{
	m_pEffectRenderCont = RENDERER->GetEffectRenderer();

	m_pCBuffer = CBuffer::CreateConstantBuffer(1, sizeof(TrailInfo), 5, BIND_VS | BIND_GS | BIND_PS, NULL);

	//XMStoreFloat4x4(&m_tTrailInfo.m_matWorld, XMMatrixIdentity());

	return S_OK;
}

int CTrail::Update(float fTimeElapsed)
{
	m_fTime = fTimeElapsed;
	if (m_pEffectRenderCont) {

		SetLerpPos();

		XMFLOAT3 xmf3NearPos;
		XMMATRIX matView, matProj;
		matView = CAMMGR->GetCamera(CAM_FREE)->GetViewMtx();
		matProj = CAMMGR->GetCamera(CAM_FREE)->GetProjectionMtx();

		XMStoreFloat3(&xmf3NearPos, XMVector3TransformCoord(XMLoadFloat3(&xmf3NearPos), matView * matProj));
		SwitchKnightSkill(fTimeElapsed);


		if (m_bRender)
			m_pEffectRenderCont->SetTrailRenderContainer(xmf3NearPos.z, this);
	}
	return 0;
}

void CTrail::Render()
{
	//CTexture* pTexture = RESOURCEMGR->GetTexture(TCHARToString(m_szTexture));
	CTexture* pTexture = RESOURCEMGR->GetTexture(TCHARToString(m_szTexture));
	pTexture->SetTextureSlot(5);
	//CResourceManager::GetInstance();

	pTexture->UpdateShaderState();
	pTexture->SetShaderState();
	SetParameter();

	GLOBALVALUEMGR->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	GLOBALVALUEMGR->GetDeviceContext()->Draw(1, 0);

	//ÇØÁ¦
	pTexture->CleanShaderState();
	m_pCBuffer->CleanShaderState();
}

void CTrail::Release()
{
}

void CTrail::SetParameter()
{
	TrailInfo* pdata = (TrailInfo*)m_pCBuffer->Map();

	for (int i = 0; i < 80; ++i)
	{
		pdata->m_xmf4Vtx[i] = m_tTrailInfo.m_xmf4Vtx[i];
	}
	pdata->m_xmf4Color = m_tTrailInfo.m_xmf4Color;

	m_pCBuffer->Unmap();
	m_pCBuffer->SetShaderState();
}

void CTrail::SetLerpPos()
{
	m_fAccTime += m_fTime;


	if (m_iMode == 0)
	{
		m_tTrailInfo.m_xmf4Vtx[0] = m_xmf4Pos1;
		m_tTrailInfo.m_xmf4Vtx[40] = m_xmf4Pos2;

		m_tTrailInfo.m_xmf4Vtx[9] = m_tTrailInfo.m_xmf4Vtx[0];
		m_tTrailInfo.m_xmf4Vtx[49] = m_tTrailInfo.m_xmf4Vtx[40];

		if (m_fAccTime > m_fFollowTime)
		{
			m_tTrailInfo.m_xmf4Vtx[39] = m_tTrailInfo.m_xmf4Vtx[29]		/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;
			m_tTrailInfo.m_xmf4Vtx[79] = m_tTrailInfo.m_xmf4Vtx[69]		/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;

			m_tTrailInfo.m_xmf4Vtx[29] = m_tTrailInfo.m_xmf4Vtx[19]		/*+ vDir * fDist*/;
			m_tTrailInfo.m_xmf4Vtx[69] = m_tTrailInfo.m_xmf4Vtx[59]		/*+ vDir * fDist*/;

			m_tTrailInfo.m_xmf4Vtx[19] = m_tTrailInfo.m_xmf4Vtx[9]	/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;
			m_tTrailInfo.m_xmf4Vtx[59] = m_tTrailInfo.m_xmf4Vtx[49]		/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;

			m_fAccTime = 0.f;
		}
	}
	else if (m_iMode == 1)
	{
		m_tTrailInfo.m_xmf4Vtx[9] = m_tTrailInfo.m_xmf4Vtx[0];
		m_tTrailInfo.m_xmf4Vtx[49] = m_tTrailInfo.m_xmf4Vtx[40];

		m_tTrailInfo.m_xmf4Vtx[0] = m_xmf4Pos1;
		m_tTrailInfo.m_xmf4Vtx[40] = m_xmf4Pos2;


		if (m_fAccTime > 0.05f)
		{
			m_tTrailInfo.m_xmf4Vtx[39] = m_tTrailInfo.m_xmf4Vtx[29]		/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;
			m_tTrailInfo.m_xmf4Vtx[79] = m_tTrailInfo.m_xmf4Vtx[69]		/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;

			m_tTrailInfo.m_xmf4Vtx[29] = m_tTrailInfo.m_xmf4Vtx[19]		/*+ vDir * fDist*/;
			m_tTrailInfo.m_xmf4Vtx[69] = m_tTrailInfo.m_xmf4Vtx[59]		/*+ vDir * fDist*/;

			m_tTrailInfo.m_xmf4Vtx[19] = m_tTrailInfo.m_xmf4Vtx[9]	/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;
			m_tTrailInfo.m_xmf4Vtx[59] = m_tTrailInfo.m_xmf4Vtx[49]		/*+ _vec3(vDir.x, vDir.x, vDir.z) * fDist*/;

			m_fAccTime = 0.f;
		}
	}


	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (0 == i)
			{
				if (0 == j)		continue;
				XMStoreFloat4(&m_tTrailInfo.m_xmf4Vtx[i * 10 + j],
					XMVectorCatmullRom(XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[0]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[0]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[9]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[19]),
						(float)(j) * 0.125f));

				XMStoreFloat4(&m_tTrailInfo.m_xmf4Vtx[i * 10 + j + 40],
					XMVectorCatmullRom(XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[9 + 40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[19 + 40]),
						(float)(j) * 0.125f));
			}
			else if (1 == i)
			{
				XMStoreFloat4(&m_tTrailInfo.m_xmf4Vtx[i * 10 + j],
					XMVectorCatmullRom(XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[0]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[9]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[19]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[29]),
						(float)(j) * 0.125f));

				XMStoreFloat4(&m_tTrailInfo.m_xmf4Vtx[i * 10 + j + 40],
					XMVectorCatmullRom(XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[0 + 40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[9 + 40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[19 + 40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[29 + 40]),
						(float)(j) * 0.125f));
			}
			else if (2 == i)
			{
				XMStoreFloat4(&m_tTrailInfo.m_xmf4Vtx[i * 10 + j],
					XMVectorCatmullRom(XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[9]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[19]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[29]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[39]),
						(float)(j) * 0.125f));

				XMStoreFloat4(&m_tTrailInfo.m_xmf4Vtx[i * 10 + j + 40],
					XMVectorCatmullRom(XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[9 + 40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[19 + 40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[29 + 40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[39 + 40]),
						(float)(j) * 0.125f));
			}
			else if (3 == i)
			{
				XMStoreFloat4(&m_tTrailInfo.m_xmf4Vtx[i * 10 + j],
					XMVectorCatmullRom(XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[19]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[29]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[39]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[39]),
						(float)(j) * 0.125f));

				XMStoreFloat4(&m_tTrailInfo.m_xmf4Vtx[i * 10 + j + 40],
					XMVectorCatmullRom(XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[19 + 40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[29 + 40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[39 + 40]),
						XMLoadFloat4(&m_tTrailInfo.m_xmf4Vtx[39 + 40]),
						(float)(j) * 0.125f));
			}
		}
	}
}

void CTrail::SwitchKnightSkill(float fTimeElapsed)
{
	if (m_bKnightSkill1)
	{
		m_fSkill1AccTime += fTimeElapsed;
		SetTexName(CString("Trail04"));
		XMStoreFloat4(&m_tTrailInfo.m_xmf4Color, XMVectorSet(1.f, 0.2f, 0.2f, 1.f));
		if (m_fSkill1AccTime > 10.f)
		{
			SetTexName(CString("Trail02"));
			XMStoreFloat4(&m_tTrailInfo.m_xmf4Color, XMVectorSet(0.8f, 0.5f, 0.4f, 1.f));
			m_bKnightSkill1 = false;
			m_fSkill1AccTime = 0.f;
		}
		m_fFireAccTime += fTimeElapsed;
		if (m_fFireAccTime > 0.001f)
		{
			float fa = float(rand() % 10 + 1);

			float fx = (m_xmf4Pos2.x - m_xmf4Pos1.x) / fa + m_xmf4Pos1.x + (m_xmf4Pos2.x - m_xmf4Pos1.x) / 3.f;
			float fy = (m_xmf4Pos2.y - m_xmf4Pos1.y) / fa + m_xmf4Pos1.y + (m_xmf4Pos2.y - m_xmf4Pos1.y) / 3.f;
			float fz = (m_xmf4Pos2.z - m_xmf4Pos1.z) / fa + m_xmf4Pos1.z + (m_xmf4Pos2.z - m_xmf4Pos1.z) / 3.f;

			int iRand = rand() % 3;
			switch (iRand) {
			case 0:
				CEffectMgr::GetInstance()->Play_Effect(L"Knight_sk1_fire", XMVectorSet(fx, fy, fz, 1.f),
					XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(0.5f, 0.5f, 0.f, 1.f));
				break;
			case 1:
				CEffectMgr::GetInstance()->Play_Effect(L"Knight_sk1_fire2", XMVectorSet(fx, fy, fz, 1.f),
					XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(0.5f, 0.5f, 0.f, 1.f));
				break;
			case 2:
				CEffectMgr::GetInstance()->Play_Effect(L"Knight_sk1_fire3", XMVectorSet(fx, fy, fz, 1.f),
					XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(0.5f, 0.5f, 0.f, 1.f));
				break;
			}
			m_fFireAccTime = 0.f;
		}
	}
}
