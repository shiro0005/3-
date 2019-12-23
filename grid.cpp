
#include "grid.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	VALUE_MOVE		(5.0f)							// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// 回転量

#define	FIELD_WIDTH		(100.0f)						// 地面の幅(X方向)
#define	FIELD_DEPTH		(100.0f)						// 地面の奥行(Z方向)
#define FIELD_HIGHT     (100.0f)                        //地面の高さ(Y方向)

#define LINE_MAX        (1000)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGrid(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGrid = NULL;	// 頂点バッファへのポインタ

D3DXMATRIX				g_mtxWorldGrid;		// ワールドマトリックス
D3DXVECTOR3				g_posGrid;				// 地面の位置
D3DXVECTOR3				g_rotGrid;				// 地面の向き(回転)
D3DXVECTOR3				g_sclGrid;				// 地面の大きさ(スケール)

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Grid_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexGrid(pDevice);

	// 位置・回転・スケールの初期設定
	g_posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclGrid = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Grid_Finalize(void)
{
	if (g_pVtxBuffGrid != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffGrid->Release();
		g_pVtxBuffGrid = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Grid_Update(void)
{
	if (Keyboard_IsPress(DIK_A))
	{
		if (Keyboard_IsPress(DIK_W))
		{// 左奥移動
			g_posGrid.x += sinf(-D3DX_PI * 0.75f) * VALUE_MOVE;
			g_posGrid.z -= cosf(-D3DX_PI * 0.75f) * VALUE_MOVE;
		}
		else if (Keyboard_IsPress(DIK_S))
		{// 左手前移動
			g_posGrid.x += sinf(-D3DX_PI * 0.25f) * VALUE_MOVE;
			g_posGrid.z -= cosf(-D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// 左移動
			g_posGrid.x += sinf(-D3DX_PI * 0.50f) * VALUE_MOVE;
			g_posGrid.z -= cosf(-D3DX_PI * 0.50f) * VALUE_MOVE;
		}
	}
	else if (Keyboard_IsPress(DIK_D))
	{
		if (Keyboard_IsPress(DIK_W))
		{// 右奥移動
			g_posGrid.x += sinf(D3DX_PI * 0.75f) * VALUE_MOVE;
			g_posGrid.z -= cosf(D3DX_PI * 0.75f) * VALUE_MOVE;
		}
		else if (Keyboard_IsPress(DIK_S))
		{// 右手前移動
			g_posGrid.x += sinf(D3DX_PI * 0.25f) * VALUE_MOVE;
			g_posGrid.z -= cosf(D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// 右移動
			g_posGrid.x += sinf(D3DX_PI * 0.50f) * VALUE_MOVE;
			g_posGrid.z -= cosf(D3DX_PI * 0.50f) * VALUE_MOVE;
		}
	}
	else if (Keyboard_IsPress(DIK_W))
	{// 奥移動
		g_posGrid.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
		g_posGrid.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
	}
	else if (Keyboard_IsPress(DIK_S))
	{// 手前移動
		g_posGrid.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
		g_posGrid.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
	}

#if 1
	if (Keyboard_IsPress(DIK_Q))
	{// Y軸左回転
		g_rotGrid.y -= VALUE_ROTATE;
		if (g_rotGrid.y > D3DX_PI)
		{
			g_rotGrid.y -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.y < -D3DX_PI)
		{
			g_rotGrid.y += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_E))
	{// Y軸右回転
		g_rotGrid.y += VALUE_ROTATE;
		if (g_rotGrid.y > D3DX_PI)
		{
			g_rotGrid.y -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.y < -D3DX_PI)
		{
			g_rotGrid.y += D3DX_PI * 2.0f;
		}
	}
#endif

#if 1
	if (Keyboard_IsPress(DIK_UP))
	{// X軸右回転
		g_rotGrid.x += VALUE_ROTATE;
		if (g_rotGrid.x > D3DX_PI)
		{
			g_rotGrid.x -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.x < -D3DX_PI)
		{
			g_rotGrid.x += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_DOWN))
	{// X軸左回転
		g_rotGrid.x -= VALUE_ROTATE;
		if (g_rotGrid.x > D3DX_PI)
		{
			g_rotGrid.x -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.x < -D3DX_PI)
		{
			g_rotGrid.x += D3DX_PI * 2.0f;
		}
	}
#endif

#if 1
	if (Keyboard_IsPress(DIK_LEFT))
	{// Z軸右回転
		g_rotGrid.z += VALUE_ROTATE;
		if (g_rotGrid.z > D3DX_PI)
		{
			g_rotGrid.z -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.z < -D3DX_PI)
		{
			g_rotGrid.z += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_RIGHT))
	{// Z軸左回転
		g_rotGrid.z -= VALUE_ROTATE;
		if (g_rotGrid.z > D3DX_PI)
		{
			g_rotGrid.z -= D3DX_PI * 2.0f;
		}
		if (g_rotGrid.z < -D3DX_PI)
		{
			g_rotGrid.z += D3DX_PI * 2.0f;
		}
	}
#endif

	if (Keyboard_IsPress(DIK_RETURN))
	{// リセット
		g_posGrid.x = 0.0f;
		g_posGrid.y = 0.0f;
		g_posGrid.z = 0.0f;

		g_rotGrid.x = 0.0f;
		g_rotGrid.y = 0.0f;
		g_rotGrid.z = 0.0f;
	}

	DebugProc_Print((char*)"*** ３Ｄポリゴン操作 ***\n");
	DebugProc_Print((char*)"位置 [%f : %f : %f]\n", g_posGrid.x, g_posGrid.y, g_posGrid.z);
	DebugProc_Print((char*)"前移動 : Ｗ\n");
	DebugProc_Print((char*)"後移動 : Ｓ\n");
	DebugProc_Print((char*)"左移動 : Ａ\n");
	DebugProc_Print((char*)"右移動 : Ｄ\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"向き [%f : %f : %f]\n", g_rotGrid.x, g_rotGrid.y, g_rotGrid.z);
	DebugProc_Print((char*)"X軸回転 : ↑ / ↓\n");
	DebugProc_Print((char*)"Y軸回転 : Ｑ / Ｅ\n");
	DebugProc_Print((char*)"Z軸回転 : ← / →\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"位置・向きリセット : ENTER\n");
}

//=============================================================================
// 描画処理
//=============================================================================
void Grid_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldGrid);

	//スケールを反映
	D3DXMatrixScaling(&mtxScl, g_sclGrid.x, g_sclGrid.y, g_sclGrid.z);
	D3DXMatrixMultiply(&g_mtxWorldGrid, &g_mtxWorldGrid, &mtxScl);

	//回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotGrid.y, g_rotGrid.x, g_rotGrid.z);
	D3DXMatrixMultiply(&g_mtxWorldGrid, &g_mtxWorldGrid, &mtxRot);

	//移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posGrid.x, g_posGrid.y, g_posGrid.z);
	D3DXMatrixMultiply(&g_mtxWorldGrid, &g_mtxWorldGrid, &mtxTranslate);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldGrid);

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffGrid, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, NUM_POLYGON);


}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGrid(LPDIRECT3DDEVICE9 pDevice)
{

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffGrid, NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffGrid->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		for (int i = 0; i < LINE_MAX+1; i+=2) {
			pVtx[i].pos = D3DXVECTOR3(-540, 0, -500+i);
			pVtx[i+1].pos = D3DXVECTOR3(540, 0, -500+i);
		}

		//頂点カラーの設定
		for (int i = 0; i < LINE_MAX+1; i++) {
			pVtx[i].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}


		//頂点データをアンロックする
		g_pVtxBuffGrid->Unlock();
	}
	return S_OK;

}

