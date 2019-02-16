//======================================================================
//	ゲーム画面管理
//
//======================================================================
#include <Windows.h>
#include <Xinput.h>
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
#include "controller.h"
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
#include "playerPatternStop.h"
#include "playerPatternNormal.h"
#include "event.h"
#include "eventStartEnemy.h"
#include "tutorialCamera.h"

static const float BAR_LENGTH = 300.0f;

CPlayer *CModeGame::player = NULL;
CLight *CModeGame::Light;
bool CModeGame::Pause = false;
bool CModeGame::GameFinish = false;
bool CModeGame::isHoldSensiBar = false;
CScene2D *CModeGame::ResultText = NULL;
CScene2D *CModeGame::EndBackground = NULL;
CScene2D *CModeGame::Black = NULL;
CScene2D *CModeGame::PauseWord = NULL;
CScene2D *CModeGame::CameraSensi = NULL;
CScene2D *CModeGame::SensiBar = NULL;
CScene2D *CModeGame::SensiPos = NULL;
CScene2D *CModeGame::Plus = NULL;
CScene2D *CModeGame::Minus = NULL;
CScene2D *CModeGame::CameraReverse = NULL;
CSound *CModeGame::BGM = NULL;
CSound *CModeGame::GameEnd_SE = NULL;
CSound *CModeGame::ZombieVoice = NULL;
GAME_RESULT CModeGame::Result = GAME_OVER;
int CModeGame::GameCount = 0;
int CModeGame::FrameCount = 0;
int CModeGame::EnemyCount = 0;
int CModeGame::KillCount = 0;
float CModeGame::SensiBarValue = 0;
CFog *CModeGame::Fog = NULL;
CField *CModeGame::Field = NULL;
CEnemyManager *CModeGame::EnemyManager = NULL;
CTargetCapsule *CModeGame::Target = NULL;
CEventStartEnemy *CModeGame::StartEnemy = NULL;

float g_FogColor[4];
float g_LightDiff[4];
float g_LightAmb[4];
float g_Density = 0.07f;

CTargetCapsule* tc;

