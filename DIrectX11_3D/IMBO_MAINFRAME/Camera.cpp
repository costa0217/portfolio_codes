#include "stdafx.h"
#include "Camera.h"

void CCamera::ActionCamStart(string sName)
{
	//CPositionInfoManager::LoadData(sName);
	m_sActionCamName = sName;
	m_bActionCam = true;
	m_CurPositionInfoIndex = 0;
	m_fProgress = 0.f;
}

void CCamera::ActionCamEnd()
{
	//CPositionInfoManager::End();
	m_bActionCam = false;
	m_CurPositionInfoIndex = 0;
	m_fProgress = 0.f;
}

void CCamera::ActionCamProc() {
	//if (INPUTMGR->KeyDown(VK_0)) {
	//	ActionCamStart("test1");
	//}
	//if (INPUTMGR->KeyDown(VK_9)) {
	//	ActionCamStart("test2");
	//}
	//if (INPUTMGR->KeyDown(VK_8)) {
	//	ActionCamStart("Firsttown_Boss1");
	//}

	if (m_bActionCam) {
		//action cam이 true라면 PositionInfo에서 
		int max_index = CPositionInfoManager::GetInfoCnt(m_sActionCamName) - 1;
		XMVECTOR xmvPos;
		XMVECTOR xmvQua;
		if (m_CurPositionInfoIndex != max_index && max_index > 0) {

			XMVECTOR xmvPos1 = CPositionInfoManager::GetAllPositionInfo()[m_sActionCamName].m_vPositionInfoData[m_CurPositionInfoIndex].GetPosition();
			XMVECTOR xmvPos2 = CPositionInfoManager::GetAllPositionInfo()[m_sActionCamName].m_vPositionInfoData[m_CurPositionInfoIndex + 1].GetPosition();
			xmvPos = XMVectorLerp(xmvPos1, xmvPos2, m_fProgress);
			XMVECTOR xmvQua1 = CPositionInfoManager::GetAllPositionInfo()[m_sActionCamName].m_vPositionInfoData[m_CurPositionInfoIndex].GetQuaternion();
			XMVECTOR xmvQua2 = CPositionInfoManager::GetAllPositionInfo()[m_sActionCamName].m_vPositionInfoData[m_CurPositionInfoIndex + 1].GetQuaternion();
			xmvQua = XMQuaternionSlerp(xmvQua1, xmvQua2, m_fProgress);
			m_fProgress += TIMEMGR->GetTimeElapsed() * CPositionInfoManager::GetActionSpeed(m_sActionCamName);
			if (m_fProgress > 1.f) {
				m_fProgress = 0.f;
				m_CurPositionInfoIndex++;
			}
		}
		else {
			if (max_index > 0) {
				xmvPos = CPositionInfoManager::GetAllPositionInfo()[m_sActionCamName].m_vPositionInfoData[m_CurPositionInfoIndex].GetPosition();
				xmvQua = CPositionInfoManager::GetAllPositionInfo()[m_sActionCamName].m_vPositionInfoData[m_CurPositionInfoIndex].GetQuaternion();
				ActionCamEnd();
			}
			else {
				ActionCamEnd();
				return;
			}
		}

		//world mtx
		XMMATRIX xmMtx = XMMatrixAffineTransformation(XMVectorSet(1, 1, 1, 1), XMQuaternionIdentity(), xmvQua, xmvPos);
		XMFLOAT4X4 xmf4x4;
		XMStoreFloat4x4(&xmf4x4, xmMtx);
		XMFLOAT3 xmf3Look = XMFLOAT3(xmf4x4._31, xmf4x4._32, xmf4x4._33);
		XMVECTOR xmvLook = XMVectorSet(xmf4x4._31, xmf4x4._32, xmf4x4._33, 1.f);
		XMVECTOR xmvLookAt = xmvPos + xmvLook;
		XMStoreFloat3(&m_xmf3Pos, xmvPos);
		//XMStoreFloat3(&m_xmf3At, xmvLook);
		XMStoreFloat3(&m_xmf3At, xmvLookAt);
	}
}
//DI
bool CCamera::Begin() {

	//변수 초기화
	XMStoreFloat4x4(&m_xmf4x4View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_xmf4x4ReflectionView, XMMatrixIdentity());
	
	////ProjectionMtx 제작
	//GenerateProjectionMatrix(fFov, fWidth / fHeight, fNear, fFar);
	
	m_xmf3Right = XMFLOAT3{ 1.0f, 0.0f, 0.0f };
	m_xmf3Up = XMFLOAT3{ 0.0f, 1.0f, 0.0f };
	m_xmf3Look = XMFLOAT3{ 0.0f, 0.0f, 1.0f };
	m_xmf3Pos = XMFLOAT3{ 0.0f, 0.0f, 0.0f };

	//상수버퍼 제작
	
	m_pVGDSBuffer = CBuffer::CreateConstantBuffer(1, sizeof(stVGDSBufferData), 11, BIND_VS | BIND_GS | BIND_DS);
	m_pHSBuffer = CBuffer::CreateConstantBuffer(1, sizeof(stHSBufferData), 11, BIND_HS);
	m_pPSBuffer = CBuffer::CreateConstantBuffer(1, sizeof(stPSBufferStaticData), 11, BIND_PS);
	m_pPS_UnpakingBuffer = CBuffer::CreateConstantBuffer(1, sizeof(stPSBufferDynamicData), 12, BIND_PS);

	m_pCS_DynamicBuffer = CBuffer::CreateConstantBuffer(1, sizeof(stCSDynamicData), 9, BIND_CS);
	m_pCS_StaticBuffer = CBuffer::CreateConstantBuffer(1, sizeof(stCSStaticData), 10, BIND_CS);
	return true;
}
bool CCamera::End() {
	if (m_pVGDSBuffer) {
		m_pVGDSBuffer->End();
		delete m_pVGDSBuffer;
	}
	if (m_pHSBuffer) {
		m_pHSBuffer->End();
		delete m_pHSBuffer;
	}
	if (m_pPSBuffer) {
		m_pPSBuffer->End();
		delete m_pPSBuffer;
	}
	if (m_pPS_UnpakingBuffer) {
		m_pPS_UnpakingBuffer->End();
		delete m_pPS_UnpakingBuffer;
	}
	if (m_pCS_StaticBuffer) {
		m_pCS_StaticBuffer->End();
		delete m_pCS_StaticBuffer;
	}
	if (m_pCS_DynamicBuffer) {
		m_pCS_DynamicBuffer->End();
		delete m_pCS_DynamicBuffer;
	}

	m_pVGDSBuffer = nullptr;
	m_pHSBuffer = nullptr;
	m_pPSBuffer = nullptr;
	m_pPS_UnpakingBuffer = nullptr;
	m_pCS_StaticBuffer = nullptr;
	m_pCS_DynamicBuffer = nullptr;

	return true;
}
void CCamera::SetPosition(XMVECTOR pos) {

	//바뀐 벡터 설정 후 
	XMStoreFloat3(&m_xmf3Pos, pos);


	//viewMtx 업데이트
	UpdateViewMtx();
}
XMMATRIX CCamera::GetWorldMtx() {
	XMFLOAT4X4 xmf4x4World{
		m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31, 0,
		m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32, 0,
		m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33, 0,
		m_xmf3Pos.x, m_xmf3Pos.y, m_xmf3Pos.z, 1
	};
	return XMLoadFloat4x4(&xmf4x4World);
}


