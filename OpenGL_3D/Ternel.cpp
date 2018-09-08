#include "Ternel.h"



CTernel::CTernel()
{
}


CTernel::~CTernel()
{
}

void CTernel::Initialize()
{
	m_vScale = MyVector(1.f, 1.f, 7.f);
	m_fRadius = 200.f;
	m_vPos.y = m_fRadius * 0.5f * m_vScale.y;

	m_vColor = MyVector(0.0f, 0.6f, 0.75f);
}

int CTernel::Update(float fTime)
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

void CTernel::Render()
{
	glPushMatrix();
	{
		glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);
		glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
		glRotatef(m_vRot.y + 90.f, 0.f, 1.f, 0.f);
		glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

		glColor3f(m_vColor.x, m_vColor.y, m_vColor.z);
		//glutSolidCube(m_fRadius);
		glutSolidTorus(10, 150, 40, 40);
	}
	glPopMatrix();
}

void CTernel::Release()
{
}