void CModeGame::Init()
{
	// テクスチャの初期化
	CTexture::Init();

	// 暗転用のテクスチャ
	Black = CScene2D::Create(TEX_ID_WHITE, SCREEN_WIDTH, SCREEN_HEIGHT, LAYER_OBJECT2D_AFTER);
	Black->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));
	Black->SetColor(D3DCOLOR_RGBA(0, 0, 0, 230));
	Black->SetVisible(false);

	// ポーズの準備
	PauseWord = CScene2D::Create(TEX_ID_PAUSE, 172.0f, 97.0f, LAYER_OBJECT2D_AFTER);
	PauseWord->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 200.0f, 0.0f));
	PauseWord->SetVisible(false);

	CameraSensi = CScene2D::Create(TEX_ID_CAMERA_SENSI, 320.0f, 70.0f, LAYER_OBJECT2D_AFTER);
	CameraSensi->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	CameraSensi->SetVisible(false);

	CameraReverse = CScene2D::Create(TEX_ID_CAMERA_REVERSE, 320.0f, 70.0f, LAYER_OBJECT2D_AFTER);
	CameraReverse->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 60.0f, 0.0f));
	CameraReverse->SetVisible(false);

	SensiBar = CScene2D::Create(TEX_ID_WHITE, BAR_LENGTH, 10.0f, LAYER_OBJECT2D_AFTER);
	SensiBar->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50.0f, 0));
	SensiBar->SetColor(D3DCOLOR_RGBA(128, 128, 128, 255));
	SensiBar->SetVisible(false);

	SensiPos = CScene2D::Create(TEX_ID_WHITE, 10.0f, 20.0f, LAYER_OBJECT2D_AFTER);
	SensiPos->SetVisible(false);

	Plus = CScene2D::Create(TEX_ID_PLUS, 20.0f, 20.0f, LAYER_OBJECT2D_AFTER);
	Plus->Set(D3DXVECTOR3(SCREEN_WIDTH / 2 + BAR_LENGTH / 2 + 30.0f, SCREEN_HEIGHT / 2 + 50.0f, 0));
	Plus->SetVisible(false);

	Minus = CScene2D::Create(TEX_ID_MINUS, 20.0f, 20.0f, LAYER_OBJECT2D_AFTER);
	Minus->Set(D3DXVECTOR3(SCREEN_WIDTH / 2 - BAR_LENGTH / 2 - 30.0f, SCREEN_HEIGHT / 2 + 50.0f, 0));
	Minus->SetVisible(false);

	// 敵の経路を作成
	CWayPoint::Init();
	CWayPoint::Debug();

	// フェード
	CFade::FadeIn();

	// ビルボードの準備
	CBillBoard::Init();

	// フィールド
	Field = CField::Create("data/output/map.txt");

	// プレイヤー
	player = CPlayer::Create(SM_ID_PLAYER, D3DXVECTOR3(-88.0f, 0.0f, -85.0f));
	player->SetField(Field);
	player->ChangePattern(new CPlayerPatternStop());
	CManager::SetCamera(player->GetCamera());

	// 空
	CSkyBox::Create(player);

	EnemyManager = new CEnemyManager(Field);
	
	// ライト
	Light = CLight::Create(0);

	// サウンドの準備
	ZombieVoice = CSound::Create(SOUND_LABEL_BGM_ZOMBIE_BREATH);
	//ZombieVoice->Play(0.01f);
	BGM = CSound::Create(SOUND_LABEL_BGM_GAME);
	//BGM->Play(0.1f);

	// スコア等のリセット
	Result = GAME_OVER;
	GameEnd_SE = NULL;
	Pause = false;
	GameFinish = false;
	isHoldSensiBar = false;
	GameCount = 0;
	KillCount = 0;
	FrameCount = 0;
	EnemyCount = 0;

	Fog->Set(D3DCOLOR_RGBA(18, 18, 36, 255), 0.0f);

	Target = CTargetCapsule::Create(D3DXVECTOR3(34.2f, 0.0f, 62.5f));

	StartEnemy = CEventStartEnemy::Create();

	// プレイヤーカメラの感度取得
	SensiBarValue = player->GetCamera()->GetCameraSensi();
	SensiBarValue = SensiBarValue / CAMERA_SENSI_MAX;
	SensiBarValue = SensiBarValue * BAR_LENGTH;

	SensiPos->Set(D3DXVECTOR3(SCREEN_WIDTH / 2 - BAR_LENGTH / 2 + SensiBarValue, SCREEN_HEIGHT / 2 + 50.0f, 0));
}

void CModeGame::Uninit()
{
	CItem::ReleaseAll();

	CEvent::ReleaseAll();

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
	CController *controller;
	float mouseX, mouseY, mouseZ;

	// キーボード取得
	inputKeyboard = CManager::GetInputKeyboard();

	// マウス取得
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	// コントローラーの取得
	controller = CManager::GetController();

	GameCount++;

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
				if (inputKeyboard->GetKeyTrigger(DIK_SPACE) ||
					inputKeyboard->GetKeyTrigger(DIK_RETURN) ||
					inputMouse->GetLeftTrigger() ||
					controller->ButtonTrigger(XINPUT_GAMEPAD_A) ||
					controller->ButtonTrigger(XINPUT_GAMEPAD_B) ||
					controller->ButtonTrigger(XINPUT_GAMEPAD_START))
				{
					CFade::FadeOut(new CModeResult(KillCount));
				}
			}
		}
		else if (Pause)
		{
			FrameCount++;

			PauseUpdate();
		}
		else
		{
			if (GameCount == 60)
			{
				player->ChangePattern(new CPlayerPatternNormal());
				player->ChangeTutorial(new CTutorialCamera());
			}

			CCharacter::UpdateAll();
			CScene::UpdateAll();
			CBullet::UpdateAll();
			CParticleEmitter::UpdateAll();
			CParticle::UpdateAll();
			CBillBoard::UpdateAll();
			CWeapon::UpdateAll();
			EnemyManager->Update();
			CItem::UpdateAll();
			CEvent::UpdateAll();

			if (inputKeyboard->GetKeyTrigger(DIK_P) ||
				inputKeyboard->GetKeyTrigger(DIK_TAB) ||
				controller->ButtonTrigger(XINPUT_GAMEPAD_START))
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
}

