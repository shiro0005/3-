//=============================================================================
//
// �v���C���[���� [player.h]
//
//=============================================================================
#pragma once

#include "main.h"

struct PLAYER
{
	D3DXMATRIX			mtxWorldModel;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3			posModel;			// ���݂̈ʒu
	D3DXVECTOR3			rotModel;			// ���݂̌���
	D3DXVECTOR3			rotDestModel;		// �ړI�̌���
	D3DXVECTOR3			moveModel;		// �ړ���
	D3DXVECTOR3			moveModelold;		// �ړ���
	int					idxShadow;
};

typedef struct
{
	D3DXVECTOR2 vec;//�X�e�B�b�N�̂�������
	float angle;//�X�e�B�b�N�̊p�x�i�x�j
	bool F[10];//�X�e�B�b�N���̓t���O
	bool rote[2];//�E��]�Ȃ�rote[0]��true�A����]�Ȃ�rote[1]��true
	int data[2];//F�̃t���O�̃f�[�^���i�[����
	int cntdata;//�O�̃t���[���Ɠ������ʂ̎��ɉ��Z
}STICK;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Player_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
PLAYER GetPlayer(void);
