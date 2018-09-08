#pragma once
#include "DXObject.h"

enum eBufferUsage {
	USAGE_CONSTANT	= 0,
	USAGE_INSTANCING
};
class CBuffer : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	//map
	void* Map();
	void Unmap();
	//unmap

	//setter
	void SetBufferStride(UINT nBufferStride) { m_BufferStride = nBufferStride; }
	void SetnObject(UINT nObject) { m_nObject = nObject; }
	void SetOffset(UINT nOffset) { m_Offset = nOffset; }
	void SetBuffer(ID3D11Buffer* pd3dBuffer) { m_pd3dBuffer = pd3dBuffer; }
	void SetSlot(UINT Slot) { m_Slot = Slot; }
	void SetBindFlag(UINT BindFlag) { m_BindFlag = BindFlag; }
	//getter
	UINT GetBufferStride() { return m_BufferStride; }
	UINT GetnObject() { return m_nObject; }
	UINT GetOffset() { return m_Offset; }
	ID3D11Buffer* GetBuffer() { return m_pd3dBuffer; }
	UINT GetSlot() { return m_Slot; }
	UINT GetBindFlag() { return m_BindFlag; }

	static  CBuffer* CreateConstantBuffer(UINT nObject, UINT BufferStride, UINT Slot = 0, UINT BindFlag = BIND_VS, UINT Offset = 0);
	static  CBuffer* CreateInstancingBuffer(UINT nObject, UINT BufferStride, UINT Offset = 0);
private:
	static ID3D11Buffer* CreateConstantBuffer(UINT nByteWidth);
	static ID3D11Buffer* CreateInstancingBuffer(UINT nByteWidth);

protected:

	//buffer 1��
	ID3D11Buffer*					m_pd3dBuffer{ nullptr };

	//map data 1��
	D3D11_MAPPED_SUBRESOURCE		m_MappedData{ nullptr };
	void*							m_pData{ nullptr };

	//buffe info 1��
	UINT							m_nObject{ 0 };
	UINT							m_BufferStride{ 0 };
	UINT							m_ByteWidth{ 0 };
	UINT							m_Offset{ 0 };
	UINT							m_Slot{ 0 };
	UINT							m_BindFlag{ 0 };

public:
	CBuffer();
	virtual ~CBuffer();

};

/*
//���۰� �־�����
if (m_ppBuffers) {
//���� ���۵� release

//���ο� ���� �迭 ����
new_ppBuffers = new  CBuffer>[m_nBuffers];
//���� �� ����
int i = 0;
for (i; i < m_nBuffers; ++i) {
new_ppBuffers[i] = m_ppBuffers[i];
}
//���� ���� �� ����
int j;
for (int j = 0; j < nBuffers; ++j) {
new_ppBuffers[m_nBuffers + j] = ppBuffers[j];
}
//���� ���� �����
delete[] m_ppBuffers;

//�� ����
m_ppBuffers = new_ppBuffers;
m_nBuffers = new_nBuffers;
}
else {
//�� ����
m_ppBuffers = ppBuffers;
m_nBuffers = nBuffers;
}
*/