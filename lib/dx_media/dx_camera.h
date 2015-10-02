
//**************************************************************************
//�J�����N���X
//��{�^�@�F�@�Ȃ�
//�h���^�@�F�@�Ȃ�
//**************************************************************************

#pragma once

#include "dx.h"

//�J�����N���X
class Dx_Camera
{
	public:
		//�ˉe�^�C�v
		enum MatrixType
		{
			//���ˉe�s��
			DX_ORTHO_MAT,
			//���ߎˉe�s��
			DX_PERS_MAT
		};
		/*���ʕϐ�*/
		enum {VERTICAL_ROT=1,CROSS_ROT=2};

	private:
		//�ˉe�^�C�v
		MatrixType mat_type;

		//�r���[�s��
		D3DXMATRIX view_mat;
		//�ˉe�s��
		D3DXMATRIX projection_mat;

		//�������W
		D3DXVECTOR3 init_pos;
		//���_���W
		D3DXVECTOR3 eye_pos;
		//�����_���W
		D3DXVECTOR3 look_pos;

		//�s��֘A�̐ݒ�
		void SetupMatrix();

		/*��]�֘A*/
		//��]�p�c��
		D3DXVECTOR3 vertical_axis;
		//��]�p����
		D3DXVECTOR3 cross_axis;
		//��]���
		short rot_state;
		/*�c����]�p�ϐ�*/
		//�c������ϐ�
		float now_vertical_rot;
		//�c����]�I���p�x
		float vertical_end;
		//1�t���[��������̑�����
		float vertical_add;
		/*������]�p�ϐ�*/
		//��������ϐ�
		float now_cross_rot;
		//������]�I���p�x
		float cross_end;
		//1�t���[��������̑�����
		float cross_add;

		float dot_ver_cross;

		//��]�֐�
		void Rotation();
		//��]�֘A�̏�����
		void InitRotation();

	public:
		//�R���X�g���N�^
		Dx_Camera(float lx=0.0f,float ly=0.0f,float lz=0.0f);

		//������
		void Init();
		//����֐�
		void Step();

		/*��]�֘A*/
		//���݂̉�]��Ԃ��擾
		short GetRotState();

		//�c����]�J�n		�i���Z�p�x,���t���[���ŉ�]���邩�j
		void StartVerticalRot(float add_deg,short frame_val=1);
		//������]�J�n		�i���Z�p�x,���t���[���ŉ�]���邩�j
		void StartCrossRot(float add_deg,short frame_val=1);
		//�c���̊p�x�w��
		void SetVerticalRot(float deg);
		//�����̊p�x�w��
		void SetCrossRot(float deg);

		//�������W�̐ݒu		�ifloat�^�j
		void SetInitPos(float ix,float iy,float iz);
		//�������W�̐ݒu		�iD3DXVECTOR3�^�j
		void SetInitPos(D3DXVECTOR3 init);

		//���_���W�̐ݒu
		void SetEyePos(D3DXVECTOR3 eye_pos);
		//���_���W�̎擾
		D3DXVECTOR3 GetEyePos();

		//�����_���W�̐ݒu		�ifloat�^�j
		void SetLookPos(float lx,float ly,float lz);
		//�����_���W�̐ݒu		�iD3DXVECTOR3�^�j
		void SetLookPos(D3DXVECTOR3 look);
		//�����_���W�̎擾
		D3DXVECTOR3 GetLookPos();

		//��]�p�����̐ݒ�
		void CheckCrossAxis();

		//���݂�Vertical�p�x�̎擾
		float GetNowVertical();
		//���݂�Cross�p�x�̎擾
		float GetNowCross();
		//���݂̊p�x���̎擾
		D3DXVECTOR2 GetNowDeg();

		//�r���[�s��̎擾
		D3DXMATRIX GetViewMat();
		//�ˉe�s��̎擾
		D3DXMATRIX GetProjectionMat();

		//�ˉe�^�C�v�̐ݒ�
		void	SetMatrixType(MatrixType type);
};
