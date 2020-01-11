
#include "camera.h"
#include "myDirect3D.h"
#include "input.h"
#include "debugproc.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAM_POS_V_X		(1500.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_V_Y		(500.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_V_Z		(-2200.0f)				// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X		(1500.0f)					// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y		(-500.0f)					// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z		(0.0f)					// �J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(2000.0f)										// �r���[���ʂ�FarZ�l

#define VALUE_MOVE_CAMERA (2.0f)
#define VALUE_ROTATE_CAMERA (D3DX_PI * 0.01f)
#define VALUE_ADJUST_DISTANCE (2.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
CAMERA g_camera;		// �J�������

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
float fVecX, fVecZ;


//=============================================================================
// �J�����̏���������
//=============================================================================
void Camera_Initialize(void)
{
	g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	g_camera.fDistance = 20.0f;
	
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	
	fVecX = g_camera.posV.x - g_camera.posR.x;
	fVecZ = g_camera.posV.z - g_camera.posR.z;
	g_camera.fDistance = sqrtf(fVecX * fVecX + fVecZ * fVecZ);
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void Camera_Finalize(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void Camera_Update(void)
{
	if (Keyboard_IsPress(DIK_A)) {
		if (Keyboard_IsPress(DIK_W)) {//�����ړ�
			g_camera.posV.x += sinf(-D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(-D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else if (Keyboard_IsPress(DIK_S)) {//����O
			g_camera.posV.x += sinf(-D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(-D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else {//��
			g_camera.posV.x += sinf(-D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(-D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (Keyboard_IsPress(DIK_D)) {
		if (Keyboard_IsPress(DIK_W)) {//�E���ړ�
			g_camera.posV.x += sinf(D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else if (Keyboard_IsPress(DIK_S)) {//�E��O
			g_camera.posV.x += sinf(D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else {//�E
			g_camera.posV.x += sinf(D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (Keyboard_IsPress(DIK_W)) {//�O�ړ�
		g_camera.posV.x += sinf(D3DX_PI * 1.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(D3DX_PI * 1.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (Keyboard_IsPress(DIK_S)) {//���
		g_camera.posV.x += sinf(D3DX_PI * 0.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(D3DX_PI * 0.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_Z)) {//���_����[��]
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y > D3DX_PI) {
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
		g_camera.posV.z = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_C)) {//���_����[�E]
		g_camera.rot.y = VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y < -D3DX_PI) {
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_Y)) { //���_�ړ�[��]
		g_camera.posV.y += VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_N)) { // ���_�ړ�[��]
		g_camera.posV.y -= VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_Q)) {//�����_����[��]
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;

		if (g_camera.rot.y < -D3DX_PI) {
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_E)) { //�����_����[�E]
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y > D3DX_PI) {
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_T)) { //�����_�ړ�[��]
		g_camera.posR.y += VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_B)) {//�����_�ړ�[��]
		g_camera.posR.y -= VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_U)) {//���_�ƒ����_�̋����k�߂�
		g_camera.fDistance -= VALUE_ADJUST_DISTANCE;
		if (g_camera.fDistance < 100.0f) {
			g_camera.fDistance = 100.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y)*g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y)*g_camera.fDistance;

	}
if (Keyboard_IsPress(DIK_M)) {//���_�ƒ����_�̋����L�΂�
	g_camera.fDistance += VALUE_ADJUST_DISTANCE;
	if (g_camera.fDistance > 800.0f) {
		g_camera.fDistance = 800.0f;
	}
	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y)*g_camera.fDistance;
	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y)*g_camera.fDistance;
}
if (Keyboard_IsTrigger(DIK_SPACE)) { //���Z�b�g 
	g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	
	fVecX = g_camera.posV.x - g_camera.posR.x;
	fVecZ = g_camera.posV.z - g_camera.posR.z;
	g_camera.fDistance = sqrtf(fVecX * fVecX + fVecZ * fVecZ);
}
        DebugProc_Print("[�J�����̎��_   : (%f : %f : %f)]\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
		DebugProc_Print("[�J�����̒����_ : (%f : %f : %f)]\n", g_camera.posR.x, g_camera.posR.y, g_camera.posR.z);
		DebugProc_Print("[�J�����̌���   : (%f)]\n", g_camera.rot.y);
		DebugProc_Print("[�J�����̋���   : (%f)]\n", g_camera.fDistance);
		DebugProc_Print("\n");


}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void Camera_SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice(); 

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								VIEW_ANGLE,				// �r���[���ʂ̎���p
								VIEW_ASPECT,			// �r���[���ʂ̃A�X�y�N�g��
								VIEW_NEAR_Z,			// �r���[���ʂ�NearZ�l
								VIEW_FAR_Z);			// �r���[���ʂ�FarZ�l

	D3DXMATRIX mtxProj;
	D3DXMatrixIdentity(&mtxProj);

	float aspect = D3DXToRadian(45);//�p�x�����W�A���ɕϊ��H

	float w = 1 / tanf((aspect*1.78)*0.5f);
	float h = 1 / tanf(aspect*0.5f); 
	float Q = VIEW_FAR_Z / (VIEW_FAR_Z - VIEW_NEAR_Z);
	
	mtxProj(0, 0) = w;
	mtxProj(1, 1) = h;
	mtxProj(2, 2) = Q;
	mtxProj(2, 3) = 1.0f;
	mtxProj(3, 2) = Q * VIEW_FAR_Z*-1;



	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView, 
						&g_camera.posV,		// �J�����̎��_
						&g_camera.posR,		// �J�����̒����_
						&g_camera.vecU);	// �J�����̏�����x�N�g��
	
	//��̏����̒��g
	/*D3DXMATRIX mtxView;
	D3DXMatrixIdentity(&mtxView);

	D3DXVECTOR3 axisZ;//3�����̂���
	D3DXVECTOR3 axisX;//3������x��
	D3DXVECTOR3 axisY;//3������y��

	axisZ = g_camera.posR - g_camera.posV;//at-pos�̃x�N�g���v�Z

	D3DXVec3Normalize(&axisZ, &axisZ);//�P�ʃx�N�g����

	//up�x�N�g���Ƃ����ŊO�ς����X�����Z�o
	D3DXVec3Cross(&axisX, &g_camera.vecU, &axisZ);//�O�όv�Z�i������������ϐ�,�v�Z�p�̕ϐ�,�v�Z�p�̕ϐ��j

	//Z����X���̊O�ς��Ƃ�AY�����Z�o
	D3DXVec3Cross(&axisY, &axisZ, &axisX);

	//pos�x�N�g���Ɗe���Ƃ̓��ς����߁��|�P
	float cx = D3DXVec3Dot(&g_camera.posV, &axisX)*-1;
	float cy = D3DXVec3Dot(&g_camera.posV, &axisY)*-1;
	float cz = D3DXVec3Dot(&g_camera.posV, &axisZ)*-1;

	mtxView(0, 0) = axisX.x;//�s��iy,x�j�ɑ��
	mtxView(1, 0) = axisX.y;
	mtxView(2, 0) = axisX.z;
	mtxView(0, 1) = axisY.x;
	mtxView(1, 1) = axisY.y;
	mtxView(2, 1) = axisY.z;
	mtxView(0, 2) = axisZ.x;
	mtxView(1, 2) = axisZ.y;
	mtxView(2, 2) = axisZ.z;
	mtxView(3, 0) = cx;
	mtxView(3, 1) = cy;
	mtxView(3, 2) = cz;
	*/
	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

CAMERA *GetCamera() {
	return &g_camera;
}

