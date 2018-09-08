#include "StdAfx.h"
#include "Obj.h"

map<string, CBitBmp*>* CObj::m_pBitMap = NULL;
CObj::CObj(void)
{
}

CObj::~CObj(void)
{
}
void CObj::SetStrKey(string _strKey)
{
	m_strKey = _strKey;
}

void CObj::SetStageNum(int _iRoomNum)
{
	m_iStageNum = _iRoomNum;
}
void CObj::SetHP(int _iHP)
{
	m_iHP = _iHP;
}
void CObj::SetBitMap(map<string, CBitBmp*>* pBitMap)
{
	m_pBitMap = pBitMap;
}
void CObj::SetIndex(int _iIndex)
{
	m_iIndex = _iIndex;
}
void CObj::SetDrawID(int _iDrawID)
{
	m_iDrawID = _iDrawID;
}
void CObj::SetAngle(float _fAngle)
{
	m_fAngle = _fAngle;
}
void CObj::SetPos(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}
void CObj::SetKey(string _strKey)
{
	m_strKey = _strKey;
}
RECT CObj::GetRect(void)
{
	RECT	rc = {
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fX + m_tInfo.fCX / 2.f),
		int(m_tInfo.fY + m_tInfo.fCY / 2.f)
	};

	return rc;
}
INFO CObj::GetInfo()
{
	return m_tInfo;
}
int	 CObj::GetDrawID()
{
	return m_iDrawID;
}
int	 CObj::GetHP()
{
	return m_iHP;
}
POINT CObj::Rot(float _fX, float _fY, float _fOX, float _fOY, float _fAngle)
{
	POINT	result;

	result.x = LONG(cos(_fAngle) * ( _fX - _fOX ) - sin(_fAngle) * ( _fY - _fOY ) + _fOX);
	result.y = LONG(sin(_fAngle) * ( _fX - _fOX ) + cos(_fAngle) * ( _fY - _fOY ) + _fOY);

	return result;
}