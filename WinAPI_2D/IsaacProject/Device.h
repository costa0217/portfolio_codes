#pragma once


// 사운드 관련 소스 확인 및 분석 해야할 곳

// 1. stdafx.h의 헤더파일 추가 부분
// 2. maingame 클래스 Initialize 및 Release 함수
// 3. Stage 클래스 Initialize 및 Progress 함수 
// 4. 프로젝트 폴더(하부 폴더로 Texture 폴더처럼 Sound폴더가 있는 지 확인)
// 5. 더불어서 wav파일만 지원되는 형식임. 
// 6. 혹여 집에서 작업하다가 안된다면 directx sdk의 문제임, 미 설치 시 인터넷에서 
//	  다운받아 필수적으로 설치해야 됨. june2010버전이 있고, 설치 후 끝다는 것이 아님.
//	  자세한 설치과정은 인터넷에 검색하면 나올 것임. 도저히 모르겠으면 수업시간에 묻도록.
//	  전화나 문자로 하기엔 너무 긴 내용임.



class CDevice
{
private:
	//directx sdk (마이크로소프트에서 컴퓨터에있는 하드웨어를 쉽게 상요할수 있게
	// 여러가지 함수와, 데이터형을 제공해놓은 헤더파일과 라이브러리파일의 집합.)
	//다이렉트 사운드 인터페이스
	LPDIRECTSOUND				m_pSound; //사운드카드를 대표하는 객체.
	vector<LPDIRECTSOUNDBUFFER> m_vecSoundBuff;	//사운드파일을 로드해서 저장할 벡터

public:
	HRESULT Init(void);	// 장치를 초기화 하는역활.
	HRESULT LoadWave(TCHAR* pFileName);	//파일경로를 통해 파일을 읽어 드리겠다.
	void SoundPlay(int iIndex /*몇번째 파일을 재생할래?*/, DWORD dwFlag/*무한루프, 한번*/); //파일을 재생 하겠다.
	void SoundStop(int iIndex);
	bool SoundPlaying(int iIndex);	// 현재 iIndex번째 사운드가 재생되고있는지 아닌지.

private:
	static CDevice* m_pInstance;

public:
	static inline CDevice* GetInstance(void)
	{
		if(!m_pInstance)
			m_pInstance = new CDevice;

		return m_pInstance;
	}
public:
	inline void DestroyInst(void)
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

private:
	CDevice(void);
	~CDevice(void);
};
