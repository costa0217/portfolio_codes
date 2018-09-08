#include "stdafx.h"
#include "MyEffect.h"

#include "Export_Function.h"
#include "FrameAni.h"
#include "..\..\Engine\Utility\Codes\RenderTargetMgr.h"
#include "CullingMgr.h"
#include "SoundMgr.h"
#include "RenderTargetMgr.h"

CMyEffect::CMyEffect(LPDIRECT3DDEVICE9 pGraphicDev, WORD wIdx, TCHAR* pName, POINT ptXY, float fFrameSpeed)
: Engine::CGameObject(pGraphicDev)
, m_wIdx(wIdx)
, m_ptCntXY(ptXY)
, m_fFrameSpeed(fFrameSpeed)
, m_vColor(255.f, 255.f, 255.f, 255.f)
, m_bBillBoard(false)
, m_vViewPos(0.f, 0.f, 0.f)
, m_fAccAniTime(0.f)
, m_iAniCnt(-1)
, m_bRender(true)
, m_iPass(0)
, m_fDeleteTime(0.f)
, m_bReStart(false)
, m_fEffectRadius(0.f)
, m_iVecLinearSize(0)
, m_vPos(0.f, 0.f, 0.f), m_vAngle(0.f, 0.f, 0.f), m_vSize(0.f, 0.f, 0.f)
, m_bAbleEffect(false)
, m_fRevolAngle(0.f)
, m_bRefeat(false)
, m_fRefeatTime(0.f)
, m_pvecParent(NULL)	
, m_bHDR(true)
, m_fColorDown(0.f)
, m_bStop(false)
, m_vAccMovePos(0.f, 0.f, 0.f)
, m_bPost(true)
, m_vNoMovePos(0.f, 0.f, 0.f)
, m_bDistortion(false)
{
	ZeroMemory(m_wstrTexName, sizeof(TCHAR) * MAX_PATH);
	lstrcpy(m_wstrTexName, pName);
	m_bSoundPlay = false;
}

CMyEffect::CMyEffect(CMyEffect& Instance)
: Engine::CGameObject(Instance)
{
	// 안씀
}

CMyEffect::~CMyEffect(void)
{
}

HRESULT CMyEffect::Initialize(void)
{
	switch(m_wIdx)
	{
	case 0:
		m_iPass = 0;
		//m_pEffect = Engine::Get_EffectHandle(L"Shader_AlphaTexture");
		break;
	case 1:
		m_iPass = 1;
		//m_pEffect = Engine::Get_EffectHandle(L"Shader_BlackTexture");
		break;
	}
	m_pEffect = Engine::Get_EffectHandle(L"Shader_AlphaTexture");
	if(NULL == m_pEffect)		return E_FAIL;

	if(FAILED(Add_Component()))		return E_FAIL;	

	m_pFrameCom->Initialize(m_ptCntXY.x, m_ptCntXY.y, m_fFrameSpeed);

	//m_iPass = 2;

	return S_OK;
}

HRESULT CMyEffect::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Frame Component
	pComponent = m_pFrameCom = Engine::CFrameAni::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Frame", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, m_wstrTexName);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	return S_OK;
}

