/*!
 * \file GraphicDev.cpp
 * \date 2015/02/25 11:45
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev
{
public:
	DECLARE_SINGLETON(CGraphicDev)

public:
	enum WINMODE {MODE_FULL, MODE_WIN};
private:
	explicit CGraphicDev(void);
public:
	~CGraphicDev(void);
public: // getter
	LPDIRECT3DDEVICE9 GetDevice(void) {
		return m_pDevice;}
	LPD3DXLINE			GetLine(void) {
		return m_pLine;}

public:
	HRESULT InitGraphicDev(WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);

private:
	LPDIRECT3D9					m_pSDK;
	LPDIRECT3DDEVICE9			m_pDevice; // 장치를 대표하는 객체다.
	LPD3DXLINE					m_pLine;
	
	LPDIRECTSOUND				m_pSound;
	vector<LPDIRECTSOUNDBUFFER> m_vecSoundBuff;

public:
	HRESULT LoadWave(TCHAR* pFileName);
	void SoundPlay(int iIndex, DWORD dwFlag = 0);
	void SoundStop(int iIndex);
	bool SoundPlaying(int iIndex);
	

private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);
	void Release(void);
};

END


#endif // GraphicDev_h__
