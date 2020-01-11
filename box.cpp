
#include "box.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	VALUE_MOVE		(5.0f)							// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.01f)				// 回転量

#define	BOX_WIDTH		(50.0f)						// 地面の幅(X方向)
#define	BOX_DEPTH		(50.0f)						// 地面の奥行(Z方向)
#define BOX_HIGHT     (50.0f)                        //地面の高さ(Y方向)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexbox(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffbox = NULL;	// 頂点バッファへのポインタ
static LPDIRECT3DINDEXBUFFER9 g_pIdxBuffbox = NULL;

#define NUM_BOX 1

typedef struct {
	D3DXMATRIX				g_mtxWorldbox;		// ワールドマトリックス
	D3DXVECTOR3				g_posbox;				// 地面の位置
	D3DXVECTOR3				g_rot1box;				// 地面の向き(回転)
	D3DXVECTOR3				g_rot2box;				// 地面の向き(回転)
	D3DXVECTOR3				g_sclbox;				// 地面の大きさ(スケール)
}BOX_2;

BOX_2 g_Box[NUM_BOX];

int g_NumIndexbox = 36;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT box_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexbox(pDevice);

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < NUM_BOX; i++) {
		g_Box[i].g_posbox = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Box[i].g_rot1box = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Box[i].g_rot2box = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Box[i].g_sclbox = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void box_Finalize(void)
{
	if (g_pIdxBuffbox != NULL) {//インデックスバッファの開放
		g_pIdxBuffbox->Release();
		g_pIdxBuffbox = NULL;
	}

	if (g_pVtxBuffbox != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffbox->Release();
		g_pVtxBuffbox = NULL;
	}
}


//消したもの
/*

	if (Keyboard_IsPress(DIK_A))
	{
		if (Keyboard_IsPress(DIK_W))
		{// 左奥移動
			g_posbox.x += sinf(-D3DX_PI * 0.75f) * VALUE_MOVE;
			g_posbox.z -= cosf(-D3DX_PI * 0.75f) * VALUE_MOVE;
		}
		else if (Keyboard_IsPress(DIK_S))
		{// 左手前移動
			g_posbox.x += sinf(-D3DX_PI * 0.25f) * VALUE_MOVE;
			g_posbox.z -= cosf(-D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// 左移動
			g_posbox.x += sinf(-D3DX_PI * 0.50f) * VALUE_MOVE;
			g_posbox.z -= cosf(-D3DX_PI * 0.50f) * VALUE_MOVE;
		}
	}
	else if (Keyboard_IsPress(DIK_D))
	{
		if (Keyboard_IsPress(DIK_W))
		{// 右奥移動
			g_posbox.x += sinf(D3DX_PI * 0.75f) * VALUE_MOVE;
			g_posbox.z -= cosf(D3DX_PI * 0.75f) * VALUE_MOVE;
		}
		else if (Keyboard_IsPress(DIK_S))
		{// 右手前移動
			g_posbox.x += sinf(D3DX_PI * 0.25f) * VALUE_MOVE;
			g_posbox.z -= cosf(D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// 右移動
			g_posbox.x += sinf(D3DX_PI * 0.50f) * VALUE_MOVE;
			g_posbox.z -= cosf(D3DX_PI * 0.50f) * VALUE_MOVE;
		}
	}
	else if (Keyboard_IsPress(DIK_W))
	{// 奥移動
		g_posbox.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
		g_posbox.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
	}
	else if (Keyboard_IsPress(DIK_S))
	{// 手前移動
		g_posbox.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
		g_posbox.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
	}

#if 1
	if (Keyboard_IsPress(DIK_Q))
	{// Y軸左回転
		g_rotbox.y -= VALUE_ROTATE;
		if (g_rotbox.y > D3DX_PI)
		{
			g_rotbox.y -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.y < -D3DX_PI)
		{
			g_rotbox.y += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_E))
	{// Y軸右回転
		g_rotbox.y += VALUE_ROTATE;
		if (g_rotbox.y > D3DX_PI)
		{
			g_rotbox.y -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.y < -D3DX_PI)
		{
			g_rotbox.y += D3DX_PI * 2.0f;
		}
	}
#endif

#if 1
	if (Keyboard_IsPress(DIK_UP))
	{// X軸右回転
		g_rotbox.x += VALUE_ROTATE;
		if (g_rotbox.x > D3DX_PI)
		{
			g_rotbox.x -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.x < -D3DX_PI)
		{
			g_rotbox.x += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_DOWN))
	{// X軸左回転
		g_rotbox.x -= VALUE_ROTATE;
		if (g_rotbox.x > D3DX_PI)
		{
			g_rotbox.x -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.x < -D3DX_PI)
		{
			g_rotbox.x += D3DX_PI * 2.0f;
		}
	}
#endif

#if 1
	if (Keyboard_IsPress(DIK_LEFT))
	{// Z軸右回転
		g_rotbox.z += VALUE_ROTATE;
		if (g_rotbox.z > D3DX_PI)
		{
			g_rotbox.z -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.z < -D3DX_PI)
		{
			g_rotbox.z += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_RIGHT))
	{// Z軸左回転
		g_rotbox.z -= VALUE_ROTATE;
		if (g_rotbox.z > D3DX_PI)
		{
			g_rotbox.z -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.z < -D3DX_PI)
		{
			g_rotbox.z += D3DX_PI * 2.0f;
		}
	}
#endif

	if (Keyboard_IsPress(DIK_RETURN))
	{// リセット
		g_posbox.x = 0.0f;
		g_posbox.y = 0.0f;
		g_posbox.z = 0.0f;

		g_rotbox.x = 0.0f;
		g_rotbox.y = 0.0f;
		g_rotbox.z = 0.0f;
	}

	DebugProc_Print((char*)"*** ３Ｄポリゴン操作 ***\n");
	DebugProc_Print((char*)"位置 [%f : %f : %f]\n", g_posbox.x, g_posbox.y, g_posbox.z);
	DebugProc_Print((char*)"前移動 : Ｗ\n");
	DebugProc_Print((char*)"後移動 : Ｓ\n");
	DebugProc_Print((char*)"左移動 : Ａ\n");
	DebugProc_Print((char*)"右移動 : Ｄ\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"向き [%f : %f : %f]\n", g_rotbox.x, g_rotbox.y, g_rotbox.z);
	DebugProc_Print((char*)"X軸回転 : ↑ / ↓\n");
	DebugProc_Print((char*)"Y軸回転 : Ｑ / Ｅ\n");
	DebugProc_Print((char*)"Z軸回転 : ← / →\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"位置・向きリセット : ENTER\n");
*/


//=============================================================================
// 更新処理
//=============================================================================
void box_Update(void)
{


	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	//ワールドマトリックスの初期化
	for (int i = 0; i < NUM_BOX; i++) {
		D3DXMatrixIdentity(&g_Box[i].g_mtxWorldbox);
	}


	//0番//

	//g_Box[0].g_rot1box.y += 0.01f;
	//回転を反映
	//Yaw：Y軸回転　Pitch : X軸回転　Roll : Z軸回転  Pitchが９０度になりRollができなくなる：ジンバルロック
	/*D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[0].g_rot1box.y, g_Box[0].g_rot1box.x, g_Box[0].g_rot1box.z);
	D3DXMatrixMultiply(&g_Box[0].g_mtxWorldbox, &g_Box[0].g_mtxWorldbox, &mtxRot);

	g_Box[0].g_posbox.x = 100.0f;
	g_Box[0].g_posbox.y = 100.0f;
	g_Box[0].g_posbox.z = 100.0f;*/

	//移動を反映
	/*D3DXMatrixTranslation(&mtxTranslate, g_Box[0].g_posbox.x, g_Box[0].g_posbox.y, g_Box[0].g_posbox.z);
	D3DXMatrixMultiply(&g_Box[0].g_mtxWorldbox, &g_Box[0].g_mtxWorldbox, &mtxTranslate);*/


	//1番//

	//g_Box[1].g_rot1box.y -= 0.03f;
	////回転を反映
	////Yaw：Y軸回転　Pitch : X軸回転　Roll : Z軸回転  Pitchが９０度になりRollができなくなる：ジンバルロック
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot1box.y, g_Box[1].g_rot1box.x, g_Box[1].g_rot1box.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldbox, &g_Box[1].g_mtxWorldbox, &mtxRot);

	/*g_Box[1].g_posbox.x = 200.0f;
	g_Box[1].g_posbox.z = 200.0f;*/

	//移動を反映
	/*D3DXMatrixTranslation(&mtxTranslate, g_Box[1].g_posbox.x, g_Box[1].g_posbox.y, g_Box[1].g_posbox.z);
	D3DXMatrixMultiply(&g_Box[1].g_mtxWorldbox, &g_Box[1].g_mtxWorldbox, &mtxTranslate);*/

	//g_Box[1].g_rot2box.y += 0.03f;
	////打消し回転を反映
	////Yaw：Y軸回転　Pitch : X軸回転　Roll : Z軸回転  Pitchが９０度になりRollができなくなる：ジンバルロック
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot2box.y, g_Box[1].g_rot2box.x, g_Box[1].g_rot2box.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldbox, &g_Box[1].g_mtxWorldbox, &mtxRot);

	//子供＊親
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldbox, &g_Box[1].g_mtxWorldbox, &g_Box[0].g_mtxWorldbox);


	//D3DXVECTOR3 pos;

	//pos.x = 100.0f;
	//pos.y = 100.0f;
	//pos.z = 100.0f;

	////移動を反映
	//D3DXMatrixTranslation(&mtxTranslate,pos.x,pos.y, pos.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldbox, &g_Box[1].g_mtxWorldbox, &mtxTranslate);


