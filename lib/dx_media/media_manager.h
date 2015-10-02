
//**************************************************************************
//���f�B�A�Ǘ��N���X
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#pragma once

#include "texture_manager.h"
#include "mesh_manager.h"

//���f�B�A�Ǘ��N���X
class MediaManager
{
	private:
		//�e�N�X�`���Ǘ��N���X
		TextureManager *tex_manager;
		//���b�V���Ǘ��N���X
		MeshManager *mesh_manager;

	public:
		//�R���X�g���N�^
		MediaManager(LPDIRECT3DDEVICE9 dev);
		//�f�X�g���N�^
		~MediaManager();

		/*�e�N�X�`���֘A*/
		//�C���[�W�t�@�C������DxTexture�����[�h�@�@�i�ȈՔŁj
		LPDIRECT3DTEXTURE9 LoadTextureFromFile(string fName,string path);
		//�C���[�W�t�@�C������DxTexture�����[�h�@�@�i�ڍהŁj
		DxTexture* LoadTextureFromFileEx(string fName,string path);

		/*�R�c���f���֘A*/
		//X�t�@�C������̂R�c���f���f�[�^�����[�h�@�@�i�ȈՔŁA�A�j���[�V��������s�j
		DxMesh* LoadMeshFromX(string fName,string path);
		//X�t�@�C������̂R�c���f���f�[�^�����[�h�@�@�i�ڍהŁA�A�j���[�V��������\�j
		DxAnimeMesh* LoadAnimeMeshFromX(string fName,string path);
};

