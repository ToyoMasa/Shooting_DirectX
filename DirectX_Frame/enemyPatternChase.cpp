//======================================================================
//	“G’ÇÕƒpƒ^[ƒ“ [enemyPatternChase.cpp]@i2018/10/31j
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
#include "texture.h"
#include "billboard.h"
#include "character.h"
#include "player.h"
#include "input.h"
#include "field.h"
#include "enemy.h"
#include "game.h"
#include "bullet.h"
#include "EnemyAnim.h"
#include "enemyPatternChase.h"
#include "enemyPatternAttack.h"
#include "enemyPatternWaypoints.h"
#include "astar.h"
#include "waypoint.h"

void CEnemyPatternChase::Init(CEnemy* enemy)
{
	enemy->GetModel()->ChangeAnim(ENEMY_WALKING, 0.3f);
	enemy->GetModel()->SetAnimPlaySpeed(2.5f);
	enemy->SetTargetState(CEnemy::TARGET_FIND);
	enemy->SetPatternType(ENEMY_PATTERN_CHASE);
}

void CEnemyPatternChase::Update(CEnemy* enemy)
{
	D3DXVECTOR3 newPos = enemy->GetPos();
	newPos.y = 0;
	D3DXVECTOR3 targetPos = CModeGame::GetPlayer()->GetPos();
	targetPos.y = 0;
	D3DXVECTOR3 vec = targetPos - newPos;

	if (enemy->GetShortestPoint() != CModeGame::GetPlayer()->GetShortestPoint())
	{
		//if (CModeGame::GetField()->GetHeight(newPos, enemy) > 0.0f)
		{
			enemy->SetTargetState(CEnemy::TARGET_SEARCH);
			enemy->ChangePattern(new CEnemyPatternWaypoints());
		}
	}

	if (D3DXVec3Length(&vec) > 1.0f)
	{
		D3DXVec3Normalize(&vec, &vec);
		newPos += enemy->GetSpeed() * vec;
	}
	else
	{
		enemy->ChangePattern(new CEnemyPatternAttack());
	}

	D3DXVec3Normalize(&vec, &vec);
	enemy->Rotate(vec);
	enemy->Move(newPos);
}