//projection Mtx 제작
void CCamera::GenerateProjectionMatrix(float fFov, float fRatio, float fNear, float fFar) {
	XMStoreFloat4x4(&m_xmf4x4Projection, XMMatrixPerspectiveFovLH(fFov, fRatio, fNear, fFar));

	m_fFarClip = fFar;
	m_fNearClip = fNear;
	//create oriented frustum
	BoundingFrustum::CreateFromMatrix(m_OriBoundingFrustum, XMLoadFloat4x4(&m_xmf4x4Projection));

	XMMATRIX xmMtxProjection = (XMLoadFloat4x4(&m_xmf4x4Projection));
	stPSBufferStaticData* pPSStaticData = (stPSBufferStaticData*)m_pPSBuffer->Map();
	XMFLOAT4 xmf4PerspectiveValues;
	xmf4PerspectiveValues.x = 1.0f / m_xmf4x4Projection._11;
	xmf4PerspectiveValues.y = 1.0f / m_xmf4x4Projection._22;
	xmf4PerspectiveValues.z = m_xmf4x4Projection._43;
	xmf4PerspectiveValues.w = -m_xmf4x4Projection._33;

	pPSStaticData->xmf4PerspectiveValues = xmf4PerspectiveValues;
	pPSStaticData->xmmtxProjection = XMMatrixTranspose(xmMtxProjection);
	m_pPSBuffer->Unmap();

	stCSStaticData* pCSStaticData = (stCSStaticData*) m_pCS_StaticBuffer->Map();
	pCSStaticData->ProjParams = xmf4PerspectiveValues;
	pCSStaticData->fMaxDepth = m_fFarClip;
	m_pCS_StaticBuffer->Unmap();
}
//viewmtx 갱신
void CCamera::UpdateViewMtx() {
	ActionCamProc();
	//if (nullptr != m_pTarget)
	//{
	//	XMVECTOR xmvAt = XMVector4Normalize(m_pTarget->GetPosition() - XMLoadFloat3(&m_xmf3Pos));
	//	XMStoreFloat3(&m_xmf3At, xmvAt);
	//}

	XMStoreFloat4x4(&m_xmf4x4View,
		XMMatrixLookAtLH(XMLoadFloat3(&m_xmf3Pos),
			/*XMVectorAdd(XMLoadFloat3(&m_xmf3Pos), */XMLoadFloat3(&m_xmf3At)/*)*/,
			XMLoadFloat3(&m_xmf3UpDefault)));

	//update frustum
	m_OriBoundingFrustum.Transform(m_BoundingFrustum, GetWorldMtx());

	m_xmf3Right = XMFLOAT3(m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31);
	m_xmf3Up = XMFLOAT3(m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32);
	m_xmf3Look = XMFLOAT3(m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33);
}


