
//**************************************************************************
//DirectX�R�c�`��Ǘ��N���X
//��{�^�@�F�@Dx_Graphics2D
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#include "dx_graphics3d.h"


//�R���X�g���N�^
Dx_Graphics3D::Dx_Graphics3D(LPDIRECT3DDEVICE9 device)
{
	this->device = device;
}
//�f�X�g���N�^
Dx_Graphics3D::~Dx_Graphics3D()
{
}

//**************************************************************************
//�ϊ��s��Z�b�g�A�b�v�֘A
//**************************************************************************
//�R�c�Z�b�g�A�b�v�@�@�i�J�����g�p�^�j
void Dx_Graphics3D::Setup3DEnv(Dx_Camera *camera)
{
	//ZBAFA
	this->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
	this->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	//CULLMODE
	this->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	//LIGHT
	this->SetRenderState(D3DRS_LIGHTING,TRUE);

	this->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);

	//�J�����̐���
	camera->Step();

	//�r���[�s��̐ݒ�
	this->device->SetTransform(D3DTS_VIEW,&camera->GetViewMat());
	//�ˉe�s��̐ݒ�
	this->device->SetTransform(D3DTS_PROJECTION,&camera->GetProjectionMat());
}
//�R�c�Z�b�g�A�b�v�@�@�i�s�񒼐ڎw��^�j
void Dx_Graphics3D::Setup3DEnv(D3DXMATRIX view,D3DXMATRIX projection)
{
	//ZBAFA
	this->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
	this->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	//CULLMODE
	this->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	//LIGHT
	this->SetRenderState(D3DRS_LIGHTING,TRUE);

	this->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);

	//�r���[�s��̐ݒ�
	this->device->SetTransform(D3DTS_VIEW,&view);
	//�ˉe�s��̐ݒ�
	this->device->SetTransform(D3DTS_PROJECTION,&projection);
}

void Dx_Graphics3D::SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
{
	this->device->SetRenderState(State,Value);
}
//���W�A�{���A�p�x���烏�[���h�ϊ��s���ݒ�
D3DXMATRIX Dx_Graphics3D::SetTransformToDevice(DxVec3 *pos,DxVec3 *scale,DxVec3 *angle)
{
	D3DXMATRIX		matScale,matRot,matPos,mat;

	//���C���s���P�ʍs��Ƃ���
	D3DXMatrixIdentity(&mat);

	//�{���֘A
	if(scale!=NULL)
	{
		//�X�P�[�����O�s���ݒ�
		D3DXMatrixScaling(&matScale,scale->x,scale->y,scale->z);
		//���C���s��ɃX�P�[�����O�s�������
		D3DXMatrixMultiply(&mat,&mat,&matScale);
	}
	//�p�x�֘A
	if(angle!=NULL)
	{
		//Yaw-Y����]�APitch-X����]�ARoll-Z����]�Ƃ��ĉ�]�s���ݒ�
		D3DXMatrixRotationYawPitchRoll(&matRot,
			D3DXToRadian(angle->y),D3DXToRadian(angle->x),D3DXToRadian(angle->z));
		//���C���s��ɉ�]�s�������
		D3DXMatrixMultiply(&mat,&mat,&matRot);
	}

	//���W�֘A
	{
		//�ʒu�s���ݒ�
		D3DXMatrixTranslation(&matPos,pos->x,pos->y,pos->z);
		//���C���s��Ɉʒu�s�������
		D3DXMatrixMultiply(&mat,&mat,&matPos);
	}

	//�f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	this->device->SetTransform(D3DTS_WORLD,&mat);

	return mat;
}

//**************************************************************************
//�J���[���ݒ�֘A
//**************************************************************************
//�J���[���̓��I�ݒ�J�n
void Dx_Graphics3D::BeginColorEnable(D3DCOLOR color)
{
	this->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	this->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	//D3DRS_TEXTUREFACTOR�̃����_�[�X�e�[�g�l��ݒ�
	this->SetRenderState( D3DRS_TEXTUREFACTOR, color );
	//ARG1��ARG2�̃J���[�l����Z���ăJ���[�l���擾
	this->device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	//�e�N�X�`���̃J���[�l
	this->device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	//���I�̃J���[�l
	this->device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_TFACTOR);

	//ARG1��ARG2�̃��l����Z���ă��l���擾
	this->device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	//�e�N�X�`���̃��l
	this->device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	//���I�̃��l
	this->device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TFACTOR);
}
//�J���[���̓��I�ݒ�I��
void Dx_Graphics3D::EndColorEnable()
{
	//�X�e�[�W0��D3DTSS_COLOROP���f�t�H���g�l�ɐݒ�
	this->device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	//�X�e�[�W0��D3DTSS_COLORARG1���f�t�H���g�l�ɐݒ�
	this->device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	//�X�e�[�W0��D3DTSS_COLORARG2���f�t�H���g�l�ɐݒ�
	this->device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_CURRENT);

	//�X�e�[�W0��D3DTSS_ALPHAOP���f�t�H���g�l�ɐݒ�
	this->device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
	//�X�e�[�W0��D3DTSS_ALPHAARG1���f�t�H���g�l�ɐݒ�
	this->device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	//�X�e�[�W0��D3DTSS_ALPHAARG2���f�t�H���g�l�ɐݒ�
	this->device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_CURRENT);
}

