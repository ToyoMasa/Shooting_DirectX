//======================================================================
//	プレイヤー　（2018/05/23）
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
#include "sceneShadow.h"
#include "texture.h"
#include "billboard.h"
#include "number.h"
#include "character.h"
#include "player.h"
#include "input.h"
#include "field.h"
#include "enemy.h"
#include "game.h"
#include "PlayerAnim.h"
#include "wall.h"
#include "bullet.h"
#include "weapon.h"
#include "rifle.h"
#include "shotgun.h"
#include "item.h"
#include "targetCapsule.h"
#include "playerPatternNormal.h"
#include "playerPatternJump.h"
#include "playerPatternReload.h"
#include "waypoint.h"
#include "skinmeshShader.h"
#include "skinmeshSpotlightShader.h"
#include "enemyManager.h"

static const float DEFAULT_FOV = 90.0f;
static const float ADS_FOV = 70.0f;
static const float LOCAL_CAMERA_X = 0.0f;
static const float LOCAL_CAMERA_Y = 1.715f;
static const float LOCAL_CAMERA_Z = 0.375f;
static const D3DXVECTOR3 AMMO_DISPLAY_POS = D3DXVECTOR3(SCREEN_WIDTH - 100.0f, SCREEN_HEIGHT - 60.0f, 0.0f);

void CPlayer::Init(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos)
{
	m_Model = CSceneSkinMesh::Create(modelId);
	m_Model->ChangeAnim(PLAYER_IDLE, 0.0f);
	m_Pos = spawnPos;
	D3DXVECTOR3 pos = m_Pos;
	m_LocalCameraPos.x = 0.0f;
	m_LocalCameraPos.y = 1.715f;
	m_LocalCameraPos.z = 0.375f;
	pos += m_LocalCameraPos;
	D3DXVECTOR3 at = pos;
	at.z += 1.0f;
	m_Model->Move(m_Pos);

	m_Camera = CCamera::Create(pos, at);
	m_Forward = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_Right = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	m_BloodEffect = CEffekseer::Create(CEffekseer::EFFECT_BLOODLOSS);
	m_BloodEffect->RepeatEffect(false);
	m_BloodEffect->SetScale(0.1f, 0.1f, 0.1f);
	m_BloodEffect->SetVisible(true);

	m_AmmoBackScreen = CScene2D::Create(TEX_ID_WHITE, 100.0f, 70.0f);
	m_AmmoBackScreen->Set(AMMO_DISPLAY_POS);
	m_AmmoBackScreen->SetColor(D3DCOLOR_RGBA(0, 0, 0, 128));
	m_DamagedEffect = CScene2D::Create(TEX_ID_DAMAGE_SCREEN, SCREEN_WIDTH, SCREEN_HEIGHT);
	m_DamagedEffect->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 5.0f, SCREEN_HEIGHT / 2.0f, 1.0f));
	m_DamagedEffect->SetColor(D3DCOLOR_RGBA(166, 19, 19, 0));

	// モデルの回転軸をカメラの位置にそろえる
	D3DXMatrixTranslation(&m_LocalLocation, m_LocalCameraPos.x, -m_LocalCameraPos.y, m_LocalCameraPos.z);
	m_Rotate = m_LocalLocation;
	Rotate(m_Camera->GetFront());

	// サウンドの設定
	m_SoundReload = CSound::Create(SOUND_LABEL_SE_RELOAD);

	// 武器を装備
	m_Weapon[0] = CRifle::Create(m_Model);
	m_Weapon[1] = CShotgun::Create(m_Model);

	m_UsingWeapon = m_Weapon[0];

	for (int i = 0; i < HAVE_WEAPON; i++)
	{
		if (m_Weapon[i] != m_UsingWeapon)
		{
			m_Weapon[i]->SetActive(false);
		}
		else
		{
			m_Weapon[i]->SetActive(true);
		}
	}

	// ステータスの設定
	m_Life = 100.0f;
	m_MaxLife = 100.0f;

	ChangePattern(new CPlayerPatternNormal());

	m_ShortestPoint = CWayPoint::SearchShortestPoint(m_Pos);

	m_Model->SetShader(CShaderSkinmeshSpotlight::GetShader());

	// 残弾表示
	m_AmmoNum = CNumber::Create();
	m_AmmoNum->SetNum(m_UsingWeapon->GetAmmo());
	m_AmmoNum->Set(AMMO_DISPLAY_POS);
}

