/**************************************************************************************
* �Q�[����: PAPER KING																  *
* �����: ���T���F																	  *
* ���t: 2014/06/12																	  *
* �Q�[���Љ�:																		  *
* �Q�[���̎�l��"���̉�"���x�z���Ă��鍑�͍ŋ߈��z���Ă���"�S�~�̉�"�̂����ŁA������  *
* �S�~���炯�ɂȂ��Ă��܂��܂����B�v���C���[�̓Q�[���̎�l���𑀍삵�āA�����⓹���  *
* �W�߂��āA"�S�~�̉�"��ǂ��o���āA�����ꂽ�������ɖ߂��܂��傤�I					  *
**************************************************************************************/
#include	<vector>
#include	<cmath>
#include	<iostream>
#include	<fstream>
#include	"../../lib/GpLib.h"
#include	"../../lib/charbase.h"
#include	"../../lib/esing.h"
#include	"../../lib/effect.h"

#define JUNKMAX 8			//�S�~�̍ő吔
#define ARROWMAX 20			//��̍ő吔
#define BEAMMAX 100			//�r�[���̍ő吔
#define GAMEFIELDW 960		//�Q�[���̗̈�̕�
#define GAMEFIELDH 850		//�Q�[���̗̈�̍���

using namespace gplib;

/**************************************************************************************
*���C�u�������ŕK�v�ȕϐ��@															  *
*�K�v�ɉ����ĕύX���邱��															  *
**************************************************************************************/
namespace gplib{
	namespace system{
		const char USERNAME[256] = "PAPER KING";

		const int	WINW = 720;				//�Q�[����ʂ̕�
		const int	WINH = 640;				//�Q�[����ʂ̍���
		bool		WindowMode = true;		//�Q�[����ʂ̃T�C�Y�͕ύX�s��
		int			KeyboardMode = 0;		//pad disable:1 pad enable:0
	}
}
/**************************************************************************************
*include�L�q																		  *
**************************************************************************************/

/**************************************************************************************
*�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`										  *
**************************************************************************************/
enum STAGE {
	STG_TITLE, STG_TUTORIAL, STG_WORLDMAP, STG_1, STG_2, STG_3, STG_4, STG_5, STG_END, STG_GAMEOVER
};

enum PLAYERCHOICE {
	KINDEND, VIOLENTEND
};

enum ARMORTYPE {
	GAUNTLET, PLACKART, GREAVE, SWORD
};

enum ATTACKTYPE {
	SHORTRANGE, LONGRANGE, EXCALIBUR
};

enum JUNKTYPE {
	LOG, DEADFISH, GARBAGEBIN, POO
};

enum WALLSIDE {
	TOP, DOWN, LEFT, RIGHT
};

enum OBSTACLETYPE {
	FOUNTAIN, STATUE, HOUSE, STREETLIGHT, TREE, ROCK
};

//�����n�}�̍\���̂�錾
typedef struct WorldMapInfo {
	charabase::CharaBase cb_;
} WorldMapInfo;

//���̍\���̂�錾
typedef struct Bridge {
	charabase::CharaBase cb_;
} Bridge;

//�X�L�b�v�{�^���̍\���̂�錾
typedef struct SkipButton {
	charabase::CharaBase cb_;
} SkipButton;

//�Z�[�u�E���[�h�m�F��ʂ̍\���̂�錾
typedef struct SaveLoadConfirmationMsg {
	charabase::CharaBase cb_;
} SaveLoadConfirmationMsg;

//�Z�[�u�E���[�h�{�^���̍\���̂�錾
typedef struct SaveLoadButton {
	charabase::CharaBase cb_;
} SaveLoadButton;

//�͂��E�������{�^���̍\���̂�錾
typedef struct YesNoButton {
	charabase::CharaBase cb_;
} YesNoButton;

//�I���̍\���̂�錾
typedef struct Options {
	charabase::CharaBase cb_;
} Options;

//HP�l�̍\���̂�錾
typedef struct BarStatus {
	charabase::CharaBase cb_;
} BarStatus;

//���̉��̍\���̂�錾
typedef struct PlayerInfo {
	charabase::CharaBase cb_;
	ARMORTYPE armors[5];
	BarStatus emptyHPBar;
	BarStatus fullHPBar;
	BarStatus fullPowerBar;
	float maxHP;
	float hp;
	float maxExcaliburPower;
	float excaliburPower;
	float moveSpeed;
	bool hasKey;
	int angle;
	float receivedDamage;
	float provideDamage;
} PlayerInfo;

//�U���̍\���̂�錾
typedef struct AttackInfo {
	charabase::CharaBase cb_;
	float attackDamage;
	bool attacking;
} AttackInfo;

//��̍\���̂�錾
typedef struct ArrowInfo {
	charabase::CharaBase cb_;
	int angle;
} ArrowInfo;

//�G�N�X�J���o�[�K�E�Z�̍\���̂�錾
typedef struct ExcaliburBeamInfo {
	charabase::CharaBase cb_;
	bool attacking;
	int angle;
} ExcaliburBeamInfo;

//�S�~�̉��̍\���̂�錾
typedef struct JunkKingInfo {
	charabase::CharaBase cb_;
	BarStatus emptyHPBar;
	BarStatus fullHPBar;
	float maxHP;
	float hp;
	float attackDamage;
	bool attacking;
	bool attacked;
	bool dashing;
	float moveSpeed;
	bool speedUp;
	bool powerUp;
	bool detectedAttack;
	float receivedDamage;
	float provideDamage;
	int decision;
	float width;
	float height;
	float angle;
} JunkKingInfo;

//�S�~�̍\���̂�錾
typedef struct JunkInfo {
	charabase::CharaBase cb_;
	JUNKTYPE type;
	BarStatus emptyHPBar;
	BarStatus fullHPBar;
	float attackDamage;
	bool attacking;
	float maxHP;
	float hp;
	bool hideKey;
	float angle;
} JunkInfo;

//��Q���̍\���̂�錾
typedef struct ObstacleInfo {
	charabase::CharaBase cb_;
	OBSTACLETYPE type;
	BarStatus emptyHPBar;
	BarStatus fullHPBar;
	float maxHP;
	float hp;
} ObstacleInfo;

//�R�C���̍\���̂�錾
typedef struct CoinInfo {
	charabase::CharaBase cb_;
	float width;
	float height;
} CoinInfo;

//��̍\���̂�錾
typedef struct TreasureInfo {
	charabase::CharaBase cb_;
	float width;
	float height;
} TreasureInfo;

//�󔠂̌��̍\���̂�錾
typedef struct KeyInfo {
	charabase::CharaBase cb_;
} KeyInfo;

//�����S�̍\���̂�錾
typedef struct AppleInfo {
	charabase::CharaBase cb_;
} AppleInfo;

PlayerInfo paperKing;	//���̉��̃I�u�W�F�N�g�����
AttackInfo shortAttack;
AttackInfo longAttack;
ArrowInfo arrows[ARROWMAX];
ExcaliburBeamInfo excaliburBeam[BEAMMAX];
ATTACKTYPE attackType;
JunkKingInfo junkKing;
JunkInfo junks[JUNKMAX];
ObstacleInfo obstacle;
std::vector<ObstacleInfo> obstacles;
CoinInfo coin;
TreasureInfo treasureBox;
ARMORTYPE tresureType;
KeyInfo key;
AppleInfo apple;
Bridge bridge;

//�X�e�[�^�X�p��X�R�A�̃O���[�o���ϐ���錾����
WorldMapInfo worldMap[5];
bool stageCleared[4];
bool firstTimeToMap;
bool checkLookedStatus;
bool showSaveTip;
SaveLoadConfirmationMsg saveLoadConfirmationMsg[5];
SaveLoadButton saveLoadButton[2];
YesNoButton yesNoButton[2];
int life = 0;
int arrowCount;
int appleCount;
bool enableSpecialAttack;
int score;
int originalScore;
int highScore;
int requiredMoney;
int money;
int collectedMoney;
bool checkAlive;
bool checkPressDirectionKey;
bool checkPressAttackKey;
bool checkPressChangeWeaponKey;
bool checkGotFirstCoin;
bool clearedTutorial = false;
bool checkCleared;
bool checkDefeatedBoss;
bool checkHit;
bool createdKey;
bool checkGotTreasure[5];
bool showTreasure;
int showTreasureTime;
bool poweredUp;
int moveTime;
int junkKingMoveTime;
int decisionTime;
bool hitWall;
WALLSIDE wallside;

//�C���g���Ŏg���ϐ���錾����
int introNumber;
float scrollX, scrollY;
int scrollSpeed;
float mouseX, mouseY;
int titleTimer;
int endingTimer;
int timer;
int dispScoreTimer;
int treasureTipTimer;
int tipTimer;
int attackTimer;
int reminderTimer;
int textNumber;
bool dialogsShown[8];
POINT pt;
SkipButton skipButton;
Options options[4];
bool selectBuffer;
bool selectEnding;
PLAYERCHOICE choice;
STAGE currentStage;

/**************************************************************************************
*�֐��L�q																			  *
**************************************************************************************/
//�Z�[�u�Q�[���f�[�^�̊֐�
void SaveGameData() {
	FILE* pFile = fopen("save.dat", "wb");

	//�K�v�ȕϐ�(�v���C���[�̑I����c��̗V�ׂ�񐔂┭�������󕨂Ȃ�)���Z�[�u�f�[�^�ɕۑ�����
	fwrite(&choice, sizeof(choice), 1, pFile);
	fwrite(&life, sizeof(life), 1, pFile);
	fwrite(&score, sizeof(score), 1, pFile);
	fwrite(&collectedMoney, sizeof(collectedMoney), 1, pFile);
	fwrite(checkGotTreasure, sizeof(checkGotTreasure[0]), 5, pFile);
	fwrite(stageCleared, sizeof(stageCleared[0]), 4, pFile);
	fwrite(&firstTimeToMap, sizeof(firstTimeToMap), 1, pFile);
	fwrite(&showSaveTip, sizeof(showSaveTip), 1, pFile);
	fwrite(&appleCount, sizeof(appleCount), 1, pFile);

	fclose(pFile);
}

//���[�h�Q�[���f�[�^�̊֐�
void LoadGameData() {
	FILE* pFile = fopen("save.dat", "rb");

	//�K�v�ȕϐ�(�v���C���[�̑I����c��̗V�ׂ�񐔂┭�������󕨂Ȃ�)���Z�[�u�f�[�^����ǂݍ��݂���
	if (pFile) {
		fread(&choice, sizeof(choice), 1, pFile);
		fread(&life, sizeof(life), 1, pFile);
		fread(&score, sizeof(score), 1, pFile);
		fread(&collectedMoney, sizeof(collectedMoney), 1, pFile);
		fread(checkGotTreasure, sizeof(checkGotTreasure[0]), 5, pFile);
		fread(stageCleared, sizeof(stageCleared[0]), 4, pFile);
		fread(&firstTimeToMap, sizeof(firstTimeToMap), 1, pFile);
		fread(&showSaveTip, sizeof(showSaveTip), 1, pFile);
		fread(&appleCount, sizeof(appleCount), 1, pFile);
		
		fclose(pFile);

		if (currentStage == STG_TITLE) {
			stage::ChangeStage(STG_WORLDMAP);
		}

		originalScore = score;
	}
	else {
		charabase::UseChar(saveLoadConfirmationMsg[2].cb_);
	}
}

//�ō��_�����Ǘ�����֐�
void SaveHighScore() {
	if (highScore < score) {
		highScore = score;

		FILE* pFile = fopen("highScore.dat", "wb");

		//�ō��_�����Z�[�u�f�[�^�ɕۑ�����
		fwrite(&highScore, sizeof(highScore), 1, pFile);

		fclose(pFile);
	}
}

//�X�e�[�W�N���A����ۂɕ\�����郁�b�Z�[�W�Ƃ��̌�̓���̊֐�
void StageCleared() {
	if (checkCleared) {
		if (currentStage != STG_TUTORIAL && (paperKing.hp / paperKing.maxHP) >= 0.5) {
			graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stageClearWBonus", 0, 0, 490, 284);
		}
		else {
			graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stageClear", 0, 0, 490, 158);
		}

		if (input::CheckAnyPush(input::KEY_BTN0)) {
			if (currentStage != STG_TUTORIAL && (paperKing.hp / paperKing.maxHP) >= 0.5) {
				score += 50;
			}

			for (ExcaliburBeamInfo beam : excaliburBeam) {
				charabase::NoUseChar(beam.cb_);
			}

			SaveHighScore();

			//�X�e�[�W���N���A��������
			switch (currentStage) {
			case STG_TUTORIAL:
				stage::ChangeStage(STG_TITLE);
				break;
			case STG_1:
				if (!stageCleared[0]) {
					treasureTipTimer = 0;
				}
				stageCleared[0] = true;
				collectedMoney += money;
				originalScore = score;
				stage::ChangeStage(STG_WORLDMAP);
				break;
			case STG_2:
				stageCleared[1] = true;
				collectedMoney += money;
				originalScore = score;
				stage::ChangeStage(STG_WORLDMAP);
				break;
			case STG_3:
				stageCleared[2] = true;
				collectedMoney += money;
				originalScore = score;
				stage::ChangeStage(STG_WORLDMAP);
				break;
			case STG_4:
				stageCleared[3] = true;
				collectedMoney += money;
				originalScore = score;
				stage::ChangeStage(STG_WORLDMAP);
				break;
			case STG_5:
				stage::ChangeStage(STG_END);
				break;
			}
		}
	}
}

//���Z�b�g�֐�
void ResetStageInfo() {
	score = 0;
	money = 0;

	//�X�e�[�W�ɂ���ĕK�v�ȋ������߂�
	switch (currentStage) {
	case STG_1:
		requiredMoney = 15000;
		break;
	case STG_2:
		requiredMoney = 18000;
		break;
	case STG_3:
		requiredMoney = 23000;
		break;
	case STG_4:
		requiredMoney = 25000;
		break;
	case STG_5:
		requiredMoney = 28000;
		break;
	}
}

//�����n�}�̏������֐�
void InitWorldMap() {
	charabase::InitCharBase(
		worldMap[0].cb_,
		92.5f, 320.f, 0.1f,
		0.f, 0.f, 0.f,
		"WorldMap_Garden",
		0, 0, 0,
		185, 640
	);

	charabase::InitCharBase(
		worldMap[1].cb_,
		232.f, 320.f, 0.1f,
		0.f, 0.f, 0.f,
		"WorldMap_River",
		0, 0, 0,
		94, 640
	);

	charabase::InitCharBase(
		worldMap[2].cb_,
		355.5f, 320.f, 0.1f,
		0.f, 0.f, 0.f,
		"WorldMap_City",
		0, 0, 0,
		153, 640
	);

	charabase::InitCharBase(
		worldMap[3].cb_,
		521.f, 320.f, 0.1f,
		0.f, 0.f, 0.f,
		"WorldMap_Forest",
		0, 0, 0,
		178, 640
	);

	charabase::InitCharBase(
		worldMap[4].cb_,
		667.f, 320.f, 0.1f,
		0.f, 0.f, 0.f,
		"WorldMap_JunkYard",
		0, 0, 0,
		114, 640
	);
}

//���̏������֐�
void InitBridge() {
	charabase::InitCharBase(
		bridge.cb_,
		487.f, 420.f, 0.1f,
		0.f, 0.f, 0.f,
		"bridge",
		0, 0, 0,
		974, 278
	);

	charabase::UseChar(bridge.cb_);
}

//���̕\���֐�
void DispBridge() {
	if (charabase::CheckUseChar(bridge.cb_)) {
		charabase::Draw_Char(bridge.cb_);
	}
}

//�R�C���̏������֐�
void InitCoin() {
	charabase::InitCharBase(coin.cb_);
	charabase::NoUseChar(coin.cb_);
}

//�R�C�������֐�
void CreateCoin() {
	//�R�C���̃T�C�Y
	coin.width = 32;
	coin.height = 32;

	//�R�C����x��y���W
	float x, y;

	//�o���ʒu�̓����_��
	x = math::GetRandom((coin.width / 2.f), ((float)GAMEFIELDW - coin.width / 2.f));
	y = math::GetRandom((coin.height / 2.f), ((float)GAMEFIELDH - coin.height / 2.f));

	charabase::InitCharBase(
		coin.cb_,
		x, y, 0.1f,
		0.f, 0.f, 0.1f,
		"coin",
		0, 0, 0,
		32, 32
	);

	charabase::UseChar(coin.cb_);
}

//�R�C���̈ړ��֐�
void MoveCoin() {
	//�����蔻��A���̉��̓R�C�����Q�b�g������A�����������Ă������������
	if (charabase::CheckUseChar(coin.cb_) && charabase::HitCheckForCircle(paperKing.cb_, coin.cb_) && !checkCleared) {
		se::DSound_Play("tinkle");
		charabase::NoUseChar(coin.cb_);
		money += 1000;
		score += 5;

		//�X�e�[�W�ɂ���āA�N���A����������ς��
		if (currentStage != STG_TUTORIAL) {
			if (currentStage != STG_5) {
				if (money >= requiredMoney) {
					checkCleared = true;
				}
			}
			else {
				if (money >= requiredMoney && checkDefeatedBoss) {
					checkCleared = true;
				}
			}
		}
		else {
			checkGotFirstCoin = true;
		}
	}

	if (!charabase::CheckUseChar(coin.cb_) && (money < requiredMoney)) {
		CreateCoin();
	}
}

//�R�C���̕\���֐�
void DispCoin() {
	if (charabase::CheckUseChar(coin.cb_)) {
		charabase::Draw_Char(coin.cb_);
	}

}

//�󔠂̌��̏������֐�
void InitKey() {
	charabase::InitCharBase(key.cb_);
	charabase::NoUseChar(key.cb_);
}

//�󔠂̌������֐�
void CreateKey(float xPos, float yPos) {
	charabase::InitCharBase(
		key.cb_,		//�������������\����
		xPos, yPos, 0.1f,		//�������W
		0.f, 0.f, 0.f,		//�ړ���
		"key",		//�g�p�摜��
		0, 0, 0,		//�Q�Ɛ�
		32, 29		//�摜�̃T�C�Y(���A����)
	);

	charabase::UseChar(key.cb_);
}

//�󔠂̌��̕\���֐�
void DispKey() {
	if (charabase::HitCheck(paperKing.cb_, key.cb_)) {
		paperKing.hasKey = true;
		charabase::NoUseChar(key.cb_);
	}
	
	if (charabase::CheckUseChar(key.cb_)) {
		charabase::Draw_Char(key.cb_);
	}
}

//�󔠂̏������֐�
void InitTreasureBox() {
	//�󔠂̈ʒu
	float xPos;
	float yPos;

	//�󔠂̃T�C�Y
	treasureBox.width = 39;
	treasureBox.height = 32;

	//�o���ʒu�̓����_��
	xPos = math::GetRandom((treasureBox.width / 2.f), ((float)GAMEFIELDW - treasureBox.width / 2.f));
	if (currentStage == STG_2) {	
		yPos = math::GetRandom((281.f + (treasureBox.height / 2.f)), (559.f - treasureBox.height / 2.f));
	}
	else {
		yPos = math::GetRandom((treasureBox.height / 2.f), ((float)GAMEFIELDH - treasureBox.height / 2.f));
	}

	charabase::InitCharBase(
		treasureBox.cb_,
		xPos, yPos, 0.1f,
		0.f, 0.f, 0.f,
		"treasureBox",
		0, 0, 0,
		35, 32
	);

	charabase::UseChar(treasureBox.cb_);
}

//�󔠂̕\���֐�
void DispTreasureBox() {
	//�X�e�[�W���ƂɈႤ�󂪂��炦��
	switch (currentStage) {
	case STG_1:
		if (checkGotTreasure[0]) {
			treasureBox.cb_.src.x = 1;
			treasureBox.cb_.width = 39;
		}
		break;
	case STG_2:
		if (checkGotTreasure[1]) {
			treasureBox.cb_.src.x = 1;
			treasureBox.cb_.width = 39;
		}
		break;
	case STG_3:
		if (checkGotTreasure[2]) {
			treasureBox.cb_.src.x = 1;
			treasureBox.cb_.width = 39;
		}
		break;
	case STG_4:
		if (checkGotTreasure[3]) {
			treasureBox.cb_.src.x = 1;
			treasureBox.cb_.width = 39;
		}
		break;
	}

	//�����蔻��A���̉����󔠂̌��������Ă�����A�󔠂��J����
	if (charabase::CheckUseChar(treasureBox.cb_) && paperKing.hasKey && charabase::HitCheck(paperKing.cb_, treasureBox.cb_)) {
		treasureBox.cb_.src.x = 1;
		treasureBox.cb_.width = 39;

		//�X�e�[�W���ƂɈႤ�󂪂��炦��
		switch (currentStage) {
		case STG_TUTORIAL:
			checkGotTreasure[4] = true;
			break;
		case STG_1:
			checkGotTreasure[0] = true;
			break;
		case STG_2:
			checkGotTreasure[1] = true;
			break;
		case STG_3:
			checkGotTreasure[2] = true;
			break;
		case STG_4:
			checkGotTreasure[3] = true;
			break;
		}
		
		//�󕨂�\��
		if (showTreasure) {
			switch (currentStage)
			{
			case STG_TUTORIAL:
				graph::Draw_Graphics(treasureBox.cb_.pos.x, treasureBox.cb_.pos.y - treasureBox.cb_.height / 2 - 10, 0.f, "head", 0, 0, 32, 32, 0, 1.f, 1.f);
				break;
			case STG_1:
				graph::Draw_Graphics(treasureBox.cb_.pos.x, treasureBox.cb_.pos.y - treasureBox.cb_.height / 2 - 10, 0.f, "hasPlackart", 0, 0, 60, 60, 0, 0.5f, 0.5f);
				paperKing.cb_.resname = "paperKingWPlackart";
				break;
			case STG_2:
				graph::Draw_Graphics(treasureBox.cb_.pos.x, treasureBox.cb_.pos.y - treasureBox.cb_.height / 2 - 10, 0.f, "hasGreave", 0, 0, 60, 60, 0, 0.5f, 0.5f);
				if (checkGotTreasure[0]) {
					paperKing.cb_.resname = "paperKingWPlac&Greave";
				}
				else {
					paperKing.cb_.resname = "paperKingWGreave";
				}
				break;
			case STG_3:
				graph::Draw_Graphics(treasureBox.cb_.pos.x, treasureBox.cb_.pos.y - treasureBox.cb_.height / 2 - 10, 0.f, "hasGauntlet", 0, 0, 60, 60, 0, 0.5f, 0.5f);
				if (checkGotTreasure[0]) {
					if (checkGotTreasure[1]) {
						paperKing.cb_.resname = "paperKingFullyArmored";
					}
					else {
						paperKing.cb_.resname = "paperKingWPlac&Gauntlet";
					}
				}
				else {
					if (checkGotTreasure[1]) {
						paperKing.cb_.resname = "paperKingWGreave&Gauntlet";
					}
					else {
						paperKing.cb_.resname = "paperKingWGauntlet";
					}
				}
				break;
			case STG_4:
				graph::Draw_Graphics(treasureBox.cb_.pos.x, treasureBox.cb_.pos.y - treasureBox.cb_.height / 2 - 10, 0.f, "hasExcalibur", 0, 0, 60, 60, 0, 0.5f, 0.5f);
				break;
			}
		}

		//�󕨂ɂ���Ĕ\�͒l���p���[�A�b�v����
		if (!poweredUp) {
			switch (currentStage)
			{
			case STG_TUTORIAL:
				life++;
				break;
			case STG_1:
				paperKing.receivedDamage = 0.5f;
				break;
			case STG_2:
				if (paperKing.moveSpeed == 5.f)	 {
					paperKing.moveSpeed += 2.f;
				}
				break;
			}

			poweredUp = true;
		}

		showTreasureTime--;
		if (showTreasureTime == 0) {
			showTreasure = false;
		}
	}
	charabase::Draw_Char(treasureBox.cb_);
}

//�����S�̏������֐�
void InitApple() {
	charabase::InitCharBase(apple.cb_);
	charabase::NoUseChar(apple.cb_);
}

