#ifndef Functor_h__
#define Functor_h__

class CDelete_Obj
{
public:
	template<typename T>
	void operator () (T& Pointer)
	{
		if(Pointer != NULL)
		{
			delete Pointer;
			Pointer = NULL;
		}
	}
};

class CRelease_Obj
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

#endif // Functor_h__
