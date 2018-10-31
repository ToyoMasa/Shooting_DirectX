//======================================================================
//	“GŽ€–Sƒpƒ^[ƒ“ [enemyPatternDeath.cpp]@i2018/10/31j
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
#include "enemyPatternDeath.h"

static float CHASE_SPEED = 0.05f;

void CEnemyPatternDeath::Init(CEnemy* enemy)
{
	enemy->GetModel()->ChangeAnim(ENEMY_DEATH, 0.3f);
	enemy->SetType(CHARACTER_NONE);
}

void CEnemyPatternDeath::Update(CEnemy* enemy)
{
	if (enemy->GetModel()->GetWeightTime() > 8.0f)
	{
		enemy->Release();
	}
}

