//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
//
//=============================================================================
#include "enemy.h"
#include "myDirect3D.h"
#include "camera.h"
#include "input.h"
//#include "shadow.h"
#include "bullet.h"
#include "billboard.h"

//***************************************************************************** �}�N����`
//*****************************************************************************
#define	MODEL_CAR			"asset/MODEL/car001.x"	// �ǂݍ��ރ��f����
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
static LPDIRECT3DTEXTURE9	g_pTextureModel;	// �e�N�X�`���ւ̃|�C���^
static LPD3DXMESH			g_pMeshModel;		// ���b�V�����ւ̃|�C���^
static LPD3DXBUFFER			g_pBuffMatModel;	// �}�e���A�����ւ̃|�C���^
static DWORD				g_nNumMatModel;		// �}�e���A�����̑���

static int deletecnt;

ENEMY g_Enemy[MAX_ENEMY];
const int GO = 1;
const int BACK = 2;
const int LEFT = 3;
const int RIGHT = 4;

//=============================================================================
// ����������
//=============================================================================
HRESULT Enemy_Initialize()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	
	g_pTextureModel = NULL;
	g_pMeshModel = NULL;
	g_pBuffMatModel = NULL;

	float offset = ((50.0f * MAX_ENEMY / 2) - 25.0f) * 5;

	deletecnt = 0;
	// �ʒu�E�����E�ړ��ʂ̏����ݒ�
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].posModel = D3DXVECTOR3(130.0f*i + offset, 0.0f, -550.0f);
		g_Enemy[i].rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].rotDestModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�����蔻��p�X�t�B�A�̔��a
		g_Enemy[i].col_circle.r = 10.0f;

		g_Enemy[i].moveset = GO;

		//�e�̍쐬
		//g_Enemy[i].idxShadow = Shadow_Create(g_Enemy[i].posModel, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		g_Enemy[i].bUse = true;
	}

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,	// �t�@�C���̖��O
		&g_pTextureModel);	// �ǂݍ��ރ������[
#endif

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Enemy_Finalize(void)
{
	////�e�̉������
	//for (int i = 0; i < MAX_ENEMY; i++)
	//{
	//	Shadow_Release(g_Enemy[i].idxShadow);
	//}

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
void Enemy_Update(void)
{
	


	//�����蔻��p���W�X�V
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].posModel.z < -1000.0f) {
			g_Enemy[i].moveset = BACK;
		}
		else if (g_Enemy[i].posModel.z > -100.0f) {
			g_Enemy[i].moveset = GO;
		}
		
		switch (g_Enemy[i].moveset)
		{
		case GO:
			g_Enemy[i].moveModel.x = 0.0f;
			g_Enemy[i].moveModel.z = -1.0f;
			break;

		case BACK:
			g_Enemy[i].moveModel.x = 0.0f;
			g_Enemy[i].moveModel.z = 1.0f;
			break;
		case LEFT:
			g_Enemy[i].moveModel.x = -1.0f;
			g_Enemy[i].moveModel.z = 0.0f;
			break;
		case RIGHT:
			g_Enemy[i].moveModel.x = 1.0f;
			g_Enemy[i].moveModel.z = 0.0f;
			break;
		default:
			break;
		}

		g_Enemy[i].posModel.x += g_Enemy[i].moveModel.x;
		g_Enemy[i].posModel.z += g_Enemy[i].moveModel.z;
		//�L���t���O���`�F�b�N
		if (g_Enemy[i].bUse == false)
			continue;

		g_Enemy[i].col_circle.cx = g_Enemy[i].posModel.x;
		g_Enemy[i].col_circle.cy = g_Enemy[i].posModel.y;
		g_Enemy[i].col_circle.cz = g_Enemy[i].posModel.z;

		g_Enemy[i].col_aabb.cx = g_Enemy[i].posModel.x;
		g_Enemy[i].col_aabb.cy = g_Enemy[i].posModel.y;
		g_Enemy[i].col_aabb.cz = g_Enemy[i].posModel.z;


		if (g_Enemy[i].posModel.x<0.0f || g_Enemy[i].posModel.x>1100.0f || g_Enemy[i].posModel.z > 0.0f || g_Enemy[i].posModel.z < -1100.0f) {
			

			if (g_Enemy[i].posModel.x < 0.0f) {
				g_Enemy[i].posModel.x = 0.0f;
			}
			if (g_Enemy[i].posModel.x > 1100.0f) {
				g_Enemy[i].posModel.x = 1100.0f;
			}
			if (g_Enemy[i].posModel.z > 0.0f) {
				g_Enemy[i].posModel.z = 0.0f;
			}
			if (g_Enemy[i].posModel.z < -1100.0f) {
				g_Enemy[i].posModel.z = -1100.0f;
			}
			MakeVertexBillboard();
			SetVertexBillboard(g_Enemy[i].posModel.x, 10.0f, g_Enemy[i].posModel.z);

			// �G�̏��ŏ���
			//Shadow_Release(g_Enemy[i].idxShadow);
			g_Enemy[i].bUse = false;

			if (g_Enemy[i].bUse == true)
				continue;
 			deletecnt++;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Enemy_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//�L���t���O���`�F�b�N
		if (g_Enemy[i].bUse == false)
			continue;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Enemy[i].mtxWorldModel);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[i].rotModel.y, g_Enemy[i].rotModel.x, g_Enemy[i].rotModel.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorldModel, &g_Enemy[i].mtxWorldModel, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy[i].posModel.x, g_Enemy[i].posModel.y, g_Enemy[i].posModel.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorldModel, &g_Enemy[i].mtxWorldModel, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[i].mtxWorldModel);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pD3DXMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();


		for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureModel);

			// �`��
			g_pMeshModel->DrawSubset(nCntMat);
		}

		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
}

bool Enemy_IsEnable(int index)
{
	return g_Enemy[index].bUse;
}

CIRCLE* Enemy_GetCollision(int index)
{
	return &g_Enemy[index].col_circle;
}

ENEMY* GetEnemy(void)
{
	return &(g_Enemy[0]);
}

int GetDel() {
	return deletecnt;
}

