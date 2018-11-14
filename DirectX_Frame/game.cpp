//======================================================================
//	ゲーム画面管理
//
//======================================================================
#include "common.h"
#include "main.h"
#include "mode.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "sceneSkinMesh.h"
#include "SkinMeshFile.h"
#include "camera.h"
#include "light.h"
#include "manager.h"
#include "texture.h"
#include "billboard.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "skybox.h"
#include "number.h"
#include "game.h"
#include "result.h"
#include "mapmake.h"
#include "particle.h"
#include "emitter.h"
#include "action.h"
#include "root.h"
#include "fade.h"
#include "Effekseer.h"
#include "wall.h"
#include "bullet.h"
#include "weapon.h"
#include "debug.h"
#include "EnemyAnim.h"
#include "enemyPatternChase.h"
#include "shader.h"
#include "fog.h"

CBillBoard *CModeGame::tree1 = NULL;
CBillBoard *CModeGame::tree2 = NULL;
CPlayer *CModeGame::player = NULL;
CEnemy *CModeGame::enemy[ENEMY_NUM] = { NULL };
CEnemy *CModeGame::Target = NULL;
CLight *CModeGame::m_Light;
bool CModeGame::m_PlayerDied = false;
bool CModeGame::m_TargetDied = false;
bool CModeGame::m_PlayBGM = false;
bool CModeGame::m_Pause = false;
CScene2D *CModeGame::Load = NULL;
CScene2D *CModeGame::LoadFrame = NULL;
CScene2D *CModeGame::LoadGage = NULL;
CScene2D *CModeGame::GameClear = NULL;
CScene2D *CModeGame::GameOver = NULL;
CScene2D *CModeGame::Mission = NULL;
CScene2D *CModeGame::Wanted = NULL;
CScene2D *CModeGame::Tutorial = NULL;
CScene2D *CModeGame::Tutorial2 = NULL;
CScene2D *CModeGame::Black = NULL;
CScene2D *CModeGame::Pause = NULL;
CScene2D *CModeGame::HowToUse = NULL;
CSound *CModeGame::BGM = NULL;
CSound *CModeGame::GameEnd_SE = NULL;
int CModeGame::m_NumKill = 0;
int CModeGame::m_NumSneak = 0; 
int CModeGame::m_CountResult = 0;
int CModeGame::m_Count = 0;
CFog *CModeGame::Fog = NULL;

float g_FogColor[4];
float g_LightDiff[4];
float g_LightAmb[4];
float g_Density = 0.07f;

void CModeGame::Init()
{
	// テクスチャの初期化
	CTexture::Init();
	
	Black = CScene2D::Create(TEX_ID_BLACK, SCREEN_WIDTH, SCREEN_HEIGHT);
	Black->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));
	Black->SetColor(D3DCOLOR_RGBA(0, 0, 0, 128));
	Black->SetVisible(false);

	// ルートの設定
	CRoot::Set();

	// フェード
	CFade::FadeIn();

	// フィールド
	//CField* field = CField::Create(NULL, 10.0f, 20, 20, true);
	CField* field = CField::Create("data/output/map.txt");

	// プレイヤー
	player = CPlayer::Create(SM_ID_PLAYER, D3DXVECTOR3(-90.0f, 0.0f, -90.0f));
	player->SetField(field);
	CManager::SetCamera(player->GetCamera());

	//for (int j = 0; j < 5; j++)
	//{
	//	for (int i = 0; i < 10; i++)
	//	{
	//		enemy[10 * j + i] = CEnemy::Create(SM_ID_ZOMBIE_A, D3DXVECTOR3(-50.0f + i * 10.0f, 0.0f, -50.0f + j * 10.0f), new CEnemyPatternChase(), field);
	//	}
	//}
	
	// 空
	CSkyBox::Create(player);

	// ライト
	m_Light = CLight::Create(0);

	// ビルボードの準備
	CBillBoard::Init();

	// スコア等のリセット
	m_PlayerDied = false;
	m_TargetDied = false;
	m_PlayBGM = false;
	m_NumKill = 0;
	m_NumSneak = 0;
	m_CountResult = 0;
	GameEnd_SE = NULL;

	//Fog->Set(D3DCOLOR_RGBA(0, 0, 0, 128), 2.0f, 50.0f);
	Fog->Set(D3DCOLOR_RGBA(18, 18, 36, 128), 0.07f);
}

void CModeGame::Uninit()
{
	CBullet::ReleaseAll();

	CWeapon::ReleaseAll();

	CCharacter::ReleaseAll();

	CSceneSkinMesh::ReleaseFileAll();

	CScene::ReleaseAll();

	CSound::ReleaseAll();

	CActionBase::ReleaseAll();

	CParticleEmitter::ReleaseAll();

	CParticle::ReleaseAll();

	m_Light->Release();

	CBillBoard::Uninit();

	// 全てのテクスチャの解放
	CTexture::ReleaseAll();
}

