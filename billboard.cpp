//=============================================================================
//
// ビルボード処理 [billboard.cpp]
//
//=============================================================================
#include "billboard.h"
#include "myDirect3D.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BILLBOARD		"asset/TEXTURE/bullet000.png"	// 読み込むテクスチャファイル名
#define	BILLBOARD_SIZE_X		(20.0f)							// ビルボードの幅
#define	BILLBOARD_SIZE_Y		(20.0f)							// ビルボードの高さ
#define	VALUE_MOVE_BILLBOARD	(0.30f)							// 移動速度
#define	VALUE_JUMP				(10.0f)							// ジャンプ力
#define	VALUE_GRAVITY			(0.45f)							// 重力
#define	RATE_REGIST				(0.075f)						// 抵抗係数
#define	RATE_REFRECT			(-0.90f)						// 反射係数
#define BILLBOARD_NUM_VERTEX    4
#define BILLBOARD_NUM_POLYGON   2
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBillboard(float fSizeX, float fSizeY);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBillboard = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBillboard = NULL;	// 頂点バッファへのポインタ

static D3DXMATRIX				g_mtxWorldBillboard;		// ワールドマトリックス
static D3DXVECTOR3				g_posBillboard;				// 位置
static D3DXVECTOR3				g_sclBillboard;				// スケール
static D3DXVECTOR3				g_moveBillboard;			// 移動量
static int						g_nIdxShadowBillboard;		// 影ID
static bool						g_bEnableGravity = false;	// 重力を有効にするかどうか

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Billboard_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexBillboard(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_BILLBOARD,		// ファイルの名前
		&g_pTextureBillboard);	// 読み込むメモリー

	g_posBillboard = D3DXVECTOR3(1500, 100, -1300);
	g_sclBillboard = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_moveBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_bEnableGravity = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Billboard_Finalize(void)
{
	if (g_pTextureBillboard != NULL)
	{// テクスチャの開放
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	if (g_pVtxBuffBillboard != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Billboard_Update(void)
{
	CAMERA *pCamera;

	// カメラの取得
	pCamera = GetCamera();

	if (Keyboard_IsTrigger(DIK_J))
	{// ジャンプ
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


	// 重力ON/OFF
	if (Keyboard_IsTrigger(DIK_G))
	{
		g_bEnableGravity ^= 1;
		if (g_bEnableGravity == false)
		{
			g_moveBillboard.y = 0.0f;
		}
	}

	DebugProc_Print("*** ボール操作 ***\n");
	DebugProc_Print("[ボールの位置  ：(%f : %f : %f)]\n", g_posBillboard.x, g_posBillboard.y, g_posBillboard.z);
	DebugProc_Print("[ボールの移動量：(%f : %f : %f)]\n", g_moveBillboard.x, g_moveBillboard.y, g_moveBillboard.z);
	DebugProc_Print("前移動   : ↑\n");
	DebugProc_Print("後移動   : ↓\n");
	DebugProc_Print("左移動   : ←\n");
	DebugProc_Print("右移動   : →\n");
	DebugProc_Print("ジャンプ : J\n");
	DebugProc_Print("\n");

	DebugProc_Print("*** 重力 ***\n");
	DebugProc_Print("ON / OFF : [G] ( 重力 = ");

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
// 描画処理
//=============================================================================
void Billboard_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//aテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// ワールドマトリックスの初期化 
	D3DXMatrixIdentity(&g_mtxWorldBillboard);
	// ビューマトリックスを取得 
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	// ポリゴンを正面に向ける
#if 1
	// 逆行列をもとめる 
	D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);
	// 平行移動の部分は無視する
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

	// スケールを反映 
	D3DXMatrixScaling(&mtxScale, g_sclBillboard.x, g_sclBillboard.y, g_sclBillboard.z);
	D3DXMatrixMultiply(&g_mtxWorldBillboard,
		&g_mtxWorldBillboard, &mtxScale);

	// 移動を反映 
	D3DXMatrixTranslation(&mtxTranslate, g_posBillboard.x,
		g_posBillboard.y, g_posBillboard.z);
	D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTranslate);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));
	// 頂点フォーマットの設定 
	pDevice->SetFVF(FVF_VERTEX_3D);
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBillboard);
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, BILLBOARD_NUM_POLYGON);
	// ラインティングを有効にする 
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//aテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BILLBOARD_NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBillboard,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f); -

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 頂点データをアンロックする
		g_pVtxBuffBillboard->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBillboard(float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pVtxBuffBillboard->Unlock();
	}
}


