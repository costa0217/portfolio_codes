// Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#ifdef _DEBUG
#include "Vld.h"
#endif

#include "Client.h"
#include "../Codes/MainApp.h"
#include "Export_Function.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE g_hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			//  �⺻ â Ŭ���� �̸��Դϴ�.
HWND g_hWnd;

_ulong VTXCNTX = 0; 
_ulong VTXCNTZ = 0;
_ulong VTXITV = 1;


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;
	CMainApp*			pMainApp = NULL;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	pMainApp = CMainApp::Create();
	if(NULL == pMainApp)
		Engine::Safe_Release(pMainApp);

	if(FAILED(Engine::Ready_InputDevice(g_hInst, g_hWnd)))
	{
		MSG_BOX("Ready_InputDevice Failed");
		return FALSE;
	}
	

	if(FAILED(Engine::Ready_Timer(L"Immediate")))
	{
		MSG_BOX("Ready_Timer(Immediate) Failed");
		return FALSE;
	}

	if(FAILED(Engine::Ready_Timer(L"Frame:60")))
	{
		MSG_BOX("Ready_Timer(Frame:60) Failed");
		return FALSE;
	}
	if(FAILED(Engine::Ready_Timer(L"Frame:50")))
	{
		MSG_BOX("Ready_Timer(Frame:50) Failed");
		return FALSE;
	}


	if(FAILED(Engine::Ready_Frame(L"FPS:60", 60.0f)))
	{
		MSG_BOX("Ready_Frame(FPS:60) Failed");
		return FALSE;
	}

	if(FAILED(Engine::Ready_Frame(L"FPS:50", 50.0f)))
	{
		MSG_BOX("Ready_Frame(FPS:50) Failed");
		return FALSE;
	}
	srand((unsigned)time(NULL));

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	//Cursor.Hide();
	ShowCursor( FALSE );


	// �⺻ �޽��� �����Դϴ�.

	while ( true )
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{	
			Engine::Set_TimeDelta(L"Immediate");
			_float	fTimeDelta = Engine::Get_TimeDelta(L"Immediate");
			if(fTimeDelta == 0.0f)
				break;

			if(true == Engine::Get_Activate(L"FPS:60", fTimeDelta))
			{
				Engine::Set_TimeDelta(L"Frame:60");

				_float fTimeDelta60 = Engine::Get_TimeDelta(L"Frame:60");
				//_float fTimeDelta50 = Engine::Get_TimeDelta(L"Frame:50");

				Engine::Set_InputState();

				if(pMainApp->Update(fTimeDelta60) & 0x80000000)
					break;
				pMainApp->Render();
			}
		}
	}

	if(::Safe_Release(pMainApp))
		MSG_BOX("pMainApp Release Failed");

	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   g_hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   RECT			rcWindow = {0, 0, WINSIZEX, WINSIZEY};  

   AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE);
   
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, NULL, hInstance, NULL);

   g_hWnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;


	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴��� ���� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

