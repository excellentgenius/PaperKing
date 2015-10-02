

#include "gplib.h"
#include "CharBase.h"

#define PAPERKINGRAD 27		//紙の王の幅と高さは違うので、高さの半径(長い方)を使います
#define GOALRAD 16

using namespace charabase;
using namespace gplib;
//------------------------------------------------------
//CharBase利用時汎用処理
//----------------------
//アニメーション設定
void charabase::SetAnim(Anim& anim, int max, float speed)
{
	anim.cnt = 0.f;
	anim.max = max;
	anim.speed = speed;
	anim.no = 0;
}

//アニメ更新
//この関数はnoの値を更新する。noの値は
//CharBase.src.xに代入する。
//アニメの順序を変更したい場合は以下
/*
	StepAnim(obj.anim);
	//以下でアニメーション指定可能
	int animTable[] = {0,1,0,2};
	//実際に利用する場合は
	animTable[obj.anim.no]
	StepAnim(obj.anim);

*/
//
bool charabase::StepAnim(Anim& anim)
{
	anim.cnt += anim.speed;
	if(anim.cnt >= 1.f){
		anim.cnt = 0;
		anim.no++;
		if(anim.no >= anim.max){
			anim.no = 0;
		}
		return true;
	}
	return false;
}

//----------------------
//アニメーション停止
//speedを0にするだけなので、再開時は再度SetAnimが必要
void charabase::StopAnim(Anim& anim)
{
	anim.speed = 0.f;
}

//----------------------
//アニメーション開始
//speedを0にするだけなので、再開時は再度SetAnimが必要
void charabase::StartAnim(Anim& anim,float speed)
{
	anim.speed = speed;
}
//------------------------------------------------------
//CharBase初期化処理
//
/*
//基本となるチップのサイズと利用する画像
BaseData base = {CHARWID,CHARHGT,charIMG};
//初期化　指定外の値は初期値になる。
//この場合はwid,hgt,img以外は初期値
InitCharBase(player.pos,base);

//この場合はwid,hgt,img,x,y以外は初期値
InitCharBase(player.pos,base,STARTPOSX,STARTPOSY);

利用する際は可視フラグshowも偽になっているため、
UseCharで可視状態にすること。
*/
void charabase::InitCharBase(CharaBase& cb, const BaseData& base,
						float x,float y,float z,float addx,float addy,float addz,
					  int srcX,int srcY,int srcZ,float angle, float scaleX,float scaleY,float scaleZ,
						float alpha, float red, float green, float blue)
{
	cb.width = base.wid;
	cb.height = base.hgt;
	cb.resname = base.resname;
	
	cb.pos.x = x;
	cb.pos.y = y;
	cb.pos.z = z;
	cb.add.x = addx;
	cb.add.y = addy;
	cb.add.z = addz;
	cb.scale.x = scaleX;
	cb.scale.y = scaleY;
	cb.scale.y = scaleZ;
	cb.src.x = (float)srcX;
	cb.src.y = (float)srcY;
	cb.src.z = (float)srcZ;
	
	cb.a = alpha;
	cb.r = red;
	cb.g = green;
	cb.b = blue;
	
	cb.show = false;
}

void charabase::InitCharBase(CharaBase& cb, float x, float y, float z, float addx, float addy, float addz,
					const std::string& resname, int srcX, int srcY, int srcZ, int w, int h,
					float angle, float scaleX,float scaleY,float scaleZ,
					float alpha, float red, float green, float blue)
{
	cb.pos.x = x;
	cb.pos.y = y;
	cb.pos.z = z;
	cb.add.x = addx;
	cb.add.y = addy;
	cb.add.z = addz;
	cb.scale.x = scaleX;
	cb.scale.y = scaleY;
	cb.scale.z = scaleZ;
	cb.src.x = (float)srcX;
	cb.src.y = (float)srcY;
	cb.src.z = (float)srcZ;
	cb.resname = resname;
	cb.width = w;
	cb.height = h;
	cb.angle = angle;
	cb.a = alpha;
	cb.r = red;
	cb.g = green;
	cb.b = blue;
	cb.show = false;

}

//------------------------------------------------------
//CharBase初期化処理
//すべて０にする。
void charabase::InitCharBase(CharaBase& cb)
{
	CharaBase zeroCharBase = {};
	cb = zeroCharBase;
}


