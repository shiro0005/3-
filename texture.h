#pragma once

#include <d3d9.h>

typedef enum 
{
    TEXTURE_INDEX_FIELD01,
	TEXTURE_INDEX_FIELD02,
	TEXTURE_INDEX_FIELD03,

    TEXTURE_INDEX_MAX
}TextureIndex;

int Texture_Load(void); //‰Šú‰»
void Texture_Release(void); //I—¹ˆ—
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index);
int Texture_GetWidth(TextureIndex index);
int Texture_GetHeight(TextureIndex index);
