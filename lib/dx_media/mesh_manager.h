
//**************************************************************************
//���b�V���f�[�^�Ǘ��N���X
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#pragma once
#include "dx.h"

#include "dx_base_mesh.h"
#include "dx_mesh.h"
#include "dx_anime_mesh.h"

#include "texture_manager.h"

//���b�V���f�[�^�Ǘ��p�N���X
class MeshManager
{
	private:
		//�f�o�C�X�A�h���X
		LPDIRECT3DDEVICE9 dev;
		//�e�N�X�`���Ǘ��N���X
		TextureManager *t_manager;
		//
		Dx_Hierarchy *hierarchy;

		//�o�^���b�V���S
		std::vector<DxBaseMesh*> mesh_data;
		//�o�^���b�V����
		short mesh_val;

	public:
		//�R���X�g���N�^
		MeshManager(LPDIRECT3DDEVICE9 dev,TextureManager *t_manager);
		//�f�X�g���N�^
		~MeshManager();

		/*���b�V���f�[�^�֘A*/
		//X�t�@�C������̂R�c���f���f�[�^�����[�h�@�@�i�ȈՔŁA�A�j���[�V��������s�j
		DxMesh* LoadMeshFromX(const string& fName,const string& path);

		/*�A�j���[�V�������b�V���f�[�^�֘A*/
		//X�t�@�C������̂R�c���f���f�[�^�����[�h�@�@�i�ڍהŁA�A�j���[�V��������\�j
		DxAnimeMesh* LoadAnimeMeshFromX(const string& fName,const string& path);

	private:
		/*���b�V���f�[�^�֘A*/
		//X�t�@�C������̂R�c���f���f�[�^�����[�h�@�@�i�ȈՔŁA�A�j���[�V��������s�j
		HRESULT LoadMeshFromX(DxMesh *mesh,const string& fName,const string& path);
		//DxMesh�̓����f�[�^�̔j������ъJ��
		void ReleaseMesh(DxBaseMesh *base_mesh);

		/*�A�j���[�V�������b�V���f�[�^�֘A*/
		//X�t�@�C������̂R�c���f���f�[�^�����[�h�@�@�i�ڍהŁA�A�j���[�V��������\�j
		HRESULT LoadAnimeMeshFromX(DxAnimeMesh *mesh,const string& fName,const string& path);
		//�A�j���̂ݕʂɓǂݍ��ݍ쐬
		HRESULT LoadAnimeFromX(LPD3DXANIMATIONCONTROLLER &pAnimControllerTmp,DxAnimeMesh *mesh,const string& fName,const string& path);

		//DxAnimeMesh�̓����f�[�^�̔j������ъJ��
		void ReleaseAnimeMesh(DxBaseMesh *base_mesh);

		/*�{�[���ݒ�֘A*/
		//�t���[�������������ă��b�V���R���e�i���̃{�[����ݒ�
		HRESULT SetupBoneFromFrame(LPD3DXFRAME pFrameRoot,LPD3DXFRAME pFrame);
		//���b�V���R���e�i���̃{�[����ݒ�
		HRESULT SetupBoneFromMeshContainer(LPD3DXFRAME pFrameRoot,LPD3DXMESHCONTAINER pMeshCont);

		//�o�^�ς݃��b�V���f�[�^�̃t�@�C�������`�F�b�N���d�����[�h��h�~����
		DxBaseMesh* CheckFileName(const string& fName);
};