//�����S�����֐�
void CreateApple(float xPos, float yPos) {
	charabase::InitCharBase(
		apple.cb_,		//�������������\����
		xPos, yPos, 0.1f,		//�������W
		0.f, 0.f, 0.f,		//�ړ���
		"apple",		//�g�p�摜��
		0, 0, 0,		//�Q�Ɛ�
		32, 32		//�摜�̃T�C�Y(���A����)
	);

	charabase::UseChar(apple.cb_);
}

//�����S�̈ړ��֐�
void MoveApple() {
	if (charabase::CheckUseChar(apple.cb_)) {
		if (charabase::HitCheck(paperKing.cb_, apple.cb_)) {
			charabase::NoUseChar(apple.cb_);
			appleCount++;
		}
	}
}

//�����S�̕\���֐�
void DispApple() {
	if (charabase::CheckUseChar(apple.cb_)) {
		charabase::Draw_Char(apple.cb_);
	}
}

//���̉��̍U���̏������֐�
void InitAttack() {
	charabase::InitCharBase(
		shortAttack.cb_,
		paperKing.cb_.pos.x + paperKing.cb_.width / 2, paperKing.cb_.pos.y, 0.f,
		0.f, 0.f, 0.f,
		"swordV",
		0, 0, 0,
		70, 14
	);

	//�v���C���[�̑I���ɂ���Ď��̉��̔\�͒l���ς��(��Փx���ς��)
	if (choice == KINDEND) {
		shortAttack.attackDamage = 20.f;
	}
	else {
		shortAttack.attackDamage = 25.f;
	}
	
	shortAttack.attacking = false;

	charabase::InitCharBase(
		longAttack.cb_,
		paperKing.cb_.pos.x + paperKing.cb_.width / 2, paperKing.cb_.pos.y, 0.f,
		0.f, 0.f, 0.f,
		"bow",
		0, 0, 0,
		12, 71
	);

	if (choice == KINDEND) {
		longAttack.attackDamage = 10.f;
	}
	else {
		longAttack.attackDamage = 20.f;
	}
	

	charabase::NoUseChar(shortAttack.cb_);
	charabase::NoUseChar(longAttack.cb_);

	for (auto& arrow : arrows) {
		charabase::InitCharBase(arrow.cb_);
		charabase::NoUseChar(arrow.cb_);
	}

	//��̍ŏ��̐�
	arrowCount = 20;
}

//���̉��̒������U���̈ړ��֐�
void MoveAttack() {
	switch (attackType)
	{
	case SHORTRANGE:
		switch ((int)paperKing.cb_.src.y)
		{
		case 0:
			shortAttack.cb_.resname = "swordH";
			shortAttack.cb_.pos.x = paperKing.cb_.pos.x + paperKing.cb_.width / 2 + 30;
			shortAttack.cb_.pos.y = paperKing.cb_.pos.y - 5;
			shortAttack.cb_.width = 70;
			shortAttack.cb_.height = 14;
			shortAttack.cb_.angle = 0;
			break;
		case 1:
			shortAttack.cb_.resname = "swordV";
			shortAttack.cb_.pos.x = paperKing.cb_.pos.x;
			shortAttack.cb_.pos.y = paperKing.cb_.pos.y - paperKing.cb_.height / 2 - 30;
			shortAttack.cb_.width = 14;
			shortAttack.cb_.height = 70;
			shortAttack.cb_.angle = 0;
			break;
		case 2:
			shortAttack.cb_.resname = "swordH";
			shortAttack.cb_.pos.x = paperKing.cb_.pos.x - paperKing.cb_.width / 2 - 30;
			shortAttack.cb_.pos.y = paperKing.cb_.pos.y - 5;
			shortAttack.cb_.width = 70;
			shortAttack.cb_.height = 14;
			shortAttack.cb_.angle = 180;
			break;
		case 3:
			shortAttack.cb_.resname = "swordV";
			shortAttack.cb_.pos.x = paperKing.cb_.pos.x;
			shortAttack.cb_.pos.y = paperKing.cb_.pos.y + paperKing.cb_.height / 2 + 30;
			shortAttack.cb_.width = 14;
			shortAttack.cb_.height = 70;
			shortAttack.cb_.angle = 180;
			break;
		}
		break;
	case LONGRANGE:
		switch ((int)paperKing.cb_.src.y)
		{
		case 0:
			longAttack.cb_.pos.x = paperKing.cb_.pos.x + paperKing.cb_.width / 2 + 6;
			longAttack.cb_.pos.y = paperKing.cb_.pos.y - 5;
			longAttack.cb_.angle = 0;
			break;
		case 1:
			longAttack.cb_.pos.x = paperKing.cb_.pos.x;
			longAttack.cb_.pos.y = paperKing.cb_.pos.y - paperKing.cb_.height / 2 - 6;
			longAttack.cb_.angle = 90;
			break;
		case 2:
			longAttack.cb_.pos.x = paperKing.cb_.pos.x - paperKing.cb_.width / 2 - 6;
			longAttack.cb_.pos.y = paperKing.cb_.pos.y - 5;
			longAttack.cb_.angle = 180;
			break;
		case 3:
			longAttack.cb_.pos.x = paperKing.cb_.pos.x;
			longAttack.cb_.pos.y = paperKing.cb_.pos.y + paperKing.cb_.height / 2 + 6;
			longAttack.cb_.angle = 270;
			break;
		}
		break;
	case EXCALIBUR:
		switch ((int)paperKing.cb_.src.y)
		{
		case 0:
			shortAttack.cb_.resname = "excaliburH";
			shortAttack.cb_.pos.x = paperKing.cb_.pos.x + paperKing.cb_.width / 2 + 30;
			shortAttack.cb_.pos.y = paperKing.cb_.pos.y - 5;
			shortAttack.cb_.width = 70;
			shortAttack.cb_.height = 14;
			shortAttack.cb_.angle = 0;
			break;
		case 1:
			shortAttack.cb_.resname = "excaliburV";
			shortAttack.cb_.pos.x = paperKing.cb_.pos.x;
			shortAttack.cb_.pos.y = paperKing.cb_.pos.y - paperKing.cb_.height / 2 - 30;
			shortAttack.cb_.width = 14;
			shortAttack.cb_.height = 70;
			shortAttack.cb_.angle = 0;
			break;
		case 2:
			shortAttack.cb_.resname = "excaliburH";
			shortAttack.cb_.pos.x = paperKing.cb_.pos.x - paperKing.cb_.width / 2 - 30;
			shortAttack.cb_.pos.y = paperKing.cb_.pos.y - 5;
			shortAttack.cb_.width = 70;
			shortAttack.cb_.height = 14;
			shortAttack.cb_.angle = 180;
			break;
		case 3:
			shortAttack.cb_.resname = "excaliburV";
			shortAttack.cb_.pos.x = paperKing.cb_.pos.x;
			shortAttack.cb_.pos.y = paperKing.cb_.pos.y + paperKing.cb_.height / 2 + 30;
			shortAttack.cb_.width = 14;
			shortAttack.cb_.height = 70;
			shortAttack.cb_.angle = 180;
			break;
		}
		break;
	}
}

//���̉��̍U���̕\���֐�
void DispAttack() {
	switch (attackType)
	{
	case SHORTRANGE:
		if (charabase::CheckUseChar(shortAttack.cb_)) {
			charabase::Draw_Char(shortAttack.cb_);
			charabase::NoUseChar(shortAttack.cb_);
		}
		break;
	case LONGRANGE:
		if (charabase::CheckUseChar(longAttack.cb_)) {
			charabase::Draw_Char(longAttack.cb_);
			charabase::NoUseChar(longAttack.cb_);
		}
		break;
	case EXCALIBUR:
		if (charabase::CheckUseChar(shortAttack.cb_)) {
			charabase::Draw_Char(shortAttack.cb_);
			charabase::NoUseChar(shortAttack.cb_);
		}
		break;
	}
}

//��̐��Y�֐�
void CreateArrows() {
	//����΂�����
	float addx = math::ROUND_X((float)paperKing.angle, 4.0f);
	float addy = math::ROUND_Y((float)paperKing.angle, 4.0f);

	for (auto& arrow : arrows) {
		if (charabase::CheckUseChar(arrow.cb_) == true) continue;

		switch ((int)longAttack.cb_.angle)
		{
		case 0:
			charabase::InitCharBase(
				arrow.cb_,
				paperKing.cb_.pos.x + paperKing.cb_.width / 2.f, paperKing.cb_.pos.y, 0.1f,
				addx, addy, 0.f,
				"arrowR",
				0, 0, 0,
				60, 6
			);
			break;
		case 90:
			charabase::InitCharBase(
				arrow.cb_,
				paperKing.cb_.pos.x, paperKing.cb_.pos.y - paperKing.cb_.height / 2.f, 0.f,
				addx, addy, 0.f,
				"arrowU",
				0, 0, 0,
				6, 60
			);
			break;
		case 180:
			charabase::InitCharBase(
				arrow.cb_,
				paperKing.cb_.pos.x - paperKing.cb_.width / 2.f, paperKing.cb_.pos.y, 0.f,
				addx, addy, 0.f,
				"arrowL",
				0, 0, 0,
				60, 6
				);
			break;
		case 270:
			charabase::InitCharBase(
				arrow.cb_,
				paperKing.cb_.pos.x, paperKing.cb_.pos.y + paperKing.cb_.height / 2.f, 0.f,
				addx, addy, 0.f,
				"arrowD",
				0, 0, 0,
				6, 60
			);
			break;
		}

		charabase::UseChar(arrow.cb_);

		arrowCount--;
		//�o��������̂͂ЂƂɂ���B�ЂƂo���シ���I��
		return;
	}
}

//��̈ړ��֐�
void MoveArrows() {
	for (auto& arrow : arrows) {
		if (!charabase::CheckUseChar(arrow.cb_)) continue;
		//�s���̂��̏o���������s��
		charabase::MoveChar(arrow.cb_);
		//��ʊO�ŏ���
		if (arrow.cb_.pos.x < 0) {
			charabase::NoUseChar(arrow.cb_);
		}
		if (arrow.cb_.pos.x > GAMEFIELDW) {
			charabase::NoUseChar(arrow.cb_);
		}
		if (arrow.cb_.pos.y < 0) {
			charabase::NoUseChar(arrow.cb_);
		}
		if (arrow.cb_.pos.y > GAMEFIELDH) {
			charabase::NoUseChar(arrow.cb_);
		}
	}
}

//��̕\���֐�
void DispArrows() {
	for (auto& arrow : arrows) {
		//�s���̂��͔̂�΂��Ď�����������
		if (!charabase::CheckUseChar(arrow.cb_)) continue;
		charabase::Draw_Char(arrow.cb_);
	}
}

//�G�N�X�J���o�[�̕K�E�Z�̍쐬�֐�
void CreateExcaliburBeam() {
	//�K�E�Z�̕���
	float addx = math::ROUND_X((float)paperKing.angle, 4.0f);
	float addy = math::ROUND_Y((float)paperKing.angle, 4.0f);

	for (auto& beam: excaliburBeam) {
		if (charabase::CheckUseChar(beam.cb_) == true) continue;

		charabase::InitCharBase(
			beam.cb_,
			shortAttack.cb_.pos.x, shortAttack.cb_.pos.y, 0.1f,
			addx, addy, 0.f,
			"excaliburBeamH",
			0, 0, 0,
			70, 32
		);

		if (paperKing.angle == 0) {
			addx += 4.f;
		}
		if (paperKing.angle == 90) {
			beam.cb_.width = 32;
			beam.cb_.height = 70;
			beam.cb_.resname = "excaliburBeamV";
			addy -= 4.f;
		}
		if (paperKing.angle == 180) {
			addx -= 4.f;
		}
		if (paperKing.angle == 270) {
			beam.cb_.width = 32;
			beam.cb_.height = 70;
			beam.cb_.resname = "excaliburBeamV";
			addy += 4.f;
		}

		beam.attacking = false;

		charabase::UseChar(beam.cb_);
	}
}

//�G�N�X�J���o�[�̕K�E�Z�̈ړ��֐�
void MoveExcaliburBeam() {
	for (auto& beam : excaliburBeam) {
		if (!checkAlive || checkCleared) {
			charabase::NoUseChar(beam.cb_);
		}

		if (charabase::CheckUseChar(beam.cb_) == false) continue;
		//�s���̂��̏o���������s��
		charabase::MoveChar(beam.cb_);
		//��ʊO�ŏ���
		if (beam.cb_.pos.x < 20) NoUseChar(beam.cb_);
		if (beam.cb_.pos.x > 940) NoUseChar(beam.cb_);
		if (beam.cb_.pos.y < 30) NoUseChar(beam.cb_);
		if (beam.cb_.pos.y > 820) NoUseChar(beam.cb_);
	}
}

//�G�N�X�J���o�[�̕K�E�Z�̕\���֐�
void DispExcaliburBeam() {
	for (auto& beam : excaliburBeam) {
		//�s���̂��͔̂�΂��Ď�����������
		if (!charabase::CheckUseChar(beam.cb_)) continue;
		charabase::Draw_Char(beam.cb_);
	}
}

//����ύX�̊֐�
void ChangeWeapon() {
	if (input::CheckPush(input::KEY_BTN1)) {
		if (checkGotTreasure[3]) {
			if (attackType == SHORTRANGE) {
				attackType = LONGRANGE;
			}
			else if (attackType == LONGRANGE) {
				attackType = EXCALIBUR;
				paperKing.provideDamage = 2.5f;
			}
			else if (attackType == EXCALIBUR) {
				attackType = SHORTRANGE;
				paperKing.provideDamage = 1.f;
			}
		}
		else {
			if (attackType == SHORTRANGE) {
				attackType = LONGRANGE;
			}
			else if (attackType == LONGRANGE) {
				attackType = SHORTRANGE;
			}
		}
	}
}

//���̉��̏������֐�
void InitPaperKing() {
	switch (currentStage) {
	case STG_TUTORIAL:
		paperKing.cb_.pos.x = system::WINW - 200.f;
		paperKing.cb_.pos.y = system::WINH - 300.f;
		break;
	case STG_1:
		paperKing.cb_.pos.x = system::WINW - 620.f;
		paperKing.cb_.pos.y = system::WINH + 120.f;
		break;
	case STG_2:
		paperKing.cb_.pos.x = 16.f;
		paperKing.cb_.pos.y = bridge.cb_.pos.y;
		break;
	default:
		paperKing.cb_.pos.x = system::WINW - 620.f;
		paperKing.cb_.pos.y = system::WINH - 200.f;
		break;
	}

	//���W�����󕨂ɂ���āA�X�e�[�W�̍ŏ��ɈႤ�������Ă��鎆�̉���\�����A�\�͒l�������ɂ���ĕς��
	if (checkGotTreasure[0]) {
		if (checkGotTreasure[1]) {
			if (checkGotTreasure[2]) {
				charabase::InitCharBase(
					paperKing.cb_,		//�������������\����
					paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.1f,		//�������W
					0.f, 0.f, 0.f,		//�ړ���
					"paperKingFullyArmored",		//�g�p�摜��
					0, 0, 0,		//�Q�Ɛ�
					32, 54		//�摜�̃T�C�Y(���A����)
				);
			}
			else {
				charabase::InitCharBase(
					paperKing.cb_,		//�������������\����
					paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.1f,		//�������W
					0.f, 0.f, 0.f,		//�ړ���
					"paperKingWPlac&Greave",		//�g�p�摜��
					0, 0, 0,		//�Q�Ɛ�
					32, 54		//�摜�̃T�C�Y(���A����)
				);
			}
			paperKing.moveSpeed = 7.f;
		}
		else {
			if (checkGotTreasure[2]) {
				charabase::InitCharBase(
					paperKing.cb_,		//�������������\����
					paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.1f,		//�������W
					0.f, 0.f, 0.f,		//�ړ���
					"paperKingWPlac&Gauntlet",		//�g�p�摜��
					0, 0, 0,		//�Q�Ɛ�
					32, 54		//�摜�̃T�C�Y(���A����)
				);
			}
			else {
				charabase::InitCharBase(
					paperKing.cb_,		//�������������\����
					paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.1f,		//�������W
					0.f, 0.f, 0.f,		//�ړ���
					"paperKingWPlackart",		//�g�p�摜��
					0, 0, 0,		//�Q�Ɛ�
					32, 54		//�摜�̃T�C�Y(���A����)
				);
			}
			paperKing.moveSpeed = 5.f;
		}
		paperKing.receivedDamage = 0.5f;
	}
	else {
		if (checkGotTreasure[1]) {
			if (checkGotTreasure[2]) {
				charabase::InitCharBase(
					paperKing.cb_,		//�������������\����
					paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.1f,		//�������W
					0.f, 0.f, 0.f,		//�ړ���
					"paperKingWGreave&Gauntlet",		//�g�p�摜��
					0, 0, 0,		//�Q�Ɛ�
					32, 54		//�摜�̃T�C�Y(���A����)
				);
			}
			else {
				charabase::InitCharBase(
					paperKing.cb_,		//�������������\����
					paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.1f,		//�������W
					0.f, 0.f, 0.f,		//�ړ���
					"paperKingWGreave",		//�g�p�摜��
					0, 0, 0,		//�Q�Ɛ�
					32, 54		//�摜�̃T�C�Y(���A����)
				);
			}
			paperKing.moveSpeed = 7.f;
		}
		else {
			if (checkGotTreasure[2]) {
				charabase::InitCharBase(
					paperKing.cb_,		//�������������\����
					paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.1f,		//�������W
					0.f, 0.f, 0.f,		//�ړ���
					"paperKingWGauntlet",		//�g�p�摜��
					0, 0, 0,		//�Q�Ɛ�
					32, 54		//�摜�̃T�C�Y(���A����)
				);
			}
			else {
				charabase::InitCharBase(
					paperKing.cb_,		//�������������\����
					paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.1f,		//�������W
					0.f, 0.f, 0.f,		//�ړ���
					"paperKing",		//�g�p�摜��
					0, 0, 0,		//�Q�Ɛ�
					32, 54		//�摜�̃T�C�Y(���A����)
				);
			}
			paperKing.moveSpeed = 5.f;
		}
		paperKing.receivedDamage = 1.f;
	}

	paperKing.provideDamage = 1.f;

	if (currentStage == STG_4 || currentStage == STG_5) {
		paperKing.maxExcaliburPower = 100.f;
		paperKing.excaliburPower = 0.f;
		charabase::InitCharBase(
			paperKing.fullPowerBar.cb_,
			paperKing.cb_.pos.x + 240.f, paperKing.cb_.pos.y + 290.f, 0.1f,
			0.f, 0.f, 0.f,
			"fullPowerBar",
			0, 0, 0,
			40, 8
		);
	}

	//���̉���HP�l
	paperKing.maxHP = 100.f;
	paperKing.hp = 100.f;

	//���̉��͕󔠂̌��������Ă��邩�ǂ���
	paperKing.hasKey = false;

	charabase::UseChar(paperKing.cb_);		//�L�����N�^�[�͉���Ԃ�ݒ肷��
	camera::SetLookAt(paperKing.cb_.pos.x, paperKing.cb_.pos.y);		//�L�����N�^�[�͏�ɉ�ʂ̒��S�ɕ\������
}

//���̉��̈ړ��̃A�j���[�V�����֐�
void PaperKingMoveAnimation() {
	moveTime++;
	if (moveTime == 10) {
		paperKing.cb_.src.x++;
		moveTime = 0;
	}

	if (paperKing.cb_.src.x == 3) {
		paperKing.cb_.src.x = 0;
	}
}

