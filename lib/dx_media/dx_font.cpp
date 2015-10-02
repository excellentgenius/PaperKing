
//**************************************************************************
//�t�H���g�Ǘ��N���X
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@�Ȃ�
//**************************************************************************
#include "dx.h"

#include "dx_font.h"


//**************************************************************************
//�f�t�H���g�ݒ�
//**************************************************************************
//�f�t�H���g�t�H���g��
int Dx_Font::default_font_val = 17;
//�f�t�H���g�t�H���g����
int Dx_Font::default_height = 40;
//�f�t�H���g�t�H���g�S
FontType Dx_Font::default_type[17] = {

		"�l�r �o�S�V�b�N",			// 0
		"�l�r �o����",				// 1
		"�c�e�o�����S�V�b�N��",     // 2
		"�c�e�����S�V�b�N��",       // 3
		"�c�e�oPOP��",				// 4
		"HGP���ȏ���",              // 5
		"HGP�s����",                // 6
		"HGP�n�p�p�߯�ߑ�",         // 7
		"HGS���ȏ���",              // 8
		"HGS�s����",                // 9
		"HG�ۺ޼��M-PRO",           // 10
		"HG���ȏ���",               // 11
		"HG�s����",                 // 12
		"HG��������-PRO",           // 13
		"�l�r �S�V�b�N",            // 14
		"�l�r ����",                // 15
		"HGP�n�p�p�޼��UB"			// 16
};


//�R���X�g���N�^
Dx_Font::Dx_Font(LPDIRECT3DDEVICE9 dev,short def_height)
{
	//�f�o�C�X�̎擾
	this->dev = dev;
	//�o�^�t�H���g��
	this->font_val = 0;

	if(def_height!=0)
	{
		this->default_height = def_height;
	}

	//�f�t�H���g�t�H���g�����[�h
	this->DefaultLoad();

	//�t�H�[�}�b�g�̐ݒ�
	this->SetDefaultFormat();
}
//�f�X�g���N�^
Dx_Font::~Dx_Font()
{
	for(short i=0;i<this->font_val;i++)
	{
		SAFE_RELEASE(this->font[i]);
	}
	this->font.clear();
}

//**************************************************************************
//�t�H�[�}�b�g�֘A
//**************************************************************************
//�t�H�[�}�b�g�̐ݒu
void Dx_Font::SetFormat(DWORD format)
{
	this->format = format;
}
//�f�t�H���g�t�H�[�}�b�g��ݒ�
void Dx_Font::SetDefaultFormat()
{
	this->SetFormat( DT_LEFT | DT_VCENTER );
}

//�f�t�H���g�t�H���g�̃��[�h
void Dx_Font::DefaultLoad()
{
	for(int i=0;i<this->default_font_val;i++)
	{
		this->CreateDxFont((LPCTSTR)this->default_type[i],this->default_height,FW_HEAVY);
	}
}

//**************************************************************************
//�t�H���g�����֘A
//**************************************************************************
//�t�H���g�̐���
void Dx_Font::CreateDxFont(LPCTSTR font_type,UINT height,UINT weight)
{
	if(this->font_val<20)
	{
		LPD3DXFONT create;

		// �t�H���g�̍쐬
		D3DXCreateFont(this->dev,			//LPDIRECT3DDEVICE9	pD3DDevice,
			height,							//UINT				Height,
			0,								//UINT				Width,
			weight,							//UINT				Weight,
			D3DX_DEFAULT,					//UINT				MipLevels,
			FALSE,							//BOOL				Italic,
			SHIFTJIS_CHARSET,				//DWORD				CharSet,
			OUT_DEFAULT_PRECIS,				//DWORD				OutputPrecision,
			PROOF_QUALITY,					//DWORD				Quality,
			DEFAULT_PITCH | FF_DONTCARE,	//DWORD				PitchAndFamily,
			font_type,						//LPCTSTR			pFacename,
			&create							//LPD3DXFONT		*ppFont		&pD3DXFont
		);
		//�t�H���g�S�ɓo�^
		this->font.push_back(create);
		this->font_val++;
	}
}

//**************************************************************************
//�t�H���g�`��֘A
//**************************************************************************
//�t�H���g�`��
void Dx_Font::DrawString(LPD3DXSPRITE sprite,int x,int y,int w,int h,LPCSTR string,D3DCOLOR color,short font_no)
{
	LPD3DXFONT select_font = NULL;

	//�w��i���o�[�̃t�H���g�����݂���ꍇ
	if(font_no < this->font_val)
	{
		//�I�����ꂽ�t�H���g���擾
		select_font = this->font[font_no];
	}
	//�Ȃ��ꍇ�ɂ̓f�t�H���g�t�H���g���g�p����
	else
	{
		select_font = this->font[0];
	}

	//�\���̈�̒�`
	RECT	rt = {x,y,x+w,y+h};
	//�t�H���g�`��֐�
	select_font->DrawText(sprite,string,-1,&rt,this->format,color);

	//�f�t�H���g�t�H�[�}�b�g��ݒ�
	this->SetDefaultFormat();
}
