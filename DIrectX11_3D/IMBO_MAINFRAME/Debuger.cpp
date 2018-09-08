
#include "stdafx.h"
#include "Debuger.h"

bool CDebuger::Begin(){
	
	//test
	m_pDebugTextureObj = new CDebugTexture;
	m_pDebugTextureObj->Begin();
	
	for (auto RenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_DEBUG]) {
		m_mDebugRenderContainer[RenderContainer.first] = RenderContainer.second;
	}
	
	//aabb��ü �̸� �Ҵ�
	m_ppBoundingBox = new CBoundingBox*[BOUNDINGBOX_NUM];
	for (int i = 0; i < BOUNDINGBOX_NUM; ++i) {
		m_ppBoundingBox[i] = new CBoundingBox;
	}
	//aabb��ü �̸� �Ҵ�

	//coordinatesys��ü �̸� �Ҵ�
	m_ppCoordinateSys = new CCoordinateSys*[COORD_NUM];
	for (int i = 0; i < COORD_NUM; ++i) {
		m_ppCoordinateSys[i] = new CCoordinateSys;
	}
	//coordinatesys��ü �̸� �Ҵ�

	//depth stencil state
	D3D11_DEPTH_STENCIL_DESC descDepthStencil;
	ZeroMemory(&descDepthStencil, sizeof(D3D11_DEPTH_STENCIL_DESC));
	descDepthStencil.DepthEnable = true;
	descDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepthStencil.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilState(&descDepthStencil, &m_pLightDepthStencilState);
	//depth stencil state

	//rasterizer state
	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	//descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.FillMode = D3D11_FILL_WIREFRAME;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	GLOBALVALUEMGR->GetDevice()->CreateRasterizerState(&descRasterizer, &m_pLightRasterizerState);
	//rasterizer state

	//blend state
	D3D11_BLEND_DESC descBlend;
	ZeroMemory(&descBlend, sizeof(D3D11_BLEND_DESC));
	descBlend.RenderTarget[0].BlendEnable = true;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//blend state
	GLOBALVALUEMGR->GetDevice()->CreateBlendState(&descBlend, &m_pLightBlendState);

	//font .
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1FontFactory)))
	{
		DebugMessageBox("��Ʈ�� ���� �� �����ϴ�", "��Ʈ������!");
		return false;
	}
	if (FAILED(m_pFW1FontFactory->CreateFontWrapper(GLOBALVALUEMGR->GetDevice(), L"�������", &m_pFW1Font)))
	{
		DebugMessageBox("��Ʈ �ε�����", "��Ʈ����");
		return false;
	}
	//font .

	return true;
}

bool CDebuger::End() {
	for (int i = 0; i < BOUNDINGBOX_NUM; ++i) {
		delete m_ppBoundingBox[i];
	}
	delete m_ppBoundingBox;

	for (int j = 0; j < COORD_NUM; ++j) {
		delete m_ppCoordinateSys[j];
	}
	delete m_ppCoordinateSys;

	//rendercontainer end�� seller�� �����̴�.
	//font
	if (m_pFW1FontFactory) m_pFW1FontFactory->Release();
	m_pFW1FontFactory = nullptr;

	m_pFW1Font->Release();

	if (m_pFWTextRender)m_pFWTextRender->Release();
	m_pFWTextRender = nullptr;

	if (m_pFW1Glyphrovider) m_pFW1Glyphrovider->Release();
	m_pFW1Glyphrovider = nullptr;

	while (false == m_qDebugFontData.empty()) {
		m_qDebugFontData.pop();
	}
	while (false == m_qGameFontData.empty()) {
		m_qGameFontData.pop();
	}
	while (false == m_qDebugTextureData.empty()) {
		m_qDebugTextureData.pop();
	}
	if (m_pDebugTextureObj) {
		m_pDebugTextureObj->End();
		delete m_pDebugTextureObj;
	}
	m_pDebugTextureObj = nullptr;
	return true;
}
void CDebuger::RegistCoordinateSys(FXMMATRIX mtx) {
	if (false == INPUTMGR->GetDebugMode()) return;

	m_ppCoordinateSys[m_nCoordinateSys]->SetCoordinateSysInfo(mtx);
	m_mDebugRenderContainer["coordinatesys"]->AddObject(m_ppCoordinateSys[m_nCoordinateSys++]);
}