/*	if (Keyboard_IsPress(DIK_A))
	{
		if (Keyboard_IsPress(DIK_W))
		{// 左奥移動
			g_posbox.x += sinf(-D3DX_PI * 0.75f) * VALUE_MOVE;
			g_posbox.z -= cosf(-D3DX_PI * 0.75f) * VALUE_MOVE;
		}
		else if (Keyboard_IsPress(DIK_S))
		{// 左手前移動
			g_posbox.x += sinf(-D3DX_PI * 0.25f) * VALUE_MOVE;
			g_posbox.z -= cosf(-D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// 左移動
			g_posbox.x += sinf(-D3DX_PI * 0.50f) * VALUE_MOVE;
			g_posbox.z -= cosf(-D3DX_PI * 0.50f) * VALUE_MOVE;
		}
	}
	else if (Keyboard_IsPress(DIK_D))
	{
		if (Keyboard_IsPress(DIK_W))
		{// 右奥移動
			g_posbox.x += sinf(D3DX_PI * 0.75f) * VALUE_MOVE;
			g_posbox.z -= cosf(D3DX_PI * 0.75f) * VALUE_MOVE;
		}
		else if (Keyboard_IsPress(DIK_S))
		{// 右手前移動
			g_posbox.x += sinf(D3DX_PI * 0.25f) * VALUE_MOVE;
			g_posbox.z -= cosf(D3DX_PI * 0.25f) * VALUE_MOVE;
		}
		else
		{// 右移動
			g_posbox.x += sinf(D3DX_PI * 0.50f) * VALUE_MOVE;
			g_posbox.z -= cosf(D3DX_PI * 0.50f) * VALUE_MOVE;
		}
	}
	else if (Keyboard_IsPress(DIK_W))
	{// 奥移動
		g_posbox.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
		g_posbox.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
	}
	else if (Keyboard_IsPress(DIK_S))
	{// 手前移動
		g_posbox.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
		g_posbox.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
	}

#if 1
	if (Keyboard_IsPress(DIK_Q))
	{// Y軸左回転
		g_rotbox.y -= VALUE_ROTATE;
		if (g_rotbox.y > D3DX_PI)
		{
			g_rotbox.y -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.y < -D3DX_PI)
		{
			g_rotbox.y += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_E))
	{// Y軸右回転
		g_rotbox.y += VALUE_ROTATE;
		if (g_rotbox.y > D3DX_PI)
		{
			g_rotbox.y -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.y < -D3DX_PI)
		{
			g_rotbox.y += D3DX_PI * 2.0f;
		}
	}
#endif

#if 1
	if (Keyboard_IsPress(DIK_UP))
	{// X軸右回転
		g_rotbox.x += VALUE_ROTATE;
		if (g_rotbox.x > D3DX_PI)
		{
			g_rotbox.x -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.x < -D3DX_PI)
		{
			g_rotbox.x += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_DOWN))
	{// X軸左回転
		g_rotbox.x -= VALUE_ROTATE;
		if (g_rotbox.x > D3DX_PI)
		{
			g_rotbox.x -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.x < -D3DX_PI)
		{
			g_rotbox.x += D3DX_PI * 2.0f;
		}
	}
#endif

#if 1
	if (Keyboard_IsPress(DIK_LEFT))
	{// Z軸右回転
		g_rotbox.z += VALUE_ROTATE;
		if (g_rotbox.z > D3DX_PI)
		{
			g_rotbox.z -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.z < -D3DX_PI)
		{
			g_rotbox.z += D3DX_PI * 2.0f;
		}
	}
	if (Keyboard_IsPress(DIK_RIGHT))
	{// Z軸左回転
		g_rotbox.z -= VALUE_ROTATE;
		if (g_rotbox.z > D3DX_PI)
		{
			g_rotbox.z -= D3DX_PI * 2.0f;
		}
		if (g_rotbox.z < -D3DX_PI)
		{
			g_rotbox.z += D3DX_PI * 2.0f;
		}
	}
#endif

	if (Keyboard_IsPress(DIK_RETURN))
	{// リセット
		g_posbox.x = 0.0f;
		g_posbox.y = 0.0f;
		g_posbox.z = 0.0f;

		g_rotbox.x = 0.0f;
		g_rotbox.y = 0.0f;
		g_rotbox.z = 0.0f;
	}

	DebugProc_Print((char*)"*** ３Ｄポリゴン操作 ***\n");
	DebugProc_Print((char*)"位置 [%f : %f : %f]\n", g_posbox.x, g_posbox.y, g_posbox.z);
	DebugProc_Print((char*)"前移動 : Ｗ\n");
	DebugProc_Print((char*)"後移動 : Ｓ\n");
	DebugProc_Print((char*)"左移動 : Ａ\n");
	DebugProc_Print((char*)"右移動 : Ｄ\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"向き [%f : %f : %f]\n", g_rotbox.x, g_rotbox.y, g_rotbox.z);
	DebugProc_Print((char*)"X軸回転 : ↑ / ↓\n");
	DebugProc_Print((char*)"Y軸回転 : Ｑ / Ｅ\n");
	DebugProc_Print((char*)"Z軸回転 : ← / →\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"位置・向きリセット : ENTER\n");

	//打消し回転を反映
	//Yaw：Y軸回転　Pitch : X軸回転　Roll : Z軸回転  Pitchが９０度になりRollができなくなる：ジンバルロック
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot1_x, g_rotbox.x, g_rotbox.z);
	D3DXMatrixMultiply(&g_mtxWorldbox, &g_mtxWorldbox, &mtxRot);

	//g_posbox.x = 100 * Xpos;
	//g_posbox.z = 100 * Zpos;

	//移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posbox.x, g_posbox.y, g_posbox.z);
	D3DXMatrixMultiply(&g_mtxWorldbox, &g_mtxWorldbox, &mtxTranslate);

	//打消し回転
	//g_rot1_x -= 0.03f;

	//回転
	//g_rot2_x += 0.03f;



	//g_sclbox.x += sizemove;
	//g_sclbox.y += sizemove;
	//g_sclbox.z += sizemove;


	//if (g_sclbox.x > 2.0f|| g_sclbox.x < 1.0f) {
	//	sizemove *= -1.0f;
	//}
	//



	//スケールを反映
	D3DXMatrixScaling(&mtxScl, g_sclbox.x, g_sclbox.y, g_sclbox.z);
	D3DXMatrixMultiply(&g_mtxWorldbox, &g_mtxWorldbox, &mtxScl);


	g_rotbox.y -= VALUE_ROTATE;
	if (g_rotbox.y > D3DX_PI)
	{
		g_rotbox.y -= D3DX_PI * 2.0f;
	}
	if (g_rotbox.y < -D3DX_PI)
	{
		g_rotbox.y += D3DX_PI * 2.0f;
	}


	//回転を反映
	//Yaw：Y軸回転　Pitch : X軸回転　Roll : Z軸回転  Pitchが９０度になりRollができなくなる：ジンバルロック
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot2_x, g_rotbox.x, g_rotbox.z);
	D3DXMatrixMultiply(&g_mtxWorldbox, &g_mtxWorldbox, &mtxRot);




	if (g_posbox.x>=box_WIDTH) {
		XX = -1.0f;
	}
	else if (g_posbox.x <= -box_WIDTH) {
		XX = 1.0f;
	}

	g_posbox.x += 1.0f* XX;

	*/


}

