#include "PartiEffect.h"
#include "MyParticle.h"

CPartiEffect::CPartiEffect()
{
}
CPartiEffect::~CPartiEffect()
{
}

void CPartiEffect::Initialize()
{
	m_fRadius = 2.f;
	m_vScale = MyVector(1.f, 1.f, 1.f);
	m_eType = OBJ_EFFECT;
	m_vColor = MyVector(0.f, 1.f, 1.f);
	m_vecParticle.reserve(200);

	for (float phi = -90.0; phi <= 70.0; phi += 20.0)
	{

		for (float theta = -180.0; theta <= 180.0; theta += 20.0)
		{
			MyVector vDir = MyVector(sin(theta)*cos(phi), cos(theta)*cos(phi), sin(phi));
			if (Vec3Dot(vDir, m_vDir) >= 0)
			{
				CMyParticle* pObj = new CMyParticle(vDir, m_vPos);
				pObj->m_eTarget  = m_eTarget;
				pObj->Initialize();
				m_vecParticle.push_back(pObj);
			}
			vDir = MyVector(sin(theta)*cos(phi + 20.0), cos(theta)*cos(phi + 20.0), sin(phi + 20.0));
			if (Vec3Dot(vDir, m_vDir) >= 0)
			{
				CMyParticle* pObj = new CMyParticle(vDir, m_vPos);
				pObj->m_eTarget = m_eTarget;
				pObj->Initialize();
				m_vecParticle.push_back(pObj);
			}
		}
	}


}

int CPartiEffect::Update(float fTime)
{
	vector<CMyParticle*>::iterator iter = m_vecParticle.begin();
	for (; iter != m_vecParticle.end();)
	{
		int iResult = (*iter)->Update(fTime);
		if (0 > iResult)
		{
			Safe_Delete((*iter));
			iter = m_vecParticle.erase(iter);
		}
		else
			++iter;
	}
	if (m_vecParticle.empty())
		return -1;

	return 0;
}

void CPartiEffect::Render()
{
	glLineWidth(1.f);
	glPushMatrix();
	{
		glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);
		glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
		glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
		glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

		glColor4f(m_vColor.x, m_vColor.y, m_vColor.z, 1.0f);
		glutSolidSphere(m_fRadius, 20, 20);
		glColor4f(0.f, 0.f, 0.f, 1.0f);
		glutWireSphere(m_fRadius + 1, 12, 12);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);

		//glGetFloatv(GL_MODELVIEW_MATRIX, m_mWorld);
		MyIdentityMatrix(&m_matWorld);
		MyTranslation(&m_matWorld, m_vPos.x, m_vPos.y, m_vPos.z);
		MyScaling(&m_matWorld, 1.f, 1.f, 1.f);
		//glColor4f(1.f, 1.f, 1.f, 1.0f);
		//glutWireCube(102);
	}
	glPopMatrix();

	size_t VecSize = m_vecParticle.size();
	for (size_t i = 0; i < VecSize; ++i)
	{
		m_vecParticle[i]->Render();
	}
}

void CPartiEffect::Release()
{
}
