//======================================================================
//	“–‚½‚è”»’è•t‚«’e•`‰æ@i2018/05/23)
//	Author : –L‘º ¹r
//======================================================================
#include "common.h"
#include "main.h"
#include "camera.h"
#include "scene3D.h"
#include "texture.h"
#include "billboard.h"
#include "character.h"
#include "enemy.h"
#include "collision.h"
#include "bullet.h"
#include "particle.h"

CBullet *CBullet::m_Bullets[BULLET_MAX] = { NULL };

void CBullet::Init(D3DXVECTOR3 spawnPos, D3DXVECTOR3 vec, float speed, float range, int damage)
{
	m_Pos = spawnPos;
	m_OldPos = m_Pos;
	m_Forward = vec;
	D3DXVec3Normalize(&m_Forward, &m_Forward);
	m_Speed = speed;
	m_Range = range;
	m_Damage = damage;
}

void CBullet ::Uninit()
{
	if (m_Billboard != NULL)
	{
		m_Billboard->Release();
	}
}

void CBullet::Update()
{
	if (m_Move > m_Range)
	{
		Release();
		return;
	}

	m_OldPos = m_Pos;

	// ’e‚ÌˆÚ“®
	m_Pos += m_Forward * m_Speed;
	m_Move += m_Speed;

	CParticle::Create(TEX_ID_CIRCLE, 2, 0.2f, m_Pos);

	// “–‚½‚è”»’è
	Capsule capsule;
	Point old, now;
	old.x = m_OldPos.x;
	old.y = m_OldPos.y;
	old.z = m_OldPos.z;
	now.x = m_Pos.x;
	now.y = m_Pos.y;
	now.z = m_Pos.z;
	capsule.Set(old, now, 0.05f);

	for (int i = 0; i < CHARACTER_MAX; i++)
	{
		CCharacter* obj = CCharacter::GetCharacter(i);
		if (obj != NULL)
		{
			if (obj->GetType() == CHARACTER_ENEMY)
			{
				CEnemy* enemy = (CEnemy*)obj;
				if (isCollisionCapsule(capsule, enemy->GetCapsule()))
				{
					CParticle::Create(TEX_ID_CIRCLE, 30, 2.0f, m_Pos);
				}
			}
		}
	}
}

void CBullet::Release()
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (m_Bullets[i] == this)
		{
			Uninit();
			m_Bullets[i] = NULL;
			delete this;
			break;
		}
	}
}

void CBullet::UpdateAll()
{
	int num = 0;

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (m_Bullets[i] != NULL)
		{
			m_Bullets[i]->Update();
			num++;
		}
	}

	ImGui::Begin("Bullet", 0);
	ImGui::Text("%d", num);
	ImGui::End();
}

void CBullet::ReleaseAll()
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (m_Bullets[i] != NULL)
		{
			m_Bullets[i]->Release();
			m_Bullets[i] = NULL;
		}
	}
}

CBullet* CBullet::Create(D3DXVECTOR3 spawnPos, D3DXVECTOR3 vec, float speed, float range, int damage)
{
	CBullet* bullet = new CBullet();
	bullet->Init(spawnPos, vec, speed, range, damage);

	return bullet;
}