//���̉��̈ړ��֐�
void MovePaperKing() {
	if (paperKing.hp <= 0) {
		paperKing.cb_.src.x = 0;
		paperKing.cb_.src.y = 0;
		paperKing.cb_.width = 60;
		paperKing.cb_.height = 22;
		paperKing.cb_.resname = "paperKingDefeated";
		checkAlive = false;
	}

	//�Q�[���I�[�o�[�ɂȂ�����A���g���C�ł��邩�ǂ����`�F�b�N����
	if (!checkAlive) {
		if (life > 0) {
			graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y - 200.f, 0.f, "retryMessage", 0, 0, 444, 209);

			if (input::CheckPush(input::KEY_F1)) {
				checkAlive = true;
				
				SaveHighScore();

				ResetStageInfo();
				life--;

				switch (currentStage) {
				case STG_1:
					score = originalScore;
					checkGotTreasure[0] = false;
					stage::ChangeStage(STG_1);
					break;
				case STG_2:
					score = originalScore;
					checkGotTreasure[1] = false;
					stage::ChangeStage(STG_2);
					break;
				case STG_3:
					score = originalScore;
					checkGotTreasure[2] = false;
					stage::ChangeStage(STG_3);
					break;
				case STG_4:
					score = originalScore;
					checkGotTreasure[3] = false;

					if (attackType == EXCALIBUR) {
						attackType = SHORTRANGE;
						paperKing.provideDamage = 1.f;
					}

					stage::ChangeStage(STG_4);
					break;
				case STG_5:
					score = originalScore;
					stage::ChangeStage(STG_5);
					break;
				}
			}

			if (input::CheckPush(input::KEY_F2)) {
				checkAlive = true;

				SaveHighScore();

				ResetStageInfo();
				life--;

				switch (currentStage) {
				case STG_1:
					checkGotTreasure[0] = false;
					paperKing.hasKey = false;
					score = originalScore;
					stage::ChangeStage(STG_WORLDMAP);
					break;
				case STG_2:
					checkGotTreasure[1] = false;
					paperKing.hasKey = false;
					score = originalScore;
					stage::ChangeStage(STG_WORLDMAP);
					break;
				case STG_3:
					checkGotTreasure[2] = false;
					paperKing.hasKey = false;
					score = originalScore;
					stage::ChangeStage(STG_WORLDMAP);
					break;
				case STG_4:
					checkGotTreasure[3] = false;
					paperKing.hasKey = false;
					score = originalScore;

					if (attackType == EXCALIBUR) {
						attackType = SHORTRANGE;
						paperKing.provideDamage = 1.f;
					}

					stage::ChangeStage(STG_WORLDMAP);
					break;
				case STG_5:
					score = originalScore;
					stage::ChangeStage(STG_WORLDMAP);
					break;
				}
			}
		}
		else {
			SaveHighScore();
			stage::ChangeStage(STG_GAMEOVER);
		}
		return;
	}

	if (checkCleared) {
		return;
	}

	//�������Ȃ��ƈړ����Ȃ�
	paperKing.cb_.add.x = 0.f;
	paperKing.cb_.add.y = 0.f;

	//�����������L�[�ɉ����āA�������Ɉړ�����
	//������L�[����������
	if (input::CheckPress(input::KEY_UP) && !input::CheckPress(input::KEY_BTN0) && !input::CheckPress(input::KEY_BTN2)) {
		paperKing.cb_.add.y = -paperKing.moveSpeed;
		paperKing.cb_.src.y = 1;
		paperKing.angle = 90;

		PaperKingMoveAnimation();
	}

	//�������L�[����������
	if (input::CheckPress(input::KEY_DOWN) && !input::CheckPress(input::KEY_BTN0) && !input::CheckPress(input::KEY_BTN2)) {
		paperKing.cb_.add.y = paperKing.moveSpeed;
		paperKing.cb_.src.y = 3;
		paperKing.angle = 270;

		PaperKingMoveAnimation();
	}

	//�������L�[����������
	if (input::CheckPress(input::KEY_LEFT) && !input::CheckPress(input::KEY_BTN0) && !input::CheckPress(input::KEY_BTN2)) {
		paperKing.cb_.add.x = -paperKing.moveSpeed;
		paperKing.cb_.src.y = 2;
		paperKing.angle = 180;

		PaperKingMoveAnimation();
	}

	//�E�����L�[����������
	if (input::CheckPress(input::KEY_RIGHT) && !input::CheckPress(input::KEY_BTN0) && !input::CheckPress(input::KEY_BTN2)) {
		paperKing.cb_.add.x = paperKing.moveSpeed;
		paperKing.cb_.src.y = 0;
		paperKing.angle = 0;

		PaperKingMoveAnimation();
	}

	//������L�[�ƉE�����L�[�𓯎��ɉ�������
	if (input::CheckPress(input::KEY_UP) && input::CheckPress(input::KEY_RIGHT) && !input::CheckPress(input::KEY_BTN0) && !input::CheckPress(input::KEY_BTN2)) {
		paperKing.cb_.add.x = paperKing.moveSpeed;
		paperKing.cb_.add.y = -paperKing.moveSpeed;
		paperKing.angle = 0;
	}

	//������L�[�ƍ������L�[�𓯎��ɉ�������
	if (input::CheckPress(input::KEY_UP) && input::CheckPress(input::KEY_LEFT) && !input::CheckPress(input::KEY_BTN0) && !input::CheckPress(input::KEY_BTN2)) {
		paperKing.cb_.add.x = -paperKing.moveSpeed;
		paperKing.cb_.add.y = -paperKing.moveSpeed;
		paperKing.angle = 180;
	}

	//�������L�[�ƉE�����L�[�𓯎��ɉ�������
	if (input::CheckPress(input::KEY_DOWN) && input::CheckPress(input::KEY_RIGHT) && !input::CheckPress(input::KEY_BTN0) && !input::CheckPress(input::KEY_BTN2)) {
		paperKing.cb_.add.x = paperKing.moveSpeed;
		paperKing.cb_.add.y = paperKing.moveSpeed;
		paperKing.angle = 0;
	}

	//�������L�[�ƍ������L�[�𓯎��ɉ�������
	if (input::CheckPress(input::KEY_DOWN) && input::CheckPress(input::KEY_LEFT) && !input::CheckPress(input::KEY_BTN0) && !input::CheckPress(input::KEY_BTN2)) {
		paperKing.cb_.add.x = -paperKing.moveSpeed;
		paperKing.cb_.add.y = paperKing.moveSpeed;
		paperKing.angle = 180;
	}

	//A�{�^���������ƁA���̉����U������
	if (input::CheckPress(input::KEY_BTN0)) {
		//�U���̃T�E���h���o��
		if (input::CheckPush(input::KEY_BTN0)) {
			switch (attackType)
			{
			case SHORTRANGE:
				se::DSound_Play("swoosh");
				break;
			case LONGRANGE:
				se::DSound_Play("shootArrow");
				break;
			case EXCALIBUR:
				se::DSound_Play("swoosh");
				break;
			}
		}

		//���̉��͍U���|�[�Y�ɕς���āA�����\������
		if (attackTimer <= 10) {
			switch (attackType)
			{
			case SHORTRANGE:
				charabase::UseChar(shortAttack.cb_);
				break;
			case LONGRANGE:
				charabase::UseChar(longAttack.cb_);
				break;
			case EXCALIBUR:
				charabase::UseChar(shortAttack.cb_);
				break;
			}
		
			switch ((int)paperKing.cb_.src.y) {
			case 0:
				paperKing.cb_.src.x = 4;
				break;
			case 1:
				paperKing.cb_.src.x = 4;
				break;
			case 2:
				paperKing.cb_.src.x = 4;
				break;
			case 3:
				paperKing.cb_.src.x = 4;
				break;
			}
		}
		else {
			paperKing.cb_.src.x = 0;
		}
		
	}
	//A�{�^���𗣂��ƁA���̉��͗��|�[�Y�ɖ߂�
	else if(input::CheckPull(input::KEY_BTN0)) {
		paperKing.cb_.src.x = 0;
	}

	//�������̍U�����鎞�A��𐶐�����
	if (attackType == LONGRANGE && input::CheckPush(input::KEY_BTN0) && arrowCount > 0) {
		CreateArrows();
	}

	//�����ύX����
	ChangeWeapon();

	//�Q�[���̈�O�ɂ͏o���Ȃ��悤�Ƀ`�F�b�N����
	//�Q�[���̈�̍����`�F�b�N
	if (paperKing.cb_.pos.x + paperKing.cb_.add.x - (paperKing.cb_.width / 2) < 0) {
		paperKing.cb_.add.x = 0;
	}

	//�Q�[���̈�̉E���`�F�b�N
	if (paperKing.cb_.pos.x + paperKing.cb_.add.x + (paperKing.cb_.width / 2) > GAMEFIELDW) {
		paperKing.cb_.add.x = 0;
	}

	//�Q�[���̈�̏㑤�`�F�b�N
	if (paperKing.cb_.pos.y + paperKing.cb_.add.y - (paperKing.cb_.height / 2) < 0) {
		paperKing.cb_.add.y = 0;
	}

	//�Q�[���̈�̉����`�F�b�N
	if (paperKing.cb_.pos.y + paperKing.cb_.add.y + (paperKing.cb_.height / 2) > GAMEFIELDH) {
		paperKing.cb_.add.y = 0;
	}	

	//�L�����N�^�[�̈ړ����s��
	charabase::MoveChar(paperKing.cb_);
	camera::SetLookAt(paperKing.cb_.pos.x, paperKing.cb_.pos.y);

	//�X�e�[�W3�̕��͏�Q�����C������\�͂������Ă���
	if (checkGotTreasure[2]) {
		for (std::vector<ObstacleInfo>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
			if (charabase::HitCheck(paperKing.cb_, it->cb_)) {
				if (!(timer % 60)) {
					if (it->hp < it->maxHP) {
						it->hp += 5;
					}
				}
			}
		}
	}

	//�X�e�[�W4�̕��͕K�E�Z���g���܂�
	if (checkGotTreasure[3]) {
		float percentPower = paperKing.excaliburPower / paperKing.maxExcaliburPower;
		paperKing.fullPowerBar.cb_.scale.x = percentPower;
		paperKing.fullPowerBar.cb_.pos.x = paperKing.cb_.pos.x + 240.f - ((paperKing.maxExcaliburPower - paperKing.maxExcaliburPower * percentPower) / 5);
		paperKing.fullPowerBar.cb_.pos.y = paperKing.cb_.pos.y + 290.f;

		if (paperKing.excaliburPower < 0) {
			enableSpecialAttack = false;
		}
		else if(paperKing.excaliburPower >= 80) {
			enableSpecialAttack = true;
		}

		if (input::CheckPress(input::KEY_BTN2) && paperKing.excaliburPower > 0 && attackType == EXCALIBUR  && enableSpecialAttack) {
			charabase::Draw_Char(shortAttack.cb_);

			switch ((int)paperKing.cb_.src.y) {
			case 0:
				paperKing.cb_.src.x = 4;
				break;
			case 1:
				paperKing.cb_.src.x = 4;
				break;
			case 2:
				paperKing.cb_.src.x = 4;
				break;
			case 3:
				paperKing.cb_.src.x = 4;
				break;
			}

			CreateExcaliburBeam();

			se::DSound_Play("beam");

			paperKing.excaliburPower -= 0.5f;
		}
		else if (input::CheckPull(input::KEY_BTN2)) {
			paperKing.cb_.src.x = 0;
		}

		if (paperKing.excaliburPower < paperKing.maxExcaliburPower && timer % 3600) {
			paperKing.excaliburPower += 0.05f;
		}
	}

	//�X�e�[�W2�ɕ��Ȃ��Ɛ��ɐG��Ɛ����͂����炷
	if (currentStage == STG_2) {
		if (!charabase::HitCheck(paperKing.cb_, bridge.cb_) && !checkGotTreasure[1]) {
			if (!(timer % 60)) {
				paperKing.hp -= 5 * paperKing.receivedDamage;
			}
			if (!(timer % 120)) {
				paperKing.cb_.r++;
			}
		}
	}

	//�X�e�[�W3�Ƀv���C���[����Q���̉ƂɉB�ꂽ��HP�l�͂ǂ�ǂ�񕜂���
	if (currentStage == STG_3) {
		for (std::vector<ObstacleInfo>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
			if (charabase::HitCheck(paperKing.cb_, it->cb_)) {
				if (!(timer % 60)) {
					if (paperKing.hp < paperKing.maxHP) {
						paperKing.hp++;
					}
				}
			}
		}
	}

	//�X�e�[�W3�Ƀv���C���[����Q���̖؂ɍU������Ɗm���I�Ƀ����S�����Ƃ��܂�
	if (currentStage == STG_4) {
		for (std::vector<ObstacleInfo>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
			if (charabase::CheckUseChar(shortAttack.cb_) && charabase::CheckUseChar(it->cb_) && charabase::HitCheck(shortAttack.cb_, it->cb_)) {
				if (!shortAttack.attacking) {
					shortAttack.attacking = true;

					int lottery = (int)math::GetRandom(0.f, 10.f);

					if (lottery < 4) {
						CreateApple(it->cb_.pos.x, it->cb_.pos.y + it->cb_.height / 2.f);
					}
					if (lottery < 3) {
						it->hp -= shortAttack.attackDamage * paperKing.provideDamage;
					}
				}

				if (it->hp <= 0) {
					charabase::NoUseChar(it->cb_);
					
					score -= 20;
					requiredMoney += 7000;
				}
			}
			else if (!charabase::CheckUseChar(shortAttack.cb_)) {
				shortAttack.attacking = false;
			}

			for (auto& arrow : arrows) {
				if (!charabase::CheckUseChar(it->cb_)) continue;
				if (charabase::CheckUseChar(arrow.cb_) && charabase::CheckUseChar(it->cb_) && charabase::HitCheck(arrow.cb_, it->cb_)) {
					charabase::NoUseChar(arrow.cb_);

					int lottery = (int)math::GetRandom(0.f, 10.f);

					if (lottery < 5) {
						CreateApple(it->cb_.pos.x, it->cb_.pos.y + it->cb_.height / 2.f);
					}

					if (lottery < 2) {
						it->hp -= longAttack.attackDamage;
					}
					
					if (it->hp <= 0) {
						charabase::NoUseChar(it->cb_);

						score -= 20;
						requiredMoney += 7000;
					}
				}
			}
		}
	}

	if (appleCount > 0) {
		if (paperKing.hp < paperKing.maxHP) {
			if (input::CheckPush(input::KEY_SPACE)) {
				se::DSound_Play("appleBite");
				if ((paperKing.hp + 30) > paperKing.maxHP) {
					paperKing.hp = paperKing.maxHP;
				}
				else
				{
					paperKing.hp += 30;
				}
				appleCount--;
			}
		}
	}
}

//���̉��̕\���֐�
void DispPaperKing() {
	//����Ԃ��`�F�b�N����F���̎��L�����N�^�[��\�����A�s���̎��L�����N�^�[��\�����Ȃ�
	if (charabase::CheckUseChar(paperKing.cb_)) {
		charabase::Draw_Char(paperKing.cb_);
	}
}

//���̉���HP�l�̏������֐�
void InitPaperKingHPBar() {
	charabase::InitCharBase(
		paperKing.emptyHPBar.cb_,
		paperKing.cb_.pos.x, paperKing.cb_.pos.y - paperKing.cb_.height / 2.f - 5.f, 0.1f,
		0.f, 0.f, 0.f,
		"emptyBar",
		0, 0, 0,
		40, 8
	);

	charabase::InitCharBase(
		paperKing.fullHPBar.cb_,
		paperKing.cb_.pos.x, paperKing.cb_.pos.y - paperKing.cb_.height / 2.f - 5.f, 0.1f,
		0.f, 0.f, 0.f,
		"fullHPBar",
		0, 0, 0,
		40, 8
	);

	charabase::UseChar(paperKing.fullHPBar.cb_);
	charabase::UseChar(paperKing.emptyHPBar.cb_);
}

//���̉���HP�l�̕ω��֐�
void MovePaperKingHPBar() {
	float percentHP = paperKing.hp / paperKing.maxHP;
	paperKing.fullHPBar.cb_.scale.x = percentHP;

	paperKing.emptyHPBar.cb_.pos.x = paperKing.cb_.pos.x;
	paperKing.emptyHPBar.cb_.pos.y = paperKing.cb_.pos.y - paperKing.cb_.height / 2 - 5;
	paperKing.fullHPBar.cb_.pos.x = paperKing.cb_.pos.x - ((paperKing.maxHP - paperKing.maxHP * percentHP) / 5);
	paperKing.fullHPBar.cb_.pos.y = paperKing.cb_.pos.y - paperKing.cb_.height / 2 - 5;
}

//���̉���HP�l�̕\���֐�
void DispPaperKingHPBar() {
	if (charabase::CheckUseChar(paperKing.emptyHPBar.cb_)) {
		charabase::Draw_Char(paperKing.emptyHPBar.cb_);
	}

	if (charabase::CheckUseChar(paperKing.fullHPBar.cb_)) {
		charabase::Draw_Char(paperKing.fullHPBar.cb_);
	}
}

//�S�~�̏������֐�
void InitJunk() {
	for (auto& junk : junks) {
		//�S�~���ŏ��̐��l��^���A�s����Ԃɐݒ肷��
		charabase::InitCharBase(junk.cb_);
		charabase::NoUseChar(junk.cb_);
		junk.attacking = false;
	}
}

//�S�~�����֐�
void CreateJunk() {
	int junkNo;
	switch (currentStage) {
		case STG_TUTORIAL:
			junkNo = 1;
			break;
		default:
			switch (choice)
			{
				case KINDEND:
					switch (currentStage)
					{
					case STG_1:
						junkNo = 4;
						break;
					case STG_2:
						junkNo = 5;
						break;
					case STG_3:
						junkNo = 6;
						break;
					case STG_4:
						junkNo = 7;
						break;
					case STG_5:
						junkNo = 8;
						break;
					}
					break;
				case VIOLENTEND:
					switch (currentStage)
					{
					case STG_1:
						junkNo = 3;
						break;
					case STG_2:
						junkNo = 4;
						break;
					case STG_3:
						junkNo = 5;
						break;
					case STG_4:
						junkNo = 6;
						break;
					case STG_5:
						junkNo = 8;
						break;
					}
					break;
			}
			break;
	}

	for (int i = 0; i < junkNo; i++) {
		if (charabase::CheckUseChar(junks[i].cb_)) {
			continue;
		}

		//�S�~��x��y���W
		float x, y;

		int lottery;

		if (currentStage != STG_5) {
			while (true) {
				//�o���ʒu�͎l�̕������烉���_���Ō����
				lottery = (int)math::GetRandom(0.f, 3.f);

				switch (lottery)
				{
					//�S�~���㑤���猻���
				case 0:
					x = math::GetRandom(0.f, (float)GAMEFIELDW);
					y = 0;
					break;

					//�S�~���������猻���
				case 1:
					x = math::GetRandom(0.f, (float)GAMEFIELDW);
					y = (float)GAMEFIELDH;
					break;

					//�S�~���������猻���
				case 2:
					x = 0;
					y = math::GetRandom(0.f, (float)GAMEFIELDH);
					break;

					//�S�~���E�����猻���
				case 3:
					x = (float)GAMEFIELDW;
					y = math::GetRandom(0.f, (float)GAMEFIELDH);
					break;
				}

				//��ʊO�݂̂Ƃ���
				if (!camera::InScreenCamera((int)x, (int)y, 32, 32)) {
					break;
				}
			}
		}
		else {
			x = junkKing.cb_.pos.x;
			y = junkKing.cb_.pos.y;
		}

		junks[i].angle = math::DegreeOfPoints2(x, y, paperKing.cb_.pos.x, paperKing.cb_.pos.y);
		
		float addx, addy;

		//�v���C���[�̑I���ɂ���āA�X�e�[�W�̓�Փx���ς��
		if (choice == VIOLENTEND) {
			if (currentStage == STG_TUTORIAL || currentStage == STG_1) {
				addx = math::ROUND_X(junks[i].angle, 3.0f);
				addy = math::ROUND_Y(junks[i].angle, 3.0f);
			}
			else if (currentStage == STG_2 || currentStage == STG_3) {
				addx = math::ROUND_X(junks[i].angle, 3.5f);
				addy = math::ROUND_Y(junks[i].angle, 3.5f);
			}
			else{
				addx = math::ROUND_X(junks[i].angle, 4.0f);
				addy = math::ROUND_Y(junks[i].angle, 4.0f);
			}
		}
		else {
			addx = math::ROUND_X(junks[i].angle, 4.5f);
			addy = math::ROUND_Y(junks[i].angle, 4.5f);
		}
		
		//�X�e�[�W�ɂ���āA�Ⴄ��ނ̃S�~���o�Ă���
		switch (currentStage) {
			case STG_TUTORIAL:
			case STG_1:
				charabase::InitCharBase(
					junks[i].cb_,
					x, y, 0.1f,
					addx, addy, 0.f,
					"log",
					0, 0, 0,
					87, 50
				);
				
				junks[i].type = LOG;
				junks[i].attackDamage = 10.f;

				//�S�~��HP�l
				junks[i].maxHP = 100;
				junks[i].hp = 100;
				break;
			case STG_2:
				charabase::InitCharBase(
					junks[i].cb_,
					x, y, 0.1f,
					addx, addy, 0.f,
					"deadFish",
					0, 0, 0,
					70, 32
				);

				junks[i].type = DEADFISH;
				junks[i].attackDamage = 5.f;

				//�S�~��HP�l
				junks[i].maxHP = 50;
				junks[i].hp = 50;
				break;
			case STG_3:
				charabase::InitCharBase(
					junks[i].cb_,
					x, y, 0.1f,
					addx, addy, 0.f,
					"garbageBin",
					0, 0, 0,
					32, 46
				);

				junks[i].type = GARBAGEBIN;
				junks[i].attackDamage = 10.f;

				//�S�~��HP�l
				junks[i].maxHP = 100;
				junks[i].hp = 100;
				break;
			case STG_4:
				charabase::InitCharBase(
					junks[i].cb_,
					x, y, 0.1f,
					addx, addy, 0.f,
					"poo",
					0, 0, 0,
					32, 28
				);

				junks[i].type = POO;
				junks[i].attackDamage = 5.f;

				//�S�~��HP�l
				junks[i].maxHP = 100;
				junks[i].hp = 100;
				break;
			case STG_5:
				int lottery = math::GetRandom(1, 4);

				switch (lottery)
				{
				case 1:
					charabase::InitCharBase(
						junks[i].cb_,
						x, y, 0.1f,
						addx, addy, 0.f,
						"log",
						0, 0, 0,
						87, 50
					);

					junks[i].type = LOG;
					junks[i].attackDamage = 10.f;

					//�S�~��HP�l
					junks[i].maxHP = 100;
					junks[i].hp = 100;
					break;
				case 2:
					charabase::InitCharBase(
						junks[i].cb_,
						x, y, 0.1f,
						addx, addy, 0.f,
						"deadFish",
						0, 0, 0,
						70, 32
					);

					junks[i].type = DEADFISH;
					junks[i].attackDamage = 5.f;

					//�S�~��HP�l
					junks[i].maxHP = 50;
					junks[i].hp = 50;
					break;
				case 3:
					charabase::InitCharBase(
						junks[i].cb_,
						x, y, 0.1f,
						addx, addy, 0.f,
						"garbageBin",
						0, 0, 0,
						32, 46
					);

					junks[i].type = GARBAGEBIN;
					junks[i].attackDamage = 10.f;

					//�S�~��HP�l
					junks[i].maxHP = 100;
					junks[i].hp = 100;
					break;
				case 4:
					charabase::InitCharBase(
						junks[i].cb_,
						x, y, 0.1f,
						addx, addy, 0.f,
						"poo",
						0, 0, 0,
						32, 28
					);

					junks[i].type = POO;
					junks[i].attackDamage = 5.f;

					//�S�~��HP�l
					junks[i].maxHP = 100;
					junks[i].hp = 100;
					break;
				}
				break;
		}
		
		//�󔠂̌��̓����_���Ɉ�̃S�~�ɉB��
		switch (currentStage)
		{
		case STG_TUTORIAL:
			lottery = (int)math::GetRandom(0.f, 2.f);
			if (lottery < junkNo && !createdKey) {
				junks[lottery].hideKey = true;
				createdKey = true;
			}
			break;
		case STG_1:
			if (!checkGotTreasure[0]) {
				lottery = (int)math::GetRandom(0.f, 8.f);
				if (lottery < junkNo && !createdKey) {
					junks[lottery].hideKey = true;
					createdKey = true;
				}
			}
			break;
		case STG_2:
			if (!checkGotTreasure[1]) {
				lottery = (int)math::GetRandom(0.f, 8.f);
				if (lottery < junkNo && !createdKey) {
					junks[lottery].hideKey = true;
					createdKey = true;
				}
			}
			break;
		case STG_3:
			if (!checkGotTreasure[2]) {
				lottery = (int)math::GetRandom(0.f, 8.f);
				if (lottery < junkNo && !createdKey) {
					junks[lottery].hideKey = true;
					createdKey = true;
				}
			}
			break;
		case STG_4:
			if (!checkGotTreasure[3]) {
				lottery = (int)math::GetRandom(0.f, 8.f);
				if (lottery < junkNo && !createdKey) {
					junks[lottery].hideKey = true;
					createdKey = true;
				}
			}
			break;
		}
		
		charabase::UseChar(junks[i].cb_);
		
		//�o��������̂͂ЂƂɂ���B�ЂƂo���シ���I��
		return;
	}
}