//=============================================================================
// 描画処理
//=============================================================================
void box_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();


	//頂点の法線をノーマライズする
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);


	for (int i = 0; i < NUM_BOX; i++) {
		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Box[i].g_mtxWorldbox);
		/*ライティングモードTRUEの処理内容
		//LIGHTベクトルを作成
		D3DXVECTOR3 light;
		light.x = 0.0f;
		light.y = -1.0f;
		light.z = 0.0f;

		{//頂点バッファの中身を埋める
			VERTEX_3D *pVtx;

			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pVtxBuffbox->Lock(0, 0, (void**)&pVtx, 0);

			D3DXVECTOR3 calc_nor;

			//ベクトルに行列をかけて変形してくれる関数
			D3DXVec3TransformNormal(&calc_nor, &pVtx[0].nor, &g_mtxWorldbox);//(答えを代入,かけるもの,かけるもの)

			float color =D3DXVec3Dot(&light, &calc_nor);//内積計算

			if (color > 0) {//内積の値が0を超えたらもう黒なので0にする
				color = 0;
			}

			//-1～0の値を1～0に書き換える
			color = fabs(color);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(color, color, color, 1.0f);
			pVtx[1].col = D3DXCOLOR(color, color, color, 1.0f);
			pVtx[2].col = D3DXCOLOR(color, color, color, 1.0f);
			pVtx[3].col = D3DXCOLOR(color, color, color, 1.0f);
			pVtx[4].col = D3DXCOLOR(color, color, color, 1.0f);
			pVtx[5].col = D3DXCOLOR(color, color, color, 1.0f);



			//頂点データをアンロックする
			g_pVtxBuffbox->Unlock();
		}
		*/
		//頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffbox, 0, sizeof(VERTEX_3D));

		pDevice->SetIndices(g_pIdxBuffbox);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

		//ポリゴンの描画(インデックスバッファ用）
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_NumIndexbox, 0, 12);//一番最後がポリゴン数

		////ポリゴンの描画
		//pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, NUM_POLYGON);

	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexbox(LPDIRECT3DDEVICE9 pDevice)
{

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffbox, NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffbox->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HIGHT, -BOX_DEPTH);//上
		pVtx[1].pos = D3DXVECTOR3(BOX_WIDTH, BOX_HIGHT, -BOX_DEPTH);
		pVtx[2].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HIGHT, -BOX_DEPTH);
		pVtx[3].pos = D3DXVECTOR3(BOX_WIDTH, -BOX_HIGHT, -BOX_DEPTH);
		pVtx[4].pos = D3DXVECTOR3(BOX_WIDTH, -BOX_HIGHT, BOX_DEPTH);
		pVtx[5].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HIGHT, BOX_DEPTH);
		pVtx[6].pos = D3DXVECTOR3(BOX_WIDTH, BOX_HIGHT, BOX_DEPTH);//前
		pVtx[7].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HIGHT, BOX_DEPTH);
		//pVtx[8].pos = D3DXVECTOR3(-box_WIDTH, -box_HIGHT, -box_DEPTH);
		//pVtx[9].pos = D3DXVECTOR3(box_WIDTH, box_HIGHT, -box_DEPTH);
		//pVtx[10].pos = D3DXVECTOR3(box_WIDTH, -box_HIGHT, -box_DEPTH);
		//pVtx[11].pos = D3DXVECTOR3(-box_WIDTH, -box_HIGHT, -box_DEPTH);
		//pVtx[12].pos = D3DXVECTOR3(box_WIDTH, box_HIGHT, -box_DEPTH);//右
		//pVtx[13].pos = D3DXVECTOR3(box_WIDTH, box_HIGHT, box_DEPTH);
		//pVtx[14].pos = D3DXVECTOR3(box_WIDTH, -box_HIGHT, -box_DEPTH);
		//pVtx[15].pos = D3DXVECTOR3(box_WIDTH, box_HIGHT, box_DEPTH);
		//pVtx[16].pos = D3DXVECTOR3(box_WIDTH, -box_HIGHT, box_DEPTH);
		//pVtx[17].pos = D3DXVECTOR3(box_WIDTH, -box_HIGHT, -box_DEPTH);
		//pVtx[18].pos = D3DXVECTOR3(box_WIDTH, -box_HIGHT, -box_DEPTH);//下
		//pVtx[19].pos = D3DXVECTOR3(box_WIDTH, -box_HIGHT, box_DEPTH);
		//pVtx[20].pos = D3DXVECTOR3(-box_WIDTH, -box_HIGHT, -box_DEPTH);
		//pVtx[21].pos = D3DXVECTOR3(box_WIDTH, -box_HIGHT, box_DEPTH);
		//pVtx[22].pos = D3DXVECTOR3(-box_WIDTH, -box_HIGHT, box_DEPTH);
		//pVtx[23].pos = D3DXVECTOR3(-box_WIDTH, -box_HIGHT, -box_DEPTH);
		//pVtx[24].pos = D3DXVECTOR3(-box_WIDTH, box_HIGHT, box_DEPTH);//左
		//pVtx[25].pos = D3DXVECTOR3(-box_WIDTH, box_HIGHT, -box_DEPTH);
		//pVtx[26].pos = D3DXVECTOR3(-box_WIDTH, -box_HIGHT, box_DEPTH);
		//pVtx[27].pos = D3DXVECTOR3(-box_WIDTH, box_HIGHT, -box_DEPTH);
		//pVtx[28].pos = D3DXVECTOR3(-box_WIDTH, -box_HIGHT, -box_DEPTH);
		//pVtx[29].pos = D3DXVECTOR3(-box_WIDTH, -box_HIGHT, box_DEPTH);
		//pVtx[30].pos = D3DXVECTOR3(box_WIDTH, -box_HIGHT, box_DEPTH);//後ろ
		//pVtx[31].pos = D3DXVECTOR3(box_WIDTH, box_HIGHT, box_DEPTH);
		//pVtx[32].pos = D3DXVECTOR3(-box_WIDTH, -box_HIGHT, box_DEPTH);
		//pVtx[33].pos = D3DXVECTOR3(box_WIDTH, box_HIGHT, box_DEPTH);
		//pVtx[34].pos = D3DXVECTOR3(-box_WIDTH, box_HIGHT, box_DEPTH);
		//pVtx[35].pos = D3DXVECTOR3(-box_WIDTH, -box_HIGHT, box_DEPTH);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
		pVtx[4].nor = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
		pVtx[5].nor = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		pVtx[6].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[7].nor = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
		/*pVtx[8].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[9].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[10].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[11].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[12].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		pVtx[13].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		pVtx[14].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		pVtx[15].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		pVtx[16].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		pVtx[17].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

		pVtx[18].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[19].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[20].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[21].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[22].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[23].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[24].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		pVtx[25].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		pVtx[26].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		pVtx[27].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		pVtx[28].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		pVtx[29].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

		pVtx[30].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[31].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[32].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[33].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[34].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[35].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);*/

		//頂点カラーの設定
		pVtx[0].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[4].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[5].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[6].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[7].col_cir = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		/*pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[16].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[17].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[18].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[19].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[20].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[21].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[22].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[23].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[24].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[25].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[26].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[27].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[28].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[29].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[30].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[31].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[32].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[33].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[34].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[35].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);*/


		//UV値設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
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
		g_pVtxBuffbox->Unlock();
	}
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD)*g_NumIndexbox,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffbox,
		NULL))) {
		return E_FAIL;
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		g_pIdxBuffbox->Lock(0, 0, (void**)&pIdx, 0);

		pIdx[0] = 0;//前
		pIdx[1] = 1;
		pIdx[2] = 2;
		pIdx[3] = 1;
		pIdx[4] = 3;
		pIdx[5] = 2;

		pIdx[6] = 1;//右
		pIdx[7] = 6;
		pIdx[8] = 3;
		pIdx[9] = 6;
		pIdx[10] = 4;
		pIdx[11] = 3;

		pIdx[12] = 7;//上
		pIdx[13] = 6;
		pIdx[14] = 0;
		pIdx[15] = 6;
		pIdx[16] = 1;
		pIdx[17] = 0;

		pIdx[18] = 7;//左
		pIdx[19] = 0;
		pIdx[20] = 5;
		pIdx[21] = 0;
		pIdx[22] = 2;
		pIdx[23] = 5;

		pIdx[24] = 4;//後ろ
		pIdx[25] = 6;
		pIdx[26] = 5;
		pIdx[27] = 6;
		pIdx[28] = 7;
		pIdx[29] = 5;

		pIdx[30] = 3;//下
		pIdx[31] = 4;
		pIdx[32] = 2;
		pIdx[33] = 4;
		pIdx[34] = 5;
		pIdx[35] = 2;


		g_pIdxBuffbox->Unlock();
	}
	return S_OK;

}

