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
#include "fade.h"
#include "Effekseer.h"
#include "wall.h"
#include "bullet.h"
#include "weapon.h"
#include "debug.h"
#include "waypoint.h"
#include "EnemyAnim.h"
#include "enemyPatternChase.h"
#include "enemyPatternWaypoints.h"
#include "shader.h"
#include "fog.h"
#include "enemyManager.h"
#include "item.h"
#include "targetCapsule.h"

CPlayer *CModeGame::player = NULL;
CLight *CModeGame::Light;
bool CModeGame::Pause = false;
bool CModeGame::GameFinish = false;
CScene2D *CModeGame::ResultText = NULL;
CScene2D *CModeGame::EndBackground = NULL;
CScene2D *CModeGame::Black = NULL;
CScene2D *CModeGame::PauseWord = NULL;
CSound *CModeGame::BGM = NULL;
CSound *CModeGame::GameEnd_SE = NULL;
CSound *CModeGame::ZombieVoice = NULL;
GAME_RESULT CModeGame::Result = GAME_OVER;
int CModeGame::FrameCount = 0;
int CModeGame::KillCount = 0;
CFog *CModeGame::Fog = NULL;
CField *CModeGame::Field = NULL;
CEnemyManager *CModeGame::EnemyManager = NULL;
CTargetCapsule *CModeGame::Target[3] = { NULL };

float g_FogColor[4];
float g_LightDiff[4];
float g_LightAmb[4];
float g_Density = 0.07f;

CTargetCapsule* tc;

void CModeGame::Init()
{
	// テクスチャの初期化
	CTexture::Init();

	// フェード用のテクスチャ
	Black = CScene2D::Create(TEX_ID_WHITE, SCREEN_WIDTH, SCREEN_HEIGHT);
	Black->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));
	Black->SetColor(D3DCOLOR_RGBA(0, 0, 0, 128));
	Black->SetVisible(false);

	// ポーズの準備
	PauseWord = CScene2D::Create(TEX_ID_PAUSE, 172.0f, 97.0f);
	PauseWord->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	PauseWord->SetVisible(false);

	// 敵の経路を作成
	CWayPoint::Init();

	// フェード
	CFade::FadeIn();

	// フィールド
	Field = CField::Create("data/output/map.txt");

	// 空
	CSkyBox::Create(player);

	// プレイヤー
	player = CPlayer::Create(SM_ID_PLAYER, D3DXVECTOR3(-68.0f, 0.0f, -74.0f));
	player->SetField(Field);
	CManager::SetCamera(player->GetCamera());

	EnemyManager = new CEnemyManager(Field);
	
	// ライト
	Light = CLight::Create(0);

	// ビルボードの準備
	CBillBoard::Init();

	// サウンドの準備
	ZombieVoice = CSound::Create(SOUND_LABEL_BGM_ZOMBIE_BREATH);
	ZombieVoice->Play(0.01f);
	BGM = CSound::Create(SOUND_LABEL_BGM_LOAD);
	BGM->Play();

	// スコア等のリセット
	Result = GAME_OVER;
	GameEnd_SE = NULL;
	Pause = false;
	GameFinish = false;
	KillCount = 0;
	FrameCount = 0;

	Fog->Set(D3DCOLOR_RGBA(18, 18, 36, 255), 0.1f);

	CTargetCapsule::Create(D3DXVECTOR3(34.2f, 0.0f, 62.5f));
	CTargetCapsule::Create(D3DXVECTOR3(53.3f, 0.0f, -82.0f));
	CTargetCapsule::Create(D3DXVECTOR3(-11.75f, 0.0f, -7.0f));
}

void CModeGame::Uninit()
{
	CItem::ReleaseAll();

	CBullet::ReleaseAll();

	CWeapon::ReleaseAll();

	CCharacter::ReleaseAll();

	CScene::ReleaseAll();

	CSound::ReleaseAll();

	CParticleEmitter::ReleaseAll();

	CParticle::ReleaseAll();

	Light->Release();

	CBillBoard::Uninit();

	// 全てのテクスチャの解放
	CTexture::ReleaseAll();

	CWayPoint::Uninit();

	delete EnemyManager;
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

	if (!CFade::GetFadeOut())
	{
		if (GameFinish)
		{
			FrameCount++;
			if (FrameCount <= 255)
			{
				if (Result == GAME_CLEAR)
				{
					EndBackground->SetColor(D3DCOLOR_RGBA(255, 255, 255, FrameCount));
				}
				else
				{
					EndBackground->SetColor(D3DCOLOR_RGBA(186, 7, 7, FrameCount / 2));
				}
				ResultText->SetColor(D3DCOLOR_RGBA(255, 255, 255, FrameCount));
			}
			else
			{
				if (inputKeyboard->GetKeyTrigger(DIK_SPACE) || inputKeyboard->GetKeyTrigger(DIK_RETURN) || inputMouse->GetLeftTrigger())
				{
					CFade::FadeOut(new CModeResult(KillCount));
				}
			}
		}
		else if (Pause)
		{
			FrameCount++;

			if (FrameCount / 256 % 2 == 0.0f)
			{
				PauseWord->SetColor(D3DCOLOR_RGBA(255, 255, 255, FrameCount % 256));
			}
			else
			{
				PauseWord->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255 - (FrameCount % 256)));
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
			EnemyManager->Update();
			CItem::UpdateAll();

			if (inputKeyboard->GetKeyTrigger(DIK_P) || inputKeyboard->GetKeyTrigger(DIK_TAB))
			{
				CallPause();
			}
		}
	}
}

void CModeGame::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	CScene::DrawAll();

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
	CBillBoard::DrawAll();
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON

	if (CManager::GetDebug())
	{
		ImGui::Begin("Debug Window");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}

void CModeGame::GameEnd(GAME_RESULT result)
{
	Result = result;
	GameFinish = true;
	FrameCount = 0;

	CSound* se;

	if (Result == GAME_CLEAR)
	{
		EndBackground = CScene2D::Create(TEX_ID_WHITE, SCREEN_WIDTH, SCREEN_HEIGHT);
		EndBackground->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));
		EndBackground->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));

		ResultText = CScene2D::Create(TEX_ID_MISSION_COMPLETE, 1039.0f, 128.0f);
		ResultText->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
		ResultText->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));

		se = CSound::Create(SOUND_LABEL_SE_GAMECLEAR);
		se->Play();
	}
	else
	{
		EndBackground = CScene2D::Create(TEX_ID_WHITE, SCREEN_WIDTH, SCREEN_HEIGHT);
		EndBackground->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));
		EndBackground->SetColor(D3DCOLOR_RGBA(186, 7, 7, 0));

		ResultText = CScene2D::Create(TEX_ID_MISSION_FAILED, 923.0f, 128.0f);
		ResultText->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
		ResultText->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));

		se = CSound::Create(SOUND_LABEL_SE_GAMEOVER);
		se->Play();
	}
}

void CModeGame::CallPause()
{
	Pause = !Pause;
	Black->SetVisible(Pause);
	PauseWord->SetVisible(Pause);
}

CCamera* CModeGame::GetCamera()
{
	return player->GetCamera(); 
}
