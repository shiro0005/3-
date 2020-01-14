//=============================================================================
//
// エネミー処理 [enemy.cpp]
//
//=============================================================================
#include "enemy.h"
#include "myDirect3D.h"
#include "camera.h"
#include "input.h"
//#include "shadow.h"
#include "bullet.h"
#include "billboard.h"

//***************************************************************************** マクロ定義
//*****************************************************************************
#define	MODEL_CAR			"asset/MODEL/car001.x"	// 読み込むモデル名
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
static LPDIRECT3DTEXTURE9	g_pTextureModel;	// テクスチャへのポインタ
static LPD3DXMESH			g_pMeshModel;		// メッシュ情報へのポインタ
static LPD3DXBUFFER			g_pBuffMatModel;	// マテリアル情報へのポインタ
static DWORD				g_nNumMatModel;		// マテリアル情報の総数

static int deletecnt;

ENEMY g_Enemy[MAX_ENEMY];
const int GO = 1;
const int BACK = 2;
const int LEFT = 3;
const int RIGHT = 4;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Enemy_Initialize()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	
	g_pTextureModel = NULL;
	g_pMeshModel = NULL;
	g_pBuffMatModel = NULL;

	float offset = ((50.0f * MAX_ENEMY / 2) - 25.0f) * 5;

	deletecnt = 0;
	// 位置・向き・移動量の初期設定
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].posModel = D3DXVECTOR3(130.0f*i + offset, 0.0f, -550.0f);
		g_Enemy[i].rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].rotDestModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//当たり判定用スフィアの半径
		g_Enemy[i].col_circle.r = 10.0f;

		g_Enemy[i].moveset = GO;

		//影の作成
		//g_Enemy[i].idxShadow = Shadow_Create(g_Enemy[i].posModel, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		g_Enemy[i].bUse = true;
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_FILENAME,	// ファイルの名前
		&g_pTextureModel);	// 読み込むメモリー
#endif

	// Xファイルの読み込み
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
// 終了処理
//=============================================================================
void Enemy_Finalize(void)
{
	////影の解放処理
	//for (int i = 0; i < MAX_ENEMY; i++)
	//{
	//	Shadow_Release(g_Enemy[i].idxShadow);
	//}

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
void Enemy_Update(void)
{
	


	//当たり判定用座標更新
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
		//有効フラグをチェック
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

			// 敵の消滅処理
			//Shadow_Release(g_Enemy[i].idxShadow);
			g_Enemy[i].bUse = false;

			if (g_Enemy[i].bUse == true)
				continue;
 			deletecnt++;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Enemy_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//有効フラグをチェック
		if (g_Enemy[i].bUse == false)
			continue;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Enemy[i].mtxWorldModel);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[i].rotModel.y, g_Enemy[i].rotModel.x, g_Enemy[i].rotModel.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorldModel, &g_Enemy[i].mtxWorldModel, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy[i].posModel.x, g_Enemy[i].posModel.y, g_Enemy[i].posModel.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorldModel, &g_Enemy[i].mtxWorldModel, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[i].mtxWorldModel);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pD3DXMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();


		for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureModel);

			// 描画
			g_pMeshModel->DrawSubset(nCntMat);
		}

		// マテリアルをデフォルトに戻す
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

