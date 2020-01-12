//=============================================================================
//
// �r���{�[�h���� [billboard.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BILLBOARD		"asset/TEXTURE/exp.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BILLBOARD_SIZE_X		(20.0f)							// �r���{�[�h�̕�
#define	BILLBOARD_SIZE_Y		(20.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_BILLBOARD	(0.30f)							// �ړ����x
#define	VALUE_JUMP				(10.0f)							// �W�����v��
#define	VALUE_GRAVITY			(0.45f)							// �d��
#define	RATE_REGIST				(0.075f)						// ��R�W��
#define	RATE_REFRECT			(-0.90f)						// ���ˌW��

#define BILLBOARD_NUM_VERTEX	4
#define BILLBOARD_NUM_POLYGON	2

#define MAX_BILLBOARD	1

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct
{
	D3DXMATRIX				g_mtxWorldBillboard;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				g_posBillboard;				// �ʒu
	D3DXVECTOR3				g_sclBillboard;				// �X�P�[��
	D3DXVECTOR3				g_moveBillboard;			// �ړ���
	int						g_nIdxShadowBillboard;		// �eID
	bool					g_bEnableGravity = false;	// �d�͂�L���ɂ��邩�ǂ���

	//CIRCLE					col_circle;					// �����蔻��p�X�t�B�A
	AABB					col_aabb;					// �����蔻��p�{�b�N�X

	int						g_BillBoardFrame = 0;
	float					g_tx = 0;
	float					g_ty = 0;
	bool						bUse;
}BILLBOARD;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Billboard_Initialize(void);
void Billboard_Finalize(void);
void Billboard_Update(void);
void Billboard_Draw(void);
HRESULT MakeVertexBillboard();
void SetVertexBillboard(float posX, float posY, float posZ);
bool BillBoard_IsEnable(int index);
AABB* BillBoard_GetCollisionBox(int index);
BILLBOARD* GetBillBoard(void);