//----------------------
//利用チェック
bool charabase::CheckUseChar(const CharaBase& cb)
{
	return (cb.show == true);
}

//----------------------
//汎用移動
void charabase::MoveChar(CharaBase& cb)
{
	cb.pos.x += cb.add.x;
	cb.pos.y += cb.add.y;
	cb.pos.z += cb.add.z;
}

//----------------------
//汎用消去処理
void charabase::NoUseChar(CharaBase& cb)
{
	cb.show = false;
}

//----------------------
//汎用出現処理
void charabase::UseChar(CharaBase& cb)
{
	cb.show = true;
}
//----------------------
//判定用矩形作成 左上座標基準
//大きさがクリッピングサイズと異なる場合に利用する。
//startx,starty : 左上点からのオフセット値、値分左上から加算される
//endx,endy : 右下点からのオフセット値、値分右下から減算される
void charabase::MakeRectLeftTop(RECT &rt, const CharaBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)cb.pos.x+startx, (int)cb.pos.y + starty,
				(int)cb.pos.x+cb.width - endx,(int)cb.pos.y+cb.height - endy);
}

//----------------------
//判定用矩形作成 中央座標基準
//大きさがクリッピングサイズと異なる場合に利用する。
//startx,starty : 左上点からのオフセット値、値分左上から加算される
//endx,endy : 右下点からのオフセット値、値分右下から減算される
void charabase::MakeRect(RECT &rt, const CharaBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)cb.pos.x - cb.width / 2 - startx, (int)cb.pos.y - cb.height / 2 - starty,
				(int)cb.pos.x+cb.width/2 - endx,(int)cb.pos.y+cb.height/2 - endy);
}

void charabase::MakeRect2(RECT &rt, const CharaBase& cb, float width, float height, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)cb.pos.x - width / 2 - startx, (int)cb.pos.y - height / 2 - starty,
		(int)cb.pos.x + width / 2 - endx, (int)cb.pos.y + height / 2 - endy);
}

//----------------------
//判定用矩形作成 中央座標基準　拡大率考慮
//大きさがクリッピングサイズと異なる場合に利用する。
//startx,starty : 左上点からのオフセット値、値分左上から加算される
//endx,endy : 右下点からのオフセット値、値分右下から減算される
void charabase::MakeRectScales(RECT &rt, const CharaBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)(cb.pos.x - (cb.width*cb.scale.x) / 2 - startx), (int)(cb.pos.y - (cb.height * cb.scale.y) / 2 - starty),
		(int)(cb.pos.x + (cb.width*cb.scale.x) / 2 - endx), (int)(cb.pos.y + (cb.height * cb.scale.y) / 2 - endy));
}

//----------------------
//汎用描画処理 中央基準
void charabase::Draw_Char(const CharaBase& pos)
{
	graph::Draw_Graphics(
				pos.pos.x,pos.pos.y,pos.pos.z,
				pos.resname,
				pos.width	* (int)(pos.src.x),
				pos.height * (int)(pos.src.y),
				pos.width,pos.height,
				(int)pos.angle,
				pos.scale.x,pos.scale.y,
				(u_char)pos.a,(u_char)pos.r,(u_char)pos.g,(u_char)pos.b);
}

//----------------------
//汎用描画処理 左上基準
void charabase::Draw_Char2(const CharaBase& pos)
{
	graph::Draw_GraphicsLeftTop(
				pos.pos.x,pos.pos.y,pos.pos.z,
				pos.resname,
				pos.width	* (int)(pos.src.x),
				pos.height * (int)(pos.src.y),
				pos.width,pos.height,
				(int)pos.angle,nullptr,
				pos.scale.x,pos.scale.y,
				(u_char)pos.a,(u_char)pos.r,(u_char)pos.g,(u_char)pos.b);
}

//----------------------
//汎用描画処理 中央基準 色設定
void charabase::Draw_Char3(const CharaBase& pos, float a, float r, float g, float b)
{
	graph::Draw_Graphics(
		pos.pos.x, pos.pos.y, pos.pos.z,
		pos.resname,
		pos.width	* (int)(pos.src.x),
		pos.height * (int)(pos.src.y),
		pos.width, pos.height,
		(int)pos.angle,
		pos.scale.x, pos.scale.y,
		(u_char)a, (u_char)r, (u_char)g, (u_char)b);
}


