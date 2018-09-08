#include "Weather.h"



CWeather::CWeather()
{
}


CWeather::~CWeather()
{
}

void CWeather::Initialize()
{
	m_vScale = MyVector(1.f, 1.f, 1.f);
	m_fRadius = 10.f;

	m_vInit = m_vPos;
	m_fSpeed = rand() % 100 + 100;
	m_fAccTime = rand() % 1000 * 0.001f;

	m_fXZ = rand() % 4 + 1;
}

int CWeather::Update(float fTime)
{
	m_fAccTime += fTime;
	if (m_iWeatherType == 0)
	{
		m_vPos.x += cosf(m_fAccTime) * m_fXZ;
		m_vPos.z += sinf(m_fAccTime) * (m_fXZ + 1);
		m_fSpeed = 400.f;
	}

	else if(m_iWeatherType == 1)
	{
		m_vPos.x = m_vInit.x;
		m_vPos.z = m_vInit.z;
		m_fSpeed = 1000.f;
	}
	else
	{

	}

	m_vPos.y -= fTime * m_fSpeed;


	if (m_vPos.y < 0)
		m_vPos.y = m_vInit.y;

	return 0;
}

void CWeather::Render()
{
	glPushMatrix();
	{
		glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);

		if (m_iWeatherType == 0)
		{
			glColor4f(1.f, 1.f, 1.f, 1.0f);
			glutSolidSphere(m_fRadius, 10, 10);
		}
		else if(m_iWeatherType == 1)
		{
			glLineWidth(2.f);
			glColor4f(0.1f, 0.2f, 0.8f, 1.0f);
			glBegin(GL_LINES);
			glVertex3f(0.f, m_fRadius * 3.f * m_fXZ, 0.f);
			glVertex3f(0.f, m_fRadius * -3.f * m_fXZ, 0.f);
			glEnd();
			glLineWidth(1.f);
		}
	}
	glPopMatrix();
}

void CWeather::Release()
{
}