//�S�~�̈ړ��֐�
void MoveJunk() {
	//���̉���HP��0�ɂȂ�������A�S�~�̓������~�܂�
	if (!checkAlive || checkCleared) {
		return;
	}

	//�S�~�̑��x�̓Q�[�����ŕς��
	int lottery = (int)math::GetRandom(0.f, 200.f);
	for (int index = 0; index < JUNKMAX; index++) {
		if (index == lottery) {
			//�v���C���[�̑I���ɂ���āA�S�~�̃X�s�[�h�̕ϊ��̕����Ⴄ
			if (currentStage == STG_TUTORIAL) {
				junks[index].cb_.add.x = math::GetRandom(1.f, 3.5f);
				junks[index].cb_.add.y = math::GetRandom(1.f, 3.5f);
			}
			else if (choice == VIOLENTEND) {
				if (currentStage == STG_1) {
					junks[index].cb_.add.x = math::GetRandom(1.f, 4.f);
					junks[index].cb_.add.y = math::GetRandom(1.f, 4.f);
				}
				else if (currentStage == STG_2) {
					junks[index].cb_.add.x = math::GetRandom(1.f, 4.5f);
					junks[index].cb_.add.y = math::GetRandom(1.f, 4.5f);
				}
				else{
					junks[index].cb_.add.x = math::GetRandom(1.f, 6.f);
					junks[index].cb_.add.y = math::GetRandom(1.f, 6.f);
				}
			}
			else if(choice == KINDEND) {
				junks[index].cb_.add.x = math::GetRandom(1.f, 6.5f);
				junks[index].cb_.add.y = math::GetRandom(1.f, 6.5f);
			}
		}
		junks[index].cb_.pos.x += junks[index].cb_.add.x;
		junks[index].cb_.pos.y += junks[index].cb_.add.y;

		if (junks[index].cb_.pos.x - (junks[index].cb_.width / 2.f) < 0.f) {
			junks[index].cb_.pos.x = junks[index].cb_.width / 2.f;
			junks[index].cb_.add.x = -junks[index].cb_.add.x;
		}
		else if (junks[index].cb_.pos.x + (junks[index].cb_.width / 2.f) > GAMEFIELDW) {
			junks[index].cb_.pos.x = GAMEFIELDW - (junks[index].cb_.width / 2.f);
			junks[index].cb_.add.x = -junks[index].cb_.add.x;
		}
		if (junks[index].cb_.pos.y - (junks[index].cb_.height / 2.f) < 0.f) {
			junks[index].cb_.pos.y = junks[index].cb_.height / 2.f;
			junks[index].cb_.add.y = -junks[index].cb_.add.y;
		}
		else if (junks[index].cb_.pos.y + (junks[index].cb_.height / 2.f) > GAMEFIELDH) {
			junks[index].cb_.pos.y = GAMEFIELDH - (junks[index].cb_.height / 2.f);
			junks[index].cb_.add.y = -junks[index].cb_.add.y;
		}
	}

	//�S�~�̂����蔻��
	for (auto& junk : junks) {
		//���Ƃ̂����蔻��
		if (charabase::CheckUseChar(shortAttack.cb_) && charabase::CheckUseChar(junk.cb_) && charabase::HitCheck(shortAttack.cb_, junk.cb_)) {
			if (!shortAttack.attacking) {
				shortAttack.attacking = true;
				junk.hp -= shortAttack.attackDamage * paperKing.provideDamage;
				junk.cb_.add.x = -junk.cb_.add.x;
				junk.cb_.add.y = -junk.cb_.add.y;
			}

			if (junk.hp <= 0) {
				charabase::NoUseChar(junk.cb_);
				score += 10;
				junk.cb_.add.x = 0;
				junk.cb_.add.y = 0;

				if (junk.hideKey) {
					CreateKey(junk.cb_.pos.x, junk.cb_.pos.y);
					junk.hideKey = false;
				}

				junk.hp = 100;
			}
		}
		else if (!charabase::CheckUseChar(shortAttack.cb_)) {
			shortAttack.attacking = false;
		}

		//��Ƃ̂����蔻��
		for (auto& arrow : arrows) {
			if (!charabase::CheckUseChar(junk.cb_)) continue;
			if (charabase::CheckUseChar(arrow.cb_) && charabase::CheckUseChar(junk.cb_) && charabase::HitCheck(arrow.cb_, junk.cb_)) {
				charabase::NoUseChar(arrow.cb_);
				junk.hp -= longAttack.attackDamage;

				int lottery = math::GetRandom(0, 3);

				if (lottery == 0) {
					junk.cb_.add.x = -junk.cb_.add.x;
					junk.cb_.add.y = -junk.cb_.add.y;
				}

				if (junk.hp <= 0) {
					charabase::NoUseChar(junk.cb_);
					arrowCount += 10;
					score += 10;

					junk.hp = 100;
				}
			}
		}
		
		//�K�E�Z�Ƃ̂����蔻��
		for (auto& beam : excaliburBeam) {
			if (charabase::CheckUseChar(beam.cb_) && charabase::CheckUseChar(junk.cb_) && charabase::HitCheck(beam.cb_, junk.cb_)) {
				junk.hp--;

				if (junk.hp <= 0) {
					charabase::NoUseChar(junk.cb_);
					score += 10;

					junk.hp = 100;
				}
			}
		}

		//��Q���Ƃ̂����蔻��
		for (std::vector<ObstacleInfo>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
			if (charabase::CheckUseChar(it->cb_)) {
				if (charabase::HitCheck(it->cb_, junk.cb_)) {
					if (charabase::CheckUseChar(junk.cb_)) {
						it->hp -= junk.attackDamage;
						junk.cb_.add.x = -junk.cb_.add.x;
						junk.cb_.add.y = -junk.cb_.add.y;

						//�X�e�[�W�ɂ���āA��Q�����󂳂ꂽ�̃y�i���e�B�[���ς��
						if (it->hp <= 0) {
							charabase::NoUseChar(it->cb_);

							if (currentStage != STG_TUTORIAL) {
								switch (obstacle.type)
								{
								case FOUNTAIN:
									requiredMoney += 4000;
									break;
								case STATUE:
									requiredMoney += 2000;
									break;
								case HOUSE:
									requiredMoney += 10000;
									break;
								case STREETLIGHT:
									requiredMoney += 3000;
									break;
								case TREE:
									requiredMoney += 7000;
									break;
								}
							}
						}
					}
				}
			}
		}

		//���̉��Ƃ̂����蔻��
		if (charabase::CheckUseChar(junk.cb_)) {
			if (charabase::HitCheck(paperKing.cb_, junk.cb_)) {
				//���̉��ƂԂ�������A�S�~�̎�ނɂ���āA�T�E���h���o��
				switch (junk.type) {
				case LOG:
					se::DSound_Play("smashing");
					break;
				case DEADFISH:
					se::DSound_Play("bump");
					break;
				case GARBAGEBIN:
					se::DSound_Play("metalBang");
					break;
				case POO:
					se::DSound_Play("farty");
					break;
				}

				//�S�~�����̉��ɓ���������A���̉���HP�l�����炵�A�S�~���t�̕����Ɉړ�
				if (!junk.attacking) {
					junk.attacking = true;
					if (currentStage != STG_TUTORIAL) {
						paperKing.hp -= junk.attackDamage * paperKing.receivedDamage;
					}
					junk.cb_.add.x = -junk.cb_.add.x;
					junk.cb_.add.y = -junk.cb_.add.y;
				}

				return;
			}
			else {
				junk.attacking = false;
			}
		}
	}

	if (currentStage != STG_5) {
		//���t���[��100����1�ŏo��
		if (math::GetRandom(0, 100) == 0) {
			CreateJunk();
		}
	}
}

//�S�~�̕\���֐�
void DispJunk() {
	for (const auto& junk : junks) {
		//����ԂɂȂ��Ă���S�~�����\�����Ȃ�
		if (!charabase::CheckUseChar(junk.cb_)) {
			continue;
		}
		charabase::Draw_Char(junk.cb_);
	}
}

//�S�~��HP�l�̏������֐�
void InitJunkHPBar() {
	for (auto& junk : junks) {
		charabase::InitCharBase(
			junk.emptyHPBar.cb_,													//�������������\����
			junk.cb_.pos.x, junk.cb_.pos.y - junk.cb_.height / 2.f - 5.f, 0.1f,		//�������W
			0.f, 0.f, 0.f,															//�ړ���
			"emptyBar",																//�g�p�摜��
			0, 0, 0,																//�Q�Ɛ�
			40, 8																	//�摜�̃T�C�Y(���A����)
		);

		charabase::InitCharBase(
			junk.fullHPBar.cb_,														//�������������\����
			junk.cb_.pos.x, junk.cb_.pos.y - junk.cb_.height / 2.f - 5.f, 0.1f,		//�������W
			0.f, 0.f, 0.f,															//�ړ���
			"fullHPBar",															//�g�p�摜��
			0, 0, 0,																//�Q�Ɛ�
			40, 8																	//�摜�̃T�C�Y(���A����)
		);

		charabase::UseChar(junk.fullHPBar.cb_);
		charabase::UseChar(junk.emptyHPBar.cb_);
	}
}

//�S�~��HP�l�̕ω��֐�
void MoveJunkHPBar() {
	for (auto& junk : junks) {
		float percentHP = junk.hp / junk.maxHP;
		junk.fullHPBar.cb_.scale.x = percentHP;

		junk.emptyHPBar.cb_.pos.x = junk.cb_.pos.x;
		junk.emptyHPBar.cb_.pos.y = junk.cb_.pos.y - junk.cb_.height / 2.f - 5.f;
		if (junk.maxHP == 50) {
			junk.fullHPBar.cb_.pos.x = junk.cb_.pos.x - ((junk.maxHP - junk.maxHP * percentHP) / 2.5f);
		}
		else {
			junk.fullHPBar.cb_.pos.x = junk.cb_.pos.x - ((junk.maxHP - junk.maxHP * percentHP) / 5);
		}
		junk.fullHPBar.cb_.pos.y = junk.cb_.pos.y - junk.cb_.height / 2.f - 5.f;
	}
}

//�S�~��HP�l�̕\���֐�
void DispJunkHPBar() {
	for (auto& junk : junks) {
		if (charabase::CheckUseChar(junk.cb_) && charabase::CheckUseChar(junk.emptyHPBar.cb_)) {
			charabase::Draw_Char(junk.emptyHPBar.cb_);
		}

		if (charabase::CheckUseChar(junk.cb_) && charabase::CheckUseChar(junk.fullHPBar.cb_)) {
			charabase::Draw_Char(junk.fullHPBar.cb_);
		}
	}
}

//�S�~�̉��̏������֐�
void InitJunkKing() {
	float xPos = GAMEFIELDW - 50.f;
	float yPos = (float)math::GetRandom(72, GAMEFIELDH - 72);

	charabase::InitCharBase(
		junkKing.cb_,		//�������������\����
		xPos, yPos, 0.1f,	//�������W
		0.f, 0.f, 0.f,		//�ړ���
		"junkKing",		//�g�p�摜��
		0, 0, 0,		//�Q�Ɛ�
		100, 144		//�摜�̃T�C�Y(���A����)
	);

	//�S�~�̉���HP�l
	junkKing.maxHP = 2000.f;
	junkKing.hp = 2000.f;

	//�S�~�̉��̍U���Ɋւ���ϐ���ݒ�
	junkKing.attackDamage = 10.f;
	junkKing.attacking = false;
	junkKing.attacked = false;
	junkKing.dashing = false;
	junkKing.speedUp = false;
	junkKing.powerUp = false;
	junkKing.detectedAttack = false;

	//�v���C���[�̑I���ɂ���āA�S�~�̉��̈ړ����x���ς��
	if (choice == VIOLENTEND) {
		junkKing.moveSpeed = 3.f;
	}
	else {
		junkKing.moveSpeed = 4.f;
	}

	junkKing.receivedDamage = 1.f;
	junkKing.provideDamage = 1.f;
	
	junkKing.decision = 1;

	junkKing.width = 100;
	junkKing.height = 144;

	junkKing.cb_.src.y = 2;

	charabase::UseChar(junkKing.cb_);		//�L�����N�^�[�͉���Ԃ�ݒ肷��
}

//�S�~�̉���AI�֐�
int JunkKingAI() {
	if (hitWall) {
		switch (wallside)
		{
		case TOP:
			do {
				junkKing.decision = math::GetRandom(1, 9);
			} while (junkKing.decision == 2 || junkKing.decision == 6 || junkKing.decision == 7);
			break;
		case DOWN:
			do {
				junkKing.decision = math::GetRandom(1, 9);
			} while (junkKing.decision == 3 || junkKing.decision == 8 || junkKing.decision == 9);
			break;
		case LEFT:
			do {
				junkKing.decision = math::GetRandom(1, 9);
			} while (junkKing.decision == 4 || junkKing.decision == 7 || junkKing.decision == 9);
			break;
		case RIGHT:
			do {
				junkKing.decision = math::GetRandom(1, 9);
			} while (junkKing.decision == 5 || junkKing.decision == 6 || junkKing.decision == 8);
			break;
		}
		hitWall = false;
	}
	else {
		if (junkKing.cb_.pos.x + junkKing.cb_.add.x - (junkKing.cb_.width / 2) < 0) {
			junkKing.decision = 5;
		}
		else if (junkKing.cb_.pos.x + junkKing.cb_.add.x + (junkKing.cb_.width / 2) > GAMEFIELDW) {
			junkKing.decision = 4;
		}
		else if (junkKing.cb_.pos.y + junkKing.cb_.add.y - (junkKing.cb_.height / 2) < 0) {
			junkKing.decision = 3;
		}
		else if (junkKing.cb_.pos.y + junkKing.cb_.add.y + (junkKing.cb_.height / 2) > GAMEFIELDH) {
			junkKing.decision = 2;
		}
		else if ((junkKing.cb_.pos.x + junkKing.cb_.add.x - (junkKing.cb_.width / 2) < 0) && 
				 (junkKing.cb_.pos.y + junkKing.cb_.add.y - (junkKing.cb_.height / 2) < 0)) {
			junkKing.decision = 8;
		}
		else if ((junkKing.cb_.pos.x + junkKing.cb_.add.x + (junkKing.cb_.width / 2) > GAMEFIELDW) &&
				 (junkKing.cb_.pos.y + junkKing.cb_.add.y - (junkKing.cb_.height / 2) < 0)) {
			junkKing.decision = 9;
		}
		else if ((junkKing.cb_.pos.x + junkKing.cb_.add.x + (junkKing.cb_.width / 2) > GAMEFIELDW) &&
			     (junkKing.cb_.pos.y + junkKing.cb_.add.y + (junkKing.cb_.height / 2) > GAMEFIELDH)) {
			junkKing.decision = 7;
		}
		else if ((junkKing.cb_.pos.x + junkKing.cb_.add.x - (junkKing.cb_.width / 2) < 0) &&
				 (junkKing.cb_.pos.y + junkKing.cb_.add.y + (junkKing.cb_.height / 2) > GAMEFIELDH)) {
			junkKing.decision = 6;
		}
		else {
			if ((junkKing.hp / junkKing.maxHP) <= 0.5f) {
				decisionTime++;
				if (decisionTime == 300) {
					int lottery = math::GetRandom(0, 1);

					switch (lottery)
					{
					case 0:
						junkKing.decision = math::GetRandom(1, 9);
						break;
					case 1:
						junkKing.decision = 10;
						break;
					}
					decisionTime = 0;
				}
			}
			else {
				decisionTime++;
				if (decisionTime == 300) {
					junkKing.decision = math::GetRandom(1, 9);
					decisionTime = 0;
				}
			}
		}
	}

	return junkKing.decision;
}

//�S�~�̉��̍U���֐�
void JunkKingDashing() {
	junkKing.dashing = true;
	if (!junkKing.attacked) {
		junkKing.angle = math::DegreeOfPoints2(junkKing.cb_.pos.x, junkKing.cb_.pos.y, paperKing.cb_.pos.x, paperKing.cb_.pos.y);
	}


	float addx = math::ROUND_X(junkKing.angle, 6.5f);
	float addy = math::ROUND_Y(junkKing.angle, 6.5f);

	if (addx > 0) {
		junkKing.cb_.src.x = 4;
		junkKing.cb_.src.y = 0;
	}
	else {
		junkKing.cb_.src.x = 4;
		junkKing.cb_.src.y = 2;
	}

	junkKing.cb_.add.x = addx;
	junkKing.cb_.add.y = addy;

	se::DSound_Play("flying");

	CreateJunk();
}

//�S�~�̉��̈ړ��̃A�j���[�V�����֐�
void JunkKingMoveAnimation() {
	junkKingMoveTime++;
	if (junkKingMoveTime == 10) {
		junkKing.cb_.src.x++;
		junkKingMoveTime = 0;
	}

	if (junkKing.cb_.src.x >= 3) {
		junkKing.cb_.src.x = 0;
	}
}

//�S�~�̉��̈ړ��֐�
void MoveJunkKing() {
	//�S�~�̉���HP�l��0��������0�ȉ��ɂȂ�����A�T�E���h���o���A�|���ꂽ���{�b�g�̉摜���ς��
	if (junkKing.hp <= 0) {
		junkKing.hp = 0;
		score += 250;
		se::DSound_Play("dyingRobot");
		checkDefeatedBoss = true;
	}

	//�v���C���[�̑I���ɂ���āA�S�~�̉��̈ړ����x���ς��
	if (choice == VIOLENTEND) {
		if (((junkKing.hp / junkKing.maxHP) <= 0.7f) && !junkKing.speedUp) {
			junkKing.speedUp = true;
			junkKing.moveSpeed += 2.f;
		}

		if (((junkKing.hp / junkKing.maxHP) <= 0.2f) && !junkKing.powerUp) {
			junkKing.powerUp = true;
			junkKing.receivedDamage = 0.5f;
			junkKing.provideDamage = 1.5f;
		}
	}
	else {
		if (((junkKing.hp / junkKing.maxHP) <= 0.8f) && !junkKing.speedUp) {
			junkKing.speedUp = true;
			junkKing.moveSpeed += 2.5f;
		}

		if (((junkKing.hp / junkKing.maxHP) <= 0.3f) && !junkKing.powerUp) {
			junkKing.powerUp = true;
			junkKing.receivedDamage = 0.5f;
			junkKing.provideDamage = 2.f;
		}
	}

	//���̉��̍U����T�m����
	if (charabase::HitCheckForRange(paperKing.cb_, junkKing.cb_) || junkKing.detectedAttack) {
		JunkKingDashing();
	}
	else if (!junkKing.dashing) {
		junkKing.attacked = false;
		junkKing.dashing = false;

		JunkKingAI();
			
		switch (junkKing.decision)
		{
		case 1:
			//�������Ȃ��ƈړ����Ȃ�
			junkKing.cb_.add.x = 0.f;
			junkKing.cb_.add.y = 0.f;
			break;
		case 2:
			//������Ɉړ�
			junkKing.cb_.src.y = 1;
			junkKing.cb_.add.x = 0.f;
			junkKing.cb_.add.y = -junkKing.moveSpeed;

			JunkKingMoveAnimation();
			break;
		case 3:
			//�������Ɉړ�
			junkKing.cb_.src.y = 3;
			junkKing.cb_.add.x = 0.f;
			junkKing.cb_.add.y = junkKing.moveSpeed;

			JunkKingMoveAnimation();
			break;
		case 4:
			//�������Ɉړ�
			junkKing.cb_.src.y = 2;
			junkKing.cb_.add.x = -junkKing.moveSpeed;
			junkKing.cb_.add.y = 0.f;

			JunkKingMoveAnimation();
			break;
		case 5:
			//�E�����Ɉړ�
			junkKing.cb_.src.y = 0;
			junkKing.cb_.add.x = junkKing.moveSpeed;
			junkKing.cb_.add.y = 0.f;

			JunkKingMoveAnimation();
			break;
		case 6:
			//�E������Ɉړ�
			junkKing.cb_.src.y = 0;
			junkKing.cb_.add.x = junkKing.moveSpeed;
			junkKing.cb_.add.y = -junkKing.moveSpeed;

			JunkKingMoveAnimation();
			break;
		case 7:
			//��������Ɉړ�
			junkKing.cb_.src.y = 2;
			junkKing.cb_.add.x = -junkKing.moveSpeed;
			junkKing.cb_.add.y = -junkKing.moveSpeed;

			JunkKingMoveAnimation();
			break;
		case 8:
			//�E�������Ɉړ�
			junkKing.cb_.src.y = 0;
			junkKing.cb_.add.x = junkKing.moveSpeed;
			junkKing.cb_.add.y = junkKing.moveSpeed;

			JunkKingMoveAnimation();
			break;
		case 9:
			//���������Ɉړ�
			junkKing.cb_.src.y = 2;
			junkKing.cb_.add.x = -junkKing.moveSpeed;
			junkKing.cb_.add.y = junkKing.moveSpeed;

			JunkKingMoveAnimation();
			break;
		case 10:
			//�v���C���[�̕����Ɉړ�
			junkKing.angle = math::DegreeOfPoints2(junkKing.cb_.pos.x, junkKing.cb_.pos.y, paperKing.cb_.pos.x, paperKing.cb_.pos.y);

			float addx = math::ROUND_X(junkKing.angle, 5.5f);
			float addy = math::ROUND_Y(junkKing.angle, 5.5f);

			if (addx > 0) {
				junkKing.cb_.src.y = 0;
			}
			else if (addx < 0) {
				junkKing.cb_.src.y = 2;
			}
			else if (addx == 0 && addy > 0) {
				junkKing.cb_.src.y = 3;
			}
			else if (addx == 0 && addy < 0) {
				junkKing.cb_.src.y = 1;
			}
			junkKing.cb_.add.x = addx;
			junkKing.cb_.add.y = addy;

			JunkKingMoveAnimation();

			break;
		}
	}

	//�Q�[���̈�O�ɂ͏o���Ȃ��悤�Ƀ`�F�b�N����
	//�Q�[���̈�̍����`�F�b�N
	if (junkKing.cb_.pos.x + junkKing.cb_.add.x - (junkKing.cb_.width / 2) < 0) {
		junkKing.dashing = false;
		hitWall = true;
		wallside = LEFT;
		JunkKingAI();
	}

	//�Q�[���̈�̉E���`�F�b�N
	if (junkKing.cb_.pos.x + junkKing.cb_.add.x + (junkKing.cb_.width / 2) > GAMEFIELDW) {
		junkKing.dashing = false;
		hitWall = true;
		wallside = RIGHT;
		JunkKingAI();
	}

	//�Q�[���̈�̏㑤�`�F�b�N
	if (junkKing.cb_.pos.y + junkKing.cb_.add.y - (junkKing.cb_.height / 2) < 0) {
		junkKing.dashing = false;
		hitWall = true;
		wallside = TOP;
		JunkKingAI();
	}

	//�Q�[���̈�̉����`�F�b�N
	if (junkKing.cb_.pos.y + junkKing.cb_.add.y + (junkKing.cb_.height / 2) > GAMEFIELDH) {
		junkKing.dashing = false;
		hitWall = true;
		wallside = DOWN;
		JunkKingAI();
	}

	//�L�����N�^�[�̈ړ����s��
	charabase::MoveChar(junkKing.cb_);

	if ((junkKing.cb_.src.y == 0 || junkKing.cb_.src.y == 2) && junkKing.cb_.src.x != 4) {
		junkKing.width = 70;
	}
	else {
		junkKing.width = 100;
	}

	//�v���C���[�̑I���ɂ���āA�S�~�̉��̋������ς��
	if (charabase::CheckUseChar(shortAttack.cb_) && charabase::HitCheck2(shortAttack.cb_, junkKing.cb_, junkKing.width, junkKing.height)) {
		if (!shortAttack.attacking) {
			shortAttack.attacking = true;
			if (choice == KINDEND) {
				if (junkKing.dashing) {
					junkKing.hp -= shortAttack.attackDamage * paperKing.provideDamage * junkKing.receivedDamage * 0.5f;
				}
				else {
					junkKing.hp -= shortAttack.attackDamage * paperKing.provideDamage * junkKing.receivedDamage * 0.8f;
				}
			}
			else {
				junkKing.hp -= shortAttack.attackDamage * paperKing.provideDamage * junkKing.receivedDamage;
			}
		}
	}
	else if (!charabase::CheckUseChar(shortAttack.cb_)) {
		shortAttack.attacking = false;
	}

	for (auto& arrow : arrows) {
		if (choice == VIOLENTEND) {
			if ((junkKing.hp / junkKing.maxHP) <= 0.6f) {
				if (charabase::CheckUseChar(arrow.cb_) && charabase::HitCheckForRange(arrow.cb_, junkKing.cb_)) {
					junkKing.detectedAttack = true;
				}
				else {
					junkKing.detectedAttack = false;
				}
			}
		}
		else {
			if ((junkKing.hp / junkKing.maxHP) <= 0.7f) {
				if (charabase::CheckUseChar(arrow.cb_) && charabase::HitCheckForRange(arrow.cb_, junkKing.cb_)) {
					junkKing.detectedAttack = true;
				}
				else {
					junkKing.detectedAttack = false;
				}
			}
		}

		if (!charabase::CheckUseChar(junkKing.cb_)) continue;
		if (charabase::CheckUseChar(arrow.cb_) && charabase::HitCheck2(arrow.cb_, junkKing.cb_, junkKing.width, junkKing.height)) {
			charabase::NoUseChar(arrow.cb_);
			if (choice == KINDEND) {
				if (junkKing.dashing) {
					junkKing.hp -= longAttack.attackDamage * junkKing.receivedDamage * 0.5f;
				}
				else {
					junkKing.hp -= longAttack.attackDamage * junkKing.receivedDamage;
				}
			}
			else {
				junkKing.hp -= longAttack.attackDamage * junkKing.receivedDamage;
			}
		}
	}

	for (auto& beam : excaliburBeam) {
		if (choice == VIOLENTEND) {
			if ((junkKing.hp / junkKing.maxHP) <= 0.6f) {
				if (charabase::CheckUseChar(beam.cb_) && charabase::HitCheckForRange(beam.cb_, junkKing.cb_)) {
					junkKing.detectedAttack = true;
				}
				else {
					junkKing.detectedAttack = false;
				}
			}
		}
		else {
			if ((junkKing.hp / junkKing.maxHP) <= 0.7f) {
				if (charabase::CheckUseChar(beam.cb_) && charabase::HitCheckForRange(beam.cb_, junkKing.cb_)) {
					junkKing.detectedAttack = true;
				}
				else {
					junkKing.detectedAttack = false;
				}
			}
		}

		if (charabase::CheckUseChar(beam.cb_) && charabase::HitCheck2(beam.cb_, junkKing.cb_, junkKing.width, junkKing.height)) {
			if (!beam.attacking) {
				beam.attacking = true;
				if (choice == KINDEND) {
					if (junkKing.dashing) {
						junkKing.hp -= 1 * junkKing.receivedDamage * 0.5f;
					}
					else {
						junkKing.hp -= 1 * junkKing.receivedDamage * 0.8f;
					}
				}
				else {
					junkKing.hp -= 1 * junkKing.receivedDamage;
				}
			}
		}
		else {
			beam.attacking = false;
		}
	}

	if (charabase::HitCheck2(paperKing.cb_, junkKing.cb_, junkKing.width, junkKing.height)) {
		se::DSound_Play("bump");
		if (!junkKing.attacking) {
			junkKing.attacking = true;
			paperKing.hp -= junkKing.attackDamage * paperKing.receivedDamage * junkKing.provideDamage;
			if (paperKing.hp < 0) {
				paperKing.hp = 0;
			}
			junkKing.attacked = true;
		}
	}
	else {
		junkKing.attacking = false;
	}

	//��Q���ɓ���������A���̍s���𕪐͂��A���܂����s�������
	for (std::vector<ObstacleInfo>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
		if (charabase::CheckUseChar(it->cb_)) {
			if (charabase::HitCheck2(it->cb_, junkKing.cb_, junkKing.width, junkKing.height)) {
				if (charabase::CheckUseChar(junkKing.cb_)) {
					it->hp -= junkKing.attackDamage * 0.2f * junkKing.provideDamage;

					decisionTime = 0;

					int lottery;

					switch (junkKing.decision)
					{
					case 2:
						lottery = math::GetRandom(0, 4);
						if (lottery < 2) {
							junkKing.decision = math::GetRandom(3, 5);
						}
						else {
							junkKing.decision = math::GetRandom(8, 9);
						}
						break;
					case 3:
						lottery = math::GetRandom(0, 4);
						if (lottery == 0) {
							junkKing.decision = 2;
						}
						else {
							junkKing.decision = math::GetRandom(4, 7);
						}
						break;
					case 4:
						lottery = math::GetRandom(0, 4);
						if (lottery <= 1) {
							junkKing.decision = math::GetRandom(2, 3);
						}
						else if(lottery >= 2 && lottery <= 3) {
							junkKing.decision = math::GetRandom(5, 6);
						}
						else {
							junkKing.decision = 8;
						}
						break;
					case 5:
						lottery = math::GetRandom(0, 4);
						if (lottery <= 2) {
							junkKing.decision = math::GetRandom(2, 4);
						}
						else if (lottery == 3) {
							junkKing.decision = 7;
						}
						else {
							junkKing.decision = 9;
						}
						break;
					case 6:
						lottery = math::GetRandom(0, 4);
						if (lottery < 3) {
							junkKing.decision = math::GetRandom(3, 5);
						}
						else {
							junkKing.decision = math::GetRandom(8, 9);
						}
						break;
					case 7:
						lottery = math::GetRandom(0, 4);
						if (lottery < 3) {
							junkKing.decision = math::GetRandom(3, 5);
						}
						else {
							junkKing.decision = math::GetRandom(8, 9);
						}
						break;
					case 8:
						lottery = math::GetRandom(0, 4);
						if (lottery == 0) {
							junkKing.decision = 2;
						}
						else {
							junkKing.decision = math::GetRandom(4, 7);
						}
						break;
					case 9:
						lottery = math::GetRandom(0, 4);
						if (lottery == 0) {
							junkKing.decision = 2;
						}
						else {
							junkKing.decision = math::GetRandom(4, 7);
						}
						break;
					}

					//��Q�����󂳂ꂽ�̃y�i���e�B�[���ς��
					if (it->hp <= 0) {
						charabase::NoUseChar(it->cb_);

						if (currentStage != STG_TUTORIAL) {
							switch (obstacle.type)
							{
							case FOUNTAIN:
								requiredMoney += 4000;
								break;
							case STATUE:
								requiredMoney += 2000;
								break;
							case HOUSE:
								requiredMoney += 10000;
								break;
							case STREETLIGHT:
								requiredMoney += 3000;
								break;
							case TREE:
								requiredMoney += 7000;
								break;
							case ROCK:
								requiredMoney += 1000;
								break;
							}
						}
					}
				}
			}
		}
	}
}

