
//**************************************************************************
//�A�j���[�V�������b�V���֘A
// DxHierarcy		: �t���[���y�у��b�V���R���e�i����p�N���X
// DxFrame			: �h���t���[���\����
// DxMeshContainer	: �h�����b�V���R���e�i�\����
// DxAnimeMesh		: �A�j���[�V�������b�V���\����
//**************************************************************************

#pragma once

#include "dx.h"

#include "dx_base_mesh.h"
#include "texture_manager.h"

#include "dx_anime_cont_auxiliary.h"

#include <string>
using namespace std;

//�h���t���[���\����
typedef struct DxFrame: public D3DXFRAME
{
	//���b�V�����̍ŏI���[���h�s��
    D3DXMATRIX					CombinedTransformationMatrix;
}DxFrame;

//�h�����b�V���R���e�i�\����
typedef struct DxMeshContainer :public D3DXMESHCONTAINER
{
	//�e�N�X�`���S
	LPDIRECT3DTEXTURE9*			ppTextures;

	//���b�V���̃|�C���^
	LPD3DXMESH					lpMesh;
	//�������_���
	LPD3DXATTRIBUTERANGE		pAttributeTable;
	//�������_��
	DWORD						NumAttributeGroups;

	LPD3DXBUFFER				pBoneCombinationBuf;
	D3DXMATRIX**				ppBoneMatrixPtrs;
	//�����ʒu�̍s��
	D3DXMATRIX*					pBoneOffsetMatrices;
	//�{�[���̐�
	UINT						NumBoneMatricesMax;
	//�{�[���s��
	D3DXMATRIXA16*				pBoneMatrices;
}DxMeshContainer;

//�t���[���⃁�b�V���R���e�i�̊��蓖�Ă܂��͊J�����s���N���X
//�h���N���X�ɂ��邱�ƂœƎ��̃t���[���⃁�b�V���R���e�i�̊��蓖�ĂȂǂ��ł���悤�ɂȂ�
class Dx_Hierarchy : public ID3DXAllocateHierarchy
{
	private:
		//�e�N�X�`���Ǘ��N���X
		TextureManager *t_manager;

	public:
		//�R���X�g���N�^
		Dx_Hierarchy(TextureManager *t_manager);
		//�Ǝ��t���[���̐���
		STDMETHOD(CreateFrame)(THIS_ LPCSTR , LPD3DXFRAME *);
		//�Ǝ����b�V���R���e�i�̐���
		STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR ,CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , 
			CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
		//�{�[���̐���
		HRESULT CreateBone(DxMeshContainer *pMeshContainer,LPDIRECT3DDEVICE9 lpD3DDev);

		//�Ǝ��t���[���̔j������ъJ��
		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME );
		//�Ǝ����b�V���R���e�i�̔j������ъJ��
		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER );
};

//�A�j���[�V�������b�V���\����
typedef struct DxAnimeMesh : public DxBaseMesh
{
	//�t���[���K�w�̍ŏ�ʃm�[�h
	LPD3DXFRAME					pFrameRoot;
	//�A�j���[�V�����R���g���[��
	LPD3DXANIMATIONCONTROLLER	pAnimController;

	//�R���X�g���N�^
	DxAnimeMesh();
	//�t�@�C�����̎擾
	string GetFileName();
}DxAnimeMesh;