

#include "gplib.h"
#include "CharBase.h"

#define PAPERKINGRAD 27		//���̉��̕��ƍ����͈Ⴄ�̂ŁA�����̔��a(������)���g���܂�
#define GOALRAD 16

using namespace charabase;
using namespace gplib;
//------------------------------------------------------
//CharBase���p���ėp����
//----------------------
//�A�j���[�V�����ݒ�
void charabase::SetAnim(Anim& anim, int max, float speed)
{
	anim.cnt = 0.f;
	anim.max = max;
	anim.speed = speed;
	anim.no = 0;
}

//�A�j���X�V
//���̊֐���no�̒l���X�V����Bno�̒l��
//CharBase.src.x�ɑ������B
//�A�j���̏�����ύX�������ꍇ�͈ȉ�
/*
	StepAnim(obj.anim);
	//�ȉ��ŃA�j���[�V�����w��\
	int animTable[] = {0,1,0,2};
	//���ۂɗ��p����ꍇ��
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
//�A�j���[�V������~
//speed��0�ɂ��邾���Ȃ̂ŁA�ĊJ���͍ēxSetAnim���K�v
void charabase::StopAnim(Anim& anim)
{
	anim.speed = 0.f;
}

//----------------------
//�A�j���[�V�����J�n
//speed��0�ɂ��邾���Ȃ̂ŁA�ĊJ���͍ēxSetAnim���K�v
void charabase::StartAnim(Anim& anim,float speed)
{
	anim.speed = speed;
}
//------------------------------------------------------
//CharBase����������
//
/*
//��{�ƂȂ�`�b�v�̃T�C�Y�Ɨ��p����摜
BaseData base = {CHARWID,CHARHGT,charIMG};
//�������@�w��O�̒l�͏����l�ɂȂ�B
//���̏ꍇ��wid,hgt,img�ȊO�͏����l
InitCharBase(player.pos,base);

//���̏ꍇ��wid,hgt,img,x,y�ȊO�͏����l
InitCharBase(player.pos,base,STARTPOSX,STARTPOSY);

���p����ۂ͉��t���Oshow���U�ɂȂ��Ă��邽�߁A
UseChar�ŉ���Ԃɂ��邱�ƁB
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
//CharBase����������
//���ׂĂO�ɂ���B
void charabase::InitCharBase(CharaBase& cb)
{
	CharaBase zeroCharBase = {};
	cb = zeroCharBase;
}


//----------------------
//���p�`�F�b�N
bool charabase::CheckUseChar(const CharaBase& cb)
{
	return (cb.show == true);
}

//----------------------
//�ėp�ړ�
void charabase::MoveChar(CharaBase& cb)
{
	cb.pos.x += cb.add.x;
	cb.pos.y += cb.add.y;
	cb.pos.z += cb.add.z;
}

//----------------------
//�ėp��������
void charabase::NoUseChar(CharaBase& cb)
{
	cb.show = false;
}

//----------------------
//�ėp�o������
void charabase::UseChar(CharaBase& cb)
{
	cb.show = true;
}
//----------------------
//����p��`�쐬 ������W�
//�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
//startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
//endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
void charabase::MakeRectLeftTop(RECT &rt, const CharaBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)cb.pos.x+startx, (int)cb.pos.y + starty,
				(int)cb.pos.x+cb.width - endx,(int)cb.pos.y+cb.height - endy);
}

//----------------------
//����p��`�쐬 �������W�
//�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
//startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
//endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
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
//����p��`�쐬 �������W��@�g�嗦�l��
//�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
//startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
//endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
void charabase::MakeRectScales(RECT &rt, const CharaBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)(cb.pos.x - (cb.width*cb.scale.x) / 2 - startx), (int)(cb.pos.y - (cb.height * cb.scale.y) / 2 - starty),
		(int)(cb.pos.x + (cb.width*cb.scale.x) / 2 - endx), (int)(cb.pos.y + (cb.height * cb.scale.y) / 2 - endy));
}

//----------------------
//�ėp�`�揈�� �����
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
//�ėp�`�揈�� ����
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
//�ėp�`�揈�� ����� �F�ݒ�
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
//�ėp�`�揈�� �����
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
//�ėp�`�揈�� ����
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
//������`�F�b�N
bool charabase::HitCheck(const CharaBase& v1, const CharaBase& v2)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRect(rt1, v1);
	charabase::MakeRect(rt2,v2);
//�f�o�b�O----------------
	//graph::Draw_CkRect(rt1);
	//graph::Draw_CkRect(rt2);
//�����܂�----------------
	if(math::RectCheck(&rt1,&rt2) ){
			return true;
	}else{
		return false;
	}
}

//----------------------
//������`�F�b�N
bool charabase::HitCheck2(const CharaBase& v1, const CharaBase& v2, float width, float height)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRect(rt1, v1);
	charabase::MakeRect2(rt2, v2, width, height);
	//�f�o�b�O----------------
	//graph::Draw_CkRect(rt1);
	//graph::Draw_CkRect(rt2);
	//�����܂�----------------
	if (math::RectCheck(&rt1, &rt2)){
		return true;
	}
	else{
		return false;
	}
}

//----------------------
//������`�F�b�N �g�嗦�l��
bool charabase::HitCheckScales(const CharaBase& v1, const CharaBase& v2)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRectScales(rt1, v1);
	charabase::MakeRectScales(rt2, v2);
	//�f�o�b�O----------------
		//graph::Draw_CkRect(rt1);
		//graph::Draw_CkRect(rt2);
	//�����܂�----------------
	if (math::RectCheck(&rt1, &rt2)){
		return true;
	}
	else{
		return false;
	}
}

//----------------------
//������`�F�b�N
bool charabase::HitCheckForRange(const CharaBase& v1, const CharaBase& v2)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRect(rt1, v1);
	charabase::MakeRect(rt2, v2, v2.width, v2.height, -v2.width, -v2.height);
	//�f�o�b�O----------------
	//graph::Draw_CkRect(rt1);
	//graph::Draw_CkRect(rt2);
	//�����܂�----------------
	if (math::RectCheck(&rt1, &rt2)){
		return true;
	}
	else{
		return false;
	}
}

//----------------------
//�ۂ��I�u�W�F�N�g�̂�����`�F�b�N
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
//�I��(�}�E�X)�I�u�W�F�N�g�̂�����`�F�b�N
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

//�����܂�
//------------------------------------------------------
