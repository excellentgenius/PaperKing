
//**************************************************************************
//���b�V���f�[�^�Ǘ��N���X
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@�Ȃ�
//**************************************************************************


#include "mesh_manager.h"


//�R���X�g���N�^
MeshManager::MeshManager(LPDIRECT3DDEVICE9 dev,TextureManager *t_manager)
{
	//�f�o�C�X�|�C���^�̐ݒ�
	this->dev = dev;
	//�e�N�X�`���Ǘ��N���X�|�C���^�̐ݒ�
	this->t_manager = t_manager;

	//Dx_Hierarchy�̐���
	this->hierarchy = new Dx_Hierarchy(t_manager);

	//�o�^���b�V�����̏�����
	this->mesh_val = 0;
}

//�f�X�g���N�^
MeshManager::~MeshManager()
{
	//
	vector<DxBaseMesh*>::iterator it;

	//vector���̃I�u�W�F�N�g�����ׂĔj������ъJ��
	for(it=this->mesh_data.begin();it!=this->mesh_data.end();it++)
	{
		//�o�^�I�u�W�F�N�g��DxMesh�̏ꍇ
		if(dynamic_cast<DxMesh*>(*it)!=0)
		{
			this->ReleaseMesh((*it));
			continue;
		}
		//�o�^�I�u�W�F�N�g��DxAnimeMesh�̏ꍇ
		if(dynamic_cast<DxAnimeMesh*>(*it)!=0)
		{
			this->ReleaseAnimeMesh((*it));
			continue;
		}
	}
	this->mesh_data.clear();

	//
	SAFE_DELETE(this->hierarchy);
	//
	this->t_manager = NULL;
	//
	this->dev = NULL;
}

//**************************************************************************
//�O���Q�Ɨp���[�h�֐�
//**************************************************************************
//X�t�@�C������̂R�c���f���f�[�^�����[�h�@�@�i�ȈՔŁA�A�j���[�V��������s�j
DxMesh* MeshManager::LoadMeshFromX(const string& fName,const string& path)
{
	//�����p�I�u�W�F�N�g�̐錾
	DxMesh *create = NULL;

	//�d�����[�h�h�~�̂��߃t�@�C�����`�F�b�N���s��
	if((create=(DxMesh*)this->CheckFileName(fName))!=NULL)
	{
		//�o�^�ς݃I�u�W�F�N�g��߂�l�Ƃ���
		return create;
	}

	//�I�u�W�F�N�g�̐���
	create = new DxMesh;
	//�����p�I�u�W�F�N�g�Ƀp�X�y�уt�@�C�������烍�[�h
	if(FAILED(this->LoadMeshFromX(create,fName,path)))
	{
		//���[�h�Ɏ��s�����ꍇ�A�̈�m�ۂ�������j��
		SAFE_DELETE(create);
		return NULL;
	}

	//���b�V���S�ɓo�^
	this->mesh_data.push_back(create);
	//���b�V���f�[�^�̑��������Z
	this->mesh_val++;

	//�V���ɐ������ꂽ�I�u�W�F�N�g�̃A�h���X��Ԃ�
	return create;
}
//X�t�@�C������̂R�c���f���f�[�^�����[�h�@�@�i�ڍהŁA�A�j���[�V��������\�j
DxAnimeMesh* MeshManager::LoadAnimeMeshFromX(const string& fName,const string& path)
{
	//�����p�I�u�W�F�N�g�̐錾
	DxAnimeMesh *create = NULL;

	//�d�����[�h�h�~�̂��߃t�@�C�����`�F�b�N���s��
	if((create=(DxAnimeMesh*)this->CheckFileName(fName))!=NULL)
	{
		//�o�^�ς݃I�u�W�F�N�g��߂�l�Ƃ���
		this->LoadAnimeFromX(create->pAnimController,create,fName,path);
		return create;
	}

	//�I�u�W�F�N�g�̐���
	create = new DxAnimeMesh;

	//�����p�I�u�W�F�N�g�Ƀp�X�y�уt�@�C�������烍�[�h
	if(FAILED(this->LoadAnimeMeshFromX(create,fName,path)))
	{
		//���[�h�Ɏ��s�����ꍇ�A�̈�m�ۂ�������j��
		SAFE_DELETE(create);
		return NULL;
	}

	//���b�V���S�ɓo�^
	this->mesh_data.push_back(create);
	//���b�V���f�[�^�̑��������Z
	this->mesh_val++;

	//���������I�u�W�F�N�g�̃A�h���X��߂�
	return create;
}

