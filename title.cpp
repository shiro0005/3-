#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include <d3d9.h>

static bool g_bEnd = false;


void Title_Initialize(void)
{
	////BGMÇçƒê∂
	//PlaySound(SOUND_LABEL_BGM000);
	g_bEnd = false;
}

void Title_Finalize(void)
{
	////BGMÇí‚é~
	//StopSound(SOUND_LABEL_BGM000);
}

void Title_Update(void)
{
	if (!g_bEnd) {
		if (Keyboard_IsTrigger(DIK_RETURN)|| GamePad_IsTrigger(0, BUTTON_C)) {
			Fade_Start(true, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_bEnd = true;
		}
	}
	else {
		if (!Fade_IsFade()) {
			Scene_Change(SCENE_INDEX_GAME);
		}
	}
}

void Title_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_TITLE, 0.0f, 0.0f);
}
