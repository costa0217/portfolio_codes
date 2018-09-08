#ifndef PlayerSkill_h__
#define PlayerSkill_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CPlayerSkill : public Engine::CGameObject
{
private:
	explicit	CPlayerSkill(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerSkill(void);

public:
	virtual HRESULT	Initialize(int iStageNum);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	

public:
	static	CPlayerSkill*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int iStageNum);

public:
	virtual		_ulong		Release(void);

private:
	// CHANGE_NULL : 가만히 있을때
	// CHANGE_UP   : 위로 교체
	// CHANGE_DOWN : 아래로 교체
	enum	CHANGE_STATE { CHANGE_NULL, CHANGE_UP, CHANGE_DOWN, CHANGE_END };
	enum	SKILL_LIST	{ SL_Judgement, SL_Charge_Slash, SL_Execution, SL_Berserker, SL_Hell, SL_End };

	CHANGE_STATE	m_eChange;

private:
	LPD3DXEFFECT		m_pEffect;
	_matrix				m_matOrtho;			// 직교투영 매트릭스
	_float				m_fTimeDelta;
	_float				m_fOverlapTime;		// 키중복 체크용 시간
	_float				m_fIconTime;		// 아이콘이 움직이는 시간 
	_float				m_fButtonTime;		// 버튼이 눌려지는 시간

	int		m_iCenterNum;	// 중앙의 스킬 넘버
	_bool	m_bMove;		// 스킬이 움직일때 true 
	_bool	m_bButtonDown;	// 스킬선택 버튼(V버튼)이 눌려지면 true 
	_bool	m_bButtonDownStart;	// 버튼이 눌려질때 한번만 pos값을 옮기기 위한 체크 변수
	_bool	m_bJudgement[2];	// 심판을 2번 눌르기 위해 선언

	Engine::CTransform*		m_pTransCom;				
	Engine::CTexture*		m_pTextureCom;				
	Engine::CVIBuffer*		m_pBufferCom[SL_End];		
	Engine::CVIBuffer*		m_pBufferCoolCom[SL_End];

	Engine::CTexture*		m_pTextureCoolCom;

// 스킬 사용 못할때 뜨는 텍스쳐 관련
// 버퍼를 동시에 띄운 다음에 만약 어느 한 폰트가 이미 띄워져 있다면 
// 다른 폰트는 그 아래에서 띄워져야 한다.

private:
	enum	SKLILLFONT_TYPE{ SF_SP, SF_COOLTIME, SF_END, SF_OFF};

	Engine::CVIBuffer*		m_pBufferComFont[SF_END];
	Engine::CTexture*		m_pTextureComFont;

	_matrix		m_matViewFont[SF_END];			// 콤보 글씨를 위한 뷰 
	_vec2		m_vPosFont[SF_END];
	_vec2		m_vPosOriginFont;
	_vec2		m_vSizeFont[SF_END];
	_vec2		m_vStartUVFont[SF_END];
	_vec2		m_vLastUVFont[SF_END];

	_float		m_fTransparentFont[SF_END];

	_bool		m_bFailFont[SF_END];
	
	SKLILLFONT_TYPE		m_eFirstFont;

	Engine::VTXTEX*			m_pVertexFont[SF_END];
	Engine::VTXTEX*			m_pOriginVertexFont[SF_END];

private:
	typedef struct	tagSkillInfo	// 5개의 스킬안에 각각 들어갈 구조체
	{
		SKILL_LIST	m_eName;		// 스킬 리스트 enum값
		_matrix		m_matView;

		_vec3		m_vPos;
		_vec2		m_vSize;
		_vec2		m_vStartUV;
		_vec2		m_vLastUV;

		_vec2		m_vOriginSize;		// 본래의 스킬 사이즈 

		_float		m_fTransparent;		// 투명값 ( 1에 가까울수록 투명이니 주의 )

		int			m_iRenderRank;		// 1에 가까울수록 나중에 랜더시키기 위한 변수
		// 이 값을 통해 내가 누른 스킬의 번호를 알아냄

		bool		m_bCoolTime;		// 쿨타임 돌때 체크 
		_float		m_fCoolTime;		// 쿨타임 남은 시간, 쿨타임이 돌때 실시간으로 빼짐
		_float		m_fCoolTimeBack;	// 쿨타임 전체의 시간, 쿨타임이 다 돌때 이 값을 m_fCoolTime에 넣어줘서 초기화 시켜줌
		_vec3		m_vOrizinPos;		// 움직인 좌표값을 되돌리기 위한 백업용 위치값
		_float		m_fPer;				// 스킬창 크기에 쿨타임 크기가 차지하는 비율
		_float		m_fMag;				// 사이즈 배율

		_float		m_fNeedSp;			// 스킬 사용 요구 SP
		_bool		m_bSkillStart;

		Engine::CVIBuffer*		m_pBufferCom;
		Engine::VTXTEX*			m_pVertex;
		Engine::VTXTEX*			m_pOriginVertex;

		tagSkillInfo(void)		// 구조체 초기화 
		{
			m_vPos			= _vec3(0.f, 0.f , 0.f);
			m_vOrizinPos	= _vec3(0.f, 0.f, 0.f);
			m_vSize			= _vec2(0.f, 0.f);
			m_vOriginSize	= _vec2(0.f, 0.f);
			m_vStartUV		= _vec2(0.f, 0.f);
			m_vLastUV		= _vec2(0.f, 0.f);
			m_fCoolTime		= 0.f;
			m_fCoolTimeBack	= 0.f;
			m_fTransparent	= 0.f;
			m_bCoolTime		= false;
			m_fPer			= 0.f;
			m_fMag			= 0.f;
			m_fNeedSp		= 0.f;
			m_bSkillStart	= true;

			m_pBufferCom	= NULL;
			m_pVertex		= NULL;
			m_pOriginVertex	= NULL;

			D3DXMatrixIdentity(&m_matView);
		}
	}SKILLUI;

	vector<SKILLUI>			m_vecSkillUI;		// 보통 스킬 창 
	vector<SKILLUI*>		m_vecSkillUIBack;	// 보통 스킬 창 백업
	vector<SKILLUI>			m_vecSkillCool;		// 쿨타임 스킬 창
	vector<SKILLUI*>		m_vecSkillCoolBack;	// 쿨타임 스킬 창 백업

	_float					m_fMoveSillUI;
	_float					m_fTempYPos[SL_End];	// 딸깍이 Y값 보정을 위한 변수

	int						m_iJudgementCnt;
	int						m_iStageNum;
public:
	static _float			m_fSkillUseValueBalance;	//플레이어에서도 사용하기 위해 그냥 static로 선언

private:
	void	Set_ConTableSkillUi(int iSkillIndex);
	void	CoolDown(int i);
	void	SkillDownChange(int iTemp);	// 스킬을 아래로 움직일때
	void	SkillUpChange(int iTemp);	// 스킬을 위로 움직일때
	void	Set_ConTableCool(int iSkillIndex);
	void	SelectMove();	

	void	SkillFontControl(int iSkillIndex);
	void	Set_ConTableSkillFont(int iFontIndex);
	

// 플레이어 연결 관련 함수 
bool	SkillSelect();			// 스킬을 선택했을때 불러주면 되는 함수
								// 쿨타임 상태일 경우 false를 반환, 아닐 경우 true를 반환

int		Get_SkillNum(){ return m_iCenterNum; }		// 현재 가운데 있는 스킬, 즉 선택키를 눌렀을때 
													// 선택된 스킬 값을 반환한다.
													// SkillSelect()가 true일때 사용하면 될듯 

void	ReduceCoolTime(int iSkillNum, int iReduceTime);

public:
	void Init_JudgementCoolTime();

};

#endif // ListSkill_h__

