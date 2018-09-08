/*!
 * \file CameraObserver.h
 * \date 2015/07/13 13:35
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef CameraObserver_h__
#define CameraObserver_h__

#include "Observer.h"

namespace Engine
{
	class CInfoSubject;
}

class CCameraObserver
	: public Engine::CObserver
{
private:
	CCameraObserver(void);

public:
	virtual ~CCameraObserver(void);

public:
	const D3DXMATRIX* GetView(void);
	const D3DXMATRIX* GetProj(void);
	const D3DXMATRIX* GetOrtho(void);

public:
	virtual void Update(int message);

public:
	static CCameraObserver* Create(void);

private:
	Engine::CInfoSubject*		m_pInfoSubject;

private:
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matProj;
	D3DXMATRIX		m_matOrtho;
};

#endif // CameraObserver_h__