//**************************************************************************
//�R�c���f���`��֘A
//**************************************************************************
//�R�c���f���`��֐�
void Dx_Graphics3D::DrawModel(DxBaseMesh *mesh,DxVec3 *pos,DxVec3 *scale,DxVec3 *angle,D3DCOLOR *color)
{
	//�w�肳�ꂽ���b�V���f�[�^��NULL�łȂ��ꍇ�̂ݕ`��
	if(mesh!=NULL)
	{
		D3DXMATRIX mat;

		//�ϊ��s��̐ݒ�
		mat = this->SetTransformToDevice(pos,scale,angle);

		//�J���[��񂪐ݒ肳��Ă���ꍇ
		if(color!=NULL)
		{
			this->BeginColorEnable(*color);
		}

		//mesh���h���N���XDxMesh�̏ꍇ
		if(dynamic_cast<DxMesh*>(mesh)!=0)
		{
			//DxMesh�̕`��
			this->DrawMesh((DxMesh*)mesh);
		}
		//mesh���h���N���XDxAnimeMesh�̏ꍇ
		else if(dynamic_cast<DxAnimeMesh*>(mesh)!=0)
		{
			//DxAnimeMesh�̕`��
			this->DrawAnimeMesh((DxAnimeMesh*)mesh,mat);
		}

		//�J���[��񂪐ݒ肳��Ă���ꍇ
		if(color!=NULL)
		{
			this->EndColorEnable();
		}
	}
}

//**************************************************************************
//�R�c�`��ɂƂ��Ċj�ƂȂ�֐��֘A
//**************************************************************************
//���b�V���T�u�Z�b�g��`��
void Dx_Graphics3D::DrawSubset(LPD3DXMESH lpMesh,DWORD AttribId,D3DMATERIAL9 *pMat,LPDIRECT3DTEXTURE9 pTex)
{
	//�f�o�C�X�Ƀ}�e���A���v���p�e�B��ݒ�
	this->device->SetMaterial(pMat);
	//�f�o�C�X�̃X�e�[�W�Ƀe�N�X�`�������蓖�Ă�
	this->device->SetTexture(0,pTex);

	//���b�V���T�u�Z�b�g�̑���ID���w�肵�ĕ`��
	lpMesh->DrawSubset(AttribId);
}

//**************************************************************************
//���b�V���`��֘A
//**************************************************************************
//���b�V���`��
void Dx_Graphics3D::DrawMesh(DxMesh *mesh)
{
	//���b�V���Ɋ܂܂�Ă���}�e���A���̐������������s��
	for(DWORD cnt=0;cnt<mesh->matCount;cnt++)
	{
		//���b�V���T�u�Z�b�g��`��
		this->DrawSubset(mesh->lpMesh,cnt,&mesh->pMat[cnt],mesh->pTex[cnt]);
	}
}

