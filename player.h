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

	int					idxShadow;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Player_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
PLAYER GetPlayer(void);
