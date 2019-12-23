//=============================================================================
//
// プレイヤー処理 [player.cpp]
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
// マクロ定義
//*****************************************************************************
#define	MODEL_CAR			"asset/MODEL/car000.x"	// 読み込むモデル名
#define	VALUE_MOVE_MODEL	(0.50f)					// 移動速度
#define	RATE_MOVE_MODEL		(0.20f)					// 移動慣性係数
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)		// 回転速度
#define	RATE_ROTATE_MODEL	(0.20f)					// 回転慣性係数
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pTextureModel;	// テクスチャへのポインタ
LPD3DXMESH			g_pMeshModel;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pBuffMatModel;	// マテリアル情報へのポインタ
DWORD				g_nNumMatModel;		// マテリアル情報の総数

struct PLAYER
{
	D3DXMATRIX			mtxWorldModel;	// ワールドマトリックス
	D3DXVECTOR3			posModel;			// 現在の位置
	D3DXVECTOR3			rotModel;			// 現在の向き
	D3DXVECTOR3			rotDestModel;		// 目的の向き
	D3DXVECTOR3			moveModel;		// 移動量

	int					idxShadow;
};

PLAYER player;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Player_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	g_pTextureModel = NULL;
	g_pMeshModel = NULL;
	g_pBuffMatModel = NULL;

	//位置・向き・移動量の初期設定
	player.posModel = pos;
	player.rotModel = rot;
	player.rotDestModel = rot;
	player.moveModel = D3DXVECTOR3(0, 0, 0);

	//Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,//アセットの指定
		D3DXMESH_SYSTEMMEM,//
		pDevice,//
		NULL,//隣接ポリゴンの設定
		&g_pBuffMatModel,//マテリアル情報へのポインタ
		NULL,//
		&g_nNumMatModel,//
		&g_pMeshModel))) {
		return S_OK;
	}
	//影の作成
	player.idxShadow = Shadow_Create(player.posModel,D3DXVECTOR3(1.0, 1.0, 1.0));
}

//=============================================================================
// 終了処理
//=============================================================================
void Player_Finalize(void)
{
	//影の開放処理
	Shadow_Release(player.idxShadow);

	if (g_pTextureModel != NULL)
	{// テクスチャの開放
		g_pTextureModel->Release();
		g_pTextureModel = NULL;
	}

	if (g_pMeshModel != NULL)
	{// メッシュの開放
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	if (g_pBuffMatModel != NULL)
	{// マテリアルの開放
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Player_Update(void)
{
	CAMERA *pCamera;


	float fDiffRotY;

	// カメラの取得
	pCamera = GetCamera();

	if (Keyboard_IsPress(DIK_LEFT))
	{
		if (Keyboard_IsPress(DIK_UP))
		{// 左奥移動
			player.moveModel.x += sinf(-D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(-D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		else if (Keyboard_IsPress(DIK_DOWN))
		{// 左手前移動
			player.moveModel.x += sinf(-D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(-D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.25f;
		}
		else
		{// 左移動
			player.moveModel.x += sinf(-D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(-D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.50f;
		}
	}
	else if (Keyboard_IsPress(DIK_RIGHT))
	{
		if (Keyboard_IsPress(DIK_UP))
		{// 右奥移動
			player.moveModel.x += sinf(D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		else if (Keyboard_IsPress(DIK_DOWN))
		{// 右手前移動
			player.moveModel.x += sinf(D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.25f;
		}
		else
		{// 右移動
			player.moveModel.x += sinf(D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			player.moveModel.z -= cosf(D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			player.rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.50f;
		}
	}
	else if (Keyboard_IsPress(DIK_UP))
	{// 前移動
		player.moveModel.x += sinf(D3DX_PI * 1.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		player.moveModel.z -= cosf(D3DX_PI * 1.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;

		player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 1.0f;
	}
	else if (Keyboard_IsPress(DIK_DOWN))
	{// 後移動
		player.moveModel.x += sinf(D3DX_PI * 0.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		player.moveModel.z -= cosf(D3DX_PI * 0.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;

		player.rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.0f;
	}

	if (Keyboard_IsPress(DIK_LSHIFT))
	{// 左回転
		player.rotDestModel.y -= VALUE_ROTATE_MODEL;
		if (player.rotDestModel.y < -D3DX_PI)
		{
			player.rotDestModel.y += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_RSHIFT))
	{// 右回転
		player.rotDestModel.y += VALUE_ROTATE_MODEL;
		if (player.rotDestModel.y > D3DX_PI)
		{
			player.rotDestModel.y -= D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsTrigger(DIK_P))
	{//弾丸射出処理
		Bullet_Create(player.posModel.x, player.posModel.z, D3DXVECTOR2(0.0f,1.0f));
	}

	// 目的の角度までの差分
	fDiffRotY = player.rotDestModel.y - player.rotModel.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// 目的の角度まで慣性をかける
	player.rotModel.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (player.rotModel.y > D3DX_PI)
	{
		player.rotModel.y -= D3DX_PI * 2.0f;
	}
	if (player.rotModel.y < -D3DX_PI)
	{
		player.rotModel.y += D3DX_PI * 2.0f;
	}

	/// 位置移動
	player.posModel.x += player.moveModel.x;
	player.posModel.z += player.moveModel.z;

	player.posModel.y = GetY(player.posModel.x, player.posModel.z);

	// 移動量に慣性をかける
	player.moveModel.x += (0.0f - player.moveModel.x) * RATE_MOVE_MODEL;
	player.moveModel.z += (0.0f - player.moveModel.z) * RATE_MOVE_MODEL;

#if 0
	// 範囲チェック
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
	{// リセット
		player.posModel = D3DXVECTOR3(1500, 100, -1300),
		player.moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		player.rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		player.rotDestModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//影の座標更新
	D3DXVECTOR3 pos = player.posModel;
	pos.y = 0.0f;//Yの値だけは０で固定する
	Shadow_SetPosition(player.idxShadow, pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void Player_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pD3DXMat;

	//ワールドマトッリクスの初期化
	D3DXMatrixIdentity(&player.mtxWorldModel);

	//回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, player.rotModel.y, player.rotModel.x, player.rotModel.z);
	D3DXMatrixMultiply(&player.mtxWorldModel, &player.mtxWorldModel, &mtxRot);

	//移動を反映
	D3DXMatrixTranslation(&mtxTranslate, player.posModel.x, player.posModel.y, player.posModel.z);
	D3DXMatrixMultiply(&player.mtxWorldModel, &player.mtxWorldModel, &mtxTranslate);

	//ワールドマトッリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &player.mtxWorldModel);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++) {
		//マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureModel);

		//描画
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

