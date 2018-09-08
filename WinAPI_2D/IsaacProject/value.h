#pragma once

extern HWND g_hWnd;

//������ ũ��
const int WINCX = 800;
const int WINCY = 600;



//Ÿ��
const int TILEX = 15;			//Ÿ�� ����
const int TILEY = 9;

const int TILESTARTX = 22;		//Ÿ�� ������
const int TILESTARTY = 120;

const int TILECX = 50;			//Ÿ�� ũ��
const int TILECY = 50;

// key ��

const DWORD		KEY_LEFT	= 0X00000001;
const DWORD		KEY_RIGHT	= 0X00000002;
const DWORD		KEY_UP		= 0X00000004;
const DWORD		KEY_DOWN	= 0X00000008;



//player state
const DWORD		ST_STAND	= 0x00000001;
const DWORD		ST_UP		= 0x00000002;
const DWORD		ST_DOWN		= 0x00000004;
const DWORD		ST_LEFT		= 0x00000008;
const DWORD		ST_RIGHT	= 0x00000010;
const DWORD		ST_ITEM		= 0x00000020;
//const DWORD		ST_DAMAGE	= 0x00000010;
//const DWORD		ST_DEATH	= 0x00000020;