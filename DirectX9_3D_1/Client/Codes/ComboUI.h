#ifndef ComboUI_h__
#define ComboUI_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

// ��ǥ 1	: �÷��̾ ���͸� ���������� ���������� �ö󰡴� ī��Ʈ
// ��ǥ 2   : ���ڸ� ���� 9���� �������� 0�� �ǰ� ��� ���ڸ� ���� +1 ī��Ʈ
// ��ǥ 3   : �߰� ������ ������ �ڿ������� ������� ȿ�� 
// ��ǥ 4	: �������� ������ �ִ��� ���� Ŭ���� 

// ����1 : �ؽ��Ĵ� �������� �ϰ� 84 * 84 �ȼ� ������ �߸� UV���� �ű�� ������ ������ �Ѵ�.
// ����2 : �޺� ���ڰ� �ö󰡸� ������ �ִ� �޺����ڴ� ���� �ö󰡸鼭 ������ �ǰ� 
//		   ���ο� �޺����ڴ� ������ �ִ� �޺����ڰ� ������� ��ġ���� �������� Ŀ������ + ��¦ ���� 
//		   ���¿��� ���۵Ǽ� ���� �־���� ��ġ�� �������� ������ ������ �������� ã�ư���.

class CComboUI : public Engine::CGameObject
{
private:
	explicit CComboUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	~CComboUI(void);


public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	

public:
	static	CComboUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	enum	COMBO_STATE { COMBO_ONE, COMBO_TEN, COMBO_END }; // �޺��� �ڸ���
	enum	RENDCOMBO_STATE   { RENDCOMBO_FRONT, RENDCOMBO_BACK, RENDCOMBO_END }; // ù ������ �ڳ� ���̳ĸ� ���� ��
	enum	COMBOMOVE_STATE { COMBOMOVE_NULL, COMBOMOVE_START, COMBOMOVE_STOP ,COMBOMOVE_MOVE , COMBOMOVE_END }; // �޺������� ������ ����

	COMBOMOVE_STATE		m_eMoveState[COMBO_END][RENDCOMBO_END];

private:
	LPD3DXEFFECT		m_pEffect;

// Combo Number
private:
    _int		m_iComboCnt[COMBO_END][RENDCOMBO_END];
	
	_bool		m_bStart;
	_bool		m_bRenderStart[COMBO_END][RENDCOMBO_END];

	_matrix		m_matOrtho;
	_matrix		m_matView[COMBO_END][RENDCOMBO_END];

	_vec2		m_vPos[COMBO_END][RENDCOMBO_END];			// �����϶��� pos
	_vec2		m_vSize[COMBO_END][RENDCOMBO_END];
	_vec2		m_vOriginSize[COMBO_END];					
	_vec2		m_vOriginPos[COMBO_END][RENDCOMBO_END];		// �����̰� ���� �ٽ� ���ƿ� pos
	_vec2		m_vStartUV[COMBO_END][RENDCOMBO_END];
	_vec2		m_vLastUV[COMBO_END][RENDCOMBO_END];

	_float			m_fTimeDelta;
	_float			m_fOverlapTime;		// �ߺ� �Է� ������ �ð� 
	_float			m_fTransparent[COMBO_END][RENDCOMBO_END];	// �����ġ

	const TCHAR*	m_pResourceKey;

	int m_iCurComboCnt;
	int m_iTemp;

private:
	Engine::CTransform*		m_pTransCom;
	Engine::CMaterial*		m_pMtrlCom[COMBO_END];
	Engine::CVIBuffer*		m_pBufferCom[COMBO_END][RENDCOMBO_END];
	Engine::CTexture*		m_pTextureCom;

	Engine::VTXTEX*			m_pVertex[COMBO_END][RENDCOMBO_END];
	Engine::VTXTEX*			m_pOriginVertex[COMBO_END][RENDCOMBO_END];


private: // Combo Font
	Engine::CTransform*		m_pTransComFont;
	Engine::CMaterial*		m_pMtrlComFont;
	Engine::CVIBuffer*		m_pBufferComFont;
	Engine::CTexture*		m_pTextureComFont;

	_matrix		m_matViewFont;			// �޺� �۾��� ���� �� 
	_vec2		m_vPosFont;
	_vec2		m_vSizeFont;

	Engine::VTXTEX*			m_pVertexFont;
	const TCHAR*			m_pResourceKeyFont;

	_bool					m_bComboPlayer;

private:
	void	ComboControl();

private:
	// Set_ContantTable
	void	Set_ConTableComboNum(COMBO_STATE _eCombo, RENDCOMBO_STATE _eComboRend);

	void	Set_Initialize();

	void	Set_ContTableComboTex(void);	// �޺� ����
	void	Set_ConTableComboFont(void);	// �޺� ��Ʈ

public:
	void	Set_Combo();

public:
	virtual	_ulong		Release(void);
};


#endif // ComboUI_h__
