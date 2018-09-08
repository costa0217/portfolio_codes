/*!
 * \file Engine_Function.h
 * \date 2015/11/07 1:24
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Engine_Function 헤더 파일
 *
 * TODO: 엔진에서 자주 사용할 함수들을 템플릿으로 만들어서 모아 높은 헤더 파일
 *
 * \note
*/


#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template <typename T> void Safe_Delete(T& pointer) //인자 값으로 받은 pointer 를 동적 해제 하기 위한 함수
	{
		if(NULL != pointer)
		{
			delete pointer;
			pointer = NULL;
		}
	}

	template <typename T> void Safe_Delete_Array(T& pointer) //인자 값으로 받은 pointer 배열을 동적 해제 하기 위한 함수
	{
		if(NULL != pointer)
		{
			delete [] pointer;
			pointer = NULL;
		}
	}

	// 객체들은 항상 자신의 레퍼런스카운트를 가지고 있는다.
	// Release함수의 리턴값 : 지우려고 시도할 때 참조된갯수(레퍼런스카운트)를 리턴한다.
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