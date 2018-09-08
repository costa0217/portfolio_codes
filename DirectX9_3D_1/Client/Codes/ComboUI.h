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

// 목표 1	: 플레이어가 몬스터를 때릴때마다 역동적으로 올라가는 카운트
// 목표 2   : 일자리 수가 9보다 높아지면 0이 되고 대신 십자리 수가 +1 카운트
// 목표 3   : 추가 공격이 없을시 자연스럽게 사라지는 효과 
// 목표 4	: 쓸때없는 변수를 최대한 줄인 클래스 

// 방향1 : 텍스쳐는 한장으로 하고 84 * 84 픽셀 단위로 잘린 UV값을 옮기는 식으로 랜더를 한다.
// 방향2 : 콤보 숫자가 올라가면 기존에 있던 콤보숫자는 위로 올라가면서 투명이 되고 
//		   새로운 콤보숫자는 기존에 있던 콤보숫자가 사라지는 위치에서 스케일이 커진상태 + 살짝 투명 
//		   상태에서 시작되서 본래 있어야할 위치로 갈때까지 본래의 투명값과 스케일을 찾아간다.

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
	enum	COMBO_STATE { COMBO_ONE, COMBO_TEN, COMBO_END }; // 콤보의 자리수
	enum	RENDCOMBO_STATE   { RENDCOMBO_FRONT, RENDCOMBO_BACK, RENDCOMBO_END }; // 첫 랜더가 뒤냐 앞이냐를 나눈 값
	enum	COMBOMOVE_STATE { COMBOMOVE_NULL, COMBOMOVE_START, COMBOMOVE_STOP ,COMBOMOVE_MOVE , COMBOMOVE_END }; // 콤보숫자의 움직임 상태

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

	_vec2		m_vPos[COMBO_END][RENDCOMBO_END];			// 움직일때의 pos
	_vec2		m_vSize[COMBO_END][RENDCOMBO_END];
	_vec2		m_vOriginSize[COMBO_END];					
	_vec2		m_vOriginPos[COMBO_END][RENDCOMBO_END];		// 움직이고 나서 다시 돌아올 pos
	_vec2		m_vStartUV[COMBO_END][RENDCOMBO_END];
	_vec2		m_vLastUV[COMBO_END][RENDCOMBO_END];

	_float			m_fTimeDelta;
	_float			m_fOverlapTime;		// 중복 입력 방지용 시간 
	_float			m_fTransparent[COMBO_END][RENDCOMBO_END];	// 투명수치

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

	_matrix		m_matViewFont;			// 콤보 글씨를 위한 뷰 
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

	void	Set_ContTableComboTex(void);	// 콤보 숫자
	void	Set_ConTableComboFont(void);	// 콤보 폰트

public:
	void	Set_Combo();

public:
	virtual	_ulong		Release(void);
};


#endif // ComboUI_h__
