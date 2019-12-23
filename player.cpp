//=============================================================================
//
// �v���C���[���� [player.cpp]
//
//=============================================================================
#include "player.h"
#include "myDirect3D.h"
#include "camera.h"
#include "input.h"
#include "field.h"
#include "shadow.h"
#include "bullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_CAR			"asset/MODEL/car000.x"	// �ǂݍ��ރ��f����
#define	VALUE_MOVE_MODEL	(0.50f)					// �ړ����x
#define	RATE_MOVE_MODEL		(0.20f)					// �ړ������W��
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)		// ��]���x
#define	RATE_ROTATE_MODEL	(0.20f)					// ��]�����W��
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pTextureModel;	// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			g_pMeshModel;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pBuffMatModel;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatModel;		// �}�e���A�����̑���

struct PLAYER
{
	D3DXMATRIX			mtxWorldModel;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3			posModel;			// ���݂̈ʒu
	D3DXVECTOR3			rotModel;			// ���݂̌���
	D3DXVECTOR3			rotDestModel;		// �ړI�̌���
	D3DXVECTOR3			moveModel;		// �ړ���

	int					idxShadow;
};

PLAYER player;

//=============================================================================
// ����������
//=============================================================================
HRESULT Player_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	g_pTextureModel = NULL;
	g_pMeshModel = NULL;
	g_pBuffMatModel = NULL;

	//�ʒu�E�����E�ړ��ʂ̏����ݒ�
	player.posModel = pos;
	player.rotModel = rot;
	player.rotDestModel = rot;
	player.moveModel = D3DXVECTOR3(0, 0, 0);

	//X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,//�A�Z�b�g�̎w��
		D3DXMESH_SYSTEMMEM,//
		pDevice,//
		NULL,//�אڃ|���S���̐ݒ�
		&g_pBuffMatModel,//�}�e���A�����ւ̃|�C���^
		NULL,//
		&g_nNumMatModel,//
		&g_pMeshModel))) {
		return S_OK;
	}
	//�e�̍쐬
	player.idxShadow = Shadow_Create(player.posModel,D3DXVECTOR3(1.0, 1.0, 1.0));
}

//=============================================================================
// �I������
//=============================================================================
void Player_Finalize(void)
{
	//�e�̊J������
	Shadow_Release(player.idxShadow);

	if (g_pTextureModel != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureModel->Release();
		g_pTextureModel = NULL;
	}

	if (g_pMeshModel != NULL)
	{// ���b�V���̊J��
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	if (g_pBuffMatModel != NULL)
	{// �}�e���A���̊J��
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Player_Update(void)
{
	CAMERA *pCamera;


	float fDiffRotY;

	// �J�����̎擾
	pCamera = GetCamera();

	if (Keyboard_IsPress(DIK_LEFT))
	{
		if (Keyboard_IsPress(DIK_UP))
		{// �����ړ�
			player.moveModel.x += sinf(-D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(-D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		else if (Keyboard_IsPress(DIK_DOWN))
		{// ����O�ړ�
			player.moveModel.x += sinf(-D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(-D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.25f;
		}
		else
		{// ���ړ�
			player.moveModel.x += sinf(-D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(-D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.50f;
		}
	}
	else if (Keyboard_IsPress(DIK_RIGHT))
	{
		if (Keyboard_IsPress(DIK_UP))
		{// �E���ړ�
			player.moveModel.x += sinf(D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		else if (Keyboard_IsPress(DIK_DOWN))
		{// �E��O�ړ�
			player.moveModel.x += sinf(D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.25f;
		}
		else
		{// �E�ړ�
			player.moveModel.x += sinf(D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.50f;
		}
	}
	else if (Keyboard_IsPress(DIK_UP))
	{// �O�ړ�
		player.moveModel.x += sinf(D3DX_PI * 1.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		player.moveModel.z -= cosf(D3DX_PI * 1.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;

		player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 1.0f;
	}
	else if (Keyboard_IsPress(DIK_DOWN))
	{// ��ړ�
		player.moveModel.x += sinf(D3DX_PI * 0.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		player.moveModel.z -= cosf(D3DX_PI * 0.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;

		player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.0f;
	}

	if (Keyboard_IsPress(DIK_LSHIFT))
	{// ����]
		player.rotDestModel.y -= VALUE_ROTATE_MODEL;
		if (player.rotDestModel.y < -D3DX_PI)
		{
			player.rotDestModel.y += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_RSHIFT))
	{// �E��]
		player.rotDestModel.y += VALUE_ROTATE_MODEL;
		if (player.rotDestModel.y > D3DX_PI)
		{
			player.rotDestModel.y -= D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsTrigger(DIK_P))
	{//�e�ێˏo����
		Bullet_Create(player.posModel.x, player.posModel.z, D3DXVECTOR2(0.0f,1.0f));
	}

	// �ړI�̊p�x�܂ł̍���
	fDiffRotY = player.rotDestModel.y - player.rotModel.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	player.rotModel.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (player.rotModel.y > D3DX_PI)
	{
		player.rotModel.y -= D3DX_PI * 2.0f;
	}
	if (player.rotModel.y < -D3DX_PI)
	{
		player.rotModel.y += D3DX_PI * 2.0f;
	}

	/// �ʒu�ړ�
	player.posModel.x += player.moveModel.x;
	player.posModel.z += player.moveModel.z;

	player.posModel.y = GetY(player.posModel.x, player.posModel.z);

	// �ړ��ʂɊ�����������
	player.moveModel.x += (0.0f - player.moveModel.x) * RATE_MOVE_MODEL;
	player.moveModel.z += (0.0f - player.moveModel.z) * RATE_MOVE_MODEL;

#if 0
	// �͈̓`�F�b�N
	if (g_posModel.x < -310.0f)
	{
		g_posModel.x = -310.0f;
	}
	if (g_posModel.x > 310.0f)
	{
		g_posModel.x = 310.0f;
	}
	if (g_posModel.z < -310.0f)
	{
		g_posModel.z = -310.0f;
	}
	if (g_posModel.z > 310.0f)
	{
		g_posModel.z = 310.0f;
	}
#endif

	if (Keyboard_IsPress(DIK_RETURN))
	{// ���Z�b�g
		player.posModel = D3DXVECTOR3(1500, 100, -1300),
		player.moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		player.rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		player.rotDestModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//�e�̍��W�X�V
	D3DXVECTOR3 pos = player.posModel;
	pos.y = 0.0f;//Y�̒l�����͂O�ŌŒ肷��
	Shadow_SetPosition(player.idxShadow, pos);
}

//=============================================================================
// �`�揈��
//=============================================================================
void Player_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pD3DXMat;

	//���[���h�}�g�b���N�X�̏�����
	D3DXMatrixIdentity(&player.mtxWorldModel);

	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rotModel.y, player.rotModel.x, player.rotModel.z);
	D3DXMatrixMultiply(&player.mtxWorldModel, &player.mtxWorldModel, &mtxRot);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, player.posModel.x, player.posModel.y, player.posModel.z);
	D3DXMatrixMultiply(&player.mtxWorldModel, &player.mtxWorldModel, &mtxTranslate);

	//���[���h�}�g�b���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &player.mtxWorldModel);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++) {
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureModel);

		//�`��
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//�}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

