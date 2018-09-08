/*!
 * \file Engine_Defines.h
 * \date 2015/11/07 1:23
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Engine_Defines �������
 *
 * TODO: Engine���� ����ϴ� �⺻ ������ϵ��� ������ �������
 *
 * \note
*/

#ifndef Engine_Defines_h__
#define Engine_Defines_h__

#include <boost/unordered_map.hpp>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <time.h>
#include <d3dx9.h>
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <string>

#include <shlwapi.h>		// ��ΰ��� �Լ��� ����ϱ� ���� ���
#include <atlimage.h>		// �̹����� ���õ� �Լ��� ����ϱ� ���� ���
//#include <fstream>

using namespace std;

#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include "Engine_Macro.h"
#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"

const _ushort WINCX = 1600;
const _ushort WINCY = 900;

const	 _float	Gausianstd		= 0.4f;
static	_float		Gausianpow		= 0.2f;
static	_float		fExposure		= 0.2f;

const	 _float	EffectGausianstd		= 0.3f;
const	 _float	EffectGausianpow		= 0.6f;




#pragma warning(disable : 4251)
#pragma warning(disable : 4996)

#endif // Engine_Defines_h__