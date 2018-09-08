#pragma once
#include "DXObject.h"

struct stBigWaterInfo {
	UINT nFlag{ 0 };
	float fHeight{ 0.f };
	UINT pad[2];
};
struct stRenderSurfaceInfo {
	XMMATRIX ViewProjMatrix;
	XMMATRIX RefViewProjMatrix;

	XMFLOAT3 eyeVector;
	UINT fresnelMode;

	float xDrawfactor;
	float fPowFactor;
	//wind
	XMFLOAT2 windDir;
	float windForce;
	float progress;

	float BumpMapBaseUVScale;
	float BumpMapDetailUVScale;
	/////////////////////
	//specular
	float specIntensity;
	XMFLOAT3 DirToLight;

	////////////////////
	float specExp;
	XMFLOAT3 DirLightColor;

	//wind2
	XMFLOAT2 windDir2;
	///////////////////
	///////////////////
	float windForce2;
	float fWaterHeight;
};

class CBigWaterRenderer :
	public DXObject {

public:
	CBigWaterRenderer();
	~CBigWaterRenderer();

public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	//ID3D11ShaderResourceView* RenderReflectionMap(CCamera* pCamera, ID3D11DepthStencilView* pDepthStencilView, CObjectRenderer* objRenderer);
	//ID3D11ShaderResourceView* RenderRefractionMap(CCamera* pCamera, ID3D11DepthStencilView* pDepthStencilView, CObjectRenderer* objRenderer);
	void RenderBigWater(CCamera* pCamera, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV, vector<CTexture*>& vTexture);

	void	ResizeBuffer();
	void	ReleaseBuffer();

	void SetbBigWater(bool b) { m_bBigWater = b; }
	bool GetbBigWater() { return m_bBigWater; }

	void LoadBigWaterInfo();
private:
	bool m_bBigWater{ false };
	void RenderWater1(CCamera* pCamera);//�� �⺻ ���� �׸���
	void RenderWaterSurface(CCamera* pCamera);//�� ǥ�� �׸���
	void RenderRefractions(CCamera* pCamera);//���� �׸���
	void RenderWater2(CCamera* pCamera, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV);//�� ǥ��, ���� ���� �̿��� �� ǥ�� ����


																									//void	CalReflectionViewProj(CCamera* pCamera);
private:
	XMFLOAT4 m_xmf4WaterColor{ XMFLOAT4(0,0,0.5f,1) };
	float m_fresnelMode{ 0.f };
	float m_xDrawfactor{ 1.f };
	float m_fPowFactor{ 5.f };
	XMFLOAT2 m_windDir;
	float m_windForce{ 1.f };
	float m_progress{ 0.f };
	float m_BumpMapBaseUVScale{ 1.f };
	float m_BumpMapDetailUVScale{ 5.f };
	//specular
	float m_specExp{ 2.f };
	/////////////////////
	float m_specIntensity{ 1.f };

	float m_fWaterHeight{ 0.1f };
	//XMFLOAT3 DirToLight;

	//tmp wind 2;
	XMFLOAT2 m_windDir2;
	float m_windForce2{ 1.f };

	CGameObject* m_pWaterSurface{ nullptr };
	CBuffer*			m_pWaterColor;//water render 01 ���� water�� �⺻���� ����
	CBuffer*			m_pWaterInfoPSBuffer;//reflection map���� �� ����ϴ� ����
	CBuffer*			m_pWaterPSBuffer;//water surface �׸� �� ����ϴ� ����

	CTexture* m_pWaterBaseBumpMap{ nullptr };
	CTexture* m_pWaterDetailBumpMap{ nullptr };

	ID3D11Texture2D			 *m_pTXTReflection{ nullptr };
	ID3D11ShaderResourceView *m_pSRVReflection{ nullptr };
	ID3D11RenderTargetView	 *m_pRTVReflection{ nullptr };

	ID3D11Texture2D			 *m_pTXTRefraction{ nullptr };
	ID3D11ShaderResourceView *m_pSRVRefraction{ nullptr };
	ID3D11RenderTargetView	 *m_pRTVRefraction{ nullptr };

	ID3D11Texture2D			 *m_pTXTDepthStencil{ nullptr };
	ID3D11ShaderResourceView *m_pSRVDepthStencil{ nullptr };
	ID3D11DepthStencilView	 *m_pDSVDepthStencil{ nullptr };

	//���� ���ٽ� ������Ʈ
	ID3D11DepthStencilState			*m_pd3dMirrorToStencilState;
	ID3D11DepthStencilState			*m_pd3dReflectDepthStencilState;
	ID3D11DepthStencilState			*m_pd3dReflectSkyDepthStencilState;

	ID3D11BlendState*	m_pAlphaBlendState;
	ID3D11RasterizerState* m_pTestRSState;
};