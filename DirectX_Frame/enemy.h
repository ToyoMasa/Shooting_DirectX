//======================================================================
//	エネミー[enemy.h]
//
//======================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "character.h"
#include "enemyPatternBase.h"
#include "sound.h"

typedef enum
{
	ENEMY_TYPE_ZOMBIE = 0,
	ENEMY_TYPE_MAX,
} ENEMY_TYPE;

class CEnemy : public CCharacter
{
public:
	typedef enum
	{
		TARGET_FIND,
		TARGET_SEARCH,
	} TARGET_STATE;

	CEnemy::CEnemy() : CCharacter() 
	{
		m_Type = CHARACTER_ENEMY;
		m_TargetState = TARGET_SEARCH;
		m_Count = 0;
	}
	~CEnemy() {}

	virtual void Init(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, CEnemyPatternBase* pattern, CField* field) {}
	virtual void Uninit()override {}
	virtual void Update()override {}
	void SetField(CField* field) { m_Field = field; }
	Capsule GetCapsule() { return m_CapsuleCollision; }
	Sphere GetSphere() { return m_MoveCollision; }
	virtual void GetCapsuleInfo(D3DXVECTOR3& pos1, D3DXVECTOR3& pos2, float& r) {}
	Capsule GetAttackCollision() { return m_AttackingCollision; }

	float GetSpeed() { return m_Speed; }
	float GetCapsuleRad() { return m_CapsuleRad; }
	float GetAttackDamage() { return m_AttackDamage; }
	bool GetAttackHit() { return m_AttackHit; }
	int GetCount() { return m_Count; }
	void SetAttackHit(const bool& hit) { m_AttackHit = hit; }
	void SetSpeed(float speed) { m_Speed = speed; }
	void SetTargetState(TARGET_STATE state) { m_TargetState = state; }

	void ChangePattern(CEnemyPatternBase* next);
	virtual void Move(D3DXVECTOR3 newPos) {}
	virtual void Death() {}
	virtual void Attack() {}

	virtual void Damaged(float damage)override {}

protected:
	CEnemyPatternBase*		m_Pattern;
	Capsule					m_AttackingCollision;
	Sphere					m_MoveCollision;
	int						m_Count;
	TARGET_STATE			m_TargetState;
	float					m_AttackDamage;
	float					m_Speed;
	float					m_CapsuleRad;
	bool					m_AttackHit;
};

#endif // !_ENEMY_H_
