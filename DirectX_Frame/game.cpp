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
CLight *CModeGame::m_Light;
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
int CModeGame::m_Count = 0;
CFog *CModeGame::Fog = NULL;
CField *CModeGame::Field = NULL;
CEnemyManager *CModeGame::EnemyManager = NULL;

float g_FogColor[4];
float g_LightDiff[4];
float g_LightAmb[4];
float g_Density = 0.07f;

CTargetCapsule* tc;

void CModeGame::Init()
{
	// テクスチャの初期化
	CTexture::Init();

	Black = CScene2D::Create(TEX_ID_BLACK, SCREEN_WIDTH, SCREEN_HEIGHT);
	Black->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));
	Black->SetColor(D3DCOLOR_RGBA(0, 0, 0, 128));
	Black->SetVisible(false);

	// 敵の経路を作成
	CWayPoint::Init();

	// フェード
	CFade::FadeIn();

	// フィールド
	Field = CField::Create("data/output/map.txt");

	// プレイヤー
	player = CPlayer::Create(SM_ID_PLAYER, D3DXVECTOR3(-90.0f, 0.0f, -90.0f));
	player->SetField(Field);
	CManager::SetCamera(player->GetCamera());

	EnemyManager = new CEnemyManager(Field);
	
	// 空
	CSkyBox::Create(player);

	// ライト
	m_Light = CLight::Create(0);

	// ビルボードの準備
	CBillBoard::Init();

	// ポーズの準備
	Pause = CScene2D::Create(TEX_ID_PAUSE, 172.0f, 97.0f);
	Pause->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	Pause->SetVisible(false);

	// スコア等のリセット
	m_PlayBGM = false;
	GameEnd_SE = NULL;

	Fog->Set(D3DCOLOR_RGBA(18, 18, 36, 255), 0.1f);

	tc = CTargetCapsule::Create(D3DXVECTOR3(34.2f, 0.0f, 62.5f));
}

void CModeGame::Uninit()
{
	CItem::ReleaseAll();

	CBullet::ReleaseAll();

	CWeapon::ReleaseAll();

	CCharacter::ReleaseAll();

	CScene::ReleaseAll();

	CSound::ReleaseAll();

	CActionBase::ReleaseAll();

	CParticleEmitter::ReleaseAll();

	CParticle::ReleaseAll();

	m_Light->Release();

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

	CImGui::EndDraw();
}

void CModeGame::CallPause()
{
	if (m_Pause)
	{
		m_Pause = false;
		Black->SetVisible(false);
		Pause->SetVisible(false);
	}
	else
	{
		m_Pause = true;
		Black->SetVisible(true);
		Pause->SetVisible(true);
	}
}

CCamera* CModeGame::GetCamera()
{
	return player->GetCamera(); 
}
