#pragma once
#include "LabClientSession.h"
#include "SingleTon.h"
#define NO_SERVER

//protocol

#define BUF_SIZE 1024

#define SERVER_PORT 9595

#define MAX_CLIENT 100

#define THREAD_NUM 8

#define WM_SOCKET WM_USER + 1

#define FREQUENCY_TRANSFER_TIME 0.001

#define MAX_BUFFER_SIZE         1024
#define MAX_PACKET_SIZE         1024
#define MAX_STR_SIZE         100
#define PACKET_SIZE_SIZE      2
#define PACKET_TYPE_SIZE      2
#define MAX_CLIENT_IN_ROOM		4

#define SC_ID                  1
#define SC_NOTICE               2
#define SC_LOBBY_CLIENT_LIST      3
#define SC_ROOM_INFO            13
#define SC_CREATE_ROOM_SUCCESS      11
#define SC_CREATE_ROOM_FAIL         12
#define SC_JOIN_ROOM_SUCCESS      4
#define SC_JOIN_ROOM_FAIL         5
#define SC_CLIENT_IN_ROOM         6
#define SC_ALL_READY_COMPLETE      7
#define SC_GAME_START            8
#define SC_REMOVE_PLAYER         9
#define SC_PLAYER_POS            10

#define CS_CREATE_ROOM            6
#define CS_JOIN_ROOM            1
#define CS_OUT_ROOM               2
#define CS_CHARACTER_READY_CHANGE   3
#define CS_LOADING_COMPLETE         4
#define CS_PLAYER_POS            5

//////////////////////////////////////////////////////
//
//   Struct
//
//////////////////////////////////////////////////////
enum EVENTTYPE {
	E_RECV, E_SEND
};

struct ClientInLobby {
	int id;
	int iRoomNumber;
};

struct ClientInRoom {
	char Character;
	bool isReady;
};

struct Player {
	float PosX{ 0.f }, PosY{ 0.f }, PosZ{ 0.f };
	float RotY{ 0.f };
	char AnimNumber{ -1 };
};

struct WSAOVERLAPPED_EX {
	WSAOVERLAPPED Overlap;
	WSABUF Wsabuf;
	unsigned char IocpBuffer[MAX_BUFFER_SIZE];
	EVENTTYPE eEventType;
};

class ClientInfo {
public:
	int Id{ -1 };
	int iRoomNumber{ -1 };
	bool bConnected{ false };
	SOCKET Sock;
	WSAOVERLAPPED_EX RecvOverEx;
	mutex myLock;

	unsigned char PacketBuffer[MAX_PACKET_SIZE];
	int iPrevRecvSize;
	int iCurrPacketSize;

	bool bLoadingComplete;
	ClientInRoom tInRoom;
	Player tPlayer;
};


static int num_other_player = 0;
static int other_id[3] = { -1, -1, -1 };

class RoomInfo {
public:
	int iRoomNumber;
	char ClientCount;
	bool isStart;
	int ClientsIDInRoom[MAX_CLIENT_IN_ROOM];
};

class SEL_SCENE_INFO {
public:
	INT ID;
	INT SLOT_ID;
	INT CHARACTER;
	BOOL READY;
};
//////////////////////////////////////////////////////////////////
//
//  Protocol
//
//////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct sc_packet_id {
	WORD Size;
	WORD Type;
	INT Id;
};

struct sc_packet_notice {
	WORD Size;
	WORD Type;
	UCHAR Message[MAX_STR_SIZE];
};

struct sc_packet_lobby_client_list {
	WORD Size;
	WORD Type;
	INT ClientId[MAX_CLIENT];
	INT ClientRoomNumber[MAX_CLIENT];
};

struct sc_packet_join_room_result {
	WORD Size;
	WORD Type;
};

struct sc_packet_room_info {
	WORD Size;
	WORD Type;
	INT RoomNumber;
	INT ClientCount;
	BOOL isStart;
	INT ClientsId[MAX_CLIENT_IN_ROOM];
};

struct sc_packet_client_info_in_room {
	WORD Size;
	WORD Type;
	INT Id;
	int Character;
	BOOL isReady;
};

struct sc_packet_all_ready {
	WORD Size;
	WORD Type;
};

