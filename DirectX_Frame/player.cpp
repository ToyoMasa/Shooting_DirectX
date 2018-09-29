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
#include "character.h"
#include "player.h"
#include "input.h"
#include "field.h"
#include "enemy.h"
#include "game.h"
#include "PlayerAnim.h"
#include "wall.h"
#include "bullet.h"

static const float VALUE_ROTATE_MOUSE = 0.003f;

CSceneModel* test;

void CPlayer::Init(int modelId, D3DXVECTOR3 spawnPos)
{
	m_Model = CSceneSkinMesh::Create(SKINMESH_SOURCE[SM_ID_PLAYER]);
	m_Model->ChangeAnim(PLAYER_IDLE, 0.0f);

	m_Pos = spawnPos;
	D3DXVECTOR3 pos = m_Pos;
	m_LocalCameraPos.x = 0.0f;
	m_LocalCameraPos.y = 1.6f;
	m_LocalCameraPos.z = 0.25f;
	pos += m_LocalCameraPos;
	D3DXVECTOR3 at = m_LocalCameraPos;
	at.z += 1.0f;
	m_Model->Move(m_Pos);

	m_Camera = CCamera::Create(pos, at);
	m_Forward = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_Right = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	m_Text_Attack = CScene2D::Create(TEX_ID_ATTACK, 128, 64);
	m_Text_Attack->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200.0f, 0.0f));
	m_Caution = CScene2D::Create(TEX_ID_CAUTION, 440, 80);
	m_Caution->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_Caution->SetColor(D3DCOLOR_RGBA(255, 0, 0, 255));
	m_Caution->SetVisible(false);

	m_BloodEffect = CEffekseer::Create(CEffekseer::Effect_BloodLoss, m_Camera);
	m_BloodEffect->RepeatEffect(false);
	m_BloodEffect->SetScale(0.1f, 0.1f, 0.1f);
	m_BloodEffect->SetVisible(true);

	m_Knife = CSound::Create(SOUND_LABEL_SE_KNIFE);
	m_Hit = CSound::Create(SOUND_LABEL_SE_HIT);

	// モデルの回転軸をカメラの位置にそろえる
	D3DXMatrixTranslation(&m_LocalLocation, m_LocalCameraPos.x, -m_LocalCameraPos.y, m_LocalCameraPos.z);
	m_Rotate = m_LocalLocation;

	Rotate(m_Camera->GetFront());

	test = CSceneModel::Create("data/models/rifle.x");
	test->Move(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	test->Scale(D3DXVECTOR3(5.0f, 5.0f, 5.0f));
}

void CPlayer::Uninit()
{
	if (m_Camera)
	{
		m_Camera->Release();
	}
	if (m_Model)
	{
		m_Model->Release();
	}
	if (m_Knife)
	{
		m_Knife->Release();
	}
}

void CPlayer::Update()
{
	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	float mouseX, mouseY, mouseZ;

	if (m_isPreDeath)
	{
		if (m_Model->GetWeightTime() >= 5.0f)
		{
			CModeGame::PlayerDied();
		}
	}
	else
	{
		// 攻撃可能か表示を一旦リセット
		m_Text_Attack->SetVisible(false);

		// キーボード取得
		inputKeyboard = CManager::GetInputKeyboard();

		// マウス取得
		inputMouse = CManager::GetInputMouse();
		mouseX = (float)inputMouse->GetAxisX();
		mouseY = (float)inputMouse->GetAxisY();
		mouseZ = (float)inputMouse->GetAxisZ();

		float moveX = 0.0f, moveZ = 0.0f;
		if (inputKeyboard->GetKeyPress(DIK_A))
		{
			moveX = -1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_D))
		{
			moveX = 1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_W))
		{
			moveZ = 1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_S))
		{
			moveZ = -1.0f;
		}

		if (!m_Model->GetPlayMontage())
		{
			{// 移動・回転
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
				newPos.y = m_Field->GetHeight(newPos);

				// コリジョンの計算
				m_CapsuleCollision.Set(Point(newPos.x, newPos.y + 0.25f, newPos.z), Point(newPos.x, newPos.y + 1.0f, newPos.z), 0.25f);

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
								newPos += vec * 0.5f;
							}
						}
					}
				}

				// 壁との当たり判定
				newPos = HitWall(newPos);

				m_Camera->Move(newPos - m_Pos);
				SetPos(newPos);
			}
		}

		m_Model->Move(m_Pos + m_LocalCameraPos);
		m_Shadow->Move(m_Pos);

		// 当たり判定の移動
		m_CapsuleCollision.Set(Point(m_Pos.x, m_Pos.y + 0.25f, m_Pos.z), Point(m_Pos.x, m_Pos.y + 1.0f, m_Pos.z), 0.25f);

		// ADS
		if (inputMouse->GetRightPress())
		{
			m_Model->ChangeAnim(PLAYER_ADS, 0.3f);
		}
		else
		{
			m_Model->ChangeAnim(PLAYER_IDLE, 0.3f);
		}

		// 攻撃
		if (inputMouse->GetLeftTrigger() || inputKeyboard->GetKeyTrigger(DIK_SPACE))
		{
			CBullet::Create(m_Camera->GetPos(), m_Camera->GetFront(), 1.0f, 100.0f, 50);
		}

		// エリア外に出るとゲームオーバー
		D3DXVECTOR3 len = D3DXVECTOR3(0, 0, 0) - m_Pos;
		if (D3DXVec3Length(&len) > 40 && !m_isGameOver)
		{
			m_Caution->SetVisible(true);

			if (D3DXVec3Length(&len) > 50)
			{
				CModeGame::PlayerDied();
				m_Caution->SetVisible(false);
				m_isGameOver = true;
			}
		}
		else
		{
			m_Caution->SetVisible(false);
		}

		// カメラの回転
		m_Camera->Rotation(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);

		// モデルの回転
		Rotate(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);
	}

}

