#ifndef Defines_h__
#define Defines_h__

#include "../Default/stdafx.h"
using namespace std;

#include "Typedef.h"
#include "Contant.h"
#include "Macro.h"
#include "Function.h"
#include "Functor.h"
#include "Enum.h"
#include "Struct.h"

#include "MathMgr.h"

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;

#define PlayerCollisionDistance 15.f
#define TargetAtPos _vec3(175.f, 6.f, 250.f)
#define ObserverEyePos _vec3(175.f, 9.f, 250.f)
#define StandardPos _vec3(175.f, 0.f, 250.f)

#endif // Defines_h__
