//======================================================================
//	�G�l�~�[�}�l�[�W���[[enemyManager.h]
//
//======================================================================
#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include "enemy.h"
#include "zombie.h"

//*********************************************************
//	�����ɑ��݂ł���]���r�̍ő吔
//*********************************************************
static const int ZOMBIE_NUM_MAX = 50;

//*********************************************************
//	Heat�l�̍ő�l
//*********************************************************
static const float MAX_HEAT = 100.0f;

//*********************************************************
//	�_���[�W���󂯂��ۂ�Heat�l�㏸��
//*********************************************************
static const float DAMAGED_TENSION = 15.0f;

class CField;
class CSpawnAIStateBase;

class CEnemyManager
{
public:
	CEnemyManager(CField* field);
	~CEnemyManager();

	void Update();
	void CreateEnemy(ENEMY_TYPE type);
	int GetFrameCount() { return m_CountFrame; }
	void AddHeat(float add) { if (m_isStart) { m_Heat += add; } }
	void SetHeat(float tension) {if (m_isStart) { m_Heat = tension; } }
	float GetHeat() { return m_Heat; }
	void ChangeSpawnAI(CSpawnAIStateBase* next);
	void DeleteZombie(CZombie* zombie);
	void SetSpawnAIState(int state) { m_SpawnAIState = state; }
	void Start();

private:
	CZombie*			m_Zombies[ZOMBIE_NUM_MAX];
	CField*				m_Field;
	CSpawnAIStateBase*	m_SpawnAI;
	float				m_Heat;
	int					m_CountFrame;
	int					m_ZombieSpawnRate;
	int					m_SpawnAIState;
	bool				m_isStart;
};


#endif // !_ENEMY_MANAGER_H_


