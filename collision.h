#pragma once

#include <d3dx9.h>


typedef struct
{
	float cx, cy, cz;	//���S���W
	float r;		//���a

}CIRCLE;


typedef struct {
	float cx, cy, cz;
	float sx, sy, sz;
}AABB;
void Collision_Initialize(void);
void Collision_Update(void);
