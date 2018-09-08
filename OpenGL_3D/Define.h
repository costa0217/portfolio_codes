#pragma once

#define NO_COPY(CLASSNAME)							\
	private:										\
		CLASSNAME(const CLASSNAME&);				\
		CLASSNAME& operator =(const CLASSNAME&);	\


#define DECLARE_SINGLETON(CLASSNAME)				\
		NO_COPY(CLASSNAME)							\
		private:									\
			static CLASSNAME*			pInstance;	\
		public:										\
			static CLASSNAME*	GetInstance(void);	\
			static void DestroyInstance(void);		\

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME* CLASSNAME::pInstance = NULL;			\
	CLASSNAME* CLASSNAME::GetInstance(void){		\
	if(pInstance == NULL){							\
		pInstance = new CLASSNAME;}					\
		return pInstance;  }						\
		void CLASSNAME::DestroyInstance(void){		\
		if(pInstance != NULL){						\
			delete pInstance;						\
			pInstance = NULL;}						\
			}

#define COLOR_RED		MyVector(1.f, 0.f, 0.f)
#define COLOR_BLUE		MyVector(0.f, 0.f, 1.f)
#define COLOR_GREEN		MyVector(0.f, 1.f, 0.f)
#define COLOR_CYAN		MyVector(0.f, 1.f, 1.f)
#define COLOR_YELLOW	MyVector(1.f, 1.f, 0.f)
#define COLOR_MAGENTA	MyVector(1.f, 0.f, 1.f)
#define COLOR_WHITE		MyVector(1.f, 1.f, 1.f)
#define COLOR_BLACK		MyVector(0.f, 0.f, 0.f)