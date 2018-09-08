/*!
 * \file Material.h
 * \date 2015/11/07 1:06
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CMaterialŬ������ CComponet�� ��ӹ޾� ������Ʈ�� ����Ͽ�
 ��ü�� �߰��Ͽ� D3DMATERIAL9�� ���� ������ ���ش�.
 *
 * \note
*/

#ifndef Material_h__
#define Material_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMaterial : public Engine::CComponent
{
private:
	explicit CMaterial(void);
	virtual ~CMaterial(void);

public:
	const D3DMATERIAL9* Get_MtrlInfo(void) {
		return &m_Material;}

public:
	HRESULT Initialize(const D3DMATERIAL9& Material);

public:
	static CMaterial* Create(const D3DMATERIAL9& Material);

private:
	D3DMATERIAL9	m_Material;

public:
	virtual _ulong Release(void);
};

END

#endif // Material_h__