


#include	"gplib.h"
#include	"charbase.h"

using namespace gplib;

#include "effect.h"

#include <vector>
#include <algorithm>


struct Effect
{
	charabase::CharaBase pos;
	effect::EFFECTTYPE type;
	int	count;		//�o���t���[����
	int renderMode;	//�`����@
	int repertDraw;	//�d�˂��
	//charabase::CharaBase�̕`��A�����ɒ��ڊւ��Ȃ����߂������
	float	animCnt;
	float	animFrm;			//�A�j�����x �؂�ւ��t���[��
	int		animMax;			//�A�j���ő吔
	float	addAngle;			//�p�x���Z���x


};

typedef void(*create)(int x, int y, effect::EFFECTTYPE type);
typedef bool (*step)(Effect& effect);



std::vector<Effect> effects;


//---------------------------
//�G�t�F�N�g�̃J�E���g����
//�J�E���g�I����false
//�{����charabase::CharaBase�ɂčs��
//charabase::CharaBase���o�R���Ȃ����@�̃T���v��
//---------------------------
bool EffectCount(Effect& effect)
{
	effect.count --;
	if(effect.count <= 0){ 
		return false;
	}
	return true;
}

//---------------------------
//�G�t�F�N�g�̉�]����
//��]�I����false
//---------------------------
bool EffectAngle(Effect& effect)
{
	charabase::CharaBase& pos = effect.pos;
	pos.angle += effect.addAngle;
	if(pos.angle > 360){ 
		return false;
	}
	return true;
}

//---------------------------
//�G�t�F�N�g�̃A�j������
//�A�j���I����false
//---------------------------
bool EffectAnime(Effect& effect)
{
	charabase::CharaBase& pos = effect.pos;
	//�A�j����0.1�t���[���X�V
	effect.animCnt ++;
	if(effect.animCnt >= effect.animFrm){ 
		effect.animCnt = 0;
		pos.src.x ++;
		if (pos.src.x >= effect.animMax){
			pos.src.x = 0;
			return false;
		}
	}
	return true;
}

//---------------------------
//�G�t�F�N�g�̃A���t�@����
//�A���t�@�I����false
//---------------------------
bool EffectAlpha(Effect& effect)
{
	charabase::CharaBase& pos = effect.pos;
	//�����悻0.5sec�ŏ���
	pos.a -= system::ONEFRAME_TIME * 255.f * 2.f;
	if(pos.a < 0){
		return false;
	}
	return true;
}

//---------------------------
//�G�t�F�N�g�̈ړ�����
//��ʊO��false
//---------------------------
bool EffectMove(Effect& effect)
{
	charabase::CharaBase& pos = effect.pos;
	MoveChar(pos);
	if (pos.pos.x + pos.width / 2 > system::WINW ||
		pos.pos.y + pos.height/2 > system::WINH || 
		pos.pos.x - pos.width/2 < 0 || 
		pos.pos.y - pos.height/2 < 0){
		return false;
	}
	return true;
}

//charabase::CharaBase& pos, 
//float x, float y,					//�o�����W
//float addx, float addy,		//�ړ���
//std::string img,					//�摜��
//int imgY, int w, int h,		//imgY = src.y ,w,h�G�t�F�N�g�̕��A����
//int anim = 0, float angle = 0.f,//anim = src.c , �p�x
//float scaleX = 1.0f, float scaleY = 1.0f		//�{��

void CreateOneEffect(charabase::CharaBase& pos, float x, float y, float addx, float addy,
						std::string img, int imgY, int w, int h,
					  int anim = 0,float angle = 0.f,
					  float scaleX = 1.0f,float scaleY = 1.0f)
{
	charabase::InitCharBase(pos,
		x, y, 0.f,
		addx, addy, 0.f, 
		img,
		anim, imgY, 0,
		w, h,
		0.f,scaleX,scaleY,0.f,
		255.f);
	UseChar(pos);

}

//���C������	
void CreateEffect_A(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 400;
	const int EFFECTASZY = 300;
	CreateOneEffect(effect.pos,(float)x,(float)y,0.f,0.f,
		"warning",0,EFFECTASZX,EFFECTASZY);

	effect.type = type;					//���
	effect.count = 60;					//���ł܂ł̃t���[��
	effect.renderMode = NORMAL;	//�����_�����O���[�h
	effect.repertDraw = 1;			//�`���
	effect.animMax = 1;					//�A�j���[�V�������̍ő喇��
	effect.animCnt = 0.f;				//�A�j���[�V�����X�^�[�g�J�E���g�@�ʏ�͂O
	effect.animFrm = 0;					//�A�j���[�V�����̑��x�@���t���[���Ő؂�ւ�邩	
	effect.addAngle = 0.f;			//��]���̑���	

	UseChar(effect.pos);
	effects.push_back(effect);

}
bool StepEffect_A(Effect& effect)
{
	if(!EffectCount(effect)){
		return false;
	}
	return true;
}

