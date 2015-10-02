
//**************************************************************************
//�J�����N���X
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#include "dx_Camera.h"


//�R���X�g���N�^
Dx_Camera::Dx_Camera(float lx,float ly,float lz)
{
	//�ˉe�^�C�v�̐ݒ�
	this->mat_type = this->DX_PERS_MAT;

	//���_���W�̐ݒ�
	this->SetLookPos(lx,ly,lz);
	//�������W�̐ݒ�
	this->SetInitPos(4.0f,0,0);
}

//�������֐�
void Dx_Camera::Init()
{
	this->SetEyePos(this->init_pos);

	this->InitRotation();

	this->SetupMatrix();
}
//����֐�
void Dx_Camera::Step()
{
	/*��]�֘A*/
	this->Rotation();

	/*�J�����{�̐ݒ�*/
	this->SetupMatrix();
}

//
void Dx_Camera::SetupMatrix()
{
	//�r���[�s��̐ݒ�
	D3DXMatrixLookAtLH(&this->view_mat,&this->eye_pos,&this->look_pos,&DxVec3(0.0f,1.0f,0.0f));

	//�ˉe�^�C�v�ɂ���Đݒ肷��
	switch(this->mat_type)
	{
		//���ߎˉe�s����g�p����ꍇ
		case this->DX_PERS_MAT:
			//���ߎˉe�s��̐ݒ�
			D3DXMatrixPerspectiveFovLH(&this->projection_mat,D3DXToRadian(90.0f),640/480,0.1f,200.0f);
			break;
		//���ˉe�s����g�p����ꍇ
		case this->DX_ORTHO_MAT:
			//���ˉe�s��̐ݒ�
			D3DXMatrixOrthoLH(&this->projection_mat,8,8,0,20);
			break;
	}
}

//��]�֐�
void Dx_Camera::Rotation()
{
	//��]��Ԃ̂Ƃ��̂ݐ���
	if(this->rot_state!=0)
	{
		D3DXMATRIX mat;
		//�J�������W�Ǝ��_���W�̍����擾
		DxVec3 sub = this->eye_pos - this->look_pos;

		//�c����]
		if(this->rot_state&VERTICAL_ROT)
		{
			//��]�s��̐ݒ�
			D3DXMatrixRotationAxis(&mat,&this->vertical_axis,D3DXToRadian(this->vertical_add));
			//���x�N�g������]
			D3DXVec3TransformCoord(&sub,&sub,&mat);
			D3DXVec3TransformCoord(&this->cross_axis,&this->cross_axis,&mat);
			//���݂̐���p�x�ɑ����ʂ����Z
			this->now_vertical_rot += this->vertical_add;
			//���݂̐���p�x�ƏI���p�x�̍������r�����ʂ�菭�Ȃ�����]�I��
			if(abs(this->vertical_end-this->now_vertical_rot) < abs(this->vertical_add) || this->vertical_add==0.0f)
			{
				//�X�e�[�^�X����VERTICAL_ROT���폜
				this->rot_state ^= VERTICAL_ROT;
				//���݂�Yaw�p�x�̒���
				if(this->now_vertical_rot >= 360)
				{
					this->now_vertical_rot -= 360;
				}
				if(this->now_vertical_rot < 0)
				{
					this->now_vertical_rot += 360;
				}
			}
		}

		//������]
		if(this->rot_state&CROSS_ROT)
		{
			//��]�s��̐ݒ�
			D3DXMatrixRotationAxis(&mat,&this->cross_axis,D3DXToRadian(this->cross_add));
			//���x�N�g������]
			D3DXVec3TransformCoord(&sub,&sub,&mat);
			//���݂̐���p�x�ɑ����ʂ����Z
			this->now_cross_rot += this->cross_add;
			//���݂̐���p�x�ƏI���p�x�̍������r�����ʂ�菭�Ȃ�����]�I��
			if(abs(this->cross_end-this->now_cross_rot) < abs(this->cross_add) || this->cross_add==0.0f)
			{
				//�X�e�[�^�X����CROSS_ROT���폜
				this->rot_state ^= CROSS_ROT;
				//���݂�Yaw�p�x�̒���
				if(this->now_cross_rot >= 180)
				{
					this->now_cross_rot -= 360;
				}
				if(this->now_cross_rot < -180)
				{
					this->now_cross_rot += 360;
				}
			}
		}

		D3DXVec3Add(&this->eye_pos,&this->look_pos,&sub);
	}
	this->dot_ver_cross = D3DXVec3Dot(&this->vertical_axis,&this->cross_axis);
}
void Dx_Camera::InitRotation()
{
	//�c���ݒ�
	this->vertical_axis = DxVec3(0,1,0);
	//�����ݒ�
	this->cross_axis = DxVec3(0,0,1);

	//��]���
	this->rot_state = 0;
	this->now_vertical_rot = 0;
	this->now_cross_rot = 0;
}
//���݂̉�]��Ԃ��擾
short Dx_Camera::GetRotState()
{
	return this->rot_state;
}
//�w��p�x�܂Ŏ�����]
void Dx_Camera::StartVerticalRot(float add_deg,short frame_val)
{
	if(!(this->rot_state&VERTICAL_ROT))
	{
		//�I���p�x��ݒ�
		this->vertical_end = this->now_vertical_rot + add_deg;
		//�����ʂ�ݒ�
		this->vertical_add = add_deg / frame_val;

		D3DXMATRIX mat;
		//�J�������W�Ǝ��_���W�̍����擾
		DxVec3 sub = this->eye_pos - this->look_pos;
		//��]�s��̐ݒ�
		D3DXMatrixRotationY(&mat,D3DXToRadian(this->vertical_end));
		//���x�N�g������]
		D3DXVec3TransformCoord(&sub,&sub,&mat);

		//��]��Ԃ�Y����]�X�^�[�g��ݒ�
		this->rot_state |= VERTICAL_ROT;
	}
}
//�w��p�x�܂Ŏ�����]
void Dx_Camera::StartCrossRot(float add_deg,short frame_val)
{
	if(!(this->rot_state&CROSS_ROT))
	{
		//�I���p�x��ݒ�
		this->cross_end = this->now_cross_rot + add_deg;
		//�����ʂ�ݒ�
		this->cross_add = add_deg / frame_val;

		D3DXMATRIX mat;
		//�J�������W�Ǝ��_���W�̍����擾
		DxVec3 sub = this->eye_pos - this->look_pos;
		//��]�s��̐ݒ�
		D3DXMatrixRotationAxis(&mat,&this->vertical_axis,D3DXToRadian(this->cross_end));
		//���x�N�g������]
		D3DXVec3TransformCoord(&sub,&sub,&mat);

		//��]��Ԃ�Y����]�X�^�[�g��ݒ�
		this->rot_state |= this->CROSS_ROT;
	}
}
//
void Dx_Camera::SetVerticalRot(float deg)
{
	if(!(this->rot_state&VERTICAL_ROT))
	{
		//�I���p�x��ݒ�
		this->vertical_end = deg;
		//�����ʂ�ݒ�
		this->vertical_add = deg - this->now_vertical_rot;

		D3DXMATRIX mat;
		//�J�������W�Ǝ��_���W�̍����擾
		DxVec3 sub = this->eye_pos - this->look_pos;
		//��]�s��̐ݒ�
		D3DXMatrixRotationY(&mat,D3DXToRadian(this->vertical_end));
		//���x�N�g������]
		D3DXVec3TransformCoord(&sub,&sub,&mat);

		//��]��Ԃ�Y����]�X�^�[�g��ݒ�
		this->rot_state |= VERTICAL_ROT;
	}
}
//
void Dx_Camera::SetCrossRot(float deg)
{
	if(!(this->rot_state&CROSS_ROT))
	{
		//�I���p�x��ݒ�
		this->cross_end = deg;
		//�����ʂ�ݒ�
		this->cross_add = deg - this->now_cross_rot;

		D3DXMATRIX mat;
		//�J�������W�Ǝ��_���W�̍����擾
		DxVec3 sub = this->eye_pos - this->look_pos;
		//��]�s��̐ݒ�
		D3DXMatrixRotationAxis(&mat,&this->vertical_axis,D3DXToRadian(this->cross_end));
		//���x�N�g������]
		D3DXVec3TransformCoord(&sub,&sub,&mat);

		//��]��Ԃ�Y����]�X�^�[�g��ݒ�
		this->rot_state |= this->CROSS_ROT;
	}
}

