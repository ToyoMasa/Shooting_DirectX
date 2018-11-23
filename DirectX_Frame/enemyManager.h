//======================================================================
//	エネミーマネージャー[enemyManager.h]
//
//======================================================================
#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include "enemy.h"
#include "zombie.h"

static const int ZOMBIE_NUM_MAX = 100;

class CField;

class CEnemyManager
{
public:
	CEnemyManager(CField* field);
	~CEnemyManager();

	void Update();
	void CreateEnemy(ENEMY_TYPE type);

private:
	CZombie* m_Zombies[ZOMBIE_NUM_MAX];
	CField* m_Field;
	int		m_CountFrame;
	int		m_ZombieSpawnRate;
};


#endif // !_ENEMY_MANAGER_H_


