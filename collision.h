#pragma once

#include <d3dx9.h>


typedef struct
{
	float cx, cy, cz;	//’†SÀ•W
	float r;		//”¼Œa

}CIRCLE;


typedef struct {
	float cx, cy, cz;
	float sx, sy, sz;
}AABB;
void Collision_Initialize(void);
void Collision_Update(void);