void CModeGame::Update()
{
	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	float mouseX, mouseY, mouseZ;

	// キーボード取得
	inputKeyboard = CManager::GetInputKeyboard();

	// マウス取得
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	if (!CFade::GetFade())
	{
		if (!m_PlayBGM)
		{
			//BGM = CSound::Create(SOUND_LABEL_BGM_GAME);
			//BGM->Play();
			m_PlayBGM = true;
		}
	}

	if (!CFade::GetFadeOut())
	{
		if (m_Pause)
		{
			m_Count++;

			if (m_Count / 256 % 2 == 0.0f)
			{
				Pause->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Count % 256));
			}
			else
			{
				Pause->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255 - (m_Count % 256)));
			}

			if (inputKeyboard->GetKeyTrigger(DIK_P) || inputKeyboard->GetKeyTrigger(DIK_TAB))
			{
				CallPause();
			}
		}
		else
		{
			CCharacter::UpdateAll();
			CScene::UpdateAll();
			CBullet::UpdateAll();
			CParticleEmitter::UpdateAll();
			CParticle::UpdateAll();
			CBillBoard::UpdateAll();
			CWeapon::UpdateAll();

			if (m_TargetDied)
			{
				m_CountResult++;

				if (GameEnd_SE == NULL)
				{
					GameEnd_SE = CSound::Create(SOUND_LABEL_SE_GAMECLEAR);
					GameEnd_SE->Play();
				}

				if (m_CountResult > 255)
				{
					if (m_CountResult > 300)
					{
						CFade::FadeOut(new CModeResult(m_NumKill, m_NumSneak, true));
					}
				}
				else
				{
					GameClear->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_CountResult));
				}
			}
			else if (m_PlayerDied)
			{
				if (GameEnd_SE == NULL)
				{
					GameEnd_SE = CSound::Create(SOUND_LABEL_SE_GAMEOVER);
					GameEnd_SE->Play();
					GameOver->SetVisible(true);
				}

				m_CountResult++;
				if (m_CountResult > 255)
				{
					if (m_CountResult > 300)
					{
						CFade::FadeOut(new CModeResult(m_NumKill, m_NumSneak, false));
					}
				}
				else
				{
					GameClear->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_CountResult));
				}
			}

			if (inputKeyboard->GetKeyTrigger(DIK_P) || inputKeyboard->GetKeyTrigger(DIK_TAB))
			{
				CallPause();
			}
		}
	}

	ImGui::Begin("MapConfig", 0);
	if (ImGui::Button("MapEditer"))
	{
		CFade::FadeOut(new CModeMapMake());
	}

	ImGui::ColorEdit4("DiffColor", g_LightDiff);
	ImGui::ColorEdit4("AmbColor", g_LightAmb);
	ImGui::ColorEdit4("FogColor", g_FogColor);
	ImGui::SliderFloat("FogDensity", &g_Density, 0.0f, 1.0f);
	ImGui::End();

	Fog->Set(D3DCOLOR_RGBA((int)(g_FogColor[0] * 255), (int)(g_FogColor[1] * 255), (int)(g_FogColor[2] * 255), (int)(g_FogColor[3] * 255)), g_Density);
	m_Light->SetLight(g_LightDiff[0], g_LightDiff[1], g_LightDiff[2], g_LightDiff[3], g_LightAmb[0], g_LightAmb[1], g_LightAmb[2], g_LightAmb[3]);
}

void CModeGame::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	CScene::DrawAll();

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：ON
	CBillBoard::DrawAll(player->GetCamera());
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON

	CImGui::EndDraw();
}

void CModeGame::PlayerDied()
{
	m_PlayerDied = true;
}

void CModeGame::TargetKilled()
{
	m_TargetDied = true;
	GameClear->SetVisible(true);
}

void CModeGame::MakeMap()
{
	CWall::Create(D3DXVECTOR3(13.5f, 0.0f, -24.5f), 23.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-13.5f, 0.0f, -24.5f), 23.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-24.5f, 0.0f, -2.0f), 1.0f, 4.0f, 44.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(24.5f, 0.0f, -13.0f), 1.0f, 4.0f, 22.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(0.0f, 0.0f, 24.5f), 50.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(9.5f, 0.0f, 12.5f), 7.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-22.0f, 0.0f, -13.5f), 4.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-8.5f, 0.0f, -16.0f), 1.0f, 4.0f, 4.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-8.5f, 0.0f, -22.0f), 1.0f, 4.0f, 4.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-13.0f, 0.0f, -13.5f), 10.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(7.5f, 0.0f, -18.5f), 1.0f, 4.0f, 11.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-14.5f, 0.0f, 2.5f), 1.0f, 4.0f, 9.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(5.5f, 0.0f, 18.0f), 1.0f, 4.0f, 12.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-12.0f, 0.0f, 18.5f), 4.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-20.0f, 0.0f, 18.5f), 8.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-19.0f, 0.0f, 12.5f), 10.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-11.5f, 0.0f, 21.5f), 1.0f, 4.0f, 5.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-5.0f, 0.0f, 18.5f), 6.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(1.0f, 0.0f, 18.5f), 2.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(1.5f, 0.0f, 21.5f), 1.0f, 4.0f, 5.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-19.0f, 0.0f, -7.5f), 10.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(20.5f, 0.0f, 12.5f), 7.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-24.5f, 0.0f, 23.5f), 1.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(24.5f, 0.0f, 13.0f), 1.0f, 4.0f, 22.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(10.5f, 0.0f, -12.5f), 7.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(21.0f, 0.0f, -12.5f), 6.0f, 4.0f, 1.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-14.5f, 0.0f, 11.0f), 1.0f, 4.0f, 2.0f, TEX_ID_WALL01);
	CWall::Create(D3DXVECTOR3(-14.5f, 0.0f, -6.0f), 1.0f, 4.0f, 2.0f, TEX_ID_WALL01);
}

void CModeGame::CallPause()
{
	if (m_Pause)
	{
		m_Pause = false;
		Black->SetVisible(false);
		Pause->SetVisible(false);
		Tutorial->SetVisible(false);
	}
	else
	{
		m_Pause = true;
		Black->SetVisible(true);
		Pause->SetVisible(true);
		Tutorial->SetVisible(true);
	}
}

CCamera* CModeGame::GetCamera()
{
	return player->GetCamera(); 
}
