#include "stdafx.h"
#include "AnimationMesh.h"

bool CAnimationMesh::Begin(UINT index){
	//mesh�� ���� �ε���, �븻 ������ ����
	CFileBasedMesh::Begin(index);
	//m_MeshIndex = index;

	return true;
}

bool CAnimationMesh::End(){
	//delete[] m_pxmf4BoneIndex;
	//delete[] m_pxmf3Weight;
	
	return CFileBasedMesh::End();
}
