#pragma once
#include "DXObject.h"
#include "Camera.h"
#include "RenderContainer.h"
#include "ComputeShader.h"

class CBlur;

struct stAOStaticData {
	float fHorResRcp;//s
	float fVerResRcp;//s
	float fOffsetRadius;//s
	float fRadius;//s
};

class CAORenderer : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	ID3D11ShaderResourceView* Excute( CCamera* pCamera);
	void ResizeBuffer();
	void ReleaseAmbientOcculutionViews();
private:
	void ChangeStaticData();

	UINT m_nWidth{ 0 };
	UINT m_nHeight{ 0 };

	//ssao
	bool m_bSSAOOnOff{ false };
	float m_fSSAORadius{ 0.f };
	float m_fSSAOOffsetRadius{ 0.f };

	//--------------------------ao----------------------------
	CComputeShader* m_pCSAONormalDepthDownScale;
	ID3D11Buffer				*m_pd3dbufAOMiniNormalDepth{ nullptr };//1/4scale texture
	ID3D11ShaderResourceView	*m_pd3dsrvAOMiniNormalDepth{ nullptr };
	ID3D11UnorderedAccessView	*m_pd3duavAOMiniNormalDepth{ nullptr };

	CComputeShader* m_pCSSSAOCompute;
	ID3D11Texture2D				*m_pd3dtxtAmbientOcculution{ nullptr };//1/4scale texture
	ID3D11ShaderResourceView	*m_pd3dsrvAmbientOcculution{ nullptr };
	ID3D11UnorderedAccessView	*m_pd3duavAmbientOcculution{ nullptr };

	//CStaticBuffer				*m_pAmbientOcculutionConstantBuffer{ nullptr };
	 CBuffer*			m_pAOStaticCB{ nullptr };
	 CTexture*		m_pAmbientOcculutionTexture;
	//--------------------------ao----------------------------

public:
	CAORenderer();
	virtual ~CAORenderer();

	//ssao
	void SetSSAORadius(float fSSAORadius) { m_fSSAORadius = fSSAORadius; };
	float GetSSAORadius() { return m_fSSAORadius; }
	void SetSSAOOffsetRadius(float SSAOOffsetRadius) { m_fSSAOOffsetRadius = SSAOOffsetRadius; }
	float GetSSAOOffsetRadius() { return m_fSSAOOffsetRadius; }
	bool GetSSAOOnOff() { return m_bSSAOOnOff; }
	void SetSSAOOnOff(bool b) { m_bSSAOOnOff = b; }
	void SetSSAOValues(bool bSSAOOnOff, float fSSAOOffsetRadius, float fSSAORadius);
};