
//**************************************************************************
//�A�j���[�V�������b�V���֘A
// DxHierarcy		: �t���[���y�у��b�V���R���e�i����p�N���X
// DxFrame			: �h���t���[���\����
// DxMeshContainer	: �h�����b�V���R���e�i�\����
// DxAnimeMesh		: �A�j���[�V�������b�V���\����
//**************************************************************************


#include "dx_anime_mesh.h"


//**************************************************************************
//�t���[���y�у��b�V���R���e�i����p�N���X
//**************************************************************************
//�R���X�g���N�^
Dx_Hierarchy::Dx_Hierarchy(TextureManager *t_manager)
{
	//�e�N�X�`���Ǘ��N���X�̃A�h���X��ݒ�
	this->t_manager = t_manager;
}

//�t���[���̐���
HRESULT Dx_Hierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	//�����p�I�u�W�F�N�g�̐錾
	DxFrame *pFrame;
	//���������I�u�W�F�N�g�̓o�^���������
    *ppNewFrame = NULL;

	//�I�u�W�F�N�g�̐���
	pFrame = new DxFrame;
	//�ϐ���NULL�������ꍇ
    if ( pFrame == NULL)
	{
		//�G���[�̈Ӗ� : Direct3D ���Ăяo�����������邽�߂̏\���ȃ����������蓖�Ă邱�Ƃ��ł��܂���ł����B
		return E_OUTOFMEMORY;
	}
 
	//�t���[���������݂��Ȃ��ꍇ
	if(Name==NULL)
	{
		//�󔒂�ݒ�
		Name = "";
	}
	//�t���[�����̗̈�m��
	pFrame->Name=new TCHAR[lstrlen(Name) + 1];
	//�̈悪�m�ۂ���Ȃ������ꍇ
    if (!pFrame->Name)
	{
		//�G���[�̈Ӗ� : Direct3D �T�u�V�X�e�����Ō����s���̃G���[���������܂����B
		return E_FAIL;
	}
	//���O���R�s�[����
	strcpy(pFrame->Name,Name);

	//�ϊ��s��ɒP�ʍs���ݒ�
	D3DXMatrixIdentity( &pFrame->TransformationMatrix);
	//���b�V�����̍ŏI���[���h�s��ɒP�ʍs���ݒ�
	//**����** �����Őݒ肵�Ă���͔̂h���N���X�ɂ����Ȃ��̂�D3DXFRAME���g�p�����ꍇ�ɂ̓G���[�ɂȂ�I�I
    D3DXMatrixIdentity( &pFrame->CombinedTransformationMatrix);

	//���b�V���R���e�i�ւ̃|�C���^��������
    pFrame->pMeshContainer = NULL;
	//�Z��t���[���ւ̃|�C���^��������
    pFrame->pFrameSibling = NULL;
	//�q�t���[���ւ̃|�C���^��������
    pFrame->pFrameFirstChild = NULL;

	//���������I�u�W�F�N�g�̃A�h���X���q���ς���
    *ppNewFrame = pFrame;

	//�����������ɏI��
    return S_OK;
}
//���b�V���R���e�i�̐���
HRESULT Dx_Hierarchy::CreateMeshContainer(
	LPCTSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials, 
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo, 
	LPD3DXMESHCONTAINER *ppMeshContainer )
{
	HRESULT hr = S_OK;
	//�����p�I�u�W�F�N�g�̐錾
	DxMeshContainer *pMeshContainer = NULL;
	//���������I�u�W�F�N�g�̓o�^���������
	*ppMeshContainer = NULL;

	int iFacesAmount;
	unsigned iMaterial;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	LPD3DXMESH pMesh = NULL;

	// patch meshes ���������͂ł��Ȃ�
	//if( pMeshData->Type != D3DXMESHTYPE_MESH ) return E_FAIL;

	// FVF �ŋL�q���ꂽ���b�V���ȊO�͓ǂ߂Ȃ�
	//if( pMesh->GetFVF() == 0 ) return E_FAIL;

	//DWORD					dwBoneAmt=0;

	//���b�V���f�[�^�̎����b�V���|�C���^���擾
	pMesh = pMeshData->pMesh;

	//�I�u�W�F�N�g�̐���
	pMeshContainer = new DxMeshContainer;
	//�ϐ���NULL�������ꍇ
    if (pMeshContainer == NULL)
	{
		//�G���[�̈Ӗ� : Direct3D ���Ăяo�����������邽�߂̏\���ȃ����������蓖�Ă邱�Ƃ��ł��܂���ł����B
		return E_OUTOFMEMORY;
	}
	//
	ZeroMemory(pMeshContainer, sizeof(DxMeshContainer));

	//���b�V���R���e�i���̗̈�m��
	pMeshContainer->Name=new TCHAR[lstrlen(Name) + 1];
	//�̈悪�m�ۂ���Ȃ������ꍇ
    if (!pMeshContainer->Name)
	{
		//�G���[�̈Ӗ� : Direct3D �T�u�V�X�e�����Ō����s���̃G���[���������܂����B
		return E_FAIL;
	}
	//���O���R�s�[����
	strcpy(pMeshContainer->Name,Name);

	//���b�V���Ɋ֘A�t�����Ă���f�o�C�X���擾
	if(FAILED(pMesh->GetDevice(&pDevice)))
	{
		/*hr = E_FAIL;
		SAFE_RELEASE( pDevice );
		if( pMeshContainer ) DestroyMeshContainer( pMeshContainer );
		return hr;*/
	}

	//���b�V���Ɋ܂܂��ʂ̐����擾
	iFacesAmount = pMesh->GetNumFaces();

    // ���Y���b�V�����@���������Ȃ��ꍇ�͖@����ǉ�����
    if (!(pMesh->GetFVF() & D3DFVF_NORMAL)) {
		//���b�V���f�[�^�̎�ނ��u���b�V���v�ɕύX
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		//FVF�R�[�h���g���ă��b�V���̃R�s�[�𐶐�
        hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                    pMesh->GetFVF() | D3DFVF_NORMAL, 
                                    pDevice, &pMeshContainer->MeshData.pMesh );
		//���������s�����ꍇ
        if (FAILED(hr))
		{
			//�G���[�̈Ӗ� : Direct3D �T�u�V�X�e�����Ō����s���̃G���[���������܂����B
			return E_FAIL;
		}

        pMesh = pMeshContainer->MeshData.pMesh;
		//pMesh�Ɋ܂܂��e���_�̖@�����v�Z���ďo��
        D3DXComputeNormals( pMesh, NULL );

    } else{
		//���b�V���R���e�i���̃��b�V���f�[�^�Ƀ��b�V���|�C���^��ݒ�
        pMeshContainer->MeshData.pMesh = pMesh;
		//���b�V���f�[�^�̎�ނ��u���b�V���v�ɐݒ�
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		//???
        pMesh->AddRef();
    }

	//���b�V���̃}�e���A���ݒ�
	//�}�e���A������ݒ�
    pMeshContainer->NumMaterials	= max(1, NumMaterials);
	//�}�e���A���̐������}�e���A���̗̈���m��
    pMeshContainer->pMaterials		= new D3DXMATERIAL[pMeshContainer->NumMaterials];
	//�}�e���A���̐������e�N�X�`���̗̈�m��
    pMeshContainer->ppTextures		= new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	//�ʂ̐������̈���m��
    pMeshContainer->pAdjacency		= new DWORD[iFacesAmount*3];

	//���ꂼ��̗̈�m�ۂɎ��s�����ꍇ
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		//�G���[�̈Ӗ� : Direct3D ���Ăяo�����������邽�߂̏\���ȃ����������蓖�Ă邱�Ƃ��ł��܂���ł����B
		return E_OUTOFMEMORY;
	}

	//�z���0��ݒ�
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
	//�z���pAbjacency��ݒ�
    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount*3);

	//�}�e���A�������݂��鎞
    if (NumMaterials > 0)
	{
		//
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		//
        for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			//�e�N�X�`���̃t�@�C���������݂���ꍇ
            if( pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				//�t�@�C���܂ł̑��΃p�X��ݒ�
				char path[256] ="res/x/";
				//�t�@�C�������擾
				LPSTR fName	= pMeshContainer->pMaterials[iMaterial].pTextureFilename;

				//�e�N�X�`�����̐���
				pMeshContainer->ppTextures[iMaterial] = 
					this->t_manager->LoadTextureFromFile(fName,path);
				//�e�N�X�`�����̔j��
                pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
            }
        }
    }
	else
	{
		//�e�N�X�`�����̔j��
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		//�}�e���A����0��ݒ�
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));

		//
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular  = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }

	//�X�L�j���O��񂪑��݂���ꍇ�A�{�[���𐶐�
	if(pSkinInfo!=NULL)
	{
		//���b�V���R���e�i���̃X�L�j���O����ݒ�
		pMeshContainer->pSkinInfo = pSkinInfo;
		//???
		pSkinInfo->AddRef();

		//�X�L�j���O��񂩂�{�[���𐶐�
		this->CreateBone(pMeshContainer,pDevice);
	}

	//���[�J���ɐ����������b�V���R���e�i���Ăяo�����ɃR�s�[����
	*ppMeshContainer = pMeshContainer;

	//�����������ɏI��
    return hr;
}
//�{�[���̐���
HRESULT Dx_Hierarchy::CreateBone(DxMeshContainer *pMeshContainer,LPDIRECT3DDEVICE9 lpD3DDev)
{
	DWORD dwBoneAmt=0;
	LPD3DXSKININFO pSkinInfo = pMeshContainer->pSkinInfo;

	pMeshContainer->lpMesh = pMeshContainer->MeshData.pMesh;
	pMeshContainer->MeshData.pMesh->AddRef();

	// Bone Offset Matrices �ۑ��p�̃��������m��
	dwBoneAmt = pSkinInfo->GetNumBones();
	pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneAmt];
	if( !pMeshContainer->pBoneOffsetMatrices ) return E_OUTOFMEMORY;

	// Bone Offset Matrices �ǂݍ���
	for (DWORD i= 0; i < dwBoneAmt; i++){
		memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i),sizeof(D3DMATRIX));	
	}

	HRESULT hr = S_OK;

	//�X�L����񂪂Ȃ��Ȃ�I��
	if( !pMeshContainer->pSkinInfo ) return hr;

	//���������Ă���
	if(pMeshContainer->MeshData.pMesh)			SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	if(pMeshContainer->pBoneCombinationBuf)		SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

	hr = pMeshContainer->lpMesh->CloneMeshFVF(
			D3DXMESH_MANAGED, pMeshContainer->lpMesh->GetFVF(),
			lpD3DDev , &pMeshContainer->MeshData.pMesh );
	if( FAILED( hr ) ) return hr;

	//���b�V���R���e�i�ɏ������Ă��钸�_���̎擾
	hr = pMeshContainer->MeshData.pMesh->GetAttributeTable( NULL, &pMeshContainer->NumAttributeGroups );
	if( FAILED( hr ) ) return hr;

	//���b�V���R���e�i�ɏ������Ă��钸�_���̊i�[
	delete[] pMeshContainer->pAttributeTable;
	pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
	if( !pMeshContainer->pAttributeTable ){
		hr = E_OUTOFMEMORY;
		return hr;
	}
	hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL );
	if( FAILED( hr ) ) return hr;

	// ���̃��b�V���ɂ���ă{�[���s��p�̃��������m�ۂ�����Ă��Ȃ��ꍇ�͊m��
	if( pMeshContainer->NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() ){
		pMeshContainer->NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

		delete []pMeshContainer->pBoneMatrices;
		pMeshContainer->pBoneMatrices = new D3DXMATRIXA16[pMeshContainer->NumBoneMatricesMax];
		if( !pMeshContainer->pBoneMatrices ){
			hr = E_OUTOFMEMORY;
			return hr;
		}
	}

   return hr;
}

