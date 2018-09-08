#include "StdAfx.h"
#include "Stage3.h"

CStage3::CStage3(void)
{
}

CStage3::~CStage3(void)
{
	Release();
}
HRESULT CStage3::Initialize(void)
{
	return S_OK;
}

int CStage3::Progress(void)
{
	return 0;
}
void CStage3::Render(void)
{
}
void CStage3::Release(void)
{
}
