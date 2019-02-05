//======================================================================
//	Å‰‚Ì“G¶¬ƒCƒxƒ“ƒg@i2018/11/20)
//	Author : –L‘º ¹r
//======================================================================
#include "common.h"
#include "main.h"
#include "eventStartEnemy.h"
#include "manager.h"
#include "game.h"
#include "camera.h"
#include "enemyManager.h"

void CEventStartEnemy::Init()
{
	m_Pos = D3DXVECTOR3(-39.0f, 0.0f, -81.0f);
}

void CEventStartEnemy::Uninit()
{

}

void CEventStartEnemy::Update()
{
	D3DXVECTOR3 player = CManager::GetCamera()->GetPos();
	if (isCollisionSquareToPoint2D(
		D3DXVECTOR2(m_Pos.x - 5.0f, m_Pos.z + 0.5f),
		D3DXVECTOR2(m_Pos.x + 5.0f, m_Pos.z - 0.5f),
		D3DXVECTOR2(player.x, player.z)))
	{
		CModeGame::GetEnemyManager()->Start();
		Release();
	}
}

CEventStartEnemy* CEventStartEnemy::Create()
{
	CEventStartEnemy* startEnemy = new CEventStartEnemy();
	startEnemy->Init();

	return startEnemy;
}
