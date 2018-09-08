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
		//로딩들어갈때		투명 => 검은색
		//스테이지 시작할때는 검은 상태에서 투명해진다.
		//카메라 작동할때는 투명 => 검은색 => 투명
		//카메라가 끝나면	투명 => 검은색 => 투명

		//true이면 검은색에서 시작해서 투명해진다.
		if(bStart)
		{
			m_bRender		= true;		//페이드 인 아웃 시작
			m_fSpeed		= SetSpeed;	//속도 조절 1.f 하면 약 2초 정도임
			m_fAlpha		= 1.f;		//시작 알파값 조절
			m_bAlphaCheck	= false;	//알파 체크 설정
		}
		else //false 이면 투명에서 시작해서 검은색으로 된후 투명해짐
		{
			m_bRender		= true;		//페이드 인 아웃 시작
			m_fSpeed		= SetSpeed;	//속도 조절 1.f 하면 약 2초 정도임
			m_fAlpha		= 0.f;		//시작 알파값 조절
			m_bAlphaCheck	= true;	//알파 체크 설정
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

	_bool					m_bRender;		//UI 전체를 랜더할지 말지 결정
	_float					m_fSpeed;		//스피드를 어는정도로 할지 설정한다.
	_bool					m_bAlphaCheck;
	_float					m_fAlpha;
public:
	virtual _ulong Release(void);

};
#endif 