//�S�~�̉��̕\���֐�
void DispJunkKing() {
	//����Ԃ��`�F�b�N����F���̎��L�����N�^�[��\�����A�s���̎��L�����N�^�[��\�����Ȃ�
	if (charabase::CheckUseChar(junkKing.cb_)) {
		charabase::Draw_Char(junkKing.cb_);
	}
}

//�S�~�̉���HP�l�̏������֐�
void InitJunkKingHPBar() {
	charabase::InitCharBase(
		junkKing.emptyHPBar.cb_,															//�������������\����
		junkKing.cb_.pos.x, junkKing.cb_.pos.y - junkKing.cb_.height / 2.f - 5.f, 0.1f,		//�������W
		0.f, 0.f, 0.f,																		//�ړ���
		"emptyBar",																			//�g�p�摜��
		0, 0, 0,																			//�Q�Ɛ�
		40, 8																				//�摜�̃T�C�Y(���A����)
	);

	charabase::InitCharBase(
		junkKing.fullHPBar.cb_,																//�������������\����
		junkKing.cb_.pos.x, junkKing.cb_.pos.y - junkKing.cb_.height / 2.f - 5.f, 0.1f,		//�������W
		0.f, 0.f, 0.f,																		//�ړ���
		"fullHPBar",																		//�g�p�摜��
		0, 0, 0,																			//�Q�Ɛ�
		40, 8																				//�摜�̃T�C�Y(���A����)
	);

	charabase::UseChar(junkKing.fullHPBar.cb_);												//����Ԃ�ݒ肷��
	charabase::UseChar(junkKing.emptyHPBar.cb_);											//����Ԃ�ݒ肷��
}

//�S�~�̉���HP�l�̕ω��֐�
void MoveJunkKingHPBar() {
	float percentHP = junkKing.hp / junkKing.maxHP;
	junkKing.fullHPBar.cb_.scale.x = percentHP;

	junkKing.emptyHPBar.cb_.pos.x = junkKing.cb_.pos.x;
	junkKing.emptyHPBar.cb_.pos.y = junkKing.cb_.pos.y - junkKing.cb_.height / 2 - 5;
	junkKing.fullHPBar.cb_.pos.x = junkKing.cb_.pos.x - ((junkKing.maxHP - junkKing.maxHP * percentHP) / 100);
	junkKing.fullHPBar.cb_.pos.y = junkKing.cb_.pos.y - junkKing.cb_.height / 2 - 5;
}

//�S�~�̉���HP�l�̕\���֐�
void DispJunkKingHPBar() {
	if (charabase::CheckUseChar(junkKing.emptyHPBar.cb_)) {
		charabase::Draw_Char(junkKing.emptyHPBar.cb_);
	}

	if (charabase::CheckUseChar(junkKing.fullHPBar.cb_)) {
		charabase::Draw_Char(junkKing.fullHPBar.cb_);
	}
}

//��Q���̏������֐�
void InitObstacles() {
	if (!obstacles.empty()) {
		obstacles.clear();
	}
	int obstacleNo = (int)math::GetRandom(4.f, 8.f);
	float xPos;
	float yPos;
	std::vector<float> usedXPos;
	std::vector<float> usedYPos;

	//�X�e�[�W���ƂɈႤ��Q�����o������
	for (int i = 0; i < obstacleNo; i++) {
		int randomObstacleType = (int)math::GetRandom(0.f, 2.f);
		std::string obstacleType = "";

		switch (currentStage) {
		case STG_TUTORIAL:
		case STG_1:
		case STG_2:
			switch (randomObstacleType)
			{
			case 0:
				obstacleType = "fountain";
				obstacle.type = FOUNTAIN;
				obstacle.cb_.width = 60;
				obstacle.cb_.height = 60;
				obstacle.maxHP = 200;
				obstacle.hp = 200;
				break;
			case 1:
				obstacleType = "statue";
				obstacle.type = STATUE;
				obstacle.cb_.width = 20;
				obstacle.cb_.height = 65;
				obstacle.maxHP = 100;
				obstacle.hp = 100;
				break;
			}
			break;
		case STG_3:
			switch (randomObstacleType)
			{
			case 0:
				obstacleType = "house";
				obstacle.type = HOUSE;
				obstacle.cb_.width = 80;
				obstacle.cb_.height = 90;
				obstacle.maxHP = 400;
				obstacle.hp = 400;
				break;
			case 1:
				obstacleType = "streetLight";
				obstacle.type = STREETLIGHT;
				obstacle.cb_.width = 31;
				obstacle.cb_.height = 70;
				obstacle.maxHP = 200;
				obstacle.hp = 200;
				break;
			}
			break;
		case STG_4:
			obstacleType = "tree";
			obstacle.type = TREE;
			obstacle.cb_.width = 79;
			obstacle.cb_.height = 70;
			obstacle.maxHP = 200;
			obstacle.hp = 200;
			break;
		case STG_5:
			obstacleType = "rock";
			obstacle.type = ROCK;
			obstacle.cb_.width = 60;
			obstacle.cb_.height = 62;
			obstacle.maxHP = 200;
			obstacle.hp = 200;
			break;
		}

		//�V�����Q�[�����n�߂���A��Q���̈ʒu���ς��
		if (!usedXPos.empty() && !usedYPos.empty()) {
			xPos = math::GetRandom(obstacle.cb_.width / 2.f, (float)GAMEFIELDW - (obstacle.cb_.width / 2));
			for (std::vector<float>::iterator it = usedXPos.begin(); it != usedXPos.end(); it++) {
				while (std::abs(xPos - *it) <= (obstacle.cb_.width + 40)) {
					xPos = math::GetRandom(obstacle.cb_.width / 2.f, (float)GAMEFIELDW - (obstacle.cb_.width / 2));
				}
			}
			yPos = math::GetRandom(obstacle.cb_.height / 2.f, (float)GAMEFIELDH - (obstacle.cb_.height / 2));
			for (std::vector<float>::iterator it = usedYPos.begin(); it != usedYPos.end(); it++) {
				while (std::abs(yPos - *it) <= (obstacle.cb_.height + 40)) {
					if (currentStage == STG_4) {
						yPos = math::GetRandom(obstacle.cb_.height / 2.f, (float)GAMEFIELDH - obstacle.cb_.height);
					}
					else {
						yPos = math::GetRandom(obstacle.cb_.height / 2.f, (float)GAMEFIELDH - (obstacle.cb_.height / 2));
					}
				}
			}
		}
		else {
			xPos = math::GetRandom(obstacle.cb_.width / 2.f, (float)GAMEFIELDW - (obstacle.cb_.width / 2));
			if (currentStage == STG_4) {
				yPos = math::GetRandom(obstacle.cb_.height / 2.f, (float)GAMEFIELDH - obstacle.cb_.height);
			}
			else {
				yPos = math::GetRandom(obstacle.cb_.height / 2.f, (float)GAMEFIELDH - (obstacle.cb_.height / 2));
			}
		}

		usedXPos.push_back(xPos);
		usedYPos.push_back(yPos);

		charabase::InitCharBase(
			obstacle.cb_,											//�������������\����
			xPos, yPos, 0.1f,										//�������W
			0.f, 0.f, 0.f,											//�ړ���
			obstacleType,											//�g�p�摜��
			0, 0, 0,												//�Q�Ɛ�
			(int)obstacle.cb_.width, (int)obstacle.cb_.height		//�摜�̃T�C�Y(���A����)
		);

		obstacles.push_back(obstacle);
	}

	for (std::vector<ObstacleInfo>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
		charabase::UseChar(it->cb_);								//����Ԃ�ݒ肷��
	}
}

//��Q���̕\���֐�
void DispObstacles() {
	for (std::vector<ObstacleInfo>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
		if (charabase::CheckUseChar(it->cb_)) {
			charabase::Draw_Char(it->cb_);
		}
	}
}

//��Q����HP�l�̏������֐�
void InitObstacleHPBar() {
	for (std::vector<ObstacleInfo>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
		charabase::InitCharBase(
			it->emptyHPBar.cb_,														//�������������\����
			it->cb_.pos.x, it->cb_.pos.y - it->cb_.height / 2.f - 5.f, 0.1f,		//�������W
			0.f, 0.f, 0.f,															//�ړ���
			"emptyBar",																//�g�p�摜��
			0, 0, 0,																//�Q�Ɛ�
			40, 8																	//�摜�̃T�C�Y(���A����)
		);

		charabase::InitCharBase(
			it->fullHPBar.cb_,														//�������������\����
			it->cb_.pos.x, it->cb_.pos.y - it->cb_.height / 2.f - 5.f, 0.1f,		//�������W
			0.f, 0.f, 0.f,															//�ړ���
			"fullHPBar",															//�g�p�摜��
			0, 0, 0,																//�Q�Ɛ�
			40, 8																	//�摜�̃T�C�Y(���A����)
		);

		charabase::UseChar(it->fullHPBar.cb_);										//����Ԃ�ݒ肷��
		charabase::UseChar(it->emptyHPBar.cb_);										//����Ԃ�ݒ肷��
	}
}

//��Q����HP�l�̕ω��֐�
void MoveObstacleHPBar() {
	for (std::vector<ObstacleInfo>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
		float percentHP = it->hp / it->maxHP;
		it->fullHPBar.cb_.scale.x = percentHP;

		it->emptyHPBar.cb_.pos.x = it->cb_.pos.x;
		it->emptyHPBar.cb_.pos.y = it->cb_.pos.y - it->cb_.height / 2 - 5;
		if (it->maxHP == 100) {
			it->fullHPBar.cb_.pos.x = it->cb_.pos.x - ((it->maxHP - it->maxHP * percentHP) / 5);
		}
		else if (it->maxHP == 200) {
			it->fullHPBar.cb_.pos.x = it->cb_.pos.x - ((it->maxHP - it->maxHP * percentHP) / 10);
		}
		else if (it->maxHP == 400) {
			it->fullHPBar.cb_.pos.x = it->cb_.pos.x - ((it->maxHP - it->maxHP * percentHP) / 20);
		}
		it->fullHPBar.cb_.pos.y = it->cb_.pos.y - it->cb_.height / 2 - 5;
	}
}

//�S�~��HP�l�̕\���֐�
void DispObstacleHPBar() {
	for (std::vector<ObstacleInfo>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
		if (charabase::CheckUseChar(it->cb_) && charabase::CheckUseChar(it->emptyHPBar.cb_)) {
			charabase::Draw_Char(it->emptyHPBar.cb_);
		}

		if (charabase::CheckUseChar(it->cb_) && charabase::CheckUseChar(it->fullHPBar.cb_)) {
			charabase::Draw_Char(it->fullHPBar.cb_);
		}
	}
}

