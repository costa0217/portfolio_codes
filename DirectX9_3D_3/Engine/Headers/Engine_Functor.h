

#ifndef Functor_h__
#define Functor_h__

namespace Engine
{
	class CDelete_Map
	{
	public:
		template<typename T>
		void operator () (T& Pointer)
		{
			if(Pointer.second != NULL)
			{
				delete Pointer.second;
				Pointer.second = NULL;
			}
		}
	};

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

	class CDelete_Com_Release
	{
	public:
		template<typename T>
		void operator () (T& ComPointer)
		{
			if(ComPointer != NULL)
			{
				ComPointer->Release();
				ComPointer = NULL;
			}
		}
	};
}

#endif // Functor_h__