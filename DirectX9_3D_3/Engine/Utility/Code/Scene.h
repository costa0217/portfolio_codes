/*!
 * \file CScene.h
 * \date 2015/07/01 12:39
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

#ifndef CScene_h__
#define CScene_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CLayer;
class CKeyMgr;
class ENGINE_DLL CScene
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CScene(void) = 0;

public:
	const VTXTEX* GetTerrainVtx(const WORD& LayerID, const wstring& wstrObjKey);

public:
	virtual HRESULT InitScene(void);
	virtual void Update(void);
	virtual void Render(void);
	
public:
	static void SetBool(bool byunsoo);

private:
	void Release(void);

protected:
	LPDIRECT3DDEVICE9	m_pDevice;

protected:
	typedef map<WORD, CLayer*>		MAPLAYER;
	MAPLAYER		m_mapLayer;

private:
	CKeyMgr*		m_pKeyMgr;
	static bool		m_bPause;
	bool			m_bRain;
	bool			m_siba;

protected:
	bool			m_bStart;
};

END

#endif // CScene_h__