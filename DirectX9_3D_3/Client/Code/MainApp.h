/*!
 * \file MainApp.h
 * \date 2015/07/20 16:17
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

#ifndef MainApp_h__
#define MainApp_h__

namespace Engine
{
	class CGraphicDev;
	class CManagement;
	class CTimeMgr;
	class CKeyMgr;
}

class CMainApp
{
public:
	HRESULT Initialize(void);
	void Update(void);
	void Render(void);
	void Release(void);
public:
	static CMainApp* Create(void);
private:
	CMainApp(void);
public:

	~CMainApp(void);
private:
	Engine::CGraphicDev*		m_pGraphicDev;
	Engine::CManagement*		m_pManagement;
	Engine::CTimeMgr*			m_pTimeMgr;
	Engine::CKeyMgr*		    m_pKeyMgr;
private:
	LPDIRECT3DDEVICE9	m_pDevice;
};

#endif // MainApp_h__