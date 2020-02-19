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
static STICK stick;
static int cntt = 0;
static float rote = 0;
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

	//�X�e�B�b�N��X�AY�������擾
	stick.vec.x = GamePad_Stick().lX;
	stick.vec.y = GamePad_Stick().lY;

	

	////���������ƂɊp�x���擾�i���W�A���j
	//rote = -atan2f(stick.vec.x, stick.vec.y) + D3DX_PI;
	////�ϐ�angle�Ɋp�x�i�x�j����
	//stick.angle = 180 * rote / D3DX_PI;
	////�p�x�ɂ����F[�Ή����鐔]��true�ɂȂ�A��{�I�ɃA�P�R���Ɠ���
	////�ړ���������Ă���
	//if (stick.vec == D3DXVECTOR2(0, 0)) {//�X�e�B�b�N����͂��ĂȂ��Ƃ�
	//	rote = 0;
	//	stick.angle = 1000.0f;
	//	for (int j = 0; j < 10; j++) {
	//		stick.F[j] = false;
	//	}
	//	stick.F[5] = true;
	//	stick.rote[0] = false;
	//	stick.rote[1] = false;
	//}
	//if ((337.5f <= stick.angle && stick.angle <= 360.0f) || (stick.angle >= 0 && stick.angle < 22.5f)) {
	//	stick.F[8] = true;//���͔���
	//	//��]����
	//	if (stick.F[7]) {
	//		stick.rote[0] = true;//�E���
	//		stick.rote[1] = false;
	//		stick.F[7] = false;//������
	//	}
	//	else if (stick.F[9]) {
	//		stick.rote[1] = true;//�����
	//		stick.rote[0] = false;
	//		stick.F[9] = false;
	//	}
	//	else {//�X�e�B�b�N����]���͂����Ă��Ȃ�
	//		for (int i = 1; i < 7; i++) {//F[7]�`F[9]�܂Œ��ׂ�K�v���Ȃ�����
	//			if (stick.F[i]) {//��]�Ɋ֌W�Ȃ�F��true�Ȃ�
	//				stick.rote[0] = false;//�ړ������Ȃ�
	//				stick.rote[1] = false;
	//				for (int j = 0; j < 10; j++) {//�S�Ă�F��������
	//					stick.F[j] = false;
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (22.5f <= stick.angle && stick.angle < 67.5f) {
	//	stick.F[9] = true;
	//	if (stick.F[8]) {
	//		stick.rote[0] = true;
	//		stick.rote[1] = false;
	//		stick.F[8] = false;
	//	}
	//	else if (stick.F[6]) {
	//		stick.rote[1] = true;
	//		stick.rote[0] = false;
	//		stick.F[6] = false;
	//	}
	//	else {
	//		for (int i = 1; i < 8; i++) {
	//			if (i == 6) {//����
	//				i++;
	//			}
	//			if (stick.F[i]) {
	//				stick.rote[0] = false;
	//				stick.rote[1] = false;
	//				for (int j = 0; j < 10; j++) {
	//					stick.F[j] = false;
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (67.5f <= stick.angle && stick.angle < 112.5f) {
	//	stick.F[6] = true;
	//	if (stick.F[9]) {
	//		stick.rote[0] = true;
	//		stick.rote[1] = false;
	//		stick.F[9] = false;
	//	}
	//	else if (stick.F[3]) {
	//		stick.rote[1] = true;
	//		stick.rote[0] = false;
	//		stick.F[3] = false;
	//	}
	//	else {
	//		for (int i = 1; i < 9; i++) {
	//			if (i == 6) {//��������Ȃ��Ɩ��񂱂��ɊY������̂ŏ��������������Ȃ�
	//				i++;
	//			}
	//			if (stick.F[i]) {
	//				stick.rote[0] = false;
	//				stick.rote[1] = false;
	//				for (int j = 0; j < 10; j++) {
	//					stick.F[j] = false;
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (112.5f <= stick.angle && stick.angle < 157.5f) {
	//	stick.F[3] = true;
	//	if (stick.F[6]) {
	//		stick.rote[0] = true;
	//		stick.rote[1] = false;
	//		stick.F[6] = false;
	//	}
	//	else if (stick.F[2]) {
	//		stick.rote[1] = true;
	//		stick.rote[0] = false;
	//		stick.F[2] = false;
	//	}
	//	else {
	//		for (int i = 1; i < 10; i++) {
	//			if (i == 3) {
	//				i++;
	//			}
	//			if (stick.F[i]) {
	//				stick.rote[0] = false;
	//				stick.rote[1] = false;
	//				for (int j = 0; j < 10; j++) {
	//					stick.F[j] = false;
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (157.5f <= stick.angle && stick.angle < 202.5f) {
	//	stick.F[2] = true;
	//	if (stick.F[3]) {
	//		stick.rote[0] = true;
	//		stick.rote[1] = false;
	//		stick.F[3] = false;
	//	}
	//	else if (stick.F[1]) {
	//		stick.rote[1] = true;
	//		stick.rote[0] = false;
	//		stick.F[1] = false;
	//	}
	//	else {
	//		for (int i = 4; i < 10; i++) {
	//			if (stick.F[i]) {
	//				stick.rote[0] = false;
	//				stick.rote[1] = false;
	//				for (int j = 0; j < 10; j++) {
	//					stick.F[j] = false;
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (202.5f <= stick.angle && stick.angle < 247.5f) {
	//	stick.F[1] = true;
	//	if (stick.F[2]) {
	//		stick.rote[0] = true;
	//		stick.rote[1] = false;
	//		stick.F[2] = false;
	//	}
	//	else if (stick.F[4]) {
	//		stick.rote[1] = true;
	//		stick.rote[0] = false;
	//		stick.F[4] = false;
	//	}
	//	else {
	//		for (int i = 3; i < 10; i++) {
	//			if (stick.F[i]) {
	//				stick.rote[0] = false;
	//				stick.rote[1] = false;
	//				for (int j = 0; j < 10; j++) {
	//					stick.F[j] = false;
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (247.5f <= stick.angle && stick.angle < 292.5f) {
	//	stick.F[4] = true;
	//	if (stick.F[1]) {
	//		stick.rote[0] = true;
	//		stick.rote[1] = false;
	//		stick.F[1] = false;
	//	}
	//	else if (stick.F[7]) {
	//		stick.rote[1] = true;
	//		stick.rote[0] = false;
	//		stick.F[7] = false;
	//	}
	//	else {
	//		for (int i = 1; i < 10; i++) {
	//			if (i == 4) {
	//				i++;
	//			}
	//			if (stick.F[i]) {
	//				stick.rote[0] = false;
	//				stick.rote[1] = false;
	//				for (int j = 0; j < 10; j++) {
	//					stick.F[j] = false;
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (292.5f <= stick.angle && stick.angle < 337.5f) {
	//	stick.F[7] = true;
	//	if (stick.F[4]) {
	//		stick.rote[0] = true;
	//		stick.rote[1] = false;
	//		stick.F[4] = false;
	//	}
	//	else if (stick.F[8]) {
	//		stick.rote[1] = true;
	//		stick.rote[0] = false;
	//		stick.F[8] = false;
	//	}
	//	else {
	//		for (int i = 1; i < 10; i++) {
	//			if (i == 7) {
	//				i++;
	//			}
	//			if (stick.F[i]) {
	//				stick.rote[0] = false;
	//				stick.rote[1] = false;
	//				for (int j = 0; j < 10; j++) {
	//					stick.F[j] = false;
	//				}
	//			}
	//		}
	//	}
	//}
	//for (int i = 0; i < 10; i++) {
	//	if (stick.F[i]) {//�t���O��true�ɂȂ��Ă���Ƃ�����i�[
	//		stick.data[cntt] = i;
	//	}
	//}
	//if (stick.data[0] == stick.data[1]) {//�O�̃t���[���Ɠ����Ƃ��낪true�Ȃ�
	//	stick.cntdata++;//�t���O�������������񐔁i�t���[�����j
	//	if (stick.cntdata >= 13) {//13�t���[������������
	//		stick.rote[0] = false;//��]
	//		stick.rote[1] = false;
	//		for (int i = 0; i < 10; i++) {
	//			stick.F[i] = false;//�S�Ă�F[]��������
	//		}
	//		rote = 0;//�v���C���[�̊p�x��0�ɐݒ�
	//	}
	//}
	//else {
	//	stick.cntdata = 0;//������
	//}
	//cntt++;//�t���[���J�E���g
	//if (cntt == 2) {
	//	cntt = 0;//������
	//}

	if (Keyboard_IsPress(DIK_LEFT)||stick.vec.x<0)
	{
		if (Keyboard_IsPress(DIK_UP)||stick.vec.y<0)
		{// �����ړ�
			player.moveModel.x += sinf(-D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(-D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		else if (Keyboard_IsPress(DIK_DOWN)||stick.vec.y>0)
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
		player.moveModelold = player.moveModel;
	}
	else if (Keyboard_IsPress(DIK_RIGHT)||stick.vec.x>0)
	{
		if (Keyboard_IsPress(DIK_UP) || stick.vec.y < 0)
		{// �E���ړ�
			player.moveModel.x += sinf(D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		else if (Keyboard_IsPress(DIK_DOWN) || stick.vec.y > 0)
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
		player.moveModelold = player.moveModel;
	}
	else if (Keyboard_IsPress(DIK_UP) || stick.vec.y < 0)
	{// �O�ړ�
		player.moveModel.x += sinf(D3DX_PI * 1.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		player.moveModel.z -= cosf(D3DX_PI * 1.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		player.moveModelold = player.moveModel;
		player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 1.0f;
	}
	else if (Keyboard_IsPress(DIK_DOWN) || stick.vec.y > 0)
	{// ��ړ�
		player.moveModel.x += sinf(D3DX_PI * 0.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		player.moveModel.z -= cosf(D3DX_PI * 0.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		player.moveModelold = player.moveModel;
		player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.0f;
	}

	/*if (stick.vec.x != 0.0f||stick.vec.y != 0.0f) {
		player.moveModelold = player.moveModel;
	}*/

	if (GamePad_IsPress(0, BUTTON_R)) {
		player.moveModel *= 1.15f;
	}
	

	//if (Keyboard_IsPress(DIK_LSHIFT))
	//{// ����]
	//	player.rotDestModel.y -= VALUE_ROTATE_MODEL;
	//	if (player.rotDestModel.y < -D3DX_PI)
	//	{
	//		player.rotDestModel.y += D3DX_PI * 2.0f;
	//	}
	//}
	//if (Keyboard_IsPress(DIK_RSHIFT))
	//{// �E��]
	//	player.rotDestModel.y += VALUE_ROTATE_MODEL;
	//	if (player.rotDestModel.y > D3DX_PI)
	//	{
	//		player.rotDestModel.y -= D3DX_PI * 2.0f;
	//	}
	//}
	if (Keyboard_IsTrigger(DIK_SPACE)|| GamePad_IsTrigger(0, BUTTON_C))
	{//�e�ێˏo����
		Bullet_Create(player.posModel.x,player.posModel.y, player.posModel.z, D3DXVECTOR2(player.moveModelold.x,player.moveModelold.z));
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

	if (player.posModel.x<5.0f || player.posModel.x>1095.0f || player.posModel.z > -5.0f || player.posModel.z < -1095.0f) {


		if (player.posModel.x <= 5.0f) {
			player.posModel.x = 5.0f;
		}
		if (player.posModel.x >= 1095.0f) {
			player.posModel.x = 1095.0f;
		}
		if (player.posModel.z >= -5.0f) {
			player.posModel.z = -5.0f;
		}
		if (player.posModel.z <= -1095.0f) {
			player.posModel.z = -1095.0f;
		}

	}
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
	//pos.y = 0.0f;//Y�̒l�����͂O�ŌŒ肷��
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

PLAYER GetPlayer() {
	return player;
}

