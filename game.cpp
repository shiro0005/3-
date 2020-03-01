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

static int g_Score = 0;// �_��
static int g_KillCount = 0;

static bool Use = false;

void Game_Initialize(void)
{
	// �J�����̏���������
	Camera_Initialize();

	// ���C�g�̏���������
	Light_Initialize();

	// �n�ʂ̏���������
	Field_Initialize();

	//wall
	Wall_Initialize();
	WallL_Initialize();
	WallB_Initialize();
	WallR_Initialize();
	WALLW_Initialize();
	//�e�̏�����
	Shadow_Initialize();//�g�p���鏈������ɏ��������Ȃ��ƃ_��

	//�O���b�h�̏�����
//	Grid_Initialize();

	//billboard������
	Billboard_Initialize();

	//�v���C���[�̏�����
	Player_Initialize(D3DXVECTOR3(500, 100, -600), D3DXVECTOR3(0, 0, 0));

	//�G�̏�����
	Enemy_Initialize();

	//�e�ۂ̏�����
	Bullet_Initialize();

	Bust_Initialize();

	//BGM�̊J�n
	//PlaySound(SOUND_LABEL_BGM001);

	g_Phase = PHASE_INDEX_FADE;
	g_GameFrameCount = 0;
	g_Score = 0;
	g_KillCount = 0;
	Fade_Start(false, 30, D3DCOLOR_RGBA(0,0,0,0));
}

void Game_Finalize(void)
{
	//BGM���~
	//StopSound(SOUND_LABEL_BGM001);
	// �J�����̏I������
	Camera_Finalize();

	// ���C�g�̏I������
	Light_Finalize();

	// �n�ʂ̏I������
	Field_Finalize();

	Wall_Finalize();
	WallB_Finalize();
	WallR_Finalize();
	WallL_Finalize();
	WALLW_Finalize();

	//box
	//box_Finalize();

	//�O���b�h�̏I������
//	Grid_Finalize();

	//�I������
	Billboard_Finalize();

	//�e�̏I������
	Shadow_Finalize();

	//�G�̏I������
	Enemy_Finalize();

	//�v���C���[�̏I������
	Player_Finalize();

	//�e�ۂ̏I������
	Bullet_Finalize();

	Bust_Finalize();
}

void Game_Update(void)
{
	g_Score++;

	// �J�����̍X�V����
	Camera_Update();

	// ���C�g�̍X�V����
	Light_Update();

	// �n�ʂ̍X�V����
	Field_Update();

	//box
	//box_Update();

	//�O���b�h�̍X�V����
//	Grid_Update();

	//
	Shadow_Update();

	//�X�V����
	Billboard_Update();

	//�v���C���[�̍X�V����
	Player_Update();

	//�G�̍X�V����
	Enemy_Update();

	//�e�ۂ̍X�V����
	Bullet_Update();

	//
	Bust_Update();

	//�����蔻��
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

	//	//�����蔻��͕K�����ׂẴA�b�v�f�[�g�������I�������ɍs��
	//	Collision_Update();

	//	//�Q�[���̏I���`�F�b�N
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
	// �J�����̐ݒ�
	Camera_SetCamera();

	// �n�ʂ̕`�揈��
	Field_Draw();


	//�ǂ̕`��

	Wall_Draw();
	WallL_Draw();
	WallB_Draw();
	WallR_Draw();
	WALLW_Draw();

	//�O���b�h�̕`�揈��
	//Grid_Draw();

	//
	Shadow_Draw();

	//�`�揈��
	Billboard_Draw();

	//�v���C���[�̕`�揈��
	Player_Draw();

	//�G�̕`�揈��
	Enemy_Draw();

	//�e�ۂ̕`�揈��
	Bullet_Draw();

	Bust_Draw();
}


