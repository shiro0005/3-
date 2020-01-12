//=============================================================================
//
// プレイヤー処理 [player.h]
//
//=============================================================================
#pragma once

#include "main.h"

struct PLAYER
{
	D3DXMATRIX			mtxWorldModel;	// ワールドマトリックス
	D3DXVECTOR3			posModel;			// 現在の位置
	D3DXVECTOR3			rotModel;			// 現在の向き
	D3DXVECTOR3			rotDestModel;		// 目的の向き
	D3DXVECTOR3			moveModel;		// 移動量

	int					idxShadow;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Player_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
PLAYER GetPlayer(void);
