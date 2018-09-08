#include "stdafx.h"
#include "Buffer.h"


bool CBuffer::Begin(){

	return true;
}

bool CBuffer::End(){
	m_pd3dBuffer->Release();
	return false;
}

void CBuffer::SetShaderState(){
	ID3D11Buffer* pBuffers[1] = { m_pd3dBuffer };

	if (m_BindFlag & BIND_VS) {
		GLOBALVALUEMGR->GetDeviceContext()->VSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_DS) {
		GLOBALVALUEMGR->GetDeviceContext()->DSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_HS) {
		GLOBALVALUEMGR->GetDeviceContext()->HSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_GS) {
		GLOBALVALUEMGR->GetDeviceContext()->GSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_PS) {
		GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_CS) {
		GLOBALVALUEMGR->GetDeviceContext()->CSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
}

void CBuffer::CleanShaderState(){

	ID3D11Buffer* pBuffers[1] = { nullptr };

	if (m_BindFlag & BIND_VS) {
		GLOBALVALUEMGR->GetDeviceContext()->VSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_DS) {
		GLOBALVALUEMGR->GetDeviceContext()->DSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_HS) {
		GLOBALVALUEMGR->GetDeviceContext()->HSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_GS) {
		GLOBALVALUEMGR->GetDeviceContext()->GSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_PS) {
		GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
	if (m_BindFlag & BIND_CS) {
		GLOBALVALUEMGR->GetDeviceContext()->CSSetConstantBuffers(m_Slot, 1, pBuffers);
	}
}

void CBuffer::UpdateShaderState(){

}

void* CBuffer::Map() {
	GLOBALVALUEMGR->GetDeviceContext()->Map(m_pd3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_MappedData);
	m_pData = m_MappedData.pData;

	return m_pData;
}

void CBuffer::Unmap() {
	GLOBALVALUEMGR->GetDeviceContext()->Unmap(m_pd3dBuffer, 0);
}

 CBuffer* CBuffer::CreateConstantBuffer(UINT nObject, UINT BufferStride, UINT Slot, UINT BindFlag, UINT Offset){
	 CBuffer* pBuffer = new CBuffer();
	pBuffer->SetnObject(nObject);
	pBuffer->SetBufferStride(BufferStride);
	pBuffer->SetSlot(Slot);
	pBuffer->SetBindFlag(BindFlag);
	pBuffer->SetOffset(Offset);
	pBuffer->SetBuffer(CreateConstantBuffer(nObject* BufferStride));
	pBuffer->Begin();
	return pBuffer;
}

 CBuffer* CBuffer::CreateInstancingBuffer(UINT nObject, UINT BufferStride, UINT Offset){
	 CBuffer* pBuffer = new CBuffer();
	pBuffer->SetnObject(nObject);
	pBuffer->SetBufferStride(BufferStride);
	pBuffer->SetOffset(Offset);
	pBuffer->SetBuffer(CreateInstancingBuffer(nObject* BufferStride));
	pBuffer->Begin();

	return pBuffer;
}

ID3D11Buffer * CBuffer::CreateInstancingBuffer(UINT nByteWidth) {
	ID3D11Buffer *pd3dInstanceBuffer = NULL;
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	/*������ �ʱ�ȭ �����Ͱ� ������ ���� ���۷� �����Ѵ�. ��, ���߿� ������ �Ͽ� ������ ä��ų� �����Ѵ�.*/
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = nByteWidth;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GLOBALVALUEMGR->GetDevice()->CreateBuffer(&d3dBufferDesc, NULL, &pd3dInstanceBuffer);
	return(pd3dInstanceBuffer);
}

ID3D11Buffer * CBuffer::CreateConstantBuffer(UINT nByteWidth) {
	ID3D11Buffer* pd3dBuffer;
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	/*������ �ʱ�ȭ �����Ͱ� ������ ���� ���۷� �����Ѵ�. ��, ���߿� ������ �Ͽ� ������ ä��ų� �����Ѵ�.*/
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = nByteWidth;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//�ʱ�ȭ data ����!
	GLOBALVALUEMGR->GetDevice()->CreateBuffer(&d3dBufferDesc, NULL, &pd3dBuffer);

	return pd3dBuffer;

}


CBuffer::CBuffer() : DXObject("buffer") { }
CBuffer::~CBuffer() { };