_int CMyEffect::Update(const _float& fTimeDelta)
{
	if(false == m_bAbleEffect)	return 0;

	//사운드 재생
	if(!m_bSoundPlay)
	{
		m_bSoundPlay = true;

		//추후 필요한 사운드 재생은 여기서 한다.
		/*if(_tcscmp(m_pStartName, L"Fire_Big2") == 0
			|| _tcscmp(m_pStartName, L"Fire_Mid2") == 0
			|| _tcscmp(m_pStartName, L"Fire_Camp") == 0)
		{
			CSoundMgr::GetInstance()->PlayEffect(SOUND::BurningFire, m_vStartPos);
		}*/
	}

	m_fDeleteTime += fTimeDelta;
	if(m_fDeleteTime >= m_VecLinear.back()->fTimePoint)	// 이부분 주목 
	{
		if(m_bReStart == false && m_bRefeat == false)
		{
			m_bAbleEffect = false;
			m_vAccMovePos	= _vec3(0.f, 0.f, 0.f);

			return 0;
		}
	}
	if(true == m_bRefeat && m_fDeleteTime > m_fRefeatTime)
	{
		m_bAbleEffect = false;
		m_pvecParent = NULL;
		//Set_OriPos();
		m_vAccMovePos	= _vec3(0.f, 0.f, 0.f);

		return 0;
	}
	if(true == m_bStop)
	{
		m_bStop			= false;
		m_bAbleEffect = false;
		m_bAbleEffect = false;
		m_pvecParent = NULL;
		//Set_OriPos();
		m_vAccMovePos	= _vec3(0.f, 0.f, 0.f);

		return 0;
	}

	Play_Animation(fTimeDelta);

	Engine::CGameObject::Update(fTimeDelta);

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	if(0 != m_fRevolAngle)
	{
		_matrix		matRevol/*, matScale*/;
		_matrix		matLocalTrans, matWorldTrans;
		D3DXMatrixTranslation(&matLocalTrans, -m_vPos.x, -m_vPos.y, -m_vPos.z);
		D3DXMatrixTranslation(&matWorldTrans, m_vPos.x, m_vPos.y, m_vPos.z);
		D3DXMatrixIdentity(&matRevol);
		D3DXMatrixRotationY(&matRevol, m_fRevolAngle/* - D3DX_PI * 0.5f*/);
		m_pTransCom->m_matWorld *= matLocalTrans * matRevol * matWorldTrans;

		if(true == m_bRefeat && NULL != m_pvecParent	)
		{
			_matrix matParent;
			D3DXMatrixTranslation(&matParent, m_pvecParent->x, m_pvecParent->y, m_pvecParent->z);
			m_pTransCom->m_matWorld *= matParent;
		}		
		//D3DXVec3TransformCoord(&m_vViewPos, &_vec3(0.f, 0.f, 0.f), &(	m_pTransCom->m_matWorld * matView));
	}
	else
	{
		if(true == m_bRefeat && NULL != m_pvecParent)
		{
			_matrix matParent;
			D3DXMatrixTranslation(&matParent, m_pvecParent->x, m_pvecParent->y, m_pvecParent->z);
			m_pTransCom->m_matWorld *= matParent;
		}		
		//D3DXVec3TransformCoord(&m_vViewPos, &_vec3(0.f, 0.f, 0.f), &(	m_pTransCom->m_matWorld * matView));
	}			

	if(m_bBillBoard)
		Adjust_BillBoard();

	_vec3 vPos = _vec3(m_pTransCom->m_matWorld._41, m_pTransCom->m_matWorld._42, m_pTransCom->m_matWorld._43);
	if(CCullingMgr::GetInstance()->CullingCamera(&vPos, m_fEffectRadius))
	{
		_vec3			vEdge[4];
		ZeroMemory(vEdge, sizeof(_vec3) * 4);
		D3DXVec3TransformCoord(&vEdge[0], &_vec3(-0.5f, -0.5f, 0.f), &(m_pTransCom->m_matWorld * matView)); 
		D3DXVec3TransformCoord(&vEdge[1], &_vec3(-0.5f, 0.5f, 0.f), &(m_pTransCom->m_matWorld * matView)); 
		D3DXVec3TransformCoord(&vEdge[2], &_vec3(0.5f, -0.5f, 0.f), &(m_pTransCom->m_matWorld * matView)); 
		D3DXVec3TransformCoord(&vEdge[3], &_vec3(0.5f, 0.5f, 0.f), &(m_pTransCom->m_matWorld * matView)); 

		float fNearDist = 1000; 
		for(int i = 0; i < 4; ++i)
		{
			if(fNearDist > vEdge[i].z)
			{
				fNearDist = vEdge[i].z;
			}
		}

		if(true == m_bDistortion)
		{
			Engine::Add_RenderGroup(Engine::CRenderer::RENDER_DISTORTION, this, fNearDist);
		}
		else
		{
			if(m_iPass < 2)
			{
				if(m_bHDR == true)	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, fNearDist);
				else 						Engine::Add_RenderGroup(Engine::CRenderer::RENDER_OBJALPHABLEND, this, fNearDist);	
			}
			else
			{
				if(m_iPass == 2)	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, fNearDist);		
				if(m_iPass == 3)	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_OBJALPHABLEND, this, fNearDist);
			}		
		}
		
	}
	return 0;
}

