//**************************************************************************************//
//
//�@GP�@���C�u����
//		gplib.h
//**************************************************************************************//
//**************************************************************************************//
//�C���N���[�h
//**************************************************************************************//
#pragma once
#define _CRT_SECURE_NO_DEPRECATE 1
#pragma warning(disable : 4995)	

//DirectSound DorectShow���g��Ȃ��ꍇ�̓R�����g�A�E�g���邱��
#define USE_DIRECTSOUND
#define USE_DIRECTSHOW

#pragma	comment(lib,"dxguid.lib")
#pragma	comment(lib,"d3dxof.lib")
#pragma	comment(lib,"dxguid.lib")
#pragma	comment(lib,"d3d9.lib")
#pragma	comment(lib,"d3dx9.lib")
#pragma	comment(lib,"winmm")
#pragma	comment(lib,"dinput8.lib")

#ifdef USE_DIRECTSHOW
#	pragma comment(lib,"strmiids")
#endif

#ifdef USE_DIRECTSOUND
#	pragma	comment(lib,"dsound.lib")
#endif

#include <d3dx9.h>
#include <dxerr.h>

#ifdef USE_DIRECTSOUND
#	include <dsound.h>
#endif

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#ifdef USE_DIRECTSHOW
#	include <dshow.h>
#endif

#include <crtdbg.h>
#include <time.h>
#include <assert.h> 


#define _USE_MATH_DEFINES
#include <cmath>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <tuple>
#include <random>
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <map>


//**************************************************************************************//
//�}�N���@�ϐ��@�錾
//**************************************************************************************//
 
//VisualStudo2012�ȑO�ɂ� nullptr �������̂Œ�`
#if (_MSC_VER<1700)
  const class {
  public:
      template<class T>
      operator T*() const { return 0; }
      // ������^�̃����o�|�C���^�ւ̕ϊ�
      template<class C, class T>
      operator T C::*() const  { return 0; }
  private:
      void operator&() const;
  } nullptr;
