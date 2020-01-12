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
	//��������x�N�g���^�̕ϐ������
	D3DXVECTOR3 dst1(p_circle_a->cx, p_circle_a->cy, p_circle_a->cz);
	D3DXVECTOR3 dst2(p_circle_b->cx, p_circle_b->cy, p_circle_b->cz);
	//��_�Ԃ̃x�N�g�������
	D3DXVECTOR3 distance = dst2 - dst1;
	//������x�N�g���̒��������߂�
	float length = D3DXVec3Length(&distance);
	//���݂��̔��a�𑫂����l�����߂�
	float size = p_circle_a->r + p_circle_b->r;

	//�x�N�g���̒����Ƃ��݂��̔��a�𑫂����l���r����
	//���x�N�g���̒����̕�����������΃q�b�g���Ă���I
	if (length < size) {
		return true;
	}
	//�q�b�g���Ă��Ȃ�����
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

	if (maxA.x > minB.x && minA.x < maxB.x) {//X�̔���
		if (maxA.y > minB.y && minA.y < maxB.y) {//Y�̔���
			if (maxA.z > minB.z && minA.z < maxB.z) {//Z�̔���
				return TRUE;
			}
		}
	}
	return FALSE;
}


/*
bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
//��������x�N�g���^�̕ϐ������
D3DXVECTOR3 dst1(p_circle_a->cx, p_circle_a->cy);
D3DXVECTOR3 dst2(p_circle_b->cx, p_circle_b->cy);
//��_�Ԃ̃x�N�g�������
D3DXVECTOR3 distance = dst2 - dst1;
//������x�N�g���̒��������߂�
float length = D3DXVec3LengthSq(&distance);
//���݂��̔��a�𑫂����l�����߂�
float size = (p_circle_a->r + p_circle_b->r) * (p_circle_a->r + p_circle_b->r);

//�x�N�g���̒����Ƃ��݂��̔��a�𑫂����l���r����
//���x�N�g���̒����̕�����������΃q�b�g���Ă���I
if (length < (p_circle_a->r + p_circle_b->r)){
return true;
}
//�q�b�g���Ă��Ȃ�����
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
		// �e�͗L�����H
		if (Bullet_IsEnable(i) == false) {
			continue;
		}

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			// �G�l�~�[�͗L�����H
			if (Enemy_IsEnable(j) == false) {
				continue;
			}

			// �e�̃R���W�����ƃG�l�~�[�̃R���W����
			if (HitCircle(Bullet_GetCollision(i), Enemy_GetCollision(j)))
			{
				// �������Ă�

				// �G�̏��ŏ���
				/*Shadow_Release(pEnemy[j].idxShadow);
				pEnemy[j].bUse = false;*/
				pEnemy[j].posModel += pBullet[i].g_moveBullet*50;

				// �e�̏��ŏ���
				Shadow_Release(pBullet[i].g_nIdxShadowBullet);
				pBullet[i].bUse = false;

				// ���̒e�̏����͏I��
				break;
			}
		}
	}
}
