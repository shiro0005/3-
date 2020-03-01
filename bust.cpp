#include "player.h"
#include "sprite.h"
#include "texture.h"
//#include "Hp_table.h"


#define HP_WIDTH	(700)
#define HP_HEIGHT	(100)


static PLAYER player;

D3DXVECTOR2 bustTable[10] =
{
	{//1
		D3DXVECTOR2(0.0f, 0.99f)
	},
	{//2
		D3DXVECTOR2(0.0f, 0.88f)
	},
	{//3
		D3DXVECTOR2(0.0f, 0.77f)
	},
	{//4
		D3DXVECTOR2(0.0f, 0.66f)
	},
	{//5
		D3DXVECTOR2(0.0f, 0.55f)
	},
	{//6
		D3DXVECTOR2(0.0f, 0.44f)
	},
	{//7
		D3DXVECTOR2(0.0f, 0.33f)
	},
	{//8
		D3DXVECTOR2(0.0f, 0.22f)
	},
	{//9
		D3DXVECTOR2(0.0f, 0.11f)
	},
	{//10
		D3DXVECTOR2(0.0f, 0.0f)
	},
};


void Bust_Initialize(void)
{
	player = GetPlayer();
}

void Bust_Finalize(void)
{

}

void Bust_Update(void)
{
	player = GetPlayer();
}

void Bust_Draw(void)
{

	Sprite_Draw(TEXTURE_INDEX_BUST,
		200.0f,
		60.0f,
		bustTable[9].x * 256,
		bustTable[9].y * 256,
		HP_WIDTH,
		HP_HEIGHT - (10 - 10)*(HP_HEIGHT / 10),
		HP_WIDTH / 2,
		(HP_HEIGHT - (10 - 10)*(HP_HEIGHT / 10)) / 2,
		0.3f,
		0.3f,
		0.0f,0xff00ff00	);
}