//----------------------
//汎用描画処理 中央基準
void charabase::Draw_CharNC(const CharaBase& pos)
{

	graph::Draw_GraphicsNC(
				pos.pos.x,pos.pos.y,pos.pos.z,
				pos.resname,
				pos.width	* (int)(pos.src.x),
				pos.height * (int)(pos.src.y),
				pos.width,pos.height,
				(int)pos.angle,nullptr,
				pos.scale.x,pos.scale.y,
				(u_char)pos.a,(u_char)pos.r,(u_char)pos.g,(u_char)pos.b);
}

//----------------------
//汎用描画処理 左上基準
void charabase::Draw_Char2NC(const CharaBase& pos)
{
	graph::Draw_GraphicsLeftTopNC(
				pos.pos.x,pos.pos.y,pos.pos.z,
				pos.resname,
				pos.width	* (int)(pos.src.x),
				pos.height * (int)(pos.src.y),
				pos.width,pos.height,
				(int)pos.angle,nullptr,
				pos.scale.x,pos.scale.y,
				(u_char)pos.a,(u_char)pos.r,(u_char)pos.g,(u_char)pos.b);
}
//----------------------
//あたりチェック
bool charabase::HitCheck(const CharaBase& v1, const CharaBase& v2)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRect(rt1, v1);
	charabase::MakeRect(rt2,v2);
//デバッグ----------------
	//graph::Draw_CkRect(rt1);
	//graph::Draw_CkRect(rt2);
//ここまで----------------
	if(math::RectCheck(&rt1,&rt2) ){
			return true;
	}else{
		return false;
	}
}

//----------------------
//あたりチェック
bool charabase::HitCheck2(const CharaBase& v1, const CharaBase& v2, float width, float height)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRect(rt1, v1);
	charabase::MakeRect2(rt2, v2, width, height);
	//デバッグ----------------
	//graph::Draw_CkRect(rt1);
	//graph::Draw_CkRect(rt2);
	//ここまで----------------
	if (math::RectCheck(&rt1, &rt2)){
		return true;
	}
	else{
		return false;
	}
}

//----------------------
//あたりチェック 拡大率考慮
bool charabase::HitCheckScales(const CharaBase& v1, const CharaBase& v2)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRectScales(rt1, v1);
	charabase::MakeRectScales(rt2, v2);
	//デバッグ----------------
		//graph::Draw_CkRect(rt1);
		//graph::Draw_CkRect(rt2);
	//ここまで----------------
	if (math::RectCheck(&rt1, &rt2)){
		return true;
	}
	else{
		return false;
	}
}

//----------------------
//あたりチェック
bool charabase::HitCheckForRange(const CharaBase& v1, const CharaBase& v2)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRect(rt1, v1);
	charabase::MakeRect(rt2, v2, v2.width, v2.height, -v2.width, -v2.height);
	//デバッグ----------------
	//graph::Draw_CkRect(rt1);
	//graph::Draw_CkRect(rt2);
	//ここまで----------------
	if (math::RectCheck(&rt1, &rt2)){
		return true;
	}
	else{
		return false;
	}
}

//----------------------
//丸いオブジェクトのあたりチェック
bool charabase::HitCheckForCircle(const CharaBase& v1, const CharaBase& v2)
{
	if (sqrt((float)((v1.pos.x - v2.pos.x) * (v1.pos.x - v2.pos.x) + (v1.pos.y - v2.pos.y) * (v1.pos.y - v2.pos.y))) <= (PAPERKINGRAD + GOALRAD)) {
		return true;
	}
	else {
		return false;
	}
}

//----------------------
//選択(マウス)オブジェクトのあたりチェック
bool charabase::HitCheckForMouse(POINT pt, const CharaBase& cb)
{
	if ((pt.x <= (cb.pos.x + cb.width / 2)) &&
		(pt.x >= (cb.pos.x - cb.width / 2)) &&
		(pt.y >= (cb.pos.y - cb.height / 2)) &&
		(pt.y <= (cb.pos.y + cb.height / 2))) {
		return true;
	}
	else {
		return false;
	}
}

//ここまで
//------------------------------------------------------
