#include "Camera.h"
#include "InputMgr.h"

#include "Bullet.h"

CCamera::CCamera()
{
	MyIdentityMatrix(&m_matCam);
	MyIdentityMatrix(&m_world);

	m_fHeight = 0.f;
	m_pInputMgr = CInputMgr::GetInstance();
	//m_vDir = MyVector(0.f, 0.f, 1.f);
	m_fMouseMoveSpd = 250.f;
	m_fSpeed = 2000.f;
	m_matCam._42 = 30.f;
	m_vPos = MyVector(0.f, 0.f, 0.f);
	m_vLook = MyVector(0.f, 0.f, 1.f);
}
CCamera::~CCamera()
{
}

void CCamera::InputData(float fTime)
{
	if (m_bCosterEye == true)
	{

		MyIdentityMatrix(&m_matCam);

		MyVector vMove	= MyVector(0.f, 50.f, 0.f);
		//MyVector vDir	= (*m_pPlayer)->m_vDir;

		//vDir = vDir + m_vPreDir;
		m_vDir = (*m_pPlayer)->m_vDir;
		MyNormalize(&m_vDir, &m_vDir);
		MyNormalize(&m_vPreDir, &m_vPreDir);
		m_vPos = (*m_pPlayer)->m_vPos + vMove;

		MyVector vLerp = CalculateLerp(m_fLerpTime, m_vPreDir, m_vDir);
		MyNormalize(&vLerp, &vLerp);

		m_matCam._31 = vLerp.x;		m_matCam._32 = vLerp.y;		m_matCam._33 = vLerp.z;
		m_matCam._41 = m_vPos.x;	m_matCam._42 = m_vPos.y;	m_matCam._43 = m_vPos.z;

		if (m_fLerpTime > 1.f)
		{
			m_vPreDir = m_vDir;
			//m_vDir = (*m_pPlayer)->m_vDir;
			m_fLerpTime = 0.f;
		}
		m_fLerpTime += fTime * 1.f;
	}
	else
	{
		if (!m_bMouseCenter)
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		else
			glutSetCursor(0);


		float fDeltaX = m_pInputMgr->m_fDeltaX * 0.1f;
		float fDeltaY = m_pInputMgr->m_fDeltaY * 0.1f;

		MyIdentityMatrix(&m_matCam);
		MATRIX matY, matX, matZ;
		MyIdentityMatrix(&matX);	MyIdentityMatrix(&matY); MyIdentityMatrix(&matZ);

		m_fAngleY += fDeltaX * M_PI / 180.f * fTime * m_fMouseMoveSpd;
		m_fAngleX += fDeltaY * M_PI / 180.f * fTime * m_fMouseMoveSpd;

		MyQuaternion Qx = MyQuaternion(sinf(m_fAngleX * 0.5f), 0.f, 0.f, cosf(m_fAngleX * 0.5f));
		MyQuaternion Qy = MyQuaternion(0.f, sinf(m_fAngleY * 0.5f), 0.f, cosf(m_fAngleY * 0.5f));
		MyQuaternion Qz = MyQuaternion(0.f, 0.f, 0.f, 1.f);
		MyQuaternion qua = Qz * Qy * Qx;

		MATRIX matQuaRot;
		MyQuternionToMat(&matQuaRot, qua);

		m_matCam = matQuaRot * m_matCam;

		MyVector vMove = MyVector(0.f, 0.f, 0.f);
		KeyInput(fTime, vMove);

		m_vPos = m_vPos + vMove;
		m_matCam._41 = m_vPos.x;	m_matCam._42 = m_vPos.y;	m_matCam._43 = m_vPos.z;

		if (m_bMouseCenter == false)
		{
			SetCursorPos(100.f + WINSIZEX * 0.5f, 100.f + WINSIZEY * 0.5f);

			m_pInputMgr->m_bCheck = true;
			m_pInputMgr->m_fDeltaX = 0.f;
			m_pInputMgr->m_fDeltaY = 0.f;
		}
	}
}

void CCamera::UpdateCamera()
{
	if (m_bFPStype == true)
	{
		float fSize = 5.f;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(WINSIZEX * -fSize * 0.5f, WINSIZEX * fSize* 0.5f, WINSIZEY * -fSize* 0.5f, WINSIZEY * fSize* 0.5f, -10000.0f, 10000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRotatef(90.f, 1.f, 0.f, 0.f);

	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(60.0, WINSIZEX / WINSIZEY, 1.0, 30000.0);
		gluLookAt
		(
			m_matCam._41, m_matCam._42, m_matCam._43,
			m_matCam._31 + m_matCam._41, m_matCam._32 + m_matCam._42, m_matCam._33 + m_matCam._43,
			0.f, 1.f, 0.f
		);
		glRotatef(0.f, 1.f, 0.f, 0.f);
	}
	glMatrixMode(GL_MODELVIEW);
	
}
void CCamera::ZoomInOut()
{
	m_bZooIn ^= true;
}

void CCamera::KeyInput(float fTime, MyVector& rMove)
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		if (m_wKey == VK_ESCAPE) return;
		m_bMouseCenter ^= true;
		m_wKey = VK_ESCAPE;
	}
	else
	{
		m_wKey = 0;
	}


	if (GetAsyncKeyState('W') & 0x8000)
	{
		MyVector vDir = MyVector(m_matCam._31, m_matCam._32, m_matCam._33);
		MyNormalize(&vDir, &vDir);
		rMove = vDir * m_fSpeed * fTime;
		m_wKey = 'W';
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		MyVector vDir = MyVector(m_matCam._31, m_matCam._32, m_matCam._33);
		MyNormalize(&vDir, &vDir);
		rMove = vDir * -m_fSpeed * fTime;
		m_wKey = 'S';
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		MyVector vDir = MyVector(m_matCam._11, m_matCam._12, m_matCam._13);
		MyNormalize(&vDir, &vDir);
		rMove = vDir * m_fSpeed * fTime;
		m_wKey = 'A';
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		MyVector vDir = MyVector(m_matCam._11, m_matCam._12, m_matCam._13);
		MyNormalize(&vDir, &vDir);
		rMove = vDir * -m_fSpeed * fTime;
		m_wKey = 'D';
	}
}