void CMyEffect::Play_Animation(const _float& fTimeDelta)
{
	if(0 == m_iVecLinearSize)
	{
		m_iVecLinearSize = m_VecLinear.size();
	}

	m_fAccAniTime += fTimeDelta;
	if(-1 == m_iAniCnt)
	{
		m_bRender = false;
		//list<Engine::POINTLINEAR*>::iterator iter = m_LinearList.begin();
			
		if(m_fAccAniTime > m_VecLinear[0]->fTimePoint)	// 처음 시간까지의 흐름
		{
			m_bRender		= true;
			m_iAniCnt		= 0;
			m_fAccAniTime	= 0.f;
			return;
		}
	}
	else
	{
		/*list<Engine::POINTLINEAR*>::iterator iter = m_LinearList.begin();
		for(int i = 0; i < m_iAniCnt; ++i)
			++iter;*/
		//Engine::POINTLINEAR*	pPrePoint	= (*iter);
		Engine::POINTLINEAR*	pPrePoint	= m_VecLinear[m_iAniCnt];
		if(m_iVecLinearSize == m_iAniCnt)
		{
			m_iAniCnt		= -1;
			m_fAccAniTime	= 0.f;
			return;
		}
		//++iter;
		if(m_iVecLinearSize == m_iAniCnt + 1)
		{
			m_iAniCnt		= -1;
			m_fAccAniTime	= 0.f;
			return;
		}
		Engine::POINTLINEAR*	pNextPoint	= m_VecLinear[m_iAniCnt +1];

		_float fTimeInterval	= pNextPoint->fTimePoint - pPrePoint->fTimePoint;
		_vec3	vOutPos			= _vec3(0.f, 0.f, 0.f);
		_vec3	vOutAng			= _vec3(0.f, 0.f, 0.f);
		_vec3	vOutScl			= _vec3(0.f, 0.f, 0.f);
		_vec4	vOutCol			= _vec4(0.f, 0.f, 0.f, 0.f);

		D3DXVec3Lerp(&vOutPos, &pPrePoint->vPosPoint, &pNextPoint->vPosPoint, m_fAccAniTime / fTimeInterval);
		D3DXVec3Lerp(&vOutAng, &pPrePoint->vAnglePoint, &pNextPoint->vAnglePoint, m_fAccAniTime / fTimeInterval);
		D3DXVec3Lerp(&vOutScl, &pPrePoint->vScalePoint, &pNextPoint->vScalePoint, m_fAccAniTime / fTimeInterval);
		D3DXVec4Lerp(&vOutCol, &pPrePoint->vColorPoint, &pNextPoint->vColorPoint, m_fAccAniTime / fTimeInterval);
		if(m_fAccAniTime / fTimeInterval >= 1)
		{
			++m_iAniCnt;
			m_fAccAniTime = 0.f;
		}
		m_pTransCom->m_vPosition	= vOutPos;	
		m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X] = D3DXToRadian(vOutAng.x);
		m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(vOutAng.y);
		m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z] = D3DXToRadian(vOutAng.z);
		m_pTransCom->m_vScale		= vOutScl;
		m_vColor = vOutCol;
	}
}

void CMyEffect::Setting_Init(_vec3 vPos, _vec3 vSize, _vec3 vAngle, _float fColorDown, _float fRevolAngle, _bool bRefeat, _float fRefeatTime, _vec3* pvecParent /*= NULL*/)
{
	m_bAbleEffect = true;

	m_bRender		= bRefeat;
	m_fRefeatTime	= fRefeatTime;
	m_pvecParent	= pvecParent;

	/*Set_Pos(vPos);
	Set_Size(vSize);
	Set_Angle(vAngle);*/
	Set_Data(vPos, vAngle, vSize, fColorDown);
	
	m_pFrameCom->Init_Frame();

	m_fDeleteTime		= 0.f;
	m_fAccAniTime	= 0.f;
	m_iAniCnt			= -1;
	m_fRevolAngle		= fRevolAngle;

	m_pTransCom->m_vPosition = m_VecLinear[0]->vPosPoint;
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X] = D3DXToRadian(m_VecLinear[0]->vAnglePoint.x);
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(m_VecLinear[0]->vAnglePoint.y);
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z] = D3DXToRadian(m_VecLinear[0]->vAnglePoint.z);
	m_pTransCom->m_vScale = m_VecLinear[0]->vScalePoint;
	m_vColor	= m_VecLinear[0]->vColorPoint;
}

