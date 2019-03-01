//======================================================================
//	エネミー　（2018/06/12）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "sceneSkinMesh.h"
#include "texture.h"
#include "billboard.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "result.h"
#include "input.h"
#include "field.h"
#include "EnemyAnim.h"
#include "effekseer.h"
#include "enemyPatternAttack.h"
#include "enemyPatternAttackRun.h"
#include "enemyPatternChase.h"
#include "enemyPatternChaseRun.h"
#include "enemyPatternWaypoints.h"
#include "enemyPatternWaypointsRun.h"
#include "enemyPatternDeath.h"
#include "skinmeshShader.h"

CEnemy::PATTERN_STATE CEnemy::m_PatternState = PATTERN_STATE_NORMAL;

void CEnemy::ChangePattern(CEnemyPatternBase* next)
{
	if (m_Pattern != NULL)
	{
		delete m_Pattern;
	}

	m_Pattern = next;
	next->Init(this);
}

void CEnemy::StartHorde()
{
	m_PatternState = PATTERN_STATE_HORDE;

	for (int i = 0; i < CHARACTER_MAX; i++)
	{
		CCharacter* obj = CCharacter::GetCharacter(i);
		if (obj == NULL)
		{
			continue;
		}

		if (obj->GetType() != CHARACTER_ENEMY)
		{
			continue;
		}

		CEnemy* enemy = (CEnemy*)obj;
		
		switch (enemy->GetPatternType())
		{
		case CEnemyPatternBase::ENEMY_PATTERN_ATTACK:
			enemy->ChangePattern(new CEnemyPatternAttackRun());
			break;
		case CEnemyPatternBase::ENEMY_PATTERN_CHASE:
			enemy->ChangePattern(new CEnemyPatternChaseRun());
			break;
		case CEnemyPatternBase::ENEMY_PATTERN_WAYPOINTS:
			enemy->ChangePattern(new CEnemyPatternWaypointsRun());
			break;
		default:
			break;
		}
	}
}

void CEnemy::StopHorde()
{
	m_PatternState = PATTERN_STATE_NORMAL;
}
