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
	D3DXVECTOR3			moveModelold;		// 移動量
	int					idxShadow;
};

typedef struct
{
	D3DXVECTOR2 vec;//スティックのｘｙ成分
	float angle;//スティックの角度（度）
	bool F[10];//スティック入力フラグ
	bool rote[2];//右回転ならrote[0]がtrue、左回転ならrote[1]がtrue
	int data[2];//Fのフラグのデータを格納する
	int cntdata;//前のフレームと同じ結果の時に加算
}STICK;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Player_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
PLAYER GetPlayer(void);
