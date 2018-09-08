#include "StdAfx.h"
#include "Obj.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

D3DXVECTOR3 CObj::m_vScroll;

CObj::CObj(void)
: m_wstrObjKey(L"")
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_tInfo.vLook = D3DXVECTOR3(1.f, 0.f, 0.f);
	m_fTheta	= 0.f;
	m_fSpeed	= 0.f;
	//m_bStart	  = false;
}

CObj::~CObj(void)
{
}

const wstring& CObj::GetObjKey( void )
{
	return m_wstrObjKey;
}
/* 이미지 돌리기 */
void CObj::SetFrame(const wstring& wstrStateKey/*, bool& _bStart*/)
{

	m_wstrStateKey = wstrStateKey;	

	m_tFrame.fFrame = 0.f;
	m_tFrame.fCount = (FLOAT)CTextureMgr::GetInstance()->GetImageCount(GetObjKey(), wstrStateKey);
	m_tFrame.fMax	= m_tFrame.fCount;

}

bool CObj::FrameMove(float _fSpeed)
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME * _fSpeed;

	if(m_tFrame.fFrame >= m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0.f;
		return true;
	}
	return false;

}