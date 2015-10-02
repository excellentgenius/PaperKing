
//**************************************************************************
//�t�H���g�Ǘ��N���X
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#pragma once

typedef LPCSTR FontType;

//�t�H���g�Ǘ��N���X
class Dx_Font
{
	private:
		//�f�o�C�X
		LPDIRECT3DDEVICE9	dev;

		//�o�^�t�H���g�S
		vector<LPD3DXFONT>	font;
		//�o�^�t�H���g�S�ɑΉ�����t�H���g��
		vector<FontType>	font_name;
		//�o�^�t�H���g��
		short				font_val;

		//�e�L�X�g�t�H�[�}�b�g
		DWORD				format;

		/*�f�t�H���g�t�H���g�֘A*/
		//�f�t�H���g�t�H���g�^�C�v�S
		static FontType		default_type[17];
		//�f�t�H���g�t�H���g��
		static int			default_font_val;
		//�f�t�H���g�t�H���g����
		static int			default_height;

		//�f�t�H���g�t�H���g�̃��[�h
		void DefaultLoad();

	public:
		//�R���X�g���N�^
		Dx_Font(LPDIRECT3DDEVICE9 dev,short def_height=0);
		//�f�X�g���N�^
		~Dx_Font();

		//�t�H�[�}�b�g�̐ݒu
		void SetFormat(DWORD format);
		//�f�t�H���g�t�H�[�}�b�g��ݒ�
		void SetDefaultFormat();

		//�t�H���g�̐���
		void CreateDxFont(LPCTSTR font_type,UINT height,UINT weight=FW_NORMAL);

		//�t�H���g�`��
		void DrawString(LPD3DXSPRITE sprite,int x,int y,int w,int h,LPCSTR string,D3DCOLOR color,short font_no);
};