//�X�e�[�^�X����\������
void DispStatus() {
	if (currentStage == STG_4 || currentStage == STG_5 || stageCleared[3]) {
		graph::Draw_GraphicsNC((system::WINW / 2.f), (system::WINH / 2.f) + 260.f, 0.1f, "scrollWApple", 0, 0, 693, 120);
	}
	else {
		graph::Draw_GraphicsNC((system::WINW / 2.f), (system::WINH / 2.f) + 260.f, 0.1f, "scroll", 0, 0, 693, 120);
	}
	graph::Draw_GraphicsNC((system::WINW / 2.f) - 200.f, (system::WINH / 2.f) + 250.f, 0.1f, "head", 0, 0, 32, 32, 0, nullptr, 0.5f, 0.5f);
	std::string paperKingLife(std::to_string(life));
	font::Draw_FontTextNC(170, system::WINH - 80, 0.1f, paperKingLife, ARGB(255, 0, 0, 0), 1);
	graph::Draw_GraphicsNC((system::WINW / 2.f) - 200.f, (system::WINH / 2.f) + 270.f, 0.1f, "coin", 0, 0, 32, 32, 0, nullptr, 0.5f, 0.5f);
	if (currentStage == STG_WORLDMAP) {
		std::string coinStatus("�̍��v:" + std::to_string(collectedMoney));
		font::Draw_FontTextNC(170, system::WINH - 60, 0.1f, coinStatus, ARGB(255, 0, 0, 0), 1);
	}
	else {
		std::string coinStatus(std::to_string(money) + " / " + std::to_string(requiredMoney));
		font::Draw_FontTextNC(170, system::WINH - 60, 0.1f, coinStatus, ARGB(255, 0, 0, 0), 1);
	}

	if (paperKing.hasKey) {
		graph::Draw_GraphicsNC((system::WINW / 2.f) - 65.f, (system::WINH / 2.f) + 282.f, 0.1f, "hasKey", 0, 0, 40, 40, 0, nullptr, 0.9f, 0.9f);
	}

	if (checkGotTreasure[0]) {
		graph::Draw_GraphicsNC((system::WINW / 2.f) - 17.f, (system::WINH / 2.f) + 270.f, 0.1f, "hasPlackart", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
		graph::Draw_GraphicsNC((system::WINW / 2.f) - 17.f, (system::WINH / 2.f) + 270.f, 0.1f, "plackartEffect", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
	}
	else {
		graph::Draw_GraphicsNC((system::WINW / 2.f) - 17.f, (system::WINH / 2.f) + 270.f, 0.1f, "questionMark", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
	}
	if (checkGotTreasure[1]) {
		graph::Draw_GraphicsNC((system::WINW / 2.f) + 42.f, (system::WINH / 2.f) + 270.f, 0.1f, "hasGreave", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
		graph::Draw_GraphicsNC((system::WINW / 2.f) + 42.f, (system::WINH / 2.f) + 270.f, 0.1f, "greaveEffect", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
	}
	else {
		graph::Draw_GraphicsNC((system::WINW / 2.f) + 42.f, (system::WINH / 2.f) + 270.f, 0.1f, "questionMark", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
	}
	if (checkGotTreasure[2]) {
		graph::Draw_GraphicsNC((system::WINW / 2.f) + 101.f, (system::WINH / 2.f) + 270.f, 0.1f, "hasGauntlet", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
		graph::Draw_GraphicsNC((system::WINW / 2.f) + 101.f, (system::WINH / 2.f) + 270.f, 0.1f, "gauntletEffect", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
	}
	else {
		graph::Draw_GraphicsNC((system::WINW / 2.f) + 101.f, (system::WINH / 2.f) + 270.f, 0.1f, "questionMark", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
	}
	if (checkGotTreasure[3]) {
		graph::Draw_GraphicsNC((system::WINW / 2.f) + 159.f, (system::WINH / 2.f) + 270.f, 0.1f, "hasExcalibur", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
		graph::Draw_GraphicsNC((system::WINW / 2.f) + 159.f, (system::WINH / 2.f) + 270.f, 0.1f, "excaliburEffect", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
	}
	else {
		graph::Draw_GraphicsNC((system::WINW / 2.f) + 159.f, (system::WINH / 2.f) + 270.f, 0.1f, "questionMark", 0, 0, 60, 60, 0, nullptr, 0.9f, 0.9f);
	}

	std::string arrowCount(std::to_string(arrowCount));
	if (checkGotTreasure[3]) {
		switch (attackType)
		{
		case SHORTRANGE:
			graph::Draw_GraphicsNC((system::WINW / 2.f) + 240.f, (system::WINH / 2.f) + 262.f, 0.1f, "usingSword", 0, 0, 75, 75, 0, nullptr, 0.9f, 0.9f);
			break;
		case LONGRANGE:
			graph::Draw_GraphicsNC((system::WINW / 2.f) + 240.f, (system::WINH / 2.f) + 262.f, 0.1f, "usingBow", 0, 0, 75, 75, 0, nullptr, 0.9f, 0.9f);
			font::Draw_FontTextNC(570, system::WINH - 40, 0.1f, arrowCount, ARGB(255, 0, 0, 0), 1);
			break;
		case EXCALIBUR:
			graph::Draw_GraphicsNC((system::WINW / 2.f) + 240.f, (system::WINH / 2.f) + 262.f, 0.1f, "usingExcalibur", 0, 0, 75, 75, 0, nullptr, 0.9f, 0.9f);
			graph::Draw_GraphicsNC((system::WINW / 2.f) + 240.f, (system::WINH / 2.f) + 290.f, 0.1f, "emptyBar", 0, 0, 40, 8, 0, nullptr, 1.f, 1.f);
			if (currentStage != STG_WORLDMAP) {
				charabase::Draw_Char(paperKing.fullPowerBar.cb_);
			}
			break;
		}
	}
	else {
		switch (attackType)
		{
		case SHORTRANGE:
				graph::Draw_GraphicsNC((system::WINW / 2.f) + 240.f, (system::WINH / 2.f) + 262.f, 0.1f, "usingSword", 0, 0, 75, 75, 0, nullptr, 0.9f, 0.9f);
			break;
		case LONGRANGE:
			graph::Draw_GraphicsNC((system::WINW / 2.f) + 240.f, (system::WINH / 2.f) + 262.f, 0.1f, "usingBow", 0, 0, 75, 75, 0, nullptr, 0.9f, 0.9f);
			font::Draw_FontTextNC(570, system::WINH - 40, 0.1f, arrowCount, ARGB(255, 0, 0, 0), 1);
			break;
		}
	}

	if (appleCount > 0) {
		std::string appleCount(std::to_string(appleCount));
		graph::Draw_GraphicsNC((system::WINW / 2.f) + 300.f, (system::WINH / 2.f) + 282.f, 0.1f, "apple", 0, 0, 32, 32, 0, nullptr, 1.f, 1.f);
		font::Draw_FontTextNC(670, system::WINH - 40, 0.1f, appleCount, ARGB(255, 255, 255, 255), 1);
	}
}

//�X�R�A��\������
void DispScore() {
	std::string score("Score: " + std::to_string(score));
	font::Draw_FontTextNC(35, system::WINH - 80, 0.1f, score, ARGB(255, 0, 0, 0), 1);
}

//�n�C�X�R�A��\������
void DispHighScore() {
	std::string hiscore("High Score: " + std::to_string(highScore));
	font::Draw_FontTextNC(35, system::WINH - 60, 0.1f, hiscore, ARGB(255, 0, 0, 0), 1);
}

//�X�L�b�v�{�^���̏������֐�
void InitSkipButton() {
	charabase::InitCharBase(
		skipButton.cb_,		//�������������\����
		650.f, 30.f, 0.f,	//�������W
		0.f, 0.f, 0.f,		//�ړ���
		"skipButton",		//�g�p�摜��
		0, 0, 0,			//�Q�Ɛ�
		100, 44				//�摜�̃T�C�Y(���A����)
	);
}

//�Z�[�u�E���[�h�m�F��ʂ̏������֐�
void InitSaveLoadConfirmationMsg() {
	charabase::InitCharBase(
		saveLoadConfirmationMsg[0].cb_,						//�������������\����
		(system::WINW / 2.f), (system::WINH / 2.f), 0.f,	//�������W
		0.f, 0.f, 0.f,										//�ړ���
		"saveConfirmationMsg",								//�g�p�摜��
		0, 0, 0,											//�Q�Ɛ�
		557, 228											//�摜�̃T�C�Y(���A����)
	);

	charabase::InitCharBase(
		saveLoadConfirmationMsg[1].cb_,						//�������������\����
		(system::WINW / 2.f), (system::WINH / 2.f), 0.f,	//�������W
		0.f, 0.f, 0.f,										//�ړ���
		"loadConfirmationMsg",								//�g�p�摜��
		0, 0, 0,											//�Q�Ɛ�
		557, 228											//�摜�̃T�C�Y(���A����)
	);

	charabase::InitCharBase(
		saveLoadConfirmationMsg[2].cb_,						//�������������\����
		(system::WINW / 2.f), (system::WINH / 2.f), 0.f,	//�������W
		0.f, 0.f, 0.f,										//�ړ���
		"noSaveDataMsg",									//�g�p�摜��
		0, 0, 0,											//�Q�Ɛ�
		557, 228											//�摜�̃T�C�Y(���A����)
	);

	charabase::InitCharBase(
		saveLoadConfirmationMsg[3].cb_,						//�������������\����
		(system::WINW / 2.f), (system::WINH / 2.f), 0.f,	//�������W
		0.f, 0.f, 0.f,										//�ړ���
		"doneSavingMsg",									//�g�p�摜��
		0, 0, 0,											//�Q�Ɛ�
		557, 228											//�摜�̃T�C�Y(���A����)
	);

	charabase::InitCharBase(
		saveLoadConfirmationMsg[4].cb_,						//�������������\����
		(system::WINW / 2.f), (system::WINH / 2.f), 0.f,	//�������W
		0.f, 0.f, 0.f,										//�ړ���
		"doneLoadingMsg",									//�g�p�摜��
		0, 0, 0,											//�Q�Ɛ�
		557, 228											//�摜�̃T�C�Y(���A����)
	);
}

//�Z�[�u�E���[�h�{�^���̏������֐�
void InitSaveLoadButton() {
	charabase::InitCharBase(
		saveLoadButton[0].cb_,	//�������������\����
		650.f, 30.f, 0.f,		//�������W
		0.f, 0.f, 0.f,			//�ړ���
		"saveButton",			//�g�p�摜��
		0, 0, 0,				//�Q�Ɛ�
		91, 36					//�摜�̃T�C�Y(���A����)
	);

	charabase::InitCharBase(
		saveLoadButton[1].cb_,	//�������������\����
		650.f, 75.f, 0.f,		//�������W
		0.f, 0.f, 0.f,			//�ړ���
		"loadButton",			//�g�p�摜��
		0, 0, 0,				//�Q�Ɛ�
		91, 36					//�摜�̃T�C�Y(���A����)
	);
}

//�͂��E�������{�^���̏������֐�
void InitYesNoButton() {
	charabase::InitCharBase(
		yesNoButton[0].cb_,														//�������������\����
		((system::WINW / 2.f) - 110.f), ((system::WINH / 2.f) + 45.f), 0.f,		//�������W
		0.f, 0.f, 0.f,															//�ړ���
		"yesButton",															//�g�p�摜��
		0, 0, 0,																//�Q�Ɛ�
		143, 41																	//�摜�̃T�C�Y(���A����)
	);

	charabase::InitCharBase(
		yesNoButton[1].cb_,														//�������������\����
		((system::WINW / 2.f) + 110.f), ((system::WINH / 2.f) + 45.f), 0.f,		//�������W
		0.f, 0.f, 0.f,															//�ړ���
		"noButton",																//�g�p�摜��
		0, 0, 0,																//�Q�Ɛ�
		143, 41																	//�摜�̃T�C�Y(���A����)
	);
}

//�I���̏������֐�
void InitOptions() {
	charabase::InitCharBase(
		options[0].cb_,			//�������������\����
		3000.f, 780.f, 0.f,		//�������W
		0.f, 0.f, 0.f,			//�ړ���
		"noTutorial",			//�g�p�摜��
		0, 0, 0,				//�Q�Ɛ�
		253, 230				//�摜�̃T�C�Y(���A����)
	);
	charabase::InitCharBase(
		options[1].cb_,			//�������������\����
		3100.f, 1120.f, 0.f,	//�������W
		0.f, 0.f, 0.f,			//�ړ���
		"yesTutorial",			//�g�p�摜��
		0, 0, 0,				//�Q�Ɛ�
		441, 325				//�摜�̃T�C�Y(���A����)
	);
	charabase::InitCharBase(
		options[2].cb_,			//�������������\����
		3790.f, 1010.f, 0.f,	//�������W
		0.f, 0.f, 0.f,			//�ړ���
		"violentEnd",			//�g�p�摜��
		0, 0, 0,				//�Q�Ɛ�
		254, 470				//�摜�̃T�C�Y(���A����)
	);
	charabase::InitCharBase(
		options[3].cb_,			//�������������\����
		4130.f, 1010.f, 0.f,	//�������W
		0.f, 0.f, 0.f,			//�ړ���
		"kindEnd",				//�g�p�摜��
		0, 0, 0,				//�Q�Ɛ�
		292, 482				//�摜�̃T�C�Y(���A����)
	);
}

//======================================================================================//
//�f�t�H���g�֐���`
//======================================================================================//

//---------------------------------
// Title
//---------------------------------
//����������
void TitleInit() {
	currentStage = STG_TITLE;
	if (clearedTutorial) {
		introNumber = 7;
		scrollX = 3960.f;
		scrollY = 960.f;
		mouseX = 4280.f;
		mouseY = 1220.f;
		camera::SetLookAt(scrollX, scrollY);
	}
	else {
		introNumber = 1;
		scrollX = 364.f;
		scrollY = 324.f;
		mouseX = 680.f;
		mouseY = 580.f;
		camera::SetLookAt(360.f, 320.f);
	}
	
	scrollSpeed = 30;
	timer = 0;
	titleTimer = 150;
	textNumber = 0;
	score = 0;
	originalScore = score;
	for (bool& dialogShown : dialogsShown) {
		dialogShown = false;
	}

	pt = input::GetMousePosition();
	selectBuffer = false;
	selectEnding = false;

	InitSkipButton();
	InitSaveLoadConfirmationMsg();
	InitSaveLoadButton();
	InitYesNoButton();
	InitOptions();

	checkLookedStatus = false;
	firstTimeToMap = true;
	showSaveTip = false;

	for (int i = 0; i < 4; ++i) {
		stageCleared[i] = false;
	}

	for (int i = 0; i < 5; ++i) {
		checkGotTreasure[i] = false;
	}

	arrowCount = 20;
	appleCount = 0;
}

//�t���[������
void TitleStep() {
	esing::Step();
	pt = input::GetMousePosition();

	if (timer >= 120) {
		timer = 0;
	}
	else {
		timer++;
	}

	if (titleTimer > 0 && introNumber == 8) {
		titleTimer--;
	}
	
	//�v���C���[���}�E�X�̍��{�^�����N���b�N����ƁA���̃_�C�A���O���ʂ�\������
	if (input::CheckPush(input::KEY_MOUSE_LBTN) && !charabase::HitCheckForMouse(pt, saveLoadButton[1].cb_) &&
		!charabase::CheckUseChar(saveLoadConfirmationMsg[1].cb_) && !charabase::CheckUseChar(saveLoadConfirmationMsg[2].cb_) &&
		!charabase::CheckUseChar(saveLoadConfirmationMsg[4].cb_)) {
		switch (introNumber) {
			case 1:
			case 2:			
				scrollX += 720;
				mouseX += 720;
				camera::SetCameraTarget(scrollX, scrollY, scrollSpeed);
				introNumber++;
				break;
			case 3:
				textNumber++;
				if (textNumber == 1) {
					dialogsShown[0] = true;
				}
				if (textNumber == 2) {
					dialogsShown[1] = true;
				}
				if (textNumber > 2) {				
					scrollX += 720;
					mouseX += 720;
					camera::SetCameraTarget(scrollX, scrollY, scrollSpeed);
					introNumber++;
					textNumber = 0;
				}
				break;
			case 4:
				textNumber++;
				if (textNumber == 1) {
					dialogsShown[2] = true;
				}
				if (textNumber == 2) {
					dialogsShown[3] = true;
				}
				if (textNumber > 2) {				
					scrollY += 640;
					mouseY += 640;
					camera::SetCameraTarget(scrollX, scrollY, scrollSpeed);
					introNumber++;
					textNumber = 0;
				}
				break;
			case 5:
				textNumber++;
				if (textNumber == 1) {
					dialogsShown[4] = true;
				}
				if (textNumber > 1) {				
					scrollX += 720;
					mouseX += 720;
					camera::SetCameraTarget(scrollX, scrollY, scrollSpeed);
					introNumber++;
					textNumber = 0;
				}
				break;
			case 6:
				textNumber++;
				if (textNumber == 1) {
					dialogsShown[5] = true;
				}
				if (textNumber >= 2) {
					dialogsShown[6] = true;
					dialogsShown[7] = true;
				}
				break;
			case 7:
				textNumber++;
				break;
			case 8:
				if (titleTimer == 0) {
					tipTimer = 0;
					stage::ChangeStage(STG_WORLDMAP);
				}
				break;
		}
	}
}

//�`�揈��
void TitleDisp() {
	if (dialogsShown[0]) {
		graph::Draw_Graphics(1810, 355, 0.f, "informKing1", 0, 0, 253, 230);
	}
	if (dialogsShown[1]) {
		graph::Draw_Graphics(1600, 290, 0.f, "what", 0, 0, 253, 230);
	}
	if (dialogsShown[2]) {
		graph::Draw_Graphics(2400, 220, 0.f, "looksBad", 0, 0, 175, 159);
	}
	if (dialogsShown[3]) {
		graph::Draw_Graphics(2570, 355, 0.f, "informKing2", 0, 0, 253, 230);
	}
	if (dialogsShown[4]) {
		graph::Draw_Graphics(2570, 730, 0.f, "kingDeclaration", 0, 0, 253, 214);
	}
	if (dialogsShown[5]) {
		graph::Draw_Graphics(3265, 780, 0.f, "tutorialSuggestion", 0, 0, 370, 336);
	}
	if (dialogsShown[6] && dialogsShown[7]) {
		pt.x += 2880;
		pt.y += 640;
		if (charabase::HitCheckForMouse(pt, options[0].cb_) && selectBuffer) {
			graph::Draw_Graphics(3000, 780, 0.f, "noTutorial", 0, 0, 253, 230);
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				scrollX += 720;
				mouseX += 720;
				camera::SetCameraTarget(scrollX, scrollY, scrollSpeed);
				selectBuffer = false;
				introNumber++;
				textNumber = 0;
			}
		}
		else {
			graph::Draw_Graphics(3000, 780, 0.f, "noTutorial", 0, 0, 253, 230, 0, 1.f, 1.f, 255, 230, 230, 230);
		}

		if (charabase::HitCheckForMouse(pt, options[1].cb_) && selectBuffer) {
			graph::Draw_Graphics(3100, 1120, 0.f, "yesTutorial", 0, 0, 441, 325);
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				stage::ChangeStage(STG_TUTORIAL);
			}
		}
		else {
			graph::Draw_Graphics(3100, 1120, 0.f, "yesTutorial", 0, 0, 441, 325, 0, 1.f, 1.f, 255, 230, 230, 230);
		}

		if (introNumber == 6) {
			selectBuffer = true;
		}
	}

	if (introNumber == 7) {
		pt = input::GetMousePosition();
		pt.x += 3600;
		pt.y += 640;

		if (charabase::HitCheckForMouse(pt, options[2].cb_) && selectEnding) {
			graph::Draw_Graphics(3790, 1040, 0.f, "violentEnd", 0, 0, 254, 470);
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				choice = VIOLENTEND;
				life += 3;
				scrollY -= 648;
				mouseY -= 648;
				camera::SetCameraTarget(scrollX, scrollY, scrollSpeed);
				introNumber++;
				textNumber = 0;
			}
		}
		else {
			graph::Draw_Graphics(3790, 1040, 0.f, "violentEnd", 0, 0, 254, 470, 0, 1.f, 1.f, 255, 230, 230, 230);
		}

		if (charabase::HitCheckForMouse(pt, options[3].cb_) && selectEnding) {
			graph::Draw_Graphics(4130, 1040, 0.f, "kindEnd", 0, 0, 292, 482);
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				choice = KINDEND;
				life += 2;
				scrollY -= 648;
				mouseY -= 648;
				camera::SetCameraTarget(scrollX, scrollY, scrollSpeed);
				introNumber++;
				textNumber = 0;
			}
		}
		else {
			graph::Draw_Graphics(4130, 1040, 0.f, "kindEnd", 0, 0, 292, 482, 0, 1.f, 1.f, 255, 230, 230, 230);
		}

		selectEnding = true;
	}
	
	graph::Draw_GraphicsLeftTop(0.f, 0.f, 1.f, "title", 0, 0, 4320, 1920);

	if (introNumber <= 4) {
		if (charabase::HitCheckForMouse(pt, skipButton.cb_)) {
			skipButton.cb_.resname = "skipButtonHover";
			charabase::Draw_CharNC(skipButton.cb_);
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				switch (introNumber)
				{
				case 2:
					scrollX += 2160;
					scrollY += 640;
					mouseX += 2160;
					mouseY += 640;
					break;
				case 3:
					scrollX += 1440;
					scrollY += 640;
					mouseX += 1440;
					mouseY += 640;
					break;
				case 4:
					scrollX += 720;
					scrollY += 640;
					mouseX += 720;
					mouseY += 640;
					break;
				}
				camera::SetCameraTarget(scrollX, scrollY, scrollSpeed);
				introNumber = 6;
				textNumber = 0;
			}
		}
		else {
			skipButton.cb_.resname = "skipButton";
			charabase::Draw_CharNC(skipButton.cb_);
		}
	}
	
	if (introNumber < 6) {
		if (charabase::HitCheckForMouse(pt, saveLoadButton[1].cb_)) {
			saveLoadButton[1].cb_.resname = "loadButtonHover";
			charabase::Draw_CharNC(saveLoadButton[1].cb_);
			if (input::CheckPull(input::KEY_MOUSE_LBTN)) {
				charabase::UseChar(saveLoadConfirmationMsg[1].cb_);
			}
		}
		else {
			saveLoadButton[1].cb_.resname = "loadButton";
			charabase::Draw_CharNC(saveLoadButton[1].cb_);
		}
	}

	if (charabase::CheckUseChar(saveLoadConfirmationMsg[1].cb_)) {
		charabase::Draw_CharNC(saveLoadConfirmationMsg[1].cb_);
		charabase::Draw_CharNC(yesNoButton[0].cb_);
		charabase::Draw_CharNC(yesNoButton[1].cb_);

		if (charabase::HitCheckForMouse(pt, yesNoButton[0].cb_)) {
			yesNoButton[0].cb_.resname = "yesButtonHover";
			if (input::CheckPull(input::KEY_MOUSE_LBTN)) {
				LoadGameData();

				charabase::NoUseChar(saveLoadConfirmationMsg[1].cb_);

				if (!charabase::CheckUseChar(saveLoadConfirmationMsg[2].cb_)) {
					charabase::UseChar(saveLoadConfirmationMsg[4].cb_);
				}
			}
		}
		else {
			yesNoButton[0].cb_.resname = "yesButton";
		}

		if (charabase::HitCheckForMouse(pt, yesNoButton[1].cb_)) {
			yesNoButton[1].cb_.resname = "noButtonHover";
			if (input::CheckPull(input::KEY_MOUSE_LBTN)) {
				charabase::NoUseChar(saveLoadConfirmationMsg[1].cb_);
			}
		}
		else {
			yesNoButton[1].cb_.resname = "noButton";
		}
	}

	if (charabase::CheckUseChar(saveLoadConfirmationMsg[2].cb_)) {
		charabase::Draw_CharNC(saveLoadConfirmationMsg[2].cb_);
		yesNoButton[0].cb_.pos.x = (system::WINW / 2.f);
		charabase::Draw_CharNC(yesNoButton[0].cb_);

		if (charabase::HitCheckForMouse(pt, yesNoButton[0].cb_)) {
			yesNoButton[0].cb_.resname = "yesButtonHover";
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				yesNoButton[0].cb_.pos.x = ((system::WINW / 2.f) - 110.f);
				charabase::NoUseChar(saveLoadConfirmationMsg[1].cb_);
				charabase::NoUseChar(saveLoadConfirmationMsg[2].cb_);
			}
		}
		else {
			yesNoButton[0].cb_.resname = "yesButton";
		}
	}

	if (charabase::CheckUseChar(saveLoadConfirmationMsg[4].cb_)) {
		charabase::Draw_CharNC(saveLoadConfirmationMsg[4].cb_);
		yesNoButton[0].cb_.pos.x = (system::WINW / 2.f);
		charabase::Draw_CharNC(yesNoButton[0].cb_);

		if (charabase::HitCheckForMouse(pt, yesNoButton[0].cb_)) {
			yesNoButton[0].cb_.resname = "yesButtonHover";
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				yesNoButton[0].cb_.pos.x = ((system::WINW / 2.f) - 110.f);
				charabase::NoUseChar(saveLoadConfirmationMsg[4].cb_);
			}
		}
		else {
			yesNoButton[0].cb_.resname = "yesButton";
		}
	}

	if (introNumber == 8) {
		if (titleTimer == 0) {
			if (timer >= 60 && timer <= 120) {
				graph::Draw_GraphicsLeftTop(mouseX, mouseY, 1.f, "mouseClicked", 0, 0, 32, 53);
			}
			else {
				graph::Draw_GraphicsLeftTop(mouseX, mouseY, 1.f, "mouseUnclick", 0, 0, 32, 53);
			}
		}
	}
	else {
		if (timer >= 60 && timer <= 120) {
			graph::Draw_GraphicsLeftTop(mouseX, mouseY, 1.f, "mouseClicked", 0, 0, 32, 53);
		}
		else {
			graph::Draw_GraphicsLeftTop(mouseX, mouseY, 1.f, "mouseUnclick", 0, 0, 32, 53);
		}
	}
}

void Title()
{
	if (stage::InitStage()){ TitleInit(); }
	TitleStep();
	TitleDisp();
}

//---------------------------------
// Tutorial�@�@�`���[�g���A��
//---------------------------------
//����������
void TutorialInit() {
	currentStage = STG_TUTORIAL;
	score = 0;
	money = 0;
	requiredMoney = 90000;
	checkAlive = true;
	checkHit = false;
	checkCleared = false;
	checkPressDirectionKey = false;
	checkPressAttackKey = false;
	checkPressChangeWeaponKey = false;
	checkGotFirstCoin = false;
	createdKey = false;
	showTreasure = true;
	showTreasureTime = 25;
	poweredUp = false;
	moveTime = 0;
	treasureTipTimer = 0;
	tipTimer = 0;
	attackTimer = 0;

	InitPaperKing();
	InitPaperKingHPBar();
	InitAttack();
	InitJunk();
	InitJunkHPBar();
	InitObstacles();
	InitObstacleHPBar();
	InitCoin();
	InitKey();
	InitTreasureBox();
}

//�t���[������
void TutorialStep() {
	//�`���[�g���A���ő�����@��������
	if (input::CheckPush(input::KEY_UP) || input::CheckPush(input::KEY_DOWN) || input::CheckPush(input::KEY_LEFT) || input::CheckPush(input::KEY_RIGHT)) {
		checkPressDirectionKey = true;
	}

	if (input::CheckPush(input::KEY_BTN0)) {
		checkPressAttackKey = true;
	}

	if (input::CheckPush(input::KEY_BTN1)) {
		checkPressChangeWeaponKey = true;
	}

	if (tipTimer <= 2880) {
		if (tipTimer < 180) {
			tipTimer++;
		}
		else if (checkPressDirectionKey && tipTimer < 360) {
			tipTimer++;
		}
		else if (checkGotFirstCoin && tipTimer < 540) {
			tipTimer++;
		}
		else if (checkPressAttackKey && tipTimer < 720) {
			tipTimer++;
		}
		else if (checkPressChangeWeaponKey && tipTimer < 2520) {
			tipTimer++;
		}
		else if (checkGotTreasure[4] && treasureTipTimer == 181) {
			tipTimer++;
		}
	}

	if (checkGotTreasure[4] && treasureTipTimer <= 180) {
		treasureTipTimer++;
	}

	if (input::CheckPress(input::KEY_BTN0)) {
		attackTimer++;
	}
	else {
		attackTimer = 0;
	}

	if (checkGotTreasure[4] && tipTimer >= 2880) {
		clearedTutorial = true;
		checkCleared = true;
	}

	MovePaperKing();
	MovePaperKingHPBar();
	MoveAttack();
	MoveArrows();
	if (tipTimer >= 2180) {
		MoveJunk();
		MoveJunkHPBar();
	}
	MoveObstacleHPBar();
	MoveCoin();
	StageCleared();
}

//�`�揈��
void TutorialDisp() {
	graph::Draw_Graphics(480, 425, 1.f, "tutorialMap", 0, 0, 1680, 1490);

	if (checkGotTreasure[4] && treasureTipTimer <= 180) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTreasureTip", 0, 0, system::WINW, system::WINH);
	}

	if (tipTimer <= 180) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip1", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 180 && tipTimer <= 360) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip2", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 360 && tipTimer <= 540) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip3", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 540 && tipTimer <= 720) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip4", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 720 && tipTimer <= 900) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip5", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 900 && tipTimer <= 1080) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip6", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 1080 && tipTimer <= 1260) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip7", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 1260 && tipTimer <= 1440) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip8", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 1440 && tipTimer <= 1620) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip9", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 1620 && tipTimer <= 1800) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip10", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 1800 && tipTimer <= 1980) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip11", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 1980 && tipTimer <= 2160) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip12", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 2160 && tipTimer <= 2340) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip13", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 2340 && tipTimer < 2520) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip14", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 2520 && tipTimer <= 2700) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip15", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 2700 && tipTimer <= 2880) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "tutTip16", 0, 0, system::WINW, system::WINH);
	}
	
	DispPaperKingHPBar();
	DispPaperKing();
	if (input::CheckPress(input::KEY_BTN0) && checkAlive) {
		DispAttack();
	}
	DispArrows();
	DispJunkHPBar();
	DispJunk();
	DispObstacles();
	DispObstacleHPBar();
	DispCoin();
	DispKey();
	DispTreasureBox();
	DispStatus();
	DispScore();
	DispHighScore();
}

void Tutorial()
{
	if (stage::InitStage()){ TutorialInit(); }
	TutorialStep();
	TutorialDisp();
}

//---------------------------------
// Map		�Q�[�����E�n�}�i�X�e�[�W�̑I���j
// �}�E�X���g���āA�X�e�[�W��I�т܂�
//---------------------------------
//����������
void WorldMapInit() {
	currentStage = STG_WORLDMAP;
	camera::SetLookAt((system::WINW / 2.f), (system::WINH / 2.f));
	InitWorldMap();
	InitSaveLoadButton();

	pt = input::GetMousePosition();
	timer = 0;
	mouseX = 680.f;
	mouseY = 580.f;

	paperKing.hasKey = false;

	if (showSaveTip) {
		treasureTipTimer = 151;
	}
}

//�t���[������
void WorldMapStep() {
	pt = input::GetMousePosition();

	if (timer >= 120) {
		timer = 0;
	}
	else {
		timer++;
	}

	if (pt.x >= 15 && pt.x <= 705 && pt.y >= 525 && pt.y <= 635) {
		checkLookedStatus = true;
	}

	if (tipTimer <= 300) {
		tipTimer++;
	}

	if (checkLookedStatus && tipTimer <= 420) {
		tipTimer++;
	}


	if (stageCleared[0] && treasureTipTimer <= 150 && !showSaveTip) {
		treasureTipTimer++;
	}

	if (treasureTipTimer == 151) {
		showSaveTip = true;
	}

	if (!firstTimeToMap) {
		if (charabase::HitCheckForMouse(pt, worldMap[0].cb_) && !(pt.x >= 15 && pt.x <= 705 && pt.y >= 525 && pt.y <= 635) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[0].cb_) && !charabase::CheckUseChar(saveLoadConfirmationMsg[1].cb_) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[2].cb_) && !charabase::CheckUseChar(saveLoadConfirmationMsg[3].cb_) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[4].cb_)) {
			charabase::Draw_Char3(worldMap[0].cb_, 255.f, 255.f, 255.f, 255.f);
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				stage::ChangeStage(STG_1);
			}
		}
		else {
			charabase::Draw_Char3(worldMap[0].cb_, 255.f, 220.f, 220.f, 220.f);
		}

		if (charabase::HitCheckForMouse(pt, worldMap[1].cb_) && stageCleared[0] && !(pt.x >= 15 && pt.x <= 705 && pt.y >= 525 && pt.y <= 635) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[0].cb_) && !charabase::CheckUseChar(saveLoadConfirmationMsg[1].cb_) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[2].cb_) && !charabase::CheckUseChar(saveLoadConfirmationMsg[3].cb_) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[4].cb_)) {
			charabase::Draw_Char3(worldMap[1].cb_, 255.f, 255.f, 255.f, 255.f);
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				stage::ChangeStage(STG_2);
			}
		}
		else if (!stageCleared[0]) {
			charabase::Draw_Char3(worldMap[1].cb_, 255, 60, 60, 60);
		}
		else {
			charabase::Draw_Char3(worldMap[1].cb_, 255.f, 220.f, 220.f, 220.f);
		}

		if (charabase::HitCheckForMouse(pt, worldMap[2].cb_) && stageCleared[1] && !(pt.x >= 15 && pt.x <= 705 && pt.y >= 525 && pt.y <= 635) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[0].cb_) && !charabase::CheckUseChar(saveLoadConfirmationMsg[1].cb_) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[2].cb_) && !charabase::CheckUseChar(saveLoadConfirmationMsg[3].cb_) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[4].cb_)) {
			charabase::Draw_Char3(worldMap[2].cb_, 255.f, 255.f, 255.f, 255.f);
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				stage::ChangeStage(STG_3);
			}
		}
		else if (!stageCleared[1]) {
			charabase::Draw_Char3(worldMap[2].cb_, 255, 60, 60, 60);
		}
		else {
			charabase::Draw_Char3(worldMap[2].cb_, 255.f, 220.f, 220.f, 220.f);
		}

		if (charabase::HitCheckForMouse(pt, worldMap[3].cb_) && stageCleared[2] && !(pt.x >= 15 && pt.x <= 705 && pt.y >= 525 && pt.y <= 635) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[0].cb_) && !charabase::CheckUseChar(saveLoadConfirmationMsg[1].cb_) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[2].cb_) && !charabase::CheckUseChar(saveLoadConfirmationMsg[3].cb_) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[4].cb_) && !charabase::HitCheckForMouse(pt, saveLoadButton[0].cb_) &&
			!charabase::HitCheckForMouse(pt, saveLoadButton[1].cb_)) {
			charabase::Draw_Char3(worldMap[3].cb_, 255.f, 255.f, 255.f, 255.f);
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				stage::ChangeStage(STG_4);
			}
		}
		else if (!stageCleared[2]) {
			charabase::Draw_Char3(worldMap[3].cb_, 255, 60, 60, 60);
		}
		else {
			charabase::Draw_Char3(worldMap[3].cb_, 255.f, 220.f, 220.f, 220.f);
		}

		if (charabase::HitCheckForMouse(pt, worldMap[4].cb_) && stageCleared[3] && !(pt.x >= 15 && pt.x <= 705 && pt.y >= 525 && pt.y <= 635) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[0].cb_) && !charabase::CheckUseChar(saveLoadConfirmationMsg[1].cb_) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[2].cb_) && !charabase::CheckUseChar(saveLoadConfirmationMsg[3].cb_) &&
			!charabase::CheckUseChar(saveLoadConfirmationMsg[4].cb_) && !charabase::HitCheckForMouse(pt, saveLoadButton[0].cb_) &&
			!charabase::HitCheckForMouse(pt, saveLoadButton[1].cb_)) {
			charabase::Draw_Char3(worldMap[4].cb_, 255.f, 255.f, 255.f, 255.f);
			if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
				stage::ChangeStage(STG_5);
			}
		}
		else if (!stageCleared[3]) {
			charabase::Draw_Char3(worldMap[4].cb_, 255, 60, 60, 60);
		}
		else {
			charabase::Draw_Char3(worldMap[4].cb_, 255.f, 220.f, 220.f, 220.f);
		}
	}
	else {
		charabase::Draw_Char3(worldMap[0].cb_, 255.f, 220.f, 220.f, 220.f);
		charabase::Draw_Char3(worldMap[1].cb_, 255, 60, 60, 60);
		charabase::Draw_Char3(worldMap[2].cb_, 255, 60, 60, 60);
		charabase::Draw_Char3(worldMap[3].cb_, 255, 60, 60, 60);
		charabase::Draw_Char3(worldMap[4].cb_, 255, 60, 60, 60);
	}

	ChangeWeapon();
}

