/*!
 * \file Base.h
 * \date 2015/11/07 0:56
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief �̱����� ������ ���ӳ��� �����ϴ� ���� ��κ��� Ŭ�������� �θ� �� class.
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
protected: //CBase�� �����ڿ� �Ҹ��ڴ� �ڽ� Ŭ�������� ȣ�� �ؾ� �Ǽ� protected�� �����Ѵ�.
	explicit CBase(void);
	virtual ~CBase(void);
public:
	void Add_Ref(void);
protected:
	_ulong	m_dwRefCnt;	//���۷��� ī����(��������)
public:
	virtual _ulong Release(void);
};

END

#endif // Base_h__