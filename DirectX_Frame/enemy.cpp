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

static const float ENEMY_LIFE = 100000.0f;

void CEnemy::Init(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, int rootId, CField* field)
{
	m_Model = CSceneSkinMesh::Create(modelId);
	m_Pos = spawnPos;
	m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + ENEMY_CUPSULE_RAD, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.5f, m_Pos.z), ENEMY_CUPSULE_RAD);
	m_Model->Move(m_Pos);
	m_Field = field;
	m_EnemyType = ENEMY_TYPE_PATROL;
	m_Action = CActionMoveToPos::Create(this, rootId, 0.02f);

	m_Count = 0;

	m_Forward = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ライフを設定
	m_Life = ENEMY_LIFE;
}

void CEnemy::Init(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, int rootId, CField* field, ENEMY_TYPE type)
{
	m_Model = CSceneSkinMesh::Create(modelId);
	m_Pos = spawnPos;
	m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + ENEMY_CUPSULE_RAD, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.5f, m_Pos.z), ENEMY_CUPSULE_RAD);
	m_Model->Move(m_Pos);
	m_Field = field;
	m_EnemyType = type;
	m_Action = CActionMoveToPos::Create(this, rootId, 0.02f);

	m_Forward = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ライフを設定
	m_Life = ENEMY_LIFE;
}

void CEnemy::Init(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, CActionBase* action, CField* field, ENEMY_TYPE type)
{
	m_Model = CSceneSkinMesh::Create(modelId);
	m_Pos = spawnPos;
	m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + ENEMY_CUPSULE_RAD, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.5f, m_Pos.z), ENEMY_CUPSULE_RAD);
	m_Model->Move(m_Pos);
	m_Field = field;
	m_Action = action;
	m_EnemyType = type;

	m_Forward = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ライフを設定
	m_Life = ENEMY_LIFE;
}

void CEnemy::Uninit()
{
	if (m_Model)
	{
		m_Model->Release();
		m_Model = NULL;
	}
	if (m_Exclamation != NULL)
	{
		m_Exclamation->Release();
	}
}

void CEnemy::Update()
{
	m_OldPos = m_Pos;

	if (m_isPreDeath)
	{
		if (m_Model->GetWeightTime() >= 8.0f)
		{
			if (m_EnemyType == ENEMY_TYPE_TARGET)
			{
				CModeGame::TargetKilled();
			}
			Release();
			return;
		}
	}
	else
	{
		if (m_isPreAttack)
		{
			if (m_Model->GetWeightTime() >= 1.0f && !m_isPlaySE)
			{
				m_isPlaySE = true;
				m_AttackSE->Play();
			}

			if (m_Model->GetWeightTime() >= 2.0f)
			{
				CPlayer* player = CModeGame::GetPlayer();
				if (isCollisionCapsule(m_AttackingCollsion, player->GetCapsule()))
				{
					CModeGame::GetPlayer()->Death();
					SetAction(CActionWait::Create(this));
				}

				m_isPreAttack = false;
				m_isPlaySE = false;
			}
		}
		else
		{
			m_Action->Update();

			Search();

			m_Pos.y = m_Field->GetHeight(m_Pos);

			// コリジョンの計算
			m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + ENEMY_CUPSULE_RAD, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.50f, m_Pos.z), ENEMY_CUPSULE_RAD);
			for (int i = 0; i < CHARACTER_MAX; i++)
			{
				CCharacter* obj = CCharacter::GetCharacter(i);
				if (obj != NULL && obj != this)
				{
					if (obj->GetType() == CHARACTER_ENEMY)
					{
						CEnemy* enemy = (CEnemy*)obj;
						if (isCollisionCapsule(m_CapsuleCollision, enemy->GetCapsule()))
						{
							D3DXVECTOR3 vec = m_Pos - enemy->GetPos();
							D3DXVec3Normalize(&vec, &vec);

							m_Pos = enemy->GetPos();
							m_Pos += vec * (ENEMY_CUPSULE_RAD * 2 + 0.05f);
						}
					}
				}
			}
			m_Model->Move(m_Pos);
			m_Shadow->Move(m_Pos);

			// 当たり判定の移動
			m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + ENEMY_CUPSULE_RAD, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.50f, m_Pos.z), ENEMY_CUPSULE_RAD);
			D3DXVECTOR3 attackPos = m_Pos + m_Forward * 1.0f;
			m_AttackingCollsion.Set(Point(attackPos.x, attackPos.y + 0.25f, attackPos.z), Point(attackPos.x, attackPos.y + 1.65f, attackPos.z), PLAYER_CUPSULE_RAD);
		}
	}

	if (m_FindPlayer)
	{
		m_Count++;
		if (m_Count < 120)
		{
			D3DXVECTOR3 markPos = m_Pos;
			markPos.y += 2.5f;
			
			m_Exclamation->Set(TEX_ID_EXCLAMATION, markPos, 1.0f, NORMAL);
		}
		else if (m_Count = 120)
		{
			m_Exclamation->Release();
			m_Exclamation = NULL;
		}
	}
}

