//=============================================================================
//
// �r���{�[�h���� [billboard.cpp]
//
//=============================================================================
#include "billboard.h"
#include "myDirect3D.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BILLBOARD		"asset/TEXTURE/bullet000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BILLBOARD_SIZE_X		(20.0f)							// �r���{�[�h�̕�
#define	BILLBOARD_SIZE_Y		(20.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_BILLBOARD	(0.30f)							// �ړ����x
#define	VALUE_JUMP				(10.0f)							// �W�����v��
#define	VALUE_GRAVITY			(0.45f)							// �d��
#define	RATE_REGIST				(0.075f)						// ��R�W��
#define	RATE_REFRECT			(-0.90f)						// ���ˌW��
#define BILLBOARD_NUM_VERTEX    4
#define BILLBOARD_NUM_POLYGON   2
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBillboard(float fSizeX, float fSizeY);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBillboard = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBillboard = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

static D3DXMATRIX				g_mtxWorldBillboard;		// ���[���h�}�g���b�N�X
static D3DXVECTOR3				g_posBillboard;				// �ʒu
static D3DXVECTOR3				g_sclBillboard;				// �X�P�[��
static D3DXVECTOR3				g_moveBillboard;			// �ړ���
static int						g_nIdxShadowBillboard;		// �eID
static bool						g_bEnableGravity = false;	// �d�͂�L���ɂ��邩�ǂ���

//=============================================================================
// ����������
//=============================================================================
HRESULT Billboard_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexBillboard(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_BILLBOARD,		// �t�@�C���̖��O
		&g_pTextureBillboard);	// �ǂݍ��ރ������[

	g_posBillboard = D3DXVECTOR3(1500, 100, -1300);
	g_sclBillboard = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_moveBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_bEnableGravity = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Billboard_Finalize(void)
{
	if (g_pTextureBillboard != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	if (g_pVtxBuffBillboard != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Billboard_Update(void)
{
	CAMERA *pCamera;

	// �J�����̎擾
	pCamera = GetCamera();

	if (Keyboard_IsTrigger(DIK_J))
	{// �W�����v
		if (g_bEnableGravity == true)
		{
			g_moveBillboard.y = VALUE_JUMP;
		}
	}

	if (g_bEnableGravity == true)
	{
		g_posBillboard.y += g_moveBillboard.y;
		if (g_posBillboard.y < 9.0f)
		{
			g_posBillboard.y = 9.0f;
			g_moveBillboard.y *= RATE_REFRECT;
		}
		g_moveBillboard.y -= VALUE_GRAVITY;
	}


	// �d��ON/OFF
	if (Keyboard_IsTrigger(DIK_G))
	{
		g_bEnableGravity ^= 1;
		if (g_bEnableGravity == false)
		{
			g_moveBillboard.y = 0.0f;
		}
	}

	DebugProc_Print("*** �{�[������ ***\n");
	DebugProc_Print("[�{�[���̈ʒu  �F(%f : %f : %f)]\n", g_posBillboard.x, g_posBillboard.y, g_posBillboard.z);
	DebugProc_Print("[�{�[���̈ړ��ʁF(%f : %f : %f)]\n", g_moveBillboard.x, g_moveBillboard.y, g_moveBillboard.z);
	DebugProc_Print("�O�ړ�   : ��\n");
	DebugProc_Print("��ړ�   : ��\n");
	DebugProc_Print("���ړ�   : ��\n");
	DebugProc_Print("�E�ړ�   : ��\n");
	DebugProc_Print("�W�����v : J\n");
	DebugProc_Print("\n");

	DebugProc_Print("*** �d�� ***\n");
	DebugProc_Print("ON / OFF : [G] ( �d�� = ");

	if (g_bEnableGravity)
	{
		DebugProc_Print("ON )\n");
	}
	else
	{
		DebugProc_Print("OFF )\n");
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Billboard_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//a�e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// ���[���h�}�g���b�N�X�̏����� 
	D3DXMatrixIdentity(&g_mtxWorldBillboard);
	// �r���[�}�g���b�N�X���擾 
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	// �|���S���𐳖ʂɌ�����
#if 1
	// �t�s������Ƃ߂� 
	D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);
	// ���s�ړ��̕����͖�������
	g_mtxWorldBillboard._41 = 0.0f;
	g_mtxWorldBillboard._42 = 0.0f;
	g_mtxWorldBillboard._43 = 0.0f;
#else
	g_mtxWorldBillboard._11 = mtxView._11;
	g_mtxWorldBillboard. 12 = mtxView._21;
	g_mtxWorldBillboard._13 = mtxView._31;
	g_mtxWorldBillboard._21 = mtxView._12;
	g_mtxWorldBillboard._22 = mtxView._22;
	g_mtxWorldBillboard._23 = mtxView._32;
	g_mtxWorldBillboard._31 = mtxView._13;
	g_mtxWorldBillboard. 32 = mtxView._23;
	g_mtxWorldBillboard._33 = mtxView._33;
#endif

	// �X�P�[���𔽉f 
	D3DXMatrixScaling(&mtxScale, g_sclBillboard.x, g_sclBillboard.y, g_sclBillboard.z);
	D3DXMatrixMultiply(&g_mtxWorldBillboard,
		&g_mtxWorldBillboard, &mtxScale);

	// �ړ��𔽉f 
	D3DXMatrixTranslation(&mtxTranslate, g_posBillboard.x,
		g_posBillboard.y, g_posBillboard.z);
	D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTranslate);
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);
	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));
	// ���_�t�H�[�}�b�g�̐ݒ� 
	pDevice->SetFVF(FVF_VERTEX_3D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBillboard);
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, BILLBOARD_NUM_POLYGON);
	// ���C���e�B���O��L���ɂ��� 
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//a�e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BILLBOARD_NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxBuffBillboard,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f); -

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffBillboard->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBillboard(float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffBillboard->Unlock();
	}
}


