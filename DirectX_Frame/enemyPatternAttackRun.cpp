//======================================================================
//	“GUŒ‚(‘–‚è)ƒpƒ^[ƒ“ [enemyPatternAttack.cpp]@i2018/2/5j
//	Author : –L‘º ¹r
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "sceneShadow.h"
#include "texture.h"
#include "billboard.h"
#include "character.h"
#include "player.h"
#include "input.h"
#include "field.h"
#include "enemy.h"
#include "game.h"
#include "wall.h"
#include "bullet.h"
#include "EnemyAnim.h"
#include "enemyPatternChaseRun.h"
#include "enemyPatternAttackRun.h"

static float CHASE_SPEED = 0.05f;

void CEnemyPatternAttackRun::Init(CEnemy* enemy)
{
	enemy->GetModel()->ChangeAnim(ENEMY_ATTACK, 0.3f);
	enemy->GetModel()->SetAnimPlaySpeed(1.2f);
	enemy->SetAttackHit(false);
	enemy->Attack();
	enemy->SetPatternType(ENEMY_PATTERN_ATTACK_RUN);
}

void CEnemyPatternAttackRun::Update(CEnemy* enemy)
{
	if (enemy->GetModel()->GetWeightTime() > 1.0f && enemy->GetModel()->GetWeightTime() < 3.0f)
	{
		CPlayer* player = CModeGame::GetPlayer();
		if (isCollisionCapsule(enemy->GetAttackCollision(), player->GetCapsule()) && !enemy->GetAttackHit())
		{
			player->Damaged(enemy->GetAttackDamage());
			enemy->SetAttackHit(true);
		}
	}

	if (enemy->GetModel()->GetWeightTime() >= 3.0f)
	{
		enemy->ChangePattern(new CEnemyPatternChaseRun());
	}
}

