// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
//����
#include "../../Assets/import/LowLib/Global.h"
//�� �ȿ� windows��� �ִ�
#define DEFAULT_PORT 9595
//����
// Windows ��� ����:

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <mmsystem.h>
#include <thread>
#include <mutex>

#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <atlcoll.h>
#include <atlstr.h>

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <timeapi.h>
#include <ctime>
#include <time.h>
#include <timeapi.h>
#include <queue>
#include <unordered_map>
#include "winuser.h"                       //���콺 �� �̺�Ʈ ó���� ���� ��� �Լ�

#ifndef WM_MOUSEWHEEL 

#define WM_MOUSEWHEEL 0x020A  //���콺 �� �̺�Ʈ �޼��� �̸�
#endif


// D3D11
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <D3DX11tex.h>
#include "DirectXTex.h"

// DirectX Math
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

// C++11
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <chrono>
#include <stack>
#include <memory>

//�߰� ���
#include "Define.h"
#include "Enum.h"
#include "Function.h"
#include "Struct.h"

//����
#include "../../Assets/import/LowLib/CriticalSection.h"
#include "../../Assets/import/LowLib/MultiThreadSync.h"
#include "../../Assets/import/LowLib/MemoryPool.h"
#include "../../Assets/import/LowLib/CircularQueue.h" 
#include "../../Assets/import/LowLib/NetworkSession.h"
#include "../../Assets/import/LowLib/PacketSession.h"
#include "../../Assets/import/LowLib/EventSelect.h"
#include "../../Assets/import/LowLib/Stream.h"
#include "../../Assets/import/LowLib/MiniDump.h"
#include "../../Assets/import/LowLib/IniFile.h"
#include "../../Assets/import/LowLib/ClientSession.h"

#include "../../Server/Protocol.h"
#include "../../Server/PT_Structure.h"
#include "../../Server/PT_ReadPacket.h"
#include "../../Server/PT_WritePacket.h"
//����

using namespace std;
using namespace std::chrono;

//using namespace D2D1;
using namespace DirectX;
using namespace DirectX::PackedVector;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

//singleton
#include "GlobalValueManager.h"
#include "Timer.h"
#include "ResourceManager.h"
#include "RenderContainerSeller.h"
#include "Debuger.h"
#include "InputManager.h"
#include "DirectoryFinder.h"
#include "Importer.h"
#include "Renderer.h"
#include "Updater.h"
#include "SceneMgr.h"
#include "CameraMgr.h"
#include "NetworkMgr.h"
#include "NaviObjectManager.h"
#include "EffectMgr.h"
#include "SoundManager.h"

#define		GLOBALVALUEMGR	CGlobalValueManager::GetInstance()
#define		TIMEMGR			CGameTimer::GetInstance()
#define		RESOURCEMGR		CResourceManager::GetInstance()
#define		RCSELLER		CRenderContainerSeller::GetInstance()
#define		DEBUGER			CDebuger::GetInstance()
#define		INPUTMGR		CInputManager::GetInstance()
#define		DIRECTORYFINDER	CDirectoryFinder::GetInstance()
#define		IMPORTER		CImporter::GetInstance()
#define		RENDERER		CRenderer::GetInstance()
#define		UPDATER			CUpdater::GetInstance()
#define		SCENEMGR		CSceneMgr::GetInstance()
#define		CAMMGR			CCameraMgr::GetInstance()
#define		NETWORKMGR		CNetworkManager::GetInstance()