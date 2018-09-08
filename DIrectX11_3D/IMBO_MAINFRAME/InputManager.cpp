
#include "stdafx.h"
#include "InputManager.h"

bool CInputManager::Begin() {
	//	m_stHashTable.clear();
	//
	//	//Pad Connect
	//	m_bConnect = false;
	//
	//	//Vibe State
	//	m_fVibrateionFrame = 0.0f;
	//	m_fVibrateDuringTime = GPAD_DURING_VIVERATE;
	//	m_bVibrate = false;
	//
	//
	//	m_bKeyState = new bool[YK_END];
	//
	memset(&m_pKeyBuffers, 0, sizeof(m_pKeyBuffers));
	memset(&m_bDownCheck, 0, sizeof(m_bDownCheck));

	//drag&drop
	DragAcceptFiles(GLOBALVALUEMGR->GethWnd(), TRUE);

	return true;
}

bool CInputManager::End() {
	return true;
}

void CInputManager::Update(float fTimeEleasped) {
	INPUTMGR->SetWheel(WHEEL_NON);

	//IsGamepadConnected(&m_bConnect);
	//m_cxDelta = 0.0f, m_cyDelta = 0.0f;

	//if (m_bConnect)
	//	UpdateGamePoad(fTimeEleasped);
	//else

	UpdateKeyBoard();
	//if (m_bCapture)
		UpdateMouse();
}
void CInputManager::UpdateKeyBoard() {
	GetKeyboardState(m_pKeyBuffers);
}

void CInputManager::UpdateMouse() {
	//POINT ptCursorPos;

	//���콺�� ĸ�������� ���콺�� �󸶸�ŭ �̵��Ͽ��� ���� ����Ѵ�.
	//if (GetCapture() == GLOBALVALUEMGR->GethWnd())
	{
		//���콺 Ŀ���� ȭ�鿡�� ���ش�(������ �ʰ� �Ѵ�).
		//SetCursor(NULL);
		//���� ���콺 Ŀ���� ��ġ�� �����´�.
		//GetCursorPos(&ptCursorPos);
		////���콺 ��ư�� ���� ä�� ���� ��ġ���� ���� ���콺 Ŀ���� ��ġ���� ������ ���� ���Ѵ�.
		//m_cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
		//m_cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
		//SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	}
}

//Key Up
bool CInputManager::KeyUp(eVK input) {
	//if (IsGamepadConnected())
	//	return KeyPadUp(input);
	return KeyBoardUp(input);
}
bool CInputManager::KeyBoardUp(eVK input)
{
	if (m_pKeyBuffers[input] & 0xF0) {
		m_bUpCheck[input] = true;
		return false;
	}
	else if (m_bUpCheck[input]) {
		m_bUpCheck[input] = false;
		return true;
	}
	return false;
}
//Only KeyDown
bool CInputManager::OnlyKeyDown(eVK input)
{
	//if (IsGamepadConnected())
	//	return OnlyKeyPadDown(input);
	return OnlyKeyBoardDown(input);
}
bool CInputManager::OnlyKeyBoardDown(eVK input)
{
	if (m_pKeyBuffers[input] & 0xF0) {
		return true;
	}
	return false;
}
BYTE CInputManager::AnyOneKeyDown(int n, eVK ...)
{
	if (false == m_bKeyBoardPressCheck) return false;

	va_list _ArgList;
	va_start(_ArgList, n);
	bool press = false;
	while (n--)
	{
		auto input = va_arg(_ArgList, eVK);
		press = OnlyKeyDown(input);
		if (press) return press;
	}
	return press;
}

void CInputManager::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID)
	{
		/*�������� ũ�Ⱑ ����� ��(����� ��Alt+Enter�� ��ü ȭ�� ���� ������ ���� ��ȯ�� ��) ���� ü���� �ĸ���� ũ�⸦ �����ϰ� �ĸ���ۿ� ���� ���� Ÿ�� �並 �ٽ� �����Ѵ�. */
	case WM_SIZE: {
		GLOBALVALUEMGR->SetrcClient(LOWORD(lParam), HIWORD(lParam));

		/*RECT rcWindow = { 0, 0, WINSIZEX, WINSIZEY };
		AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE);*/

		GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(0, NULL, NULL);

		RENDERER->ResizeBuffer();

		if (UPDATER->GetCamera()) UPDATER->GetCamera()->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);
		break;
	}
	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_DROPFILES:
		INPUTMGR->ProcDropFile(wParam);

		break;
	}
}

