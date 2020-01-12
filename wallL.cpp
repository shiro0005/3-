#include "wallL.h"
#include "box.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "box.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	VALUE_MOVE		(5.0f)							// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.01f)				// ��]��

#define	FIELD_WIDTH		(100.0f)						// �n�ʂ̕�(X����)
#define	FIELD_DEPTH		(100.0f)						// �n�ʂ̉��s(Z����)
#define FIELD_HIGHT     (100.0f)                        //�n�ʂ̍���(Y����)

#define BLOCK_X 11                                       //�u���b�N���̐�  �ς���Ƃ��͔g�����̐������ς���
#define BLOCK_Y 1                                       //�c�̐�

//int wallmap[WIDTH][DEPTH]=
//{
//	{1,1,1,1,1,1,1,1,1,1,1},
//    {1,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,1},
//	{1,0,0,0,0,0,0,0,0,0,1},
//	{1,1,1,1,1,1,1,1,1,1,1},
//
//};
//int x;
//int z;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexWallL(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField1 = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DINDEXBUFFER9 g_pIdxBuffField1 = NULL;

#define NUM_BOX 1

typedef struct {
	D3DXMATRIX				g_mtxWorldField;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				g_posField;				// �n�ʂ̈ʒu
	D3DXVECTOR3				g_rot1Field;				// �n�ʂ̌���(��])
	D3DXVECTOR3				g_rot2Field;				// �n�ʂ̌���(��])
	D3DXVECTOR3				g_sclField;				// �n�ʂ̑傫��(�X�P�[��)
}BOX_wallL;

BOX_wallL g_Field[NUM_BOX];

static int g_NumVertex1 = (BLOCK_X + 1)*(BLOCK_Y + 1);                //���_��
static int g_NumIndex1 = ((BLOCK_X + 1)*BLOCK_Y + (BLOCK_Y - 1)) * 2; //�C���f�b�N�X�� ((BLOCK_X + 1)*2)*BLOCK_Y+(BLOCK_Y-1)*2
static int g_NumPolygon1 = (BLOCK_X * 2)*BLOCK_Y + (BLOCK_Y - 1) * 4; //�|���S����
static int zcnt1[6] = {};//�g�����p


