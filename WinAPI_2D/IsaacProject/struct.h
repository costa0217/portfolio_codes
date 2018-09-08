#pragma once

typedef struct tagInfo
{
	float fX;
	float fY;
	float fCX;
	float fCY;

	tagInfo(){}
	tagInfo(float _fX, float _fY, float _fCX, float _fCY)
		: fX(_fX), fY(_fY), fCX(_fCX), fCY(_fCY)
	{

	}
}INFO;

typedef struct tagTile
{
	float fX;
	float fY;
	float fCX;
	float fCY;
	int	  iDrawID;
	int	  iOption;



}TILE;

typedef struct tagFrame
{
	int		iStart;
	int		iLast;
	int		iScene;
	DWORD	dwTime;

	tagFrame(void){}
	tagFrame(int _iStart, int _iLast, int _iScene, DWORD _dwTime)
		: iStart(_iStart), iLast(_iLast), iScene(_iScene), dwTime(_dwTime)
	{	}

}FRAME;