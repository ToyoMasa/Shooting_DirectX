//======================================================================
//	エネミー[enemy.h]
//
//======================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "character.h"
#include "actionBase.h"
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
		m_Type = CHARACTER_ENEMY;
		m_Exclamation = NULL;
		m_FindPlayer = false;
		m_Action = NULL;
		m_isPreDeath = false;
		m_isPreAttack = false;
		m_isPlaySE = false;
		m_StabSE = CSound::Create(SOUND_LABEL_SE_STAB);
		m_AttackSE = CSound::Create(SOUND_LABEL_SE_ENEMY_ATTACK);
		m_FindSE = CSound::Create(SOUND_LABEL_SE_FIND);

		m_Count = 0;
	}
	~CEnemy() {}

	void Init(int modelId, D3DXVECTOR3 spawnPos, int rootId, CField* field);
	void Init(int modelId, D3DXVECTOR3 spawnPos, int rootId, CField* field, ENEMY_TYPE type);
	void Init(int modelId, D3DXVECTOR3 spawnPos, CActionBase* action, CField* field, ENEMY_TYPE type);
	void Uninit();
	void Update();
	void SetField(CField* field) { m_Field = field; }
	void SetAction(CActionBase* action);
	void ReleaseAction() { delete m_Action; }
	static CEnemy* Create(int modelId, D3DXVECTOR3 spawnPos, int rootId, CField* field);
	static CEnemy* Create(int modelId, D3DXVECTOR3 spawnPos, int rootId, CField* field, ENEMY_TYPE type);
	static CEnemy* Create(int modelId, D3DXVECTOR3 spawnPos, CActionBase* action, CField* field, ENEMY_TYPE type);
	Capsule GetCapsule() { return m_CapsuleCollision; }
	ENEMY_TYPE GetEnemyType() { return m_EnemyType; }
	void Search();
	void Death();
	void Attack();

private:
	CActionBase* m_Action;
	CBillBoard* m_Exclamation;
	ENEMY_TYPE m_EnemyType;
	Capsule m_AttackingCollsion;
	CSound* m_AttackSE;
	CSound* m_FindSE;
	CSound* m_StabSE;
	bool m_FindPlayer;
	bool m_isPreDeath;
	bool m_isPreAttack;
	bool m_isPlaySE;
	int m_Count;
};

#endif // !_ENEMY_H_