CEnemy* CEnemy::Create(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, int rootId, CField* field)
{
	CEnemy* enemy = new CEnemy();
	enemy->Init(modelId, spawnPos, rootId, field);

	return enemy;
}

CEnemy* CEnemy::Create(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, int rootId, CField* field, ENEMY_TYPE type)
{
	CEnemy* enemy = new CEnemy();
	enemy->Init(modelId, spawnPos, rootId, field, type);

	return enemy;
}

CEnemy* CEnemy::Create(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, CActionBase* action, CField* field, ENEMY_TYPE type)
{
	CEnemy* enemy = new CEnemy();
	enemy->Init(modelId, spawnPos, action, field, type);

	return enemy;
}

void CEnemy::GetCapsule(D3DXVECTOR3& pos1, D3DXVECTOR3& pos2, float& r)
{
	pos1 = D3DXVECTOR3(m_Pos.x, m_Pos.y + ENEMY_CUPSULE_RAD, m_Pos.z);
	pos2 = D3DXVECTOR3(m_Pos.x, m_Pos.y + 1.50f, m_Pos.z);
	r = ENEMY_CUPSULE_RAD;
}

void CEnemy::Search()
{
	if (!m_FindPlayer)
	{
		D3DXVECTOR3 vec = CModeGame::GetPlayer()->GetPos() - m_Pos;

		// 敵とプレイヤーの距離
		float len = D3DXVec3Length(&vec);

		D3DXVec3Normalize(&vec, &vec);

		float dot = D3DXVec3Dot(&vec, &m_Forward);
		float rad = acosf(dot);

		float degree = D3DXToDegree(rad);

		if (degree < 60.0f && len < 12.0f)
		{
			m_FindPlayer = true;
			m_FindSE->Play();

			m_Exclamation = CBillBoard::Create(TEX_ID_EXCLAMATION);
			D3DXVECTOR3 markPos = m_Pos;
			markPos.y += 2.5f;
			m_Exclamation->Set(TEX_ID_EXCLAMATION, markPos, 1.0f, NORMAL);

			SetAction(CActionMoveToPlayer::Create(this, CModeGame::GetPlayer(), 0.05f));
		}
	}
	else
	{
		D3DXVECTOR3 vec = CModeGame::GetPlayer()->GetPos() - m_Pos;

		// 敵とプレイヤーの距離
		float len = D3DXVec3Length(&vec);

		bool playerdeath = CModeGame::GetPlayer()->GetPlayerDeath();
		if (len < 1.25f && !playerdeath && !m_isPreAttack)
		{
			Attack();
		}
	}
}

void CEnemy::SetAction(CActionBase* action)
{
	if (m_Action != NULL)
	{
		m_Action->Release();
	}
	m_Action = action;
}

void CEnemy::Death()
{
	m_Model->PlayMontage(ENEMY_DEATH, 0.3f, 8.0f, ENEMY_DEATH);
	m_Model->SetAnimPlaySpeed(1.5f);
	m_StabSE->Play();

	m_isPreDeath = true;
}

void CEnemy::Attack()
{
	m_Model->PlayMontage(ENEMY_ATTACK, 0.2f, 2.5f, ENEMY_IDLE, 1.5f);

	m_isPreAttack = true;
}