void CMyEffect::Render(void)
{
	if(NULL == m_pEffect)		return;
	if(false == m_bRender)		return;
	if(false == m_bAbleEffect) return;

	Set_ContantTable();
	LPDIRECT3DTEXTURE9 pDepthTexture = Engine::CRenderTargetMgr::GetInstance()->Get_TargetTexture(L"Target_Depth");
	m_pEffect->SetTexture("g_DepthTexture", pDepthTexture);

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(m_iPass);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_DepthTexture", NULL);

	//무브 스피어
#ifdef _DEBUG
	/*D3DXCreateSphere(m_pGraphicDev, m_fEffectRadius, 30, 15, &m_pSphereMesh, &m_pSphereBuffer );

	_matrix MoveWorld;
	D3DXMatrixIdentity(&MoveWorld);

	D3DXMatrixTranslation(&MoveWorld, m_pTransCom->m_vPosition.x, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &( MoveWorld ) );
	m_pSphereMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	Engine::Safe_Release(m_pSphereMesh);
	Engine::Safe_Release(m_pSphereBuffer);*/
#endif
}

void CMyEffect::Adjust_BillBoard(void)
{
	//_matrix		matRotX, matRotY, matRotZ, matScale, matResult;

	//D3DXMatrixRotationX(&matRotX, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X]);
	//D3DXMatrixRotationY(&matRotY, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
	//D3DXMatrixRotationZ(&matRotZ, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z]);

	//matResult = matRotX * matRotY * matRotZ;

	//D3DXMATRIX matBilboardWorld, pmatView;               // 빌보드행렬

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &pmatView);
	//D3DXMatrixIdentity(&matBilboardWorld);         // 초기화
	//for(int i = 0; i < 3; ++i)
	//{
	//	for(int j = 0; j < 3; ++j)
	//	{
	//		matBilboardWorld.m[i][j] = pmatView.m[i][j];
	//	}
	//}
	//D3DXMatrixInverse(&matBilboardWorld, 0, &matBilboardWorld); // 빌보드행렬의역행렬을구한다
	//D3DXMatrixScaling(&matScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);

	//matBilboardWorld.m[3][0] = m_pTransCom->m_matWorld.m[3][0];
	//matBilboardWorld.m[3][1] = m_pTransCom->m_matWorld.m[3][1];
	//matBilboardWorld.m[3][2] = m_pTransCom->m_matWorld.m[3][2];

	//matBilboardWorld		= matScale * matResult * matBilboardWorld;
	//m_pTransCom->m_matWorld = matBilboardWorld;

	_matrix		matRotX, matRotY, matRotZ, matResult;

	D3DXMatrixRotationX(&matRotX, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X]);
	D3DXMatrixRotationY(&matRotY, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
	D3DXMatrixRotationZ(&matRotZ, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z]);
	matResult = matRotX * matRotY * matRotZ;

	_matrix		matView, matScale, matBill;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXMatrixScaling(&matScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);

	D3DXMatrixIdentity(&matBill);
	matBill._11		= matView._11;
	matBill._13		= matView._13;
	matBill._31		= matView._31;
	matBill._33		= matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	matBill.m[3][0] = m_pTransCom->m_matWorld.m[3][0];
	matBill.m[3][1] = m_pTransCom->m_matWorld.m[3][1];
	matBill.m[3][2] = m_pTransCom->m_matWorld.m[3][2];

	matBill		= matScale * matResult * matBill;
	m_pTransCom->m_matWorld = matBill;
}

CMyEffect* CMyEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, WORD wIdx, TCHAR* pName, POINT ptXY, float fFrameSpeed)
{
	CMyEffect*		pObj = new CMyEffect(pGraphicDev, wIdx, pName, ptXY, fFrameSpeed);

	if(FAILED(pObj->Initialize()))
	{
		MSG_BOX("Effect Create Faild");
		Engine::Safe_Release(pObj);
	}
	return pObj;
}

