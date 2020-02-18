
#include "light.h"
#include "myDirect3D.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_LIGHT		(4)		// ���C�g�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DLIGHT9 g_aLight[NUM_LIGHT];		// ���C�g���
static int cnt;
static bool flag;
//=============================================================================
// ���C�g�̏���������
//=============================================================================
void Light_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice(); 
	D3DXVECTOR3 vecDir;

	cnt = 0;
	flag = false;

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ����
	g_aLight[0].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.20f, -0.80f, 0.40f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(0, &g_aLight[0]);

	// ���C�g��L����
	pDevice->LightEnable(0,TRUE);//�������S�̂̌�





	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	g_aLight[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// ����
	g_aLight[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

    // ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(1, &g_aLight[1]);

	// ���C�g��L����
	pDevice->LightEnable(1, TRUE);


	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[2], sizeof(D3DLIGHT9));

	// ���C�g2�̃^�C�v�̐ݒ�
	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g2�̊g�U���̐ݒ�
	g_aLight[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ���C�g2�̋��ʔ��ˌ��̐ݒ�
	g_aLight[2].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

    // ���C�g2�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.40f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[2].Direction, &vecDir);

	// ���C�g2�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(2, &g_aLight[2]);

	// ���C�g2��L����
	pDevice->LightEnable(2, FALSE);


	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[3], sizeof(D3DLIGHT9));

	// ���C�g3�̃^�C�v�̐ݒ�
	g_aLight[3].Type = D3DLIGHT_SPOT;

	//���C�g3�̈ʒu�̐ݒ�
	g_aLight[3].Position = D3DXVECTOR3(500, 101, -600);

	// ���C�g3�̊g�U���̐ݒ�
	//g_aLight[3].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	//���C�g�̐F
	g_aLight[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);



	g_aLight[3].Direction= D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	////���C�g3�͈̔͂̐ݒ�
	g_aLight[3].Range = 100.0f;

	//0�`1.0f�܂Őݒ�@
	g_aLight[3].Falloff = 1.0f;

	//������
	g_aLight[3].Attenuation0 = 0.9f;
	g_aLight[3].Attenuation1 = 0.0f;
	g_aLight[3].Attenuation2 = 0.0f;

	//�����ƊO���̊p�x
	g_aLight[3].Theta = 1.1f;//����
	g_aLight[3].Phi = 2.21f;//�O��



	// ���C�g3�̋��ʔ��ˌ��̐ݒ�
	//g_aLight[3].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g3�̕����̐ݒ�
	/*vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[3].Direction, &vecDir);*/

	// ���C�g3�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(3, &g_aLight[3]);



	// ���C�g3��L����
	pDevice->LightEnable(3, FALSE);



	// ���C�e�B���O���[�h�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);//���C�e�B���O����TRUE���Ȃ�FALSE
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void Light_Finalize(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void Light_Update(void)
{
	PLAYER player = GetPlayer();
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	g_aLight[3].Position = player.posModel;
	g_aLight[3].Position.y = player.posModel.y+20.0f;
	// ���C�g3�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(3, &g_aLight[3]);

	cnt++;
	if (cnt >= 120) {
		LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
		if (!flag) {
			pDevice->LightEnable(0, FALSE);//�������S�̂̌�
			pDevice->LightEnable(1, FALSE);//�������S�̂̌�

			g_aLight[3].Position = player.posModel;
			g_aLight[3].Position.y = player.posModel.y + 20.0f;
			// ���C�g3�������_�����O�p�C�v���C���ɐݒ�
			pDevice->SetLight(3, &g_aLight[3]);
			pDevice->LightEnable(3, TRUE);//�������S�̂̌�

			flag = true;
		}
		else {
			pDevice->LightEnable(0, TRUE);//�������S�̂̌�
			pDevice->LightEnable(1, TRUE);//�������S�̂̌�

			pDevice->LightEnable(3, FALSE);//�������S�̂̌�
			flag = false;
		}
		cnt = 0;
	}
}

