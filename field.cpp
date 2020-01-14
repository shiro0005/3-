
#include "field.h"
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
#define BLOCK_Y 11                                       //縦の数
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffField = NULL;

#define NUM_BOX 1

typedef struct {
	D3DXMATRIX				g_mtxWorldField;		// ワールドマトリックス
	D3DXVECTOR3				g_posField;				// 地面の位置
	D3DXVECTOR3				g_rot1Field;				// 地面の向き(回転)
	D3DXVECTOR3				g_rot2Field;				// 地面の向き(回転)
	D3DXVECTOR3				g_sclField;				// 地面の大きさ(スケール)
}BOX_T;

BOX_T g_Field[NUM_BOX];

int g_NumVertex = (BLOCK_X + 1)*(BLOCK_Y + 1);                //頂点数
int g_NumIndex = ((BLOCK_X + 1)*BLOCK_Y + (BLOCK_Y - 1)) * 2; //インデックス数 ((BLOCK_X + 1)*2)*BLOCK_Y+(BLOCK_Y-1)*2
int g_NumPolygon = (BLOCK_X * 2)*BLOCK_Y + (BLOCK_Y - 1) * 4; //ポリゴン数
int zcnt[6] = {};//波生成用
float XX = 1.0f;
float sizemove = 0.01f;
float Xpos = 1;
float Zpos = 0;
float g_rot1_x = 0.01f;
float g_rot2_x = 0.01f;
int g_NumIndexField = 36;
int FrameCnt;
static bool flag = false;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Field_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexField(pDevice);

	//初期位置用
	zcnt[0] = 0;
	zcnt[1] = 21;
	zcnt[2] = 42;
	zcnt[3] = 63;
	zcnt[4] = 84;

	//カウント用
	zcnt[5] = 0;


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
void Field_Finalize(void)
{
	if (g_pIdxBuffField != NULL) {//インデックスバッファの開放
		g_pIdxBuffField->Release();
		g_pIdxBuffField = NULL;
	}

	if (g_pVtxBuffField != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}



//=============================================================================
// 更新処理
//=============================================================================
void Field_Update(void)
{
	FrameCnt++;

	if (FrameCnt >= 240) {
		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		//ワールドマトリックスの初期化
		for (int i = 0; i < NUM_BOX; i++) {
			D3DXMatrixIdentity(&g_Field[i].g_mtxWorldField);
		}

		VERTEX_3D *pVtx;


		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		if (!flag) {
			for (int z = 0; z < BLOCK_Y + 1; z++) {
				for (int x = 0; x < BLOCK_X + 1; x++) {
					//頂点座標の設定
					pVtx[z * (BLOCK_X + 1) + x].pos = D3DXVECTOR3(x*100.0f, rand() % 100, z*-100.0f);
				}
			}
			flag = true;
		}
		else {
			for (int z = 0; z < BLOCK_Y + 1; z++) {
				for (int x = 0; x < BLOCK_X + 1; x++) {
					//頂点座標の設定
					pVtx[z * (BLOCK_X + 1) + x].pos = D3DXVECTOR3(x*100.0f, 0.0f, z*-100.0f);
				}
			}
			flag = false;
		}
		FrameCnt = 0;
	}


	//	zcnt[5] ++;//カウント
	//	if (zcnt[5] >= 20) {
	//		zcnt[5] = 0;//初期化
	//	}

	//	//波生成
	//	for (int x = 0; x < BLOCK_X + 1; x++) {

	//		//初期化用
	//		for (int i = 0; i < 5; i++) {
	//			if (i == 4 && zcnt[5] >= 17) {
	//				break;
	//			}

	//			//前後ひとつ前のフレームで高くしていたところを低く
	//			if (zcnt[5] == 0) {
	//				pVtx[zcnt[i] + 20 * (BLOCK_X + 1) + x].pos = D3DXVECTOR3(x*100.0f, 0.0f, BLOCK_Y*-100.0f);
	//			}
	//			else {
	//				pVtx[((zcnt[i] + zcnt[5]) - 1) * (BLOCK_X + 1) + x].pos = D3DXVECTOR3(x*100.0f, 0.0f, ((zcnt[i] + zcnt[5]) - 1)*-100.0f);
	//			}


	//			////左右ひとつ前のフレームで高くしていたところを低く
	//			//if (zcnt[5] == 0) {
	//			//	pVtx[x * (BLOCK_X + 1) + zcnt[i] + 20].pos = D3DXVECTOR3((zcnt[i] + 20)*100.0f, 0.0f, x * -100.0f);
	//			//}
	//			//else {
	//			//	pVtx[x * (BLOCK_X + 1) + ((zcnt[i] + zcnt[5]) - 1)].pos = D3DXVECTOR3((zcnt[i] + zcnt[5] - 1) * 100.0f, 0.0f, x *-100.0f);
	//			//}
	//		}


	//		//前後の波
	//		for (int i = 0; i < 5; i++) {
	//			
	//			if (i == 4 && zcnt[5] >= 17) {
	//				break;//ここにブレイクがあるので二つに分けた
	//			}
	//			//横一列高くする
	//			pVtx[(zcnt[i] + zcnt[5]) * (BLOCK_X + 1) + x].pos = D3DXVECTOR3(x*100.0f, 80.0f, (zcnt[i] + zcnt[5]) * -100.0f);

	//			//横一列高くする
	//			//pVtx[(zcnt[i] + zcnt[5]+1) * (BLOCK_X + 1) + x].pos = D3DXVECTOR3(x*100.0f, 80.0f, (zcnt[i] + zcnt[5]) * -100.0f);

	//		}
 //           
	//		////左右の波
	//		//for (int i = 0; i < 5; i++) {
	//		//	
	//		//	if (i == 4 && zcnt[5] >= 17) {
	//		//		break;
	//		//	}
	//		//	//縦一列高く
	//		//	pVtx[x  * (BLOCK_X + 1) + zcnt[i] + zcnt[5]].pos = D3DXVECTOR3((zcnt[i] + zcnt[5]) *100.0f, 80.0f, x * -100.0f);
	//		//}
	//	}


	//	FrameCnt = 0;

	//	//頂点データをアンロックする
	//	g_pVtxBuffField->Unlock();
	//}




	////0番//

	//g_Box[0].g_rot1Field.y += 0.01f;
	////回転を反映
	////Yaw：Y軸回転　Pitch : X軸回転　Roll : Z軸回転  Pitchが９０度になりRollができなくなる：ジンバルロック
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[0].g_rot1Field.y, g_Box[0].g_rot1Field.x, g_Box[0].g_rot1Field.z);
	//D3DXMatrixMultiply(&g_Box[0].g_mtxWorldField, &g_Box[0].g_mtxWorldField, &mtxRot);

	//g_Box[0].g_posField.x = 100.0f;
	//g_Box[0].g_posField.y = 100.0f;
	//g_Box[0].g_posField.z = 100.0f;

	////移動を反映
	//D3DXMatrixTranslation(&mtxTranslate, g_Box[0].g_posField.x, g_Box[0].g_posField.y, g_Box[0].g_posField.z);
	//D3DXMatrixMultiply(&g_Box[0].g_mtxWorldField, &g_Box[0].g_mtxWorldField, &mtxTranslate);


	////1番//

	////g_Box[1].g_rot1Field.y -= 0.03f;
	//////回転を反映
	//////Yaw：Y軸回転　Pitch : X軸回転　Roll : Z軸回転  Pitchが９０度になりRollができなくなる：ジンバルロック
	////D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot1Field.y, g_Box[1].g_rot1Field.x, g_Box[1].g_rot1Field.z);
	////D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxRot);

	//g_Box[1].g_posField.x = 200.0f;
	//g_Box[1].g_posField.z = 200.0f;

	////移動を反映
	//D3DXMatrixTranslation(&mtxTranslate, g_Box[1].g_posField.x, g_Box[1].g_posField.y, g_Box[1].g_posField.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxTranslate);

	////g_Box[1].g_rot2Field.y += 0.03f;
	//////打消し回転を反映
	//////Yaw：Y軸回転　Pitch : X軸回転　Roll : Z軸回転  Pitchが９０度になりRollができなくなる：ジンバルロック
	////D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot2Field.y, g_Box[1].g_rot2Field.x, g_Box[1].g_rot2Field.z);
	////D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxRot);

	////子供＊親 1個目の引数に２と３の引数をかけて代入
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &g_Box[0].g_mtxWorldField);


	////D3DXVECTOR3 pos;

	////pos.x = 100.0f;
	////pos.y = 100.0f;
	////pos.z = 100.0f;

	//////移動を反映
	////D3DXMatrixTranslation(&mtxTranslate,pos.x,pos.y, pos.z);
	////D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxTranslate);

}

