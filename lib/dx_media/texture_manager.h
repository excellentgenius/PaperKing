
//**************************************************************************
//�e�N�X�`���Ǘ��N���X
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#pragma once


#include "dx_texture.h"

//�e�N�X�`���Ǘ��N���X
class TextureManager
{
	private:
		//�f�o�C�X�A�h���X
		LPDIRECT3DDEVICE9 dev;

		//�o�^�e�N�X�`���S
		vector<DxTexture*> tex_data;
		//�o�^�e�N�X�`����
		short tex_val;

	public:
		//�R���X�g���N�^
		TextureManager(LPDIRECT3DDEVICE9 dev);
		//�f�X�g���N�^
		~TextureManager();

		//�C���[�W�t�@�C������DxTexture�����[�h�@�@�i�ȈՔŁj
		LPDIRECT3DTEXTURE9 LoadTextureFromFile(const string& fName,const string& path);
		//�C���[�W�t�@�C������DxTexture�����[�h�@�@�i�ڍהŁj
		DxTexture* LoadTextureFromFileEx(const string& fName,const string& path);

	private:
		//�C���[�W�t�@�C������DxTexture�����[�h�@�@�i�ȈՔŁj
		HRESULT LoadTextureFromFile(DxTexture* tex,const string& fName,const string& path);
		//�C���[�W�t�@�C������DxTexture�����[�h�@�@�i�ڍהŁj
		HRESULT LoadTextureFromFileEx(DxTexture* tex,const string& fName,const string& path);

		//�e�N�X�`���f�[�^�̊J��
		void ReleaseTexture(DxTexture *tex);

		//�o�^�ς݃e�N�X�`���f�[�^�̃t�@�C�������`�F�b�N���d�����[�h��h�~����
		DxTexture* CheckFileName(const string& fName);
};

