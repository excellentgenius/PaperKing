//**************************************************************************************//
//
//�@GP�@���C�u����
//		gplib.cpp
//**************************************************************************************//
//**************************************************************************************//
//**************************************************************************************//
//inlude
//**************************************************************************************//
#include	"GpLib.h"

using namespace gplib;
//**************************************************************************************//
//�}�N���@�ϐ��@�錾
//**************************************************************************************//
//�E�C���h�E�n���h��
namespace gplib{
	namespace system{
		HWND		hWnd;
		float		FrameTime;
		const int  REFRESHRATE = 60;
		const float  ONEFRAME_TIME = (1.0f / static_cast<float>(REFRESHRATE));
	}
	namespace math{
		std::mt19937 engine;
	}

	//�}�E�X�֘A�擾
	//�z�C�[���l�擾
	namespace input{
		int delta;
	}
	//�X�e�[�W�ԍ�
	namespace system{
		int	nowStageNo;
	}
}
//�����ϐ��錾
#define		RELEASE(X)		{if(X!=NULL)	{X->Release();X = NULL;}}
//D3Draw �֘A
LPDIRECT3D9				pD3D;
D3DPRESENT_PARAMETERS	D3DPP;
LPDIRECT3DDEVICE9		pD3DDevice;
LPD3DXSPRITE			pSprite;
LPD3DXFONT				pD3DXFont;
D3DDISPLAYMODE			d3ddm;
//�C���[�W�Ǘ��\����
struct ImgObj final
{
	LPDIRECT3DTEXTURE9 tex;
	std::string filename;
	int color;
	D3DXIMAGE_INFO info;
	void init()
	{
		tex = nullptr;
		filename = "";
		color = 0;
		D3DXIMAGE_INFO temp = {};
		info = temp;
	}
	ImgObj()
	{
		init();
	}
};
//unordered_map�ɂ�郊�\�[�X�Ǘ�
//"���\�[�X��"�ɂ��Ǘ����s���B
#include "unordered_map"
std::unordered_map< std::string, ImgObj>	ImgTable;

//FONT
const int DDFONTMAX = 10;
LPD3DXFONT			pFontData[DDFONTMAX];

//-------------------------------------------------------
//BGM�Đ��֘A
#ifdef USE_DIRECTSHOW
struct DxShow{
	IGraphBuilder	*pBuilder;
	IMediaControl	*pMediaControl;
	IMediaEvent		*pMediaEvent;
	IMediaSeeking	*pMediaSeeking;
	IBasicAudio		*pBasicAudio;

	int				use;
};
//BGM�Ǘ��\����
struct BgmObj
{
	DxShow				bgm;
	std::string		filename;
};
std::unordered_map< std::string, BgmObj>	BgmTable;

#endif
//-------------------------------------------------------
//SE�Đ��֘A
#ifdef USE_DIRECTSOUND
// DirectSound8
LPDIRECTSOUND8          lpDS;   
// �v���C�}���T�E���h�o�b�t�@
LPDIRECTSOUNDBUFFER		lpPrimary;

//���������ł���ő吔	
enum					{DSNONE,DSSTART,DSPLAY,DSSTOP,DSEND};
struct PlayBuffer{
	LPDIRECTSOUNDBUFFER     pBuffer;		// �v���C�}���T�E���h�o�b�t�@
	int						ID;				//�x�[�X�ɂȂ����o�b�t�@��ID
	WORD					BufferSize;		//�x�[�X�ɂȂ����o�b�t�@�̃T�C�Y
	int						State;			//�o�b�t�@�̏��
	bool					Loop;			//���[�v�t���O
};

const int				DSPLAYMAX = 10;
struct SoundBuffer{
	LPDIRECTSOUNDBUFFER     sBuffer;	//�Z�J���_���o�b�t�@
	WORD										BufferSize;		//�o�b�t�@�̃T�C�Y
	DWORD										Fre;
	struct PlayBuffer	PlayBuffer[DSPLAYMAX];
};
struct SeObj
{
	SoundBuffer		se;
	std::string		filename;
};

std::unordered_map< std::string, SeObj>		SeTable;

#endif





//**************************************************************************************//
//�֐�																					
//**************************************************************************************//
//======================================================================================//
//�֐���`
//======================================================================================//

//�z�C�[���̏�Ԃ�߂�
int input::GetWheelDelta()
{
	return input:: delta;
}

//�z�C�[���̏�Ԃ�߂�
WHEEL input::GetWheelRoll()
{
	if (input::delta < 0){
		return MWHEEL_UP;
	}
	else if (input::delta > 0){
		return MWHEEL_DOWN;
	}
	return MWHEEL_NEUTRAL;
}

//�f���^�l�̃N���A
void ClearWheelDelta()
{
	input::delta = 0;
}
namespace gplib{
	namespace camera{

		//********************************************************************//
		//
		//				�X�N���[���@�֘A�֐�
		//
		//********************************************************************//
		class Camera
		{
			float cx_, cy_;		 //��ʒ���
			float addPx_, addPy_;//1�t���[��������̈ړ���
			float tx_, ty_;		 //�����_�؂�ւ����ڕW
			int	  moveStep_;	 //�����_�؂�ւ��������� �傫���قǒx���Ȃ�
			float scale_;		 //�g��k����
			float addScale_;	 //�g�嗦���Z�l
			int	  scalingStep_;	 //�g�嗦�ω��l
			float tScale_;		 //�ړI�g�嗦

			float sx(float x) const { return (x - viewLeft()) * scale_; }
			float sy(float y) const { return (y - viewTop()) * scale_; }
			float offsetX() const   { return system::WINW / 2.0f / scale_; };
			float offsetY() const   { return system::WINH / 2.0f / scale_; };
		public:
			int  getMousePosX(int x) { return static_cast<int>((x - system::WINW / 2) / scale() + cx()); }
			int  getMousePosY(int y) { return static_cast<int>((y - system::WINH / 2) / scale() + cy()); }
			POINT getMousePos(const POINT& pt)
			{
				POINT r = { getMousePosX(pt.x), getMousePosY(pt.y) };
				return r;
			}

			float viewLeft()   const { return cx_ - offsetX(); }
			float viewTop()    const { return cy_ - offsetY(); }
			float viewRight()  const { return cx_ + offsetX(); }
			float viewBottom() const { return cy_ + offsetY(); }
			int   iviewLeft()  const { return static_cast<int>(viewLeft()); }
			int   iviewTop()   const { return static_cast<int>(viewTop()); }
			int   iviewRight() const { return static_cast<int>(viewRight()); }
			int   iviewBottom()const { return static_cast<int>(viewBottom()); }

			Camera(float cx = system::WINW / 2, float cy = system::WINH / 2, float addpx = 0, float addpy = 0, float scale = 1.f)
				:
				cx_(cx),
				cy_(cy),
				addPx_(addpx),
				addPy_(addpy),
				scale_(scale)
			{
				tx_ = cx_;
				ty_ = cy_;
				tScale_ = scale_;
			}

			//---------------------------------------------------------------------------------------
			//�������W���E�C���h�E�̒��S�ɂȂ�悤�ɃJ�����ʒu��ύX����
			//---------------------------------------------------------------------------------------
			void setLookAt(float x, float y)
			{
				if (addPx_ == 0 && addPy_ == 0){
					tx_ = cx_ = (x);
					ty_ = cy_ = (y);
				}
			}
			//---------------------------------------------------------------------------------------
			//�������W���J�����ʒu�ɕύX����
			//---------------------------------------------------------------------------------------
			//void setPos(float x, float y)
			//{
			//	tx_ = cx_ = x + offsetX();
			//	ty_ = cy_ = y + offsetY();
			//}
			//---------------------------------------------------------------------------------------
			//�J���������i���t���[���ĂԂ��Ɓj
			//---------------------------------------------------------------------------------------
			void step()
			{
				//X���ړ�
				if (abs((int)cx_ - (int)tx_) >= 5){
					addPx_ = (tx_ - cx_) / moveStep_;
				}
				else if (abs((int)cx_ - (int)tx_) < 5){
					addPx_ = 0;
					tx_ = cx_;
				}
				else{
					addPx_ = 0;
					tx_ = cx_;
				}
				//Y���ړ�
				if (abs((int)cy_ - (int)ty_) >= 5){
					addPy_ = (ty_ - cy_) / moveStep_;
				}
				else if (abs((int)cy_ - (int)ty_) < 5){
					addPy_ = 0;
					ty_ = cy_;
				}
				else{
					addPy_ = 0;
					ty_ = cy_;
				}
				cx_ += addPx_;
				cy_ += addPy_;

				if (abs(addScale_) <= 0.005f){
					scalingStep_ = 0;
					addScale_ = 0;
				}
				else{
					addScale_ = (float)((tScale_ - scale_) / scalingStep_);
					scale_ += addScale_;
				}
			}
			//---------------------------------------------------------------------------------------
			//1�t���[��������̃J�����ړ��ʂ�ݒ�
			//---------------------------------------------------------------------------------------
			void addForce(float x, float y)
			{
				addPx_ = x;
				addPy_ = y;
			}
			//---------------------------------------------------------------------------------------
			//�w��t���[�����̃J�����ړ��ʂ�ݒ�
			//---------------------------------------------------------------------------------------
			void setTagetLength(float x, float y, int step)
			{
				tx_ = x;
				ty_ = y;
				moveStep_ = step;
				addForce((tx_ - cx_) / moveStep_, (ty_ - cy_) / moveStep_);
			}
			//---------------------------------------------------------------------------------------
			//�g�嗦�ύX
			//---------------------------------------------------------------------------------------
			void setScale(float scale)
			{
				scale_ = scale;
			}
			void addScale(float offset)
			{
				scale_ += offset;
			}
			void addScaleSteps(float newScale, int step)
			{
				scalingStep_ = step;
				tScale_ = newScale;
				addScale_ = (float)((newScale - scale_) / step);
			}

			float cx()  const { return cx_; }
			float cy()  const { return cy_; }
			int   icx() const { return static_cast<int>(cx_); }
			int   icy() const { return static_cast<int>(cy_); }
			float scale() const { return scale_; }
			RECT getCameraViewRt() const
			{
				RECT rc = { iviewLeft(), iviewTop(), iviewRight(), iviewBottom() };
				return rc;
			}
			//���̓X�P�[�����A�J�������l�������X�P�[���ɕϊ�
			float changeScale(float s) const { return s * scale_; }

			//���͍��W�ɑ΂��āA�E�C���h�E�̍��W(float)�ɕϊ�
			float xchange(float x) const { return sx(x); }
			float ychange(float y) const { return sy(y); }
			float xchange(int x)   const { return sx(static_cast<float>(x)); }
			float ychange(int y)   const { return sy(static_cast<float>(y)); }

			//���͍��W�ɑ΂��āA�E�C���h�E�̍��W(int)�ɕϊ�
			int   ixchange(float x) const { return static_cast<int>(sx(x)); }
			int   iychange(float y) const { return static_cast<int>(sy(y)); }
			int   ixchange(int x)   const { return static_cast<int>(sx(static_cast<float>(x))); }
			int   iychange(int y)   const { return static_cast<int>(sy(static_cast<float>(y))); }
		}*camera;
	}
}
//---------------------------------------------------------------------------------------
//�J����������
//---------------------------------------------------------------------------------------
void camera::InitCamera(float x, float y, float addpx, float addpy)
{
	static Camera c(x, y, addpx, addpy);
	camera = &c;
}
//---------------------------------------------------------------------------------------
//�������p�@�J��������
//---------------------------------------------------------------------------------------
void camera::StepCamera()
{
	camera->step();
}

//---------------------------------------------------------------------------------------
//�J�������_�w��
//---------------------------------------------------------------------------------------
void camera::SetLookAt(float x, float y)
{
	camera->setLookAt(x, y);
}

//---------------------------------------------------------------------------------------
//�J��������_�w��@���@��{�I��SetLookAt�Ŗ��Ȃ�
//---------------------------------------------------------------------------------------
//void SetPos(float x, float y)
//{
//	camera->setPos(x, y);
//}

//---------------------------------------------------------------------------------------
//�P�t���[��������̃J�����̎��_�ړ��ʂ��w��
//---------------------------------------------------------------------------------------
void camera::AddCameraForce(float x, float y)
{
	camera->addForce(x, y);
}

//---------------------------------------------------------------------------------------
//�w�肵�����x�Ŏw�肵�����_�܂ŃJ�����ړ�
//---------------------------------------------------------------------------------------
void camera::SetCameraTarget(float x, float y, int speed)
{
	camera->setTagetLength(x,y,speed);
}
//---------------------------------------------------------------------------------------
//�J�����{���w��
//---------------------------------------------------------------------------------------
void camera::SetScale(float scale)
{
	camera->setScale(scale);
}

//---------------------------------------------------------------------------------------
//�J�����{���擾�@���@���܂藘�p���Ȃ�
//---------------------------------------------------------------------------------------
float camera::GetScale()
{
	return camera->scale();
}

//---------------------------------------------------------------------------------------
//�J�����̔{�����w�肳�ꂽ�l���Z��������
//---------------------------------------------------------------------------------------
void camera::AddScale(float add)
{
	camera->addScaleSteps(add,0);
}

//---------------------------------------------------------------------------------------
//�w�肳�ꂽ���x�ŁA�w�肳�ꂽ�{���܂Ŋg��k��
//---------------------------------------------------------------------------------------
void camera::AddScale(float add, int step)
{
	camera->addScaleSteps(add,step);
}

POINT camera::GetLookAt()
{
	POINT pt = {camera->icx(),camera->icy()};
	return pt;
}
POINT camera::GetCameraPos()
{
	POINT pt = {camera->iviewLeft(),camera->iviewTop()};
	return pt;
}
//---------------------------------------------------------------------------------------
//�ϊ��ς݂̃J�������W��߂�
//---------------------------------------------------------------------------------------
POINT camera::GetCursorPosition()
{
	return camera->getMousePos(input::GetMousePosition()); 
}

//---------------------------------------------------------------------------------------
//�J�[�\�����W��߂�
//---------------------------------------------------------------------------------------
POINT camera::GetCursorPositionNC()
{
	POINT pt = input::GetMousePosition();
	return pt;
}

//---------------------------------------------------------------------------------------
//�}�E�X�̍��W���J�������W�ɕϊ�
//---------------------------------------------------------------------------------------
void camera::ChangeScreenMouse(POINT& pt)
{
	pt = camera->getMousePos(pt); 
}

//---------------------------------------------------------------------------------------
//�ϊ��ς݂̃J�������W��߂�
//---------------------------------------------------------------------------------------
void camera::GetCursorPosition(POINT& pt)
{
	pt = camera->getMousePos(input::GetMousePosition()); 
}

//---------------------------------------------------------------------------------------
//�J�[�\�����W��߂�
//---------------------------------------------------------------------------------------
void camera::GetCursorPositionNC(POINT* pt_ptr)
{
	input::GetMousePosition(pt_ptr);
}


RECT camera::GetScreenRect()
{
	RECT cameraRt = camera->getCameraViewRt();
//	debug::TToM("%d:%d:%d:%d",cameraRt.left,cameraRt.top,cameraRt.right,cameraRt.bottom);
	return cameraRt;
}

