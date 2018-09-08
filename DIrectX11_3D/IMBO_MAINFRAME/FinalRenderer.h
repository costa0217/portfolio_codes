#pragma once

struct OptInfo
{
	int			iOpt;	// 0 - noRadial | 1 - Radial
	float		BlurStart;
	float		BlurWidth;
	int			noUse;
};

class CFinalRenderer
{
public:
	CFinalRenderer();
	~CFinalRenderer();

public:
	HRESULT Initialize();

private:
	CRenderShader*	m_pRenderShader{ nullptr };
	CMesh*			m_pMesh{ nullptr };
	//CBuffer*		m_pBuffer{ nullptr };
	CBuffer*		m_pCBuffer{ nullptr };
	OptInfo			m_tOptInfo;
private:
	CRenderShader*	m_pRadialBlurShader{ nullptr };

public:
	void	ResizeBuffer();
	void	RenderFinalPass(ID3D11ShaderResourceView* pPostProcessSRV, ID3D11ShaderResourceView* pAlphaSRV, ID3D11ShaderResourceView* pSkyBoxSRV, ID3D11ShaderResourceView * pDistortion
			,bool bRadial, float fBlurStart, float fBlurWidth);

};

