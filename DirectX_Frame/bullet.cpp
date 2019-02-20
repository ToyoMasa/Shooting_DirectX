//======================================================================
//	ìñÇΩÇËîªíËïtÇ´íeï`âÊÅ@Åi2018/05/23)
//	Author : ñLë∫ èπèr
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "scene3D.h"
#include "Effekseer.h"
#include "texture.h"
#include "billboard.h"
#include "character.h"
#include "enemy.h"
#include "collision.h"
#include "bullet.h"
#include "particle.h"
#include "debug.h"
#include "emitter.h"
#include <map>

CBullet *CBullet::m_Bullets[BULLET_MAX] = { NULL };

void CBullet::Init(D3DXVECTOR3 spawnPos, D3DXVECTOR3 vec, float speed, float range, float damage)
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

	if (m_Debug != NULL)
	{
		m_Debug->Release();
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

	// íeÇÃà⁄ìÆ
	m_Pos += m_Forward * m_Speed;
	m_Move += m_Speed;

	CParticle::Create(TEX_ID_CIRCLE, 2, 0.2f, m_Pos, D3DCOLOR_RGBA(255, 255, 0, 255));

	// ìñÇΩÇËîªíË
	Capsule capsule;
	Point old, now;
	old.x = m_OldPos.x;
	old.y = m_OldPos.y;
	old.z = m_OldPos.z;
	now.x = m_Pos.x;
	now.y = m_Pos.y;
	now.z = m_Pos.z;
	capsule.Set(old, now, BULLET_RADIUS);

	std::map<float, int> enemyDist;
	enemyDist.clear();

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
					float len = D3DXVec3Length(&(enemy->GetPos() - CManager::GetCamera()->GetPos()));
					enemyDist[len] = i;
				}
			}
		}
	}

	if (enemyDist.size() != 0)
	{
		auto it = enemyDist.begin();
		CEnemy* enemy = (CEnemy*)CCharacter::GetCharacter(it->second);

		D3DXVECTOR3 pos1, pos2;
		float r;
		enemy->GetCapsuleInfo(pos1, pos2, r);

		D3DXVECTOR3 ray;
		ray = m_Forward;
		D3DXVec3Normalize(&ray, &ray);

		D3DXVECTOR3 getPoint1, getPoint2;
		calcRayCapsule(m_OldPos, ray, pos1, pos2, r, getPoint1, getPoint2);

		//CParticle::Create(TEX_ID_CIRCLE, 60, 0.5f, getPoint1);
		CParticleEmitter::Create(TEX_ID_CIRCLE,
			7,
			1,
			10,
			0.5f,
			-0.05f,
			getPoint1,
			D3DXVECTOR3(-0.1f, 0.1f, -0.1f),
			D3DXVECTOR3(0.1f, 0.2f, 0.1f),
			D3DXVECTOR3(0.0f, -0.035f, 0.0f),
			false,
			D3DCOLOR_RGBA(255, 16, 16, 255));

		if (getPoint1.y > 1.25f)
		{
			enemy->Damaged(m_Damage * HEADSHOT_BONUS);
		}
		else
		{
			enemy->Damaged(m_Damage);
		}
	}
	//if (enemyDist.size() < 3)
	//{
	//	for (auto it = enemyDist.begin(); it != enemyDist.end(); ++it)
	//	{
	//		CEnemy* enemy = (CEnemy*)CCharacter::GetCharacter(it->second);

	//		D3DXVECTOR3 pos1, pos2;
	//		float r;
	//		enemy->GetCapsuleInfo(pos1, pos2, r);

	//		D3DXVECTOR3 ray;
	//		ray = m_Forward;
	//		D3DXVec3Normalize(&ray, &ray);

	//		D3DXVECTOR3 getPoint1, getPoint2;
	//		calcRayCapsule(m_OldPos, ray, pos1, pos2, r, getPoint1, getPoint2);

	//		//CParticle::Create(TEX_ID_CIRCLE, 60, 0.5f, getPoint1);
	//		CParticleEmitter::Create(TEX_ID_CIRCLE,
	//			7,
	//			1,
	//			10,
	//			0.5f,
	//			-0.05f,
	//			getPoint1,
	//			D3DXVECTOR3(-0.1f, 0.1f, -0.1f),
	//			D3DXVECTOR3(0.1f, 0.2f, 0.1f),
	//			D3DXVECTOR3(0.0f, -0.035f, 0.0f),
	//			false,
	//			D3DCOLOR_RGBA(255, 16, 16, 255));

	//		if (getPoint1.y > 1.25f)
	//		{
	//			enemy->Damaged(m_Damage * HEADSHOT_BONUS);
	//		}
	//		else
	//		{
	//			enemy->Damaged(m_Damage);
	//		}
	//	}
	//}
	//else
	//{
	//	for (int i = 0; i < 3; i++)
	//	{
	//		auto it = enemyDist.begin();
	//		for (int j = 0; j < 0; j++)
	//		{
	//			it++;
	//		}
	//		CEnemy* enemy = (CEnemy*)CCharacter::GetCharacter(it->second);

	//		D3DXVECTOR3 pos1, pos2;
	//		float r;
	//		enemy->GetCapsuleInfo(pos1, pos2, r);

	//		D3DXVECTOR3 ray;
	//		ray = m_Forward;
	//		D3DXVec3Normalize(&ray, &ray);

	//		D3DXVECTOR3 getPoint1, getPoint2;
	//		calcRayCapsule(m_OldPos, ray, pos1, pos2, r, getPoint1, getPoint2);

	//		//CParticle::Create(TEX_ID_CIRCLE, 60, 0.5f, getPoint1);
	//		CParticleEmitter::Create(TEX_ID_CIRCLE,
	//			7,
	//			1,
	//			10,
	//			0.5f,
	//			-0.05f,
	//			getPoint1,
	//			D3DXVECTOR3(-0.1f, 0.1f, -0.1f),
	//			D3DXVECTOR3(0.1f, 0.2f, 0.1f),
	//			D3DXVECTOR3(0.0f, -0.035f, 0.0f),
	//			false,
	//			D3DCOLOR_RGBA(255, 16, 16, 255));

	//		if (getPoint1.y > 1.25f)
	//		{
	//			enemy->Damaged(m_Damage * HEADSHOT_BONUS);
	//		}
	//		else
	//		{
	//			enemy->Damaged(m_Damage);
	//		}
	//	}
	//}
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

CBullet* CBullet::Create(D3DXVECTOR3 spawnPos, D3DXVECTOR3 vec, float speed, float range, float damage)
{
	CBullet* bullet = new CBullet();
	bullet->Init(spawnPos, vec, speed, range, damage);

	return bullet;
}
