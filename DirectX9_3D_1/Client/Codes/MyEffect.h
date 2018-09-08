#ifndef MyEffect_h__
#define MyEffect_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CFrameAni;
}

class CMyEffect	: public Engine::CGameObject
{
private:
	explicit CMyEffect(LPDIRECT3DDEVICE9 pGraphicDev
		, WORD wIdx
		, TCHAR* pName
		, POINT ptXY
		, float fFrameSpeed);

	virtual ~CMyEffect(void);

public:
	explicit CMyEffect(CMyEffect& Instance);

public:
	_float 	Get_PosZ(void);

public:
	virtual HRESULT			Initialize(void);
	virtual HRESULT			Add_Component(void);
	virtual _int					Update(const _float& fTimeDelta);
	virtual void					Render(void);

public:
	static		CMyEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, WORD wIdx
		, TCHAR* pName
		, POINT ptXY
		, float fFrameSpeed);
	static		CMyEffect*	CloneCreate(CMyEffect& Instance);

private:
	Engine::CTransform*		m_pTransCom;
	Engine::CVIBuffer*		m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CFrameAni*		m_pFrameCom;

public:
	TCHAR					m_wstrTexName[MAX_PATH];
	WORD					m_wIdx;
	POINT					m_ptCntXY;
	float						m_fFrameSpeed;
	int							m_iPass;


private:
	LPD3DXEFFECT		m_pEffect;
	_bool						m_bSoundPlay;
	_vec3					m_vStartPos;
	TCHAR*					m_pStartName;

	_int						m_iVecLinearSize;

	_vec3					m_vAccMovePos;
public:
	vector<Engine::POINTLINEAR*>		m_VecLinear;	//리스트로하고 성공하면바꾸자
	//vector<Engine::POINTLINEAR*>		m_OriVecLinear;

	_vec3		m_vPos, m_vAngle, m_vSize;
	_vec3		m_vNoMovePos;
	_float			m_fColorDown;
	//list<Engine::POINTLINEAR*>			m_LinearList;//


private:
	void			Set_ContantTable(void);
	void			Adjust_BillBoard(void);

private:
	_vec3		m_vViewPos;

	ID3DXMesh*				m_pSphereMesh;		// 경계구 그리기용 메쉬 & 버퍼
	LPD3DXBUFFER				m_pSphereBuffer;


	_float			m_fEffectRadius;	//컬링을 하기 위한 이펙트의 반지름

public:
	_bool			m_bBillBoard;
	_float			m_fAccAniTime;
	_int			m_iAniCnt;
	_bool			m_bRender;
	_vec4		m_vColor;
	_bool			m_bReStart;

	_float			m_fRevolAngle;	//공전

	_float			m_fDeleteTime;


	_bool			m_bAbleEffect;		// 실행중
	_bool			m_bHDR;

	_bool			m_bRefeat;
	_float			m_fRefeatTime;
	_vec3*		m_pvecParent;

	_bool			m_bStop;

	_bool			m_bPost;
	_float			m_fStartTime;

	_bool		m_bDistortion;

public:
	void			Setting_Init(_vec3 vPos, _vec3 vSize, _vec3 vAngle, _float fColorDown, _float fRevolAngle, _bool bRefeat, _float fRefeatTime, _vec3* pvecParent );

	void			Set_Pos( _vec3 vOriPos, _vec3 vPos);
	void			Set_OriPos();
	void			Set_Angle(_vec3 vAngle);
	void			Set_Size(_vec3 vSize);
	void			Set_ColorDown(float fColorDown);


	void			Set_Data(_vec3 vPos, _vec3 vAngle, _vec3 vSize, _float fColorDown);

	void			Set_ReStart(_bool	bReStart);
	void			Set_MaxScale(void);
	void			Set_Name(TCHAR* pName) { m_pStartName = pName; }
	void			Set_NoTesting(void);
	void			Set_StartTime(_float vStartTime);

	void			Play_Animation(const _float& fTimeDelta);

public:
	void			Copy_Point(CMyEffect* pEffect);

public:
	virtual _ulong Release(void);
};

#endif // MyEffect_h__
