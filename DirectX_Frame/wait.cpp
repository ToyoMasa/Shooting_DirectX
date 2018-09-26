//======================================================================
//	‚»‚Ìê‘Ò‹@@i2018/07/18j
//	Author : –L‘º ¹r
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "texture.h"
#include "billboard.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "input.h"
#include "field.h"
#include "wait.h"
#include "sceneSkinMesh.h"
#include "EnemyAnim.h"

void CActionWait::Init()
{

}

void CActionWait::Update()
{
	m_Character->GetModel()->ChangeAnim(ENEMY_IDLE, 0.3f);
}

CActionWait* CActionWait::Create(CCharacter* chara)
{
	CActionWait* wait = new CActionWait(chara);
	wait->Init();

	return wait;
}