#endif

  namespace gplib{

		//�L���X�g�֘A
		template<typename T1, typename T2>
		inline std::shared_ptr<T1> weak_to_shared(const std::weak_ptr<T2>& ptr)
		{
			return std::static_pointer_cast<T1>(ptr.lock());
		}
		template<typename T>
		inline std::shared_ptr<T> weak_to_shared(const std::weak_ptr<T>& ptr)
		{
			return std::static_pointer_cast<T>(ptr.lock());
		}
		template<typename T1, typename T2>
		inline std::shared_ptr<T1> shared_cast(const std::shared_ptr<T2>& ptr)
		{
			return std::static_pointer_cast<T1>(ptr);
		}

		namespace input{
			//KEY
			enum KEYSTATE{ FREE_KEY, PUSH_KEY, HOLD_KEY, PULL_KEY };

			enum JOYID{
				JOY1,
				JOY2
			};
			enum JOY4DIR
			{
				JOY_DOWN = 0x1000,
				JOY_LEFT = 0x1001,
				JOY_UP = 0x1002,
				JOY_RIGHT = 0x1004
			};

			//�L�[�`�F�b�N
			//�L�[�ɖ��O������
			enum CODE_KEY
			{
				KEY_NEUTRAL = -1,	//�j���[�g�������
				KEY_DOWN,			//���L�[
				KEY_LEFT,			//���L�[
				KEY_UP,				//��L�[
				KEY_RIGHT,			//�E�L�[
				KEY_BTN0,			//�{�^��0
				KEY_BTN1,			//�{�^��1
				KEY_BTN2,			//�{�^��2
				KEY_SPACE,			//�X�y�[�X�L�[
				KEY_F1,				//F1�L�[
				KEY_F2,				//F2�L�[
				KEY_BTNMAX,			//�L�[�{�[�h�̍ő吔�@�L�[�ǉ��̍ۂ͂��̏�ɋL�q
				KEY_MOUSE_LBTN = KEY_BTNMAX,//�}�E�X�̍��{�^��
				KEY_MOUSE_RBTN,		//�}�E�X�̉E�{�^��
				KEY_MAX				//�ő吔
			};
		}
	  //�f�o�b�O�֘A
	  //#define RELEASE
	  //�ƋL�q���邱�ƂŃf�o�b�N�p�̊֐��𖳌�������B
	  //�eR�t���̊֐��̓V�X�e���p�̂���
#ifdef _DEBUG
#define	TToM Dbg_TilteToMessage
#define	FToM Dbg_FileOut
#define	BToM Dbg_BoxToMessage
#define	SToM Dbg_DisplayToMessage
#define	SToMNC Dbg_DisplayToMessageNC
#define	DFPS Draw_ShowFps
#else
#define	TToM Dbg_ReleaseDammy
#define	FToM Dbg_ReleaseDammy
#define	BToM Dbg_ReleaseDammy
#define	SToM Dbg_ReleaseDammy
#define	SToMNC Dbg_ReleaseDammy
#define	DFPS Dbg_ReleaseDammy
#endif

#define	TToMR	Dbg_TilteToMessage
#define	FToMR	Dbg_FileOut
#define	BToMR	Dbg_BoxToMessage
#define	SToMR	Dbg_DisplayToMessage
#define	SToMNCR Dbg_DisplayToMessageNC

	  enum
	  {
		  NORMAL,	//�������`��
		  ADD,	//���Z����
		  HALFADD,	//�����Z����
		  MULTIPL,	//��Z����
		  SUBTRACT,	//���Z
		  BY,
		  BF_MAX
	  };

	  //�_�C�A���O�p
	  enum DLG_ERROR
	  {
		  DLG_ERROR_INVALIDBUFFER,	//get�̃|�C���^��NULL�ɂȂ��Ă���
		  DLG_ERROR_CANCEL,			//�L�����Z���������ꂽ
		  DLG_OK
	  };
	  namespace math{
		  //�O�p�֐��֌W
			const float		PAI = 3.1415926535897f;
			const float		PI	= 3.1415926535897f;
			const double	PI_D = 3.14159265358979323846;
	  }
	  //�z�C�[���֘A
	  enum WHEEL
	  {
		  MWHEEL_UP = -1,
		  MWHEEL_NEUTRAL,
		  MWHEEL_DOWN,
	  };

	  //**************************************************************************************//
	  //�O���ϐ�
	  //**************************************************************************************//
	  namespace system{
		  extern	HWND		hWnd;				//�E�C���h�E�n���h��
		  extern	const char	USERNAME[256];		//�^�C�g����
		  extern	const int 	WINW;				//WINDOW��
		  extern	const int		WINH;				//WINDOW����
		  extern	bool		WindowMode;			//�E�C���h�E���[�h
		  extern	float		FrameTime;			//�t���[���^�C��
		  //extern const int PLAYER_NUM;			//�@�v���C���[�̐�
		  const int	PLAYER_NUM = 2;

		  extern	int			KeyboardMode;			//�����L�[�{�[�h�t���O
			extern	int			nowStageNo;

			extern const int  REFRESHRATE;
			extern const float  ONEFRAME_TIME;
		}
	  //**************************************************************************************//
	  //extern �錾
	  //**************************************************************************************//
	  namespace system{
		  void GameLoop();
		  int  DoWindow(HINSTANCE hInstance, HINSTANCE hPreInst,
			  LPSTR lpszCmdLine, int nCmdShow);
		  void OnCreate();
		  void OnDestroy();
		  void InitDx();
		  void DelDx();
	  }
	  namespace camera{
		  //�J��������
		  void SetLookAt(float x, float y);
		  void StepCamera();
		  void InitCamera(float x, float y, float addpx, float addpy);
		  //void SetPos(float x, float y);
		  void AddCameraForce(float x, float y);
		  void SetCameraTarget(float x, float y, int speed);
#define SetLookAtTarget SetCameraTarget
		  void SetScale(float scale);
		  float GetScale();

		  void AddScale(float add);
		  void AddScale(float add, int step);
		  POINT GetCameraPos();
		  POINT GetLookAt();
		  bool InScreenCamera(int x, int y, int w, int h);
		  bool InScreenCamera(RECT rt);
		  bool InScreenCameraCenter(float x, float y, float w, float h);

		  POINT GetCursorPosition();
		  POINT GetCursorPositionNC();
		  void ChangeScreenMouse(POINT& pt);
		  void GetCursorPosition(POINT& pt);
		  void GetCursorPositionNC(POINT* pt_ptr);

		  RECT GetScreenRect();
	  }
	  namespace stage{
		  //�X�e�[�W�Ǘ��֘A
		  void ChangeStage(int);
		  bool InitStage();
		  int  CheckStage();
			void InitFlagStage();
	  }
	  namespace graph{

#define ARGB D3DCOLOR_ARGB
			//�`��֘A
		  void Draw_Clear();
		  void Draw_Refresh();

		  //�摜�֘A 
		  void Draw_LoadObject(const std::string& resname, const std::string& filename,
			  D3DCOLOR Transparent = ARGB(255, 0, 0, 0));
			void Draw_DeleteObject(const std::string& resname);
		  void Draw_SetRenderMode(int Mode);
		  void Draw_EndRenderMode();


			void Draw_Graphics(
				int x, int y, float z,
				const std::string& resname,
				int srcX, int srcY, int sizeX, int sizeY,
				int	degree = 0,
				float	scaleX = 1.0f, float scaleY = 1.0f,
				u_char a = 255,
				u_char r = 255,
				u_char g = 255,
				u_char b = 255
				);
			void Draw_Graphics(
				float x, float y, float z,
				const std::string& resname,
				int srcX, int srcY, int sizeX, int sizeY,
				int	degree = 0,
				float	scaleX = 1.0f, float scaleY = 1.0f,
				u_char a = 255,
				u_char r = 255,
				u_char g = 255,
				u_char b = 255
				);
				void Draw_GraphicsLeftTop(
			  int x, int y, float z,
			  const std::string& resname,
			  int srcX, int srcY,
			  int sizeX, int sizeY,
			  int	degree = 0, POINT *center = nullptr,
			  float	scaleX = 1.0f, float scaleY = 1.0f,
			  u_char a = 255,
			  u_char r = 255,
			  u_char g = 255,
			  u_char b = 255
			  );

		  void Draw_GraphicsLeftTop(
			  float x, float y, float z,
			  const std::string& resname,
			  int srcX, int srcY,
			  int sizeX, int sizeY,
			  int	degree = 0, POINT *center = nullptr,
			  float	scaleX = 1.0f, float scaleY = 1.0f,
			  u_char a = 255,
			  u_char r = 255,
			  u_char g = 255,
			  u_char b = 255
			  );
		  void Draw_Line(
			  int startX, int startY,
			  int endX, int endY,
			  float z, D3DCOLOR color, int size
			  );
		  void Draw_GraphicsNC(
			  int x, int y, float z,
			  const std::string& resname,
			  int srcX, int srcY,
			  int sizeX, int sizeY,
			  int	degree = 0, POINT *center = nullptr,
			  float	scaleX = 1.0f, float scaleY = 1.0f,
			  u_char a = 255,
			  u_char r = 255,
			  u_char g = 255,
			  u_char b = 255
			  );
		  void Draw_GraphicsLeftTopNC(
			  int x, int y, float z,
			  const std::string& resname,
			  int srcX, int srcY,
			  int sizeX, int sizeY,
			  int	degree = 0, POINT *center = nullptr,
			  float	scaleX = 1.0f, float scaleY = 1.0f,
			  u_char a = 255,
			  u_char r = 255,
			  u_char g = 255,
			  u_char b = 255
			  );
		  void Draw_GraphicsNC(
			  float x, float y, float z,
			  const std::string& resname,
			  int srcX, int srcY,
			  int sizeX, int sizeY,
			  int	degree = 0, POINT *center = nullptr,
			  float	scaleX = 1.0f, float scaleY = 1.0f,
			  u_char a = 255,
			  u_char r = 255,
			  u_char g = 255,
			  u_char b = 255
			  );
		  void Draw_GraphicsLeftTopNC(
			  float x, float y, float z,
			  const std::string& resname,
			  int srcX, int srcY,
			  int sizeX, int sizeY,
			  int	degree = 0, POINT *center = nullptr,
			  float	scaleX = 1.0f, float scaleY = 1.0f,
			  u_char a = 255,
			  u_char r = 255,
			  u_char g = 255,
			  u_char b = 255
			  );
		  void Draw_Box(
			  int left, int top, int right, int bottom,
			  float z,
			  D3DCOLOR fillColor, D3DCOLOR frameColor,
			  int lineSize, bool nakanuri
			  );
		  void Draw_BoxCenter(
			  int x, int y, int sizeX, int sizeY,
			  float z,
			  D3DCOLOR fillColor, D3DCOLOR frameColor,
			  int lineSize, bool nakanuri
			  );
		  void Draw_BoxNC(
			  int left, int top, int right, int bottom,
			  float z,
			  D3DCOLOR fillColor, D3DCOLOR frameColor,
			  int lineSize, bool nakanuri
			  );
		  void Draw_BoxCenterNC(
			  int left, int top, int right, int bottom,
			  float z,
			  D3DCOLOR fillColor, D3DCOLOR frameColor,
			  int lineSize, bool nakanuri
			  );
		  void Draw_LineNC(
			  int startX, int startY,
			  int endX, int endY,
			  float z, D3DCOLOR color, int size
			  );

		  void Draw_2DClear();
		  void Draw_3DClear();
		  void Draw_2DRefresh();
		  bool Draw_3DRefresh();

		  void Draw_CkRect(const RECT& rt, D3DCOLOR color = ARGB(255, 255, 0, 0));
		  void Draw_CkRectNC(const RECT& rt, D3DCOLOR color = ARGB(255, 255, 0, 0));
		  D3DSURFACE_DESC Draw_GetImageSize(const std::string& resname);
		  SIZE Draw_GetImageSize2(const std::string& resname);
		  unsigned int Draw_GetImageWidth(const std::string& resname);
		  unsigned int Draw_GetImageHeight(const std::string& resname);

	  }
	  namespace font{
		  //�t�H���g�֘A	
		  void Draw_FontText(int x, int y, float z, const std::string& msg, D3DCOLOR color, int fontID);
		  void Draw_FontTextNC(int x, int y, float z, const std::string& msg, D3DCOLOR color, int fontID);
		  int  Draw_FontText(const RECT& rt, float z, const std::string& msg, D3DCOLOR color, int fontID);
		  int  Draw_FontTextNC(const RECT& rt, float z, const std::string& msg, D3DCOLOR color, int fontID);
		  void Draw_TextXY(int x, int y, const std::string& msg, D3DCOLOR Textcolor, int fontID = 0);
		  void Draw_TextXYNC(int x, int y, const std::string& msg, D3DCOLOR Textcolor, int fontID = 0);
		  void Draw_CreateFont(int Num, int size, LPCTSTR	fontname);
		  void Draw_CreateFontItalic(int Num, int size, LPCTSTR	fontname);
	  }
	  namespace input{
		  WHEEL GetWheelRoll();
		  int GetWheelDelta();
		  //���͊֘A
		  void CheckKey();
		  bool CheckPress(int keyno, int interval = 1, int player = 0);	//������Ă��邩�`�F�b�N
		  bool CheckAnyPress(int keyno);
		  bool CheckFree(int keyno, int interval = 1, int player = 0);		//�͂Ȃ���Ă��邩�`�F�b�N
		  bool CheckPush(int keyno, int player = 0);		//�����������`�F�b�N
		  bool CheckAnyPush(int keyno);
		  bool CheckPull(int keyno, int player = 0);		//�͂Ȃ��������`�F�b�N
		  int  GetKeyStatus(int keyno, int player = 0);	//�L�[�̏�Ԃ��`�F�b�N
		  int  GetKeyFrameHold(int keyno, int player = 0);	//������Ă���t���[�������擾
		  int  GetKeyFrameFree(int keyno, int player = 0);	//�͂Ȃ���Ă���t���[�������擾
		  void InitKeyInfo(int player = 0);				//�X�e�[�^�X�ƃt���[����������
		  void Key_GetKey(unsigned int Code, int* Flag);
		  void SetKeyFormat(CODE_KEY key, DWORD keycode, int playernum);
		  void SetKeysFormat(DWORD* keycode, int playernum);
		  bool IsUsePad(int playerID);

		  POINT GetMousePosition();				//�}�E�X�J�[�\���̍��W���擾
		  void  GetMousePosition(POINT* pt_ptr);//�}�E�X�J�[�\���̍��W���擾
	  }
	  namespace debug{
		  //debug�֘A
		  void Dbg_FileOut(char *str, ...);
		  void Dbg_ExitApp();//�A�v���P�[�V�������I��������v����WIndows�ɑ���܂��i���������[�N�ɒ��ӁI�j
		  void Dbg_DisplayToMessage(int x, int y, char * str, ...);
		  void Dbg_TilteToMessage(char * str, ...);
		  void Dbg_BoxToMessage(char *str, ...);
		  void Dbg_StringOutXY(int x, int y, char* str, ...);
		  void Draw_ShowFps();
		  DLG_ERROR	Dbg_InputDialog(char* get, int size, const char* title, const char* label);
		  void Dbg_DisplayToMessageNC(int x, int y, char * str, ...);
			void Dbg_ReleaseDammy(char * str, ...);
			void Dbg_ReleaseDammy(int x, int y, char * str, ...);
			void Dbg_ReleaseDammy();
		}
	  namespace bgm{
#ifdef USE_DIRECTSHOW
		  typedef int DSHOWOBJ;
		  //BGM�Đ��֐�
		  void DShow_Init();
		  void DShow_Del();
		  void DShow_LoadFile(const std::string& resname, const std::string& filename);
		  bool DShow_Play(const std::string& resname);
		  void DShow_RateControl(const std::string& resname, float rate);
		  void DShow_EndCheck();
		  void DShow_Stop(const std::string& resname);
		  void DShow_AllStop();
		  LONGLONG DShow_GetCurrentPos(const std::string& resname);
		  void DShow_SetStartPos(const std::string& resname);
		  void DShow_VolumeControl(const std::string& resname, int volume);
			void DShow_Pause(const std::string& resname);
			LONGLONG DShow_GetEndPosition(const std::string& resname);
#endif
	  }
	  namespace se{
#ifdef USE_DIRECTSOUND
		  typedef int DSOUNDOBJ;
		  //SE�Đ��֐�
		  void DSound_Init();
		  bool DSound_Create();
		  void DSound_CreateSecondaryBuffer();
		  bool DSound_CreatePrimaryBuffer();
		  bool DSound_Del();
		  void DSound_Play(const std::string& resname);
		  void DSound_PlayLoop(const std::string& resname);
		  bool DSound_Del();
		  void DSound_LoadFile(const std::string& resname, const std::string& filename);
		  void DSound_AllStop();
		  void DSound_Stop(const std::string& resname);
		  void DSound_EndCheck();
		  void DSound_SetFrequency(const std::string& resname, int Fre);
		  void DSound_SetVolume(const std::string& resname, int Vol);
#endif
	  }
	  namespace math{
		  //���w�֘A
		  bool RectCheck(const RECT& rt1, const RECT& rt2);
		  bool RectCheck(const RECT *rt1, const RECT *rt2);
		  bool IsFrameOut_Center(float x, float y, float width, float height);
		  bool IsFrameOut_LeftTop(float x, float y, float width, float height);

		  float Calc_XYToRad(int x, int y);		//���WXY�ւ̊p�x�����߂�
		  float Calc_XYToSinCos(int x, int y, float *addx, float *addy);	//���WXY�ւ̈ړ���XY�����߂�
		  float Calc_RadToDegree(float rad);		//���W�A������p�x��
		  float Calc_DegreeToRad(float degree);		//�p�x���烉�W�A����

			extern std::mt19937 engine;
			// [min, max]�̈�l�ȗ������擾
			template <typename T>
			inline T GetRandom(const T min, const T max) {
				std::uniform_real_distribution<T> range(min, max);
				return range(engine);
			}
			// ���ꉻ�͂������ɏ����c
			template <>
			inline int GetRandom<int>(const int min, const int max) {
				std::uniform_int_distribution<> range(min, max);
				return range(engine);
			}

			void RandomInit();

		  //�_�Ɠ_�̊p�x�����߂�i-180~180�j
		  float RadianOfPoints2(float basisX, float basisY, float targetX, float targetY);
		  float DegreeOfPoints2(float basisX, float basisY, float targetX, float targetY);
		  //��_�Ԃ̋��������Ƃ߂�
		  float Distance2(float x1, float y1, float x2, float y2);

			float ROUND_X(float angle, float length, float center = 0.f);
			float ROUND_Y(float angle, float length, float center = 0.f);
			float ROUND_Z(float angle, float length, float center = 0.f);

			void reverseRect(RECT& baseRect, int width);
	  }
	  namespace time{
		  //�����x�^�C�}�[�֘A
		  void  Time_ResetTimer(int id);
		  void  Time_StartTimer(int id);
		  bool  Time_MarkOfOneSec(int id);
		  bool  Time_MarkOfTime(int id, float marktime);
		  float Time_GetTimerCount();
		  float Time_GetTimerCount(int id);
		  float Time_Update();
		  float Time_GetOneFrameCount();
		  void  Time_StartTimerCount();
		  bool  Time_CheckTimer();
	  }
	  ////�g���@�\���g��Ȃ��ꍇ�̓R�����g�A�E�g���邱��
	  //#include "ci_ext/ci_ext.hpp"
  }
#ifdef CI_EXT
bool IsFrameOut_Center(const ci_ext::Vec3f& pos, const ci_ext::Vec3f& size);
bool IsFrameOut_LeftTop(const ci_ext::Vec3f& pos, const ci_ext::Vec3f& size);
#endif
