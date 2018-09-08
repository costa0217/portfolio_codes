/*!
 * \file Base.h
 * \date 2015/11/07 0:56
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 싱글톤을 제외한 게임내에 구현하는 거의 대부분의 클래스들의 부모가 될 class.
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Base_h__
#define Base_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CBase
{
protected: //CBase의 생성자와 소멸자는 자식 클래스에서 호출 해야 되서 protected로 선언한다.
	explicit CBase(void);
	virtual ~CBase(void);
public:
	void Add_Ref(void);
protected:
	_ulong	m_dwRefCnt;	//레퍼런스 카운팅(참조숫자)
public:
	virtual _ulong Release(void);
};

END

#endif // Base_h__