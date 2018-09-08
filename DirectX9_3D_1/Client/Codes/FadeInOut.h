#ifndef FadeInOut_h__
#define FadeInOut_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CPlayer;
class CFadeInOut : public Engine::CGameObject
{
	DECLARE_SINGLETON(CFadeInOut)
private:
	explicit CFadeInOut(void);
	virtual ~CFadeInOut(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public://getter, setter
	void SetFadeInOut(_bool bStart, _float SetSpeed)
	{
		//�ε�����		���� => ������
		//�������� �����Ҷ��� ���� ���¿��� ����������.
		//ī�޶� �۵��Ҷ��� ���� => ������ => ����
		//ī�޶� ������	���� => ������ => ����

		//true�̸� ���������� �����ؼ� ����������.
		if(bStart)
		{
			m_bRender		= true;		//���̵� �� �ƿ� ����
			m_fSpeed		= SetSpeed;	//�ӵ� ���� 1.f �ϸ� �� 2�� ������
			m_fAlpha		= 1.f;		//���� ���İ� ����
			m_bAlphaCheck	= false;	//���� üũ ����
		}
		else //false �̸� ������ �����ؼ� ���������� ���� ��������
		{
			m_bRender		= true;		//���̵� �� �ƿ� ����
			m_fSpeed		= SetSpeed;	//�ӵ� ���� 1.f �ϸ� �� 2�� ������
			m_fAlpha		= 0.f;		//���� ���İ� ����
			m_bAlphaCheck	= true;	//���� üũ ����
		}
	}

	_bool	GetRender(void) { return m_bRender; }

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
	_float					m_fSpeed;		//���ǵ带 ��������� ���� �����Ѵ�.
	_bool					m_bAlphaCheck;
	_float					m_fAlpha;
public:
	virtual _ulong Release(void);

};
#endif 