//**************************************************************************
//�����Q�Ɨp���[�h�֐�
//**************************************************************************
//X�t�@�C������̂R�c���f���f�[�^�����[�h�@�@�i�ȈՔŁA�A�j���[�V��������s�j
HRESULT MeshManager::LoadMeshFromX(DxMesh *mesh,const string& fName,const string& path)
{
	//���[�h�p�o�b�t�@
	LPD3DXBUFFER		lpD3DBuffer;
	D3DXMATERIAL		*d3dxMat;

	string pathfilename = path + fName;
	//���b�V���f�[�^�̃��[�h
	if(FAILED(D3DXLoadMeshFromX(pathfilename.c_str(),D3DXMESH_SYSTEMMEM,this->dev,
			nullptr,&lpD3DBuffer,NULL,&mesh->matCount,&mesh->lpMesh)))
	{
		return E_FAIL;
	}

	//�t�@�C�����̐ݒu
	mesh->file_name+=fName;

	//���[�h�p�o�b�t�@����}�e���A�����S���擾
	d3dxMat = (D3DXMATERIAL*)lpD3DBuffer->GetBufferPointer();

	//�}�e���A�������̃}�e���A���z��𐶐�
	mesh->pMat=new D3DMATERIAL9[mesh->matCount];
	//�}�e���A�������̃e�N�X�`���z��𐶐�
	mesh->pTex = new LPDIRECT3DTEXTURE9[mesh->matCount];

	//�}�e���A�������J��Ԃ�
	for(DWORD i=0;i<mesh->matCount;i++)
	{
		//�}�e���A�����̎擾
		mesh->pMat[i]=d3dxMat[i].MatD3D;
		//�e�N�X�`�����̏�����
		mesh->pTex[i] = NULL;
		//�t�@�C�������擾
		LPSTR tex_name = d3dxMat[i].pTextureFilename;
		//�}�e���A�����ɖ��O�̏�񂪑��݂���ꍇ�A�e�N�X�`�������擾
		if(d3dxMat[i].pTextureFilename!=NULL)
		{
			mesh->pTex[i] = this->t_manager->LoadTextureFromFile(tex_name,path);
		}
	}

	//���[�h�p�o�b�t�@���̏����J��
	lpD3DBuffer->Release();

	return S_OK;
}
//X�t�@�C������̂R�c���f���f�[�^�����[�h�@�@�i�ڍהŁA�A�j���[�V��������\�j
HRESULT MeshManager::LoadAnimeMeshFromX(DxAnimeMesh *mesh,const string& fName,const string& path)
{
	//�ǂݍ��ݗp�A�j���[�V�����R���g���[��
	LPD3DXANIMATIONCONTROLLER pAnimControllerTmp = NULL;

	string str = path + fName;

	//X�t�@�C������t���[���K�w�ƃA�j���[�V�����������[�h
	if(FAILED(D3DXLoadMeshHierarchyFromX(
		str.c_str(),
		D3DXMESH_MANAGED,
		this->dev,
		this->hierarchy,
		NULL,
		&mesh->pFrameRoot,
		&pAnimControllerTmp)))
	{
		return E_FAIL;
	}

	//�t�@�C�����̐ݒu
	mesh->file_name=fName;

	if(FAILED(LoadAnimeFromX(pAnimControllerTmp,mesh,fName,path)))
	{
		return E_FAIL;
	}
	SAFE_RELEASE(pAnimControllerTmp);
	return S_OK;
}

HRESULT MeshManager::LoadAnimeFromX(LPD3DXANIMATIONCONTROLLER &pAnimControllerTmp,DxAnimeMesh *mesh,
																		const string& fName,const string& path)
{
	//�ǂݍ��ݗp�A�j���[�V�����R���g���[��
//	LPD3DXANIMATIONCONTROLLER pAnimControllerTmp = NULL;

	/*�A�j���[�V�����R���g���[���֘A*/
	//�A�j���[�V�����R���g���[���̐ݒ�
	if(pAnimControllerTmp!=NULL)
	{
		//�g���b�N�����A�j���[�V�����Z�b�g���ɑ��₷
		unsigned TracksNum   = pAnimControllerTmp->GetMaxNumTracks();
		unsigned AnimSetsNum = pAnimControllerTmp->GetMaxNumAnimationSets();

		//
		if(TracksNum < AnimSetsNum)
		{
			TracksNum = AnimSetsNum;
		}

		//�ǂݍ��񂾃A�j���[�V�����R���g���[������V���ɐݒ肳�ꂽ�����𐶐�
		if(FAILED(pAnimControllerTmp->CloneAnimationController(
					pAnimControllerTmp->GetMaxNumAnimationOutputs(),
					pAnimControllerTmp->GetMaxNumAnimationSets(),
					TracksNum,
					pAnimControllerTmp->GetMaxNumEvents(),
					&mesh->pAnimController)))
		{
		}
		//�ǂݍ��ݗp�A�j���[�V�����R���g���[���̊J��
//		SAFE_RELEASE(pAnimControllerTmp);

		//�g���b�N�ɑS�ẴA�j���[�V�����Z�b�g��ǂݍ���
		for(unsigned i = 1; i < AnimSetsNum; i++ )
		{
			//�ǂݍ��ݗp�I�u�W�F�N�g�̐錾
			LPD3DXANIMATIONSET pAnimSet = NULL;
			//�A�j���[�V�����R���g���[������A�j���[�V�����Z�b�g��ǂݍ���
			if(FAILED(mesh->pAnimController->GetAnimationSet(i, &pAnimSet)))
			{
			}
			//�w��g���b�N�ɃA�j���[�V�����Z�b�g��ݒ�
			if(FAILED(mesh->pAnimController->SetTrackAnimationSet(i, pAnimSet)))
			{
			}
			//�g���b�N�ɐݒ�ς݂̃A�j���[�V�����Z�b�g���J��
			SAFE_RELEASE(pAnimSet);
		}
	}
	else
	{
		mesh->pAnimController = NULL;
	}

	//�t���[���̑S�K�w���`�F�b�N���ă{�[���̊֘A�t�����s��
	if(FAILED((this->SetupBoneFromFrame(mesh->pFrameRoot, mesh->pFrameRoot))))
	{
		return E_FAIL;
	}

	return S_OK;
}