//=============================================================================
// ����������
//=============================================================================
HRESULT WallL_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexWallL(pDevice);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < NUM_BOX; i++) {
		g_Field[i].g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Field[i].g_rot1Field = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Field[i].g_rot2Field = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Field[i].g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void WallL_Finalize(void)
{
	if (g_pIdxBuffField1 != NULL) {//�C���f�b�N�X�o�b�t�@�̊J��
		g_pIdxBuffField1->Release();
		g_pIdxBuffField1 = NULL;
	}

	if (g_pVtxBuffField1 != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffField1->Release();
		g_pVtxBuffField1 = NULL;
	}
}



//=============================================================================
// �X�V����
//=============================================================================


//=============================================================================
// �`�揈��
//=============================================================================
void WallL_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();


	for (int i = 0; i < NUM_BOX; i++) {
		//���_�̖@�����m�[�}���C�Y����
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

		//���[���h�}�g���b�N�X�̏�����
		D3DXMATRIX mtxWorld;
		D3DXMatrixIdentity(&mtxWorld);


		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pVtxBuffField1, 0, sizeof(VERTEX_3D));

		pDevice->SetIndices(g_pIdxBuffField1);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

		//�|���S���̕`��(�C���f�b�N�X�o�b�t�@�p�j
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVertex1, 0, g_NumPolygon1);//��ԍŌオ�|���S����,�E����2�Ԗڂ����_��

		////�|���S���̕`��
		//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexWallL(LPDIRECT3DDEVICE9 pDevice)
{

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVertex1,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffField1, NULL)))
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;


		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffField1->Lock(0, 0, (void**)&pVtx, 0);


		for (int z = 0; z < BLOCK_Y + 1; z++) {
			for (int x = 0; x < BLOCK_X + 1; x++) {
				//���_���W�̐ݒ�
				pVtx[z * (BLOCK_X + 1) + x].pos = D3DXVECTOR3(0.0f, 100 + z * -100.0f, -1100+x*100.0f);//D3DXVECTOR3(x*100.0f,0-z*-100.0f, 0.0f);

				//�@���x�N�g���̐ݒ�
				pVtx[z * (BLOCK_X + 1) + x].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

				//���_�J���[�̐ݒ�
				pVtx[z * (BLOCK_X + 1) + x].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//UV�l�ݒ�
				pVtx[z * (BLOCK_X + 1) + x].tex = D3DXVECTOR2(z, x);

			}
		}


		/*pVtx[8].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[9].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[10].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[11].tex = D3DXVECTOR2(0.0f, 0.0f);

		pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[16].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[17].tex = D3DXVECTOR2(0.0f, 0.0f);

		pVtx[18].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[19].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[20].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[21].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[22].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[23].tex = D3DXVECTOR2(0.0f, 0.0f);

		pVtx[24].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[25].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[26].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[27].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[28].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[29].tex = D3DXVECTOR2(0.0f, 0.0f);

		pVtx[30].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[31].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[32].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[33].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[34].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[35].tex = D3DXVECTOR2(0.0f, 0.0f);*/

		//���_�f�[�^���A�����b�N����
		g_pVtxBuffField1->Unlock();
	}
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD)*g_NumIndex1,//�C���f�b�N�X���͒��_���W�̐���葽���Ȃ�
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffField1,
		NULL))) {
		return E_FAIL;
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		g_pIdxBuffField1->Lock(0, 0, (void**)&pIdx, 0);

		/*for (int i = 0; i < VERTEXCNT; i+=4) {
			for (int z = 0; z < 101; z++) {
				for (int x = 0; x < 101; x++) {
					pIdx[i] = 101 + 2 * i;
					pIdx[i + 1] = 0 + 2 * i;
					pIdx[i + 2] = 102 + 2 * i;
					pIdx[i + 3] = 1 + 2 * i;
				}
			}
		}*/

		int cntX;
		int cntY;
		for (cntY = 0; cntY < BLOCK_Y; cntY++) {
			for (cntX = 0; cntX < BLOCK_X + 1; cntX++) {
				pIdx[((BLOCK_X + 1) * 2 * cntY + cntY * 2) + cntX * 2] = (BLOCK_X + 1) + cntX + cntY * (BLOCK_X + 1);
				pIdx[((BLOCK_X + 1) * 2 * cntY + cntY * 2) + cntX * 2 + 1] = cntX + cntY * (BLOCK_X + 1);
			}
			pIdx[((BLOCK_X + 1) * 2 * cntY + cntY * 2) + cntX * 2] = (BLOCK_X + 1) * cntY + cntX - 1;
			pIdx[((BLOCK_X + 1) * 2 * cntY + cntY * 2) + cntX * 2 + 1] = (BLOCK_X + 1) * cntY + (BLOCK_X + 1) + cntX;
		}





		/*for (int i = 1; i < 102;i++) {
			pIdx[i + (i - 1)] = i - 1;
		}
		for (int j = 0; j < 102; j++) {
			for (int i = 0; i < 102; i++) {
				pIdx[102*j+i * 2] = 101 + 2 * i;
			}
		}*/
		g_pIdxBuffField1->Unlock();
	}
	return S_OK;

}



