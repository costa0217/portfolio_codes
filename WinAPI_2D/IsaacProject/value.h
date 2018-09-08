#pragma once

extern HWND g_hWnd;

//윈도우 크기
const int WINCX = 800;
const int WINCY = 600;



//타일
const int TILEX = 15;			//타일 갯수
const int TILEY = 9;

const int TILESTARTX = 22;		//타일 시작점
const int TILESTARTY = 120;

const int TILECX = 50;			//타일 크기
const int TILECY = 50;

// key 값

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