void CInputManager::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID)
	{

	case WM_LBUTTONDOWN:
		SetbCapture(true);

		//SetOldCursorPos();
		SetMouseLeft(true);
		break;

	case WM_RBUTTONDOWN:
		SetbCapture(true);

		//SetOldCursorPos();
		SetMouseRight(true);
		break;

	case WM_LBUTTONUP:
	{
		//static bool showCusor = true;
		//showCusor = showCusor ? false : true;
		//ShowCursor(showCusor);
		SetMouseLeft(false);
		SetbCapture(false);
		break;
	}

	case WM_RBUTTONUP:
		SetMouseRight(false);
		SetbCapture(false);
		break;

	case WM_MOUSEMOVE:
		SetMousePoint((int)LOWORD(lParam), (int)HIWORD(lParam));
		//SetOldCursorPos();
		
			if (m_bCheck)
			{
				POINT ptCursorPos;
				GetCursorPos(&ptCursorPos);
				//���콺 ��ư�� ���� ä�� ���� ��ġ���� ���� ���콺 Ŀ���� ��ġ���� ������ ���� ���Ѵ�.
				m_cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
				m_cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
				m_ptOldCursorPos = ptCursorPos;
			}
			else
			{
				m_cxDelta = 0.f;
				m_cyDelta = 0.f;
				m_bCheck = true;
			}
		
		
		break;
	case WM_MOUSEWHEEL:
		if (((short)HIWORD(wParam) < WHEEL_NON))
		{
			SetWheel(WHEEL_DOWN);
		}
		else
		{
			SetWheel(WHEEL_UP);
		}
		break;
		//for drag&drop
	case WM_DROPFILES:
		ProcDropFile(wParam);

		break;
	default:
		break;
	}
}

void CInputManager::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	//m_stackScene.top()->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		}
	}
}

//KeyDown
bool CInputManager::KeyDown(eVK input)
{
	//if (IsGamepadConnected())
	//	return KeyPadDown(input);
	return KeyBoardDown(input);
}
bool CInputManager::KeyBoardDown(eVK input) {
	if (m_pKeyBuffers[input] & 0xF0) {
		if (false == m_bDownCheck[input]) {
			m_bDownCheck[input] = true;
			return true;
		}
	}
	else {
		m_bDownCheck[input] = false;
	}
	return false;
}

//mouse
void CInputManager::SetWheel(int wheel)
{
	//	m_cWheelDelta = 0.f;
	float fTimeDelta = TIMEMGR->GetTimeElapsed();
	float fSpeed = 40.f;
	if (wheel > WHEEL_NON)
	{
		m_sWheel = WHEEL_UP;
		m_cWheelDelta -= fTimeDelta * fSpeed;

		if (m_cWheelDelta < -8.f)
			m_cWheelDelta = -8.f;
	}
	else if (wheel < WHEEL_NON)
	{
		m_sWheel = WHEEL_DOWN;
		m_cWheelDelta += fTimeDelta * fSpeed;


		if (m_cWheelDelta > 14.f)
			m_cWheelDelta = 14.f;
	}
	else {
		m_sWheel = WHEEL_NON;
	}
	/*m_cWheelDelta = m_fCurWheelValue - m_fOldWheelValue;
	m_fOldWheelValue = m_fCurWheelValue;*/
	//float m_fCurWheelValue{ 0.f };
}

void CInputManager::MouseWheel()
{
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
		std::cout << "0x800" << std::endl;

	if (GetAsyncKeyState(VK_MBUTTON) & 0xF0)
		std::cout << "0xF0" << std::endl;

	if (GetAsyncKeyState(VK_MBUTTON) & 0x00)
		std::cout << "0x00" << std::endl;

	if (GetAsyncKeyState(VK_MBUTTON) & 0x01)
		std::cout << "0x01" << std::endl;

	if (GetAsyncKeyState(VK_MBUTTON))
		std::cout << "VK_MBUTTON" << std::endl;
}

void CInputManager::SetMousePoint(int x, int y) {
	pt.x = x; pt.y = y;
}

bool CInputManager::MouseLeftDown() {
	return m_bLeftCheck;
}

bool CInputManager::MouseLeftOnlyDown() {
	if (m_bLeftCheck) {
		m_bLeftOnlyCheck = true;
		return false;
	}
	else if (m_bLeftOnlyCheck) {
		m_bLeftOnlyCheck = false;
		return true;
	}
}

bool CInputManager::MouseRightDown() {
	return m_bRightCheck;
}

//^ ������ 1, �ٸ��� 0
bool CInputManager::MouseLeftUp() {
	return (!m_bLeftCheck);
}

bool CInputManager::MouseRightUp() {
	return (!m_bRightCheck);
}

void CInputManager::SetbCapture(bool b) {
	if (b) SetCapture(GLOBALVALUEMGR->GethWnd());
	else ReleaseCapture();

	m_bCapture = b;
}

void CInputManager::ProcDropFile(WPARAM wParam) {
	m_hDropInfo = (HDROP)wParam;
	DragQueryFileA(m_hDropInfo, 0, m_DropFileNameBuf, m_DropFileBuffSize);
	m_bCanUseDropFileName = true;
}

const char* CInputManager::GetDropFileName() {

	char* returnVal{ nullptr };

	if (m_bCanUseDropFileName) {
		returnVal = m_DropFileNameBuf;
		m_bCanUseDropFileName = false;
	}

	return returnVal;
}

void CInputManager::SetMousePoint(POINT _pt) {
	pt = _pt;
}

CInputManager::CInputManager() :CSingleTonBase<CInputManager>("inputmanagersingleton") {

}

CInputManager::~CInputManager() {

}
