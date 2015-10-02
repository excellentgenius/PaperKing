#pragma once

namespace charabase{

	//代入可能なベースデータ
	//初期値が0,1.0fのものは除外する。
	struct BaseData{
		int					wid, hgt;		//画像幅高さ
		const std::string 	resname;		//使用イメージ名
	};

	struct Vec3f{
		float x, y, z;
	};

	//
	//
	//
	//
	struct Anim{
		int		no;					//現在のアニメNO　この値は0,1,2,3,4,とカウントするだけ
		float	cnt;				//アニメ更新カウンタ
		float	speed;			//アニメ更新速度
		int		max;				//アニメ枚数　この値にnoが来ると0からカウント
	};

	struct CharaBase
	{
		Vec3f   pos;							//座標
		Vec3f		add;							//移動量
		int			width, height;			//画像幅高さ

		Vec3f   scale;						//拡大率
		Vec3f   src;							//抽出位置
		std::string resname;			//使用イメージ名

		bool		show;							//利用フラグ

		float		angle;						//角度
		float		a;								//アルファ
		float		r, g, b;						//色
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