//---------------------------------------------------------------------------------------
//�w�肳�ꂽ��`���`��Ώۗ̈�Ɋ܂܂�邩���ׂ�
//---------------------------------------------------------------------------------------
bool camera::InScreenCamera(RECT rt)
{
	RECT ObjRect = {
		(rt.left), (rt.top),
		(rt.right), (rt.bottom)
	};
	if(math::RectCheck(&ObjRect,&GetScreenRect())){
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------
//�w�肳�ꂽ���W�̒��S���`��Ώۗ̈�Ɋ܂܂�邩���ׂ�
//---------------------------------------------------------------------------------------
bool camera::InScreenCameraCenter(float x, float y, float w, float h)
{
	RECT ObjRect = {
		static_cast<LONG>(x - w / 2.0f),
		static_cast<LONG>(y - h / 2.0f),
		static_cast<LONG>(x + w / 2.0f),
		static_cast<LONG>(y + h / 2.0f)
	};
	return math::RectCheck(&ObjRect,&GetScreenRect());
}

//---------------------------------------------------------------------------------------
//�w�肳�ꂽ���W���`��Ώۗ̈�Ɋ܂܂�邩���ׂ�
//---------------------------------------------------------------------------------------
bool camera::InScreenCamera(int x, int y, int w, int h)
{
	RECT ObjRect = {
		x, y,
		x+w, y+h
	};

	if (math::RectCheck(&ObjRect, &GetScreenRect())){
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------
//�����_�����p
//---------------------------------------------------------------------------------------
//int i = math::GetRandom(10,100); //10����100�܂ł̃����_���Ȑ���
//float f = math::GetRandom(10.0f,100.0f); //10.0f����100�܂ł̃����_���Ȏ���
//---------------------------------------------------------------------------------------
//// [min, max]�̈�l�ȗ������擾
//template <typename T>
//inline T GetRandom(const T min, const T max) {
//	std::uniform_real_distribution<T> range(min, max);
//	return range(engine);
//}
//// ���ꉻ�͂������ɏ����c
//template <>
//inline int GetRandom<int>(const int min, const int max) {
//	std::uniform_int_distribution<> range(min, max);
//	return range(engine);
//}

//---------------------------------------------------------------------------------------
//�����_���̏������F�������p
//---------------------------------------------------------------------------------------
void math::RandomInit() {
	std::random_device device;
	engine.seed(device());
}

//---------------------------------------------------------------------------------------
//�Q�_�Ԃ̊p�����߂�F�ʓx�@
//---------------------------------------------------------------------------------------
float math::RadianOfPoints2(float basisX, float basisY, float targetX, float targetY)
{
	float x =  targetX - basisX;
	float y = -(targetY - basisY);  //�f�B�X�v���C���W�ɍ��킹�邽�߂ɁA�}�C�i�X�ɂ���
	return atan2(y, x);
}

//---------------------------------------------------------------------------------------
//�Q�_�Ԃ̊p�x�����߂�F�x���@
//---------------------------------------------------------------------------------------
float math::DegreeOfPoints2(float basisX, float basisY, float targetX, float targetY)
{
	return Calc_RadToDegree(RadianOfPoints2(basisX, basisY, targetX, targetY));
}

//---------------------------------------------------------------------------------------
//�Q�_�Ԃ̋��������߂�
//---------------------------------------------------------------------------------------
float math::Distance2(float x1, float y1, float x2, float y2)
{
  float x = pow(x1 - x2, 2);
  float y = pow(y1 - y2, 2);
  return sqrt(x + y);
}

//********************************************************************//
//
//				�����x�^�C�}�[�@�֘A�֐�
//
//********************************************************************//
const int	_TIMER_MAX = 10;
//�o�ߎ��Ԋϑ��p
LARGE_INTEGER _start[_TIMER_MAX];
LARGE_INTEGER _systimer;
LARGE_INTEGER _now;
LARGE_INTEGER _sys;
LARGE_INTEGER _lastFrame;

//---------------------------------------------------------------------------------------
//�@�o�ߎ��Ԃ̃��Z�b�g
//========================================
//	//��b���ɐ^��Ԃ�
//	if(Time_MarkOfOneSec(0)){
//		����
//		Time_ResetTimer(0);
//	}
//	//�T�b���ɐ^��Ԃ�
//	if(Time_MarkOfTime(0,5)){
//		����
//		Time_ResetTimer(0);
//	}
//	Time_Update();
//---------------------------------------------------------------------------------------
void time::Time_ResetTimer(int id)
{
	QueryPerformanceCounter( &_start[id] );
}
//---------------------------------------------------------------------------------------
//�@�o�ߎ��Ԃ̌v���J�n
//---------------------------------------------------------------------------------------
void time::Time_StartTimer(int id)
{
	QueryPerformanceCounter( &_start[id] );
}

//---------------------------------------------------------------------------------------
//�@��b�o�߃}�[�J�[
//---------------------------------------------------------------------------------------
bool time::Time_MarkOfOneSec(int id)
{
	return (static_cast<float>(_now.QuadPart - _start[id].QuadPart) / static_cast<float>(_sys.QuadPart) >= 1.0f);
}

//---------------------------------------------------------------------------------------
//�@���Ԍo�߃}�[�J�[
//---------------------------------------------------------------------------------------
bool time::Time_MarkOfTime(int id, float marktime)
{
	return (static_cast<float>(_now.QuadPart - _start[id].QuadPart) / static_cast<float>(_sys.QuadPart) >= marktime);
}

//---------------------------------------------------------------------------------------
//�@�o�ߎ��Ԃ̎擾
//---------------------------------------------------------------------------------------
float time::Time_GetTimerCount(int id)
{
	return  static_cast<float>(_now.QuadPart - _start[id].QuadPart) / static_cast<float>(_sys.QuadPart);
}

//---------------------------------------------------------------------------------------
//�@�o�ߎ��Ԃ̎擾
//---------------------------------------------------------------------------------------
float time::Time_GetTimerCount()
{
	return  static_cast<float>(_now.QuadPart - _systimer.QuadPart) / static_cast<float>(_sys.QuadPart);
}


//---------------------------------------------------------------------------------------
//	�^�C�}�[�̍X�V	1Frame�ɂ����̌Ăяo��
//	�������p
//---------------------------------------------------------------------------------------
float time::Time_Update()
{
	QueryPerformanceCounter( &_now );
	system::FrameTime = static_cast<float>(_now.QuadPart - _lastFrame.QuadPart) / static_cast<float>(_sys.QuadPart);
	QueryPerformanceCounter( &_lastFrame );
	return system::FrameTime;
}
//---------------------------------------------------------------------------------------
//�@�P�t���[��������̌o�ߎ���
//	static float cnt;
//  cnt += TimeGetOneFrameCount();
//	if((int)cnt >= 1) //1�b�o��
//---------------------------------------------------------------------------------------
float time::Time_GetOneFrameCount()
{
	return system::FrameTime;
}

//---------------------------------------------------------------------------------------
//�@�o�ߎ��Ԃ̎擾�J�n
//---------------------------------------------------------------------------------------
void time::Time_StartTimerCount()
{
	QueryPerformanceCounter( &_systimer );
}

//---------------------------------------------------------------------------------------
//�@�����x�^�C�}�[�����p�\�����ׂ�
//  �������Z�b�g����
//---------------------------------------------------------------------------------------
bool time::Time_CheckTimer()
{
	if( !QueryPerformanceFrequency(&_sys) )
		return false;
	QueryPerformanceCounter( &_lastFrame );
	time::Time_StartTimerCount();
	time::Time_Update();
	return true;
}
//********************************************************************//
//
//				�f�o�b�O�@�֘A�֐�
//
//********************************************************************//
//---------------------------------------------------------------------------------------
//�������p�����[�X���̃f�o�b�O�֐��̃_�~�[�֐�
//---------------------------------------------------------------------------------------
void debug::Dbg_ReleaseDammy(char * str, ...)
{
}

void debug::Dbg_ReleaseDammy(int x, int y, char * str, ...)
{
}

void debug::Dbg_ReleaseDammy()
{
}

//--------------------------------------------------------------------------------------------
// FPS�̕\��
//--------------------------------------------------------------------------------------------
//�@DD_Clear();
//	DD_ShowFPS();
//	DFPS();		�ł���
//�@DD_Refresh();
//--------------------------------------------------------------------------------------------
void debug::Draw_ShowFps()
{
	static double FpsTime = (time::Time_GetTimerCount() + 1.0);
	static double FpsData = 0.0;
	static int FpsCnt = 1;
	if (time::Time_GetTimerCount() >= (unsigned)FpsTime){
		FpsData=FpsCnt;
		FpsCnt = 0;
		FpsTime = (time::Time_GetTimerCount() + 1.0);
	}
	//FPS�J�E���g
	FpsCnt++;
	char Buffer[1024];	
	sprintf_s(Buffer,"fps=%3.2f:ft=%0.3f",FpsData,system::FrameTime);
	font::Draw_FontTextNC(0,0,0.0f,Buffer,ARGB(255,255,200,200),0);
}

//---------------------------------------------------------------------------------------
//�E�C���h�E�փ��b�Z�[�W�̕\��
//	����
//		x,y			�\���ʒu
//		str			�t�H�[�}�b�g�O������
//		...			�ό����w��
//---------------------------------------------------------------------------------------
void debug::Dbg_DisplayToMessage(int x, int y, char * str, ...){

	char buf[256];
	vsprintf(buf,str,(char*)(&str + 1));
	font::Draw_TextXY(x,y,
		buf,ARGB(255,255,0,0));
}

//---------------------------------------------------------------------------------------
//�E�C���h�E�փ��b�Z�[�W�̕\��
//	����
//		x,y			�\���ʒu
//		str			�t�H�[�}�b�g�O������
//		...			�ό����w��
//---------------------------------------------------------------------------------------
void debug::Dbg_DisplayToMessageNC(int x, int y, char * str, ...){

	char buf[256];
	vsprintf(buf,str,(char*)(&str + 1));
	font::Draw_TextXYNC(x, y,
		buf,ARGB(255,255,0,0));
}
//---------------------------------------------------------------------------------------
//�^�C�g���փ��b�Z�[�W�̕\��
//	����
//		str			�t�H�[�}�b�g�O������
//		...			�ό����w��
//---------------------------------------------------------------------------------------
void debug::Dbg_TilteToMessage(char * str, ...){

	char buf[256];
	vsprintf(buf,str,(char*)(&str + 1));
	SetWindowText(system::hWnd,(LPCSTR)buf);
}

//---------------------------------------------------------------------------------------
//���b�Z�[�W�{�b�N�X�ւ̕\��
//	����
//		str			�t�H�[�}�b�g�O������
//		...			�ό����w��
//---------------------------------------------------------------------------------------
void debug::Dbg_BoxToMessage(char *str, ...){

	char buf[256];
	vsprintf(buf,str,(char*)(&str + 1));
	MessageBox(NULL,(LPCSTR)buf,"Message",MB_OK);
}

//---------------------------------------------------------------------------------------
//�t�@�C��OUT�֐�
//	����
//		str			�t�H�[�}�b�g�O������
//		...			�ό����w��
//---------------------------------------------------------------------------------------
void debug::Dbg_FileOut(char *str, ...){
	
	FILE*	fp;
	char buff[128];
	vsprintf(buff,(char*)str,(char*)(&str+1));
	strcat(buff,"\n");
	if ((fp=fopen("debug.txt","at"))!=NULL){
		fprintf(fp,"%s",buff);
		fclose(fp);
	}
}

//---------------------------------------------------------------------------------------
//�A�v���P�[�V�������I��������v����WIndows�ɑ���܂��i���������[�N�ɒ��ӁI�j
//---------------------------------------------------------------------------------------
void debug::Dbg_ExitApp()
{
	DestroyWindow(system::hWnd);
}

//-------------------------------------------------------------//
// Dialog �֘Adefine
#define IDD_DIALOG1                     101
#define ID_DLG_INPUT                    101
#define ID_EDIT                         1001
#define ID_LABEL                        1002
//-------------------------------------------------------------//
//	���͊֐�
//-------------------------------------------------------------//
struct DLG_DATA
{
	char title[MAX_PATH];
	char label[MAX_PATH];
	char get[MAX_PATH];
	int size;
};

//-------------------------------------------------------------//
//�_�C�A���O�v���V�[�W��
//	�O���t�@�C������Ăяo���o���܂���
//-------------------------------------------------------------//
static int CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static DLG_DATA* d_ptr = NULL;

	switch(msg)
	{
		//�_�C�A���O����������
		case WM_INITDIALOG:
			d_ptr = (DLG_DATA*)lParam;	//�Ăяo��������f�[�^�ւ̃|�C���^���󂯎��
			if(d_ptr->title[0] != '\0') SetWindowText(hDlg, d_ptr->title);	//�^�C�g���ύX
			if(d_ptr->label[0] != '\0') SetWindowText(GetDlgItem(hDlg, ID_LABEL), d_ptr->label);//���x���ύX
			SetFocus(hDlg);
			break;
		//�{�^������
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				//�uok�v�{�^���������ꂽ
				case IDOK:
					GetDlgItemText(hDlg, ID_EDIT, d_ptr->get, MAX_PATH);
					EndDialog(hDlg, 1);
					break;
				//�u�L�����Z���v�{�^���������́ux�v�{�^���������ꂽ
				case IDCANCEL:
					EndDialog(hDlg, 0);
					break;
				default:
					return 0;	//Windows�ɔC����̂�0��Ԃ�
			}
			break;
		//����̏����FWindows�ɔC����
		default:
			return 0;	//Windows�ɔC����̂�0��Ԃ�
	}
	return 1;		//�����Ń��b�Z�[�W�����������̂�1��Ԃ�
}

//-------------------------------------------------------------//
//��������͊֐�
//	����
//		get			�擾���������̊i�[��
//		size		get�̃T�C�Y
//		title		�^�C�g���̕�����
//		label		�G�f�B�b�g�{�b�N�X�ɕ\�����镶����
//-------------------------------------------------------------//
//	�g�p��
//		char str[100];
//		Dbg_InputDialog(str, 100, "�m�F", "���O����͂��Ă�������");
//-------------------------------------------------------------//
DLG_ERROR debug::Dbg_InputDialog(char* get, int size, const char* title, const char* label)
{
	DLG_DATA data;

	if(get == NULL) return DLG_ERROR_INVALIDBUFFER;
	//�^�C�g���R�s�[
	if(title != NULL)
	{
		memcpy(&data.title, title, MAX_PATH);
	}
	else
	{
		data.title[0] = '\0';
	}
	//���x���R�s�[
	if(label != NULL)
	{
		memcpy(&data.label, label, MAX_PATH);
	}
	else
	{
		data.label[0] = '\0';
	}
	//�_�C�A���O�\��
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_DLG_INPUT),
					  system::hWnd, (DLGPROC)DlgProc, (LPARAM)&data) == 0)
	{
		//�u�L�����Z���v�{�^���������́ux�v�{�^���������ꂽ
		return DLG_ERROR_CANCEL;
	}
	//�uok�v�������ꂽ�i���͂���Ă��Ȃ��Ă��R�R�ɂ���j
	memcpy(get, data.get, size - 1);
	get[size-1] = '\0';
	return DLG_OK;
}

//********************************************************************//
//
//				�f�o�b�O�@�֘A�֐� �����܂�
//
//********************************************************************//

//********************************************************************//
//
//				�X�e�[�W�Ǘ��@�֘A�֐�
//
//********************************************************************//
int			_stageNo;
int			_initFlag;
//---------------------------------------------------------------------------------------
//�X�e�[�W�������Ŏw�肳�ꂽ�X�e�[�W�ɐ؂�ւ���
// ---------------------------------------------------------------------------------------
void stage::ChangeStage(int stage)
{
	_stageNo = stage;
	_initFlag = 1;
}

