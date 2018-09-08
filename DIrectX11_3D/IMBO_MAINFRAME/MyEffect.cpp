#include "stdafx.h"
#include "MyEffect.h"
#include "EffectRenderCont.h"

bool Compare_Time(POINTLERP* Dest, POINTLERP* Sour)
{
	return Dest->fTimePoint < Sour->fTimePoint;
}

CMyEffect::CMyEffect()
{
}
CMyEffect::~CMyEffect()
{
	Release();
}

HRESULT CMyEffect::Initialize()
{
	m_pEffectRenderCont = RENDERER->GetEffectRenderer();
	//m_pMesh = RESOURCEMGR->GetMesh("Effect").get();
	m_pCBuffer = CBuffer::CreateConstantBuffer(1, sizeof(EffectInfo), 5, BIND_VS | BIND_GS | BIND_PS, NULL);

	ZeroMemory(&m_tEffectInfo, sizeof(EffectInfo));
	XMStoreFloat4x4(&m_tEffectInfo.m_matWorld, XMMatrixIdentity());


	POINTLERP* pLinear = NULL;
	pLinear = new POINTLERP;
	pLinear->fTimePoint = 0.f;
	XMStoreFloat3(&pLinear->xmf3PosPoint, XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 0.f)));
	XMStoreFloat3(&pLinear->xmf3AnglePoint, XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 0.f)));
	XMStoreFloat3(&pLinear->xmf3ScalePoint, XMLoadFloat3(&XMFLOAT3(1.f, 1.f, 1.f)));
	XMStoreFloat4(&pLinear->xmf4ColorPoint, XMLoadFloat4(&m_f4Color));
	m_PointList.push_back(pLinear);

	pLinear = new POINTLERP;
	pLinear->fTimePoint = 1.f;
	XMStoreFloat3(&pLinear->xmf3PosPoint, XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 0.f)));
	XMStoreFloat3(&pLinear->xmf3AnglePoint, XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 0.f)));
	XMStoreFloat3(&pLinear->xmf3ScalePoint, XMLoadFloat3(&XMFLOAT3(1.f, 1.f, 1.f)));
	XMStoreFloat4(&pLinear->xmf4ColorPoint, XMLoadFloat4(&m_f4Color));
	m_PointList.push_back(pLinear);

	return S_OK;
}
int CMyEffect::Update(float fTimeElapsed)
{
	if (false == m_bAlive) {
		return -1 ;
	}
	m_fAccTime += fTimeElapsed;
	if (m_fAccTime > m_fEffectEndTime)
	{
		InitData();
		m_bAlive = false;
		return -1 ;
	}

	if (true == m_bPlayEffect || true == m_bRepeat){
		PlayEffectAnimation(fTimeElapsed);
	}
	
	UpdateFrameAnimation(fTimeElapsed);
	MakeQuaternion();	// 이동, 회전, 스케일링 = 월드구하기
	XMStoreFloat4x4(&m_tEffectInfo.m_matWorld, XMMatrixAffineTransformation(XMLoadFloat4(&m_f4Scale),
		XMQuaternionIdentity(),
		XMLoadFloat4(&m_f4Quaternion),
		XMLoadFloat3(&m_f3Pos)));

	if (m_szTexture == CString("Parents"))
	{
		if (m_pmParent)
		{
			
			XMMATRIX matPos = XMMatrixTranslation(m_pmParent->GetPositionX(), m_pmParent->GetPositionY() + m_pmParent->GetBBox()->Extents.y, m_pmParent->GetPositionZ());
			XMStoreFloat4x4(&m_tEffectInfo.m_matWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_tEffectInfo.m_matWorld),
				matPos));
		}

		return -1;
	}


	//XMStoreFloat4x4(&matResult, XMMatrixRotationQuaternion(XMLoadFloat4(&m_f4Quaternion)));

	if (m_bBillboard == true){
		AdjustBillboard();
	}
	

	XMStoreFloat4x4(&m_tEffectInfo.m_matWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_tEffectInfo.m_matWorld),
		XMLoadFloat4x4(&m_pParent->m_tEffectInfo.m_matWorld)));


	// 뷰변환 행렬의 z값 얻기

	if (m_pEffectRenderCont) {
		XMMATRIX matView;
		matView = CAMMGR->GetCamera(CAM_FREE)->GetViewMtx();

		XMFLOAT3		vEdge[4];
		ZeroMemory(vEdge, sizeof(XMFLOAT3) * 4);
		XMStoreFloat3(&vEdge[0], XMVector3TransformCoord(XMLoadFloat3(&XMFLOAT3(-1.f, -1.f, 0.f)),
														XMMatrixMultiply(XMLoadFloat4x4(&m_tEffectInfo.m_matWorld), matView)));
		XMStoreFloat3(&vEdge[1], XMVector3TransformCoord(XMLoadFloat3(&XMFLOAT3(-1.f, 1.f, 0.f)),
														XMMatrixMultiply(XMLoadFloat4x4(&m_tEffectInfo.m_matWorld), matView)));
		XMStoreFloat3(&vEdge[2], XMVector3TransformCoord(XMLoadFloat3(&XMFLOAT3(1.f, -1.f, 0.f)),
														XMMatrixMultiply(XMLoadFloat4x4(&m_tEffectInfo.m_matWorld), matView)));
		XMStoreFloat3(&vEdge[3], XMVector3TransformCoord(XMLoadFloat3(&XMFLOAT3(1.f, 1.f, 0.f)),
														XMMatrixMultiply(XMLoadFloat4x4(&m_tEffectInfo.m_matWorld), matView)));

		float fNearDist = 1000;
		for (int i = 0; i < 4; ++i)	{
			if (fNearDist > vEdge[i].z){
				fNearDist = vEdge[i].z;
			}
		}
		m_pEffectRenderCont->SetRenderContainer(fNearDist, this, m_bDistortion);
	}
	return 0;
}
void CMyEffect::Render()
{
	if (m_szTexture == CString("Parents")) return;
	if (false == m_bRender)		return;
	if (true == m_bEffectDie)		return;
	

	 CTexture* pTexture = RESOURCEMGR->GetTexture(TCHARToString(m_szTexture));
	pTexture->SetTextureSlot(5);
	//CResourceManager::GetInstance();

	pTexture->UpdateShaderState();
	pTexture->SetShaderState();
	//ID3D11ShaderResourceView* pSRV[] = { pTexture->GetShaderResourceView() };
	//GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(5, 1, pSRV);

	//m_pCBuffer->UpdateShaderState();
	SetParameter();
	// Mesh 그리기

	//m_pMesh->Render(1);
	GLOBALVALUEMGR->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	GLOBALVALUEMGR->GetDeviceContext()->Draw(1, 0);

	//해제
	pTexture->CleanShaderState();
	m_pCBuffer->CleanShaderState();
}

