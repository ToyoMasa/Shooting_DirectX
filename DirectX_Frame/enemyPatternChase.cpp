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

void CEnemyPatternChase::Init(CEnemy* enemy)
{
	enemy->GetModel()->ChangeAnim(ENEMY_RUNNING, 0.3f);
	enemy->GetModel()->SetAnimPlaySpeed(2.0f);
}

void CEnemyPatternChase::Update(CEnemy* enemy)
{
	D3DXVECTOR3 newPos = enemy->GetPos();
	newPos.y = 0;
	D3DXVECTOR3 targetPos = CModeGame::GetPlayer()->GetPos();
	targetPos.y = 0;
	D3DXVECTOR3 vec = targetPos - newPos;

	if (D3DXVec3Length(&vec) > 1.0f)
	{
		D3DXVec3Normalize(&vec, &vec);
		newPos += enemy->GetSpeed() * 2.5f * vec;
	}
	else
	{
		enemy->ChangePattern(new CEnemyPatternAttack());
	}

	D3DXVec3Normalize(&vec, &vec);
	enemy->Rotate(vec);
	enemy->Move(newPos);
}

