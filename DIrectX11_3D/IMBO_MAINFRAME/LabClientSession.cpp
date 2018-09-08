#include "stdafx.h"
#include "LabClientSession.h"


VOID CLabClientSession::OnIoConnected(VOID)
{
	_tprintf(_T("OnIoConnected\n"));
}

VOID CLabClientSession::OnIoDisconnected(VOID)
{
	_tprintf(_T("OnIoDisconnected\n"));
}
