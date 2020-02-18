#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "score_draw.h"
#include "main.h"

static int time;
static int fig;
void Result_Initialize(void)
{ 
	time = GetTime();
	fig = Getfig();
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
}

void Result_Finalize(void)
{
}

void Result_Update(void)
{
	if( Keyboard_IsTrigger(DIK_RETURN)|| GamePad_IsTrigger(0, BUTTON_A)) {
		Scene_Change(SCENE_INDEX_TITLE);
	}
}

void Result_Draw(void)
{
	
	Sprite_Draw(TEXTURE_INDEX_RESULT, 0.0f, 0.0f);
	Score_Draw(500.0f, 200.0f, time, fig, true);
}


