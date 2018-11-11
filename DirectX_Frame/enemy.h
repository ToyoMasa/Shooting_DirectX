//======================================================================
//	エネミー[enemy.h]
//
//======================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "character.h"
#include "actionBase.h"
#include "enemyPatternBase.h"
#include "sound.h"

static const float ENEMY_MOVE_SPEED = 0.03f;
static const float ENEMY_CUPSULE_RAD = 0.4f;

typedef enum
{
	ENEMY_TYPE_PATROL = 0,
	ENEMY_TYPE_TARGET,
} ENEMY_TYPE;

class CEnemy : public CCharacter
{
public:
	CEnemy::CEnemy() : CCharacter() 
	{
		m_Speed = ENEMY_MOVE_SPEED;
		m_Type = CHARACTER_ENEMY;

		m_Count = 0;
	}
	~CEnemy() {}

	void Init(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, CEnemyPatternBase* pattern, CField* field);
	void Uninit();
	void Update();
	void SetField(CField* field) { m_Field = field; }
	static CEnemy* Create(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, CEnemyPatternBase* pattern, CField* field);
	Capsule& GetCapsule() { return m_CapsuleCollision; }
	void GetCapsule(D3DXVECTOR3& pos1, D3DXVECTOR3& pos2, float& r);
	Capsule& GetAttackCollision() { return m_AttackingCollsion; }

	float& GetSpeed() { return m_Speed; }
	void SetSpeed(float speed) { m_Speed = speed; }

	void ChangePattern(CEnemyPatternBase* next);
	void Move(D3DXVECTOR3 newPos);
	void Search();
	void Death();
	void Attack();

private:
	CEnemyPatternBase *m_Pattern;
	Capsule m_AttackingCollsion;
	int m_Count;
	float m_Speed;
};

#endif // !_ENEMY_H_