//---------------------------------------------------------------------------------------
//�X�e�[�W�؂�ւ����ɃX�e�[�W���������s�����𒲂ׂ�
//---------------------------------------------------------------------------------------
bool stage::InitStage()
{
	if(_initFlag == 1)
	{
		_initFlag = 0;
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------
//�X�e�[�W�Ǘ��֐��̏�����
//---------------------------------------------------------------------------------------
void stage::InitFlagStage()
{
	_initFlag = 1;
	_stageNo = 0;
}
//---------------------------------------------------------------------------------------
//�X�e�[�W�ԍ��𒲂ׂ�
//---------------------------------------------------------------------------------------
int stage::CheckStage()
{
	return _stageNo;
}
//********************************************************************//
//
//				�L�[�@�֘A�֐�
//
//********************************************************************//
//�L�[���\����
struct KEYINFO
{
	int code;	//���@�[�`�����L�[�R�[�h
	int status;	//�X�e�[�^�X
	int frameHold;	//������Ă���t���[����
	int frameFree;	//�͂Ȃ���Ă���t���[����
};
//�L�[���
static KEYINFO g_key[system::PLAYER_NUM][input::KEY_MAX];

static KEYINFO padbase[input::KEY_MAX] =
{
	{ input::JOY_DOWN, input::FREE_KEY, 0 }, //DOWN,
	{ input::JOY_LEFT, input::FREE_KEY, 0 }, //LEFT,
	{ input::JOY_UP, input::FREE_KEY, 0 }, //UP,
	{ input::JOY_RIGHT, input::FREE_KEY, 0 }, //RIGHT,
	{ JOY_BUTTON1, input::FREE_KEY, 0 }, //BTN_0,
	{ JOY_BUTTON2, input::FREE_KEY, 0 }, //BTN_1,
	{ JOY_BUTTON3, input::FREE_KEY, 0 }, //BTN_2,
	{ JOY_BUTTON4, input::FREE_KEY, 0 }, //SPACE,
	{ VK_F1, input::FREE_KEY, 0 }, //F1,
	{ VK_F2, input::FREE_KEY, 0 }, //F2,
	{ MK_LBUTTON, input::FREE_KEY, 0 }, //KEY_MOUSE_LBTN,
	{ MK_RBUTTON, input::FREE_KEY, 0 }, //KEY_MOUSE_RBTN,
};

//�L�[���蓖��
//�QP�ŃL�[�{�[�h�g�p�̍ۂ͏��������K�{
static KEYINFO keybase[system::PLAYER_NUM][input::KEY_MAX] =
{
	{
		//1P
		{ VK_DOWN, input::FREE_KEY, 0 }, //DOWN,
		{ VK_LEFT, input::FREE_KEY, 0 }, //LEFT,
		{ VK_UP, input::FREE_KEY, 0 }, //UP,
		{ VK_RIGHT, input::FREE_KEY, 0 }, //RIGHT,
		{ 'A', input::FREE_KEY, 0 }, //BTN_0,
		{ 'S', input::FREE_KEY, 0 }, //BTN_1,
		{ 'D', input::FREE_KEY, 0 }, //BTN_2,
		{ VK_SPACE, input::FREE_KEY, 0 }, //SPACE,
		{ VK_F1, input::FREE_KEY, 0 }, //F1,
		{ VK_F2, input::FREE_KEY, 0 }, //F2,
		{ MK_LBUTTON, input::FREE_KEY, 0 }, //KEY_MOUSE_LBTN,
		{ MK_RBUTTON, input::FREE_KEY, 0 }, //KEY_MOUSE_RBTN,
	},
	{
		//2P
		{ VK_DOWN, input::FREE_KEY, 0 }, //DOWN,
		{ VK_LEFT, input::FREE_KEY, 0 }, //LEFT,
		{ VK_UP, input::FREE_KEY, 0 }, //UP,
		{ VK_RIGHT, input::FREE_KEY, 0 }, //RIGHT,
		{ 'A', input::FREE_KEY, 0 }, //BTN_0,
		{ 'S', input::FREE_KEY, 0 }, //BTN_1,
		{ 'D', input::FREE_KEY, 0 }, //BTN_2,
		{ VK_SPACE, input::FREE_KEY, 0 }, //SPACE,
		{ VK_F1, input::FREE_KEY, 0 }, //F1,
		{ VK_F2, input::FREE_KEY, 0 }, //F2,
		{ MK_LBUTTON, input::FREE_KEY, 0 }, //KEY_MOUSE_LBTN,
		{ MK_RBUTTON, input::FREE_KEY, 0 }, //KEY_MOUSE_RBTN,
	}
};

//--------------------------------------------------------------------------------------------
//�@�������p�@�p�b�h�̐��擾
//--------------------------------------------------------------------------------------------
JOYINFO		*JoyInfo;
int		padcount;
bool usePad[system::PLAYER_NUM]; // true�Ȃ�p�b�h�g�p

bool input::IsUsePad(int playerID)
{
	if (playerID < 0 || playerID >= system::PLAYER_NUM) assert(0);
	return usePad[playerID];
}

static void SetKeysInfo()
{
	for (int i = 0; i < system::PLAYER_NUM; i++)
	{
		//�p�b�h�D��
		if(i < padcount){
			usePad[i] = true;
			memcpy(g_key[i], padbase, sizeof(KEYINFO)* input::KEY_MAX);
		}else{
			usePad[i] = false;
			memcpy(g_key[i], keybase, sizeof(KEYINFO)* input::KEY_MAX);
		}
	}
}

void InitPlayerInput()
{
	if (system::KeyboardMode == 0){
		//�p�b�h�J�E���g
		for (int i = 0; i < system::PLAYER_NUM; i++){
			JOYINFO		JoyInfoTemp;
			if (joyGetPos(i, &JoyInfoTemp) == JOYERR_NOERROR){
				padcount ++;
			}
		}
	}else{	//1���͋����I��0�ɂ���B
		padcount = 0;
	}
	//�������m��
	JoyInfo = (JOYINFO*)malloc(sizeof(JOYINFO)*padcount);
	//�p�b�h����
	SetKeysInfo();
}

//--------------------------------------------------------------------------------------------
//	�������p�@�p�b�h�f�[�^����
//--------------------------------------------------------------------------------------------
void DeletePlayerInput()
{
	free(JoyInfo);		
}
//--------------------------------------------------------------------------------------------
//	�������p�@�p�b�h�f�[�^�擾
//--------------------------------------------------------------------------------------------
BOOL JoyGet(int JoyID)
{
	//�W���C�X�e�B�b�N��Ԏ擾
	if(joyGetPos(JoyID,&JoyInfo[JoyID]) != JOYERR_NOERROR){
		return 0;
	}
	return 1;
}
//-------------------------------------------------------------//
//�@�������p�@�W���C�X�e�B�b�N�̃{�^��������Ԕ���
//		�����F	Joy�@�W���C�X�e�B�b�NID�@JOY�P�@JOY2�̂ǂ��炩
//				Code �{�^���ԍ��@BUTTON1�@���珇�ɂQ�C�R�C�S
//				Flag�@��Ԃ��󂯎��|�C���^�ϐ�
//-------------------------------------------------------------//
void JoyPressButton(int Joy,UINT Code,int *Flag)
{
	if ((JoyInfo[Joy].wButtons	&	Code)){
		if (*Flag == input::FREE_KEY)	*Flag = input::PUSH_KEY;
		else					*Flag = input::HOLD_KEY;
	}
	else{
		if (*Flag == input::PUSH_KEY || *Flag == input::HOLD_KEY)	*Flag = input::PULL_KEY;
		else									*Flag = input::FREE_KEY;
	}
}

//-------------------------------------------------------------//
//�@�������p�@�W���C�X�e�B�b�N�̏\���L�[������Ԕ���
//		�����F	Joy�@�W���C�X�e�B�b�NID�@JOY�P�@JOY2�̂ǂ��炩
//				Direct �����@JOY_UP,JOY_DOWN,JOY_LEFT,JOY_RIGHT�̂ǂꂩ
//				Flag�@��Ԃ��󂯎��|�C���^�ϐ�
//-------------------------------------------------------------//
void JoyPressXY(int Joy,int Direct,int *Flag)
{
	switch(Direct){
	case input::JOY_DOWN:
			if(JoyInfo[Joy].wYpos > 40000 ){
				if (*Flag == input::FREE_KEY)		*Flag = input::PUSH_KEY;
				else						*Flag = input::HOLD_KEY;
			}
			else{
				if (*Flag == input::PUSH_KEY || *Flag == input::HOLD_KEY)	*Flag = input::PULL_KEY;
				else									*Flag = input::FREE_KEY;
			}
			break;

	case input::JOY_UP:
			if(JoyInfo[Joy].wYpos < 2000 ){
				if (*Flag == input::FREE_KEY)		*Flag = input::PUSH_KEY;
				else						*Flag = input::HOLD_KEY;
			}
			else{
				if (*Flag == input::PUSH_KEY || *Flag == input::HOLD_KEY)	*Flag = input::PULL_KEY;
				else									*Flag = input::FREE_KEY;
			}
			break;
	case input::JOY_RIGHT:
			if(JoyInfo[Joy].wXpos > 40000 ){
				if (*Flag == input::FREE_KEY)		*Flag = input::PUSH_KEY;
				else						*Flag = input::HOLD_KEY;
			}
			else{
				if (*Flag == input::PUSH_KEY || *Flag == input::HOLD_KEY)	*Flag = input::PULL_KEY;
				else									*Flag = input::FREE_KEY;
			}
			break;
	case input::JOY_LEFT:
			if(JoyInfo[Joy].wXpos < 2000 ){
				if (*Flag == input::FREE_KEY)		*Flag = input::PUSH_KEY;
				else						*Flag = input::HOLD_KEY;
			}
			else{
				if (*Flag == input::PUSH_KEY || *Flag == input::HOLD_KEY)	*Flag = input::PULL_KEY;
				else									*Flag = input::FREE_KEY;
			}
			break;
	}


}

//--------------------------------------------------------------------------------------------
//�����ꂽ�^�C�~���O���`�F�b�N���܂�
//--------------------------------------------------------------------------------------------
static bool IsHoldTiming(int now, int prev)
{
	return now == input::PUSH_KEY ||					//�������ꂽ�@��������
		(prev != input::PUSH_KEY && prev != now);	//1�t���[���O�ɉ�����ĂȂ��A1�t���[���O�ƃX�e�[�^�X���Ⴄ�Ƃ�
}

//--------------------------------------------------------------------------------------------
//�͂Ȃ��ꂽ�^�C�~���O���`�F�b�N���܂�
//--------------------------------------------------------------------------------------------
static bool IsPullTiming(int now, int prev)
{
	return now == input::PULL_KEY ||					//���͂Ȃ��ꂽ�@��������
		(prev != input::PULL_KEY && prev != now);	//1�t���[���O�ɂ͂Ȃ���ĂȂ��A1�t���[���O�ƃX�e�[�^�X���Ⴄ�Ƃ�
}

//--------------------------------------------------------------------------------------------
//�t���[���J�n���ɌĂсA�Q�[���Ŏg���L�[�̏�Ԃ𒲂ׂ܂�
//���t���[���K���Ă΂Ȃ���΂Ȃ�܂���
//--------------------------------------------------------------------------------------------
void input::CheckKey()
{
	int i, prevStatus;

	for (int playernum = 0; playernum < system::PLAYER_NUM; playernum++)
	{
		if(playernum < padcount){
			for (i = 0; i < input::KEY_MAX; i++)
			{
				//�p�b�h�ł��}�E�X�̃{�^���͔F��������
				if (i < input::KEY_BTNMAX){
					//�t���[���J�E���^�`�F�b�N
					if(CheckPress(i,1,playernum)) g_key[playernum][i].frameHold++;	//������Ă���t���[�����J�E���g
					else						g_key[playernum][i].frameFree++;	//�͂Ȃ���Ă���t���[�����J�E���g

					//�O��̃L�[��ێ����A���ɉ����ꂽ�L�[�Ɣ��ʂ���
					prevStatus = g_key[playernum][i].status;
					JoyGet(playernum);
					if (g_key[playernum][i].code == input::JOY_DOWN || g_key[playernum][i].code == input::JOY_UP ||
						g_key[playernum][i].code == input::JOY_RIGHT || g_key[playernum][i].code == input::JOY_LEFT){
						JoyPressXY(playernum,g_key[playernum][i].code,&g_key[playernum][i].status);
					}else{
						JoyPressButton(playernum,g_key[playernum][i].code, &g_key[playernum][i].status);
					}
					//�����ꂽ�^�C�~���O�Ƃ͂Ȃ��ꂽ�^�C�~���O�ŃJ�E���^��0�N���A����
					if( IsHoldTiming(g_key[playernum][i].status, prevStatus))		g_key[playernum][i].frameHold = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
					else if(IsPullTiming(g_key[playernum][i].status, prevStatus))	g_key[playernum][i].frameFree = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
				}else{
					//�t���[���J�E���^�`�F�b�N
					if(CheckPress(i,1,playernum)) g_key[playernum][i].frameHold++;	//������Ă���t���[�����J�E���g
					else						g_key[playernum][i].frameFree++;	//�͂Ȃ���Ă���t���[�����J�E���g

					//�O��̃L�[��ێ����A���ɉ����ꂽ�L�[�Ɣ��ʂ���
					prevStatus = g_key[playernum][i].status;
					Key_GetKey(g_key[playernum][i].code, &g_key[playernum][i].status);

					//�����ꂽ�^�C�~���O�Ƃ͂Ȃ��ꂽ�^�C�~���O�ŃJ�E���^��0�N���A����
					if( IsHoldTiming(g_key[playernum][i].status, prevStatus))		g_key[playernum][i].frameHold = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
					else if(IsPullTiming(g_key[playernum][i].status, prevStatus))	g_key[playernum][i].frameFree = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
				}
			}
		}else{
			for (i = 0; i < input::KEY_MAX; i++)
			{
				//�t���[���J�E���^�`�F�b�N
				if(CheckPress(i,1,playernum)) g_key[playernum][i].frameHold++;	//������Ă���t���[�����J�E���g
				else			  g_key[playernum][i].frameFree++;	//�͂Ȃ���Ă���t���[�����J�E���g

				//�O��̃L�[��ێ����A���ɉ����ꂽ�L�[�Ɣ��ʂ���
				prevStatus = g_key[playernum][i].status;
				Key_GetKey(g_key[playernum][i].code, &g_key[playernum][i].status);

				//�����ꂽ�^�C�~���O�Ƃ͂Ȃ��ꂽ�^�C�~���O�ŃJ�E���^��0�N���A����
				if( IsHoldTiming(g_key[playernum][i].status, prevStatus))		g_key[playernum][i].frameHold = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
				else if(IsPullTiming(g_key[playernum][i].status, prevStatus))	g_key[playernum][i].frameFree = 0;	//�X�e�[�^�X���Ⴄ�̂Ńt���[����0�ɖ߂�
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
//�L�[���u������Ă���v�����`�F�b�N���܂�
//interval�ŗ^�����t���[���P�ʂ�true���������Ă��܂�
//�y��z
//interval��1�Ȃ疈�t���[��true
//interval��2�Ȃ�2�t���[���Ɉ��true
//interval��5�Ȃ�5�t���[���Ɉ��true
//--------------------------------------------------------------------------------------------
bool input::CheckPress(int keyno, int interval, int playernum)
{
	interval = interval > 0 ? interval: 1;	//0���Z�΍�

	//�L�[�X�e�[�^�X���u�����ꂽ�v�u������Ă���v�̂ǂ��炩��
	//�L�[�������ꂽ�t���[������]�ݒ�ƈ�v���Ă��邩�H
	if ((g_key[playernum][keyno].status == input::HOLD_KEY ||
		g_key[playernum][keyno].status == input::PUSH_KEY) &&
		g_key[playernum][keyno].frameHold % interval == 0)
	{
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------
//�����ꂩ�̃f�o�C�X�̃L�[��������Ă��邩���`�F�b�N���܂�
//--------------------------------------------------------------------------------------------
bool input::CheckAnyPress(int keyno)
{
	for(int i = 0; i < system::PLAYER_NUM; ++i)
		if(CheckPress(keyno, 1, i)) return true;

	return false;
}

//--------------------------------------------------------------------------------------------
//�L�[���u�͂Ȃ���Ă���v�����`�F�b�N���܂�
//interval�ŗ^�����t���[���P�ʂ�true���������Ă��܂�
//�y��z
//interval��1�Ȃ疈�t���[��true
//interval��2�Ȃ�2�t���[���Ɉ��true
//interval��5�Ȃ�5�t���[���Ɉ��true
//--------------------------------------------------------------------------------------------
bool input::CheckFree(int keyno, int interval, int playernum)
{
	interval = interval > 0 ? interval: 1;	//0���Z�΍�

	//�L�[�X�e�[�^�X���u�͂Ȃ��ꂽ�v�u�͂Ȃ���Ă���v�̂ǂ��炩��
	//�L�[�������ꂽ�t���[������]�ݒ�ƈ�v���Ă��邩�H
	if ((g_key[playernum][keyno].status == input::FREE_KEY ||
		g_key[playernum][keyno].status == input::PULL_KEY) &&
		g_key[playernum][keyno].frameFree % interval == 0)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------
//�L�[�����̃t���[���Łu�����ꂽ�v�����`�F�b�N���܂�
//--------------------------------------------------------------------------------------------
bool input::CheckPush(int keyno, int playernum)
{
	if (g_key[playernum][keyno].status == input::PUSH_KEY)
	{
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------
//�����ꂩ�̃f�o�C�X�̃L�[��������Ă��邩���`�F�b�N���܂�
//--------------------------------------------------------------------------------------------
bool input::CheckAnyPush(int keyno)
{
	for(int i = 0; i < system::PLAYER_NUM; ++i)
		if(CheckPush(keyno, i)) return true;

	return false;
}

//--------------------------------------------------------------------------------------------
//�L�[�����̃t���[���Łu�͂Ȃ��ꂽ�v�����`�F�b�N���܂�
//--------------------------------------------------------------------------------------------
bool input::CheckPull(int keyno, int playernum)
{
	if (g_key[playernum][keyno].status == input::PULL_KEY)
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------
//�L�[�̃X�e�[�^�X��Ԃ��܂�
//--------------------------------------------------------------------------------------------
int input::GetKeyStatus(int keyno, int playernum)
{
	return g_key[playernum][keyno].status;
}

//--------------------------------------------------------------------------------------------
//�L�[�̃t���[����Ԃ��܂�
//--------------------------------------------------------------------------------------------
int input::GetKeyFrameHold(int keyno, int playernum)
{
	return g_key[playernum][keyno].frameHold;
}
int input::GetKeyFrameFree(int keyno, int playernum)
{
	return g_key[playernum][keyno].frameFree;
}

//--------------------------------------------------------------------------------------------
//�L�[�̃t���[�������������܂�
//--------------------------------------------------------------------------------------------
void input::InitKeyInfo(int playernum)
{
	for (int i = 0; i < input::KEY_MAX; i++)
	{
		g_key[playernum][i].status = input::FREE_KEY;
		g_key[playernum][i].frameHold  = 0;
		g_key[playernum][i].frameFree  = 0;
	}
}

//--------------------------------------------------------------------------------------------
//�L�[�R���t�B�O
//--------------------------------------------------------------------------------------------
void input::SetKeyFormat(input::CODE_KEY key, DWORD keycode, int playernum)
{
	g_key[playernum][key].code	  = keycode;
	g_key[playernum][key].status = input::FREE_KEY;
	g_key[playernum][key].frameHold  = 0;
	g_key[playernum][key].frameFree  = 0;
}

//--------------------------------------------------------------------------------------------
//�L�[�R���t�B�O
//--------------------------------------------------------------------------------------------
void input::SetKeysFormat(DWORD* keycode, int playernum)
{
	for (int i = 0; i < input::KEY_MAX; ++i)
	{
		g_key[playernum][i].code	 = keycode[i];
		g_key[playernum][i].status = input::FREE_KEY;
		g_key[playernum][i].frameHold  = 0;
		g_key[playernum][i].frameFree  = 0;
	}
}

//---------------------------------------------------------------------------------------
//�}�E�X�J�[�\���̍��W���擾(�߂�l)
//---------------------------------------------------------------------------------------
POINT input::GetMousePosition()
{
	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(system::hWnd, &pt);
	return pt;
}

//---------------------------------------------------------------------------------------
//�}�E�X�J�[�\���̍��W���擾(����)
//---------------------------------------------------------------------------------------
void input::GetMousePosition(POINT* pt_ptr)
{
	GetCursorPos(pt_ptr);
	ScreenToClient(system::hWnd, pt_ptr);
}

//---------------------------------------------------------------------------------------
//�L�[��Ԏ擾
//Code�@�擾�������L�[�̉��z�L�[�R�[�h�A�`�r�b�h�h�R�[�h
//Flag�@�L�[�̏�Ԃ��擾�������o�b�t�@�ւ̃|�C���^
//---------------------------------------------------------------------------------------
void input::Key_GetKey(unsigned int Code, int* Flag)
{
	if ((GetKeyState(Code)&0x80)==0x80){
		if (*Flag == input::FREE_KEY){
			*Flag = input::PUSH_KEY;
		}else{
			*Flag = input::HOLD_KEY;
		}
	}
	else{
		if (*Flag == input::PUSH_KEY || *Flag == input::HOLD_KEY){
			*Flag = input::PULL_KEY;
		}else{
			*Flag = input::FREE_KEY;
		}
	}
}
//********************************************************************//
//
//				�`��@�֘A�֐�
//
//********************************************************************//

//---------------------------------------------------------------------------------------
//�������p�@
//�@�������`�掞�ɍĕ`����s��
//---------------------------------------------------------------------------------------
void DD_ReDraw()
{
	pSprite->End();
	//pD3DDevice->EndScene();
	//pD3DDevice->BeginScene();
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}
#define DD_ReDrawCheck(x) if((x&0xff000000)!=0xff000000)DD_ReDraw();
//#define DD_ReDrawCheck(x) DD_ReDraw();

//--------------------------------------------------------------------------------------------
//�摜�f�[�^�T�C�Y�擾
//--------------------------------------------------------------------------------------------
D3DSURFACE_DESC graph::Draw_GetImageSize(const std::string& resname)
{
	D3DSURFACE_DESC TexData;
	ImgTable.at(resname).tex->GetLevelDesc(0,&TexData);
	return TexData;
}
unsigned int graph::Draw_GetImageWidth(const std::string& resname)
{
	return ImgTable.at(resname).info.Width;
}
unsigned int graph::Draw_GetImageHeight(const std::string& resname)
{
	return ImgTable.at(resname).info.Height;
}
SIZE graph::Draw_GetImageSize2(const std::string& resname)
{
	SIZE sz = { ImgTable.at(resname).info.Width, ImgTable.at(resname).info.Height };
	return sz;
}

//-------------------------------------------------------------//
//���ꍇ���J�n
//-------------------------------------------------------------//
//�g�p��
//	Draw_SetRenderMode(ADD);
//	�`��
//	Draw_EndRenderMode();
//-------------------------------------------------------------//
void graph::Draw_SetRenderMode(int Mode)
{
	if(Mode < 0 || Mode >= BF_MAX) return;

	//Clear���Ɉ�xBegin����Ă���̂ň��I��点��
	pSprite->End();
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

//�A���t�@�u�����f�B���O�̗L������InitDx()�ֈړ�
//	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);

	DWORD flag[6][3] = 
	{
		{D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA},
		{D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_ONE},
		{D3DBLENDOP_ADD, D3DBLEND_ONE, D3DBLEND_INVSRCALPHA},
		{D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_SRCCOLOR},
		{D3DBLENDOP_REVSUBTRACT, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCCOLOR},
		{D3DBLENDOP_ADD, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCCOLOR}
	};

	pD3DDevice->SetRenderState(D3DRS_BLENDOP, flag[Mode][0]);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, flag[Mode][1]);  //SRC�̐ݒ�
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, flag[Mode][2]);//DEST�̐ݒ�
}


//-------------------------------------------------------------//
//������ʏI��
//-------------------------------------------------------------//
//�g�p��
//	Draw_SetRenderMode�Q��
//-------------------------------------------------------------//
void graph::Draw_EndRenderMode()
{
	graph::Draw_SetRenderMode(NORMAL);
}
//---------------------------------------------------------------------------------------
//�t�H���g�̍쐬
//	Num			�t�H���g�ԍ�
//	size		�t�H���g�T�C�Y
//	fontname	�t�H���g�̖��O
//-------------------------------------------------------------//
//�g�p��
// void OnCreate(){
//		Draw_CreateFont(0,30);	//�w��Ȃ��͂l�r�S�V�b�N
// }
//�g�p��@0�Ԃɐݒ肵���T�C�Y30�̃t�H���g�ŕ`�揈�����s��
//	Draw_FontText(0,0,"MainStage",ARGB(255,255,255,0),0);
//
//---------------------------------------------------------------------------------------
void font::Draw_CreateFont(int Num,int size,LPCTSTR	fontname)
{
	if(pFontData[Num] == NULL){
		 // �t�H���g�̍쐬
		D3DXCreateFont(pD3DDevice,			//LPDIRECT3DDEVICE9	lpD3DDev,
			size,							//UINT				Height,
			0,								//UINT				Width,
			FW_NORMAL,						//UINT				Weight,
			D3DX_DEFAULT,					//UINT				MipLevels,
			FALSE,							//BOOL				Italic,
			DEFAULT_CHARSET ,				//DWORD				CharSet,
			OUT_DEFAULT_PRECIS,				//DWORD				OutputPrecision,
			PROOF_QUALITY,					//DWORD				Quality,
			DEFAULT_PITCH | FF_DONTCARE,	//DWORD				PitchAndFamily,
			fontname,						//LPCTSTR			pFacename,
			&pFontData[Num]						//LPD3DXFONT		*ppFont		&pD3DXFont
		);
	}else{
		return;
	}
}

void font::Draw_CreateFontItalic(int Num, int size, LPCTSTR	fontname)
{
	if(pFontData[Num] == NULL){
		 // �t�H���g�̍쐬
		D3DXCreateFont(pD3DDevice,			//LPDIRECT3DDEVICE9	lpD3DDev,
			size,							//UINT				Height,
			0,								//UINT				Width,
			FW_NORMAL,						//UINT				Weight,
			D3DX_DEFAULT,					//UINT				MipLevels,
			TRUE,							//BOOL				Italic,
			DEFAULT_CHARSET ,				//DWORD				CharSet,
			OUT_DEFAULT_PRECIS,				//DWORD				OutputPrecision,
			PROOF_QUALITY,					//DWORD				Quality,
			DEFAULT_PITCH | FF_DONTCARE,	//DWORD				PitchAndFamily,
			fontname,						//LPCTSTR			pFacename,
			&pFontData[Num]						//LPD3DXFONT		*ppFont		&pD3DXFont
		);
	}else{
		return;
	}
}


//---------------------------------------------------------------------------------------
//�t�H���g�e�[�u���������@�����g�p
//---------------------------------------------------------------------------------------
void Draw_InitFont()
{
	for(int i = 0 ; i < DDFONTMAX ; i++){
		pFontData[i]= NULL;
	}
}
//---------------------------------------------------------------------------------------
//�t�H���g�폜�@�����g�p
//---------------------------------------------------------------------------------------
void Draw_DeleteFont()
{
	for(int i = 0 ; i < DDFONTMAX ; i++){
		if(pFontData[i] != NULL){
			RELEASE(pFontData[i]);
		}
	}
}

//---------------------------------------------------------------------------------------
//�������p�@
// ImgTable������������B
//---------------------------------------------------------------------------------------
void Draw_InitImgTable()
{
	ImgTable.clear();
}

//---------------------------------------------------------------------------------------
//�������p�@
// ImgTable��j������B
//---------------------------------------------------------------------------------------
void Draw_DelImgTable()
{
	for(auto& img : ImgTable)
	{
		RELEASE(img.second.tex);
	}
}
//-------------------------------------------------------------//
//�����g�p�@�|���S���`��
//	����
//		X1,Y1			��`�̊J�n���W
//		X2,Y2			��`�̕�����
//		Color1			�h��F
//-------------------------------------------------------------//
void Draw_2DPolygonNC(
	int startX, int startY,
	int endX, int endY,
	float z, D3DCOLOR color)
{
	struct CUSTOMVERTEX {
			FLOAT x, y, z;
			FLOAT rhw;
			DWORD color;
	};

	CUSTOMVERTEX v[4] = {
		{static_cast<float>(endX),   static_cast<float>(startY), z, 1.0f, color}, 
		{static_cast<float>(endX),   static_cast<float>(endY), z, 1.0f, color}, 
		{static_cast<float>(startX), static_cast<float>(startY), z, 1.0f, color}, 
		{static_cast<float>(startX), static_cast<float>(endY), z, 1.0f, color}
	}; 
	pD3DDevice->SetTexture(0,NULL);
	pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v , sizeof (CUSTOMVERTEX));
}

//-------------------------------------------------------------//
//��`�`��
//	����
//		left,top			��`�̊J�n���W
//		right,bottom			��`�̕�����
//		fillColor			�h��F
//		frameColor			�g�F
//		lineSize			���C����
//		nakanuri		�h��Ԃ����H
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//-------------------------------------------------------------//
void graph::Draw_BoxNC(
	int left, int top, int right, int bottom,
	float z,
	D3DCOLOR fillColor, D3DCOLOR frameColor,
	int lineSize, bool nakanuri)
{
	DD_ReDrawCheck(fillColor);
	if(nakanuri)
	{
		Draw_2DPolygonNC(left, top, right, bottom, z, fillColor);
	}

	POINT vec[] =
	{
		{left, top},
		{left, bottom},
		{right, bottom},	// ���C��
		{right, top},	// ���C��
		{left, top},	// ���C���̏I�_
	};
	DD_ReDrawCheck(frameColor);
	for(int i = 0 ; i < 4 ; i ++)
	{
		graph::Draw_LineNC(
			vec[i].x,   vec[i].y, 
			vec[i+1].x, vec[i+1].y,
			z, frameColor, lineSize);
	}
}
void graph::Draw_BoxCenterNC(
	int x, int y, int sizeX, int sizeY,
	float z,
	D3DCOLOR fillColor, D3DCOLOR frameColor,
	int lineSize, bool nakanuri)
{
	int sx = sizeX/2,
		  sy = sizeY/2;
	graph::Draw_BoxNC(x - sx, y - sy, x + sx, y + sy, z,
		fillColor, frameColor, lineSize, nakanuri);
}
//-------------------------------------------------------------//
//�����`��
//	����
//		X1,Y1			��`�̊J�n���W
//		X2,Y2			��`�̕�����
//		Color2			�F
//		Size			���C����
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//-------------------------------------------------------------//
void graph::Draw_LineNC(
	int startX, int startY,
	int endX, int endY,
	float z, D3DCOLOR color, int size)
{
	struct CUSTOMVERTEX {
			FLOAT x, y, z;
			FLOAT rhw;
			DWORD color;
	};

	D3DCOLOR c = color;
	for(int i = 0; i < size ; i++){
		int tx,ty;
		if(abs(startX-endX) > abs(startY-endY)){
			tx = 0;
			ty = i-size/2;
		}else{
			tx = i-size/2;
			ty = 0;
		}
		CUSTOMVERTEX v[2] = {
			{static_cast<float>(startX+tx), static_cast<float>(startY+ty), z, 1.0f, c}, 
			{static_cast<float>(endX+tx), static_cast<float>(endY+ty), z, 1.0f, c}, 
		}; 
		pD3DDevice->SetTexture(0,NULL);
		pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v , sizeof (CUSTOMVERTEX));
	}
}

//-------------------------------------------------------------//
//�����`��
//	x,y,		�����`��ʒu
//	string	�����̕�
//	color	�����F
//	t = 0	�t�H���g�ԍ�
//-------------------------------------------------------------//
//�g�p��
//	Draw_TextXY(0,0,"MainStage",ARGB(255,255,255,0),1);
//	
//�t�H���g�ԍ��w�莞��OnCreate�ɂăt�H���g��p�ӂ��邱��
//void OnCreate()
//{
//		Draw_CreateFont(0,15);
//		Draw_CreateFont(1,30,"HG�n�p�p�߯�ߑ�");
//}
//
//	Draw_TextXY(0,0,"MainStage",ARGB(255,255,255,0),1);
//-------------------------------------------------------------//
void font::Draw_TextXYNC(int x, int y, const std::string& msg, D3DCOLOR color, int fontID)
{
	font::Draw_FontTextNC(x, y, 0.0f, msg, color, fontID);
}

//---------------------------------------------------------------------------------------
//�����`��
//	rt	�\���̈�
//	z	�����`��ʒu
//	string	�����̕�
//	color	�����F
//	Num		�g�p�t�H���g�ԍ�
//-------------------------------------------------------------//
//�g�p��
//	Draw_FontDrawText(0,0,"MainStage",ARGB(255,255,0),1);
//---------------------------------------------------------------------------------------
int font::Draw_FontTextNC(const RECT& rt,float z,const std::string& msg,D3DCOLOR color, int fontID)
{
	DD_ReDrawCheck(color);
	if(pFontData[fontID] == NULL)	return 0;
	
	//Z��
	D3DXMATRIX matDraw;	//	���W�ϊ��}�g���b�N�X�̊i�[��
	D3DXVECTOR3 position(0.0f, 0.0f, z);	//	�\������ʒu���w��
	D3DXMatrixTransformation(&matDraw, NULL, NULL,NULL, 
						NULL,NULL, &position);
	pSprite->SetTransform(&matDraw);
	//�`��
	RECT rc = rt;
	int h = pFontData[fontID]->DrawText(
		pSprite,
		msg.c_str(),
		-1,
		&rc,
		DT_LEFT | DT_WORDBREAK,
		color);
	//���[���h�̃��Z�b�g
	D3DXMatrixRotationZ(&matDraw,0.0f);
	pSprite->SetTransform(&matDraw);
	return h;
}
//---------------------------------------------------------------------------------------
//�����`��
//	x,y,z	�����`��ʒu
//	string	�����̕�
//	color	�����F
//	Num		�g�p�t�H���g�ԍ�
//-------------------------------------------------------------//
//�g�p��
//	Draw_FontDrawText(0,0,"MainStage",ARGB(255,255,0),1);
//---------------------------------------------------------------------------------------
void font::Draw_FontTextNC(int x, int y, float z, const std::string& msg, D3DCOLOR color, int fontID)
{
	DD_ReDrawCheck(color);
	if(pFontData[fontID] == NULL)	return;
	//�\���e�L�X�g�T�C�Y�v�Z
	TEXTMETRIC  TextMetric;
	pFontData[fontID]->GetTextMetrics(&TextMetric);
	int w = x+(TextMetric.tmMaxCharWidth * msg.length());
	int h = y+TextMetric.tmHeight;
	//�\���pRECT�쐬
	RECT	rt = {x,y,w,h};
	
	//Z��
	D3DXMATRIX matDraw;	//	���W�ϊ��}�g���b�N�X�̊i�[��
	D3DXVECTOR3 position(0.0f, 0.0f, z);	//	�\������ʒu���w��
	D3DXMatrixTransformation(&matDraw, NULL, NULL,NULL, 
						NULL,NULL, &position);
	pSprite->SetTransform(&matDraw);
	pFontData[fontID]->DrawText(pSprite,msg.c_str(),-1,&rt,DT_LEFT,color);
	//���[���h�̃��Z�b�g
	D3DXMatrixRotationZ(&matDraw,0.0f);
	pSprite->SetTransform(&matDraw);
}
//-------------------------------------------------------------//
//RECT��\������B
//-------------------------------------------------------------//
void		graph::Draw_CkRectNC(const RECT& rt, D3DCOLOR color)
{
	graph::Draw_BoxNC(rt.left, rt.top, rt.right, rt.bottom,
		0.0f, ARGB(254,0,0,0), color, 1, 0);
}
//---------------------------------------------------------------------------------------
//IMGOBJ��\������B��]�A�g��A�F�ύX�Ή��@�w�肵�����W�̒��S�ɕ`��
//	����
//		X1,Y1			�\�����S�J�n���W
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//		IMGOBJ			�`�悵����IMGNO
//		TempX1,TempY1	IMGNO����̎Q�ƍ��W
//		TempX2,TempY2	TempX1,TempY1����̕��A����
//		deg				��]�p�x
//		ScaleX			X���̔{�� 1.0�����{
//		ScaleY			X���̔{�� 1.0�����{
//		a				���`�����l��
//		r				�Ԃ̋���
//		g				�΂̋���
//		b				�̋���
//---------------------------------------------------------------------------------------
//Draw_Graphics(100,100,1.0f,TestImg,0,0,100,100,
//				90,1.0f,1.0f,255,255,255,255);
//---------------------------------------------------------------------------------------
void graph::Draw_GraphicsNC(
	int x, int y, float z,
	const std::string& resname,
	int srcX, int srcY, int sizeX,int sizeY,
	int	degree, POINT *center,
	float	scaleX,float scaleY,
	u_char a, u_char r, u_char g, u_char b)
{
	//�`���`�v�Z
	RECT rt = 
	{
		srcX,
		srcY,
		srcX + sizeX,
		srcY + sizeY
	};

	D3DXVECTOR2 centerPos(static_cast<float>(sizeX) / 2.0f ,static_cast<float>(sizeY) / 2.0f );

//	if(a != 255)DD_ReDraw();
	D3DXMATRIX matDraw;	//	���W�ϊ��}�g���b�N�X�̊i�[��
	D3DXVECTOR3 ptCenter(centerPos.x, centerPos.y, 0.0f);	//	�`��̊�l�̐ݒ�
	D3DXVECTOR3 position(0.0f, 0.0f, z);	//	�\������ʒu���w��
	D3DXVECTOR2 draw(static_cast<float>(x),static_cast<float>(y));	//	�`�����W�i���Z�O�j
	D3DXVECTOR2 scale(scaleX, scaleY);	//	�X�P�[�����O�W���i�{�����w��j
	
	D3DXVECTOR2	 sCenter;
	if(center == nullptr)
	{
		sCenter = D3DXVECTOR2(0, 0);
	}
	else
	{
		sCenter = D3DXVECTOR2(static_cast<float>(center->x), static_cast<float>(center->y));
	}

	D3DXMatrixTransformation2D(
		&matDraw,
		&sCenter,
		0.0f,
		&scale, 
		NULL,
		D3DXToRadian(-degree),
		&draw);
	pSprite->SetTransform(&matDraw);
	//�`��
	pSprite->Draw(
		ImgTable.at(resname).tex,
		&rt,
		&ptCenter,
		&position,
		D3DCOLOR_ARGB(a,r,g,b));
	//���[���h�̃��Z�b�g
//	D3DXMatrixRotationZ(&matDraw,0.0f);
//	pSprite->SetTransform(&matDraw);
}
void graph::Draw_GraphicsNC(
	float x, float y, float z,
	const std::string& resname,
	int srcX, int srcY, int sizeX,int sizeY,
	int	degree, POINT *center,
	float	scaleX,float scaleY,
	u_char a, u_char r, u_char g, u_char b)
{
	graph::Draw_GraphicsNC(
		static_cast<int>(x), 
		static_cast<int>(y),
		z, resname,
		srcX, srcY, sizeX, sizeY,
		degree, center, scaleX, scaleY,
		a, r, g, b);
}

//---------------------------------------------------------------------------------------
//IMGOBJ��\������B��]�A�g��A�F�ύX�Ή�
//	����
//		X1,Y1			�\���J�n���W
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//		IMGOBJ			�`�悵����IMGNO
//		TempX1,TempY1	IMGNO����̎Q�ƍ��W
//		TempX2,TempY2	TempX1,TempY1����̕��A����
//		deg				��]�p�x
//		*pCenter		��]�̒��S
//		ScaleX			X���̔{�� 1.0�����{
//		ScaleY			X���̔{�� 1.0�����{
//		a				���`�����l��
//		r				�Ԃ̋���
//		g				�΂̋���
//		b				�̋���
//---------------------------------------------------------------------------------------
//Draw_Graphics(100,100,1.0f,TestImg,0,0,100,100,
//				90,&pt,1.0f,1.0f,255,255,255,255);
//---------------------------------------------------------------------------------------
void graph::Draw_GraphicsLeftTopNC(
	int x, int y, float z,
	const std::string& resname,
	int srcX, int srcY, int sizeX,int sizeY,
	int	degree, POINT *center,
	float	scaleX,float scaleY,
	u_char a, u_char r, u_char g, u_char b)
{
	//�`���`�v�Z
	RECT rt = 
	{
		srcX,
		srcY,
		srcX + sizeX,
		srcY + sizeY
	};

	D3DXVECTOR2 centerPos;
	if(center == nullptr)
	{
		centerPos = D3DXVECTOR2(0,0);
	}
	else
	{
		centerPos = D3DXVECTOR2(
			static_cast<float>(center->x),
			static_cast<float>(center->y));
	}
//	if(a != 255)DD_ReDraw();

	D3DXMATRIX matDraw;	//	���W�ϊ��}�g���b�N�X�̊i�[��
	D3DXVECTOR3 ptCenter(0, 0, 0.0f);	//	�`��̊�l�̐ݒ�
	D3DXVECTOR3 position(0.0f, 0.0f, z);	//	�\������ʒu���w��
	D3DXVECTOR2 draw(static_cast<float>(x),static_cast<float>(y));	//	�`�����W�i���Z�O�j
	D3DXVECTOR2 scale(scaleX, scaleY);	//	�X�P�[�����O�W���i�{�����w��j

	D3DXMatrixTransformation2D(
		&matDraw,
		NULL,
		0.0f,
		&scale, 
		&centerPos,
		D3DXToRadian(-degree),
		&draw);
	pSprite->SetTransform(&matDraw);
	//�`��
	pSprite->Draw(
		ImgTable.at(resname).tex,
		&rt,
		&ptCenter,
		&position,
		D3DCOLOR_ARGB(a,r,g,b));
	//���[���h�̃��Z�b�g
//	D3DXMatrixRotationZ(&matDraw,0.0f);
//	pSprite->SetTransform(&matDraw);
}
void graph::Draw_GraphicsLeftTopNC(
	float x, float y, float z,
	const std::string& resname,
	int srcX, int srcY, int sizeX, int sizeY,
	int	degree, POINT *center,
	float	scaleX,float scaleY,
	u_char a, u_char r, u_char g, u_char b)
{
	graph::Draw_GraphicsLeftTopNC(
		static_cast<int>(x), 
		static_cast<int>(y),
		z, resname,
		srcX, srcY, sizeX, sizeY,
		degree, center, scaleX, scaleY,
		a, r, g, b);
}
//-------------------------------------------------------------//
//�����g�p�@�|���S���`��
//	����
//		X1,Y1			��`�̊J�n���W
//		X2,Y2			��`�̕�����
//		Color1			�h��F
//-------------------------------------------------------------//
void Draw_2DPolygon(
	int startX, int startY,
	int endX, int endY,
	float z, D3DCOLOR color)
{
	Draw_2DPolygonNC(
		camera::camera->ixchange(startX), camera::camera->iychange(startY),
		camera::camera->ixchange(endX), camera::camera->iychange(endY),
		z, color);
}

//-------------------------------------------------------------//
//��`�`��
//	����
//		X1,Y1			��`�̊J�n���W
//		X2,Y2			��`�̕�����
//		Color1			�h��F
//		Color2			�g�F
//		Size			���C����
//		Nakanuri		�h��Ԃ����H
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//-------------------------------------------------------------//
void graph::Draw_Box(
	int left, int top, int right, int bottom,
	float z,
	D3DCOLOR fillColor, D3DCOLOR frameColor,
	int lineSize, bool nakanuri)
{
	graph::Draw_BoxNC(
		camera::camera->ixchange(left), camera::camera->iychange(top),
		camera::camera->ixchange(right), camera::camera->iychange(bottom),
		z, fillColor, frameColor,
		lineSize, nakanuri);
}
void graph::Draw_BoxCenter(
	int x, int y, int sizeX, int sizeY,
	float z,
	D3DCOLOR fillColor, D3DCOLOR frameColor,
	int lineSize, bool nakanuri)
{
	int sx = sizeX/2,
		  sy = sizeY/2;
	graph::Draw_BoxNC(
		camera::camera->ixchange(x) - sx, camera::camera->iychange(y) - sy,
		camera::camera->ixchange(x) + sx, camera::camera->iychange(y) + sy,
		z,
		fillColor, frameColor,
		lineSize, nakanuri);
}

//-------------------------------------------------------------//
//�����`��
//	����
//		X1,Y1			��`�̊J�n���W
//		X2,Y2			��`�̕�����
//		Color2			�F
//		Size			���C����
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//-------------------------------------------------------------//
void graph::Draw_Line(
	int startX, int startY,
	int endX, int endY,
	float z, D3DCOLOR color, int size)
{
	graph::Draw_LineNC(
		camera::camera->ixchange(startX), camera::camera->iychange(startY),
		camera::camera->ixchange(endX), camera::camera->iychange(endY),
		z, color, size);
}

//-------------------------------------------------------------//
//�����`��
//	x,y,		�����`��ʒu
//	string	�����̕�
//	color	�����F
//	t = 0	�t�H���g�ԍ�
//-------------------------------------------------------------//
//�g�p��
//	Draw_TextXY(0,0,"MainStage",ARGB(255,255,255,0),1);
//	
//�t�H���g�ԍ��w�莞��OnCreate�ɂăt�H���g��p�ӂ��邱��
//void OnCreate()
//{
//		Draw_CreateFont(0,15);
//		Draw_CreateFont(1,30,"HG�n�p�p�߯�ߑ�");
//}
//
//	Draw_TextXY(0,0,"MainStage",ARGB(255,255,255,0),1);
//-------------------------------------------------------------//
void font::Draw_TextXY(int x, int y, const std::string& msg, D3DCOLOR color, int fontID)
{
	font::Draw_FontTextNC(
		camera::camera->ixchange(x), camera::camera->iychange(y),
		0, msg, color, fontID);
}

//---------------------------------------------------------------------------------------
//�����`��
//	rt	�\���̈�
//	z	�����`��ʒu
//	string	�����̕�
//	color	�����F
//	Num		�g�p�t�H���g�ԍ�
//-------------------------------------------------------------//
//�g�p��
//	Draw_FontDrawText(0,0,"MainStage",ARGB(255,255,0),1);
//---------------------------------------------------------------------------------------
int font::Draw_FontText(const RECT& rt, float z, const std::string& msg, D3DCOLOR color, int fontID)
{
	RECT rc = rt;
	OffsetRect(&rc, -camera::camera->iviewLeft(), -camera::camera->iviewTop());
	return font::Draw_FontTextNC(rc, z, msg, color, fontID);
}
//---------------------------------------------------------------------------------------
//�����`��
//	x,y,z	�����`��ʒu
//	string	�����̕�
//	color	�����F
//	Num		�g�p�t�H���g�ԍ�
//-------------------------------------------------------------//
//�g�p��
//	Draw_FontDrawText(0,0,"MainStage",ARGB(255,255,0),1);
//---------------------------------------------------------------------------------------
void font::Draw_FontText(int x, int y, float z, const std::string& msg, D3DCOLOR color, int fontID)
{
	font::Draw_FontTextNC(
		x - camera::camera->iviewLeft(), y - camera::camera->iviewTop(),
		z, msg, color, fontID);
}
//-------------------------------------------------------------//
//RECT��\������B
//-------------------------------------------------------------//
void		graph::Draw_CkRect(const RECT& rt,D3DCOLOR color)
{
	RECT rc = rt;

	graph::Draw_Box(rc.left, rc.top, rc.right, rc.bottom,
		0.0f, ARGB(254,0,0,0), color, 1, 0);
}
//---------------------------------------------------------------------------------------
//IMGOBJ��\������B��]�A�g��A�F�ύX�Ή��@�w�肵�����W�̒��S�ɕ`��
//	����
//		X1,Y1			�\�����S�J�n���W
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//		IMGOBJ			�`�悵����IMGNO
//		TempX1,TempY1	IMGNO����̎Q�ƍ��W
//		TempX2,TempY2	TempX1,TempY1����̕��A����
//		deg				��]�p�x
//		ScaleX			X���̔{�� 1.0�����{
//		ScaleY			X���̔{�� 1.0�����{
//		a				���`�����l��
//		r				�Ԃ̋���
//		g				�΂̋���
//		b				�̋���
//---------------------------------------------------------------------------------------
//Draw_Graphics(100,100,1.0f,TestImg,0,0,100,100,
//				90,1.0f,1.0f,255,255,255,255);
//---------------------------------------------------------------------------------------
void graph::Draw_Graphics(
	int x, int y, float z,
	const std::string& resname,
	int srcX, int srcY, int sizeX, int sizeY,
	int	degree,
	float	scaleX, float scaleY,
	u_char a, u_char r, u_char g, u_char b)
{
	graph::Draw_GraphicsNC(
		camera::camera->ixchange(x), camera::camera->iychange(y),
		z, resname,
		srcX, srcY, sizeX, sizeY,
		degree, nullptr,
		camera::camera->changeScale(scaleX),
		camera::camera->changeScale(scaleY),
		a, r, g, b);
}

void graph::Draw_Graphics(
	float x, float y, float z,
	const std::string& resname,
	int srcX, int srcY, int sizeX, int sizeY,
	int	degree,
	float	scaleX, float scaleY,
	u_char a, u_char r, u_char g, u_char b)
{
	graph::Draw_GraphicsNC(
		camera::camera->ixchange(x), camera::camera->iychange(y),
		z, resname,
		srcX, srcY, sizeX, sizeY,
		degree, nullptr,
		camera::camera->changeScale(scaleX),
		camera::camera->changeScale(scaleY),
		a, r, g, b);
}
//---------------------------------------------------------------------------------------
//IMGOBJ��\������B��]�A�g��A�F�ύX�Ή�
//	����
//		X1,Y1			�\���J�n���W
//		z				�\��Z�����W0.0 ����1.0�̊ԂŎw��@0.0�ɋ߂��قǎ�O�ɂȂ�
//		IMGOBJ			�`�悵����IMGNO
//		TempX1,TempY1	IMGNO����̎Q�ƍ��W
//		TempX2,TempY2	TempX1,TempY1����̕��A����
//		deg				��]�p�x
//		*pCenter		��]�̒��S
//		ScaleX			X���̔{�� 1.0�����{
//		ScaleY			X���̔{�� 1.0�����{
//		a				���`�����l��
//		r				�Ԃ̋���
//		g				�΂̋���
//		b				�̋���
//---------------------------------------------------------------------------------------
//Draw_Graphics(100,100,1.0f,TestImg,0,0,100,100,
//				90,&pt,1.0f,1.0f,255,255,255,255);
//---------------------------------------------------------------------------------------
void graph::Draw_GraphicsLeftTop(
	int x, int y, float z,
	const std::string& resname,
	int srcX, int srcY, int sizeX,int sizeY,
	int	degree, POINT *center,
	float	scaleX,float scaleY,
	u_char a, u_char r, u_char g, u_char b)
{
	graph::Draw_GraphicsLeftTopNC(
		camera::camera->ixchange(x), camera::camera->iychange(y),
		z, resname,
		srcX, srcY, sizeX, sizeY,
		degree, center, 
		camera::camera->changeScale(scaleX),
		camera::camera->changeScale(scaleY),
		a, r, g, b);
}
void graph::Draw_GraphicsLeftTop(
	float x, float y, float z,
	const std::string& resname,
	int srcX, int srcY, int sizeX,int sizeY,
	int	degree, POINT *center,
	float	scaleX,float scaleY,
	u_char a, u_char r, u_char g, u_char b)
{
	graph::Draw_GraphicsLeftTopNC(
		camera::camera->ixchange(x), camera::camera->iychange(y),
		z, resname,
		srcX, srcY, sizeX, sizeY,
		degree, center,
		camera::camera->changeScale(scaleX),
		camera::camera->changeScale(scaleY),
		a, r, g, b);
}

//---------------------------------------------------------------------------------------
//�摜�f�[�^��ǂݍ���
//---------------------------------------------------------------------------------------
//�߂�l�@
//�@�ǂݍ��܂ꂽ�t�@�C��������IMGNO(int)
//
//string resname	//�o�^���郊�\�[�X��
//string filename	//�ǂݍ��ރt�@�C����
//int Transparent	//���ߐF�̐ݒ�
//
//---------------------------------------------------------------------------------------
void graph::Draw_LoadObject(const std::string& resname, const std::string& filename, D3DCOLOR Transparent)
{
	//���ɓǂݍ��܂�Ă��郊�\�[�X���Ȃ�A�G���[
	if (ImgTable.count(filename) != 0){
		assert(!"�������O�̃��\�[�X���o�^�ς݂ł��B���O�̊m�F�����Ă��������B" );
	}
	for (const auto img : ImgTable){
		if (img.second.filename == filename){
			assert(!"�����t�@�C�����̃��\�[�X���o�^�ς݂ł��B���O�̊m�F�����Ă��������B");
		}
	}
	ImgObj img;
	//�ǂݍ��݊J�n
	if(FAILED(D3DXCreateTextureFromFileEx(
		pD3DDevice,
		filename.c_str(),	//�p�X
		D3DX_DEFAULT,	    //��
		D3DX_DEFAULT ,		//����
		D3DX_DEFAULT ,		//�~�b�v�}�b�v
		0,					//�����_�����O�^�[�Q�b�g�Ƃ��Ă������ǂ���
		D3DFMT_UNKNOWN,		//�e�N�X�`���t�@�C���t�H�[�}�b�g
		D3DPOOL_MANAGED,	//�������̎���
		D3DX_FILTER_NONE,	//�t�B���^�����O���@�i�g��k���̉��Z���@�j
		D3DX_DEFAULT,		//mip���̃t�B���^�����O���@
		Transparent,		//�����F�i0�w��Ŕ����F�����j
		&img.info,			//�ǂݍ��񂾉摜�̏��
		nullptr,			//�p���b�g�ւ̃|�C���^�i�g���ۂɖʓ|�Ȃ̂ŁA�����j
		&img.tex)			//�e�N�X�`���I�u�W�F�N�g�ւ̃|�C���^
	))
	{
		debug::BToMR("%s�̓ǂݍ��݂Ɏ��s���܂���", filename.c_str());
		assert(0);
		return ;
	}
	img.filename = filename;
	ImgTable.insert(std::unordered_map< std::string, ImgObj&>::value_type(resname, img));
}

void graph::Draw_DeleteObject(const std::string& resname)
{
	ImgTable.erase(resname);
}

void graph::Draw_2DClear()
{
	if (pSprite != NULL)
		pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

void graph::Draw_3DClear()
{
	//Scene Clear Black
	if (pD3DDevice != NULL)
		pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	//Draw Start
	if (pD3DDevice != NULL)
		pD3DDevice->BeginScene();

}

void graph::Draw_2DRefresh()
{
	if (pSprite != NULL)
		pSprite->End();

}

bool graph::Draw_3DRefresh()
{
	//Draw End
	if (pD3DDevice != NULL)
		pD3DDevice->EndScene();
	//ReDraw
	HRESULT hr;
	if (pD3DDevice != NULL)
		hr = pD3DDevice->Present(NULL, NULL, NULL, NULL);

	if (hr == D3DERR_DEVICELOST){
		//��ԃ`�F�b�N
		if (pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			pSprite->OnLostDevice();
			//���Z�b�g
			hr = pD3DDevice->Reset(&D3DPP);
			if (hr != D3D_OK){
				return false;
			}
			//�Ď擾
			pSprite->OnResetDevice();
		}
	}
	return true;
}

//---------------------------------------------------------------------------------------
//�`��I��
//---------------------------------------------------------------------------------------
void graph::Draw_Refresh()
{
	Draw_2DRefresh();
	if (Draw_3DRefresh() == false)
	{
		return;
	}
}
//---------------------------------------------------------------------------------------
//�`��J�n
//---------------------------------------------------------------------------------------
void graph::Draw_Clear()
{
	Draw_3DClear();
	Draw_2DClear();
}

//---------------------------------------------------------------------------------------
//DirectDraw������
//---------------------------------------------------------------------------------------
void system::InitDx()
{
	//DX MAIN INIT
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(pD3D == NULL){
		return;
	}

	//WINDOW MODE INIT
	ZeroMemory(&D3DPP,sizeof(D3DPP));
	if(system::WindowMode == true){
		D3DPP.Windowed			= TRUE;							//WINDOW MODE or FULL MODE
		D3DPP.SwapEffect		= D3DSWAPEFFECT_DISCARD;		//SWAP AUTO Select
		D3DPP.BackBufferFormat	= D3DFMT_UNKNOWN;				//FORMAT AUTO Select
		D3DPP.BackBufferCount	= 1;
		D3DPP.EnableAutoDepthStencil = TRUE;
		D3DPP.AutoDepthStencilFormat = D3DFMT_D16;
		D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}else{
		D3DPP.BackBufferWidth = system::WINW;
		D3DPP.BackBufferHeight = system::WINH;
		D3DPP.BackBufferCount	= 1;
		D3DPP.BackBufferFormat	= D3DFMT_X8R8G8B8;				//FORMAT Select
		D3DPP.Windowed			= FALSE;						//WINDOW MODE or FULL MODE
		D3DPP.SwapEffect		= D3DSWAPEFFECT_DISCARD;		//SWAP AUTO Select		
		D3DPP.EnableAutoDepthStencil = TRUE;
		D3DPP.AutoDepthStencilFormat = D3DFMT_D16;
		D3DPP.FullScreen_RefreshRateInHz = REFRESHRATE;
		D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}

	//Create Device
	HRESULT hr;
	//HAL(pure vp)
	if(FAILED(hr=pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,system::hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,&D3DPP,&pD3DDevice)))
	{
		//HAL(soft vp)
		if(FAILED(hr=pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,system::hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,&D3DPP,&pD3DDevice)))
		{
			//REF
			if(FAILED(hr=pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,system::hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,&D3DPP,&pD3DDevice)))
			{
				return ;
			}
		}
	}
	
	// ���ʕ`�惂�[�h�̎w��
    pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// �y��r���s�Ȃ�
    pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// �f�B�U�����O���s�Ȃ��i���i���`��j
	pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE);
	// �m�[���C�e�B���O���[�h 	
    pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	//--------------------------------------
	// �e�N�X�`���X�e�[�W�̐ݒ�
	//--------------------------------------
	// �e�N�X�`���X�e�[�W�̐ݒ� 
	pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
    pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
    pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE );

	// �e�N�X�`���t�B���^�[�̐ݒ�
	pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pD3DDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pD3DDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	D3DXCreateSprite(pD3DDevice,&pSprite);

}

//---------------------------------------------------------------------------------------
//DirectDraw�I��
//---------------------------------------------------------------------------------------
//�@�v���O�����I�����ɌĂяo������
//---------------------------------------------------------------------------------------
void system::DelDx()
{
	RELEASE(pSprite);
	RELEASE(pD3DDevice);
	RELEASE(pD3D);
}

//---------------------------------------------------------------------------------------
//���[�v����
//�������p
//---------------------------------------------------------------------------------------
bool System_LoopControl(int FPS){
	static int _Time = timeGetTime();	//�������Ԋi�[
	static float _FPS;					//FPS�v�Z�p�o�b�t�@
	if( FPS != 0){
		_FPS = 1000.0f / static_cast<float>(FPS);		//�^����ꂽFPS����FrameTime���v�Z	
		if(timeGetTime() - _Time > (unsigned)_FPS){		//FrameTime���o�߂���܂ő҂�
			_Time = timeGetTime();
			return 1;
		}
		return 0;
	}else{
		return 1;
	}
}

//---------------------------------------------------------------------------------------
//WINDOW�@�v���V�[�W��
//---------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT	ps;
	HRESULT res;
	switch (msg) {
		case WM_KEYDOWN: 
			if(wParam == VK_ESCAPE){ DestroyWindow(hWnd);}
			break;
		case WM_CREATE:
			res = timeBeginPeriod(1);
			break;
		case WM_PAINT:
			BeginPaint(hWnd,&ps);
			EndPaint(hWnd,&ps);
			break;
        case WM_DESTROY:
			timeEndPeriod(1);
			PostQuitMessage(0); 
			break;
		case WM_MOUSEWHEEL:
			input::delta = GET_WHEEL_DELTA_WPARAM(wParam);
			break;
		default: return(DefWindowProc(hWnd, msg, wParam, lParam)); 
    }
    return (0L);
}

#ifdef CI_EXT
ci_ext::TaskManager* ci_ext::Task::p_ = nullptr;
int ci_ext::Task::uid_ = 0;
#endif

//---------------------------------------------------------------------------------------
//WINDOW�@�ݒ�֐�
//---------------------------------------------------------------------------------------
int system::DoWindow(HINSTANCE hInstance,HINSTANCE hPreInst,LPSTR lpszCmdLine,int nCmdShow)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	MSG lpMsg;

	{

	WNDCLASS	myProg;
	LPCSTR		Name = "Window_Class_Name";
	if (!hPreInst)
	{                                        
		myProg.style            = CS_HREDRAW | CS_VREDRAW;
		myProg.lpfnWndProc      = WndProc;
		myProg.cbClsExtra       = 0;
		myProg.cbWndExtra       = 0;
		myProg.hInstance        = hInstance;
		myProg.hIcon            = LoadIcon(hInstance,"MAIN");
		myProg.hCursor          = LoadCursor(NULL, IDC_ARROW);
		myProg.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
		myProg.lpszMenuName     = NULL;
		myProg.lpszClassName    = (Name);
		if (!RegisterClass(&myProg)) return false;
	}

	RECT rc;
	SetRect(&rc, 0, 0, system::WINW, system::WINH);
	AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, 0);

	DWORD style;
	if (system::WindowMode) style = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU;
	else           style = WS_POPUP;

	//DX MAIN INIT
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL){
		return E_FAIL;
	}

	pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);		//�p�\�R����ʂ̏��

	//�p�\�R����ʂ̐^�񒆂ɒu��
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int windowCenterWidth = screenWidth / 2 - system::WINW / 2;
	int windowCenterHeight = screenHeight / 2 - system::WINH / 2;
	int height = rc.bottom - rc.top;

	system::hWnd=CreateWindow(
		(Name), 
		(system::USERNAME),
		style,
		windowCenterWidth, windowCenterHeight,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL);


	system::InitDx();
	//resource table init
	Draw_InitImgTable();
	Draw_InitFont();
	//player input init
	InitPlayerInput();
	for (int i = 0; i < system::PLAYER_NUM; i++)
		input::InitKeyInfo(i);
	//stage flag init
	stage::InitFlagStage();
	math::RandomInit();
	//sound com init
#ifdef CI_EXT 
	CoInitialize(NULL);
		//	�T�E���h���̏�����
	{
		ci_ext::MYERROR_DS ret_ds;	//	DirectSound�G���[�ԍ��i�[�p
		ret_ds = ci_ext::dsInitDirectSound(system::hWnd);
		if(ret_ds != ci_ext::eERRORDS_NOTHING)
		{
		//	�����������ł��Ȃ��̂ŁA�E�C���h�E���I��������
			std::string szMsg = ci_ext::dsGetErrorMessage(ret_ds);
			MessageBox(system::hWnd, szMsg.c_str(), NULL, 0);
			DestroyWindow(system::hWnd);
		}
	}
#endif
#ifdef USE_DIRECTSOUND
	se::DSound_Init();
#endif
#ifdef USE_DIRECTSHOW
	bgm::DShow_Init();
#endif
	srand((unsigned)::time(NULL));
	time::Time_CheckTimer();
	//user init
	system::OnCreate();
#ifdef CI_EXT
//	StartOutputDbgString();
  ShowConsole();
	ci_ext::TaskManager game;
	//TaskManager�̃|�C���^���^�X�N�ɓo�^���Ă���
	{ci_ext::TaskFirst dummy(&game);} //{}�͈Ӑ}�I�Ȃ̂ŏ������Ȃ�����
	game.changeScene(0);
#endif

	ShowWindow(system::hWnd,nCmdShow);
	UpdateWindow(system::hWnd);

	while(true){
		if(PeekMessage(&lpMsg,NULL,0,0,PM_REMOVE)){			//���b�Z�[�W����
			if(lpMsg.message == WM_QUIT)	break;		
			TranslateMessage(&lpMsg);
			DispatchMessage(&lpMsg);
		}else if( system::hWnd == GetActiveWindow()){
			graph::Draw_Clear();
			input::CheckKey();
			//�Q�[�����[�v
#ifdef CI_EXT
			if(game.run()) DestroyWindow(system::hWnd);
			ci_ext::dsCheck_Stream();
#else
			system::GameLoop();
#endif
			camera::StepCamera();
			graph::Draw_Refresh();
			//�T�E���h�I���`�F�b�N
#ifdef USE_DIRECTSHOW
			bgm::DShow_EndCheck();
#endif
#ifdef USE_DIRECTSOUND
			se::DSound_EndCheck();
#endif
			//�z�C�[��delta�N���A
			ClearWheelDelta();
			//TIMER�X�V
			system::FrameTime = time::Time_Update();
		}
	}
	//user release
	system::OnDestroy();			//�Q�[����n������
	//resource release
	Draw_DelImgTable();
	Draw_DeleteFont();
	//sound com release
#ifdef CI_EXT
	ci_ext::dsRelease();
#endif
#ifdef USE_DIRECTSOUND
	se::DSound_Del();

#endif
#ifdef USE_DIRECTSHOW
	bgm::DShow_Del();
#endif
	//directx release
	DelDx();
	//pad delete
	DeletePlayerInput();
	//memory leaks dump
//	_CrtDumpMemoryLeaks();
	}
	return static_cast<int>(lpMsg.wParam);
}