void CMyEffect::Release()
{
	list<POINTLERP*>::iterator iter = m_PointList.begin();
	list<POINTLERP*>::iterator iter_end = m_PointList.end();
	for (; iter != iter_end; ++iter) {
		if (*iter) {
			delete (*iter);
			(*iter) = nullptr;
		}
	}
	m_PointList.clear();

	if (m_pCBuffer) {
		m_pCBuffer->End();
		delete m_pCBuffer;
	}
	m_pCBuffer = nullptr;
}

void CMyEffect::SetTexName(CString name)
{
	_tcscpy_s(m_szTexture, _countof(m_szTexture), name);
}

void CMyEffect::SortingPoints()
{
	m_PointList.sort(Compare_Time);
}

void CMyEffect::SetParameter()
{
	// Set할거 set하기
	EffectInfo* pdata = (EffectInfo*)m_pCBuffer->Map();
	pdata->m_iU = m_tEffectInfo.m_iU;
	pdata->m_iV = m_tEffectInfo.m_iV;
	pdata->m_fFrameSpd = m_tEffectInfo.m_fFrameSpd;
	pdata->m_fFrameCnt = m_tEffectInfo.m_fFrameCnt;
	pdata->m_iFrameHeight = m_tEffectInfo.m_iFrameHeight;
	pdata->noUse1 = 0;
	pdata->noUse2 = 0;
	pdata->m_xmColor = XMFLOAT4(m_f4Color.x / 255.f, m_f4Color.y / 255.f, m_f4Color.z / 255.f, m_f4Color.w / 255.f);
	pdata->m_iTexOpt = m_tEffectInfo.m_iTexOpt;
	XMStoreFloat4x4(&pdata->m_matWorld, XMMatrixTranspose(XMLoadFloat4x4(&m_tEffectInfo.m_matWorld)));

	m_pCBuffer->Unmap();
	m_pCBuffer->SetShaderState();
}