void CDebuger::RegistAABB(BoundingBox& aabb, utag ut) {
	if (false == INPUTMGR->GetDebugMode()) return;

	m_ppBoundingBox[m_nAABB]->SetBoundingBoxInfo(aabb, ut);
	m_mDebugRenderContainer["aabb"]->AddObject(m_ppBoundingBox[m_nAABB++]);

}
void CDebuger::RegistOBB(BoundingOrientedBox & obb, utag ut) {
	if (false == INPUTMGR->GetDebugMode()) return;

	m_ppBoundingBox[m_nAABB]->SetBoundingBoxInfo(obb, ut);
	m_mDebugRenderContainer["aabb"]->AddObject(m_ppBoundingBox[m_nAABB++]);
}

void CDebuger::RegistToDebugRenderContainer(CGameObject * pObject){
	if (false == INPUTMGR->GetDebugMode()) return;

	string name = pObject->GetName();

	if (name == "pointlight") {
		m_mDebugRenderContainer["debugpointlight"]->AddObject(pObject);
	}
	else if (name == "capsulelight") {
		m_mDebugRenderContainer["debugcapsulelight"]->AddObject(pObject);
	}
	else if (name == "spotlight") {
		m_mDebugRenderContainer["debugspotlight"]->AddObject(pObject);
	}
	

}

void CDebuger::DebugRender( CCamera* pCamera){
	if (false == INPUTMGR->GetDebugMode()) return;
	
	CNaviObjectManager::Render();
	RenderAABB(pCamera);
	RenderLightVolume(pCamera);
	RenderCoordinateSys(pCamera);
	
	for (auto pRenderContaier : m_mDebugRenderContainer) {
		pRenderContaier.second->ClearObjectList();
	}
	
	m_nAABB = 0;
	m_nCoordinateSys = 0;
	//ClearDebuger();
}

void CDebuger::RenderAABB( CCamera* pCamera){
	if (false == INPUTMGR->GetDebugMode()) return;
	//render aabb
	
	m_mDebugRenderContainer["aabb"]->Render(pCamera);
}
void CDebuger::RenderCoordinateSys( CCamera* pCamera) {
	if (false == INPUTMGR->GetDebugMode()) return;
	//render coordinatesystem
	m_mDebugRenderContainer["coordinatesys"]->Render(pCamera);
}
void CDebuger::RenderLightVolume( CCamera* pCamera){
	
	//���� ���� ����
	//m_pd3dDeviceContext->OMGetDepthStencilState(&m_pPreDepthStencilState, &m_PreStencilRef);
	//m_pd3dDeviceContext->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&m_pPreRasterizerState);
	
	//m_pd3dDeviceContext->OMSetBlendState(m_pLightBlendState, nullptr, 0xffffffff);
	//m_pd3dDeviceContext->OMSetDepthStencilState(m_pLightDepthStencilState, 0);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pLightRasterizerState);
	
	m_mDebugRenderContainer["debugpointlight"]->Render(pCamera);
	m_mDebugRenderContainer["debugcapsulelight"]->Render(pCamera);
	m_mDebugRenderContainer["debugspotlight"]->Render(pCamera);
	
	//for (auto RenderContainer : m_mDebugRenderContainer) {
	//	RenderContainer.second->Render(pCamera);
	//}
	
	//���� ���·� �ǵ���
	//m_pd3dDeviceContext->OMSetBlendState(m_pPreBlendState, m_pPreBlendFactor, m_PreSampleMask);
	//m_pd3dDeviceContext->OMSetDepthStencilState(m_pPreDepthStencilState, m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pPreRasterizerState);

}

