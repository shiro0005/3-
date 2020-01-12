#include "wallL.h"
#include "box.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "box.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	VALUE_MOVE		(5.0f)							// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.01f)				// 回転量

#define	FIELD_WIDTH		(100.0f)						// 地面の幅(X方向)
#define	FIELD_DEPTH		(100.0f)						// 地面の奥行(Z方向)
#define FIELD_HIGHT     (100.0f)                        //地面の高さ(Y方向)

#define BLOCK_X 11                                       //ブロック横の数  変えるときは波生成の数字も変える
#define BLOCK_Y 1                                       //縦の数

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
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexWallL(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField1 = NULL;	// 頂点バッファへのポインタ
static LPDIRECT3DINDEXBUFFER9 g_pIdxBuffField1 = NULL;

#define NUM_BOX 1

typedef struct {
	D3DXMATRIX				g_mtxWorldField;		// ワールドマトリックス
	D3DXVECTOR3				g_posField;				// 地面の位置
	D3DXVECTOR3				g_rot1Field;				// 地面の向き(回転)
	D3DXVECTOR3				g_rot2Field;				// 地面の向き(回転)
	D3DXVECTOR3				g_sclField;				// 地面の大きさ(スケール)
}BOX_wallL;

BOX_wallL g_Field[NUM_BOX];

static int g_NumVertex1 = (BLOCK_X + 1)*(BLOCK_Y + 1);                //頂点数
static int g_NumIndex1 = ((BLOCK_X + 1)*BLOCK_Y + (BLOCK_Y - 1)) * 2; //インデックス数 ((BLOCK_X + 1)*2)*BLOCK_Y+(BLOCK_Y-1)*2
static int g_NumPolygon1 = (BLOCK_X * 2)*BLOCK_Y + (BLOCK_Y - 1) * 4; //ポリゴン数
static int zcnt1[6] = {};//波生成用


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT WallL_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexWallL(pDevice);

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < NUM_BOX; i++) {
		g_Field[i].g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Field[i].g_rot1Field = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Field[i].g_rot2Field = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Field[i].g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void WallL_Finalize(void)
{
	if (g_pIdxBuffField1 != NULL) {//インデックスバッファの開放
		g_pIdxBuffField1->Release();
		g_pIdxBuffField1 = NULL;
	}

	if (g_pVtxBuffField1 != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffField1->Release();
		g_pVtxBuffField1 = NULL;
	}
}



//=============================================================================
// 更新処理
//=============================================================================


//=============================================================================
// 描画処理
//=============================================================================
void WallL_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();


	for (int i = 0; i < NUM_BOX; i++) {
		//頂点の法線をノーマライズする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

		//ワールドマトリックスの初期化
		D3DXMATRIX mtxWorld;
		D3DXMatrixIdentity(&mtxWorld);


		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
		//頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffField1, 0, sizeof(VERTEX_3D));

		pDevice->SetIndices(g_pIdxBuffField1);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

		//ポリゴンの描画(インデックスバッファ用）
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVertex1, 0, g_NumPolygon1);//一番最後がポリゴン数,右から2番目が頂点数

		////ポリゴンの描画
		//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexWallL(LPDIRECT3DDEVICE9 pDevice)
{

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVertex1,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffField1, NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;


		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffField1->Lock(0, 0, (void**)&pVtx, 0);


		for (int z = 0; z < BLOCK_Y + 1; z++) {
			for (int x = 0; x < BLOCK_X + 1; x++) {
				//頂点座標の設定
				pVtx[z * (BLOCK_X + 1) + x].pos = D3DXVECTOR3(0.0f, 100 + z * -100.0f, -1100+x*100.0f);//D3DXVECTOR3(x*100.0f,0-z*-100.0f, 0.0f);

				//法線ベクトルの設定
				pVtx[z * (BLOCK_X + 1) + x].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

				//頂点カラーの設定
				pVtx[z * (BLOCK_X + 1) + x].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//UV値設定
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

		//頂点データをアンロックする
		g_pVtxBuffField1->Unlock();
	}
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD)*g_NumIndex1,//インデックス数は頂点座標の数より多くなる
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffField1,
		NULL))) {
		return E_FAIL;
	}

	{//インデックスバッファの中身を埋める
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
//	/*//棒倒し法を使った壁（1）の生成（1行めのみ）
//	z = 2; //1行め
//	for (x = 2; x < WIDTH - 1; x += 2) //xの要素番号2から要素番号mWIDTH-1まで、1マス飛ばしで棒倒し。
//	{
//		wallmap[z][x] = 1; //中心から……
//		if (ran >= 1 && ran <= 3) //rが1から3のとき
//		{
//			if (wallmap[z - 1][x] == 0) //上に棒（壁）がなければ
//			{
//				wallmap[z - 1][x] = 1; //上に棒を倒す。
//			}
//			else if (wallmap[z - 1][x] == 1) //上に棒（壁）があれば
//			{
//				x = x - 2; //棒を倒さずに、乱数生成をやり直す。
//			}
//		}
//		if (ran >= 4 && ran <= 6) //rが4から6のとき
//		{
//			if (wallmap[z + 1][x] == 0) //下に棒（壁）がなければ
//			{
//				wallmap[z + 1][x] = 1; //下に棒を倒す。
//			}
//			else if (wallmap[z + 1][x] == 1) //下に棒（壁）があれば
//			{
//				x = x - 2; //棒を倒さずに、乱数生成をやり直す。
//			}
//		}
//		if (ran >= 7 && ran <= 9) //rが7から9のとき
//		{
//			if (wallmap[z][x - 1] == 0) //左に棒（壁）がなければ
//			{
//				wallmap[z][x - 1] = 1; //左に棒を倒す。
//			}
//			else if (wallmap[z][x - 1] == 1) //左に棒（壁）があれば
//			{
//				x = x - 2; //棒を倒さずに、乱数生成をやり直す。
//			}
//		}
//		if (ran >= 10 && ran <= 12) //rが10から12のとき
//		{
//			if (wallmap[z][x + 1] == 0) //右に棒（壁）がなければ
//			{
//				wallmap[z][x + 1] = 1; //右に棒を倒す。
//			}
//			else if (wallmap[z][x + 1] == 1) //右に棒（壁）があれば
//			{
//				x = x - 2; //棒を倒さずに、乱数生成をやり直す。
//			}
//		}
//	}
//
//	//棒倒し法を使った壁（1）の生成（2行め以降）
//	for (z = 4; z < DEPTH - 1; z += 2) //yの要素番号4から要素番号max_y-1まで、1マス飛ばしで棒倒し。
//	{
//		for (x = 2; x < WIDTH - 1; x += 2) //xの要素番号2から要素番号max_x-1まで、1マス飛ばしで棒倒し。
//		{
//			wallmap[z][x] = 1; //中心から……
//			if (ran >= 1 && ran <= 4) //rが1から4のとき
//			{
//				if (wallmap[z + 1][x] == 0) //下に棒（壁）がなければ
//				{
//					wallmap[z + 1][x] = 1; //下に棒を倒す。
//				}
//				else if (wallmap[z + 1][x] == 1) //下に棒（壁）があれば
//				{
//					x = x - 2; //棒を倒さずに、乱数生成をやり直す。
//				}
//			}
//			if (ran >= 5 && ran <= 8) //rが5から8のとき
//			{
//				if (wallmap[z][x - 1] == 0) //左に棒（壁）がなければ
//				{
//					wallmap[z][x - 1] = 1; //左に棒を倒す。
//				}
//				else if (wallmap[z][x - 1] == 1) //左に棒（壁）があれば
//				{
//					x = x - 2; //棒を倒さずに、乱数生成をやり直す。
//				}
//			}
//			if (ran >= 9 && ran <= 12) //rが9から12のとき
//			{
//				if (wallmap[z][x + 1] == 0) //右に棒（壁）がなければ
//				{
//					wallmap[z][x + 1] = 1; //右に棒を倒す。
//				}
//				else if (wallmap[z][x + 1] == 1) //右に棒（壁）があれば
//				{
//					x = x - 2; //棒を倒さずに、乱数生成をやり直す。
//				}
//			}
//		}
//	}*/
//
//	wallmap[0][1] = 0; //スタート地点の壁を撤去する。
//	wallmap[DEPTH - 1][WIDTH - 2] = 0; //ゴール地点の壁を撤去する。
//
//	//wallmapを使ってボックス作成
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