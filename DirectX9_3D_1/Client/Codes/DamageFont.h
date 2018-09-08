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

// 목표 1 : 데미지와 Pos를 받으면 그 값에 따라 데미지를 적절한 위치에 띄운다.
// 목표 2 : 몬스터의 체력도 고려해서 데미지가 체력을 넘기면 
//			만큼의 OverKill 데미지 폰트도 넘겨준다.
// 목표 3 : 5000 ~ 9999의 데미지 범위로 띄운다.

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

// enum값 모음
private:
	enum DAMAGE_UPDATE	 { DAMAGE_INIT, DAMAGE_WAVE, DAMAGE_DEL, DAMAGE_END };
	enum OVERKILL_UPDATE { OVERKILL_INIT, OVERSKILL_STOP, OVERSKILL_DEL , OVERSKILL_END };

private:
	DFONT_FONT_TYPE		m_eDfontType;		// 폰트의 종류
	DFONT_OBJ_TYPE		m_eDfontState;		// 데미지 폰트의 상태

	DAMAGE_UPDATE		m_eDamage;
	OVERKILL_UPDATE		m_eOverkill;

// 공통 변수
private:
	_float			m_fTimeDelta;
	_float			m_fOverlapTime;		// 중복 입력 방지용 시간
	_bool			m_bStart;
	_matrix			m_matOrtho;
	_matrix			m_matView[4];		// 최대 4자리 수를 고려
	_float			m_fDelTime;			// 소멸되기까지의 시간 

	int m_iOne;
	int m_iTen;
	int m_iHundred;
	int m_iThousand;

	_vec3			m_iRandPos;			// 랜덤한 위치 지정을 위한 변수
	_bool			m_bDelete;

private:
	Engine::CTransform*		m_pTransCom;
	Engine::CMaterial*		m_pMtrlCom;
	Engine::CVIBuffer*		m_pBufferCom[4];
	Engine::CTexture*		m_pTextureCom;

	Engine::VTXTEX*			m_pVertex[4];
	Engine::VTXTEX*			m_pOriginVertex;
	

// 외부 변수 저장
private:
	_vec3		m_vPos;
	_int		m_iDamage;

// Damage Number
// 1. 크리티컬 데미지일 경우 
// 2. 일반 데미지일 경우
// 3. 내가 적에게 피격당할 경우
// >> 색깔만 바꿔주면 된다.
// 데미지가 뜰경우 플레이어와 몬스터의 x,y 값은 맞춰주고
// z값은 거리 비율을 재서 Size를 조절하는 방식으로 한다.
// 처음 데미지가 뜰때 살짝 상하 진동 느낌으로 한두번 움직이다 Y값으로 올라가면서
// Size값은 커지고 투명값은 작아지며 사라진다.
// 버퍼는 4개로 충분
// 투명값이 0이 되면 데미지 폰트를 지워주는걸로
private:
	_vec3		m_vDamPos[4];
	_vec2		m_vDamSize[4];
	_vec2		m_vDamStartUV[4];
	_vec2		m_vDamLastUV[4];

	_float		m_fDamTransparent[4];
	_int		m_iWaveCnt[4];
	_float		m_fWaveTime[4];		// Wave 타임
	_bool		m_bWaveDir[4];		// false일때 아래 , true일때 위로 웨이브
	_float		m_fPosGap[4];		// 문자 사이들의 갭

private:
	void	Set_ConTableDamage(int iNum);
	void	DamageFontControl(void);

// OverKill Number
// 1. 몬스터 체력을 넘는 데미지가 있으면 그 넘은 만큼의 값을 따로 저장한다.
// 2. 잠시동안 랜덤한 숫자로 빠르게 바꾸다 본래 넘은 만큼의 값으로 바꾼다.
// 3. 사라질때는 위치변화는 없이 그냥 Size만 키워주면서 투명하게 만들고 삭제시킨다.
private:
	_vec3		m_vOverPos[4];
	_vec2		m_vOverSize[4];
	
	_vec2		m_vOverStartUV[4];
	_vec2		m_vOverLastUV[4];
	_vec2		m_vOriOverStartUV[4];
	_vec2		m_vOriOverLastUV[4];

	_float		m_fOverTransparent[4];
	_float		m_fOverKillTime;			// 오버킬 숫자가 촤르륵 도는 시간
	_float		m_fOverKillGapTime;			// 오버킬 숫자가 하나 바뀔때 걸리는 시간

	_int		m_iRandNum[4];

	_bool		m_bOverKill;

// font
private:
	_float		m_fFontTransparent;

	Engine::CMaterial*		m_pMtrlComFont;
	Engine::CVIBuffer*		m_pBufferComFont;
	Engine::CTexture*		m_pTextureComFont;

	_matrix		m_matViewFont;			// 콤보 글씨를 위한 뷰 
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
