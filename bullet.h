//=============================================================================
//
// 弾丸処理 [bullet.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"

#define MAX_BULLET           100

typedef struct {
	D3DXMATRIX				g_mtxWorldBullet;		// ワールドマトリックス
	D3DXVECTOR3				g_posBullet;			// 位置
	D3DXVECTOR3				g_sclBullet;			// スケール
	D3DXVECTOR3				g_moveBullet;			// 移動量
	int						g_nIdxShadowBullet;		// 影ID
	int						frame;					//フレーム数
	CIRCLE                  col_cir;                    //当たり判定サークル
	AABB                    col_aabb;               //当たり判定ボックス
	bool                    bUse;					//使用フラグ
	float                   g_speed;                //speed
}BULLET;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Bullet_Initialize(void);
void Bullet_Finalize(void);
void Bullet_Update(void);
void Bullet_Draw(void);
void Bullet_Create(float, float, float, D3DXVECTOR2);
bool Bullet_IsEnable(int index);
CIRCLE *Bullet_GetCollision(int index);
AABB *Bullet_GetCollisionBox(int index);
BULLET *GetBullet();
