/*!
 * \file Subject.h
 * \date 2015/07/13 13:02
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
#ifndef Subject_h__
#define Subject_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CObserver;

class ENGINE_DLL CSubject
{
protected:
	CSubject(void);
	virtual ~CSubject(void);

public:
	virtual void Subscribe(CObserver* pObserver);
	virtual void UnSubscribe(CObserver* pObserver);
	virtual void Notify(int message);

private:
	void Release(void);

protected:
	typedef list<CObserver*>		OBSERVERLIST;
	OBSERVERLIST		m_Observerlist;
};

END

#endif // Subject_h__