void CMyEffect::MakeQuaternion()
{
	XMStoreFloat4(&m_f4Quaternion, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(m_f3Angle.x),
		XMConvertToRadians(m_f3Angle.y),
		XMConvertToRadians(m_f3Angle.z)));
}

void CMyEffect::StopEffectAniamtion()
{
	/*m_bPlayEffect = false;
	m_bEffectDie = false;
	m_bRender = true;
	m_fAccPlayTime = 0.f;
	m_iAniCnt = -1;*/

	m_bPlayEffect = true;
	m_bEffectDie = false;
	m_bRender = true;
	m_fAccPlayTime = 0.f;
	m_iAniCnt = -1;

	m_f3Pos = m_PointList.front()->xmf3PosPoint;
	m_f3Angle = m_PointList.front()->xmf3AnglePoint;
	m_f4Scale.x = m_PointList.front()->xmf3ScalePoint.x;
	m_f4Scale.y = m_PointList.front()->xmf3ScalePoint.y;
	m_f4Scale.z = m_PointList.front()->xmf3ScalePoint.z;
	m_f4Color = m_PointList.front()->xmf4ColorPoint;
}

void CMyEffect::Copy_Point(CMyEffect * pCopy)
{
	Delete_PointAll();

	list<POINTLERP*>::iterator Pointiter = pCopy->GetVectorLinear()->begin();
	list<POINTLERP*>::iterator Pointiter_end = pCopy->GetVectorLinear()->end();
	for (; Pointiter != Pointiter_end; ++Pointiter)
	{
		POINTLERP* pPointLinear = new POINTLERP;
		memcpy(pPointLinear, *Pointiter, sizeof(POINTLERP));
		m_PointList.push_back(pPointLinear);
	}
}

void CMyEffect::Delete_PointAll()
{
	list<POINTLERP*>::iterator iter = m_PointList.begin();
	list<POINTLERP*>::iterator iter_end = m_PointList.end();
	for (; iter != iter_end; ++iter)
	{
		delete (*iter);
		(*iter) = nullptr;
	}
	m_PointList.clear();
}

void CMyEffect::Modify_Point(XMVECTOR xmPos, XMVECTOR xmRot, XMVECTOR xmScl)
{
	list<POINTLERP*>::iterator iter = m_PointList.begin();
	list<POINTLERP*>::iterator iter_end = m_PointList.end();
	for (; iter != iter_end; ++iter)
	{
		XMStoreFloat3(&(*iter)->xmf3PosPoint, XMVectorAdd(XMLoadFloat3(&(*iter)->xmf3PosPoint), xmPos));
		XMStoreFloat3(&(*iter)->xmf3AnglePoint, XMVectorAdd(XMLoadFloat3(&(*iter)->xmf3AnglePoint), xmRot));
	}
}

void CMyEffect::Modify_Point(CGameObject * pParent)
{
	m_pmParent = pParent;
}

void CMyEffect::InitData()
{
	Copy_Point(m_pOriEffect);
	StopEffectAniamtion();
	m_pmParent = nullptr;
}