//**************************************************************************
//�{�[���ݒ�֘A
//**************************************************************************
//�t���[�������������ă��b�V���R���e�i���̃{�[����ݒ�
HRESULT MeshManager::SetupBoneFromFrame(LPD3DXFRAME pFrameRoot,LPD3DXFRAME pFrame)
{
	//���b�V���R���e�i�����݂���ꍇ
	if(pFrame->pMeshContainer!=NULL)
	{
		//���b�V���R���e�i�̃{�[����ݒ�
		if(FAILED(this->SetupBoneFromMeshContainer(pFrameRoot, pFrame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}

	//�Z��t���[�������݂���ꍇ
	if(pFrame->pFrameSibling!=NULL)
	{
		//�Z��t���[��������
		if(FAILED(this->SetupBoneFromFrame(pFrameRoot, pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}
	//�q�t���[�������݂���ꍇ
	if(pFrame->pFrameFirstChild!=NULL)
	{
		//�q�t���[��������
		if(FAILED(this->SetupBoneFromFrame(pFrameRoot, pFrame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}
//���b�V���R���e�i���̃{�[����ݒ�
HRESULT MeshManager::SetupBoneFromMeshContainer(LPD3DXFRAME pFrameRoot,LPD3DXMESHCONTAINER pMeshCont)
{
	DxMeshContainer *pMeshContainer = (DxMeshContainer*)pMeshCont;

	//�X�L�����b�V���̂Ƃ��̓{�[���s����Z�b�g
	if(pMeshContainer->pSkinInfo)
	{
		DWORD NumBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[NumBones];
		if( !pMeshContainer->ppBoneMatrixPtrs ) return false;

		LPCSTR str;

		for( DWORD i = 0; i < NumBones; i++ )
		{
			str = pMeshContainer->pSkinInfo->GetBoneName(i);
			DxFrame *pFrame = (DxFrame*)D3DXFrameFind(pFrameRoot, str );
			if( !pFrame) return false;
			pMeshContainer->ppBoneMatrixPtrs[i] = &pFrame->CombinedTransformationMatrix;
		}
	}
	return S_OK;
}


//**************************************************************************
//�j���y�ъJ���p�֐�
//**************************************************************************
//DxMesh�Ɋ܂܂��f�[�^�̔j������ъJ��
void MeshManager::ReleaseMesh(DxBaseMesh *base_mesh)
{
	DxMesh *mesh = (DxMesh*)base_mesh;

	//�e�N�X�`�����̗̈��j��
	SAFE_DELETE_ARRAY(mesh->pTex);
	//�}�e���A�����̗̈��j��
	SAFE_DELETE_ARRAY(mesh->pMat);
	//�t�@�C�����̔j��
//	SAFE_DELETE_ARRAY(mesh->file_name);
	//���b�V���f�[�^�̊J��
	SAFE_RELEASE(mesh->lpMesh);

	//DxMesh�̈�̔j��
	SAFE_DELETE(mesh);
}
//DxAnimeMesh�Ɋ܂܂��f�[�^�̔j������ъJ��
void MeshManager::ReleaseAnimeMesh(DxBaseMesh *base_mesh)
{
	DxAnimeMesh *ani_mesh = (DxAnimeMesh*)base_mesh;

	//�t�@�C�����̔j��
//	SAFE_DELETE_ARRAY(ani_mesh->file_name);

	//�A�j���[�V�����R���g���[���̊J��
	SAFE_RELEASE(ani_mesh->pAnimController);

	//�t���[���K�w�̍ŏ�ʃm�[�h�Ɋ܂܂��S�m�[�h�̔j��
	D3DXFrameDestroy(ani_mesh->pFrameRoot,this->hierarchy);
	//�t���[���K�w�̍ŏ�ʃm�[�h��NULL��ݒ�
	ani_mesh->pFrameRoot = NULL;

	//DxAnimeMesh�̈�̔j��
	SAFE_DELETE(ani_mesh);
}

//**************************************************************************
//���̑��̊֐�
//**************************************************************************
//�o�^�ς݃��b�V���f�[�^�̃t�@�C�������`�F�b�N���d�����[�h��h�~����
DxBaseMesh* MeshManager::CheckFileName(const string& fName)
{
	string str1="";
	string str2="";

	str1+=fName;

	for(short i=0;i<this->mesh_val;i++)
	{
		str2+=this->mesh_data[i]->file_name;
		if((str1 == str2))
		{
			return this->mesh_data[i];
		}
	}

	return NULL;
}
