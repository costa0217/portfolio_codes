/*!
 * \file Engine_Function.h
 * \date 2015/11/07 1:24
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Engine_Function ��� ����
 *
 * TODO: �������� ���� ����� �Լ����� ���ø����� ���� ��� ���� ��� ����
 *
 * \note
*/


#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template <typename T> void Safe_Delete(T& pointer) //���� ������ ���� pointer �� ���� ���� �ϱ� ���� �Լ�
	{
		if(NULL != pointer)
		{
			delete pointer;
			pointer = NULL;
		}
	}

	template <typename T> void Safe_Delete_Array(T& pointer) //���� ������ ���� pointer �迭�� ���� ���� �ϱ� ���� �Լ�
	{
		if(NULL != pointer)
		{
			delete [] pointer;
			pointer = NULL;
		}
	}

	// ��ü���� �׻� �ڽ��� ���۷���ī��Ʈ�� ������ �ִ´�.
	// Release�Լ��� ���ϰ� : ������� �õ��� �� �����Ȱ���(���۷���ī��Ʈ)�� �����Ѵ�.
	// 0 != m_pGraphicDev->Release();
	template <typename T> _ulong Safe_Release (T& pointer)
	{
		_ulong	iRefCnt = 0;

		if(NULL != pointer)
		{
			iRefCnt = pointer->Release();

			if(0 == iRefCnt)
				pointer = NULL;
		}

		return iRefCnt;
	}
}
#endif // Engine_Function_h__