//=============================================================================
// 描画処理
//=============================================================================
void Field_Draw(void)
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
		pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

		pDevice->SetIndices(g_pIdxBuffField);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD02));

		//ポリゴンの描画(インデックスバッファ用）
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVertex, 0, g_NumPolygon);//一番最後がポリゴン数,右から2番目が頂点数

		////ポリゴンの描画
		//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVertex,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffField, NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;


		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);


		for (int z = 0; z < BLOCK_Y + 1; z++) {
			for (int x = 0; x < BLOCK_X + 1; x++) {
				//頂点座標の設定
				pVtx[z * (BLOCK_X + 1) + x].pos = D3DXVECTOR3(x*100.0f, 0.0f, z*-100.0f);

				//法線ベクトルの設定
				pVtx[z * (BLOCK_X + 1) + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
		g_pVtxBuffField->Unlock();
	}
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD)*g_NumIndex,//インデックス数は頂点座標の数より多くなる
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffField,
		NULL))) {
		return E_FAIL;
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		g_pIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

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
			if (cntY != BLOCK_Y - 1) {
				pIdx[((BLOCK_X + 1) * 2 * cntY + cntY * 2) + cntX * 2] = (BLOCK_X + 1) * cntY + cntX - 1;
				pIdx[((BLOCK_X + 1) * 2 * cntY + cntY * 2) + cntX * 2 + 1] = (BLOCK_X + 1) * cntY + (BLOCK_X + 1) + cntX;

			}
		}
		/*pIdx[0] = 5;        pIdx[1] = 0;
		pIdx[2] = 6;     	pIdx[3] = 1;
		pIdx[4] = 7;		pIdx[5] = 2;
		pIdx[6] = 8;		pIdx[7] = 3;
		pIdx[8] = 9;		pIdx[9] = 4;*/

		//pIdx[10] = 4;		pIdx[11] = 10;

	/*	pIdx[12] = 10;		pIdx[13] = 5;
		pIdx[14] = 11;		pIdx[15] = 6;
		pIdx[16] = 12;		pIdx[17] = 7;
		pIdx[18] = 13;		pIdx[19] = 8;
		pIdx[20] = 14;		pIdx[21] = 9;

		pIdx[22] = 9;		pIdx[23] = 15;

		pIdx[24] = 15;		pIdx[25] = 10;*/



		/*for (int i = 1; i < 102;i++) {
			pIdx[i + (i - 1)] = i - 1;
		}
		for (int j = 0; j < 102; j++) {
			for (int i = 0; i < 102; i++) {
				pIdx[102*j+i * 2] = 101 + 2 * i;
			}
		}*/
		g_pIdxBuffField->Unlock();
	}
	return S_OK;

}


