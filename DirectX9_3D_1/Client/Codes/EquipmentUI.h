#ifndef EquipmentUI_h__
#define EquipmentUI_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CEquipmentUI : public Engine::CGameObject
{
	DECLARE_SINGLETON(CEquipmentUI)
private:
	explicit CEquipmentUI(void);
	virtual ~CEquipmentUI(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public://getter, setter
	void RenderEquipmentUI(int iNumber)
	{ 
		m_iTextureNumber	= iNumber; //0��� 1���� 2�尩 3���� 4�Ź�
		m_bRender			= true;
		m_bAlphaCheck	= false;
		m_fAlpha		= 1.f;
	}

private:
	LPD3DXEFFECT			m_pEffect;
	_matrix					m_matOrtho;

	_float					m_fSizeX;
	_float					m_fSizeY;
	_float					m_fX;
	_float					m_fY;

	Engine::CTransform*		m_pTransCom;
	Engine::CVIBuffer*		m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

	_bool					m_bRender;		//UI ��ü�� �������� ���� ����

	_bool					m_bAlphaCheck;
	_float					m_fAlpha;

	_int					m_iTextureNumber;

public:
	virtual _ulong Release(void);

};
#endif 
