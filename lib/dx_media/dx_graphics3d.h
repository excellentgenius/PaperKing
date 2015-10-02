
//**************************************************************************
//DirectX�R�c�`��Ǘ��N���X
//��{�^�@�F�@Dx_Graphics2D
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#pragma once

//-------------------------------------
//�RDlib setup
#include "dx_camera.h"
#include "media_manager.h"
#include "dx_font.h"
//
typedef D3DXVECTOR2 DxVec2;
typedef D3DXVECTOR3 DxVec3;

//�R�c�֘A�̕`��Ǘ��N���X
class Dx_Graphics3D
{
	private:
		LPDIRECT3DDEVICE9 device;
		/*�J���[���֘A*/
		//�J���[���̓��I�ݒ�J�n
		void BeginColorEnable(D3DCOLOR color);
		//�J���[���̓��I�ݒ�I��
		void EndColorEnable();
	
		/*�`��ݒ荀��*/
		//�����_�[�X�e�[�g�p�����[�^�̐ݒ�
		void SetRenderState(D3DRENDERSTATETYPE State,DWORD Value);

		/*���[���h�ϊ��s��ݒ�֘A*/
		//���W�A�{���A�p�x���烏�[���h�ϊ��s���ݒ�
		D3DXMATRIX SetTransformToDevice(DxVec3 *pos,DxVec3 *scale,DxVec3 *angle);

		//���b�V���T�u�Z�b�g��`��
		void DrawSubset(LPD3DXMESH lpMesh,DWORD AttribId,D3DMATERIAL9 *pMat,LPDIRECT3DTEXTURE9 pTex);

		/*���b�V���֘A*/
		//���b�V���`��
		void DrawMesh(DxMesh *mesh);

		/*�A�j���[�V�������b�V���֘A*/
		//�A�j���[�V�������b�V���`��
		void DrawAnimeMesh(DxAnimeMesh *mesh, D3DXMATRIX mat);
		//�t���[���`��
		void DrawFrame(LPD3DXFRAME pFrame);
		//���b�V�����̍ŏI���[���h�s��̍X�V
		void UpdateFrame(LPD3DXFRAME pFrame,LPD3DXMATRIX mat);
		//���b�V���R���e�i�`��
		void DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, LPD3DXFRAME pFrame);

	public:
		//�R���X�g���N�^
		Dx_Graphics3D(LPDIRECT3DDEVICE9 dev);
		//�f�X�g���N�^
		~Dx_Graphics3D();

		/*�R�c�`��֘A*/
		//�R�c�Z�b�g�A�b�v�@�@�i�J�����g�p�^�j
		void Setup3DEnv(Dx_Camera *camera);
		//�R�c�Z�b�g�A�b�v�@�@�i�s�񒼐ڎw��^�j
		void Setup3DEnv(D3DXMATRIX view,D3DXMATRIX projection);

		//�R�c���f���̕`��
		void DrawModel(DxBaseMesh *mesh,DxVec3 *pos,DxVec3 *scale=NULL,DxVec3 *angle=NULL,D3DCOLOR *color=NULL);
};


