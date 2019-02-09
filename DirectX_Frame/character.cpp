//======================================================================
//	プレイヤー　（2018/05/23）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "sceneShadow.h"
#include "texture.h"
#include "billboard.h"
#include "player.h"
#include "input.h"
#include "field.h"
#include "character.h"
#include "enemy.h"
#include "game.h"
#include "wall.h"
#include <array>
#include <algorithm>
using namespace std;

CCharacter *CCharacter::m_Characters[CHARACTER_MAX] = { NULL };

CCharacter::CCharacter()
{
	for (int i = 0; i < CHARACTER_MAX; i++)
	{
		if (m_Characters[i] == NULL)
		{
			m_Characters[i] = this;
			m_Type = CHARACTER_NONE;
			break;
		}
	}

	m_Model = NULL;
	m_Forward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_WalkSpeed = 0.02f;
	m_CameraDist = 999.0f;
	m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + 0.5f, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.0f, m_Pos.z), 0.5f);
	D3DXMatrixIdentity(&m_Rotate);
	m_AreaID = 0;
}

void CCharacter::UpdateAll()
{
	D3DXVECTOR3 camerapos = CManager::GetCamera()->GetPos();
	for (int i = 0; i < CHARACTER_MAX; i++)
	{
		if (m_Characters[i] != NULL)
		{
			m_Characters[i]->Update();
		}

		if (m_Characters[i] != NULL)
		{
			D3DXVECTOR3 dist = m_Characters[i]->m_Pos - camerapos;
			m_Characters[i]->m_CameraDist = D3DXVec3Length(&dist);
		}
	}

	array<float, CHARACTER_MAX> enemyDist;
	enemyDist.fill(DRAW_DIST);
	D3DXVECTOR3 camFront = CManager::GetCamera()->GetFront();
	camFront.y = 0;
	D3DXVec3Normalize(&camFront, &camFront);
	int num = 0;

	for (int i = 0; i < CHARACTER_MAX; i++)
	{
		if (m_Characters[i] == NULL)
		{
			continue;
		}
		if(m_Characters[i]->m_Type != CHARACTER_ENEMY)
		{
			continue;
		}
		if (m_Characters[i]->m_CameraDist > DRAW_DIST)
		{
			continue;
		}

		D3DXVECTOR3 vec = m_Characters[i]->m_Pos - camerapos;
		vec.y = 0;

		D3DXVec3Normalize(&vec, &vec);

		float dot = D3DXVec3Dot(&vec, &camFront);
		float rad = acosf(dot);

		float degree = D3DXToDegree(rad);

		if (degree > 90.0f)
		{
			continue;
		}

		enemyDist[i] = m_Characters[i]->m_CameraDist;
		num++;
	}

	sort(enemyDist.begin(), enemyDist.end());
	CManager::SetSkinMeshDrawDist(enemyDist[ZOMBIE_DRAW_NUM]);

	if (CManager::GetDebug())
	{
		ImGui::Begin("Debug");
		ImGui::Text("DRAW_DIST:%.2f", CManager::GetSkinMeshDrawDist());
		ImGui::Text("DRAW_NUM:%d", num);
		ImGui::End();
	}
}

void CCharacter::Release()
{
	for (int i = 0; i < CHARACTER_MAX; i++)
	{
		if (m_Characters[i] == this)
		{
			m_Characters[i]->m_Model->Release();
			m_Characters[i]->Uninit();
			delete this;
			m_Characters[i] = NULL;

			break;
		}
	}
}

void CCharacter::ReleaseAll()
{
	for (int i = 0; i < CHARACTER_MAX; i++)
	{
		if (m_Characters[i] != NULL)
		{
			m_Characters[i]->Release();
		}
	}
}

void CCharacter::SetRotation(D3DXVECTOR3 vec)
{
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxIdentity;	// 比較用
	D3DXVECTOR3 v1 = m_Forward;
	D3DXVECTOR3 v2 = { vec.x, 0, vec.z };

	v1.y = 0.0f;

	D3DXVec3Normalize(&v1, &v1);
	D3DXVec3Normalize(&v2, &v2);

	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxIdentity);

	// 今向いている方角と入力された方角の内積を取る
	float fInner = D3DXVec3Dot(&v2, &v1);

	if (fInner != 1)
	{
		float sita = fInner / (D3DXVec3Length(&v1) * D3DXVec3Length(&v2));
		float rad = acosf(fInner);

		D3DXMatrixRotationY(&mtxRot, rad);

		//mtxIdentity = m_Rotate;
		m_Rotate *= mtxRot;
		m_Model->Rotate(m_Rotate);

		D3DXVec3TransformNormal(&m_Forward, &m_Forward, &mtxRot);	// ベクトルの座標変換(出力, 入力, 変換行列)
		D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRot);	// ベクトルの座標変換(出力, 入力, 変換行列)
		D3DXVec3Normalize(&m_Forward, &m_Forward);

		D3DXVec3Normalize(&m_Right, &m_Right);
	}
}

