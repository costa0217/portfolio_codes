#pragma once
#include "DXObject.h"
#include "Buffer.h"
#include "PositionInfoManager.h"

enum ANGLETYPE {
	ANGLE_X,
	ANGLE_Y,
	ANGLE_END
};

enum CAMERA_MODE {
	MODE_FIX,
	MODE_FREE,
	MODE_END
};
struct stPSBufferStaticData {
	XMMATRIX xmmtxProjection;
	XMFLOAT4 xmf4PerspectiveValues;
};
struct stPSBufferDynamicData {
	XMMATRIX xmmtxViewInv;
	XMFLOAT3 xmf3CameraPos;
	UINT pad;
	XMMATRIX xmtxViewProj;
};
struct stVGDSBufferData {
	XMMATRIX xmmtxViewProjection;
};
struct stHSBufferData {
	XMFLOAT3 xmf3CameraPos;
	UINT pad;
};

struct stCSStaticData {
	XMFLOAT4 ProjParams;// camera
	float fMaxDepth;//camera
	UINT pad[3];
};
struct stCSDynamicData {
	XMMATRIX ViewMtx;//camera
};
class CPlayer;
class CCamera :public DXObject{
public:
	//action cam
	void ActionCamStart(string sName);
	void ActionCamEnd();
	void ActionCamProc();

	bool m_bActionCam{ false };
	UINT m_CurPositionInfoIndex{ 0 };
	float m_fProgress{ 0.f };
	string m_sActionCamName;
	//----------------------------dxobject-----------------------------
	virtual bool Begin();
	virtual bool End();

public:
	virtual void SetShaderState();
	virtual void CleanShaderState();
	void UpdateShaderState();

public:
	//flustum culling
	bool IsInFrustum(BoundingBox& boundingBox);
	//---------------------------------------카메라 조정---------------------------------
	void SetLookAt(XMVECTOR xmvPos, XMVECTOR xmvLookAt, XMVECTOR xmvUp);
	virtual void SetPosition(XMVECTOR pos);
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f) = 0;
	
public:
	virtual void Update(float fTimeElapsed) = 0;
	virtual void Render() {};				//debug모드시 카메라 그리기 

	virtual void ProcessInput(float fTimeElapsed) = 0;
	virtual XMMATRIX GetWorldMtx();			//world mtx 제작 후 반환 render할 때 사용 

public:
	void GenerateProjectionMatrix(float fFov, float fRatio, float fNear, float fFar);
	void SetViewport(DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ);
	
	void UpdateViewMtx();				//viewmtx 갱신
	//void UpdateReflectionViewMtx();		//viewReflectionmtx 갱신
	void UpdateReflectionViewMtx(XMVECTOR xmvReflectePlane);
	////player
	//void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	//CPlayer* GetPlayer() { return m_pPlayer; }
	////player

public:		// Getter
	XMMATRIX GetViewMtx() { return XMLoadFloat4x4(&m_xmf4x4View); }
	XMMATRIX GetReflectionViewMtx() { return XMLoadFloat4x4(&m_xmf4x4ReflectionView); }
	XMMATRIX GetProjectionMtx() { return XMLoadFloat4x4(&m_xmf4x4Projection); }
	XMFLOAT4X4& GetProjectionFloat4x4() { return m_xmf4x4Projection; }
	D3D11_VIEWPORT& GetViewport() { return m_d3dViewport; }
	RECT GetClientSize() { return m_rcClient; };
	XMVECTOR GetPosition() { return XMLoadFloat3(&m_xmf3Pos); }
	XMVECTOR GetLook() { return XMLoadFloat3(&m_xmf3Look); }
	XMVECTOR GetLookAt(){return XMLoadFloat3(&m_xmf3At); }
	XMVECTOR GetUp() { return XMLoadFloat3(&m_xmf3Up); }
	float GetFarClip() { return m_fFarClip; }
	float GetNearClip() { return m_fNearClip; }

protected:
	float m_fNearClip{ 0.f };
	float m_fFarClip{ 0.f };

protected:	//aabb flustum rcClient
	BoundingFrustum m_OriBoundingFrustum;
	BoundingFrustum m_BoundingFrustum;

	RECT m_rcClient;		//rect size

protected:
	CGameObject*	m_pTarget{ nullptr };
	CAMERA_MODE		m_eMode{ MODE_END };

	bool			m_bFix{ false };
public:
	void SetFixCamera(bool bFix) { m_bFix = bFix; }
	CGameObject* GetTarget() {return m_pTarget;}
	void SetTarget(CGameObject* pTarget);
	void SetMode(CAMERA_MODE eMode) { m_eMode = eMode; }
	bool IsTarget() { return m_pTarget == nullptr ? false : true; }

protected:	//카메라 상수버퍼
	D3D11_VIEWPORT m_d3dViewport;
	
	 CBuffer* m_pVGDSBuffer{ nullptr };
	 CBuffer* m_pHSBuffer{ nullptr };
	 CBuffer* m_pPSBuffer{ nullptr };

	 CBuffer* m_pPS_UnpakingBuffer{ nullptr };

	 CBuffer* m_pCS_StaticBuffer{ nullptr };
	 CBuffer* m_pCS_DynamicBuffer{ nullptr };

	XMFLOAT4X4 m_xmf4x4View;
	XMFLOAT4X4 m_xmf4x4ReflectionView;
	XMFLOAT4X4 m_xmf4x4Projection;

	XMFLOAT3 m_xmf3Right;
	XMFLOAT3 m_xmf3Up;
	XMFLOAT3 m_xmf3Look;
	XMFLOAT3 m_xmf3Pos;

	XMFLOAT3 m_xmf3At{ 0.f, 0.f, 1.f };
	XMFLOAT3 m_xmf3UpDefault{ 0.f, 1.f, 0.f };

public:
	float m_fCurrentAngle[ANGLE_END]{ 0.f };
	float m_fTargetAngle[ANGLE_END]{ 0.f };

	float m_fCurDistance{ 10.f };
	float m_fTgtDistance{ 10.f };

	float m_cxDelta{ 0.f };
	float m_cyDelta{ 0.f };

protected:
	bool	m_bVibrationStart{ false };
	float	m_fVibrationYValue{ 0.f };	// 진동수치
	float	m_fVibrationSpeed{ 0.f };	// 진동 시간

	float	m_fVibrationY{ 0.f };		// 현재 진동된 값
	float	m_fVibrationTime{ 0.f };	// 카메라 현재 시간
	float	m_fVibrationMaxTime{ 10.f };	// 카메라 최대 시간

	bool	m_bFovChange{ false };
	float	m_fFovY{ XMConvertToRadians(60.f) };

public:
	virtual void CameraStartVibration(float fVibeYvalue, float fVibeSpeed) {};
	void AttackStartZoomInOut(bool bChange) { m_bFovChange = bChange; };
	virtual void CameraVibration(float fTime) {};
	virtual void AttackZoomInOut(float fTime) {};



public:
	CCamera();
	virtual ~CCamera();

};