//********************************************************************//
//
//				BGM�Đ��@�֘A�֐� 
//
//********************************************************************//
#ifdef USE_DIRECTSHOW
//�֐��ꗗ
/*
bool		DShow_Init()						DShow�̏�����
void		DShow_Del();						DShow_�̌�n��
DSHOWOBJ	DShow_LoadFile(LPCWSTR)				���y�t�@�C���̓ǂݍ���
bool		DShow_Play();						�Đ�
void		DShow_RateControl(DSHOWOBJ,float)	�Đ����x�ύX
void		DShow_EndCheck()					�I���`�F�b�N
void		DShow_Stop(DSHOWOBJ)				�Đ��X�g�b�v
void		DShow_AllStop()						�S��~
LONGLONG	DShow_GetCurrentPos(int index)		�Đ��ʒu�擾
void		DShow_SetStartPos(int index)		�X�^�[�g�ʒu�ɐݒ�
*/
//-------------------------------------------------------
/*
�g����

	�O���[�o���Ƃ��Đ錾
	DSHOWOBJ		test;

	�Đ�����
	DShow_Play(test);		//�ʏ�Đ�-���ׂă��[�v�Đ�

	��~����
	DShow_Stop(test);		//�������������ׂĒ�~
	DShow_AllStop();		//�Đ����̂��ׂẲ������~

	�G�t�F�N�g
	DShow_RateControl(test,1.0);	//test�̍Đ����x���w�肵���{���ɕύX����B

*/
//--------------------------------------------------------------------------------------------
// DirectShow�̏�����
//--------------------------------------------------------------------------------------------
void DShow_ReleaseObj(DxShow& bgm){
	bgm.pMediaControl->Release();
	bgm.pMediaEvent->Release();
	bgm.pMediaSeeking->Release();
	bgm.pBuilder->Release();
	bgm.pBasicAudio->Release();
}