float GetY(float px, float pz) {

	VERTEX_3D *pVtx;
	float py = 100;
	float diffX;
	float diffZ;
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	/*

	2平面のなす角を求めてプレイヤーに入れる


	*/

	for (int z = 0; z < BLOCK_Y + 1; z++) {
		for (int x = 0; x < BLOCK_X + 1; x++) {

			if ((px >= pVtx[z * (BLOCK_X + 1) + x].pos.x &&px <= pVtx[z * (BLOCK_X + 1) + x].pos.x + 100.0f)
				&& (pz <= pVtx[z * (BLOCK_X + 1) + x].pos.z&&pz >= pVtx[z * (BLOCK_X + 1) + x].pos.z - 100.0f)) {
				diffX = px - pVtx[z * (BLOCK_X + 1) + x].pos.x;
				diffZ = pz - pVtx[z * (BLOCK_X + 1) + x].pos.z;

				if (diffX + diffZ >= 0) {
					float a = (pVtx[z * (BLOCK_X + 1) + x].pos.y - pVtx[z * (BLOCK_X + 1) + x + 1].pos.y)
						*(pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.z - pVtx[z * (BLOCK_X + 1) + x + 1].pos.z)
						- (pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.y - pVtx[z * (BLOCK_X + 1) + x + 1].pos.y)
						*(pVtx[z * (BLOCK_X + 1) + x].pos.z - pVtx[z * (BLOCK_X + 1) + x + 1].pos.z);

					float b = (pVtx[z * (BLOCK_X + 1) + x].pos.z - pVtx[z * (BLOCK_X + 1) + x + 1].pos.z)
						*(pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.x - pVtx[z * (BLOCK_X + 1) + x + 1].pos.x)
						- (pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.z - pVtx[z * (BLOCK_X + 1) + x + 1].pos.z)
						*(pVtx[z * (BLOCK_X + 1) + x].pos.x - pVtx[z * (BLOCK_X + 1) + x + 1].pos.x);

					float c = (pVtx[z * (BLOCK_X + 1) + x].pos.x - pVtx[z * (BLOCK_X + 1) + x + 1].pos.x)
						*(pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.y - pVtx[z * (BLOCK_X + 1) + x + 1].pos.y)
						- (pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.x - pVtx[z * (BLOCK_X + 1) + x + 1].pos.x)
						*(pVtx[z * (BLOCK_X + 1) + x].pos.y - pVtx[z * (BLOCK_X + 1) + x + 1].pos.y);

					float d = -(a*pVtx[z * (BLOCK_X + 1) + x + 1].pos.x + b * pVtx[z * (BLOCK_X + 1) + x + 1].pos.y + c * pVtx[z * (BLOCK_X + 1) + x + 1].pos.z);

					py = (-a * px - c * pz - d) / b;
				}
				else {
					float a = (pVtx[z * (BLOCK_X + 1) + x].pos.y - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.y)
						*(pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.z - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.z)
						- (pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.y - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.y)
						*(pVtx[z * (BLOCK_X + 1) + x].pos.z - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.z);

					float b = (pVtx[z * (BLOCK_X + 1) + x].pos.z - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.z)
						*(pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.x - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.x)
						- (pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.z - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.z)
						*(pVtx[z * (BLOCK_X + 1) + x].pos.x - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.x);

					float c = (pVtx[z * (BLOCK_X + 1) + x].pos.x - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.x)
						*(pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.y - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.y)
						- (pVtx[(z + 1) * (BLOCK_X + 1) + x + 1].pos.x - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.x)
						*(pVtx[z * (BLOCK_X + 1) + x].pos.y - pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.y);

					float d = -(a*pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.x + b * pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.y + c * pVtx[(z + 1) * (BLOCK_X + 1) + x].pos.z);

					py = (-a * px - c * pz - d) / b;
				}
				//py = pVtx[z * (BLOCK_X + 1) + x].pos.y;
			}
		}
	}
	//頂点データをアンロックする
	g_pVtxBuffField->Unlock();
	return py;
}