//�t���[���̔j��
HRESULT Dx_Hierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	DxFrame *frame = (DxFrame*)pFrameToFree;

	//�t�@�C�����̗̈��j��
	SAFE_DELETE_ARRAY(frame->Name);
	//�t���[����j��
	SAFE_DELETE(frame);

	//�����������ɏI��
	return S_OK;
}
//���b�V���R���e�i�̔j��
HRESULT Dx_Hierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	DxMeshContainer *pMeshContainer = (DxMeshContainer*)pMeshContainerToFree;

	
	SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );
	SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs);
	SAFE_DELETE(pMeshContainer->pAttributeTable);
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneMatrices);

	SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

	//Adjacency�̗̈�j��
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	//�}�e���A���S�̗̈�j��
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	//�e�N�X�`���S�̗̈�j��
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	//
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	//�X�L�j���O���̊J��
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	//�e�N�X�`�����̗̈�j��
	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	//���b�V���R���e�i�̔j��
	SAFE_DELETE(pMeshContainer);

	//�����������ɏI��
	return S_OK;
}

//**************************************************************************
//�A�j���[�V�������b�V���\����
//**************************************************************************
//�R���X�g���N�^
DxAnimeMesh::DxAnimeMesh() : DxBaseMesh()
{
	//�t���[���K�w�̍ŏ�ʃm�[�h�̏�����
	this->pFrameRoot=NULL;
	//�A�j���[�V�����R���g���[���̏�����
	this->pAnimController=NULL;
}
//�t�@�C�����̎擾
string DxAnimeMesh::GetFileName()
{
	return this->file_name;
}
