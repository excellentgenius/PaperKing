
//**************************************************************************
//�����e�N�X�`���\����
//��{�^�@�F�@DxTexture
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#pragma once

#include "dx_texture.h"

//?
typedef struct DxPieceTexture : public DxTexture
{
	//�e�N�X�`����̕\���̈�
	RECT tex_pos;
	//�؂������e�N�X�`�����猩�����S���W
	DxVec2 tex_center;
	//�J���[���
	D3DCOLOR color;

	//�R���X�g���N�^
	DxPieceTexture();
}DxPieceTexture;