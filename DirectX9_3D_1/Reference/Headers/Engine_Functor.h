/*!
 * \file Engine_Functor.h
 * \date 2015/11/07 1:24
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Engine_Functor �������
 *
 * TODO: �������� ����ϴ� �Լ� ��ü���� ��� ���� ��� ����
 *
 * \note
*/

#ifndef Engine_Functor_h__
#define Engine_Functor_h__

namespace Engine
{
	class CTagFinder	//�������̳ʿ��� find_if�� ���ϴ� ��ũ�� ���� ã���� ���
	{
	public:
		explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
		~CTagFinder(void) {}
	public:
		template <typename T> bool operator () (T& Pair)
		{
			int iResult = lstrcmp(m_pTag, Pair.first); //lstrcmp �Լ��� �����ڵ�, ��Ƽ����Ʈ�� ������� �ʰ� ���ڿ��� ���Ѵ�.
			
			if(0 == iResult) //���� ���� 0�̸� �� ���ڿ��� ������ �����̸� ���� ���ڿ��� �� �������̰� ����̸� ���� ���ڿ��� �� ū ���ڿ� �̴�.
				return true;

			return false;
		}
	private:
		const TCHAR*	m_pTag;	//�Լ� ��ü�� �����ڷ� �����Ҷ� ���� ���ڰ����� �ʱ�ȭ�Ͽ� () �Լ� ȣ���� ���� ���ϱ� ���ؼ� ����
	};

	class CRelease_Pair	//�������̳ʿ��� for_each�� �ʵ��� Release�Լ��� ȣ���ϰ� NULL������ ä���� �����ֱ� ���� �Լ� ��ü
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
				Pair.second->Release();	//Release() �Լ� ȣ��
				Pair.second = NULL;		//NULL������ ä����
			}
		}
	};

	class CRelease_Single //�̱������� �� �Ŵ����鿡�� ����Ʈ���� ������ �����ֱ� ���� ����� �Լ� ��ü
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
				Pointer->Release();	//Relaease() �Լ� ȣ��
				Pointer = NULL;		//NULL������ ä����
			}
		}
	};

	class CRelease_Obj	//for_each���� �Լ���ü�� �Ѱ� �ش� ������Ʈ�� �����ִ� �Լ�
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