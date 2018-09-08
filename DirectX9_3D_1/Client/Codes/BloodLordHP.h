#ifndef BloodLordHP_h__
#define BloodLordHP_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CMonster;
class CBloodLordHP : public Engine::CGameObject
{
	DECLARE_SINGLETON(CBloodLordHP)
private:
	explicit CBloodLordHP(void);
	virtual ~CBloodLordHP(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public://getter, setter
	void SetpMonster(CMonster*	pMonster);
	void SetRender(_bool SetBool) { m_bRender = SetBool; }

	enum MONSTERSTATE { HPBar, HPBack, HPFront, HPEND };


private:
	CMonster*				m_pMonster;

	LPD3DXEFFECT			m_pEffect[2];
	_matrix					m_matOrtho;

	//�������� ����
	_float					m_fSizeX[2];
	_float					m_fSizeY[2];
	_float					m_fX;
	_float					m_fY;

	Engine::CTransform*		m_pTransCom;
	Engine::CVIBuffer*		m_pBufferCom[HPEND];
	Engine::CTexture*		m_pTextureCom[HPEND];
	Engine::VTXTEX*			m_pVTXInfo;

	//�̸�, ���ϱ�
	Engine::CVIBuffer*		m_pNameBufferCom;
	Engine::CVIBuffer*		m_pMulBufferCom;
	Engine::CTexture*		m_pNameTextureCom;
	Engine::CTexture*		m_pMulTextureCom;

	_int					m_iNameNumber;	//�������� �̱������� ���鶧 �̺����� ���� �̸��� �ٲ��ش�.

	_int					m_iHpNumber;
	_float					m_fDivision;
	_float					m_fBossMaxHp[7];
	_float					m_fBossHp[7];

	_bool					m_bBarRender;	//�ٸ� �׸��� ����
	_float					m_fAlpha;		//�� ���İ�
	_bool					m_bAlphaCheck;

	_bool					m_bRender;		//UI ��ü�� �������� ���� ����

public:
	virtual _ulong Release(void);

};
#endif 
