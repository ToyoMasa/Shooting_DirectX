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
#include "sound.h"
#include "billboard.h"
#include "player.h"
#include "enemy.h"
#include "zombie.h"
#include "game.h"
#include "result.h"
#include "input.h"
#include "field.h"
#include "EnemyAnim.h"
#include "Effekseer.h"
#include "enemyPatternDeath.h"
#include "enemyPatternChase.h"
#include "skinmeshShader.h"
#include "skinmeshSpotlightShader.h"
#include "enemyManager.h"

static const float ZOMBIE_LIFE = 200.0f;
static const float ZOMBIE_ATTACK_DAMAGE = 23.0f;

CSound *CZombie::m_ZombieDeath = NULL;
CSound *CZombie::m_ZombieVoice[ZOMBIE_VOICE_NUM] = { NULL };

void CZombie::ZombieInit()
{
	m_ZombieDeath = CSound::Create(SOUND_LABEL_SE_ZOMBIE_DEATH);
	for (int i = 0; i < ZOMBIE_VOICE_NUM; i++)
	{
		if (m_ZombieVoice[i] == NULL)
		{
			m_ZombieVoice[i] = CSound::Create((SOUND_LABEL)(SOUND_LABEL_SE_ZOMBIEVOICE1 + i));
		}
	}
}

void CZombie::ZombieUninit()
{
	for (int i = 0; i < ZOMBIE_VOICE_NUM; i++)
	{
		if (m_ZombieVoice[i])
		{
			m_ZombieVoice[i]->Release();
		}
		m_ZombieVoice[i] = NULL;
	}
}

void CZombie::Init(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, CEnemyPatternBase* pattern, CField* field)
{
	m_Model = CSceneSkinMesh::Create(modelId);
	m_Model->SetShader(CShaderSkinmeshSpotlight::GetShader());
	m_Pos = spawnPos;
	m_OldPos = m_Pos;
	m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + ZOMBIE_CUPSULE_RAD, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.5f, m_Pos.z), ZOMBIE_CUPSULE_RAD);
	m_Model->Move(m_Pos);
	m_Field = field;

	m_Forward = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	ChangePattern(pattern);

	// ステータスを設定
	m_Life = ZOMBIE_LIFE;
	m_AttackDamage = ZOMBIE_ATTACK_DAMAGE;
}

void CZombie::Uninit()
{
	CModeGame::GetEnemyManager()->DeleteZombie(this);

	if (m_Pattern != NULL)
	{
		delete m_Pattern;
		m_Pattern = NULL;
	}
}

void CZombie::Update()
{
	m_OldPos = m_Pos;
	m_Count++;

	m_Pattern->Update(this);

	if (CManager::GetDebug())
	{
		ImGui::Begin("Enemy");
		ImGui::Text("Pos  :X = %.2f Y = %.2f Z = %.2f", m_Pos.x, m_Pos.y, m_Pos.z);
		ImGui::End();
	}
}

CZombie* CZombie::Create(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos, CEnemyPatternBase* pattern, CField* field)
{
	CZombie* enemy = new CZombie();
	enemy->Init(modelId, spawnPos, pattern, field);
	CModeGame::AddEnemyCount();

	return enemy;
}

void CZombie::Move(D3DXVECTOR3 newPos)
{
	m_Pos = newPos;

	SearchArea(newPos);

	if (m_CameraDist < CManager::GetSkinMeshDrawDist())
	{
		D3DXVECTOR3 vec = newPos - CManager::GetCamera()->GetPos();

		D3DXVec3Normalize(&vec, &vec);

		D3DXVECTOR3 camFront = CManager::GetCamera()->GetFront();
		camFront.y = 0;

		D3DXVec3Normalize(&camFront, &camFront);

		float dot = D3DXVec3Dot(&vec, &camFront);
		float rad = acosf(dot);

		float degree = D3DXToDegree(rad);

		if (degree <= 90.0f)
		{
			m_Pos.y = m_Field->GetHeight(m_Pos, this);
		}
	}

	// コリジョンの計算
	m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + ZOMBIE_CUPSULE_RAD, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.50f, m_Pos.z), ZOMBIE_CUPSULE_RAD);
	m_MoveCollision.Set(m_Pos, ZOMBIE_CUPSULE_RAD);
	for (int i = 0; i < CHARACTER_MAX; i++)
	{
		CCharacter* obj = CCharacter::GetCharacter(i);
		if (obj == NULL || obj == this)
		{
			continue;
		}

		if (obj->GetType() != CHARACTER_ENEMY)
		{
			continue;
		}

		CEnemy* enemy = (CEnemy*)obj;
		D3DXVECTOR3 vec = m_Pos - enemy->GetPos();
		if (D3DXVec3Length(&vec) < ZOMBIE_CUPSULE_RAD * 2)
		{
			if (isCollisionSphere(m_MoveCollision, enemy->GetSphere()))
			{
				D3DXVec3Normalize(&vec, &vec);

				m_Pos = enemy->GetPos();
				m_Pos += vec * (ZOMBIE_CUPSULE_RAD * 2 + 0.05f);
			}
		}
	}
	m_Model->Move(m_Pos);
	m_Shadow->Move(m_Pos);

	// 当たり判定の移動
	m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + ZOMBIE_CUPSULE_RAD, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.50f, m_Pos.z), ZOMBIE_CUPSULE_RAD); 
	m_MoveCollision.Set(m_Pos, ZOMBIE_CUPSULE_RAD);
	D3DXVECTOR3 attackPos = m_Pos + m_Forward * 1.0f;
	m_AttackingCollision.Set(Point(attackPos.x, attackPos.y + 0.25f, attackPos.z), Point(attackPos.x, attackPos.y + 1.65f, attackPos.z), PLAYER_CUPSULE_RAD);
}

void CZombie::Death()
{
	ChangePattern(new CEnemyPatternDeath());
	int random = rand() % 4;
	if (random == 3)
	{
		m_ZombieDeath->Play(0.01f);
	}
	else
	{
		m_ZombieVoice[random]->Play(0.08f);
	}

	CModeGame::RemoveEnemyCount();
	CModeGame::IncrementKillCount();

	CModeGame::GetEnemyManager()->AddPlayerTension((100.0f - m_CameraDist) / 20.0f);
}

void CZombie::Attack()
{
	m_ZombieVoice[rand() % 3]->Play(0.08f);
}

void CZombie::Damaged(float damage)
{
	CCharacter::Damaged(damage);
	if (m_Life <= 0.0f)
	{
		Death();
	}

	if (m_TargetState == CEnemy::TARGET_SEARCH)
	{
		ChangePattern(new CEnemyPatternChase());
	}
}

void CZombie::GetCapsuleInfo(D3DXVECTOR3& pos1, D3DXVECTOR3& pos2, float& r)
{
	pos1 = D3DXVECTOR3(m_Pos.x, m_Pos.y + ZOMBIE_CUPSULE_RAD, m_Pos.z);
	pos2 = D3DXVECTOR3(m_Pos.x, m_Pos.y + 1.50f, m_Pos.z);
	r = ZOMBIE_CUPSULE_RAD;
}
