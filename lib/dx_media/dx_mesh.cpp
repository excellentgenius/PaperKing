
//**************************************************************************
//���b�V���\����
//��{�^�@�F�@DxBaseMesh
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#include "dx_mesh.h"


//�R���X�g���N�^
DxMesh::DxMesh() : DxBaseMesh()
{
	this->lpMesh = NULL;
	this->matCount = 0;
	this->pMat = NULL;
	this->pTex = NULL;
}
//�t�@�C�����̎擾
string DxMesh::GetFileName()
{
	return this->file_name;
}