//**************************************************************************
//�A�j���[�V�������b�V���`��֘A
//**************************************************************************
//�A�j���[�V�������b�V���`��
void Dx_Graphics3D::DrawAnimeMesh(DxAnimeMesh *mesh, D3DXMATRIX mat)
{
	//�S�t���[���̍ŏI���[���h�s��̍X�V
	this->UpdateFrame(mesh->pFrameRoot,&mat);
	//�t���[���`��
	this->DrawFrame(mesh->pFrameRoot);
}
//�t���[���`��
void Dx_Graphics3D::DrawFrame(LPD3DXFRAME pFrame)
{
	DxFrame *frame = (DxFrame*)pFrame;

	//���b�V���R���e�i�����݂���ꍇ
	if(frame->pMeshContainer!=NULL)
	{
		//���b�V���R���e�i��`��
		this->DrawMeshContainer(frame->pMeshContainer,frame);
	}

	//�Z��t���[�������݂���ꍇ
	if(frame->pFrameSibling!=NULL)
	{
		//�Z��t���[����`��
		this->DrawFrame(frame->pFrameSibling);
	}
	//�q�t���[�������݂���ꍇ
	if(frame->pFrameFirstChild!=NULL)
	{
		//�q�t���[����`��
		this->DrawFrame(frame->pFrameFirstChild);
	}
}
//���b�V�����̍ŏI���[���h�s��̍X�V
void Dx_Graphics3D::UpdateFrame(LPD3DXFRAME pFrame,LPD3DXMATRIX mat)
{
	//DxFrame�ɃL���X�g����
	DxFrame *frame = (DxFrame*)pFrame;

	//��{�s��f�[�^�����݂���ꍇ
	if(mat!=NULL)
	{
		//�ŏI���[���h�s��ɕϊ��s��Ɗ�{�s��̍������o��
		D3DXMatrixMultiply( &frame->CombinedTransformationMatrix, &frame->TransformationMatrix, mat );
	}
	else
	{ 
		//�ŏI���[���h�s��Ɏ��g�̕ϊ��s����o��
		frame->CombinedTransformationMatrix = frame->TransformationMatrix;
	}

	//�Z��t���[�������݂���ꍇ
	if(frame->pFrameSibling!=NULL)
	{
		//�Z��t���[���̍X�V
		this->UpdateFrame(frame->pFrameSibling,mat);
	}
	//�q�t���[�������݂���ꍇ
	if(frame->pFrameFirstChild!=NULL)
	{
		//�q�t���[���̍X�V
		this->UpdateFrame(frame->pFrameFirstChild,&frame->CombinedTransformationMatrix);
	}
}
//���b�V���R���e�i�`��
void Dx_Graphics3D::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, LPD3DXFRAME pFrame)
{
	DxMeshContainer *mesh_container = (DxMeshContainer*)pMeshContainer;
	DxFrame *frame = (DxFrame*)pFrame;

	//
	D3DMATERIAL9 mat;
	LPDIRECT3DTEXTURE9 pTex=NULL;
	//D3DXMESHDATA���̃��b�V���f�[�^�𒊏o
	LPD3DXMESH lpMesh = mesh_container->MeshData.pMesh;

	//�X�L�j���O��񂪂Ȃ��ꍇ
	if(pMeshContainer->pSkinInfo==NULL)
	{
		//�f�o�C�X�Ƀt���[���̃��[���h�s���ݒu
		this->device->SetTransform( D3DTS_WORLD, &frame->CombinedTransformationMatrix);

		//���b�V���`��
		for (DWORD i=0;i<mesh_container->NumMaterials;i++)
		{
			//�}�e���A�����̎擾
			mat = mesh_container->pMaterials[i].MatD3D;
			//�e�N�X�`�����̎擾
			pTex = mesh_container->ppTextures[i];

			//���b�V���T�u�Z�b�g��`��
			this->DrawSubset(lpMesh,i,&mat,pTex);
		}
	}
	//�X�L�j���O��񂪂���ꍇ
	else
	{
		D3DXMATRIX matId;
		PBYTE			pVerticesSrc;
		PBYTE			pVerticesDest;

		//�{�[�������擾
		DWORD NumBones = pMeshContainer->pSkinInfo->GetNumBones();

		for( DWORD i = 0; i < NumBones; i++ ){
			D3DXMatrixMultiply(
				&mesh_container->pBoneMatrices[i],
				&mesh_container->pBoneOffsetMatrices[i], 
				mesh_container->ppBoneMatrixPtrs[i]
			);
		}

		//���[���h�s����N���A
		D3DXMatrixIdentity(&matId);
		this->device->SetTransform(D3DTS_WORLD, &matId);

		//���_�o�b�t�@�����b�N
		mesh_container->lpMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pVerticesSrc);
		lpMesh->LockVertexBuffer( 0, (LPVOID*)&pVerticesDest);

		//�X�L�����b�V���쐬
		mesh_container->pSkinInfo->UpdateSkinnedMesh( mesh_container->pBoneMatrices, NULL, pVerticesSrc, pVerticesDest);

		//���_�o�b�t�@�̃��b�N������
		mesh_container->lpMesh->UnlockVertexBuffer();
		lpMesh->UnlockVertexBuffer();

		//���b�V���`��
		for(UINT i = 0;i<mesh_container->NumAttributeGroups;i++)
		{
			//���b�V���T�u�Z�b�g�̑���ID���擾
			unsigned AttribId = mesh_container->pAttributeTable[i].AttribId;
			//�}�e���A�������擾
			mat = mesh_container->pMaterials[AttribId].MatD3D;
			//�e�N�X�`�������擾
			pTex = mesh_container->ppTextures[AttribId];

			//���b�V���̃T�u�Z�b�g��`��
			this->DrawSubset(lpMesh,AttribId,&mat,pTex);
		}
	}
}