//�`�揈��
void WorldMapDisp() {
	if (timer >= 60 && timer <= 120) {
		graph::Draw_GraphicsLeftTop(mouseX, mouseY, 0.f, "mouseClicked", 0, 0, 32, 53);
	}
	else {
		graph::Draw_GraphicsLeftTop(mouseX, mouseY, 0.f, "mouseUnclick", 0, 0, 32, 53);
	}

	graph::Draw_Graphics(system::WINW / 2.f, 85.f, 0.f, "WorldMapInfoMsg", 0, 0, 642, 123);

	if (firstTimeToMap) {
		if (tipTimer <= 120) {
			graph::Draw_Graphics(system::WINW / 2.f, system::WINH / 2.f, 0.f, "worldMapTip1", 0, 0, system::WINW, system::WINH);
		}
		else if (tipTimer > 120 && tipTimer <= 300) {
			graph::Draw_Graphics(system::WINW / 2.f, system::WINH / 2.f, 0.f, "worldMapTip2", 0, 0, system::WINW, system::WINH);
		}
		else if (tipTimer > 300 && tipTimer <= 420) {
			graph::Draw_Graphics(system::WINW / 2.f, system::WINH / 2.f, 0.f, "worldMapTip3", 0, 0, system::WINW, system::WINH);
		}
		else {
			firstTimeToMap = false;
		}
	}

	if (stageCleared[0] && treasureTipTimer <= 150) {
		graph::Draw_Graphics(system::WINW / 2.f, system::WINH / 2.f, 0.f, "worldMapStg1ClearedTip", 0, 0, 720, 640);
	}

	if (stageCleared[0]) {
		if (charabase::HitCheckForMouse(pt, saveLoadButton[0].cb_)) {
			saveLoadButton[0].cb_.resname = "saveButtonHover";
			charabase::Draw_Char(saveLoadButton[0].cb_);
			if (input::CheckPull(input::KEY_MOUSE_LBTN)) {
				charabase::UseChar(saveLoadConfirmationMsg[0].cb_);
			}
		}
		else {
			saveLoadButton[0].cb_.resname = "saveButton";
			charabase::Draw_Char(saveLoadButton[0].cb_);
		}

		if (charabase::HitCheckForMouse(pt, saveLoadButton[1].cb_)) {
			saveLoadButton[1].cb_.resname = "loadButtonHover";
			charabase::Draw_Char(saveLoadButton[1].cb_);
			if (input::CheckPull(input::KEY_MOUSE_LBTN)) {
				charabase::UseChar(saveLoadConfirmationMsg[1].cb_);
			}
		}
		else {
			saveLoadButton[1].cb_.resname = "loadButton";
			charabase::Draw_Char(saveLoadButton[1].cb_);
		}

		if (charabase::CheckUseChar(saveLoadConfirmationMsg[0].cb_)) {
			charabase::Draw_Char(saveLoadConfirmationMsg[0].cb_);
			charabase::Draw_Char(yesNoButton[0].cb_);
			charabase::Draw_Char(yesNoButton[1].cb_);

			if (charabase::HitCheckForMouse(pt, yesNoButton[0].cb_)) {
				yesNoButton[0].cb_.resname = "yesButtonHover";
				if (input::CheckPull(input::KEY_MOUSE_LBTN)) {
					SaveGameData();

					charabase::NoUseChar(saveLoadConfirmationMsg[0].cb_);
					charabase::UseChar(saveLoadConfirmationMsg[3].cb_);
				}
			}
			else {
				yesNoButton[0].cb_.resname = "yesButton";
			}

			if (charabase::HitCheckForMouse(pt, yesNoButton[1].cb_)) {
				yesNoButton[1].cb_.resname = "noButtonHover";
				if (input::CheckPull(input::KEY_MOUSE_LBTN)) {
					charabase::NoUseChar(saveLoadConfirmationMsg[0].cb_);
				}
			}
			else {
				yesNoButton[1].cb_.resname = "noButton";
			}
		}

		if (charabase::CheckUseChar(saveLoadConfirmationMsg[1].cb_)) {
			charabase::Draw_Char(saveLoadConfirmationMsg[1].cb_);
			charabase::Draw_Char(yesNoButton[0].cb_);
			charabase::Draw_Char(yesNoButton[1].cb_);

			if (charabase::HitCheckForMouse(pt, yesNoButton[0].cb_)) {
				yesNoButton[0].cb_.resname = "yesButtonHover";
				if (input::CheckPull(input::KEY_MOUSE_LBTN)) {
					LoadGameData();

					charabase::NoUseChar(saveLoadConfirmationMsg[1].cb_);

					if (!charabase::CheckUseChar(saveLoadConfirmationMsg[2].cb_)) {
						charabase::UseChar(saveLoadConfirmationMsg[4].cb_);
					}
				}
			}
			else {
				yesNoButton[0].cb_.resname = "yesButton";
			}

			if (charabase::HitCheckForMouse(pt, yesNoButton[1].cb_)) {
				yesNoButton[1].cb_.resname = "noButtonHover";
				if (input::CheckPull(input::KEY_MOUSE_LBTN)) {
					charabase::NoUseChar(saveLoadConfirmationMsg[1].cb_);
				}
			}
			else {
				yesNoButton[1].cb_.resname = "noButton";
			}
		}

		if (charabase::CheckUseChar(saveLoadConfirmationMsg[2].cb_)) {
			charabase::Draw_Char(saveLoadConfirmationMsg[2].cb_);
			yesNoButton[0].cb_.pos.x = (system::WINW / 2.f);
			charabase::Draw_Char(yesNoButton[0].cb_);

			if (charabase::HitCheckForMouse(pt, yesNoButton[0].cb_)) {
				yesNoButton[0].cb_.resname = "yesButtonHover";
				if (input::CheckPull(input::KEY_MOUSE_LBTN)) {
					yesNoButton[0].cb_.pos.x = ((system::WINW / 2.f) - 110.f);
					charabase::NoUseChar(saveLoadConfirmationMsg[1].cb_);
					charabase::NoUseChar(saveLoadConfirmationMsg[2].cb_);
				}
			}
			else {
				yesNoButton[0].cb_.resname = "yesButton";
			}
		}

		if (charabase::CheckUseChar(saveLoadConfirmationMsg[3].cb_)) {
			charabase::Draw_CharNC(saveLoadConfirmationMsg[3].cb_);
			yesNoButton[0].cb_.pos.x = (system::WINW / 2.f);
			charabase::Draw_CharNC(yesNoButton[0].cb_);

			if (charabase::HitCheckForMouse(pt, yesNoButton[0].cb_)) {
				yesNoButton[0].cb_.resname = "yesButtonHover";
				if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
					yesNoButton[0].cb_.pos.x = ((system::WINW / 2.f) - 110.f);
					charabase::NoUseChar(saveLoadConfirmationMsg[3].cb_);
				}
			}
			else {
				yesNoButton[0].cb_.resname = "yesButton";
			}
		}

		if (charabase::CheckUseChar(saveLoadConfirmationMsg[4].cb_)) {
			charabase::Draw_CharNC(saveLoadConfirmationMsg[4].cb_);
			yesNoButton[0].cb_.pos.x = (system::WINW / 2.f);
			charabase::Draw_CharNC(yesNoButton[0].cb_);

			if (charabase::HitCheckForMouse(pt, yesNoButton[0].cb_)) {
				yesNoButton[0].cb_.resname = "yesButtonHover";
				if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
					yesNoButton[0].cb_.pos.x = ((system::WINW / 2.f) - 110.f);
					charabase::NoUseChar(saveLoadConfirmationMsg[4].cb_);
				}
			}
			else {
				yesNoButton[0].cb_.resname = "yesButton";
			}
		}
	}
	

	if (pt.x >= 15 && pt.x <= 705 && pt.y >= 525 && pt.y <= 635) {
		graph::Draw_Graphics((system::WINW / 2.f) + 290.f, (system::WINH / 2.f) + 185.f, 0.1f, "worldMapKeyInfo", 0, 0, 100, 31);
		DispStatus();
		DispScore();
		DispHighScore();
	}
}

void WorldMap()
{
	if (stage::InitStage()){ WorldMapInit(); }
	WorldMapStep();
	WorldMapDisp();
}

//---------------------------------
// Stage1
//---------------------------------
//����������		
void Stage1Init()
{
	currentStage = STG_1;
	money = 0;
	requiredMoney = 15000;
	checkAlive = true;
	checkHit = false;
	checkCleared = false;
	createdKey = false;
	poweredUp = false;
	showTreasure = true;
	showTreasureTime = 25;
	moveTime = 0;
	treasureTipTimer = 0;
	tipTimer = 0;
	attackTimer = 0;
	reminderTimer = 0;
	
	InitPaperKing();
	InitPaperKingHPBar();
	InitAttack();
	InitJunk();
	InitJunkHPBar();
	InitObstacles();
	InitObstacleHPBar();
	InitCoin();
	InitKey();
	InitTreasureBox();
}

//�t���[������
void Stage1Step()
{
	if (tipTimer <= 360) {
		tipTimer++;
	}

	if (checkGotTreasure[0] && treasureTipTimer <= 120) {
		treasureTipTimer++;
	}

	if (input::CheckPress(input::KEY_BTN0)) {
		attackTimer++;
	}
	else {
		attackTimer = 0;
	}

	MovePaperKing();
	MovePaperKingHPBar();
	MoveAttack();
	MoveArrows();
	MoveJunk();
	MoveJunkHPBar();
	MoveObstacleHPBar();
	MoveCoin();
	StageCleared();
}

//�`�揈��
void Stage1Disp()
{
	graph::Draw_Graphics(480, 425, 1.f, "stg1Map", 0, 0, 1680, 1490);
	
	if (checkGotTreasure[0] && treasureTipTimer <= 120) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg1TreasureTip", 0, 0, system::WINW, system::WINH);
	}
	
	if (tipTimer <= 120) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg1Tip1", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 120 && tipTimer <= 240) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg1Tip2", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 240 && tipTimer <= 360) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg1Tip3", 0, 0, system::WINW, system::WINH);
	}

	if ((paperKing.hp / paperKing.maxHP) <= 0.5f) {
		if (reminderTimer <= 120) {
			graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "lowHPReminder", 0, 0, system::WINW, system::WINH);
			reminderTimer++;
		}
	}
	else {
		reminderTimer = 0;
	}

	DispPaperKingHPBar();
	DispPaperKing();
	if (input::CheckPress(input::KEY_BTN0) && checkAlive) {
		DispAttack();
	}
	DispArrows();
	DispJunkHPBar();
	DispJunk();
	DispObstacles();
	DispObstacleHPBar();
	DispCoin();
	DispKey();
	DispTreasureBox();
	DispStatus();
	DispScore();
	DispHighScore();
}

void Stage1()
{
	if (stage::InitStage()){ Stage1Init(); }
	Stage1Step();
	Stage1Disp();
}


//---------------------------------
// Stage2
//---------------------------------
//����������		
void Stage2Init()
{
	currentStage = STG_2;
	money = 0;
	requiredMoney = 18000;
	checkAlive = true;
	checkHit = false;
	checkCleared = false;
	createdKey = false;
	poweredUp = false;
	showTreasure = true;
	showTreasureTime = 25;
	moveTime = 0;
	timer = 0;
	treasureTipTimer = 0;
	tipTimer = 0;
	attackTimer = 0;
	reminderTimer = 0;

	InitBridge();
	InitPaperKing();
	InitPaperKingHPBar();
	InitAttack();
	InitJunk();
	InitJunkHPBar();
	InitObstacles();
	InitObstacleHPBar();
	InitCoin();
	InitKey();
	InitTreasureBox();
}

//�t���[������
void Stage2Step()
{
	if (timer >= 120) {
		timer = 0;
	}
	else {
		timer++;
	}

	if (checkGotTreasure[1] && treasureTipTimer <= 120) {
		treasureTipTimer++;
	}

	if (tipTimer <= 240) {
		tipTimer++;
	}

	if (input::CheckPress(input::KEY_BTN0)) {
		attackTimer++;
	}
	else {
		attackTimer = 0;
	}

	MovePaperKing();
	MovePaperKingHPBar();
	MoveAttack();
	MoveArrows();
	MoveJunk();
	MoveJunkHPBar();
	MoveObstacleHPBar();
	MoveCoin();
	StageCleared();
}

//�`�揈��
void Stage2Disp()
{
	graph::Draw_Graphics(480, 425, 1.f, "stg2Map", 0, 0, 1680, 1490);

	if (checkGotTreasure[1] && treasureTipTimer <= 120) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg2TreasureTip", 0, 0, system::WINW, system::WINH);
	}

	if (tipTimer <= 120) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg2Tip1", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 120 && tipTimer <= 240) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg2Tip2", 0, 0, system::WINW, system::WINH);
	}

	if ((paperKing.hp / paperKing.maxHP) <= 0.5f) {
		if (reminderTimer <= 120) {
			graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "lowHPReminder", 0, 0, system::WINW, system::WINH);
			reminderTimer++;
		}
	}
	else {
		reminderTimer = 0;
	}

	DispBridge();
	DispPaperKingHPBar();
	DispPaperKing();
	if (input::CheckPress(input::KEY_BTN0) && checkAlive) {
		DispAttack();
	}
	DispArrows();
	DispJunkHPBar();
	DispJunk();
	DispObstacles();
	DispObstacleHPBar();
	DispCoin();
	DispKey();
	DispTreasureBox();
	DispStatus();
	DispScore();
	DispHighScore();
}

void Stage2()
{
	if (stage::InitStage()){ Stage2Init(); }
	Stage2Step();
	Stage2Disp();
}

//---------------------------------
// Stage3
//---------------------------------
//����������		
void Stage3Init()
{
	currentStage = STG_3;
	money = 0;
	requiredMoney = 23000;
	checkAlive = true;
	checkHit = false;
	checkCleared = false;
	createdKey = false;
	showTreasure = true;
	showTreasureTime = 25;
	moveTime = 0;
	timer = 0;
	treasureTipTimer = 0;
	tipTimer = 0;
	attackTimer = 0;
	reminderTimer = 0;

	InitPaperKing();
	InitPaperKingHPBar();
	InitAttack();
	InitJunk();
	InitJunkHPBar();
	InitObstacles();
	InitObstacleHPBar();
	InitCoin();
	InitKey();
	InitTreasureBox();
}

//�t���[������
void Stage3Step()
{
	if (timer >= 120) {
		timer = 0;
	}
	else {
		timer++;
	}

	if (checkGotTreasure[2] && treasureTipTimer <= 120) {
		treasureTipTimer++;
	}

	if (tipTimer <= 120) {
		tipTimer++;
	}

	if (input::CheckPress(input::KEY_BTN0)) {
		attackTimer++;
	}
	else {
		attackTimer = 0;
	}

	MovePaperKing();
	MovePaperKingHPBar();
	MoveAttack();
	MoveArrows();
	MoveJunk();
	MoveJunkHPBar();
	MoveObstacleHPBar();
	MoveCoin();
	StageCleared();
}

//�`�揈��
void Stage3Disp()
{
	graph::Draw_Graphics(480, 425, 1.f, "stg3Map", 0, 0, 1680, 1490);

	if (checkGotTreasure[2] && treasureTipTimer <= 120) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg3TreasureTip", 0, 0, system::WINW, system::WINH);
	}

	if (tipTimer <= 120) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg3Tip1", 0, 0, system::WINW, system::WINH);
	}

	if ((paperKing.hp / paperKing.maxHP) <= 0.5f) {
		if (reminderTimer <= 120) {
			graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "lowHPReminder", 0, 0, system::WINW, system::WINH);
			reminderTimer++;
		}
	}
	else {
		reminderTimer = 0;
	}

	DispPaperKingHPBar();
	DispPaperKing();
	if (input::CheckPress(input::KEY_BTN0) && checkAlive) {
		DispAttack();
	}
	DispArrows();
	DispJunkHPBar();
	DispJunk();
	DispObstacles();
	DispObstacleHPBar();
	DispCoin();
	DispKey();
	DispTreasureBox();
	DispStatus();
	DispScore();
	DispHighScore();
}

void Stage3()
{
	if (stage::InitStage()){ Stage3Init(); }
	Stage3Step();
	Stage3Disp();
}

//---------------------------------
// Stage4
//---------------------------------
//����������		
void Stage4Init()
{
	currentStage = STG_4;
	money = 0;
	requiredMoney = 25000;
	checkAlive = true;
	checkHit = false;
	checkCleared = false;
	createdKey = false;
	enableSpecialAttack = false;
	showTreasure = true;
	showTreasureTime = 25;
	moveTime = 0;
	timer = 0;
	treasureTipTimer = 0;
	tipTimer = 0;
	attackTimer = 0;
	reminderTimer = 0;

	InitPaperKing();
	InitPaperKingHPBar();
	InitAttack();
	InitJunk();
	InitJunkHPBar();
	InitObstacles();
	InitObstacleHPBar();
	InitCoin();
	InitKey();
	InitApple();
	InitTreasureBox();
}

//�t���[������
void Stage4Step()
{
	if (timer >= 3600) {
		timer = 0;
	}
	else {
		timer++;
	}

	if (checkGotTreasure[3] && treasureTipTimer <= 380) {
		treasureTipTimer++;
	}

	if (tipTimer <= 600) {
		tipTimer++;
	}

	if (input::CheckPress(input::KEY_BTN0)) {
		attackTimer++;
	}
	else {
		attackTimer = 0;
	}

	MovePaperKing();
	MovePaperKingHPBar();
	MoveAttack();
	MoveArrows();
	MoveExcaliburBeam();
	MoveJunk();
	MoveJunkHPBar();
	MoveObstacleHPBar();
	MoveCoin();
	MoveApple();
	StageCleared();
}

//�`�揈��
void Stage4Disp()
{
	graph::Draw_Graphics(480, 425, 1.f, "stg4Map", 0, 0, 1680, 1490);

	if (checkGotTreasure[3] && treasureTipTimer <= 120) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg4TreasureTip1", 0, 0, system::WINW, system::WINH);
	}
	else if (checkGotTreasure[3] && treasureTipTimer > 120 && treasureTipTimer <= 240) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg4TreasureTip2", 0, 0, system::WINW, system::WINH);
	}
	else if (checkGotTreasure[3] && treasureTipTimer > 240 && treasureTipTimer <= 380) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg4TreasureTip3", 0, 0, system::WINW, system::WINH);
	}

	if (tipTimer <= 120) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg4Tip1", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 120 && tipTimer <= 240) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg4Tip2", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 240 && tipTimer <= 360) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg4Tip3", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 360 && tipTimer <= 480) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg4Tip4", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 480 && tipTimer <= 600) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg4Tip5", 0, 0, system::WINW, system::WINH);
	}

	if ((paperKing.hp / paperKing.maxHP) <= 0.5f) {
		if (reminderTimer <= 120) {
			graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "lowHPReminder", 0, 0, system::WINW, system::WINH);
			reminderTimer++;
		}
	}
	else {
		reminderTimer = 0;
	}

	DispPaperKingHPBar();
	DispPaperKing();
	if (input::CheckPress(input::KEY_BTN0) && checkAlive) {
		DispAttack();
	}
	DispArrows();
	DispExcaliburBeam();
	DispJunkHPBar();
	DispJunk();
	DispObstacles();
	DispObstacleHPBar();
	DispCoin();
	DispKey();
	DispApple();
	DispTreasureBox();
	DispStatus();
	DispScore();
	DispHighScore();
}

void Stage4()
{
	if (stage::InitStage()){ Stage4Init(); }
	Stage4Step();
	Stage4Disp();
}

//---------------------------------
// Stage5
//---------------------------------
//����������		
void Stage5Init()
{
	currentStage = STG_5;
	money = 0;
	requiredMoney = 28000;
	checkAlive = true;
	checkHit = false;
	checkCleared = false;
	checkDefeatedBoss = false;
	enableSpecialAttack = false;
	moveTime = 0;
	junkKingMoveTime = 0;
	decisionTime = 0;
	hitWall = false;
	timer = 0;
	tipTimer = 0;
	attackTimer = 0;
	reminderTimer = 0;

	InitPaperKing();
	InitPaperKingHPBar();
	InitAttack();
	InitJunkKing();
	InitJunkKingHPBar();
	InitJunk();
	InitJunkHPBar();
	InitObstacles();
	InitObstacleHPBar();
	InitCoin();
}

//�t���[������
void Stage5Step()
{
	if (timer >= 3600) {
		timer = 0;
	}
	else {
		timer++;
	}

	if (tipTimer <= 360) {
		tipTimer++;
	}

	if (input::CheckPress(input::KEY_BTN0)) {
		attackTimer++;
	}
	else {
		attackTimer = 0;
	}

	MovePaperKing();
	MovePaperKingHPBar();
	MoveAttack();
	MoveArrows();
	MoveExcaliburBeam();
	if (!checkDefeatedBoss) {
		MoveJunkKing();
		MoveJunkKingHPBar();
	}
	MoveJunk();
	MoveJunkHPBar();
	MoveObstacleHPBar();
	MoveCoin();
	StageCleared();
}

//�`�揈��
void Stage5Disp()
{
	graph::Draw_Graphics(480, 425, 1.f, "stg5Map", 0, 0, 1680, 1490);

	if (tipTimer <= 120) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg5Tip1", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 120 && tipTimer <= 240) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg5Tip2", 0, 0, system::WINW, system::WINH);
	}
	else if (tipTimer > 240 && tipTimer <= 360) {
		graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "stg5Tip3", 0, 0, system::WINW, system::WINH);
	}
	
	if ((paperKing.hp / paperKing.maxHP) <= 0.5f) {
		if (reminderTimer <= 120) {
			graph::Draw_Graphics(paperKing.cb_.pos.x, paperKing.cb_.pos.y, 0.f, "lowHPReminder", 0, 0, system::WINW, system::WINH);
			reminderTimer++;
		}
	}
	else {
		reminderTimer = 0;
	}

	DispPaperKingHPBar();
	DispPaperKing();
	if (input::CheckPress(input::KEY_BTN0) && checkAlive) {
		DispAttack();
	}
	DispArrows();
	DispExcaliburBeam();

	if (checkDefeatedBoss) {
		junkKing.cb_.src.x = 0;
		junkKing.cb_.src.y = 0;
		junkKing.cb_.width = 147;
		junkKing.cb_.height = 42;
		junkKing.cb_.resname = "junkKingDefeated";
	}

	DispJunkKing();
	DispJunkKingHPBar();
	DispJunkHPBar();
	DispJunk();
	DispObstacles();
	DispObstacleHPBar();
	DispCoin();
	DispStatus();
	DispScore();
	DispHighScore();
}

