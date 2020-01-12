//=============================================================================
//
// ビルボード処理 [billboard.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BILLBOARD		"asset/TEXTURE/exp.png"	// 読み込むテクスチャファイル名
#define	BILLBOARD_SIZE_X		(20.0f)							// ビルボードの幅
#define	BILLBOARD_SIZE_Y		(20.0f)							// ビルボードの高さ
#define	VALUE_MOVE_BILLBOARD	(0.30f)							// 移動速度
#define	VALUE_JUMP				(10.0f)							// ジャンプ力
#define	VALUE_GRAVITY			(0.45f)							// 重力
#define	RATE_REGIST				(0.075f)						// 抵抗係数
#define	RATE_REFRECT			(-0.90f)						// 反射係数

#define BILLBOARD_NUM_VERTEX	4
#define BILLBOARD_NUM_POLYGON	2

#define MAX_BILLBOARD	1

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	D3DXMATRIX				g_mtxWorldBillboard;		// ワールドマトリックス
	D3DXVECTOR3				g_posBillboard;				// 位置
	D3DXVECTOR3				g_sclBillboard;				// スケール
	D3DXVECTOR3				g_moveBillboard;			// 移動量
	int						g_nIdxShadowBillboard;		// 影ID
	bool					g_bEnableGravity = false;	// 重力を有効にするかどうか

	//CIRCLE					col_circle;					// 当たり判定用スフィア
	AABB					col_aabb;					// 当たり判定用ボックス

	int						g_BillBoardFrame = 0;
	float					g_tx = 0;
	float					g_ty = 0;
	bool						bUse;
}BILLBOARD;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Billboard_Initialize(void);
void Billboard_Finalize(void);
void Billboard_Update(void);
void Billboard_Draw(void);
HRESULT MakeVertexBillboard();
void SetVertexBillboard(float posX, float posY, float posZ);
bool BillBoard_IsEnable(int index);
AABB* BillBoard_GetCollisionBox(int index);
BILLBOARD* GetBillBoard(void);

