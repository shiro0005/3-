#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "shadow.h"
#include <d3dx9.h>


static bool HitCircle(const CIRCLE* pCircle_a, const CIRCLE* pCircle_b);
void Collision_Bullet_vs_Enemy(void);


bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
	//引数からベクトル型の変数を作る
	D3DXVECTOR3 dst1(p_circle_a->cx, p_circle_a->cy, p_circle_a->cz);
	D3DXVECTOR3 dst2(p_circle_b->cx, p_circle_b->cy, p_circle_b->cz);
	//二点間のベクトルを作る
	D3DXVECTOR3 distance = dst2 - dst1;
	//作ったベクトルの長さを求める
	float length = D3DXVec3Length(&distance);
	//お互いの半径を足した値を求める
	float size = p_circle_a->r + p_circle_b->r;

	//ベクトルの長さとお互いの半径を足した値を比較する
	//→ベクトルの長さの方が小さければヒットしている！
	if (length < size) {
		return true;
	}
	//ヒットしていなかった
	return false;
}

bool HitAABB(const AABB *p_bb_a, const AABB *p_bb_b) {
	D3DXVECTOR3 minA, maxA, minB, maxB;

	minA.x = p_bb_a->cx - p_bb_a->sx;
	minA.y = p_bb_a->cy - p_bb_a->sy;
	minA.z = p_bb_a->cz - p_bb_a->sz;
	maxA.x = p_bb_a->cx + p_bb_a->sx;
	maxA.y = p_bb_a->cy + p_bb_a->sy;
	maxA.z = p_bb_a->cz + p_bb_a->sz;

	minB.x = p_bb_b->cx - p_bb_b->sx;
	minB.y = p_bb_b->cy - p_bb_b->sy;
	minB.z = p_bb_b->cz - p_bb_b->sz;
	maxB.x = p_bb_b->cx + p_bb_b->sx;
	maxB.y = p_bb_b->cy + p_bb_b->sy;
	maxB.z = p_bb_b->cz + p_bb_b->sz;

	if (maxA.x > minB.x && minA.x < maxB.x) {//Xの判定
		if (maxA.y > minB.y && minA.y < maxB.y) {//Yの判定
			if (maxA.z > minB.z && minA.z < maxB.z) {//Zの判定
				return TRUE;
			}
		}
	}
	return FALSE;
}


/*
bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
//引数からベクトル型の変数を作る
D3DXVECTOR3 dst1(p_circle_a->cx, p_circle_a->cy);
D3DXVECTOR3 dst2(p_circle_b->cx, p_circle_b->cy);
//二点間のベクトルを作る
D3DXVECTOR3 distance = dst2 - dst1;
//作ったベクトルの長さを求める
float length = D3DXVec3LengthSq(&distance);
//お互いの半径を足した値を求める
float size = (p_circle_a->r + p_circle_b->r) * (p_circle_a->r + p_circle_b->r);

//ベクトルの長さとお互いの半径を足した値を比較する
//→ベクトルの長さの方が小さければヒットしている！
if (length < (p_circle_a->r + p_circle_b->r)){
return true;
}
//ヒットしていなかった
return false;
}
*/

void Collision_Initialize(void)
{
}

void Collision_Update(void)
{
	Collision_Bullet_vs_Enemy();
}

void Collision_Bullet_vs_Enemy(void)
{
	BULLET* pBullet = GetBullet();
	ENEMY* pEnemy = GetEnemy();

	for (int i = 0; i < MAX_BULLET; i++)
	{
		// 弾は有効か？
		if (Bullet_IsEnable(i) == false) {
			continue;
		}

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			// エネミーは有効か？
			if (Enemy_IsEnable(j) == false) {
				continue;
			}

			// 弾のコリジョンとエネミーのコリジョン
			if (HitCircle(Bullet_GetCollision(i), Enemy_GetCollision(j)))
			{
				// 当たってる

				// 敵の消滅処理
				/*Shadow_Release(pEnemy[j].idxShadow);
				pEnemy[j].bUse = false;*/
				pEnemy[j].posModel += pBullet[i].g_moveBullet*50;

				// 弾の消滅処理
				Shadow_Release(pBullet[i].g_nIdxShadowBullet);
				pBullet[i].bUse = false;

				// この弾の処理は終了
				break;
			}
		}
	}
}
