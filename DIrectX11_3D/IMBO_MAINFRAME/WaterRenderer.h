#pragma once

struct tWvsBuffer {
	XMFLOAT4X4	reflectionMatrix;
	XMFLOAT4X4	worldMatrix;
	XMFLOAT4X4	viewMatrix;
	XMFLOAT4X4	projectionMatrix;	
};

struct tWpsBuffer {
	float		waterTranslation;
	float		reflectRefractScale;
	XMFLOAT2	padding;
};


class CWaterRenderer :
	public DXObject
{
public:
	CWaterRenderer();
	~CWaterRenderer();

public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();
	
	ID3D11ShaderResourceView* RenderReflectionMap( CCamera* pCamera, ID3D11DepthStencilView* pDepthStencilView, CObjectRenderer* objRenderer);
	ID3D11ShaderResourceView* RenderRefractionMap( CCamera* pCamera, ID3D11DepthStencilView* pDepthStencilView, CObjectRenderer* objRenderer);
	void	RenderWater( CCamera* pCamera, ID3D11ShaderResourceView* pDepthsrv);
	

	void	ResizeBuffer();
	void	ReleaseBuffer();

private:
	void	CalReflectionViewProj( CCamera* pCamera);
private:
	 CBuffer*			m_pWaterVSBuffer;
	 CBuffer*			m_pWaterPSBuffer;
	 CBuffer*			m_pReflractionVSBuffer;
	CRenderShader*				m_pWaterShader{ nullptr };
	CMesh*						m_pWaterMesh{ nullptr };

	ID3D11Texture2D			 *m_pd3dtxtReflection{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvReflection{ nullptr };
	ID3D11RenderTargetView	 *m_pd3drtvReflection{ nullptr };

	ID3D11Texture2D			 *m_pd3dtxtRefraction{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvRefraction{ nullptr };
	ID3D11RenderTargetView	 *m_pd3drtvRefraction{ nullptr };

	ID3D11BlendState*	m_pAlphaBlendState;
};

