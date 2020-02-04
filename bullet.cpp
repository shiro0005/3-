//=============================================================================
//
// 弾丸処理 [bullet.cpp]
//
//=============================================================================
#include "bullet.h"
#include "myDirect3D.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLET		"asset/TEXTURE/bullet000.png"	// 読み込むテクスチャファイル名
#define	BULLET_SIZE_X		(20.0f)							// ビルボードの幅
#define	BULLET_SIZE_Y		(20.0f)							// ビルボードの高さ
#define	VALUE_MOVE_BULLET	(0.30f)							// 移動速度
#define BULLET_NUM_VERTEX    4
#define BULLET_NUM_POLYGON   2
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBullet(float fSizeX, float fSizeY);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBullet = NULL;	// 頂点バッファへのポインタ


BULLET g_Bullet[MAX_BULLET];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Bullet_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexBullet(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_BULLET,		// ファイルの名前
		&g_pTextureBullet);	// 読み込むメモリー

	for (int i = 0; i < MAX_BULLET; i++) {
		g_Bullet[i].g_posBullet = D3DXVECTOR3(1500, 100, -1300);
		g_Bullet[i].g_sclBullet = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bullet[i].g_moveBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_Bullet[i].col_cir = { 0.0f, 0.0f, 0.0f,10.0f };
		//g_Bullet[i].col.r = 10.0f;

		g_Bullet[i].col_aabb = { 0.0f,0.0f,0.0f,10.0f,10.0f,10.0f };


		g_Bullet[i].g_speed = 10.0f;
		g_Bullet[i].bUse = false;	}
	//g_nIndxShadowBullet=Shadow_Create(g_Bullet)

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Bullet_Finalize(void)
{
	if (g_pTextureBullet != NULL)
	{// テクスチャの開放
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	if (g_pVtxBuffBullet != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Bullet_Update(void)
{
	CAMERA *pCamera;

	// カメラの取得
	pCamera = GetCamera();

	for (int i= 0; i < MAX_BULLET; i++) {
		if (g_Bullet[i].bUse) {
			//時間経過させる
			g_Bullet[i].frame++;

			//100フレーム経過で消滅
			if (g_Bullet[i].frame > 100) {
				//影の開放処理
				Shadow_Release(g_Bullet[i].g_nIdxShadowBullet);
				g_Bullet[i].bUse = false;
			}

			//座標更新処理
			g_Bullet[i].g_posBullet += g_Bullet[i].g_moveBullet;

			//当たり判定座標更新
			//g_Bullet[i].col = { g_Bullet[i].g_posBullet.x, g_Bullet[i].g_posBullet.y, g_Bullet[i].g_posBullet.z};
			g_Bullet[i].col_cir.cx = g_Bullet[i].g_posBullet.x;
			g_Bullet[i].col_cir.cy = g_Bullet[i].g_posBullet.y;
			g_Bullet[i].col_cir.cz = g_Bullet[i].g_posBullet.z;

			g_Bullet[i].col_aabb.cx = g_Bullet[i].g_posBullet.x;
			g_Bullet[i].col_aabb.cy = g_Bullet[i].g_posBullet.y;
			g_Bullet[i].col_aabb.cz = g_Bullet[i].g_posBullet.z;

					   

			//影の位置を弾に合わせる
			D3DXVECTOR3 pos = g_Bullet[i].g_posBullet;
			pos.y = 0.0f;
			Shadow_SetPosition(g_Bullet[i].g_nIdxShadowBullet, pos);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Bullet_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//aテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < MAX_BULLET; i++) {
		
		//有効フラグチェック
		if (!g_Bullet[i].bUse) {
			continue;
		}
		// ワールドマトリックスの初期化 
		D3DXMatrixIdentity(&g_Bullet[i].g_mtxWorldBullet);
		// ビューマトリックスを取得 
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		// ポリゴンを正面に向ける
#if 1
	// 逆行列をもとめる 
		D3DXMatrixInverse(&g_Bullet[i].g_mtxWorldBullet, NULL, &mtxView);

		// 平行移動の部分は無視する
		g_Bullet[i].g_mtxWorldBullet._41 = 0.0f;
		g_Bullet[i].g_mtxWorldBullet._42 = 0.0f;
		g_Bullet[i].g_mtxWorldBullet._43 = 0.0f;
#else
		g_Bullet[i].g_mtxWorldBullet._11 = mtxView._11;
		g_Bullet[i].g_mtxWorldBullet. 12 = mtxView._21;
		g_Bullet[i].g_mtxWorldBullet._13 = mtxView._31;
		g_Bullet[i].g_mtxWorldBullet._21 = mtxView._12;//
		g_Bullet[i].g_mtxWorldBullet._22 = mtxView._22;//この三つをコメントアウトすると垂直方向を無視できる
		g_Bullet[i].g_mtxWorldBullet._23 = mtxView._32;//
		g_Bullet[i].g_mtxWorldBullet._31 = mtxView._13;
		g_Bullet[i].g_mtxWorldBullet. 32 = mtxView._23;
		g_Bullet[i].g_mtxWorldBullet._33 = mtxView._33;
#endif

		// スケールを反映 
		D3DXMatrixScaling(&mtxScale, g_Bullet[i].g_sclBullet.x, g_Bullet[i].g_sclBullet.y, g_Bullet[i].g_sclBullet.z);
		D3DXMatrixMultiply(&g_Bullet[i].g_mtxWorldBullet,
			&g_Bullet[i].g_mtxWorldBullet, &mtxScale);

		// 移動を反映 
		D3DXMatrixTranslation(&mtxTranslate, g_Bullet[i].g_posBullet.x,
			g_Bullet[i].g_posBullet.y, g_Bullet[i].g_posBullet.z);
		D3DXMatrixMultiply(&g_Bullet[i].g_mtxWorldBullet, &g_Bullet[i].g_mtxWorldBullet, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Bullet[i].g_mtxWorldBullet);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定 
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBullet);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, BULLET_NUM_POLYGON);
	}
	// ラインティングを有効にする 
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	//aテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BULLET_NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBullet,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f); -

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
		g_pVtxBuffBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBullet(float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pVtxBuffBullet->Unlock();
	}
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void Bullet_Create(float x, float y,float z, D3DXVECTOR2 dir)
{
	for (int i = 0; i < MAX_BULLET; i++) {

		// もし弾がすでに有効だったら？
		if (g_Bullet[i].bUse) {
			continue;
		}

		// 弾の座標に引数の値を代入する
		g_Bullet[i].g_posBullet.x = x;
		g_Bullet[i].g_posBullet.y = y;
		g_Bullet[i].g_posBullet.z = z;

		// 弾の移動方向を指定する
		D3DXVec2Normalize(&dir, &dir);
		g_Bullet[i].g_moveBullet.x= dir.x*5.0f;
		g_Bullet[i].g_moveBullet.y = 0.0f;
		g_Bullet[i].g_moveBullet.z = dir.y*5.0f;

		//// 弾の移動方向から弾の回転角度を求める
		//g_Bullets[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		//g_Bullets[i].collision.cx = x;
		//g_Bullets[i].collision.cy = y;
		//g_Bullets[i].collision.r = BULLET_WIDTH * 0.8f;

		// 有効フレームを初期化
		g_Bullet[i].frame = 0;

		//影を作成
		g_Bullet[i].g_nIdxShadowBullet=Shadow_Create(g_Bullet[i].g_posBullet, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		// 弾を有効にする
		g_Bullet[i].bUse = true;

		break;
	}
}

bool Bullet_IsEnable(int index) {
	return g_Bullet[index].bUse;
}

CIRCLE *Bullet_GetCollision(int index){
	return &g_Bullet[index].col_cir;
}

AABB *Bullet_GetCollisionBox(int index) {
	return &g_Bullet[index].col_aabb;
}

BULLET *GetBullet() {
	return &g_Bullet[0];
}