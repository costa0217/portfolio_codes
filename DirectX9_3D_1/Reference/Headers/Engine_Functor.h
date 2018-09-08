/*!
 * \file Engine_Functor.h
 * \date 2015/11/07 1:24
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Engine_Functor 헤더파일
 *
 * TODO: 엔진에서 사용하는 함수 객체들을 모아 놓은 헤더 파일
 *
 * \note
*/

#ifndef Engine_Functor_h__
#define Engine_Functor_h__

namespace Engine
{
	class CTagFinder	//맵컨테이너에서 find_if로 원하는 태크의 맵을 찾을때 사용
	{
	public:
		explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
		~CTagFinder(void) {}
	public:
		template <typename T> bool operator () (T& Pair)
		{
			int iResult = lstrcmp(m_pTag, Pair.first); //lstrcmp 함수는 유니코드, 멀티바이트를 고려하지 않고 문자열을 비교한다.
			
			if(0 == iResult) //리턴 값이 0이면 두 문자열은 같으며 음수이면 왼쪽 문자열이 더 작은것이고 양수이면 왼쪽 문자열이 더 큰 문자열 이다.
				return true;

			return false;
		}
	private:
		const TCHAR*	m_pTag;	//함수 객체를 생성자로 생성할때 받은 인자값으로 초기화하여 () 함수 호출을 통해 비교하기 위해서 선언
	};

	class CRelease_Pair	//맵컨테이너에서 for_each로 맵들의 Release함수를 호출하고 NULL값으로 채워서 지워주기 위한 함수 객체
	{
	public:
		explicit CRelease_Pair() {}
		~CRelease_Pair(void) {}
	public:
		template <typename T>
		void operator () (T& Pair)
		{
			if(NULL != Pair.second)
			{
				Pair.second->Release();	//Release() 함수 호출
				Pair.second = NULL;		//NULL값으로 채워줌
			}
		}
	};

	class CRelease_Single //싱글톤으로 된 매니저들에서 리스트들의 값들을 지워주기 위해 선언된 함수 객체
	{
	public:
		explicit CRelease_Single() {}
		~CRelease_Single(void) {}
	public:
		template <typename T>
		void operator () (T& Pointer)
		{
			if(NULL != Pointer)
			{
				Pointer->Release();	//Relaease() 함수 호출
				Pointer = NULL;		//NULL값으로 채워줌
			}
		}
	};

	class CRelease_Obj	//for_each에서 함수객체로 넘겨 해당 오브젝트를 지워주는 함수
	{
	public:
		template <typename T>
		_ulong operator() (T& pointer)
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
	};
}
#endif // Engine_Functor_h__