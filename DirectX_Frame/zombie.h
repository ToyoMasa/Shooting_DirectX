//======================================================================
//	ƒ]ƒ“ƒr[zombie.h]
//
//======================================================================
#ifndef _ZOMBIE_H_
#define _ZOMBIE_H_

#include "enemy.h"

static const float ZOMBIE_MOVE_SPEED = 0.015f;
static const float ZOMBIE_CUPSULE_RAD = 0.4f;

class CZombie : public CEnemy
{
public:
	CZombie::CZombie() : CEnemy()
	{
		m_Speed = ZOMBIE_MOVE_SPEED;
		m_CapsuleRad = ZOMBIE_CUPSULE_RAD;
		m_Type = CHARACTER_ENEMY;

		m_Count = 0;
	}
	~CZombie() {}

	void Init(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, CEnemyPatternBase* pattern, CField* field)override;
	void Uninit()override;
	void Update()override;
	static CZombie* Create(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, CEnemyPatternBase* pattern, CField* field);

	void Move(D3DXVECTOR3 newPos);
	void Death()override;
	void Attack()override;

	void Damaged(float damage)override;
	void GetCapsuleInfo(D3DXVECTOR3& pos1, D3DXVECTOR3& pos2, float& r)override;
private:
};

#endif // !_ZOMBIE_H_
