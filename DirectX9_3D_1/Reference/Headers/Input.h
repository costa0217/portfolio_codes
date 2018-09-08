/*!
 * \file Input.h
 * \date 2015/11/06 22:11
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Ű����, ���콺�� Input�� ���õ� class.
 *
 * \note
*/

#ifndef Input_h__
#define Input_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CInput
{
	DECLARE_SINGLETON(CInput)

public:
	enum MOUSECLICK {DIM_LBUTTON, DIM_RBUTTON, DIM_MBUTTON, DIM_XBUTTON};
	enum MOUSEMOVE {DIM_X, DIM_Y, DIM_Z};

private:
	explicit CInput(void);
	~CInput(void);

public: //getter
	_byte GetDIKeyState(BYTE KeyFlag);
	_byte GetDIMouseState(MOUSECLICK KeyFlag);
	_long GetDIMouseMove(MOUSEMOVE KeyFlag);

public: //setter
	void Set_InputState(void); //�Է� ��ġ�� ���¸� �޾ƿ´�.

public:
	HRESULT Ready_InputDevice(HINSTANCE hInst, HWND hWnd);

private:
	HRESULT Init_KeyBoard(HWND hWnd);
	HRESULT Init_Mouse(HWND hWnd);

private:
	LPDIRECTINPUT8			m_pInput;		//��ġ ��ü�� �ʱ�ȭ �ϱ� ���� ���� ��ü
	LPDIRECTINPUTDEVICE8	m_pKeyBoard;	//Ű���带 ��ǥ�ϴ� ��ü�̴�.
	LPDIRECTINPUTDEVICE8	m_pMouse;		//���콺�� ��ǥ�ϴ� ��ü�̴�.

private:
	_byte			m_byKeyState[256];
	DIMOUSESTATE	m_MouseState;

public:
	void Release(void);
};

END

#endif // Input_h__