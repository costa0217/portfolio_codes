#pragma once
#include "DXObject.h"

struct CB_OCCLUSSION
{
	UINT nWidth;//s
	UINT nHeight;
	UINT pad[2];
};

struct stSSLRDynamicData{
	XMFLOAT2 vSunPos;//d
	UINT pad[2];
	XMFLOAT3 vRayColor;//s
	UINT pad1;
};
struct stSSLRStaticData {
	float fInitDecay;//s
	float fDistDecay;//s
	float fMaxDeltaLen;//s
	UINT pad2;
};

class CSSLR : public DXObject{
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	void Excute( CCamera* pCamera, ID3D11RenderTargetView* pLightAccumRTV, ID3D11ShaderResourceView* pMiniDepthSRV);
	void ResizeBuffer();
	void ReleaseBuffer();

//	void SetOffsetSunPos(float fOffsetSunPos) { m_fOffsetSunPos = fOffsetSunPos; }
//	void SetSunDist(float fMaxSunDist) { m_fMaxSunDist = fMaxSunDist; }
//	void SetInitDecay(float fInitDecay) { m_fInitDecay = fInitDecay; }
//	void SetDistDecay(float fDistDecay) { m_fDistDecay = fDistDecay; }
//	void SetMaxDeltaLen(float fMaxDeltaLen) { m_fMaxDeltaLen = fMaxDeltaLen; }
//
//	float& GetOffsetSunPos() { return m_fOffsetSunPos; }
//	float& GetMaxSunDist() { return m_fMaxSunDist; }
//	float& GetInitDecay() { return m_fInitDecay; }
//	float& GetDistDecay() { return m_fDistDecay; }
//	float& GetMaxDeltaLen() { return m_fMaxDeltaLen; }
private:
	void ChangeStaticData();
	//sslr
	bool m_bSSLROnOff{ false };
	float m_fSSLRMaxSunDist{ 1000.f };
	float m_fSSLRInitDecay{ 0.05f };
	float m_fSSLRDistDecay{ 0.05f };
	float m_fSSLRMaxDeltaLen{ 0.05f };
	//effects

	// Prepare the occlusion map
	void PrepareOcclusion(ID3D11ShaderResourceView* pMiniDepthSRV);

	// Ray trace the occlusion map to generate the rays
	void RayTrace( CCamera* pCamera, const XMFLOAT2& vSunPosSS, const XMFLOAT3& vSunColor,
		float fInitDecay = 0.2f,
		float fDistDecay = 0.8f,
		float fMaxDeltaLen = 0.005f);

	// Combine the rays with the scene
	void Combine( CCamera* pCamera, ID3D11RenderTargetView* pLightAccumRTV);

	UINT m_nWidth{ 0 };
	UINT m_nHeight{ 0 };
	UINT m_nThreadGroup{ 0 };

	ID3D11Texture2D* m_pOcclusionTex{ nullptr };
	ID3D11UnorderedAccessView* m_pOcclusionUAV{ nullptr };
	ID3D11ShaderResourceView* m_pOcclusionSRV{ nullptr };
	CComputeShader* m_pMakeOcclussionComputeShader{ nullptr };
	 CTexture* m_pOcclussionTexture{ nullptr };

	CRenderContainer* m_pLayTraceRenderContainer{ nullptr };
	ID3D11Texture2D* m_pLightRaysTex{ nullptr };
	ID3D11RenderTargetView* m_pLightRaysRTV{ nullptr };
	ID3D11ShaderResourceView* m_pLightRaysSRV{ nullptr };
	 CTexture* m_pLightRaysTexture{ nullptr };
	
	CRenderContainer* m_pConmbineRenderContainer{ nullptr };

	// Shaders
	 CBuffer* m_pSSLRStaticBuffer{ nullptr };
	 CBuffer* m_pSSLRDynamicBuffer{ nullptr };

	// Additive blend state to add the light rays on top of the scene lights
	ID3D11BlendState* m_pAdditiveBlendState{ nullptr };
public:
	CSSLR();
	virtual ~CSSLR();


	//sslr
	void SetSSLROnOff(bool SSLROnOff) { m_bSSLROnOff = SSLROnOff; }
	bool GetSSLROnOff() { return m_bSSLROnOff; }
	void SetSSLRMaxSunDist(float SSLRMaxSunDist) { m_fSSLRMaxSunDist = SSLRMaxSunDist; }
	float GetSSLRMaxSunDist() { return m_fSSLRMaxSunDist; }
	void SetSSLRInitDecay(float SSLRInitDecay) { m_fSSLRInitDecay = SSLRInitDecay; }
	float GetSSLRInitDecay() { return m_fSSLRInitDecay; }
	void SetSSLRDistDecay(float SSLRDistDecay) { m_fSSLRDistDecay = SSLRDistDecay; }
	float GetSSLRDistDecay() { return m_fSSLRDistDecay; }
	void SetSSLRMaxDeltaLen(float fSSLRMaxDeltaLen) { m_fSSLRMaxDeltaLen = fSSLRMaxDeltaLen; }
	float GetSSLRMaxDeltaLen() { return m_fSSLRMaxDeltaLen; }

	void SetSSLRValues(bool bOnOff, float fMaxSunDist, float fInitDecay, float fDistDecay, float fMaxDeltaLen);
};

