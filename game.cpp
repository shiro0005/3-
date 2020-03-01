#include "camera.h"
#include "light.h"
#include "field.h"
#include "grid.h"
//#include "debugproc.h"
#include "player.h"
#include "billboard.h"
#include "shadow.h"
#include "bullet.h"
#include "box.h"
#include "wallmap.h"
#include "wallB.h"
#include "wallL.h"
#include "wallR.h"
#include "WALLW.h"
#include "enemy.h"
#include "fade.h"
#include "bust.h"
#include "collision.h"
#include "scene.h"
#include "result.h"
#include <time.h>


typedef enum PHASE_INDEX
{
	PHASE_INDEX_FADE,
	PHASE_INDEX_PLAYER_IN,
	PHASE_INDEX_PLAYER_MUTEKI,
	PHASE_INDEX_PLAYER_NORMAL,
	PHASE_INDEX_STAGE_CLEAR,
	PHASE_INDEX_END,

	PHASE_INDEX_MAX
};

static PHASE_INDEX g_Phase;
static int g_GameFrameCount;

static int g_Score = 0;// 点数
static int g_KillCount = 0;

static bool Use = false;

void Game_Initialize(void)
{
	// カメラの初期化処理
	Camera_Initialize();

	// ライトの初期化処理
	Light_Initialize();

	// 地面の初期化処理
	Field_Initialize();

	//wall
	Wall_Initialize();
	WallL_Initialize();
	WallB_Initialize();
	WallR_Initialize();
	WALLW_Initialize();
	//影の初期化
	Shadow_Initialize();//使用する処理より先に初期化しないとダメ

	//グリッドの初期化
//	Grid_Initialize();

	//billboard初期化
	Billboard_Initialize();

	//プレイヤーの初期化
	Player_Initialize(D3DXVECTOR3(500, 100, -600), D3DXVECTOR3(0, 0, 0));

	//敵の初期化
	Enemy_Initialize();

	//弾丸の初期化
	Bullet_Initialize();

	Bust_Initialize();

	//BGMの開始
	//PlaySound(SOUND_LABEL_BGM001);

	g_Phase = PHASE_INDEX_FADE;
	g_GameFrameCount = 0;
	g_Score = 0;
	g_KillCount = 0;
	Fade_Start(false, 30, D3DCOLOR_RGBA(0,0,0,0));
}

void Game_Finalize(void)
{
	//BGMを停止
	//StopSound(SOUND_LABEL_BGM001);
	// カメラの終了処理
	Camera_Finalize();

	// ライトの終了処理
	Light_Finalize();

	// 地面の終了処理
	Field_Finalize();

	Wall_Finalize();
	WallB_Finalize();
	WallR_Finalize();
	WallL_Finalize();
	WALLW_Finalize();

	//box
	//box_Finalize();

	//グリッドの終了処理
//	Grid_Finalize();

	//終了処理
	Billboard_Finalize();

	//影の終了処理
	Shadow_Finalize();

	//敵の終了処理
	Enemy_Finalize();

	//プレイヤーの終了処理
	Player_Finalize();

	//弾丸の終了処理
	Bullet_Finalize();

	Bust_Finalize();
}

void Game_Update(void)
{
	g_Score++;

	// カメラの更新処理
	Camera_Update();

	// ライトの更新処理
	Light_Update();

	// 地面の更新処理
	Field_Update();

	//box
	//box_Update();

	//グリッドの更新処理
//	Grid_Update();

	//
	Shadow_Update();

	//更新処理
	Billboard_Update();

	//プレイヤーの更新処理
	Player_Update();

	//敵の更新処理
	Enemy_Update();

	//弾丸の更新処理
	Bullet_Update();

	//
	Bust_Update();

	//当たり判定
	Collision_Update();

	if (GetDel() == 5) {
		if (!Use) {
			Fade_Start(true, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
			Use = true;
		}
		if (!Fade_IsFade()) {
			Givetime(g_Score);
			Scene_Change(SCENE_INDEX_RESULT);

		}
	}
	//switch(g_Phase)
	//{
	//case PHASE_INDEX_FADE:
	//	if( !Fade_IsFade() ) {
	//		g_Phase = PHASE_INDEX_PLAYER_NORMAL;
	//		// PlaySound(SOUND_LABEL_BGM000);
	//	}
	//	break;
	//case PHASE_INDEX_PLAYER_IN:
	//case PHASE_INDEX_PLAYER_MUTEKI:
	//case PHASE_INDEX_PLAYER_NORMAL:
	//	Player_Update();
	//	Enemy_Update();
	//	Bullet_Update();
	//	EnemyBullet_Update();
	//	EnemyLaser_Update();
	//	Explosion_Update();

	//	//当たり判定は必ずすべてのアップデート処理が終わった後に行う
	//	Collision_Update();

	//	//ゲームの終了チェック
	//	if (Game_EndCheck())
	//	{
	//		Fade_Start(true, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
	//		g_Phase = PHASE_INDEX_STAGE_CLEAR;
	//	}
	//	break;
	//case PHASE_INDEX_STAGE_CLEAR:
	//	if (!Fade_IsFade()) {
	//		Scene_Change(SCENE_INDEX_RESULT);
	//		g_Phase = PHASE_INDEX_END;
	//	}
	//	break;
	//case PHASE_INDEX_END:
	//	break;
	//}

}

void Game_Draw(void)
{
	// カメラの設定
	Camera_SetCamera();

	// 地面の描画処理
	Field_Draw();


	//壁の描画

	Wall_Draw();
	WallL_Draw();
	WallB_Draw();
	WallR_Draw();
	WALLW_Draw();

	//グリッドの描画処理
	//Grid_Draw();

	//
	Shadow_Draw();

	//描画処理
	Billboard_Draw();

	//プレイヤーの描画処理
	Player_Draw();

	//敵の描画処理
	Enemy_Draw();

	//弾丸の描画処理
	Bullet_Draw();

	Bust_Draw();
}


