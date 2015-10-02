
//**************************************************************************
//�e�N�X�`���Ǘ��N���X
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#include "dx.h"
#include "texture_manager.h"


//�R���X�g���N�^
TextureManager::TextureManager(LPDIRECT3DDEVICE9 dev)
{
	//�f�o�C�X�̓o�^
	this->dev = dev;

	//�o�^���̏�����
	this->tex_val = 0;
}
//�f�X�g���N�^
TextureManager::~TextureManager()
{
	vector<DxTexture*>::iterator it;

	//vector���̃I�u�W�F�N�g�����ׂĔj������ъJ��
	for(it=this->tex_data.begin();it!=this->tex_data.end();it++)
	{
		this->ReleaseTexture((*it));
	}
	this->tex_data.clear();
}

//**************************************************************************
//�O���Q�Ɨp���[�h�֐�
//**************************************************************************
//�C���[�W�t�@�C������DxTexture�����[�h�@�@�i�ȈՔŁj
LPDIRECT3DTEXTURE9 TextureManager::LoadTextureFromFile(const string& fName,const string& path)
{
	//�����p�I�u�W�F�N�g
	DxTexture *create=NULL;

	//��΃p�X�ݒ�ɂȂ��Ă���\��������̂ŕ�����`�F�b�N���s��
	string s_f_name;
	for(unsigned i=0;i<(fName.size());i++)
	{
		s_f_name.push_back(fName[i]);

		if(fName[i]=='\\' || fName[i]=='/')
		{
			s_f_name.clear();
		}
	}

	//�d�����[�h�h�~�̂��߃t�@�C�����`�F�b�N���s��
	if((create=this->CheckFileName(s_f_name))!=NULL)
	{
		//�o�^�ς݃I�u�W�F�N�g��߂�l�Ƃ���
		return create->lpTex;
	}

	//�I�u�W�F�N�g�̐���
	create = new DxTexture;
	//�����p�I�u�W�F�N�g�Ƀp�X�y�уt�@�C�������烍�[�h
	if(FAILED(this->LoadTextureFromFile(create,s_f_name,path)))
	{
		//���[�h�Ɏ��s�����ꍇ�A�̈�m�ۂ�������j��
		SAFE_DELETE(create);
		return NULL;
	}

	//�e�N�X�`���S�ɓo�^
	this->tex_data.push_back(create);
	//�e�N�X�`���̑��������Z
	this->tex_val++;

	//�V���ɐ������ꂽ�I�u�W�F�N�g��Ԃ�
	return create->lpTex;
}
//�C���[�W�t�@�C������DxTexture�����[�h�@�@�i�ڍהŁj
DxTexture* TextureManager::LoadTextureFromFileEx(const string& fName,const string& path)
{
	//�����p�I�u�W�F�N�g
	DxTexture *create=NULL;

	//��΃p�X�ݒ�ɂȂ��Ă���\��������̂ŕ�����`�F�b�N���s��
	string s_f_name;
//	char c_f_name[20]="";
	for(unsigned i=0;i<fName.size();i++)
	{
		s_f_name.push_back(fName[i]);

		if(fName[i]=='\\' || fName[i]=='/')
		{
			s_f_name.clear();
		}
	}

	//�d�����[�h�h�~�̂��߃t�@�C�����`�F�b�N���s��
	if((create=this->CheckFileName(s_f_name))!=NULL)
	{
		//�o�^�ς݃I�u�W�F�N�g��߂�l�Ƃ���
		return create;
	}

	//�I�u�W�F�N�g�̐���
	create = new DxTexture;
	//�����p�I�u�W�F�N�g�Ƀp�X�y�уt�@�C�������烍�[�h
	if(FAILED(this->LoadTextureFromFileEx(create,s_f_name,path)))
	{
		//���[�h�Ɏ��s�����ꍇ�A�̈�m�ۂ�������j��
		SAFE_DELETE(create);
		return NULL;
	}

	//�e�N�X�`���S�ɓo�^
	this->tex_data.push_back(create);
	//�e�N�X�`���̑��������Z
	this->tex_val++;

	//�V���ɐ������ꂽ�I�u�W�F�N�g��Ԃ�
	return create;
}

//**************************************************************************
//�����Q�Ɨp���[�h�֐�
//**************************************************************************
//�C���[�W�t�@�C������DxTexture�����[�h�@�@�i�ȈՔŁj
HRESULT TextureManager::LoadTextureFromFile(DxTexture* tex,const string& fName,const string& path)
{
	HRESULT hr=S_OK;
	string  str = path + fName;

	D3DXGetImageInfoFromFile(str.c_str(),&tex->info);

	if(FAILED(D3DXCreateTextureFromFile(this->dev,str.c_str(),&tex->lpTex)))
	{
		return E_FAIL;
	}

	//�t�@�C�����̐ݒu
	tex->file_name += fName;

  return S_OK;
}
//�C���[�W�t�@�C������DxTexture�����[�h�@�@�i�ڍהŁj
HRESULT TextureManager::LoadTextureFromFileEx(DxTexture* tex,const string& fName,const string& path)
{
	HRESULT hr=S_OK;
	string str = path + fName;

    if(FAILED(D3DXCreateTextureFromFileEx(
		this->dev,				//LPDIRECT3DDEVICE9     pDevice,
		str.c_str(),					//LPCTSTR               pSrcFile,
		D3DX_DEFAULT,			//UINT                  Width,
    D3DX_DEFAULT,			//UINT                  Height,
    D3DX_DEFAULT,			//UINT                  MipLevels,
    0,						//DWORD                 Usage,
    D3DFMT_UNKNOWN,			//D3DFORMAT             Format,
    D3DPOOL_MANAGED,        //D3DPOOL               Pool,
		D3DX_FILTER_NONE,			//DWORD                 Filter,
    D3DX_DEFAULT,			//DWORD                 MipFilter,
		0,						//D3DCOLOR              ColorKey,   //�����F��ݒ�
		&tex->info,					//D3DXIMAGE_INFO        *pSrcInfo,
		NULL,					//PALETTEENTRY          *pPalette,
		&tex->lpTex				//LPDIRECT3DTEXTURE9    *ppTexture
    ))){
       return E_FAIL;
    }

	//�t�@�C�����̗̈�m��
	tex->file_name = fName;
	//�t�@�C�����̐ݒu
	tex->file_name = fName;

    return S_OK;
}

//**************************************************************************
//�j���y�ъJ���p�֐�
//**************************************************************************
//DxTexture�Ɋ܂܂��f�[�^�̔j���y�ъJ��
void TextureManager::ReleaseTexture(DxTexture *tex)
{
	//�e�N�X�`���̊J��
	SAFE_RELEASE(tex->lpTex);

	//DxTexture�̈�̔j��
	SAFE_DELETE(tex);
	this->tex_val--;
}

//**************************************************************************
//���̑��̊֐�
//**************************************************************************
//�o�^�ς݃e�N�X�`���f�[�^�̃t�@�C�������`�F�b�N���d�����[�h��h�~����
DxTexture* TextureManager::CheckFileName(const string& fName)
{
	string str1;
	string str2;
	if(fName.size()>25)
	{
		return NULL;
	}

	str1 += fName;
	for(short i=0;i<this->tex_val;i++)
	{
		str2=this->tex_data[i]->file_name;
		if(str1==str2)
		{
			return this->tex_data[i];
		}
	}

	return NULL;
}