void CPlayer::Uninit()
{
	if (m_Pattern != NULL)
	{
		delete m_Pattern;
	}
	if (m_Camera)
	{
		m_Camera->Release();
	}
	if (m_Model)
	{
		m_Model->Release();
	}
	if (m_DamagedEffect)
	{
		m_DamagedEffect->Release();
	}
}

void CPlayer::Update()
{
	CInputKeyboard *inputKeyboard;
	inputKeyboard = CManager::GetInputKeyboard();

	if (CManager::GetDebug())
	{
		ImGui::Begin("Player", 0);
		ImGui::SliderFloat("Life", &m_Life, 0.0f, 100.0f);
		ImGui::Text("Area:%d", m_AreaID);
		ImGui::End();
	}
	else
	{
		ADS();
		m_Pattern->Update(this);

		m_DamagedEffect->SetColor(D3DCOLOR_RGBA(172, 15, 15, (int)(255 * (1 - (m_Life / m_MaxLife)))));
		m_ShortestPoint = CWayPoint::SearchShortestPoint(m_Pos);
	}
}

void CPlayer::Draw()
{
}

CPlayer* CPlayer::Create(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos)
{
	CPlayer* player = new CPlayer();
	player->Init(modelId, spawnPos);

	return player;
}

void CPlayer::Shoot()
{
	m_UsingWeapon->Shoot();
	m_AmmoNum->SetNum(m_UsingWeapon->GetAmmo());
}

void CPlayer::TriggerRelease()
{
	m_UsingWeapon->ReleaseTrigger();
}

void CPlayer::Death()
{

}

void CPlayer::Rotate(D3DXVECTOR3 vec)
{
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxIdentity;	// 比較用
	D3DXVECTOR3 v1 = m_Forward;
	D3DXVECTOR3 v2 = { vec.x, 0, vec.z };

	//*********************************************************
	//	左右回転
	//*********************************************************
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
			if (!isnan(rot))
			{
					D3DXMatrixRotationY(&mtxRot, D3DXToRadian(rot));
			}
		}
		else
		{
			if (!isnan(rot))
			{
				D3DXMatrixRotationY(&mtxRot, -D3DXToRadian(rot));
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

	D3DXVECTOR3 right = m_Camera->GetRight();
}

void CPlayer::Rotate(const float& horizontal, const float& vertical)
{
	m_Camera->Rotation(horizontal, vertical);

	//*********************************************************
	//	左右回転
	//*********************************************************
	{
		// 回転行列を宣言
		D3DXMATRIX mtxRotation;

		// 回転行列(Y軸回転)を作る(回転速度)
		D3DXMatrixRotationY(&mtxRotation, CAMERA_AT_RAD * horizontal);

		D3DXVECTOR3 dir = m_Camera->GetFront();

		D3DXVec3TransformNormal(&dir, &dir, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)

		D3DXVec3TransformNormal(&m_Forward, &m_Forward, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)
		D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)
		D3DXVec3Normalize(&m_Forward, &m_Forward);
		D3DXVec3Normalize(&m_Right, &m_Right);

		m_Rotate *= mtxRotation;

		m_Model->Rotate(m_Rotate);
	}

	//*********************************************************
	//	上下回転
	//*********************************************************
	{
		// 回転行列を宣言
		D3DXMATRIX mtxRotation;

		// 回転行列を作る(回転速度)
		D3DXMatrixRotationAxis(&mtxRotation, &m_Right, CAMERA_AT_RAD * vertical);

		D3DXVECTOR3 dir = m_Camera->GetFront();

		D3DXVec3TransformNormal(&dir, &dir, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)

		D3DXVECTOR3 front;

		D3DXVec3TransformNormal(&front, &m_Forward, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)

		D3DXVECTOR3 vec = front; // 移動方向
		vec.y = 0;

		// vecを正規化する
		D3DXVec3Normalize(&vec, &vec);

		float dot = D3DXVec3Dot(&front, &vec);

		// 上下の限界を設ける
		if (dot > 0.5f)
		{
			D3DXVec3TransformNormal(&m_Forward, &m_Forward, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)
			D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)
			D3DXVec3Normalize(&m_Forward, &m_Forward);
			D3DXVec3Normalize(&m_Right, &m_Right);

			m_Rotate *= mtxRotation;

			m_Model->Rotate(m_Rotate);
		}
	}
}