void CModeGame::IncrementKillCount()
{
	KillCount++;
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
		se->Play(0.1f);
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
		se->Play(0.1f);
	}
}

void CModeGame::CallPause()
{
	Pause = !Pause;
	Black->SetVisible(Pause);
	PauseWord->SetVisible(Pause);
	CameraSensi->SetVisible(Pause);
	SensiBar->SetVisible(Pause);
	SensiPos->SetVisible(Pause);
	Plus->SetVisible(Pause);
	Minus->SetVisible(Pause);

	CInput::ChangeShowCursol(Pause);
}

void CModeGame::PauseUpdate()
{
	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	CController *controller;
	float mouseX, mouseY, mouseZ;

	// キーボード取得
	inputKeyboard = CManager::GetInputKeyboard();

	// マウス取得
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	// コントローラーの取得
	controller = CManager::GetController();

	if (FrameCount / 256 % 2 == 0.0f)
	{
		PauseWord->SetColor(D3DCOLOR_RGBA(255, 255, 255, FrameCount % 256));
	}
	else
	{
		PauseWord->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255 - (FrameCount % 256)));
	}

	D3DXVECTOR3 bar = SensiPos->GetPos();

	D3DXVECTOR2 mousepos = D3DXVECTOR2(inputMouse->GetPosWorld()->x - 128.0f, inputMouse->GetPosWorld()->y - 85.0f);

	if (inputMouse->GetLeftTrigger())
	{
		if (isCollisionSquareToPoint2D(
			D3DXVECTOR2(bar.x - 5.0f, bar.y + 10.0f),
			D3DXVECTOR2(bar.x + 5.0f, bar.y - 10.0f),
			mousepos))
		{
			isHoldSensiBar = true;
		}
	}

	if (inputMouse->GetLeftRelease())
	{
		isHoldSensiBar = false;
	}

	if (isHoldSensiBar)
	{
		// プレイヤーカメラの感度取得
		SensiBarValue = mousepos.x;

		if (SensiBarValue < SCREEN_WIDTH / 2 - BAR_LENGTH / 2 + 0.05f)
		{
			SensiBarValue = SCREEN_WIDTH / 2 - BAR_LENGTH / 2 + 5.0f;
		}
		else if (SensiBarValue > SCREEN_WIDTH / 2 + BAR_LENGTH / 2)
		{
			SensiBarValue = SCREEN_WIDTH / 2 + BAR_LENGTH / 2;
		}

		SensiPos->Set(D3DXVECTOR3(SensiBarValue, SCREEN_HEIGHT / 2 + 50.0f, 0));

		player->GetCamera()->SetCameraSensi(
			(SensiBarValue - (SCREEN_WIDTH / 2 - BAR_LENGTH / 2)) / BAR_LENGTH);
	}

	if (inputKeyboard->GetKeyTrigger(DIK_P) ||
		inputKeyboard->GetKeyTrigger(DIK_TAB) ||
		controller->ButtonTrigger(XINPUT_GAMEPAD_START))
	{
		isHoldSensiBar = false;
		CallPause();
	}
}

CCamera* CModeGame::GetCamera()
{
	return player->GetCamera(); 
}

D3DXVECTOR3 CModeGame::GetTargetPos()
{
	return Target->GetPos();
}
