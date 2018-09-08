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
	// CHANGE_NULL : ������ ������
	// CHANGE_UP   : ���� ��ü
	// CHANGE_DOWN : �Ʒ��� ��ü
	enum	CHANGE_STATE { CHANGE_NULL, CHANGE_UP, CHANGE_DOWN, CHANGE_END };
	enum	SKILL_LIST	{ SL_Judgement, SL_Charge_Slash, SL_Execution, SL_Berserker, SL_Hell, SL_End };

	CHANGE_STATE	m_eChange;

private:
	LPD3DXEFFECT		m_pEffect;
	_matrix				m_matOrtho;			// �������� ��Ʈ����
	_float				m_fTimeDelta;
	_float				m_fOverlapTime;		// Ű�ߺ� üũ�� �ð�
	_float				m_fIconTime;		// �������� �����̴� �ð� 
	_float				m_fButtonTime;		// ��ư�� �������� �ð�

	int		m_iCenterNum;	// �߾��� ��ų �ѹ�
	_bool	m_bMove;		// ��ų�� �����϶� true 
	_bool	m_bButtonDown;	// ��ų���� ��ư(V��ư)�� �������� true 
	_bool	m_bButtonDownStart;	// ��ư�� �������� �ѹ��� pos���� �ű�� ���� üũ ����
	_bool	m_bJudgement[2];	// ������ 2�� ������ ���� ����

	Engine::CTransform*		m_pTransCom;				
	Engine::CTexture*		m_pTextureCom;				
	Engine::CVIBuffer*		m_pBufferCom[SL_End];		
	Engine::CVIBuffer*		m_pBufferCoolCom[SL_End];

	Engine::CTexture*		m_pTextureCoolCom;

// ��ų ��� ���Ҷ� �ߴ� �ؽ��� ����
// ���۸� ���ÿ� ��� ������ ���� ��� �� ��Ʈ�� �̹� ����� �ִٸ� 
// �ٸ� ��Ʈ�� �� �Ʒ����� ������� �Ѵ�.

private:
	enum	SKLILLFONT_TYPE{ SF_SP, SF_COOLTIME, SF_END, SF_OFF};

	Engine::CVIBuffer*		m_pBufferComFont[SF_END];
	Engine::CTexture*		m_pTextureComFont;

	_matrix		m_matViewFont[SF_END];			// �޺� �۾��� ���� �� 
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
	typedef struct	tagSkillInfo	// 5���� ��ų�ȿ� ���� �� ����ü
	{
		SKILL_LIST	m_eName;		// ��ų ����Ʈ enum��
		_matrix		m_matView;

		_vec3		m_vPos;
		_vec2		m_vSize;
		_vec2		m_vStartUV;
		_vec2		m_vLastUV;

		_vec2		m_vOriginSize;		// ������ ��ų ������ 

		_float		m_fTransparent;		// ���� ( 1�� �������� �����̴� ���� )

		int			m_iRenderRank;		// 1�� �������� ���߿� ������Ű�� ���� ����
		// �� ���� ���� ���� ���� ��ų�� ��ȣ�� �˾Ƴ�

		bool		m_bCoolTime;		// ��Ÿ�� ���� üũ 
		_float		m_fCoolTime;		// ��Ÿ�� ���� �ð�, ��Ÿ���� ���� �ǽð����� ����
		_float		m_fCoolTimeBack;	// ��Ÿ�� ��ü�� �ð�, ��Ÿ���� �� ���� �� ���� m_fCoolTime�� �־��༭ �ʱ�ȭ ������
		_vec3		m_vOrizinPos;		// ������ ��ǥ���� �ǵ����� ���� ����� ��ġ��
		_float		m_fPer;				// ��ųâ ũ�⿡ ��Ÿ�� ũ�Ⱑ �����ϴ� ����
		_float		m_fMag;				// ������ ����

		_float		m_fNeedSp;			// ��ų ��� �䱸 SP
		_bool		m_bSkillStart;

		Engine::CVIBuffer*		m_pBufferCom;
		Engine::VTXTEX*			m_pVertex;
		Engine::VTXTEX*			m_pOriginVertex;

		tagSkillInfo(void)		// ����ü �ʱ�ȭ 
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

	vector<SKILLUI>			m_vecSkillUI;		// ���� ��ų â 
	vector<SKILLUI*>		m_vecSkillUIBack;	// ���� ��ų â ���
	vector<SKILLUI>			m_vecSkillCool;		// ��Ÿ�� ��ų â
	vector<SKILLUI*>		m_vecSkillCoolBack;	// ��Ÿ�� ��ų â ���

	_float					m_fMoveSillUI;
	_float					m_fTempYPos[SL_End];	// ������ Y�� ������ ���� ����

	int						m_iJudgementCnt;
	int						m_iStageNum;
public:
	static _float			m_fSkillUseValueBalance;	//�÷��̾���� ����ϱ� ���� �׳� static�� ����

private:
	void	Set_ConTableSkillUi(int iSkillIndex);
	void	CoolDown(int i);
	void	SkillDownChange(int iTemp);	// ��ų�� �Ʒ��� �����϶�
	void	SkillUpChange(int iTemp);	// ��ų�� ���� �����϶�
	void	Set_ConTableCool(int iSkillIndex);
	void	SelectMove();	

	void	SkillFontControl(int iSkillIndex);
	void	Set_ConTableSkillFont(int iFontIndex);
	

// �÷��̾� ���� ���� �Լ� 
bool	SkillSelect();			// ��ų�� ���������� �ҷ��ָ� �Ǵ� �Լ�
								// ��Ÿ�� ������ ��� false�� ��ȯ, �ƴ� ��� true�� ��ȯ

int		Get_SkillNum(){ return m_iCenterNum; }		// ���� ��� �ִ� ��ų, �� ����Ű�� �������� 
													// ���õ� ��ų ���� ��ȯ�Ѵ�.
													// SkillSelect()�� true�϶� ����ϸ� �ɵ� 

void	ReduceCoolTime(int iSkillNum, int iReduceTime);

public:
	void Init_JudgementCoolTime();

};

#endif // ListSkill_h__