//�ڂ���
void CreateEffect_B(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 400;
	const int EFFECTASZY = 300;
	CreateOneEffect(effect.pos,(float)x,(float)y,0.f,0.f,
		"warning",0,EFFECTASZX,EFFECTASZY,1);

	effect.type = type;
	effect.count = 120;
	effect.renderMode = ADD;
	effect.repertDraw = 2;
	effect.animMax = 1;
	effect.animCnt = 0.f;
	effect.animFrm = 0;
	effect.addAngle = 0.f;

	UseChar(effect.pos);
	effects.push_back(effect);

}
bool StepEffect_B(Effect& effect)
{
	if(!EffectAlpha(effect)){
		return false;
	}
	//�g����s��
	effect.pos.scale.x += system::ONEFRAME_TIME * 10.f;
	effect.pos.scale.y += system::ONEFRAME_TIME * 10.f;
	return true;
}


//��
void CreateEffect_C(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 80;
	const int EFFECTASZY = 80;
	CreateOneEffect(effect.pos,(float)x,(float)y,0.f,0.f,
		"myst",0,EFFECTASZX,EFFECTASZY,0,1,0.3f,0.3f);

	effect.pos.a = 200.0f;
	effect.type = type;
	effect.count = 120;
	effect.renderMode = NORMAL;
	effect.repertDraw = 1;
	effect.animMax = 1;
	effect.animCnt = 0.f;
	effect.animFrm = 0;
	effect.addAngle = 0.f;


	UseChar(effect.pos);
	effects.push_back(effect);

}
bool StepEffect_C(Effect& effect)
{
	EffectMove(effect);
	effect.pos.scale.x += 0.01f;
	effect.pos.scale.y += 0.01f;
	if (!EffectAlpha(effect)){
		return false;
	}
	return true;
}


void CreateEffect_D(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 40;
	const int EFFECTASZY = 40;

	//�\���̒����w��
	effect.count = 30;
	//�J�n�ʒu����
	float len = math::GetRandom(30.f,100.f);
	float angle = math::GetRandom(0.f,360.f);
	//�J�n�ʒu
	float xx = math::ROUND_X(angle,len,(float)x);
	float yy = math::ROUND_Y(angle, len, (float)y);
	//�ړ���������
//	�g�U
//	float addx = ROUND_X(angle,len/effect.count,0);
//	float addy = ROUND_Y(angle,len/effect.count,0);
	float addx = -math::ROUND_X(angle, len / effect.count, 0.f);
	float addy = -math::ROUND_Y(angle, len / effect.count, 0.f);
	//���
	int no = math::GetRandom(0,6);
	//�W��
	CreateOneEffect(effect.pos,xx,yy,addx,addy,
		"exp",no,EFFECTASZX,EFFECTASZY,0,0.f,1.f,1.f);
	effect.pos.a = 200.0f;
	effect.type = type;
	effect.renderMode = ADD;
	effect.repertDraw = 1;
	effect.animMax = 7;
	effect.animCnt = 0.f;
	effect.animFrm = 5;
	effect.addAngle = 0.f;

	UseChar(effect.pos);
	effects.push_back(effect);

//	DSound_Play(flashWAV);

}
bool StepEffect_D(Effect& effect)
{
	EffectMove(effect);
	if(!EffectAnime(effect)){
		return false;
	}
	if(!EffectCount(effect)){
		return false;
	}
	return true;
}


//����
void CreateEffect_E(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 96;
	const int EFFECTASZY = 96;
	CreateOneEffect(effect.pos,(float)x,(float)y,0.f,0.f,
		"explode",0,EFFECTASZX,EFFECTASZY,0,0.f,2.f,2.f);

	effect.type = type;
	effect.count = 0;
	effect.renderMode = ADD;
	effect.repertDraw = 1;
	effect.animMax = 16;
	effect.animCnt = 0.f;
	effect.animFrm = 1;
	effect.addAngle = 3.f;

	UseChar(effect.pos);
	effects.push_back(effect);

//	DSound_Play(bombWAV);
}

bool StepEffect_E(Effect& effect)
{
	EffectAngle(effect);
	if(!EffectAnime(effect)){
		return false;
	}
	return true;
}

//�~
void CreateEffect_F(int x, int y, effect::EFFECTTYPE type)
{
	Effect effect;
	const int EFFECTASZX = 400;
	const int EFFECTASZY = 400;

	const float angleTable[] = {5.f,-5.f,5.f,-5.f,5.f,0};
	const int imgType[] = {0,1,2,3,3,4};
	//�~��̃G�t�F�N�g��5��
	for(int i = 0 ; i < 6 ; i ++){
		CreateOneEffect(effect.pos,(float)x,(float)y,0.f,0.f,
			"circle",0,
			EFFECTASZX,EFFECTASZY,0,0.f,1.f,1.f);

		effect.pos.src.x = (float)imgType[i];
		effect.pos.a = 200.f;
		effect.type = type;
		effect.count = 120;
		effect.renderMode = ADD;
		effect.repertDraw = 5;
		effect.animMax = 1;
		effect.animCnt = 0.f;
		effect.animFrm = 0;
		effect.addAngle = angleTable[i];
	
		UseChar(effect.pos);
		effects.push_back(effect);
	}
}