void CCamera::UpdateReflectionViewMtx(XMVECTOR xmvReflectePlane)
{
	/*
	ref는 position을 반사시킨 위치로 이동하는 것이다.
	*/
	XMVECTOR xmvPos = XMLoadFloat3(&m_xmf3Pos);
	XMVECTOR xmvLookAt = XMLoadFloat3(&m_xmf3At);//카메라 기준 look벡터
	XMVECTOR xmvUp = XMLoadFloat3(&m_xmf3Up);

	//XMVECTOR xmvLookAt = xmvLook;//pos + look은 내가 보고있는 방향의 살짝 앞 포지션
	XMVECTOR xmvUpAt = xmvPos + xmvUp;//pos + look은 내가 보고있는 방향의 살짝 앞 포지션


	XMMATRIX xmmtxReflect = XMMatrixReflect(xmvReflectePlane);
	//XMMATRIX xmmtxReflectCamera = XMMatrixMultiply(GetWorldMtx(), xmmtxReflect);//카메라와 ref를 곱해서 카메라를 이동시킨다.

	XMVECTOR xmvRefPos = XMVector3Transform(xmvPos, xmmtxReflect);
	XMVECTOR xmvRefLookAt = XMVector3Transform(xmvLookAt, xmmtxReflect);//내가 보고있는 방향의 살짝 앞 포지션
	XMVECTOR xmvRefUpAt = XMVector3Transform(xmvUpAt, xmmtxReflect);

	XMVECTOR xmvRefLook = xmvRefLookAt - xmvRefPos;
	XMVECTOR xmvRefUp = xmvRefUpAt - xmvRefPos;

	SetLookAt(xmvRefPos, xmvRefLookAt, xmvRefUp);


	UpdateViewMtx();
	CCamera::UpdateShaderState();

	XMStoreFloat4x4(&m_xmf4x4ReflectionView, GetViewMtx());
}

//void CCamera::UpdateReflectionViewMtx()
//{
//	XMFLOAT3 xmReflePos = { m_xmf3Pos.x, -m_xmf3Pos.y, m_xmf3Pos.z };
//	XMFLOAT3 xmRefleAt = { m_xmf3At.x, -m_xmf3At.y, m_xmf3At.z };
//	XMStoreFloat4x4(&m_xmf4x4ReflectionView,
//		XMMatrixLookAtLH(XMLoadFloat3(&xmReflePos),
//		/*XMVectorAdd(XMLoadFloat3(&xmReflePos), */XMLoadFloat3(&xmRefleAt)/*)*/,
//		XMLoadFloat3(&m_xmf3UpDefault)));
//}

bool CCamera::IsInFrustum(BoundingBox& boundingBox){
	bool result = m_BoundingFrustum.Intersects(boundingBox);
	return result;
}

