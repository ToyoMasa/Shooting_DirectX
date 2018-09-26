//======================================================================
//	指定した時間待機　（2018/07/18）
//	Author : 豊村 昌俊
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
#include "waitTime.h"
#include "sceneSkinMesh.h"
#include "EnemyAnim.h"

void CActionWaitTime::Init(int waitFrame, CActionBase* nextAction)
{
	m_WaitCount = waitFrame;
	m_NextAction = nextAction;
}

void CActionWaitTime::Update()
{
	if (m_WaitCount > 0)
	{
		m_WaitCount--;

		m_Character->GetModel()->ChangeAnim(ENEMY_IDLE, 0.3f);
	}
	else
	{
		CEnemy* enemy = (CEnemy*)m_Character;
		enemy->SetAction(m_NextAction);
	}
}

CActionWaitTime* CActionWaitTime::Create(CCharacter* chara, int waitFrame, CActionBase* nextAction)
{
	CActionWaitTime* wait = new CActionWaitTime(chara);
	wait->Init(waitFrame, nextAction);

	return wait;
}

