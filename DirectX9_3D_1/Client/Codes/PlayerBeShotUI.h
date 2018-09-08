#ifndef PlayerBeShotUI_h__
#define PlayerBeShotUI_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CPlayerBeShotUI : public Engine::CGameObject
{
	DECLARE_SINGLETON(CPlayerBeShotUI)
private:
	explicit CPlayerBeShotUI(void);
	virtual ~CPlayerBeShotUI(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	void PlayerBeShotUI(void);

private:
	LPD3DXEFFECT			m_pEffect;
	_matrix					m_matOrtho;

	//�ǰ��� �ؽ��İ� �� 15���̴� 15���� �迭�� �����Ͽ� �Ѵ�.
	_float					m_fSizeX[15];
	_float					m_fSizeY[15];
	_float					m_fX[15];
	_float					m_fY[15];

	Engine::CTransform*		m_pTransCom;
	Engine::CVIBuffer*		m_pBufferCom[15];
	Engine::CTexture*		m_pTextureCom;
	Engine::VTXTEX*			m_pVTXInfo[15];

	//�׸��� ���� ����
	_bool					m_bSelect[15];	//�׸��� �ȱ۸���
	_float					m_fAlpha[15];	//�ش� �ؽ��� ���İ�
	
	_float					m_fMaxTime[15];
	_float					m_fTime[15];

	_float					m_fMaxSize[15];
	_float					m_fSize[15];

	_float					m_fSpeed[15];

public:
	virtual _ulong Release(void);

};
#endif 
