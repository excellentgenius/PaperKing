#pragma once

namespace charabase{

	//����\�ȃx�[�X�f�[�^
	//�����l��0,1.0f�̂��̂͏��O����B
	struct BaseData{
		int					wid, hgt;		//�摜������
		const std::string 	resname;		//�g�p�C���[�W��
	};

	struct Vec3f{
		float x, y, z;
	};

	//
	//
	//
	//
	struct Anim{
		int		no;					//���݂̃A�j��NO�@���̒l��0,1,2,3,4,�ƃJ�E���g���邾��
		float	cnt;				//�A�j���X�V�J�E���^
		float	speed;			//�A�j���X�V���x
		int		max;				//�A�j�������@���̒l��no�������0����J�E���g
	};

	struct CharaBase
	{
		Vec3f   pos;							//���W
		Vec3f		add;							//�ړ���
		int			width, height;			//�摜������

		Vec3f   scale;						//�g�嗦
		Vec3f   src;							//���o�ʒu
		std::string resname;			//�g�p�C���[�W��

		bool		show;							//���p�t���O

		float		angle;						//�p�x
		float		a;								//�A���t�@
		float		r, g, b;						//�F
	};

	extern void MoveChar(CharaBase& cb);
	extern bool CheckUseChar(const CharaBase& cb);
	extern void NoUseChar(CharaBase& cb);
	extern void UseChar(CharaBase& cb);
	extern void MakeRectLeftTop(RECT &rt, const CharaBase& cb, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
	extern void MakeRect(RECT &rt, const CharaBase& cb, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
	extern void MakeRect2(RECT &rt, const CharaBase& cb, float width = 0.f, float height = 0.f, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
	extern void MakeRectScales(RECT &rt, const CharaBase& cb, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
	extern void Draw_Char(const CharaBase& cb);
	extern void Draw_Char2(const CharaBase& cb);
	extern void Draw_Char3(const CharaBase& cb, float a, float r, float g, float b);
	extern void Draw_CharNC(const CharaBase& cb);
	extern void Draw_Char2NC(const CharaBase& cb);
	extern bool HitCheck(const CharaBase& cb1, const CharaBase& cb2);
	extern bool HitCheck2(const CharaBase& cb1, const CharaBase& cb2, float width, float height);
	extern bool HitCheckScales(const CharaBase& cb1, const CharaBase& cb2);
	extern bool HitCheckForRange(const CharaBase& cb1, const CharaBase& cb2);
	extern bool HitCheckForCircle(const CharaBase& cb1, const CharaBase& cb2);
	extern bool HitCheckForMouse(POINT pt, const CharaBase& cb);

	extern void InitCharBase(CharaBase& cb, const BaseData& base,
		float x = 0.f, float y = 0.f, float z = 0.f, float addx = 0.f, float addy = 0.f, float addz = 0.f,
		int srcX = 0, int srcY = 0, int srcZ = 0, float angle = 0.f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.f,
		float alpha = 255.f, float red = 255.f, float green = 255.f, float blue = 255.f);

	extern void	InitCharBase(CharaBase& cb, float x, float y, float z,float addx, float addy,float addz,
		const std::string& resname, int srcX, int srcY, int srcZ, int w, int h,
		float angle = 0.f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f,
		float alpha = 255.f, float red = 255.f, float green = 255.f, float blue = 255.f);
	extern void InitCharBase(CharaBase& cb);

	extern void SetAnim(Anim& anim, int max, float speed);
	extern bool StepAnim(Anim& anim);
	extern void StopAnim(Anim& anim);
	extern void StartAnim(Anim& anim,float speed);


}



