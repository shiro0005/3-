
#include "camera.h"
#include "myDirect3D.h"
#include "input.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CAM_POS_V_X		(1500.0f)					// カメラの視点初期位置(X座標)
#define	CAM_POS_V_Y		(500.0f)				// カメラの視点初期位置(Y座標)
#define	CAM_POS_V_Z		(-2200.0f)				// カメラの視点初期位置(Z座標)
#define	CAM_POS_R_X		(1500.0f)					// カメラの注視点初期位置(X座標)
#define	CAM_POS_R_Y		(-500.0f)					// カメラの注視点初期位置(Y座標)
#define	CAM_POS_R_Z		(0.0f)					// カメラの注視点初期位置(Z座標)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(2000.0f)										// ビュー平面のFarZ値

#define VALUE_MOVE_CAMERA (2.0f)
#define VALUE_ROTATE_CAMERA (D3DX_PI * 0.01f)
#define VALUE_ADJUST_DISTANCE (2.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
CAMERA g_camera;		// カメラ情報

//*****************************************************************************
// グローバル変数
//*****************************************************************************
float fVecX, fVecZ;


//=============================================================================
// カメラの初期化処理
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
// カメラの終了処理
//=============================================================================
void Camera_Finalize(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void Camera_Update(void)
{
	if (Keyboard_IsPress(DIK_A)) {
		if (Keyboard_IsPress(DIK_W)) {//左奥移動
			g_camera.posV.x += sinf(-D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(-D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else if (Keyboard_IsPress(DIK_S)) {//左手前
			g_camera.posV.x += sinf(-D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(-D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else {//左
			g_camera.posV.x += sinf(-D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(-D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (Keyboard_IsPress(DIK_D)) {
		if (Keyboard_IsPress(DIK_W)) {//右奥移動
			g_camera.posV.x += sinf(D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else if (Keyboard_IsPress(DIK_S)) {//右手前
			g_camera.posV.x += sinf(D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else {//右
			g_camera.posV.x += sinf(D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (Keyboard_IsPress(DIK_W)) {//前移動
		g_camera.posV.x += sinf(D3DX_PI * 1.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(D3DX_PI * 1.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (Keyboard_IsPress(DIK_S)) {//後ろ
		g_camera.posV.x += sinf(D3DX_PI * 0.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(D3DX_PI * 0.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_Z)) {//視点旋回[左]
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y > D3DX_PI) {
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
		g_camera.posV.z = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_C)) {//視点旋回[右]
		g_camera.rot.y = VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y < -D3DX_PI) {
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_Y)) { //視点移動[上]
		g_camera.posV.y += VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_N)) { // 視点移動[下]
		g_camera.posV.y -= VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_Q)) {//注視点旋回[左]
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;

		if (g_camera.rot.y < -D3DX_PI) {
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_E)) { //注視点旋回[右]
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y > D3DX_PI) {
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_T)) { //注視点移動[上]
		g_camera.posR.y += VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_B)) {//注視点移動[下]
		g_camera.posR.y -= VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_U)) {//視点と注視点の距離縮める
		g_camera.fDistance -= VALUE_ADJUST_DISTANCE;
		if (g_camera.fDistance < 100.0f) {
			g_camera.fDistance = 100.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y)*g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y)*g_camera.fDistance;

	}
if (Keyboard_IsPress(DIK_M)) {//視点と注視点の距離伸ばす
	g_camera.fDistance += VALUE_ADJUST_DISTANCE;
	if (g_camera.fDistance > 800.0f) {
		g_camera.fDistance = 800.0f;
	}
	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y)*g_camera.fDistance;
	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y)*g_camera.fDistance;
}
if (Keyboard_IsTrigger(DIK_SPACE)) { //リセット 
	g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	
	fVecX = g_camera.posV.x - g_camera.posR.x;
	fVecZ = g_camera.posV.z - g_camera.posR.z;
	g_camera.fDistance = sqrtf(fVecX * fVecX + fVecZ * fVecZ);
}
        DebugProc_Print("[カメラの視点   : (%f : %f : %f)]\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
		DebugProc_Print("[カメラの注視点 : (%f : %f : %f)]\n", g_camera.posR.x, g_camera.posR.y, g_camera.posR.z);
		DebugProc_Print("[カメラの向き   : (%f)]\n", g_camera.rot.y);
		DebugProc_Print("[カメラの距離   : (%f)]\n", g_camera.fDistance);
		DebugProc_Print("\n");


}

//=============================================================================
// カメラの設定処理
//=============================================================================
void Camera_SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice(); 

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								VIEW_ANGLE,				// ビュー平面の視野角
								VIEW_ASPECT,			// ビュー平面のアスペクト比
								VIEW_NEAR_Z,			// ビュー平面のNearZ値
								VIEW_FAR_Z);			// ビュー平面のFarZ値

	D3DXMATRIX mtxProj;
	D3DXMatrixIdentity(&mtxProj);

	float aspect = D3DXToRadian(45);//角度をラジアンに変換？

	float w = 1 / tanf((aspect*1.78)*0.5f);
	float h = 1 / tanf(aspect*0.5f); 
	float Q = VIEW_FAR_Z / (VIEW_FAR_Z - VIEW_NEAR_Z);
	
	mtxProj(0, 0) = w;
	mtxProj(1, 1) = h;
	mtxProj(2, 2) = Q;
	mtxProj(2, 3) = 1.0f;
	mtxProj(3, 2) = Q * VIEW_FAR_Z*-1;



	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView, 
						&g_camera.posV,		// カメラの視点
						&g_camera.posR,		// カメラの注視点
						&g_camera.vecU);	// カメラの上方向ベクトル
	
	//上の処理の中身
	/*D3DXMATRIX mtxView;
	D3DXMatrixIdentity(&mtxView);

	D3DXVECTOR3 axisZ;//3次元のｚ軸
	D3DXVECTOR3 axisX;//3次元のx軸
	D3DXVECTOR3 axisY;//3次元のy軸

	axisZ = g_camera.posR - g_camera.posV;//at-posのベクトル計算

	D3DXVec3Normalize(&axisZ, &axisZ);//単位ベクトル化

	//upベクトルとｚ軸で外積を取りX軸を算出
	D3DXVec3Cross(&axisX, &g_camera.vecU, &axisZ);//外積計算（答えを代入する変数,計算用の変数,計算用の変数）

	//Z軸とX軸の外積をとり、Y軸を算出
	D3DXVec3Cross(&axisY, &axisZ, &axisX);

	//posベクトルと各軸との内積を求め＊－１
	float cx = D3DXVec3Dot(&g_camera.posV, &axisX)*-1;
	float cy = D3DXVec3Dot(&g_camera.posV, &axisY)*-1;
	float cz = D3DXVec3Dot(&g_camera.posV, &axisZ)*-1;

	mtxView(0, 0) = axisX.x;//行列（y,x）に代入
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
	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

CAMERA *GetCamera() {
	return &g_camera;
}

