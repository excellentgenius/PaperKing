
//**************************************************************************
//�e�N�X�`���\����
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@DxPieceTexture
//**************************************************************************

#pragma once

#include "dx.h"

//�e�N�X�`���\����
typedef struct DxTexture
{
	//�e�N�X�`���f�[�^
	LPDIRECT3DTEXTURE9 lpTex;
	//�e�N�X�`�����
	D3DXIMAGE_INFO info;
	//�t�@�C����
	string file_name;

	//�R���X�g���N�^
	DxTexture();
}DxTexture;