bool StepEffect_F(Effect& effect)
{
	EffectAngle(effect);
	if(!EffectCount(effect)){
		return false;
	}
	return true;
}

//�~��ɍL����
void CreateEffect_G(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = {0};
	const int EFFECTASZX = 40;
	const int EFFECTASZY = 40;
	const float len = 4.0f;
	for (int i = 0; i < 8; i++){
		e = zero;
		//�ړ���������
		//	�g�U
		float addx = math::ROUND_X(i * 45.f, len);
		float addy = math::ROUND_Y(i * 45.f, len);
		//���
		int no = math::GetRandom(0, 6);
		//�W��
		CreateOneEffect(e.pos, (float)x, (float)y, addx, addy,
			"exp", no, EFFECTASZX, EFFECTASZY, 0, 0.f, 1.f, 1.f);
		e.pos.a = 200.0f;
		e.type = type;
		e.renderMode = NORMAL;
		e.repertDraw = 1;
		e.animMax = 7;
		e.animCnt = 0.f;
		e.animFrm = 5;
		e.addAngle = 0.f;

		UseChar(e.pos);
		effects.push_back(e);
	}
	//	DSound_Play(flashWAV);
}

bool StepEffect_G(Effect& effect)
{
	EffectMove(effect);
	if (!EffectAlpha(effect)){
		return false;
	}
	return true;
}

//����
void CreateEffect_H(int x, int y, effect::EFFECTTYPE type)
{
	Effect e;
	Effect zero = {0};

	const int EFFECTASZX = 40;
	const int EFFECTASZY = 40;
	const float len = 4.0f;
	for (int i = 0; i < 4; i++){
		e = zero;
		//�ړ���������
		//	�g�U
		float addx = math::GetRandom(-2.f, 2.f);
		float addy = math::GetRandom(-10.f, 3.f);
		//���
		int no = math::GetRandom(0, 6);
		//�W��
		CreateOneEffect(e.pos, (float)x, (float)y, addx, addy,
			"exp", no, EFFECTASZX, EFFECTASZY, 0, 0.f, 1.f, 1.f);
		e.pos.a = 200.0f;
		e.type = type;
		e.renderMode = ADD;
		e.repertDraw = 1;
		e.animMax = 7;
		e.animCnt = 0.f;
		e.animFrm = 5;
		e.addAngle = 0.f;

		UseChar(e.pos);
		effects.push_back(e);
	}
	//	DSound_Play(flashWAV);
}

bool StepEffect_H(Effect& effect)
{
	effect.pos.add.y += 0.3f;
	if (!EffectMove(effect)){
		return false;
	}
//	if (!EffectAlpha(effect)){
//		return false;
//	}
	return true;
}

struct EffectFunc{
	create	create;
	step	step;
}EffectFuncTable[] = {
	{CreateEffect_A,StepEffect_A},
	{CreateEffect_B,StepEffect_B},
	{CreateEffect_C,StepEffect_C},
	{CreateEffect_D,StepEffect_D},
	{CreateEffect_E,StepEffect_E},
	{CreateEffect_F,StepEffect_F},
	{ CreateEffect_G, StepEffect_G },
	{ CreateEffect_H, StepEffect_H },
};

void effect::Create(int x, int y, effect::EFFECTTYPE type)
{
	EffectFuncTable[(int)type].create(x,y,type);
}

void effect::Init()
{
	effects.clear();
}

void effect::Draw()
{
	for (const auto& effect : effects){
		graph::Draw_SetRenderMode(effect.renderMode);
		for(int i = 0 ; i < effect.repertDraw ; i ++){
			Draw_Char(effect.pos);
		}
		graph::Draw_EndRenderMode();
	}
}

static bool CheckUseEffect(Effect effect){ return !CheckUseChar(effect.pos);}

void effect::Step()
{
	for (auto& effect : effects){
		if(!EffectFuncTable[(int)effect.type].step(effect)){
			NoUseChar(effect.pos);
		}
	}
	auto it = remove_if(effects.begin(), effects.end(), CheckUseEffect);
	effects.erase(it, effects.end());
}

void effect::LoadReses()
{
	graph::Draw_LoadObject("warning","res/gra/effect/warning.png");
	graph::Draw_LoadObject("circle", "res/gra/effect/circle.png");
	graph::Draw_LoadObject("myst", "res/gra/effect/myst.png");
	graph::Draw_LoadObject("explode", "res/gra/effect/explode.png");
	graph::Draw_LoadObject("exp", "res/gra/effect/exp.png");


}





