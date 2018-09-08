#include "stdafx.h"
#include "CameraObserver.h"

#include "Export_Function.h"
#include "Include.h"

CCameraObserver::CCameraObserver(void)
: m_pInfoSubject(Engine::Get_InfoSubject())
{

}

CCameraObserver::~CCameraObserver(void)
{

}

const D3DXMATRIX* CCameraObserver::GetView(void)
{
	return &m_matView;
}

const D3DXMATRIX* CCameraObserver::GetProj(void)
{
	return &m_matProj;
}

const D3DXMATRIX* CCameraObserver::GetOrtho(void)
{
	return &m_matOrtho;
}

void CCameraObserver::Update(int message)
{
	Engine::DATALIST*		pDatalist = m_pInfoSubject->GetDatalist(message);

	NULL_CHECK(pDatalist);

	switch(message)
	{
	case D3DTS_VIEW:
		m_matView = *((D3DXMATRIX*)pDatalist->front());
		break;

	case D3DTS_PROJECTION:
		m_matProj = *((D3DXMATRIX*)pDatalist->front());
		break;

	case D3DTS_ORTHOGONAL:
		m_matOrtho = *((D3DXMATRIX*)pDatalist->front());
		break;
	}
}

CCameraObserver* CCameraObserver::Create(void)
{
	return new CCameraObserver;
}