//void wallmake() {
//	/*for (z = 0; z < DEPTH; z++) {
//		for (x = 0; x < WIDTH; x++) {
//			if (x == 0 || z == 0 || x == WIDTH - 1 || z == DEPTH - 1) {
//				wallmap[z][x] = 1;
//			}
//			else {
//				wallmap[z][x] = 0;
//			}
//		}
//	}*/
//
//	/*//�_�|���@���g�����ǁi1�j�̐����i1�s�߂̂݁j
//	z = 2; //1�s��
//	for (x = 2; x < WIDTH - 1; x += 2) //x�̗v�f�ԍ�2����v�f�ԍ�mWIDTH-1�܂ŁA1�}�X��΂��Ŗ_�|���B
//	{
//		wallmap[z][x] = 1; //���S����c�c
//		if (ran >= 1 && ran <= 3) //r��1����3�̂Ƃ�
//		{
//			if (wallmap[z - 1][x] == 0) //��ɖ_�i�ǁj���Ȃ����
//			{
//				wallmap[z - 1][x] = 1; //��ɖ_��|���B
//			}
//			else if (wallmap[z - 1][x] == 1) //��ɖ_�i�ǁj�������
//			{
//				x = x - 2; //�_��|�����ɁA������������蒼���B
//			}
//		}
//		if (ran >= 4 && ran <= 6) //r��4����6�̂Ƃ�
//		{
//			if (wallmap[z + 1][x] == 0) //���ɖ_�i�ǁj���Ȃ����
//			{
//				wallmap[z + 1][x] = 1; //���ɖ_��|���B
//			}
//			else if (wallmap[z + 1][x] == 1) //���ɖ_�i�ǁj�������
//			{
//				x = x - 2; //�_��|�����ɁA������������蒼���B
//			}
//		}
//		if (ran >= 7 && ran <= 9) //r��7����9�̂Ƃ�
//		{
//			if (wallmap[z][x - 1] == 0) //���ɖ_�i�ǁj���Ȃ����
//			{
//				wallmap[z][x - 1] = 1; //���ɖ_��|���B
//			}
//			else if (wallmap[z][x - 1] == 1) //���ɖ_�i�ǁj�������
//			{
//				x = x - 2; //�_��|�����ɁA������������蒼���B
//			}
//		}
//		if (ran >= 10 && ran <= 12) //r��10����12�̂Ƃ�
//		{
//			if (wallmap[z][x + 1] == 0) //�E�ɖ_�i�ǁj���Ȃ����
//			{
//				wallmap[z][x + 1] = 1; //�E�ɖ_��|���B
//			}
//			else if (wallmap[z][x + 1] == 1) //�E�ɖ_�i�ǁj�������
//			{
//				x = x - 2; //�_��|�����ɁA������������蒼���B
//			}
//		}
//	}
//
//	//�_�|���@���g�����ǁi1�j�̐����i2�s�߈ȍ~�j
//	for (z = 4; z < DEPTH - 1; z += 2) //y�̗v�f�ԍ�4����v�f�ԍ�max_y-1�܂ŁA1�}�X��΂��Ŗ_�|���B
//	{
//		for (x = 2; x < WIDTH - 1; x += 2) //x�̗v�f�ԍ�2����v�f�ԍ�max_x-1�܂ŁA1�}�X��΂��Ŗ_�|���B
//		{
//			wallmap[z][x] = 1; //���S����c�c
//			if (ran >= 1 && ran <= 4) //r��1����4�̂Ƃ�
//			{
//				if (wallmap[z + 1][x] == 0) //���ɖ_�i�ǁj���Ȃ����
//				{
//					wallmap[z + 1][x] = 1; //���ɖ_��|���B
//				}
//				else if (wallmap[z + 1][x] == 1) //���ɖ_�i�ǁj�������
//				{
//					x = x - 2; //�_��|�����ɁA������������蒼���B
//				}
//			}
//			if (ran >= 5 && ran <= 8) //r��5����8�̂Ƃ�
//			{
//				if (wallmap[z][x - 1] == 0) //���ɖ_�i�ǁj���Ȃ����
//				{
//					wallmap[z][x - 1] = 1; //���ɖ_��|���B
//				}
//				else if (wallmap[z][x - 1] == 1) //���ɖ_�i�ǁj�������
//				{
//					x = x - 2; //�_��|�����ɁA������������蒼���B
//				}
//			}
//			if (ran >= 9 && ran <= 12) //r��9����12�̂Ƃ�
//			{
//				if (wallmap[z][x + 1] == 0) //�E�ɖ_�i�ǁj���Ȃ����
//				{
//					wallmap[z][x + 1] = 1; //�E�ɖ_��|���B
//				}
//				else if (wallmap[z][x + 1] == 1) //�E�ɖ_�i�ǁj�������
//				{
//					x = x - 2; //�_��|�����ɁA������������蒼���B
//				}
//			}
//		}
//	}*/
//
//	wallmap[0][1] = 0; //�X�^�[�g�n�_�̕ǂ�P������B
//	wallmap[DEPTH - 1][WIDTH - 2] = 0; //�S�[���n�_�̕ǂ�P������B
//
//	//wallmap���g���ă{�b�N�X�쐬
//	for (z = 0; z < DEPTH; z++) {
//		for (x = 0; x < WIDTH; x++) {
//	
//			if (wallmap[z][x] == 1) {
//				box_Initialize(x*100.0f, z*-100.0f);
//	        }
//		}
//	}
//
//}