struct sc_packet_all_loading_complete {
	WORD Size;
	WORD Type;
};

struct sc_packet_remove_player {
	WORD Size;
	WORD Type;
	INT Id;
};

struct sc_packet_player_position {
	WORD Size;
	WORD Type;

	INT Id;

	float PosX;
	float PosY;
	float PosZ;

	float RotY;

	UINT AnimNumber;
};
////////////////////////////////////////////////////////
// CS
////////////////////////////////////////////////////////
struct cs_packet_create_room {
	WORD Size;
	WORD Type;
};
struct cs_packet_join_room {
	WORD Size;
	WORD Type;
	INT RoomNumber;
};

struct cs_packet_out_room {
	WORD Size;
	WORD Type;
	INT RoomNumber;
};

struct cs_packet_client_info_in_room {
	WORD Size;
	WORD Type;
	int Character;
	BOOL isReady;
};

struct cs_packet_loading_complete {
	WORD Size;
	WORD Type;
};

struct cs_packet_player_position {
	WORD Size;
	WORD Type;

	float PosX;
	float PosY;
	float PosZ;

	float RotY;

	UINT AnimNumber;
};
#pragma pack(pop)
//protocol
unsigned char Notice[MAX_STR_SIZE] = "sssss";

class CPawn;

//Hero Sel Scene Info -> MainScene Info
struct PLAYR_FREQUENCY_DATA {
	float fPosX{ 0.f };
	float fPosY{ 0.f };
	float fPosZ{ 0.f };
	float fAngleY{ 0.f };
	//DWORD dwDirection{ 0 };
	//bool bJump{ false };
	INT iAnimNum{ 0 };
};
struct BOSS_FREQUENCY_DATA {
	float fPosX{ 0.f };
	float fPosY{ 0.f };
	float fPosZ{ 0.f };
	float fAngleY{ 0.f };
	//DWORD dwDirection{ 0 };
	//bool bJump{ false };
	INT iAnimNum{ 0 };
};
struct PLAYER_CLICK_DATA {
	float fX, fY, fZ;
};
struct ServerPlayerInfo {
	BOOL READY{ 0 };
	BOOL ACTIONCAM{ 0 };
	INT CHARACTER{ 0 };
	//std::queue<PLAYR_FREQUENCY_DATA> m_qFREQUENCY_DATA;
	PLAYR_FREQUENCY_DATA FREQUENCY_DATA;
	PLAYER_CLICK_DATA CLICK_DATA;
	BOOL ATTACK{ false };

	ServerPlayerInfo(INT c) : CHARACTER(c) {}
};


class CNetworkManager : public CSingleTonBase<CNetworkManager> {

public:
	bool Begin();
	bool End();

private:
	SOCKET      ClientSocket;
	WSABUF      send_wsabuf;
	char		send_buffer[BUF_SIZE];
	WSABUF      recv_wsabuf;
	char		recv_buffer[BUF_SIZE];
	char		packet_buffer[BUF_SIZE];
	DWORD		in_packet_size = 0;
	int         saved_packet_size = 0;
	int         g_myid;

public:
	//외부에서 패킷을 보낼때 호출하는 함수
	BOOL WritePacket(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	//서버에서 보낸 패킷을 처리하는 함수
	void PTProc(CScene* pScene);
	//데이터 전송관련 변수 업데이트 함수.
	void Update(float fTimeElapsed);

	//플레이어 입력 처리 함수
	BOOL KeyInputProc(DWORD dwDirection);
	BOOL MouseMoveProc(float cxDelta, float cyDelta, DWORD dwMouseButton);
	BOOL MouseButtonProc(DWORD dwMouseButton);


	BOOL Login();
	//행성진입
	BOOL EnterEarth();

	//서버 접속 성공
	VOID PROC_PT_ENTER_SERVER_SUC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);

	//로그임 성공 실패
	VOID PROC_PT_LOGIN_SERVER_SUC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_LOGIN_SERVER_FAIL(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);