void CPlayer::Draw()
{
}

CPlayer* CPlayer::Create(int modelId, D3DXVECTOR3 spawnPos)
{
	CPlayer* player = new CPlayer();
	player->Init(modelId, spawnPos);

	return player;
}

void CPlayer::Attack()
{
	//m_Model->PlayMontage(PLAYER_STAB, 0.2f, 3.4f, PLAYER_IDLE, 2.0f);
	m_Knife->Play();

	m_isPreAttack = true;
}

void CPlayer::Death()
{
	//m_Model->PlayMontage(PLAYER_DEATH, 0.3f, 8.0f, PLAYER_DEATH, 0.75f);
	m_Hit->Play();

	m_isPreDeath = true;
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

void CPlayer::Rotate(float horizontal, float vertical)
{
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

D3DXMATRIX CPlayer::GetMatrix()
{
	D3DXMATRIX target = m_Model->GetBoneMatrix("root_hand_l");

	D3DXMATRIX target2 = m_Model->GetBoneMatrix("root_hand_l");

	ImGui::Begin("Matrix2", 0);
	ImGui::Text("m11 = %.2f m12 = %.2f m13 = %.2f m14 = %.2f", target2._11, target2._12, target2._13, target2._14);
	ImGui::Text("m21 = %.2f m22 = %.2f m23 = %.2f m24 = %.2f", target2._21, target2._22, target2._23, target2._24);
	ImGui::Text("m31 = %.2f m32 = %.2f m33 = %.2f m34 = %.2f", target2._31, target2._32, target2._33, target2._34);
	ImGui::Text("m41 = %.2f m42 = %.2f m43 = %.2f m44 = %.2f", target2._41, target2._42, target2._43, target2._44);
	ImGui::End();

	//test->Rotate(D3DXVECTOR3(target2._41, target2._42, target2._43));
	test->Scale(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	test->SetWorld(target2);
	ImGui::Begin("Weapon", 0);
	ImGui::Text("m11 = %.2f m12 = %.2f m13 = %.2f m14 = %.2f", test->GetWorld()._11, test->GetWorld()._12, test->GetWorld()._13, test->GetWorld()._14);
	ImGui::Text("m21 = %.2f m22 = %.2f m23 = %.2f m24 = %.2f", test->GetWorld()._21, test->GetWorld()._22, test->GetWorld()._23, test->GetWorld()._24);
	ImGui::Text("m31 = %.2f m32 = %.2f m33 = %.2f m34 = %.2f", test->GetWorld()._31, test->GetWorld()._32, test->GetWorld()._33, test->GetWorld()._34);
	ImGui::Text("m41 = %.2f m42 = %.2f m43 = %.2f m44 = %.2f", test->GetWorld()._41, test->GetWorld()._42, test->GetWorld()._43, test->GetWorld()._44);
	ImGui::Text("X = %.2f Y = %.2f Z = %.2f", test->GetPos().x, test->GetPos().y, test->GetPos().z);
	ImGui::End();

	return target;
}
