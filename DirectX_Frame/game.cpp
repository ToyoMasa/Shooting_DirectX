//======================================================================
//	�Q�[����ʊǗ�
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
CLight *CModeGame::Light;
bool CModeGame::Pause = false;
bool CModeGame::GameFinish = false;
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
CScene2D *CModeGame::PauseWord = NULL;
CScene2D *CModeGame::HowToUse = NULL;
CSound *CModeGame::BGM = NULL;
CSound *CModeGame::GameEnd_SE = NULL;
CSound *CModeGame::ZombieVoice = NULL;
GAME_RESULT CModeGame::Result = GAME_OVER;
int CModeGame::FrameCount = 0;
int CModeGame::KillCount = 0;
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
	// �e�N�X�`���̏�����
	CTexture::Init();

	// �t�F�[�h�p�̃e�N�X�`��
	Black = CScene2D::Create(TEX_ID_BLACK, SCREEN_WIDTH, SCREEN_HEIGHT);
	Black->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));
	Black->SetColor(D3DCOLOR_RGBA(0, 0, 0, 128));
	Black->SetVisible(false);

	// �|�[�Y�̏���
	PauseWord = CScene2D::Create(TEX_ID_PAUSE, 172.0f, 97.0f);
	PauseWord->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	PauseWord->SetVisible(false);

	// �G�̌o�H���쐬
	CWayPoint::Init();

	// �t�F�[�h
	CFade::FadeIn();

	// �t�B�[���h
	Field = CField::Create("data/output/map.txt");

	// �v���C���[
	player = CPlayer::Create(SM_ID_PLAYER, D3DXVECTOR3(-68.0f, 0.0f, -74.0f));
	player->SetField(Field);
	CManager::SetCamera(player->GetCamera());

	EnemyManager = new CEnemyManager(Field);
	
	// ��
	CSkyBox::Create(player);

	// ���C�g
	Light = CLight::Create(0);

	// �r���{�[�h�̏���
	CBillBoard::Init();

	// �T�E���h�̏���
	ZombieVoice = CSound::Create(SOUND_LABEL_BGM_ZOMBIE_BREATH);
	ZombieVoice->Play(0.01f);
	BGM = CSound::Create(SOUND_LABEL_BGM_TITLE);
	BGM->Play();

	// �X�R�A���̃��Z�b�g
	Result = GAME_OVER;
	GameEnd_SE = NULL;
	Pause = false;
	GameFinish = false;
	KillCount = 0;
	FrameCount = 0;

	Fog->Set(D3DCOLOR_RGBA(18, 18, 36, 255), 0.1f);

	tc = CTargetCapsule::Create(D3DXVECTOR3(-68.0f, 0.0f, -79.0f));
	//tc = CTargetCapsule::Create(D3DXVECTOR3(34.2f, 0.0f, 62.5f));
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

	Light->Release();

	CBillBoard::Uninit();

	// �S�Ẵe�N�X�`���̉��
	CTexture::ReleaseAll();

	CWayPoint::Uninit();

	delete EnemyManager;
}

void CModeGame::Update()
{
	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	float mouseX, mouseY, mouseZ;

	// �L�[�{�[�h�擾
	inputKeyboard = CManager::GetInputKeyboard();

	// �}�E�X�擾
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	if (!CFade::GetFadeOut())
	{
		if (GameFinish)
		{
			if (inputKeyboard->GetKeyTrigger(DIK_SPACE) || inputKeyboard->GetKeyTrigger(DIK_RETURN) || inputMouse->GetLeftTrigger())
			{
				CFade::FadeOut(new CModeResult(KillCount));
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

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
	CBillBoard::DrawAll();
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON

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
