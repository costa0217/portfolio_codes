// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include <string>
#include <map>
#include <list>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <functional>
#include <shlwapi.h>		// ��ΰ��� �Լ��� ����ϱ� ���� ���
#include <atlimage.h>		// �̹����� ���õ� �Լ��� ����ϱ� ���� ���
#include <fstream>

// ����
#include <MMSystem.h>
#include <dsound.h>
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dsound.lib")



#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

using namespace std;

#include "inc/fmod.hpp"
#pragma comment (lib, "fmodex_vc.lib")

using namespace FMOD;