void	bgm::DShow_LoadFile(const std::string& resname, const std::string& filename)
{
	//���ɓǂݍ��܂�Ă��郊�\�[�X���Ȃ�A�G���[
	if (BgmTable.count(filename) != 0){
		assert(!"�������O�̃��\�[�X���o�^�ς݂ł��B���O�̊m�F�����Ă��������B");
	}
	for (const auto bgm : BgmTable){
		if (bgm.second.filename == filename){
			assert(!"�����t�@�C�����̃��\�[�X���o�^�ς݂ł��B���O�̊m�F�����Ă��������B");
		}
	}
	BgmObj bgmObj;
	GUID format = TIME_FORMAT_FRAME;
	wchar_t name[256];
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)filename.c_str(), -1, name, 256);

	HRESULT	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void **)&bgmObj.bgm.pBuilder);
	if (FAILED(hr)){
		debug::BToM("DShow:GraphBuilder�I�u�W�F�N�g�̐����Ɏ��s���܂���");
		goto error;
	}

	if (FAILED(bgmObj.bgm.pBuilder->RenderFile(name, NULL))){
		debug::BToM("DShow\n�t�@�C���ǂݍ��݂Ɏ��s���܂���\n�t�@�C�����A�R�[�f�b�N�̊m�F�����肢���܂��B");
		goto error;
	}
	if (FAILED(bgmObj.bgm.pBuilder->QueryInterface(IID_IMediaControl,
		(void**)&bgmObj.bgm.pMediaControl))){
		debug::BToMR("DShow\nMediaControl�̐ݒ�Ɏ��s���܂���");
		goto error;
	}
	if (FAILED(bgmObj.bgm.pBuilder->QueryInterface(IID_IMediaEvent,
		(void**)&bgmObj.bgm.pMediaEvent))){
		debug::BToMR("DShow\nMediaEvent�̐ݒ�Ɏ��s���܂���");
		goto error;
	}
	if (FAILED(bgmObj.bgm.pBuilder->QueryInterface(IID_IMediaSeeking,
		(void**)&bgmObj.bgm.pMediaSeeking))){
		debug::BToMR("DShow\nMediaSeeking�̐ݒ�Ɏ��s���܂���");
		goto error;
	}
	if (FAILED(bgmObj.bgm.pBuilder->QueryInterface(IID_IBasicAudio,
		(void**)&bgmObj.bgm.pBasicAudio))){
		debug::BToMR("DShow\nBasicAudio�̐ݒ�Ɏ��s���܂���");
		goto error;
	}

	bgmObj.bgm.pMediaSeeking->SetTimeFormat(&format);
	bgmObj.bgm.use = 1;
	bgmObj.filename = filename;
	BgmTable.insert(std::unordered_map<std::string, BgmObj>::value_type(resname, bgmObj));
	return;