CMyEffect* CMyEffect::CloneCreate(CMyEffect& Instance)
{
	CMyEffect*		pObj = new CMyEffect(Instance);
	if(FAILED(pObj->Initialize()))
	{
		MSG_BOX("Effect Clone Create Faild");
		Engine::Safe_Release(pObj);
	}
	return pObj;
}

void CMyEffect::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	m_pEffect->SetInt("g_ICntX", m_pFrameCom->m_iCntX);
	m_pEffect->SetInt("g_ICntY", m_pFrameCom->m_iCntY);
	m_pEffect->SetInt("g_iFrameCnt", int(m_pFrameCom->m_fFrameX));
	m_pEffect->SetInt("g_iFrameHeight", m_pFrameCom->m_iFrameHeight);

	float fDivide = 1.f / 255.f;
	m_pEffect->SetVector("g_vColor", &_vec4(m_vColor.x * fDivide, m_vColor.y * fDivide, m_vColor.z * fDivide, m_vColor.w * fDivide));

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");

	if(2 <= m_iPass)
	{
		//m_pEffect->SetTexture("g_DepthTexture", Engine::CRenderTargetMgr::GetInstance()->Get_TargetTexture(L"Target_Depth"));
		
		_vec3		vCamera;
		D3DXMatrixInverse(&matView, NULL, &matView);
		memcpy(&vCamera, &matView.m[3][0], sizeof(_vec3));
		_vec3 vCamDir = vCamera;
		vCamDir.y = 0.f;
		D3DXVec3Normalize(&vCamDir, &vCamDir);
		m_pEffect->SetVector("g_vCamDir", &_vec4(vCamDir, 0.f));		
	}
}

_ulong CMyEffect::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{

		size_t iVecSize = m_VecLinear.size();
		for(size_t i = 0; i < iVecSize; ++i)
		{
			Engine::Safe_Delete(m_VecLinear[i]);
		}
		m_VecLinear.clear();

		delete this;
	}
	return dwRefCnt;
}

void CMyEffect::Set_Pos( _vec3 vOriPos, _vec3 vPos)
{
	m_vAccMovePos += _vec3(vPos.x ,0.f ,vPos.z);
	m_pTransCom->m_vPosition -= m_vPos;	
	m_pTransCom->m_vPosition += vOriPos - m_vAccMovePos;
	/*printf("◎ : %f   %f   %f \n",vPos.x, vPos.y, vPos.z);
	printf("★ : %f   %f   %f \n",m_vAccMovePos.x, m_vAccMovePos.y, m_vAccMovePos.z);*/
	size_t iVecSize = m_VecLinear.size();
	for(size_t i = 0; i < iVecSize; ++i)
	{
		m_VecLinear[i]->vPosPoint	 -= m_vPos;	
		m_VecLinear[i]->vPosPoint	 += vOriPos - m_vAccMovePos;
	}
	m_vPos = vOriPos - m_vAccMovePos;
}
void CMyEffect::Set_OriPos()
{
	//size_t iVecSize = m_VecLinear.size();
	//for(size_t i = 0; i < iVecSize; ++i)
	//{
	//	if(m_bPost)
	//		m_VecLinear[i]->vPosPoint += m_vAccMovePos;
	//	else
	//		m_VecLinear[i]->vPosPoint = m_vAccMovePos;
	//}
}
void CMyEffect::Set_Angle(_vec3 vAngle)
{
	size_t iVecSize = m_VecLinear.size();

	for(size_t i = 0; i < iVecSize; ++i)
	{
		////m_VecLinear[i]->vAnglePoint = m_OriVecLinear[i]->vAnglePoint + vAngle;
		m_VecLinear[i]->vAnglePoint	-= m_vAngle;
		m_VecLinear[i]->vAnglePoint	+= vAngle;
	}
	m_vAngle = vAngle;
}

