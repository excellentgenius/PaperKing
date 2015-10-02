
//**************************************************************************
//�A�j���[�V�����R���g���[���⏕�N���X
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#include "dx_anime_cont_auxiliary.h"


//�R���X�g���N�^
Dx_AnimeAuxiliary::Dx_AnimeAuxiliary()
{
	//�A�j���[�V�����R���g���[���|�C���^�̏�����
	this->lpAnimCont = NULL;
	//�ő�g���b�N���̏�����
	this->max_num_track = 0;
}
//�R���X�g���N�^�@�@�i�A�j���[�V�����R���g���[���ݒu�^�j
Dx_AnimeAuxiliary::Dx_AnimeAuxiliary(LPD3DXANIMATIONCONTROLLER lpAnimCont)
{
	//�A�j���[�V�����R���g���[���|�C���^�̏�����
	this->lpAnimCont = NULL;
	//�ő�g���b�N���̏�����
	this->max_num_track = 0;

	this->SetAnimController(lpAnimCont);
}

//�A�j���[�V��������
void Dx_AnimeAuxiliary::PlayAnimation(double speed)
{
	if(this->lpAnimCont!=NULL)
	{
		this->lpAnimCont->AdvanceTime(speed,NULL);
	}
}

//�A�j���[�V�����R���g���[���̐ݒu
void Dx_AnimeAuxiliary::SetAnimController(LPD3DXANIMATIONCONTROLLER lpAnimCont)
{
	//
	if(lpAnimCont!=NULL)
	{
		//�A�j���[�V�����R���g���[���̃A�h���X��o�^
		this->lpAnimCont = lpAnimCont;

		//�o�^�g���b�N�����擾
		this->max_num_track = lpAnimCont->GetMaxNumTracks();
	}
}

//�g���b�N���̐ݒu
void Dx_AnimeAuxiliary::SetTrackDesc(short no,D3DXTRACK_DESC desc)
{
	if(no < this->max_num_track)
	{
		this->lpAnimCont->SetTrackDesc(no,&desc);
	}
}
//�g���b�N���̎擾
void Dx_AnimeAuxiliary::GetTrackDesc(short no,LPD3DXTRACK_DESC desc)
{
	if(no < this->max_num_track)
	{
		this->lpAnimCont->GetTrackDesc(no,desc);
	}
}

//�A�j���[�V�����̐؂�ւ�
void Dx_AnimeAuxiliary::ChangeAnim(int ID)
{
	D3DXTRACK_DESC desc;
	//��x�g���b�N���N���A����
	for(int i = 0 ; i <= this->max_num_track ; i ++)
	{	
		this->lpAnimCont->GetTrackDesc(i,&desc);
		desc.Enable = FALSE;
		desc.Position = 0.0f;
		desc.Speed = 0.0f;
		desc.Weight = 0;
		this->lpAnimCont->SetTrackDesc(i,&desc);
	}
	//���̌�K�v�ȃg���b�N�̒ǉ����s��
	this->lpAnimCont->GetTrackDesc(ID,&desc);
	desc.Enable = TRUE;
	desc.Position = 0.0f;
	desc.Speed = 0.1f;
	desc.Weight = 10;
	this->lpAnimCont->SetTrackDesc(ID,&desc);
	this->lpAnimCont->AdvanceTime(0.0f,NULL);

}

//�w��̃A�j���[�V�����̏I���𒲂ׂ�
BOOL Dx_AnimeAuxiliary::CheckAnimEnd(int ID)
{
	D3DXTRACK_DESC desc;
	this->lpAnimCont->GetTrackDesc(ID,&desc);
	//���̃��[�V�������P���[�v�I�������
	if(desc.Position >= 1.0f){
		return TRUE;
	}
	return FALSE;
}