error:
	DShow_ReleaseObj(bgmObj.bgm);

}
//--------------------------------------------------------------------------------------------
// DirectShow�̌�n��
//--------------------------------------------------------------------------------------------
void	bgm::DShow_Del()
{
	bgm::DShow_AllStop();
	for(auto& bgm : BgmTable){
		if (bgm.second.bgm.use == 1){
			DShow_ReleaseObj(bgm.second.bgm);
		}
	}
	CoUninitialize();
}

//--------------------------------------------------------------------------------------------
// GraphBuilder�̐���
//--------------------------------------------------------------------------------------------
void bgm::DShow_Init()
{
	CoInitialize(NULL);
}

//--------------------------------------------------------------------------------------------
// �t�@�C���̍Đ�
//--------------------------------------------------------------------------------------------
bool	bgm::DShow_Play(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return false;
	HRESULT	hr = BgmTable.at(resname).bgm.pMediaControl->Run();
	if(FAILED(hr)){
		debug::BToMR("DShow:�Đ��Ɏ��s���܂���1");
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------------
// �Đ����x�̍Đ�
//--------------------------------------------------------------------------------------------
void bgm::DShow_RateControl(const std::string& resname, float rate)
{
	if (BgmTable.at(resname).bgm.use == 0) return;
	if (rate < 0)	return;
	BgmTable.at(resname).bgm.pMediaSeeking->SetRate(rate);
}
//--------------------------------------------------------------------------------------------
// �Đ����ʂ̐ݒ�
// 0����100�Őݒ�@0�͖��� 100�͍ő� 
//--------------------------------------------------------------------------------------------
void bgm::DShow_VolumeControl(const std::string& resname, int volume)
{
	if (BgmTable.at(resname).bgm.use == 0) return;
	if (volume < 0 || volume > 100)	return;
	//0����-10000�Őݒ� 0���ő�@-10000�͖���
	//100�Ŋ������l���f�V�x��
	long vol = (long)(-10000 + (volume*100));
	BgmTable.at(resname).bgm.pBasicAudio->put_Volume(vol);
}
//--------------------------------------------------------------------------------------------
// ��~
//--------------------------------------------------------------------------------------------
void bgm::DShow_Stop(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return;
	LONGLONG	start = 0;
	BgmTable.at(resname).bgm.pMediaSeeking->SetPositions(&start, AM_SEEKING_AbsolutePositioning,
			NULL,AM_SEEKING_NoPositioning);
	BgmTable.at(resname).bgm.pMediaControl->Stop();
}

//--------------------------------------------------------------------------------------------
// �ꎞ��~
//--------------------------------------------------------------------------------------------
void bgm::DShow_Pause(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return;
	if (BgmTable.at(resname).bgm.use == 1){
		BgmTable.at(resname).bgm.pMediaControl->Stop();
	}
}

//--------------------------------------------------------------------------------------------
// �S��~
//--------------------------------------------------------------------------------------------
void bgm::DShow_AllStop()
{
	for(auto& bgm : BgmTable){
		if(bgm.second.bgm.use == 1){
			bgm.second.bgm.pMediaControl->Stop();
		}
	}
}

//--------------------------------------------------------------------------------------------
// �I���m�F
//--------------------------------------------------------------------------------------------
void bgm::DShow_EndCheck()
{
	for (auto& bgm : BgmTable){
		if (bgm.second.bgm.use == 1){
//			long code;
			//�Đ��ʒu�擾
			LONGLONG endPos = bgm::DShow_GetEndPosition(bgm.first);
			LONGLONG nowPos = bgm::DShow_GetCurrentPos(bgm.first);
			//�u���b�L���O����悤�Ȃ̂ł��
			//bgm.second.bgm.pMediaEvent->WaitForCompletion(0, &code);
			//�Đ��I�����̓��[�v���s���B
//			if(code == EC_COMPLETE){
			if (endPos <= nowPos){
				LONGLONG	start = 0;
				bgm.second.bgm.pMediaSeeking->SetPositions(&start, AM_SEEKING_AbsolutePositioning,
					NULL,AM_SEEKING_NoPositioning);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// �Đ��I���ʒu�擾
//--------------------------------------------------------------------------------------------
LONGLONG bgm::DShow_GetEndPosition(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return -1L;
	LONGLONG	end;
	//�I���ʒu�̎擾
	BgmTable.at(resname).bgm.pMediaSeeking->GetStopPosition(&end);
	return end;
}

//--------------------------------------------------------------------------------------------
// �Đ��ʒu�擾
//--------------------------------------------------------------------------------------------
LONGLONG bgm::DShow_GetCurrentPos(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return -1L;
	LONGLONG	Current;
	BgmTable.at(resname).bgm.pMediaSeeking->GetCurrentPosition(&Current);
	return Current;
}

//--------------------------------------------------------------------------------------------
// �Đ��ʒu������
//--------------------------------------------------------------------------------------------
void bgm::DShow_SetStartPos(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return;
	LONGLONG	start = 0;
	BgmTable.at(resname).bgm.pMediaSeeking->SetPositions(&start, AM_SEEKING_AbsolutePositioning,
			NULL,AM_SEEKING_NoPositioning);
}


//********************************************************************//
//
//				BGM�Đ��@�֘A�֐� �����܂�
//
//********************************************************************//
#endif

//********************************************************************//
//
//				SE�Đ��@�֘A�֐�
//
//********************************************************************//
#ifdef USE_DIRECTSOUND
//--------------------------------------------------------------------------------------------
// DirectSound�̏�����
//--------------------------------------------------------------------------------------------
void se::DSound_Init()
{
	if(se::DSound_Create()){
		if(!DSound_CreatePrimaryBuffer()){
			debug::BToMR("�v���C�}���o�b�t�@�̐����Ɏ��s���܂���");
		}
		DSound_CreateSecondaryBuffer();
	}else{
		debug::BToMR("DSound�̐����Ɏ��s���܂���");
	}

}

//--------------------------------------------------------------------------------------------
// DirectSound�̍쐬
//--------------------------------------------------------------------------------------------
bool se::DSound_Create( void )
{
    HRESULT ret;

    // DirectSound8���쐬
    ret = DirectSoundCreate8( NULL,&lpDS,NULL );
    if( FAILED(ret) ) {
			debug::BToMR("�T�E���h�I�u�W�F�N�g�쐬���s\n", "");
        return false;
    }

    // �������[�h
    ret = lpDS->SetCooperativeLevel( system::hWnd,DSSCL_EXCLUSIVE|DSSCL_PRIORITY );
    if( FAILED(ret) ) {
			debug::BToMR("�������x���ݒ莸�s\n", "");
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------
// �o�b�t�@�̍쐬
//--------------------------------------------------------------------------------------------
void se::DSound_CreateSecondaryBuffer()
{
	SeTable.clear();
}

//--------------------------------------------------------------------------------------------
// �T�E���h�Đ�
//--------------------------------------------------------------------------------------------
void se::DSound_Play(const std::string& resname)
{
	int i;
	auto& se = SeTable.at(resname).se;
	for (i = 0; i < DSPLAYMAX; i++){
		if (se.PlayBuffer[i].State == DSNONE){
			//�Đ��J�n
//			se.PlayBuffer[i].ID = SoundNo;
			se.PlayBuffer[i].State = DSPLAY;
			se.PlayBuffer[i].pBuffer->Play(0, 0, 0);
			se.PlayBuffer[i].Loop = 0;
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------
// �T�E���h���[�v�Đ�
//--------------------------------------------------------------------------------------------
void se::DSound_PlayLoop(const std::string& resname)
{
	int i;
	auto& se = SeTable.at(resname).se;
	for (i = 0; i < DSPLAYMAX; i++){
		if (se.PlayBuffer[i].State == DSNONE){
			//�Đ��J�n
//			se.PlayBuffer[i].ID = SoundNo;
			se.PlayBuffer[i].State = DSPLAY;
			se.PlayBuffer[i].pBuffer->Play(0, 0, 0);
			se.PlayBuffer[i].Loop = 1;
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------
// �T�E���h��~
//	�������ʉ������ׂĒ�~����
//--------------------------------------------------------------------------------------------
void se::DSound_Stop(const std::string& resname)
{
	auto& se = SeTable.at(resname).se;
	for (int j = 0; j < DSPLAYMAX; j++){
		if (se.PlayBuffer[j].State == DSPLAY){
			se.PlayBuffer[j].pBuffer->Stop();
			se.PlayBuffer[j].State = DSNONE;
		}
	}
}

//--------------------------------------------------------------------------------------------
// ���ׂẴT�E���h��~
//--------------------------------------------------------------------------------------------
void se::DSound_AllStop()
{
	for (auto& seObj : SeTable){
		for (int j = 0; j < DSPLAYMAX; j++){
			if (seObj.second.se.PlayBuffer[j].State == DSPLAY){
				seObj.second.se.PlayBuffer[j].pBuffer->Stop();
				seObj.second.se.PlayBuffer[j].State = DSNONE;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// �I���`�F�b�N
//--------------------------------------------------------------------------------------------
void se::DSound_EndCheck()
{
	DWORD Status;
	int a = 0;
	for (auto& seObj : SeTable){
		for(int j = 0 ; j < DSPLAYMAX ; j++){
			if(seObj.second.se.PlayBuffer[j].State == DSPLAY){		//���ݍĐ����̕����o�b�t�@
				//�I���ƔF������
				seObj.second.se.PlayBuffer[j].pBuffer->GetStatus(&Status);
				if( (Status & DSBSTATUS_PLAYING) != 1){
					if (seObj.second.se.PlayBuffer[j].Loop){
						seObj.second.se.PlayBuffer[j].pBuffer->SetCurrentPosition(0);
						seObj.second.se.PlayBuffer[j].pBuffer->Play(0, 0, 0);
					}else{
						seObj.second.se.PlayBuffer[j].State = DSNONE;
						seObj.second.se.PlayBuffer[j].BufferSize = 0;
						seObj.second.se.PlayBuffer[j].pBuffer->Stop();
					}
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
// �v���C�}���T�E���h�o�b�t�@�̍쐬
//--------------------------------------------------------------------------------------------
bool se::DSound_CreatePrimaryBuffer(void)
{
    HRESULT ret;
    WAVEFORMATEX wf;

    // �v���C�}���T�E���h�o�b�t�@�̍쐬
    DSBUFFERDESC dsdesc;
    ZeroMemory( &dsdesc,sizeof(DSBUFFERDESC) );
    dsdesc.dwSize = sizeof( DSBUFFERDESC );
    dsdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    dsdesc.dwBufferBytes = 0;
    dsdesc.lpwfxFormat = NULL;
    ret = lpDS->CreateSoundBuffer( &dsdesc,&lpPrimary,NULL );
    if( FAILED(ret) ) {
			debug::BToMR("�v���C�}���T�E���h�o�b�t�@�쐬���s\n", "");
        return false;
    }

    // �v���C�}���o�b�t�@�̃X�e�[�^�X������
    wf.cbSize = sizeof( WAVEFORMATEX );
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = 2;
    wf.nSamplesPerSec = 44100;
    wf.wBitsPerSample = 16;
    wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
    ret = lpPrimary->SetFormat( &wf );
    if( FAILED(ret) ) {
			debug::BToMR("�v���C�}���o�b�t�@�̃X�e�[�^�X���s\n", "");
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------
//DSound�̌�n��
//--------------------------------------------------------------------------------------------
bool se::DSound_Del(void)
{
	for (auto& seObj : SeTable){
		if (seObj.second.se.BufferSize != 0){
			seObj.second.se.sBuffer->Release();
		}
	}
    if( lpPrimary ) {
        lpPrimary->Release();
        lpPrimary = NULL;
    }
    if( lpDS ) {
        lpDS->Release();
        lpDS = NULL;
    }

    return true;
}

//--------------------------------------------------------------------------------------------
// �T�E���h�o�b�t�@�̍쐬
//--------------------------------------------------------------------------------------------
void se::DSound_LoadFile(const std::string& resname, const std::string& filename)
{
	HRESULT ret;
	MMCKINFO mSrcWaveFile;
	MMCKINFO mSrcWaveFmt;
	MMCKINFO mSrcWaveData;
  LPWAVEFORMATEX wf;

	//���ɓǂݍ��܂�Ă��郊�\�[�X���Ȃ�A�G���[
	if (SeTable.count(filename) != 0){
		assert(!"DSound:�������O�̃��\�[�X���o�^�ς݂ł��B���O�̊m�F�����Ă��������B");
	}
	for (const auto& se : SeTable){
		if (se.second.filename == filename){
			assert(!"DSound:�����t�@�C�����̃��\�[�X���o�^�ς݂ł��B���O�̊m�F�����Ă��������B");
		}
	}

	// WAV�t�@�C�������[�h
	HMMIO hSrc;
	hSrc = mmioOpen( (LPSTR)filename.c_str(),NULL,MMIO_ALLOCBUF|MMIO_READ|MMIO_COMPAT );
	if( !hSrc ) {
		debug::BToMR("WAV�t�@�C�����[�h�G���[\n", "");
			return;
	}

	// 'WAVE'�`�����N�`�F�b�N
	ZeroMemory( &mSrcWaveFile,sizeof(mSrcWaveFile) );
	ret = mmioDescend( hSrc,&mSrcWaveFile,NULL,MMIO_FINDRIFF );
	if( mSrcWaveFile.fccType!=mmioFOURCC('W','A','V','E') ) {
		debug::BToMR("WAVE�`�����N�`�F�b�N�G���[\n", "");
			mmioClose( hSrc,0 );
			return;
	}

	// 'fmt '�`�����N�`�F�b�N
	ZeroMemory( &mSrcWaveFmt,sizeof(mSrcWaveFmt) );
	ret = mmioDescend( hSrc,&mSrcWaveFmt,&mSrcWaveFile,MMIO_FINDCHUNK );
	if( mSrcWaveFmt.ckid!=mmioFOURCC('f','m','t',' ') ) {
		debug::BToMR("fmt �`�����N�`�F�b�N�G���[\n", "");
			mmioClose( hSrc,0 );
			return;
	}

	// �w�b�_�T�C�Y�̌v�Z
	int iSrcHeaderSize = mSrcWaveFmt.cksize;
	if( iSrcHeaderSize<sizeof(WAVEFORMATEX) )
			iSrcHeaderSize=sizeof(WAVEFORMATEX);

	// �w�b�_�������m��
	wf = (LPWAVEFORMATEX)malloc( iSrcHeaderSize );
	if( !wf ) {
		debug::BToMR("�������m�ۃG���[\n", "");
			mmioClose( hSrc,0 );
			return;
	}
	ZeroMemory( wf,iSrcHeaderSize );

	// WAVE�t�H�[�}�b�g�̃��[�h
	ret = mmioRead( hSrc,(char*)wf,mSrcWaveFmt.cksize );
	if( FAILED(ret) ) {
		debug::BToMR("WAVE�t�H�[�}�b�g���[�h�G���[\n", "");
			free( wf );
			mmioClose( hSrc,0 );
			return;
	}

	// fmt�`�����N�ɖ߂�
	mmioAscend( hSrc,&mSrcWaveFmt,0 );

	// data�`�����N��T��
	while(1) {
		// ����
		ret = mmioDescend( hSrc,&mSrcWaveData,&mSrcWaveFile,0 );
		if( FAILED(ret) ) {
			debug::BToMR("data�`�����N��������Ȃ�\n", "");
			free( wf );
			mmioClose( hSrc,0 );
			return;
		}
		if (mSrcWaveData.ckid == mmioStringToFOURCC("data", 0)){
			break;
		}
		// ���̃`�����N��
		ret = mmioAscend( hSrc,&mSrcWaveData,0 );
	}

	SeObj seObj;
	// �T�E���h�o�b�t�@�̍쐬
	DSBUFFERDESC dsdesc;
	ZeroMemory( &dsdesc,sizeof(DSBUFFERDESC) );
	dsdesc.dwSize = sizeof( DSBUFFERDESC );
	dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME;
	dsdesc.dwBufferBytes = mSrcWaveData.cksize;
	dsdesc.lpwfxFormat = wf;
	dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	ret = lpDS->CreateSoundBuffer(&dsdesc, &seObj.se.sBuffer, NULL);
	seObj.se.BufferSize = (WORD)dsdesc.dwBufferBytes;
	if( FAILED(ret) ) {
		debug::BToMR("�T�E���h�o�b�t�@�̍쐬�G���[\n", "");
		free( wf );
		mmioClose( hSrc,0 );
		return;
	}

	// ���b�N�J�n
	LPVOID pMem1,pMem2;
	DWORD dwSize1,dwSize2;
	ret = (seObj.se.sBuffer)->Lock(0, mSrcWaveData.cksize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0);
	seObj.se.sBuffer->GetFrequency(&seObj.se.Fre);
	if( FAILED(ret) ) {
		debug::BToMR("���b�N���s\n", "");
			free( wf );
			mmioClose( hSrc,0 );
			return;
	}

	// �f�[�^��������
	mmioRead( hSrc,(char*)pMem1,dwSize1 );
	mmioRead( hSrc,(char*)pMem2,dwSize2 );

	// ���b�N����
	(seObj.se.sBuffer)->Unlock(pMem1, dwSize1, pMem2, dwSize2);
	// �w�b�_�p���������J��
	free( wf );
	// WAV�����
	mmioClose( hSrc,0 );

	seObj.se.sBuffer->SetVolume(-50);

	//�����J�n
	for(int t = 0 ; t < 10 ; t++){
		if (lpDS->DuplicateSoundBuffer(seObj.se.sBuffer,
			&(seObj.se.PlayBuffer[t].pBuffer)) != DS_OK){
			debug::BToMR("�Đ��p�o�b�t�@�쐬���s\n", "");
			break;
		}else{
			seObj.se.PlayBuffer[t].State = DSNONE;
			seObj.se.PlayBuffer[t].BufferSize = seObj.se.BufferSize;
			seObj.se.PlayBuffer[t].Loop = 0;
		}
	}
	SeTable.insert(std::unordered_map<std::string, SeObj>::value_type(resname, seObj));
}


//--------------------------------------------------------------------------------------------
// ���g���ύX �Đ����x���ς��
//--------------------------------------------------------------------------------------------
void se::DSound_SetFrequency(const std::string& resname, int Fre)
{
	auto se = SeTable.at(resname).se;
	se.sBuffer->SetFrequency(se.Fre + Fre);
}

void se::DSound_SetVolume(const std::string& resname, int Vol)
{
	auto se = SeTable.at(resname).se;
	long volume;
    if(Vol >= 1.0f){
        volume = DSBVOLUME_MAX;
    }else if(Vol <= 0.0f){
        volume = DSBVOLUME_MIN;
	}else{
		volume = static_cast<long>((10 * log10((double)(Vol/100))) * 100); 
	}

	HRESULT hr = se.sBuffer->SetVolume(volume);
}
//********************************************************************//
//
//				SE�Đ��@�֘A�֐� �����܂�
//
//********************************************************************//
#endif

//********************************************************************//
//
//						���w�֘A
//
//********************************************************************//
//-------------------------------------------------------------//
//��`���m�̏d�Ȃ�𒲂ׂ�
//-------------------------------------------------------------//
bool math::RectCheck(const RECT& rt1,const RECT& rt2)
{
	return (rt1.left < rt2.right && 
          rt2.left < rt1.right && 
		      rt1.top  < rt2.bottom &&
					rt2.top  < rt1.bottom);
}
bool math::RectCheck(const RECT *rt1, const RECT *rt2)
{
	return RectCheck(*rt1, *rt2);
}

//-------------------------------------------------------------//
//�w����Wxy����ʊO�ɂ��邩�𒲂ׂ܂�
//-------------------------------------------------------------//
bool math::IsFrameOut_Center(float x, float y, float width, float height)
{
	width  /= 2.0f;
	height /= 2.0f;
	x -= camera::camera->viewLeft();
	y -= camera::camera->viewTop();
	return ((x - width) < 0 || (y - height) < 0 || (x + width) > system::WINW || (y + height) > system::WINH);
}
bool math::IsFrameOut_LeftTop(float x, float y, float width, float height)
{
	x -= camera::camera->viewLeft();
	y -= camera::camera->viewTop();
	return (x < 0 || y < 0 || (x + width) > system::WINW || (y + height) > system::WINH);
}
#ifdef CI_EXT
bool IsFrameOut_Center(const ci_ext::Vec3f& pos, const ci_ext::Vec3f& size)
{
	return IsFrameOut_Center(pos.x(), pos.y(), size.x(), size.y());
}
bool IsFrameOut_LeftTop(const ci_ext::Vec3f& pos, const ci_ext::Vec3f& size)
{
	return IsFrameOut_LeftTop(pos.x(), pos.y(), size.x(), size.y());
}

#endif

//-------------------------------------------------------------//
//��X�y�э���Y�̒l����p�x�Ƃ����߁ACosSin�̒l�����߂�
//�߂�l�@���W�A��
//-------------------------------------------------------------//
// ��
//float rad = CalcXYToSinCos(30,30);
//-------------------------------------------------------------//
float math::Calc_XYToRad(int x, int y)
{
	return static_cast<float>(atan2((double)y,(double)x));
}

//-------------------------------------------------------------//
//��X�y�э���Y�̒l����p�x�Ƃ����߁ACosSin�̒l�����߂�
//�߂�l�@���W�A��
//-------------------------------------------------------------//
// ��
//float rad = CalcXYToSinCos(30,30,&AddPx,&AddPy);
//-------------------------------------------------------------//
float math::Calc_XYToSinCos(int x, int y, float *addx, float *addy)
{
	float rad = atan2(static_cast<float>(y), static_cast<float>(x));
	*addx = cos(rad);
	*addy = -sin(rad);

	return rad;
}

//-------------------------------------------------------------//
//���W�A���l����p�x�ɕϊ�����
//�߂�l�@float �p�x
//-------------------------------------------------------------//
//float deg = RadToDegree(r);
//-------------------------------------------------------------//
float math::Calc_RadToDegree(float rad)
{
	float d = (float)(rad * 180.0f / math::PAI);
	if(d < 0 ) d += 360;
	return d;
}
//-------------------------------------------------------------//
//�p�x���烉�W�A���l�ɕϊ�����
//�߂�l�@float ���W�A��
//-------------------------------------------------------------//
//float rad = DegreeToRad(180);
//-------------------------------------------------------------//
float math::Calc_DegreeToRad(float degree)
{
	return static_cast<float>(degree * 2.0f * M_PI / 360.0f);
}
//---------------------------------------------------------------------------------------
//�R�T�C�������߂�@�p�x�ɑ΂��Ă�X���̒��������߂�
//---------------------------------------------------------------------------------------
float math::ROUND_X(float angle, float length, float center)
{
	return (float)(cos(angle * M_PI * 2 / 360) * length) + center;
}
//---------------------------------------------------------------------------------------
//�T�C�������߂�@�p�x�ɑ΂��Ă�Y���̒��������߂�
//---------------------------------------------------------------------------------------
float math::ROUND_Y(float angle, float length, float center)
{
	return (float)(sin(-angle * M_PI * 2 / 360) * length) + center;
}
//---------------------------------------------------------------------------------------
//�R�T�C�������߂�F�p�x�ɑ΂��Ă�Z���̒��������߂�
//---------------------------------------------------------------------------------------
float math::ROUND_Z(float angle, float length, float center)
{
	return (float)(tan(angle * M_PI * 2 / 360) * length) + center;
}

//---------------------------------------------------------------------------------------
//�l�p�`�𔽓]����B
//---------------------------------------------------------------------------------------
void math::reverseRect(RECT& baseRect, int width)
{
	RECT temp;
	SetRect(&temp, width - (baseRect.right), baseRect.top,
		width - (baseRect.left), baseRect.bottom);
	CopyRect(&baseRect, &temp);
}
//********************************************************************//
//
//				End Of File
//
//********************************************************************//

