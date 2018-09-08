#include "CubePlayer.h"

Engine::CCubePlayer::CCubePlayer(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CCubePlayer::~CCubePlayer(void)
{
}

Engine::CResources* Engine::CCubePlayer::CloneResource(void)
{
	++(*m_pwRefCnt);

	return new CCubePlayer(*this);
}

HRESULT Engine::CCubePlayer::CreateBuffer(void)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = /*8*/ 24;
	m_dwFVF = VTXTEX_FVF;

	m_dwIdxSize = sizeof(INDEX16);
	m_dwTriCnt = 12;
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK(CVIBuffer::CreateBuffer());

	VTXTEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	//
	pVertex[0].vPos		= D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVertex[0].vTexUV	= D3DXVECTOR2(0.f, 1.f);

	pVertex[15].vPos	= D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVertex[15].vTexUV	= D3DXVECTOR2(1.f, 1.f);

	pVertex[20].vPos	= D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVertex[20].vTexUV	= D3DXVECTOR2(0.f, 1.f);
	//
	pVertex[1].vPos		= D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVertex[1].vTexUV	= D3DXVECTOR2(0.f, 0.f);

	pVertex[14].vPos	= D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVertex[14].vTexUV	= D3DXVECTOR2(1.f, 0.f);

	pVertex[16].vPos	= D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVertex[16].vTexUV	= D3DXVECTOR2(0.f, 1.f);
	//
	pVertex[2].vPos		= D3DXVECTOR3( 1.f, 1.f, -1.f);
	pVertex[2].vTexUV	= D3DXVECTOR2(1.f, 0.f);

	pVertex[5].vPos		= D3DXVECTOR3( 1.f, 1.f, -1.f);
	pVertex[5].vTexUV	= D3DXVECTOR2(0.f, 0.f);

	pVertex[19].vPos	= D3DXVECTOR3( 1.f, 1.f, -1.f);
	pVertex[19].vTexUV	= D3DXVECTOR2(1.f, 1.f);
	//
	pVertex[3].vPos		= D3DXVECTOR3( 1.f,-1.f, -1.f);
	pVertex[3].vTexUV	= D3DXVECTOR2(1.f, 1.f);

	pVertex[4].vPos		= D3DXVECTOR3( 1.f,-1.f, -1.f);
	pVertex[4].vTexUV	= D3DXVECTOR2(0.f, 1.f);

	pVertex[23].vPos	= D3DXVECTOR3( 1.f,-1.f, -1.f);
	pVertex[23].vTexUV	= D3DXVECTOR2(1.f, 1.f);
	//
	pVertex[10].vPos	= D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVertex[10].vTexUV	= D3DXVECTOR2(1.f, 0.f);

	pVertex[13].vPos	= D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVertex[13].vTexUV	= D3DXVECTOR2(0.f, 0.f);

	pVertex[17].vPos	= D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVertex[17].vTexUV	= D3DXVECTOR2(0.f, 0.f);
	//
	pVertex[11].vPos	= D3DXVECTOR3(-1.f,-1.f, 1.f);
	pVertex[11].vTexUV	= D3DXVECTOR2(1.f, 1.f);

	pVertex[12].vPos	= D3DXVECTOR3(-1.f,-1.f, 1.f);
	pVertex[12].vTexUV	= D3DXVECTOR2(0.f, 1.f);

	pVertex[21].vPos	= D3DXVECTOR3(-1.f,-1.f, 1.f);
	pVertex[21].vTexUV	= D3DXVECTOR2(0.f, 0.f);
	//
	pVertex[6].vPos		= D3DXVECTOR3( 1.f, 1.f, 1.f);
	pVertex[6].vTexUV	= D3DXVECTOR2(1.f, 0.f);

	pVertex[9].vPos		= D3DXVECTOR3( 1.f, 1.f, 1.f);
	pVertex[9].vTexUV	= D3DXVECTOR2(0.f, 0.f);

	pVertex[18].vPos	= D3DXVECTOR3( 1.f, 1.f, 1.f);
	pVertex[18].vTexUV	= D3DXVECTOR2(1.f, 0.f);
	//
	pVertex[7].vPos		= D3DXVECTOR3( 1.f,-1.f, 1.f);
	pVertex[7].vTexUV	= D3DXVECTOR2(1.f, 1.f);

	pVertex[8].vPos		= D3DXVECTOR3( 1.f,-1.f, 1.f);
	pVertex[8].vTexUV	= D3DXVECTOR2(0.f, 1.f);

	pVertex[22].vPos	= D3DXVECTOR3( 1.f,-1.f, 1.f);
	pVertex[22].vTexUV	= D3DXVECTOR2(1.f, 0.f);	



	m_pVB->Unlock();


	INDEX16*		pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//+x
	pIndex[0]._1 = 0;	pIndex[0]._2 = 1;	pIndex[0]._3 = 2;
	pIndex[1]._1 = 2;	pIndex[1]._2 = 3;	pIndex[1]._3 = 0;
	//-x
	pIndex[2]._1 = 4;	pIndex[2]._2 = 5;	pIndex[2]._3 = 6;
	pIndex[3]._1 = 6;	pIndex[3]._2 = 7;	pIndex[3]._3 = 4;
	//+y
	pIndex[4]._1 = 8;	pIndex[4]._2 = 9;	pIndex[4]._3 = 10;
	pIndex[5]._1 = 10;	pIndex[5]._2 = 11;	pIndex[5]._3 = 8;
	//-y
	pIndex[6]._1 = 12;	pIndex[6]._2 = 13;	pIndex[6]._3 = 14;
	pIndex[7]._1 = 14;	pIndex[7]._2 = 15;	pIndex[7]._3 = 12;
	//+z
	pIndex[8]._1 = 16;	pIndex[8]._2 = 17;	pIndex[8]._3 = 18;
	pIndex[9]._1 = 18;	pIndex[9]._2 = 19;	pIndex[9]._3 = 16;
	//-z
	pIndex[10]._1 = 20;	pIndex[10]._2 = 21;	pIndex[10]._3 = 22;
	pIndex[11]._1 = 22;	pIndex[11]._2 = 23;	pIndex[11]._3 = 20;

	m_pIB->Unlock();

	return S_OK;
}

Engine::CCubePlayer* Engine::CCubePlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubePlayer* pBuffer = new CCubePlayer(pDevice);
	if(FAILED(pBuffer->CreateBuffer()))
		Engine::Safe_Delete(pBuffer);

	return pBuffer;
}