//�������W�̐ݒu		�ifloat�^�j
void Dx_Camera::SetInitPos(float ix, float iy, float iz)
{
	this->init_pos = this->look_pos + DxVec3(ix,iy,iz);
	this->Init();
	this->CheckCrossAxis();
}
//�������W�̐ݒu		�iDxVec3�^�j
void Dx_Camera::SetInitPos(DxVec3 init)
{
	this->init_pos = this->look_pos + init;
	this->Init();
	this->CheckCrossAxis();
}
//�J�������W�̐ݒu
void Dx_Camera::SetEyePos(DxVec3 eye_pos)
{
	this->eye_pos = eye_pos;
}
//�J�������W�̎擾
DxVec3 Dx_Camera::GetEyePos()
{
	return this->eye_pos;
}
//���_���W�̐ݒu		�ifloat�^�j
void Dx_Camera::SetLookPos(float lx, float ly, float lz)
{
	this->look_pos = DxVec3(lx,ly,lz);
}
//���_���W�̐ݒu		�iDxVec3�^�j
void Dx_Camera::SetLookPos(DxVec3 look)
{
	this->look_pos = look;
}
//���_���W�̎擾
DxVec3 Dx_Camera::GetLookPos()
{
	return this->look_pos;
}

//��]�p�����̐ݒ�
void Dx_Camera::CheckCrossAxis()
{
	//
	DxVec3 vec1,vec2,cross;
	//���W���王�_���W�܂ł̃x�N�g��
	vec1 = this->eye_pos-this->look_pos;
	//
	vec2 = this->vertical_axis;

	D3DXVec3Cross(&cross,&vec1,&vec2);
	D3DXVec3Normalize(&this->cross_axis,&cross);
}

//���݂�Vertical�p�x�̎擾
float Dx_Camera::GetNowVertical()
{
	return this->now_vertical_rot;
}
//���݂�Cross�p�x�̎擾
float Dx_Camera::GetNowCross()
{
	return this->now_cross_rot;
}
//���݂̊p�x���̎擾
DxVec2 Dx_Camera::GetNowDeg()
{
	return DxVec2(this->now_vertical_rot,this->now_cross_rot);
}

//�r���[�s��̎擾
D3DXMATRIX Dx_Camera::GetViewMat()
{
	return this->view_mat;
}
//�ˉe�s��̎擾
D3DXMATRIX Dx_Camera::GetProjectionMat()
{
	return this->projection_mat;
}

//�ˉe�^�C�v�̐ݒ�
void	Dx_Camera::SetMatrixType(MatrixType type)
{
	this->mat_type = type;
}