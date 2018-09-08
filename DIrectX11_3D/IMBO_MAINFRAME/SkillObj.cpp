#include "stdafx.h"
#include "SkillObj.h"


CSkillObj::CSkillObj()
{
}


CSkillObj::~CSkillObj()
{
}

void CSkillObj::SetName(CString name)
{
	_tcscpy_s(m_szName, _countof(m_szName), name);
}
