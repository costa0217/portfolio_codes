#pragma once

class CUIObject;
class CUIRenderer
{
public:
	CUIRenderer();
	~CUIRenderer();

public:
	HRESULT Initialize();
	void	SetRenderContainer(float fLayer, CUIObject* pUI, int i = 0) { 
		if(i == 0)m_mapUI.insert(make_pair(fLayer, pUI));
		else m_mapPreUI.insert(make_pair(fLayer, pUI));
	}
	void	RenderUI();
	void	PreRenderUI();

	void	ClearData() { m_mapUI.clear(); }

private:
	multimap<float, CUIObject*> m_mapUI;
	multimap<float, CUIObject*> m_mapPreUI;

	CRenderShader*	m_pRenderShader{ nullptr };


	ID3D11BlendState*	m_pAlphaBlendState{ nullptr };
	ID3D11BlendState*	m_pPreBlendState{ nullptr };
	float* m_pPreBlendFactor{ nullptr };
	UINT m_PreSampleMask{ 0 };
};

