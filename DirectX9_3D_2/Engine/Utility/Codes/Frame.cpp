#include "Frame.h"

USING(Engine)

Engine::CFrame::CFrame(void)
{

}
Engine::CFrame::~CFrame(void)
{

}

HRESULT Engine::CFrame::Initialize(_int CntX, _int CntY, _float fFrameSpeed /*= 1.f*/)
{
	m_iCntX	= CntX;
	m_iCntY	= CntY;

	m_fFrameX = 0.f;
	m_iFrameHeight = 0;
	
	m_fFrameSpeed = fFrameSpeed;
	m_bEnd	= false;
	
	return S_OK;
}

_int Engine::CFrame::Update(const _float& fTimeDelta)
{
	m_fFrameX += fTimeDelta * m_fFrameSpeed;
	m_bEnd = false;
	if(int(m_fFrameX) >= m_iCntX)
	{
		++m_iFrameHeight;
		if(m_iFrameHeight >= m_iCntY)
		{
			m_iFrameHeight = 0;
			m_bEnd = true;
		}
		m_fFrameX= 0.f;
	}
	return 0;
}

CFrame* Engine::CFrame::Create()
{
	CFrame*	pFrame = new CFrame;

	return pFrame;

}


_ulong Engine::CFrame::Release()
{
	_ulong dwRefCnt = Engine::CComponent::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}
	return dwRefCnt;
}