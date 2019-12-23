//=============================================================================
//
// �G�l�~�[���� [enemy.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY			(5)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct ENEMY
{
	D3DXMATRIX			mtxWorldModel;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3			posModel;			// ���݂̈ʒu
	D3DXVECTOR3			rotModel;			// ���݂̌���
	D3DXVECTOR3			rotDestModel;		// �ړI�̌���
	D3DXVECTOR3			moveModel;		// �ړ���
	int					idxShadow;

	CIRCLE				col_circle;		// �����蔻��p�T�[�N��
	AABB                col_aabb;        //�����蔻��{�b�N�X
	bool				bUse;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Enemy_Initialize(void);
void Enemy_Finalize(void);
void Enemy_Update(void);
void Enemy_Draw(void);
bool Enemy_IsEnable(int index);

CIRCLE* Enemy_GetCollision(int index);
ENEMY* GetEnemy(void);