void CMyEffect::PlayEffectAnimation(float fDeltaTime)
{
	m_fAccPlayTime += fDeltaTime;
	if (-1 == m_iAniCnt)
	{
		m_bRender = false;
		list<POINTLERP*>::iterator iter = m_PointList.begin();
		if (m_fAccPlayTime > (*iter)->fTimePoint)	// 처음 시간까지의 흐름
		{
			m_bRender = true;
			m_iAniCnt = 0;
			m_fAccPlayTime = 0.f;
			return;
		}
	}
	else
	{
		list<POINTLERP*>::iterator iter = m_PointList.begin();
		for (int i = 0; i < m_iAniCnt; ++i)
			++iter;
		POINTLERP*	pPrePoint = (*iter);
		if (iter == m_PointList.end())
		{
			m_bPlayEffect = false;
			m_iAniCnt = -1;
			m_fAccPlayTime = 0.f;
			return;
		}
		++iter;
		if (iter == m_PointList.end())
		{
			m_bPlayEffect = false;
			m_iAniCnt = -1;
			m_fAccPlayTime = 0.f;
			m_bEffectDie = true;
			return;
		}
		POINTLERP*	pNextPoint = (*iter);

		float fTimeInterval = pNextPoint->fTimePoint - pPrePoint->fTimePoint;
		XMFLOAT3	vOutPos = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3	vOutAng = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3	vOutScl = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT4	vOutCol = XMFLOAT4(0.f, 0.f, 0.f, 0.f);

		XMStoreFloat3(&vOutPos, XMVectorLerp(XMLoadFloat3(&pPrePoint->xmf3PosPoint),
			XMLoadFloat3(&pNextPoint->xmf3PosPoint), m_fAccPlayTime / fTimeInterval));
		XMStoreFloat3(&vOutAng, XMVectorLerp(XMLoadFloat3(&pPrePoint->xmf3AnglePoint),
			XMLoadFloat3(&pNextPoint->xmf3AnglePoint), m_fAccPlayTime / fTimeInterval));
		XMStoreFloat3(&vOutScl, XMVectorLerp(XMLoadFloat3(&pPrePoint->xmf3ScalePoint),
			XMLoadFloat3(&pNextPoint->xmf3ScalePoint), m_fAccPlayTime / fTimeInterval));
		XMStoreFloat4(&vOutCol, XMVectorLerp(XMLoadFloat4(&pPrePoint->xmf4ColorPoint),
			XMLoadFloat4(&pNextPoint->xmf4ColorPoint), m_fAccPlayTime / fTimeInterval));

		if (m_fAccPlayTime / fTimeInterval >= 1) {
			++m_iAniCnt;
			m_fAccPlayTime = 0.f;
		}
		m_f3Pos = vOutPos;
		m_f3Angle = vOutAng;
		m_f4Scale.x = vOutScl.x;	m_f4Scale.y = vOutScl.y;		m_f4Scale.z = vOutScl.z;
		m_f4Color = vOutCol;
	}
}

void CMyEffect::UpdateFrameAnimation(float fDeltaTime)
{
	m_tEffectInfo.m_fFrameCnt += fDeltaTime * m_tEffectInfo.m_fFrameSpd;
	//m_bEnd = false;
	if (int(m_tEffectInfo.m_fFrameCnt) >= m_tEffectInfo.m_iU)
	{
		++m_tEffectInfo.m_iFrameHeight;
		if (m_tEffectInfo.m_iFrameHeight >= m_tEffectInfo.m_iV)
		{
			m_tEffectInfo.m_iFrameHeight = 0;
			//	m_bEnd = true;
		}
		m_tEffectInfo.m_fFrameCnt = 0.f;
	}
}

void CMyEffect::AdjustBillboard()
{
	XMFLOAT4X4		matView, matBill;
	//if (nullptr == m_pEffectRenderCont->m_pCamera) return;
	//XMStoreFloat4x4(&matRot, XMMatrixRotationQuaternion(XMLoadFloat4(&m_f4Quaternion)));

	//XMStoreFloat4x4(&matScl, XMMatrixScaling(m_f4Scale.x, m_f4Scale.y, m_f4Scale.z));

	XMStoreFloat4x4(&matView, CAMMGR->GetCamera(CAM_FREE)->GetViewMtx());
	XMStoreFloat4x4(&matBill, XMMatrixIdentity());
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	XMStoreFloat4x4(&matBill, XMMatrixInverse(nullptr, XMLoadFloat4x4(&matBill)));

	matBill._41 = m_f3Pos.x;
	matBill._42 = m_f3Pos.y;
	matBill._43 = m_f3Pos.z;

	XMMATRIX matResult, matrixView, matrixBill;
	matResult = XMMatrixScaling(m_f4Scale.x, m_f4Scale.y, m_f4Scale.z)
		* XMMatrixRotationQuaternion(XMLoadFloat4(&m_f4Quaternion))
		* XMLoadFloat4x4(&matBill);

	XMStoreFloat4x4(&m_tEffectInfo.m_matWorld, matResult);	
}

void CMyEffect::SettingPoint(POINTLERP * pIter)
{
	m_f3Pos = pIter->xmf3PosPoint;
	m_f3Angle = pIter->xmf3AnglePoint;
	m_f4Scale.x = pIter->xmf3ScalePoint.x;	m_f4Scale.y = pIter->xmf3ScalePoint.y;		m_f4Scale.z = pIter->xmf3ScalePoint.z;
	m_f4Color = pIter->xmf4ColorPoint;
}



