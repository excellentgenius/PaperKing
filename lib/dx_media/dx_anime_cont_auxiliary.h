
//**************************************************************************
//�A�j���[�V�����R���g���[���⏕�N���X
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#pragma once

#include "dx.h"

//�A�j���[�V�����R���g���[���⏕�N���X
class Dx_AnimeAuxiliary
{
	private:
		//�A�j���[�V�����R���g���[��
		LPD3DXANIMATIONCONTROLLER lpAnimCont;

		//�o�^�g���b�N��
		short max_num_track;

	public:
		//�R���X�g���N�^
		Dx_AnimeAuxiliary();
		Dx_AnimeAuxiliary(LPD3DXANIMATIONCONTROLLER lpAnimCont);

		//�A�j���[�V��������
		void PlayAnimation(double speed);

		//�A�j���[�V�����R���g���[���̐ݒu
		void SetAnimController(LPD3DXANIMATIONCONTROLLER lpAnimCont);

		//�g���b�N���̐ݒu
		void SetTrackDesc(short no,D3DXTRACK_DESC desc);
		//�g���b�N���̎擾
		void GetTrackDesc(short no,LPD3DXTRACK_DESC desc);
		//�A�j���[�V�����̐؂�ւ�
		void ChangeAnim(int ID);
		//�A�j���[�V�����̏I���`�F�b�N
		BOOL CheckAnimEnd(int ID);

};