void CCamera::SetLookAt(XMVECTOR xmvPos, XMVECTOR xmvLookAt, XMVECTOR xmvUp)
{
	XMVECTOR vATpos = XMVectorAdd(xmvPos, xmvLookAt);

	XMStoreFloat4x4(&m_xmf4x4View, XMMatrixLookAtLH(xmvPos, vATpos, xmvUp));

	XMStoreFloat3(&m_xmf3Pos, xmvPos);
	XMStoreFloat3(&m_xmf3At, xmvLookAt);
	//XMStoreFloat3(&m_xmf3Pos, xmvPos);
	
	m_xmf3Right = XMFLOAT3(m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31);
	m_xmf3Up = XMFLOAT3(m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32);
	m_xmf3Look = XMFLOAT3(m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33);
}

void CCamera::SetViewport(DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ) {
	//0,0, r, b
	m_d3dViewport.TopLeftX = float(xTopLeft);
	m_d3dViewport.TopLeftY = float(yTopLeft);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;
	GLOBALVALUEMGR->GetDeviceContext()->RSSetViewports(1, &m_d3dViewport);

	m_rcClient.bottom = nHeight;
	m_rcClient.right = nWidth;
	m_rcClient.top = 0;
	m_rcClient.left = 0;
}
void CCamera::SetShaderState(){
	m_pVGDSBuffer->SetShaderState();
	m_pHSBuffer->SetShaderState();
	m_pPS_UnpakingBuffer->SetShaderState();
	m_pCS_DynamicBuffer->SetShaderState();

	m_pPSBuffer->SetShaderState();
	m_pCS_StaticBuffer->SetShaderState();
}
void CCamera::CleanShaderState(){
	m_pVGDSBuffer->CleanShaderState();
	m_pHSBuffer->CleanShaderState();
	m_pPS_UnpakingBuffer->CleanShaderState();
	m_pCS_DynamicBuffer->CleanShaderState();

	m_pPSBuffer->CleanShaderState();
	m_pCS_StaticBuffer->CleanShaderState();
}
//상수버퍼 갱신
void CCamera::UpdateShaderState() {
	//정보 갱신
	XMMATRIX xmMtxProjection = (XMLoadFloat4x4(&m_xmf4x4Projection));
	UpdateViewMtx();
	XMMATRIX xmMtxView = XMLoadFloat4x4(&m_xmf4x4View);

	stVGDSBufferData* pVSData = (stVGDSBufferData*)m_pVGDSBuffer->Map();
	pVSData->xmmtxViewProjection = XMMatrixTranspose(xmMtxView * xmMtxProjection);
	m_pVGDSBuffer->Unmap();

	stHSBufferData* pHSData = (stHSBufferData*)m_pHSBuffer->Map();
	pHSData->xmf3CameraPos = m_xmf3Pos;
	m_pHSBuffer->Unmap();

	stPSBufferDynamicData* pPSDynamicData = (stPSBufferDynamicData*)m_pPS_UnpakingBuffer->Map();
	pPSDynamicData->xmmtxViewInv = XMMatrixTranspose(XMMatrixInverse(nullptr, xmMtxView));
	pPSDynamicData->xmf3CameraPos = m_xmf3Pos;
	pPSDynamicData->xmtxViewProj = XMMatrixTranspose(xmMtxView * xmMtxProjection);
	m_pPS_UnpakingBuffer->Unmap();

	stCSDynamicData* pCSDynamicData = (stCSDynamicData*) m_pCS_DynamicBuffer->Map();
	pCSDynamicData->ViewMtx = xmMtxView;
	m_pCS_DynamicBuffer->Unmap();
}

void CCamera::SetTarget(CGameObject * pTarget)
{
	m_pTarget = pTarget;
	XMVECTOR xvTargetDir = m_pTarget->GetLook();
	XMVECTOR xvTargetRight = m_pTarget->GetRight();

	xvTargetDir = XMVector4Transform(xvTargetDir * -1, XMMatrixRotationAxis(xvTargetRight, XMConvertToRadians(45.f)));
	XMStoreFloat3(&m_xmf3Pos, m_pTarget->GetPosition() + xvTargetDir * 100.f);
	m_xmf3Pos.y = m_xmf3Pos.y + 10.f;
	m_pTarget->SetCamera(this);
	//XMStoreFloat3(&m_xmf3At, xvTargetDir * -1);
}



CCamera::CCamera() : DXObject("camera"){
	XMStoreFloat4x4(&m_xmf4x4View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_xmf4x4Projection, XMMatrixIdentity());


}
CCamera::~CCamera() {
	
}
