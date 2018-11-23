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
#include "sceneShadow.h"
#include "texture.h"
#include "billboard.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "result.h"
#include "input.h"
#include "field.h"
#include "actionBase.h"
#include "action.h"
#include "EnemyAnim.h"
#include "Effekseer.h"
#include "enemyPatternDeath.h"
#include "skinmeshShader.h"

void CEnemy::ChangePattern(CEnemyPatternBase* next)
{
	if (m_Pattern != NULL)
	{
		delete m_Pattern;
	}

	m_Pattern = next;
	next->Init(this);
}