void Stage5()
{
	if (stage::InitStage()){ Stage5Init(); }
	Stage5Step();
	Stage5Disp();
}

//---------------------------------
// StageEnd
//---------------------------------
//����������		
void StageEndInit()
{
	currentStage = STG_END;
	introNumber = 1;
	textNumber = 0;
	scrollSpeed = 30;
	timer = 0;
	dispScoreTimer = 0;
	endingTimer = 150;
	scrollX = 0.f;
	scrollY = 0.f;
	mouseX = 320.f;
	mouseY = 260.f;
	camera::SetLookAt(scrollX, scrollY);

	for (bool& dialogShown : dialogsShown) {
		dialogShown = false;
	}

	clearedTutorial = false;
}
//�t���[������
void StageEndStep()
{
	if (timer >= 120) {
		timer = 0;
	}
	else {
		timer++;
	}

	if (introNumber == 4) {
		if (dispScoreTimer < 150) {
			dispScoreTimer++;
		}
	}

	if (endingTimer > 0 && introNumber == 4) {
		endingTimer--;
	}

	if (input::CheckPush(input::KEY_MOUSE_LBTN)) {
		switch (introNumber) {
		case 1:
			textNumber++;
			if (textNumber == 1) {
				dialogsShown[0] = true;
			}
			if (textNumber == 2) {
				dialogsShown[1] = true;
			}
			if (textNumber == 3) {
				dialogsShown[2] = true;
			}
			if (textNumber > 3) {
				scrollX += 723;
				mouseX += 720;
				camera::SetCameraTarget(scrollX, scrollY, scrollSpeed);
				introNumber++;
				textNumber = 0;
			}
			break;
		case 2:
			textNumber++;
			if (textNumber == 1) {
				dialogsShown[3] = true;
			}
			if (textNumber == 2) {
				dialogsShown[4] = true;
			}
			if (textNumber > 2) {
				scrollX += 721;
				mouseX += 720;
				camera::SetCameraTarget(scrollX, scrollY, scrollSpeed);
				introNumber++;
				textNumber = 0;
			}
			break;
		case 3:
			scrollX += 720;
			mouseX += 720;
			camera::SetCameraTarget(scrollX, scrollY, scrollSpeed);
			introNumber++;
			break;
		case 4:
			if (endingTimer == 0) {
				clearedTutorial = false;
				stage::ChangeStage(STG_TITLE);
			}
			break;
		}
	}
}

//�`�揈��
void StageEndDisp()
{
	if (choice == KINDEND) {
		graph::Draw_Graphics(1080, 0, 1.f, "StageEnd_Kind", 0, 0, 2880, 640);

		if (dialogsShown[0]) {
			graph::Draw_Graphics(-170, -25, 0.f, "victoryDeclaration", 0, 0, 253, 230);
		}
		if (dialogsShown[1]) {
			graph::Draw_Graphics(210, 65, 0.f, "surrender", 0, 0, 253, 230);
		}
		if (dialogsShown[2]) {
			graph::Draw_Graphics(-40, 40, 0.f, "thinking", 0, 0, 253, 164);
		}
		if (dialogsShown[3]) {
			graph::Draw_Graphics(950, -130, 0.f, "apologize", 0, 0, 253, 230);
		}
		if (dialogsShown[4]) {
			graph::Draw_Graphics(600, 0, 0.f, "askForFavor", 0, 0, 253, 230);
		}
	}
	else if (choice == VIOLENTEND) {
		graph::Draw_Graphics(1080, 0, 1.f, "StageEnd_Violent", 0, 0, 2880, 640);
	
	
		if (dialogsShown[0]) {
			graph::Draw_Graphics(-170, -25, 0.f, "victoryDeclaration", 0, 0, 253, 230);
		}
		if (dialogsShown[1]) {
			graph::Draw_Graphics(210, 65, 0.f, "surrender", 0, 0, 253, 230);
		}
		if (dialogsShown[2]) {
			graph::Draw_Graphics(-40, 40, 0.f, "expel1", 0, 0, 253, 164);
		}
		if (dialogsShown[3]) {
			graph::Draw_Graphics(670, 0, 0.f, "expel2", 0, 0, 253, 230);
		}
		if (dialogsShown[4]) {
			graph::Draw_Graphics(883, -100, 0.f, "silence", 0, 0, 253, 230);
		}
	}

	if (timer >= 60 && timer <= 120) {
		graph::Draw_GraphicsLeftTop(mouseX, mouseY, 1.f, "mouseClicked", 0, 0, 32, 53);
	}
	else {
		graph::Draw_GraphicsLeftTop(mouseX, mouseY, 1.f, "mouseUnclick", 0, 0, 32, 53);
	}

	if (introNumber == 4 && dispScoreTimer == 150) {
		std::string score("You got " + std::to_string(score));
		font::Draw_FontTextNC(320, 15, 0.f, score, ARGB(255, 0, 0, 0), 0);
		std::string highScore("High Score: " + std::to_string(highScore));
		font::Draw_FontTextNC(285, 45, 0.f, highScore, ARGB(255, 0, 0, 0), 0);
	}
}

void StageEnd()
{
	if (stage::InitStage()){ StageEndInit(); }
	StageEndStep();
	StageEndDisp();
}

//---------------------------------
// GameOver
//---------------------------------
//����������		
void GameOverInit()
{
	currentStage = STG_GAMEOVER;
	camera::SetLookAt(0.f, 0.f);
}
//�t���[������
void GameOverStep()
{
	if (input::CheckPush(input::KEY_F1)) {
		clearedTutorial = false;
		stage::ChangeStage(STG_TITLE);
	}
}

//�`�揈��
void GameOverDisp()
{
	graph::Draw_Graphics(0, 0, 1.f, "gameOver", 0, 0, 720, 640);

	std::string score("You got " + std::to_string(score));
	font::Draw_FontTextNC(320, 400, 0.f, score, ARGB(255, 255, 255, 255), 0);
	std::string highScore("High Score: " + std::to_string(highScore));
	font::Draw_FontTextNC(285, 430, 0.f, highScore, ARGB(255, 255, 255, 255), 0);
}

void GameOver()
{
	if (stage::InitStage()){ GameOverInit(); }
	GameOverStep();
	GameOverDisp();
}

//---------------------------------------------------------------------------------------
//�Q�[�����[�v
//---------------------------------------------------------------------------------------
void system::GameLoop()
{
	switch (stage::CheckStage()) {
	case STG_TITLE:
		Title();
		break;
	case STG_TUTORIAL:
		Tutorial();
		break;
	case STG_WORLDMAP:
		WorldMap();
		break;
	case STG_1:
		Stage1();
		break;
	case STG_2:
		Stage2();
		break;
	case STG_3:
		Stage3();
		break;
	case STG_4:
		Stage4();
		break;
	case STG_5:
		Stage5();
		break;
	case STG_END:
		StageEnd();
		break;
	case STG_GAMEOVER:
		GameOver();
		break;
	}
}

//---------------------------------------------------------------------------------------
//�Q�[���J�n���̃��[�h����
//---------------------------------------------------------------------------------------
void LoadGame()
{
	font::Draw_CreateFont(0, 30, "Buxton Sketch");
	font::Draw_CreateFont(1, 20, "Buxton Sketch");
	font::Draw_CreateFont(2, 512, "�܂ǂ̃t�H���g");

	//�����[�h����
	se::DSound_LoadFile("tinkle", "res/snd/tinkle.wav");
	se::DSound_LoadFile("swoosh", "res/snd/swoosh.wav");
	se::DSound_LoadFile("shootArrow", "res/snd/shootArrow.wav");
	se::DSound_LoadFile("beam", "res/snd/beam.wav");
	se::DSound_LoadFile("appleBite", "res/snd/appleBite.wav");
	se::DSound_LoadFile("smashing", "res/snd/smashing.wav");
	se::DSound_LoadFile("bump", "res/snd/bump.wav");
	se::DSound_LoadFile("metalBang", "res/snd/metalBang.wav");
	se::DSound_LoadFile("farty", "res/snd/farty.wav");
	se::DSound_LoadFile("flying", "res/snd/flying.wav");
	se::DSound_LoadFile("dyingRobot", "res/snd/dyingRobot.wav");

	//�摜���[�h����
	graph::Draw_LoadObject("title", "res/gra/maps/title.png");
	graph::Draw_LoadObject("StageEnd_Violent", "res/gra/maps/StageEnd_Violent.png");
	graph::Draw_LoadObject("StageEnd_Kind", "res/gra/maps/StageEnd_Kind.png");
	graph::Draw_LoadObject("gameOver", "res/gra/maps/gameOver.png");
	graph::Draw_LoadObject("WorldMapInfoMsg", "res/gra/maps/WorldMapInfoMsg.png");
	graph::Draw_LoadObject("WorldMap_Garden", "res/gra/maps/WorldMap_Garden.png");
	graph::Draw_LoadObject("WorldMap_River", "res/gra/maps/WorldMap_River.png");
	graph::Draw_LoadObject("WorldMap_City", "res/gra/maps/WorldMap_City.png");
	graph::Draw_LoadObject("WorldMap_Forest", "res/gra/maps/WorldMap_Forest.png");
	graph::Draw_LoadObject("WorldMap_JunkYard", "res/gra/maps/WorldMap_JunkYard.png");
	graph::Draw_LoadObject("informKing1", "res/gra/maps/informKing.png");
	graph::Draw_LoadObject("informKing2", "res/gra/maps/informKing2.png");
	graph::Draw_LoadObject("kindEnd", "res/gra/maps/kindEnd.png");
	graph::Draw_LoadObject("kingDeclaration", "res/gra/maps/kingDeclaration.png");
	graph::Draw_LoadObject("looksBad", "res/gra/maps/looksBad.png");
	graph::Draw_LoadObject("noTutorial", "res/gra/maps/noTutorial.png");
	graph::Draw_LoadObject("tutorialSuggestion", "res/gra/maps/tutorialSuggestion.png");
	graph::Draw_LoadObject("violentEnd", "res/gra/maps/violentEnd.png");
	graph::Draw_LoadObject("what", "res/gra/maps/what.png");
	graph::Draw_LoadObject("yesTutorial", "res/gra/maps/yesTutorial.png");
	graph::Draw_LoadObject("skipButton", "res/gra/messages/skipButton.png");
	graph::Draw_LoadObject("skipButtonHover", "res/gra/messages/skipButtonHover.png");
	graph::Draw_LoadObject("saveButton", "res/gra/messages/saveButton.png");
	graph::Draw_LoadObject("saveButtonHover", "res/gra/messages/saveButtonHover.png");
	graph::Draw_LoadObject("loadButton", "res/gra/messages/loadButton.png");
	graph::Draw_LoadObject("loadButtonHover", "res/gra/messages/loadButtonHover.png");
	graph::Draw_LoadObject("saveConfirmationMsg", "res/gra/messages/saveConfirmationMsg.png");
	graph::Draw_LoadObject("loadConfirmationMsg", "res/gra/messages/loadConfirmationMsg.png");
	graph::Draw_LoadObject("doneSavingMsg", "res/gra/messages/doneSavingMsg.png");
	graph::Draw_LoadObject("doneLoadingMsg", "res/gra/messages/doneLoadingMsg.png");
	graph::Draw_LoadObject("noSaveDataMsg", "res/gra/messages/noSaveDataMsg.png");
	graph::Draw_LoadObject("yesButton", "res/gra/messages/yesButton.png");
	graph::Draw_LoadObject("yesButtonHover", "res/gra/messages/yesButtonHover.png");
	graph::Draw_LoadObject("noButton", "res/gra/messages/noButton.png");
	graph::Draw_LoadObject("noButtonHover", "res/gra/messages/noButtonHover.png");
	graph::Draw_LoadObject("mouseUnclick", "res/gra/messages/mouseUnclick.png");
	graph::Draw_LoadObject("mouseClicked", "res/gra/messages/mouseClicked.png");
	graph::Draw_LoadObject("victoryDeclaration", "res/gra/maps/victoryDeclaration.png");
	graph::Draw_LoadObject("surrender", "res/gra/maps/surrender.png");
	graph::Draw_LoadObject("expel1", "res/gra/maps/expel1.png");
	graph::Draw_LoadObject("expel2", "res/gra/maps/expel2.png");
	graph::Draw_LoadObject("silence", "res/gra/maps/silence.png");
	graph::Draw_LoadObject("thinking", "res/gra/maps/thinking.png");
	graph::Draw_LoadObject("apologize", "res/gra/maps/apologize.png");
	graph::Draw_LoadObject("askForFavor", "res/gra/maps/askForFavor.png");

	graph::Draw_LoadObject("worldMapKeyInfo", "res/gra/messages/worldMapKeyInfo.png");
	graph::Draw_LoadObject("scroll", "res/gra/messages/scroll.png");
	graph::Draw_LoadObject("scrollWApple", "res/gra/messages/scrollWApple.png");
	graph::Draw_LoadObject("head", "res/gra/messages/head.png");
	graph::Draw_LoadObject("hasKey", "res/gra/messages/key.png");
	graph::Draw_LoadObject("questionMark", "res/gra/messages/questionMark.png");
	graph::Draw_LoadObject("hasPlackart", "res/gra/messages/plackart.png");
	graph::Draw_LoadObject("plackartEffect", "res/gra/messages/plackartEffect.png");
	graph::Draw_LoadObject("hasGreave", "res/gra/messages/greave.png");
	graph::Draw_LoadObject("greaveEffect", "res/gra/messages/greaveEffect.png");
	graph::Draw_LoadObject("hasGauntlet", "res/gra/messages/gauntlet.png");
	graph::Draw_LoadObject("gauntletEffect", "res/gra/messages/gauntletEffect.png");
	graph::Draw_LoadObject("hasExcalibur", "res/gra/messages/excalibur.png");
	graph::Draw_LoadObject("excaliburEffect", "res/gra/messages/excaliburEffect.png");
	graph::Draw_LoadObject("usingSword", "res/gra/messages/sword.png");
	graph::Draw_LoadObject("usingBow", "res/gra/messages/bow.png");
	graph::Draw_LoadObject("usingExcalibur", "res/gra/messages/usingExcalibur.png");
	graph::Draw_LoadObject("paperKing", "res/gra/characters/paperKing.png");
	graph::Draw_LoadObject("paperKingWPlackart", "res/gra/characters/paperKingWithP.png");
	graph::Draw_LoadObject("paperKingWGreave", "res/gra/characters/paperKingWithGreave.png");
	graph::Draw_LoadObject("paperKingWGauntlet", "res/gra/characters/paperKingWithGauntlet.png");
	graph::Draw_LoadObject("paperKingWPlac&Greave", "res/gra/characters/paperKingWithPGreave.png");
	graph::Draw_LoadObject("paperKingWPlac&Gauntlet", "res/gra/characters/paperKingWithPGauntlet.png");
	graph::Draw_LoadObject("paperKingWGreave&Gauntlet", "res/gra/characters/paperKingWithGG.png");
	graph::Draw_LoadObject("paperKingFullyArmored", "res/gra/characters/paperKingFullyArmored.png");
	graph::Draw_LoadObject("paperKingDefeated", "res/gra/characters/paperKingDefeated.png");
	graph::Draw_LoadObject("junkKing", "res/gra/characters/junkKing.png");
	graph::Draw_LoadObject("junkKingDefeated", "res/gra/characters/junkKingDefeated.png");
	graph::Draw_LoadObject("tutorialMap", "res/gra/maps/trainingArena.png");
	graph::Draw_LoadObject("stg1Map", "res/gra/maps/dirtyGarden.png");
	graph::Draw_LoadObject("stg2Map", "res/gra/maps/dirtyRiver.png");
	graph::Draw_LoadObject("bridge", "res/gra/maps/bridge.png");
	graph::Draw_LoadObject("stg3Map", "res/gra/maps/dirtyCity.png");
	graph::Draw_LoadObject("stg4Map", "res/gra/maps/dirtyForest.png");
	graph::Draw_LoadObject("stg5Map", "res/gra/maps/junkYard.png");

	graph::Draw_LoadObject("lowHPReminder", "res/gra/messages/lowHPReminder.png");
	graph::Draw_LoadObject("tutTip1", "res/gra/messages/tutorialTip1.png");
	graph::Draw_LoadObject("tutTip2", "res/gra/messages/tutorialTip2.png");
	graph::Draw_LoadObject("tutTip3", "res/gra/messages/tutorialTip3.png");
	graph::Draw_LoadObject("tutTip4", "res/gra/messages/tutorialTip4.png");
	graph::Draw_LoadObject("tutTip5", "res/gra/messages/tutorialTip5.png");
	graph::Draw_LoadObject("tutTip6", "res/gra/messages/tutorialTip6.png");
	graph::Draw_LoadObject("tutTip7", "res/gra/messages/tutorialTip7.png");
	graph::Draw_LoadObject("tutTip8", "res/gra/messages/tutorialTip8.png");
	graph::Draw_LoadObject("tutTip9", "res/gra/messages/tutorialTip9.png");
	graph::Draw_LoadObject("tutTip10", "res/gra/messages/tutorialTip10.png");
	graph::Draw_LoadObject("tutTip11", "res/gra/messages/tutorialTip11.png");
	graph::Draw_LoadObject("tutTip12", "res/gra/messages/tutorialTip12.png");
	graph::Draw_LoadObject("tutTip13", "res/gra/messages/tutorialTip13.png");
	graph::Draw_LoadObject("tutTip14", "res/gra/messages/tutorialTip14.png");
	graph::Draw_LoadObject("tutTip15", "res/gra/messages/tutorialTip15.png");
	graph::Draw_LoadObject("tutTip16", "res/gra/messages/tutorialTip16.png");
	graph::Draw_LoadObject("tutTreasureTip", "res/gra/messages/tutorialTreasureTip.png");
	graph::Draw_LoadObject("worldMapTip1", "res/gra/messages/worldMapTip1.png");
	graph::Draw_LoadObject("worldMapTip2", "res/gra/messages/worldMapTip2.png");
	graph::Draw_LoadObject("worldMapTip3", "res/gra/messages/worldMapTip3.png");
	graph::Draw_LoadObject("worldMapStg1ClearedTip", "res/gra/messages/worldMapStg1ClearedTip.png");
	graph::Draw_LoadObject("stg1Tip1", "res/gra/messages/stg1Tip1.png");
	graph::Draw_LoadObject("stg1Tip2", "res/gra/messages/stg1Tip2.png");
	graph::Draw_LoadObject("stg1Tip3", "res/gra/messages/stg1Tip3.png");
	graph::Draw_LoadObject("stg1TreasureTip", "res/gra/messages/stg1TreasureTip.png");
	graph::Draw_LoadObject("stg2Tip1", "res/gra/messages/stg2Tip1.png");
	graph::Draw_LoadObject("stg2Tip2", "res/gra/messages/stg2Tip2.png");
	graph::Draw_LoadObject("stg2TreasureTip", "res/gra/messages/stg2TreasureTip.png");
	graph::Draw_LoadObject("stg3Tip1", "res/gra/messages/stg3Tip1.png");
	graph::Draw_LoadObject("stg3TreasureTip", "res/gra/messages/stg3TreasureTip.png");
	graph::Draw_LoadObject("stg4Tip1", "res/gra/messages/stg4Tip1.png");
	graph::Draw_LoadObject("stg4Tip2", "res/gra/messages/stg4Tip2.png");
	graph::Draw_LoadObject("stg4Tip3", "res/gra/messages/stg4Tip3.png");
	graph::Draw_LoadObject("stg4Tip4", "res/gra/messages/stg4Tip4.png");
	graph::Draw_LoadObject("stg4Tip5", "res/gra/messages/stg4Tip5.png");
	graph::Draw_LoadObject("stg4TreasureTip1", "res/gra/messages/stg4TreasureTip1.png");
	graph::Draw_LoadObject("stg4TreasureTip2", "res/gra/messages/stg4TreasureTip2.png");
	graph::Draw_LoadObject("stg4TreasureTip3", "res/gra/messages/stg4TreasureTip3.png");
	graph::Draw_LoadObject("stg5Tip1", "res/gra/messages/stg5Tip1.png");
	graph::Draw_LoadObject("stg5Tip2", "res/gra/messages/stg5Tip2.png");
	graph::Draw_LoadObject("stg5Tip3", "res/gra/messages/stg5Tip3.png");

	graph::Draw_LoadObject("log", "res/gra/junks/log.png");
	graph::Draw_LoadObject("deadFish", "res/gra/junks/deadFish.png");
	graph::Draw_LoadObject("garbageBin", "res/gra/junks/garbageBin.png");
	graph::Draw_LoadObject("poo", "res/gra/junks/poo.png");
	graph::Draw_LoadObject("swordV", "res/gra/items/swordV.png");
	graph::Draw_LoadObject("swordH", "res/gra/items/swordH.png");
	graph::Draw_LoadObject("excaliburV", "res/gra/items/excaliburV.png");
	graph::Draw_LoadObject("excaliburH", "res/gra/items/excaliburH.png");
	graph::Draw_LoadObject("excaliburBeamH", "res/gra/items/excaliburBeamH.png");
	graph::Draw_LoadObject("excaliburBeamV", "res/gra/items/excaliburBeamV.png");
	graph::Draw_LoadObject("bow", "res/gra/items/bow.png");
	graph::Draw_LoadObject("arrowR", "res/gra/items/arrowR.png");
	graph::Draw_LoadObject("arrowU", "res/gra/items/arrowU.png");
	graph::Draw_LoadObject("arrowD", "res/gra/items/arrowD.png");
	graph::Draw_LoadObject("arrowL", "res/gra/items/arrowL.png");
	graph::Draw_LoadObject("coin", "res/gra/items/coin.png");
	graph::Draw_LoadObject("key", "res/gra/items/key.png");
	graph::Draw_LoadObject("apple", "res/gra/items/apple.png");
	graph::Draw_LoadObject("treasureBox", "res/gra/items/treasureBox.png");
	graph::Draw_LoadObject("fountain", "res/gra/items/fountain.png");
	graph::Draw_LoadObject("statue", "res/gra/items/statue.png");
	graph::Draw_LoadObject("house", "res/gra/items/house.png");
	graph::Draw_LoadObject("streetLight", "res/gra/items/streetLight.png");
	graph::Draw_LoadObject("tree", "res/gra/items/tree.png");
	graph::Draw_LoadObject("rock", "res/gra/items/rock.png");
	graph::Draw_LoadObject("emptyBar", "res/gra/messages/emptyBar.png");
	graph::Draw_LoadObject("fullHPBar", "res/gra/messages/fullHPBar.png");
	graph::Draw_LoadObject("fullPowerBar", "res/gra/messages/fullPowerBar.png");

	graph::Draw_LoadObject("stageClear", "res/gra/messages/stageClear.png");
	graph::Draw_LoadObject("stageClearWBonus", "res/gra/messages/stageClearWBonus.png");
	graph::Draw_LoadObject("retryMessage", "res/gra/messages/retryMessage.png");

	collectedMoney = 0;

	//�n�C�X�R�A�̋L�^��ǂݍ���
	FILE* pFile = fopen("highScore.dat", "rb");

	if (pFile) {
		fread(&highScore, sizeof(highScore), 1, pFile);

		fclose(pFile);
	}
}

//---------------------------------------------------------------------------------------
//�v���O�����S�̂̏�����
//�t�H���g�̍쐬�A���[�f�B���O��ʂŎg�p����摜�̓ǂݍ��݂Ȃ�
//�N�����Ɉ�񂾂��s�������͂����ōs��
//�ȉ��͊�{�I�ɕύX���Ȃ��B
//---------------------------------------------------------------------------------------
void InitGame()
{
	//2D�J�����̒����_
	camera::InitCamera(system::WINW / 2, system::WINH / 2, 0.0f, 0.0f);

}

void system::OnCreate()
{
	LoadGame();
	InitGame();

}

//---------------------------------------------------------------------------------------
//�v���O�����S�̂̌�n��
//�I�����Ɉ�񂾂��s�������͂����ōs��
//---------------------------------------------------------------------------------------
void system::OnDestroy()
{
}

//======================================================================================//
//WINDOW�@���C��
//======================================================================================//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	return system::DoWindow(hInstance, hPreInst, lpszCmdLine, nCmdShow);
}