void CPlayer::ChangeWeapon(const int& id)
{
	m_UsingWeapon = m_Weapon[id];

	for (int i = 0; i < HAVE_WEAPON; i++)
	{
		if (m_Weapon[i] != m_UsingWeapon)
		{
			m_Weapon[i]->SetActive(false);
		}
		else
		{
			m_Weapon[i]->SetActive(true);
		}
	}

	m_AmmoNum->SetNum(m_UsingWeapon->GetAmmo());
}

void CPlayer::ChangeWeapon()
{
	for (int i = 0; i < HAVE_WEAPON; i++)
	{
		if (m_Weapon[i] != m_UsingWeapon)
		{
			m_UsingWeapon = m_Weapon[i];
			break;
		}
	}

	for (int i = 0; i < HAVE_WEAPON; i++)
	{
		if (m_Weapon[i] != m_UsingWeapon)
		{
			m_Weapon[i]->SetActive(false);
		}
		else
		{
			m_Weapon[i]->SetActive(true);
		}
	}

	m_AmmoNum->SetNum(m_UsingWeapon->GetAmmo());
}

void CPlayer::Move(const float& moveX, const float& moveZ)
{	
	// 移動・回転
	D3DXVECTOR3 cameraFront = m_Camera->GetFront();
	D3DXVECTOR3 cameraRight = m_Camera->GetRight();
	D3DXVECTOR3 newPos = m_Pos;
	D3DXVECTOR3 moveVec = { moveX, 0.0f, moveZ };

	D3DXVec3Normalize(&moveVec, &moveVec);

	// 前方向ベクトルを地面と平行に正規化
	cameraFront.y = 0;
	D3DXVec3Normalize(&cameraFront, &cameraFront);

	newPos += cameraFront * PLAYER_MOVE_SPEED * moveZ;
	newPos += cameraRight * PLAYER_MOVE_SPEED * moveX;

	SearchArea(newPos);
	newPos.y = m_Field->GetHeight(newPos, this);

	if (newPos.y - m_Pos.y > 0.016)
	{
		return;
	}

	// コリジョンの計算
	m_CapsuleCollision.Set(Point(newPos.x, newPos.y + PLAYER_CUPSULE_RAD, newPos.z),
		Point(newPos.x, newPos.y + 1.2f, newPos.z),
		PLAYER_CUPSULE_RAD);

	// キャラクターとの当たり判定
	for (int i = 0; i < CHARACTER_MAX; i++)
	{
		CCharacter* obj = CCharacter::GetCharacter(i);
		if (obj != NULL)
		{
			if (obj->GetType() == CHARACTER_ENEMY)
			{
				CEnemy* enemy = (CEnemy*)obj;
				D3DXVECTOR3 vec = newPos - enemy->GetPos();
				if (D3DXVec3Length(&vec) < 3.0f)
				{
					if (isCollisionCapsule(m_CapsuleCollision, enemy->GetCapsule()))
					{
						D3DXVec3Normalize(&vec, &vec);

						newPos = enemy->GetPos();
						newPos += vec * (PLAYER_CUPSULE_RAD + enemy->GetCapsuleRad());
					}
				}
			}
		}
	}

	// アイテムとの当たり判定
	for (int i = 0; i < ITEM_MAX; i++)
	{
		CItem* item = CItem::GetItem(i);
		if (item != NULL)
		{
			if (item->GetType() == ITEM_TYPE_CAPSULE)
			{
				CTargetCapsule* capsule = (CTargetCapsule*)item;

				if (isCollisionCapsule(m_CapsuleCollision, capsule->GetCapsule()))
				{
					D3DXVECTOR3 vec = newPos - capsule->GetPos();

					D3DXVec3Normalize(&vec, &vec);

					newPos = capsule->GetPos();
					newPos += vec * (PLAYER_CUPSULE_RAD + 1.0f);
				}
			}
		}
	}

	m_Camera->Move(newPos - m_Pos);

	SetPos(newPos);

	m_Model->Move(m_Pos + m_LocalCameraPos);
	m_Shadow->Move(m_Pos + m_LocalCameraPos);

	// 当たり判定の移動
	m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + PLAYER_CUPSULE_RAD, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.0f, m_Pos.z), PLAYER_CUPSULE_RAD);
}

