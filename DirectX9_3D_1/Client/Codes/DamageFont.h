#ifndef DamageFont_h__
#define DamageFont_h__

#include "Defines.h"
#include "GameObject.h"

#include "Layer.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

// ��ǥ 1 : �������� Pos�� ������ �� ���� ���� �������� ������ ��ġ�� ����.
// ��ǥ 2 : ������ ü�µ� ����ؼ� �������� ü���� �ѱ�� 
//			��ŭ�� OverKill ������ ��Ʈ�� �Ѱ��ش�.
// ��ǥ 3 : 5000 ~ 9999�� ������ ������ ����.

class CDamageFont : public Engine::CGameObject
{
private:
	explicit CDamageFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDamageFont(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	

public:
	static	CDamageFont*	Create(LPDIRECT3DDEVICE9 pGraphicDev, DFONT_FONT_TYPE eType, DFONT_OBJ_TYPE eState, _int iDamage, _vec3 vPos);
	static	Engine::CLayer* m_pLayer;

private:
	LPD3DXEFFECT		m_pEffect;

// enum�� ����
private:
	enum DAMAGE_UPDATE	 { DAMAGE_INIT, DAMAGE_WAVE, DAMAGE_DEL, DAMAGE_END };
	enum OVERKILL_UPDATE { OVERKILL_INIT, OVERSKILL_STOP, OVERSKILL_DEL , OVERSKILL_END };

private:
	DFONT_FONT_TYPE		m_eDfontType;		// ��Ʈ�� ����
	DFONT_OBJ_TYPE		m_eDfontState;		// ������ ��Ʈ�� ����

	DAMAGE_UPDATE		m_eDamage;
	OVERKILL_UPDATE		m_eOverkill;

// ���� ����
private:
	_float			m_fTimeDelta;
	_float			m_fOverlapTime;		// �ߺ� �Է� ������ �ð�
	_bool			m_bStart;
	_matrix			m_matOrtho;
	_matrix			m_matView[4];		// �ִ� 4�ڸ� ���� ���
	_float			m_fDelTime;			// �Ҹ�Ǳ������ �ð� 

	int m_iOne;
	int m_iTen;
	int m_iHundred;
	int m_iThousand;

	_vec3			m_iRandPos;			// ������ ��ġ ������ ���� ����
	_bool			m_bDelete;

private:
	Engine::CTransform*		m_pTransCom;
	Engine::CMaterial*		m_pMtrlCom;
	Engine::CVIBuffer*		m_pBufferCom[4];
	Engine::CTexture*		m_pTextureCom;

	Engine::VTXTEX*			m_pVertex[4];
	Engine::VTXTEX*			m_pOriginVertex;
	

// �ܺ� ���� ����
private:
	_vec3		m_vPos;
	_int		m_iDamage;

// Damage Number
// 1. ũ��Ƽ�� �������� ��� 
// 2. �Ϲ� �������� ���
// 3. ���� ������ �ǰݴ��� ���
// >> ���� �ٲ��ָ� �ȴ�.
// �������� ���� �÷��̾�� ������ x,y ���� �����ְ�
// z���� �Ÿ� ������ �缭 Size�� �����ϴ� ������� �Ѵ�.
// ó�� �������� �㶧 ��¦ ���� ���� �������� �ѵι� �����̴� Y������ �ö󰡸鼭
// Size���� Ŀ���� ������ �۾����� �������.
// ���۴� 4���� ���
// ������ 0�� �Ǹ� ������ ��Ʈ�� �����ִ°ɷ�
private:
	_vec3		m_vDamPos[4];
	_vec2		m_vDamSize[4];
	_vec2		m_vDamStartUV[4];
	_vec2		m_vDamLastUV[4];

	_float		m_fDamTransparent[4];
	_int		m_iWaveCnt[4];
	_float		m_fWaveTime[4];		// Wave Ÿ��
	_bool		m_bWaveDir[4];		// false�϶� �Ʒ� , true�϶� ���� ���̺�
	_float		m_fPosGap[4];		// ���� ���̵��� ��

private:
	void	Set_ConTableDamage(int iNum);
	void	DamageFontControl(void);

// OverKill Number
// 1. ���� ü���� �Ѵ� �������� ������ �� ���� ��ŭ�� ���� ���� �����Ѵ�.
// 2. ��õ��� ������ ���ڷ� ������ �ٲٴ� ���� ���� ��ŭ�� ������ �ٲ۴�.
// 3. ��������� ��ġ��ȭ�� ���� �׳� Size�� Ű���ָ鼭 �����ϰ� ����� ������Ų��.
private:
	_vec3		m_vOverPos[4];
	_vec2		m_vOverSize[4];
	
	_vec2		m_vOverStartUV[4];
	_vec2		m_vOverLastUV[4];
	_vec2		m_vOriOverStartUV[4];
	_vec2		m_vOriOverLastUV[4];

	_float		m_fOverTransparent[4];
	_float		m_fOverKillTime;			// ����ų ���ڰ� �Ҹ��� ���� �ð�
	_float		m_fOverKillGapTime;			// ����ų ���ڰ� �ϳ� �ٲ� �ɸ��� �ð�

	_int		m_iRandNum[4];

	_bool		m_bOverKill;

// font
private:
	_float		m_fFontTransparent;

	Engine::CMaterial*		m_pMtrlComFont;
	Engine::CVIBuffer*		m_pBufferComFont;
	Engine::CTexture*		m_pTextureComFont;

	_matrix		m_matViewFont;			// �޺� �۾��� ���� �� 
	_vec3		m_vPosFont;
	_vec2		m_vSizeFont;

	_vec2		m_vStartUVFont;
	_vec2		m_vLastUVFont;

	Engine::VTXTEX*			m_pVertexFont;
	const TCHAR*			m_pResourceKeyFont;

private:
	void	Set_ConTableOverKill(int iNum);
	void	OverKillFontControl(void);
	void	Set_ConTableOverKillFont(void);
public:
	virtual		_ulong	Release(void);
};

#endif // DamageFont_h__
