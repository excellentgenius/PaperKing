
//**************************************************************************
//���b�V���\����
//��{�^�@�F�@DxBaseMesh
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#pragma once

#include "dx_base_mesh.h"

//���b�V���\����
typedef struct DxMesh : public DxBaseMesh
{
	//���b�V�����
	LPD3DXMESH				lpMesh;
	//�}�e���A����
	DWORD					matCount;
	//�}�e���A���S
	D3DMATERIAL9			*pMat;
	//�e�N�X�`���S
	LPDIRECT3DTEXTURE9		*pTex;

	//�R���X�g���N�^
	DxMesh();
	//�t�@�C�����̎擾
	string GetFileName();
}DxMesh;
