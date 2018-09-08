#pragma once
#include "DXObject.h"

struct SSReflectionVSConstants {//d
	XMMATRIX ViewProjection;//vs ds
	XMMATRIX View;//vs ds
};
struct SSReflectionPSConstants {
	float ViewAngleThreshold;
	float EdgeDistThreshold;
	float minDepthBias;
	float maxDepthBias;

	float ReflectionScale;
	float PixelSize;
	int nNumSteps;
	UINT pad;
};

class CSSRF : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	void Excute( CCamera* pCamera, ID3D11RenderTargetView* prtvHDR, ID3D11DepthStencilView* pdsvReadOnlyDepthStencil, ID3D11ShaderResourceView* psrvHDR, ID3D11ShaderResourceView* psrvDepth, ID3D11ShaderResourceView* psrvNormal);
	void ResizeBuffer();
	void ReleaseBuffer();

	void SetMinDepthBias(float fMinDepthBias) { m_fMinDepthBias = fMinDepthBias; }
	void SetMaxDepthBias(float fMaxDepthBias) { m_fMaxDepthBias = fMaxDepthBias; }
	void SetEdgeDistThreshold(float fEdgeDistThreshold) { m_fEdgeDistThreshold = fEdgeDistThreshold; }
	void SetReflectionScale(float fReflectionScale) { m_fReflectionScale = fReflectionScale; }
	void SetViewAngleThreshold(float fViewAngleThreshold) { m_fViewAngleThreshold = fViewAngleThreshold; }
	void SetPixelScale(float fPixelScale) { m_fPixelScale = fPixelScale; }
	void NumStepScale(float fNumStepScale) { m_fNumStepScale = fNumStepScale; }

	void SetSSRFValues(float fMinDepthBias, float fMaxDepthBias, float fEdgeDistThreshold, float fReflectionScale,
		float fViewAngleThreshold, float fPixelScale, float fNumStepScale);
private:
	void ChangeStaticData();
	 CBuffer* m_pSSReflectionVSConstants{ nullptr };
	 CBuffer* m_pSSReflectionPSConstants{ nullptr };
	ID3D11BlendState* m_pd3dBlendState{ nullptr };

	ID3D11ShaderResourceView* m_pd3dsrvRef{ nullptr };
	ID3D11RenderTargetView* m_pd3drtvRef{ nullptr };
	ID3D11Texture2D* m_pd3dtxtRef{ nullptr };

	float m_fMinDepthBias{ 0.1f };
	float m_fMaxDepthBias{ 0.50f };
	float m_fEdgeDistThreshold{ 0.2f };
	float m_fReflectionScale{ 2.0f };
	float m_fViewAngleThreshold{ 0.2f };

	float m_fPixelScale{ 4.f };
	float m_fNumStepScale{ 0.2f };
public:
	CSSRF();
	virtual ~CSSRF();
};

