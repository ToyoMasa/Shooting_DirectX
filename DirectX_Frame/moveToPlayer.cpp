////======================================================================
////	プレイヤーに向かう　（2018/07/18）
////	Author : 豊村 昌俊
////======================================================================
//#include "common.h"
//#include "main.h"
//#include "manager.h"
//#include "camera.h"
//#include "scene.h"
//#include "scene3D.h"
//#include "sceneModel.h"
//#include "texture.h"
//#include "billboard.h"
//#include "character.h"
//#include "player.h"
//#include "enemy.h"
//#include "game.h"
//#include "input.h"
//#include "field.h"
//#include "moveToPlayer.h"
//#include "root.h"
//#include "sceneSkinMesh.h"
//#include "EnemyAnim.h"
//
//void CActionMoveToPlayer::Init(CCharacter* target, float speed)
//{
//	m_Target = target;
//	m_Speed = speed;
//}
//
//void CActionMoveToPlayer::Update()
//{
//	D3DXVECTOR3 newPos = m_Character->GetPos();
//	newPos.y = 0;
//	D3DXVECTOR3 targetPos = m_Target->GetPos();
//	targetPos.y = 0;
//	D3DXVECTOR3 vec = targetPos - newPos;
//
//	m_Character->GetModel()->ChangeAnim(ENEMY_RUNNING, 0.3f);
//
//	if (D3DXVec3Length(&vec) > m_Speed)
//	{
//		D3DXVec3Normalize(&vec, &vec);
//		newPos += m_Speed * vec;
//	}
//	else
//	{
//
//	}
//
//	// 壁との当たり判定
//	newPos = m_Character->HitWall(newPos);
//
//	D3DXVec3Normalize(&vec, &vec);
//	m_Character->Rotate(vec);
//	m_Character->SetPos(newPos);
//}
//
//CActionMoveToPlayer* CActionMoveToPlayer::Create(CCharacter* chara, CCharacter* target, float speed)
//{
//	CActionMoveToPlayer* moveToPos = new CActionMoveToPlayer(chara);
//	moveToPos->Init(target, speed);
//
//	return moveToPos;
//}