	//행성진입 성공 실패
	VOID PROC_PT_ENTER_EARTH_SUC(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_ENTER_EARTH_FAIL(DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);



	VOID PROC_PT_ENTER_SPACE(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_ENTER_SERVER_S(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_ENTER_SERVER_ALL(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_LEAVE_SERVER_ALL(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);

	VOID PROC_PT_KEY_INPUT_ALL_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_MOUSE_INPUT_ALL_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_FIRE_BULLET_ALL_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);

	//시야처리 프록
	VOID PROC_PT_PUT_OBJ_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_REMOVE_OBJ_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_CHANGE_STATE_OBJ_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);

	VOID PROC_PT_PUT_OBJS_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_REMOVE_OBJS_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);

	VOID PROC_PT_PUT_PLAYER_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_REMOVE_PLAYER_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);
	VOID PROC_PT_CHANGE_STATE_PLAYER_SC(CScene* pScene, DWORD dwProtocol, BYTE* Packet, DWORD dwPacketLength);


	void SetNETWORK_ID(int id) { NETWORK_ID = id; }
	void SetConnect(bool b) { m_bConnect = b; }
	void SetSLOT_ID(int id) { SLOT_ID = id; }
	void SetROOM_ID(int id) { ROOM_ID = id; }
	//void SetCharacterIndex(int idx) { m_CharacterIndex = idx; }
	int GetNETWORK_ID() { return NETWORK_ID; }
	bool GetConnect() { return m_bConnect; }
	int GetSLOT_ID() { return SLOT_ID; }
	int GetROOM_ID() { return ROOM_ID; }
	//int GetCharacterIndex() { m_CharacterIndex; }

	CLabClientSession* GetClientSession() { return m_pClientSession; }

	//serverplayer data 그냥 char번호임
	vector<ServerPlayerInfo>& GetServerPlayerInfos() { return m_vServerPlayerInfo; }
	void AddServerPlayerInfo(INT CHARACTER);//내가 순서대로 넣어줄 꺼니까 index가 곧 SLOT_ID임
	ServerPlayerInfo GetServerPlayerInfo(INT SLOT_ID);
	void ClearServerPlayerInfo() { m_vServerPlayerInfo.clear(); }

	BOSS_FREQUENCY_DATA GetBossInfo() { return m_tBossInfo; }
	void SetBossInfo(BOSS_FREQUENCY_DATA data) { m_tBossInfo = data; }

	INT GetBossHP() { return BOSS_HP; }
	void SetBossHP(INT hp) { BOSS_HP = hp; };

	INT GetPlayerHP(int slot_id) { return PlayerHP[slot_id]; }
	void SetPlayerHP(int slot_id, int hp) { PlayerHP[slot_id] = hp; }
	//network pawn input 
	PLAYR_FREQUENCY_DATA GetPlayerFrequencyData(INT SLOT_ID);
	
	BOOL GetAttack(INT SLOT_ID);
private:
	//hero sel scene에서 
	vector<ServerPlayerInfo> m_vServerPlayerInfo;
	//서버와의 통신을 총괄하는 객체
	CLabClientSession* m_pClientSession;
	BOSS_FREQUENCY_DATA m_tBossInfo;
	//현재 내가 조종하는 플레이어는 scene에 존재한다.
	INT NETWORK_ID{ 0 };
	INT ROOM_ID{ 0 };
	INT SLOT_ID{ 0 };
	INT CHARACTER{ 0 };
	INT BOSS_HP{ 0 };
	INT PlayerHP[4]{ 0,0,0,0 };
	////현재 접속한 적 플레이어를 관리할 객체
	////<그림그리고, 입력관리> - 쉐이더를 가짐.
	//COtherPlayerManager* m_pOtherPlayerManager;
	//
	//CServerObjectManager* m_pServerObjectManager;

	//마우스 패킷을 전송하는 시간 관련 변수
	float m_fMousePacketTime;
	//마우스 버튼의 변화를 관찰하기위한 변수
	DWORD m_dwprevMouseButton;

	//키보드 패킷을 전송하는 시간 관련 변수
	float m_fKeyPacketTime;

	bool m_bConnect{ false };
	//서버에서 데이터가 오기를 기다리는 플래그
	bool m_bWaitData{ false };
	//로그인 성공 여부 플래그
	bool m_bLogin{ false };
	//행성진입 성공 여부 플래그
	bool m_bEnterEarth;
public:
	CNetworkManager();
	virtual ~CNetworkManager();

};