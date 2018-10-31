//======================================================================
//	�G�U���p�^�[�� [enemyPatternAttack.cpp]�@�i2018/10/31�j
//	Author : �L�� ���r
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
#include "enemyPatternChase.h"
#include "enemyPatternAttack.h"

static float CHASE_SPEED = 0.05f;

void CEnemyPatternAttack::Init(CEnemy* enemy)
{
	//enemy->GetModel()->PlayMontage(ENEMY_ATTACK, 0.2f, 4.0f, ENEMY_RUNNING, 1.5f);
	enemy->GetModel()->ChangeAnim(ENEMY_ATTACK, 0.3f);
	enemy->GetModel()->SetAnimPlaySpeed(1.5f);
}

void CEnemyPatternAttack::Update(CEnemy* enemy)
{
	if (enemy->GetModel()->GetWeightTime() > 2.0f && enemy->GetModel()->GetWeightTime() < 3.0f)
	{	
		CPlayer* player = CModeGame::GetPlayer();
		if (isCollisionCapsule(enemy->GetAttackCollision(), player->GetCapsule()))
		{
		}
	}

	if (enemy->GetModel()->GetWeightTime() >= 3.0f)
	{
		enemy->ChangePattern(new CEnemyPatternChase());
	}
}

