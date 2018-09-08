#ifndef ObjDataMgr_h__
#define ObjDataMgr_h__

#include "Engine_Include.h"

#include "VIBuffer.h"

namespace Engine
{
	class CResources;

}
class CObjDataMgr
{
public:
	DECLARE_SINGLETON(CObjDataMgr)

public:
	CObjDataMgr(void);
	~CObjDataMgr(void);

public:
	HRESULT	LoadObjectData(LPDIRECT3DDEVICE9 pDevice
		, wstring wstrDataPath
		, Engine::RESOURCETYPE eResourceType 
		, Engine::CVIBuffer::BUFFERTYPE eBufferType 
		, const wstring& wstrResourceKey);

public:
	HRESULT LoadObjectPositionFromFile(wstring wstrPath);

private:
	void MakeObject(wstring wstrObjectName, D3DXVECTOR3 vPosition);

private:
	void Release(void);

public:
	map<wstring, vector<D3DXVECTOR3>>		m_mapObjPos;
	map<wstring, vector<D3DXVECTOR3>>*		 GetMapObPos(void); // 0812


};
#endif // ObjDataMgr_h__