void CDebuger::ClearDebuger(){
	Timemeasurement_count = 0;
	
//font
	while (false == m_qDebugFontData.empty()) {
		m_qDebugFontData.pop();
	}
	while (false == m_qGameFontData.empty()) {
		m_qGameFontData.pop();
	}
//texture
	while (false == m_qDebugTextureData.empty()) {
		m_qDebugTextureData.pop();
	}
	while (false == m_qDebugDepthTextureData.empty()) {
		m_qDebugDepthTextureData.pop();
	}
	for (auto RenderContainer : m_mDebugRenderContainer) {
		RenderContainer.second->ClearObjectList();
	}
	
	m_nAABB = 0;
	m_nCoordinateSys = 0;

	// ps
	GLOBALVALUEMGR->GetDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	// gs
	GLOBALVALUEMGR->GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	//hs
	GLOBALVALUEMGR->GetDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	//ds
	GLOBALVALUEMGR->GetDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	// ps
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);

}

//utill func
void CDebuger::AddText(float fontSize, float posX, float posY, UINT32 color, TCHAR * msg, ...) {
	va_list arg;
	va_start(arg, msg);
	TCHAR TEXT[500] = { NULL };
	vswprintf(TEXT, msg, arg);
	
	CDebugFontData Data(TEXT, fontSize, posX, posY, color, m_pivot);
	m_qDebugFontData.push(Data);
}
void CDebuger::RenderText(){
	Timemeasurement_count = 0;
	
	if (false == m_pFW1Font) return;
	if (false == INPUTMGR->GetDebugMode()) return;
	
	//���� ���� ����
	GLOBALVALUEMGR->GetDeviceContext()->OMGetDepthStencilState(&m_pPreDepthStencilState, &m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&m_pPreRasterizerState);
	
	
	while (false == m_qDebugFontData.empty()) {
		//get
		CDebugFontData Data = m_qDebugFontData.front();
	
		//draw
		m_pFW1Font->DrawString
		(
			GLOBALVALUEMGR->GetDeviceContext(),
			Data.msg.c_str(),
			Data.fontSize,
			Data.x,
			Data.y,
			Data.color,
			Data.pivot
		);
	
		//pop
		m_qDebugFontData.pop();
	}
	

	//���� ���·� �ǵ���
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pPreDepthStencilState, m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pPreBlendState, m_pPreBlendFactor, m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pPreRasterizerState);

}
void CDebuger::AddGameText(float fontSize, float posX, float posY, UINT32 color, TCHAR * msg, ...)
{
	va_list arg;
	va_start(arg, msg);
	TCHAR TEXT[500] = { NULL };
	vswprintf(TEXT, msg, arg);

	CDebugFontData Data(TEXT, fontSize, posX, posY, color, m_pivot);
	m_qGameFontData.push(Data);
}
void CDebuger::RenderGameText()
{
	if (false == m_pFW1Font) return;
	//if (false == INPUTMGR->GetDebugMode()) return;

	//���� ���� ����
	GLOBALVALUEMGR->GetDeviceContext()->OMGetDepthStencilState(&m_pPreDepthStencilState, &m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&m_pPreRasterizerState);


	while (false == m_qGameFontData.empty()) {
		//get
		CDebugFontData Data = m_qGameFontData.front();

		//draw
		m_pFW1Font->DrawString
		(
			GLOBALVALUEMGR->GetDeviceContext(),
			Data.msg.c_str(),
			Data.fontSize,
			Data.x,
			Data.y,
			Data.color,
			Data.pivot
		);

		//pop
		m_qGameFontData.pop();
	}


	//���� ���·� �ǵ���
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pPreDepthStencilState, m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pPreBlendState, m_pPreBlendFactor, m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pPreRasterizerState);
}
void CDebuger::AddTexture(XMFLOAT2 lt, XMFLOAT2 rb, ID3D11ShaderResourceView* pSRV){
	if (false == INPUTMGR->GetDebugMode()) return;
	CDebugTextureData DebugTextureData(pSRV, lt, rb);
	m_qDebugTextureData.push(DebugTextureData);
}
void CDebuger::AddDepthTexture(XMFLOAT2 fLeftTop, XMFLOAT2 fRightBottom, ID3D11ShaderResourceView * pSRV){
	if (false == INPUTMGR->GetDebugMode()) return;
	CDebugTextureData DebugTextureData(pSRV, fLeftTop, fRightBottom);
	m_qDebugDepthTextureData.push(DebugTextureData);
}
void CDebuger::RenderTexture(){
	if (false == INPUTMGR->GetDebugMode()) return;
	while (false == m_qDebugTextureData.empty()) {
	
		CDebugTextureData DebugTextureData = m_qDebugTextureData.front();
		m_qDebugTextureData.pop();
	
		m_pDebugTexture = CTexture::CreateTexture(DebugTextureData.m_pSRV);
	
		m_pDebugTextureObj->SetTextureInfo(DebugTextureData.lt, DebugTextureData.rb);
		string csName = "debugtexture";
		m_mDebugRenderContainer[csName]->AddObject(m_pDebugTextureObj);
		m_mDebugRenderContainer[csName]->AddTexture(m_pDebugTexture);
		m_mDebugRenderContainer[csName]->Render(nullptr);
		m_mDebugRenderContainer[csName]->ClearObjectList();
		m_mDebugRenderContainer[csName]->ClearTextures();
	
	//	m_pDebugTexture->End();
		delete m_pDebugTexture;
		m_pDebugTexture = nullptr;
	}
	
	//dpeh thexture
	while (false == m_qDebugDepthTextureData.empty()) {
	
		CDebugTextureData DebugTextureData = m_qDebugDepthTextureData.front();
		m_qDebugDepthTextureData.pop();
	
		m_pDebugTexture = CTexture::CreateTexture(DebugTextureData.m_pSRV);
	
		m_pDebugTextureObj->SetTextureInfo(DebugTextureData.lt, DebugTextureData.rb);
		string csName = "debugdepthtexture";
		m_mDebugRenderContainer[csName]->AddObject(m_pDebugTextureObj);
		m_mDebugRenderContainer[csName]->AddTexture(m_pDebugTexture);
		m_mDebugRenderContainer[csName]->Render(nullptr);
		m_mDebugRenderContainer[csName]->ClearObjectList();
		m_mDebugRenderContainer[csName]->ClearTextures();
	
	//	m_pDebugTexture->End();
		delete m_pDebugTexture;
		m_pDebugTexture = nullptr;
	}
}
int CDebuger::DebugMessageBox(std::string _title, std::string _message)
{
	const char* msgChar = _message.c_str();
	int msgLen = static_cast<int>(1 + strlen(msgChar));
	TCHAR* message = new TCHAR[msgLen];
	mbstowcs(message, msgChar, msgLen);
	
	const char* titleChar = _title.c_str();
	int titleLen = static_cast<int>(1 + strlen(titleChar));
	TCHAR* title = new TCHAR[titleLen];
	mbstowcs(title, titleChar, titleLen);
	
	return MessageBox(nullptr, message, title, MB_OK);
	//return 1;
}

int CDebuger::DebugGMessageBox(TCHAR* title, TCHAR* message, ...)
{
	va_list arg;
	va_start(arg, message);
	TCHAR TEXT[500] = { NULL };
	vswprintf(TEXT, message, arg);
	
	return MessageBox(nullptr, TEXT, title, MB_OK);
}


void CDebuger::start_Timemeasurement() {
	start = std::chrono::system_clock::now();
}
static int x = 400;
static int y = 20;
void CDebuger::end_Timemeasurement(WCHAR* msg) {
	std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
	
	WCHAR wMsg[256];
	WCHAR wTime[256];
	float time = (float)sec.count();
	_stprintf(wMsg, L"%s :", msg);
	_stprintf(wTime, L"%f", time);
	
	DEBUGER->AddText(15, x, y * Timemeasurement_count, YT_Color(255, 0, 0, 255), wMsg);
	DEBUGER->AddText(15, x + 150, y * Timemeasurement_count++, YT_Color(255, 0, 0, 255), wTime);
}

CDebuger::CDebuger() :CSingleTonBase<CDebuger>("debugersingleton"){

}

CDebuger::~CDebuger(){

}
