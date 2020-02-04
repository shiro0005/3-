//=============================================================================
//
// �e�ۏ��� [bullet.cpp]
//
//=============================================================================
#include "bullet.h"
#include "myDirect3D.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLET		"asset/TEXTURE/bullet000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLET_SIZE_X		(20.0f)							// �r���{�[�h�̕�
#define	BULLET_SIZE_Y		(20.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_BULLET	(0.30f)							// �ړ����x
#define BULLET_NUM_VERTEX    4
#define BULLET_NUM_POLYGON   2
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBullet(float fSizeX, float fSizeY);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBullet = NULL;	// ���_�o�b�t�@�ւ̃|�C���^


BULLET g_Bullet[MAX_BULLET];

//=============================================================================
// ����������
//=============================================================================
HRESULT Bullet_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexBullet(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_BULLET,		// �t�@�C���̖��O
		&g_pTextureBullet);	// �ǂݍ��ރ������[

	for (int i = 0; i < MAX_BULLET; i++) {
		g_Bullet[i].g_posBullet = D3DXVECTOR3(1500, 100, -1300);
		g_Bullet[i].g_sclBullet = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bullet[i].g_moveBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_Bullet[i].col_cir = { 0.0f, 0.0f, 0.0f,10.0f };
		//g_Bullet[i].col.r = 10.0f;

		g_Bullet[i].col_aabb = { 0.0f,0.0f,0.0f,10.0f,10.0f,10.0f };


		g_Bullet[i].g_speed = 10.0f;
		g_Bullet[i].bUse = false;	}
	//g_nIndxShadowBullet=Shadow_Create(g_Bullet)

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Bullet_Finalize(void)
{
	if (g_pTextureBullet != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	if (g_pVtxBuffBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Bullet_Update(void)
{
	CAMERA *pCamera;

	// �J�����̎擾
	pCamera = GetCamera();

	for (int i= 0; i < MAX_BULLET; i++) {
		if (g_Bullet[i].bUse) {
			//���Ԍo�߂�����
			g_Bullet[i].frame++;

			//100�t���[���o�߂ŏ���
			if (g_Bullet[i].frame > 100) {
				//�e�̊J������
				Shadow_Release(g_Bullet[i].g_nIdxShadowBullet);
				g_Bullet[i].bUse = false;
			}

			//���W�X�V����
			g_Bullet[i].g_posBullet += g_Bullet[i].g_moveBullet;

			//�����蔻����W�X�V
			//g_Bullet[i].col = { g_Bullet[i].g_posBullet.x, g_Bullet[i].g_posBullet.y, g_Bullet[i].g_posBullet.z};
			g_Bullet[i].col_cir.cx = g_Bullet[i].g_posBullet.x;
			g_Bullet[i].col_cir.cy = g_Bullet[i].g_posBullet.y;
			g_Bullet[i].col_cir.cz = g_Bullet[i].g_posBullet.z;

			g_Bullet[i].col_aabb.cx = g_Bullet[i].g_posBullet.x;
			g_Bullet[i].col_aabb.cy = g_Bullet[i].g_posBullet.y;
			g_Bullet[i].col_aabb.cz = g_Bullet[i].g_posBullet.z;

					   

			//�e�̈ʒu��e�ɍ��킹��
			D3DXVECTOR3 pos = g_Bullet[i].g_posBullet;
			pos.y = 0.0f;
			Shadow_SetPosition(g_Bullet[i].g_nIdxShadowBullet, pos);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Bullet_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//a�e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < MAX_BULLET; i++) {
		
		//�L���t���O�`�F�b�N
		if (!g_Bullet[i].bUse) {
			continue;
		}
		// ���[���h�}�g���b�N�X�̏����� 
		D3DXMatrixIdentity(&g_Bullet[i].g_mtxWorldBullet);
		// �r���[�}�g���b�N�X���擾 
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		// �|���S���𐳖ʂɌ�����
#if 1
	// �t�s������Ƃ߂� 
		D3DXMatrixInverse(&g_Bullet[i].g_mtxWorldBullet, NULL, &mtxView);

		// ���s�ړ��̕����͖�������
		g_Bullet[i].g_mtxWorldBullet._41 = 0.0f;
		g_Bullet[i].g_mtxWorldBullet._42 = 0.0f;
		g_Bullet[i].g_mtxWorldBullet._43 = 0.0f;
#else
		g_Bullet[i].g_mtxWorldBullet._11 = mtxView._11;
		g_Bullet[i].g_mtxWorldBullet. 12 = mtxView._21;
		g_Bullet[i].g_mtxWorldBullet._13 = mtxView._31;
		g_Bullet[i].g_mtxWorldBullet._21 = mtxView._12;//
		g_Bullet[i].g_mtxWorldBullet._22 = mtxView._22;//���̎O���R�����g�A�E�g����Ɛ��������𖳎��ł���
		g_Bullet[i].g_mtxWorldBullet._23 = mtxView._32;//
		g_Bullet[i].g_mtxWorldBullet._31 = mtxView._13;
		g_Bullet[i].g_mtxWorldBullet. 32 = mtxView._23;
		g_Bullet[i].g_mtxWorldBullet._33 = mtxView._33;
#endif

		// �X�P�[���𔽉f 
		D3DXMatrixScaling(&mtxScale, g_Bullet[i].g_sclBullet.x, g_Bullet[i].g_sclBullet.y, g_Bullet[i].g_sclBullet.z);
		D3DXMatrixMultiply(&g_Bullet[i].g_mtxWorldBullet,
			&g_Bullet[i].g_mtxWorldBullet, &mtxScale);

		// �ړ��𔽉f 
		D3DXMatrixTranslation(&mtxTranslate, g_Bullet[i].g_posBullet.x,
			g_Bullet[i].g_posBullet.y, g_Bullet[i].g_posBullet.z);
		D3DXMatrixMultiply(&g_Bullet[i].g_mtxWorldBullet, &g_Bullet[i].g_mtxWorldBullet, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Bullet[i].g_mtxWorldBullet);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ� 
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBullet);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, BULLET_NUM_POLYGON);
	}
	// ���C���e�B���O��L���ɂ��� 
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	//a�e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BULLET_NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxBuffBullet,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f); -

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
		g_pVtxBuffBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBullet(float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffBullet->Unlock();
	}
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void Bullet_Create(float x, float y,float z, D3DXVECTOR2 dir)
{
	for (int i = 0; i < MAX_BULLET; i++) {

		// �����e�����łɗL����������H
		if (g_Bullet[i].bUse) {
			continue;
		}

		// �e�̍��W�Ɉ����̒l��������
		g_Bullet[i].g_posBullet.x = x;
		g_Bullet[i].g_posBullet.y = y;
		g_Bullet[i].g_posBullet.z = z;

		// �e�̈ړ��������w�肷��
		D3DXVec2Normalize(&dir, &dir);
		g_Bullet[i].g_moveBullet.x= dir.x*5.0f;
		g_Bullet[i].g_moveBullet.y = 0.0f;
		g_Bullet[i].g_moveBullet.z = dir.y*5.0f;

		//// �e�̈ړ���������e�̉�]�p�x�����߂�
		//g_Bullets[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		//g_Bullets[i].collision.cx = x;
		//g_Bullets[i].collision.cy = y;
		//g_Bullets[i].collision.r = BULLET_WIDTH * 0.8f;

		// �L���t���[����������
		g_Bullet[i].frame = 0;

		//�e���쐬
		g_Bullet[i].g_nIdxShadowBullet=Shadow_Create(g_Bullet[i].g_posBullet, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		// �e��L���ɂ���
		g_Bullet[i].bUse = true;

		break;
	}
}

bool Bullet_IsEnable(int index) {
	return g_Bullet[index].bUse;
}

CIRCLE *Bullet_GetCollision(int index){
	return &g_Bullet[index].col_cir;
}

AABB *Bullet_GetCollisionBox(int index) {
	return &g_Bullet[index].col_aabb;
}

BULLET *GetBullet() {
	return &g_Bullet[0];
}