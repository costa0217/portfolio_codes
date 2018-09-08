#include "Rail.h"



CRail::CRail()
{
}


CRail::~CRail()
{
}

void CRail::Initialize()
{
	fTerm = 100.f;
	m_vScale = MyVector(1.f, 1.f, 1.f);
	m_fRadius = 40.f;
	m_vPos.y = m_fRadius * 0.5f * m_vScale.y;

	m_vColor = MyVector(0.8f, 0.8f, 0.8f);
}

int CRail::Update(float fTime)
{
	MyVector vDirXZ = MyVector(m_vDir.x, 0.f, m_vDir.z);
	MyNormalize(&vDirXZ, &vDirXZ);
	float fYAngle = acosf(Vec3Dot(vDirXZ, MyVector(1.f, 0.f, 0.f)));
	if (vDirXZ.z > 0)
	{
		fYAngle = M_PI * 2.f - fYAngle;
	}
	fYAngle *= 180 / M_PI;
	m_vRot.y = fYAngle;
	return 0;
}

void CRail::Render()
{
	glPushMatrix();
	{
		//glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z + fTerm * 0.5f);
		glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);
		glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
		glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
		glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);
		glTranslatef(0.f, 0.f, fTerm * 0.5f);
		
		glColor3f(m_vColor.x, m_vColor.y, m_vColor.z);
		glutSolidCube(m_fRadius);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(m_vPos.x , m_vPos.y, m_vPos.z );
		glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
		glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
		glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);
		glTranslatef(0.f, 0.f, fTerm * -0.5f);
	
		glColor3f(m_vColor.x, m_vColor.y, m_vColor.z);
		glutSolidCube(m_fRadius);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);
		glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
		glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
		glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

		glLineWidth(3.f);
		glColor3f(0.f, 0.f, 0.f);
		glBegin(GL_QUADS);

		glVertex3f(fTerm * -0.1f, 0.f, -fTerm * 0.7f);
		glVertex3f(fTerm * 0.1f, 0.f, -fTerm * 0.7f);
		glVertex3f(fTerm * 0.1f, 0.f, fTerm* 0.7f);
		glVertex3f(fTerm * -0.1f, 0.f, fTerm* 0.7f);

		glEnd();
		glLineWidth(1.f);
	}
	glPopMatrix();
}

void CRail::Release()
{
}
