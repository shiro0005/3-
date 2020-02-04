//=============================================================================
//
// �e�ۏ��� [bullet.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

#define MAX_BULLET           100

typedef struct {
	D3DXMATRIX				g_mtxWorldBullet;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				g_posBullet;			// �ʒu
	D3DXVECTOR3				g_sclBullet;			// �X�P�[��
	D3DXVECTOR3				g_moveBullet;			// �ړ���
	int						g_nIdxShadowBullet;		// �eID
	int						frame;					//�t���[����
	CIRCLE                  col_cir;                    //�����蔻��T�[�N��
	AABB                    col_aabb;               //�����蔻��{�b�N�X
	bool                    bUse;					//�g�p�t���O
	float                   g_speed;                //speed
}BULLET;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Bullet_Initialize(void);
void Bullet_Finalize(void);
void Bullet_Update(void);
void Bullet_Draw(void);
void Bullet_Create(float, float, float, D3DXVECTOR2);
bool Bullet_IsEnable(int index);
CIRCLE *Bullet_GetCollision(int index);
AABB *Bullet_GetCollisionBox(int index);
BULLET *GetBullet();