void CPlayer::MoveAir(const float& moveX, const float& moveY, const float& moveZ)
{
	// 移動・回転
	D3DXVECTOR3 cameraFront = m_Camera->GetFront();
	D3DXVECTOR3 cameraRight = m_Camera->GetRight();
	D3DXVECTOR3 newPos = m_Pos;
	D3DXVECTOR3 moveVec = { moveX, 0.0f, moveZ };

	D3DXVec3Normalize(&moveVec, &moveVec);

	// 前方向ベクトルを地面と平行に正規化
	cameraFront.y = 0;
	D3DXVec3Normalize(&cameraFront, &cameraFront);

	newPos += cameraFront * PLAYER_MOVE_SPEED * moveZ;
	newPos += cameraRight * PLAYER_MOVE_SPEED * moveX;
	newPos += m_Up * moveY;

	SearchArea(newPos);
	float fieldHeight = m_Field->GetHeight(newPos, this);
	if (newPos.y < fieldHeight)
	{
		newPos.y = fieldHeight;
		ChangePattern(new CPlayerPatternNormal());
	}

	// コリジョンの計算
	m_CapsuleCollision.Set(Point(newPos.x, newPos.y + PLAYER_CUPSULE_RAD, newPos.z),
		Point(newPos.x, newPos.y + 1.2f, newPos.z),
		PLAYER_CUPSULE_RAD);

	// キャラクターとの当たり判定
	for (int i = 0; i < CHARACTER_MAX; i++)
	{
		CCharacter* obj = CCharacter::GetCharacter(i);
		if (obj != NULL)
		{
			if (obj->GetType() == CHARACTER_ENEMY)
			{
				CEnemy* enemy = (CEnemy*)obj;
				if (isCollisionCapsule(m_CapsuleCollision, enemy->GetCapsule()))
				{
					D3DXVECTOR3 vec = newPos - enemy->GetPos();
					D3DXVec3Normalize(&vec, &vec);

					newPos = enemy->GetPos();
					newPos += vec * (PLAYER_CUPSULE_RAD + enemy->GetCapsuleRad());
				}
			}
		}
	}

	m_Camera->Move(newPos - m_Pos);

	SetPos(newPos);

	m_Model->Move(m_Pos + m_LocalCameraPos);
	m_Shadow->Move(m_Pos + m_LocalCameraPos);

	// 当たり判定の移動
	m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + PLAYER_CUPSULE_RAD, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.0f, m_Pos.z), PLAYER_CUPSULE_RAD);
}

void CPlayer::ADS()
{
	if (m_isADS)
	{
		if (m_Camera->GetFov() > ADS_FOV)
		{
			m_Camera->SetFov(m_Camera->GetFov() - (DEFAULT_FOV - ADS_FOV) / 10.0f);
		}
		else
		{
			m_Camera->SetFov(ADS_FOV);
		}
	}
	else
	{
		if (m_Camera->GetFov() < DEFAULT_FOV)
		{
			m_Camera->SetFov(m_Camera->GetFov() + (DEFAULT_FOV - ADS_FOV) / 10.0f);
		}
		else
		{
			m_Camera->SetFov(DEFAULT_FOV);
		}
	}
}

void CPlayer::SetADS(bool ads)
{
	m_isADS = ads;
}

void CPlayer::Reload()
{
	m_UsingWeapon->Reload();
	m_AmmoNum->SetNum(m_UsingWeapon->GetAmmo());
}

void CPlayer::SetWeaponADS(bool ads)
{
	m_UsingWeapon->SetADS(ads);
}

void CPlayer::PlayReload()
{ 
	m_SoundReload->Play(); 
}

void CPlayer::ChangePattern(CPlayerPatternBase* next)
{
	if (m_Pattern != NULL)
	{
		m_Pattern->Uninit(this);
		delete m_Pattern;
	}

	m_Pattern = next;
	next->Init(this);
}

void CPlayer::Damaged(float damage)
{
	if (CManager::GetDebug())
	{
		return;
	}

	CCharacter::Damaged(damage);
	if (m_Life <= 0.0f)
	{
		Death();
		CModeGame::GetEnemyManager()->AddPlayerTension(DAMAGED_TENSION);
		CModeGame::GameEnd(GAME_OVER);
	}
}