void CMyEffect::Set_Size(_vec3 vSize)
{
	size_t iVecSize = m_VecLinear.size();

	for(size_t i = 0; i < iVecSize; ++i)
	{
		m_VecLinear[i]->vScalePoint -= m_vSize;
		m_VecLinear[i]->vScalePoint += vSize;
	}
	m_vSize = vSize;
}
void CMyEffect::Set_ColorDown(float fColorDown)
{
	size_t iVecSize = m_VecLinear.size();

	for(size_t i = 0; i < iVecSize; ++i)
	{
		m_VecLinear[i]->vColorPoint += _vec4(m_fColorDown, m_fColorDown, m_fColorDown, 0.f);
		m_VecLinear[i]->vColorPoint -= _vec4(fColorDown, fColorDown, fColorDown, 0.f);
	}
	m_fColorDown	= fColorDown;
}

void CMyEffect::Set_StartTime(_float vStartTime)
{
	size_t iVecSize = m_VecLinear.size();

	for(size_t i = 0; i < iVecSize; ++i)
	{
		m_VecLinear[i]->fTimePoint -= m_fStartTime;
		m_VecLinear[i]->fTimePoint += vStartTime;
	}
	m_fStartTime = vStartTime;
}



void CMyEffect::Set_Data(_vec3 vPos, _vec3 vAngle, _vec3 vSize, _float fColorDown)
{
	m_vStartPos = vPos;
	size_t iVecSize = m_VecLinear.size();

	for(size_t i = 0; i < iVecSize; ++i)
	{
		m_VecLinear[i]->vPosPoint -= m_vPos;
		m_VecLinear[i]->vPosPoint += vPos;

		m_VecLinear[i]->vAnglePoint	-= m_vAngle;
		m_VecLinear[i]->vAnglePoint	+= vAngle;

		m_VecLinear[i]->vScalePoint -= m_vSize;
		m_VecLinear[i]->vScalePoint += vSize;

		m_VecLinear[i]->vColorPoint += _vec4(m_fColorDown, m_fColorDown, m_fColorDown, 0.f);
		m_VecLinear[i]->vColorPoint -= _vec4(fColorDown, fColorDown, fColorDown, 0.f);

		if(m_VecLinear[i]->vColorPoint.x < 0)
			m_VecLinear[i]->vColorPoint.x = 0.f;
		if(m_VecLinear[i]->vColorPoint.y < 0)
			m_VecLinear[i]->vColorPoint.y = 0.f;
		if(m_VecLinear[i]->vColorPoint.z < 0)
			m_VecLinear[i]->vColorPoint.z = 0.f;
	}
	m_vPos			= vPos;
	m_vAngle		= vAngle;
	m_vSize			= vSize;
	m_fColorDown	= fColorDown;
	m_vNoMovePos= vPos;
}

void CMyEffect::Set_ReStart(_bool	bReStart)
{
	m_bReStart = bReStart;
}

void CMyEffect::Copy_Point(CMyEffect* pEffect)
{
	size_t iVecSize = pEffect->m_VecLinear.size();

	for(size_t i = 0; i < iVecSize; ++i)
	{
		Engine::POINTLINEAR* pPointLinear = new Engine::POINTLINEAR;
		memcpy(pPointLinear, pEffect->m_VecLinear[i], sizeof(Engine::POINTLINEAR));
		m_VecLinear.push_back(pPointLinear);
	}
}

void CMyEffect::Set_MaxScale(void)
{
	size_t iVecSize = m_VecLinear.size();

	_float	fSizeX = 0.f;
	_float	fSizeY = 0.f;

	for(size_t i = 0; i < iVecSize; ++i)
	{
		fSizeX = fSizeX > m_VecLinear[i]->vScalePoint.x ? fSizeX : m_VecLinear[i]->vScalePoint.x;
		fSizeY = fSizeY > m_VecLinear[i]->vScalePoint.y ? fSizeX : m_VecLinear[i]->vScalePoint.y;
	}
	m_fEffectRadius = sqrt(pow(fSizeX * 0.5f, 2) + pow(fSizeY * 0.5f, 2));
}

void CMyEffect::Set_NoTesting(void)
{
	if(true == m_bHDR)
		m_iPass = 2;
	else
		m_iPass = 3;
}

_float CMyEffect::Get_PosZ()
{
	return m_pTransCom->m_vPosition.z;
}