void CCharacter::Rotate(D3DXVECTOR3 vec)
{
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxIdentity;	// 比較用
	D3DXVECTOR3 v1 = m_Forward;
	D3DXVECTOR3 v2 = { vec.x, 0, vec.z };

	v1.y = 0.0f;

	D3DXVec3Normalize(&v1, &v1);
	D3DXVec3Normalize(&v2, &v2);

	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxIdentity);

	// 今向いている方角と入力された方角の内積を取る
	float fInner = D3DXVec3Dot(&v2, &v1);

	if (fInner != 1)
	{
		float sita = fInner / (D3DXVec3Length(&v1) * D3DXVec3Length(&v2));
		float rad = acosf(fInner);
		float rot = D3DXToDegree(rad);

		D3DXVECTOR3 cross;
		D3DXVec3Cross(&cross, &v2, &v1);

		if (cross.y < 0.0f)
		{
			if (rot > 7.0f)
			{
				// 回転行列(Y軸回転)を作る(回転速度)
				D3DXMatrixRotationY(&mtxRot, D3DXToRadian(7.0f));
			}
			else
			{
				if (!isnan(rot))
				{
					D3DXMatrixRotationY(&mtxRot, D3DXToRadian(rot));
				}
			}
		}
		else
		{
			if (rot > 7.0f)
			{
				// 回転行列(Y軸回転)を作る(回転速度)
				D3DXMatrixRotationY(&mtxRot, -D3DXToRadian(7.0f));
			}
			else
			{
				if (!isnan(rot))
				{
					D3DXMatrixRotationY(&mtxRot, -D3DXToRadian(rot));
				}
			}
		}

		//mtxIdentity = m_Rotate;
		m_Rotate *= mtxRot;
		m_Model->Rotate(m_Rotate);

		D3DXVec3TransformNormal(&m_Forward, &m_Forward, &mtxRot);	// ベクトルの座標変換(出力, 入力, 変換行列)
		D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRot);	// ベクトルの座標変換(出力, 入力, 変換行列)
		D3DXVec3Normalize(&m_Forward, &m_Forward);

		D3DXVec3Normalize(&m_Right, &m_Right);
	}
}

//======================================================================
//	現在のエリアを検索
//======================================================================
void CCharacter::SearchArea(D3DXVECTOR3 pos)
{
	int numVertex = m_Field->GetSizeX();

	float blockLen = numVertex / (float)SEPARATE_NUM;

	int x = 0, y = 0;
	int sizeX = m_Field->GetSizeX();
	int sizeY = m_Field->GetSizeY();

	for (int j = 0; j < SEPARATE_NUM; j++)
	{
		if (pos.x < -sizeX / 2.0f + blockLen * (j + 1))
		{
			x = j;
			break;
		}
	}
	for (int j = 0; j < SEPARATE_NUM; j++)
	{
		if (pos.z < -sizeY / 2.0f + blockLen * (j + 1))
		{
			y = j;
			break;
		}
	}
	m_AreaID = x * 1000 + y;
}

//======================================================================
//	壁にめり込んだプレイヤーを押し出す
//======================================================================
D3DXVECTOR3 CCharacter::PushOut(D3DXVECTOR3 newPos, D3DXVECTOR3 oldPos, D3DXVECTOR3 normal)
{
	D3DXVECTOR3 moveVec = newPos - oldPos;

	D3DXVECTOR3 newVec;

	D3DXVec3Cross(&newVec, &moveVec, &normal);
	D3DXVec3Cross(&newVec, &normal, &newVec);

	newVec += normal * 0.002f;

	return oldPos + newVec;
}

void CCharacter::Damaged(float damage)
{
	m_Life -= damage;

	if (m_Life <= 0)
	{
		m_Life = 0;
	}
}
