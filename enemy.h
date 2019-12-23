//=============================================================================
//
// エネミー処理 [enemy.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY			(5)


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ENEMY
{
	D3DXMATRIX			mtxWorldModel;	// ワールドマトリックス
	D3DXVECTOR3			posModel;			// 現在の位置
	D3DXVECTOR3			rotModel;			// 現在の向き
	D3DXVECTOR3			rotDestModel;		// 目的の向き
	D3DXVECTOR3			moveModel;		// 移動量
	int					idxShadow;

	CIRCLE				col_circle;		// 当たり判定用サークル
	AABB                col_aabb;        //当たり判定ボックス
	bool				bUse;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Enemy_Initialize(void);
void Enemy_Finalize(void);
void Enemy_Update(void);
void Enemy_Draw(void);
bool Enemy_IsEnable(int index);

CIRCLE* Enemy_GetCollision(int index);
ENEMY* GetEnemy(void);
