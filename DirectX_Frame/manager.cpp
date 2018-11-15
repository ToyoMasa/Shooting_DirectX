//======================================================================
//	�Q�[���Ǘ�
//
//======================================================================
#include "common.h"
#include "main.h"
#include "mode.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
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
#include "title.h"
#include "game.h"
#include "mapmake.h"
#include "fade.h"
#include "sound.h"
#include "shader.h"

//======================================================================
//	�ÓI�����o�ϐ�
//======================================================================
CInputKeyboard *CManager::m_InputKeyboard = NULL;		// �L�[�{�[�h�ւ̃|�C���^
CInputMouse *CManager::m_InputMouse = NULL;			// �}�E�X�ւ̃|�C���^
CLight		*CManager::m_Light;
CMode		*CManager::m_Mode = NULL;
CCamera		*CManager::m_UsingCamera = NULL;

bool CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	if (!CRenderer::Init(hWnd, bWindow))
	{
		return false;
	}

	CSound::Init();

	// imgui�̏������Ɛݒ�
	CImGui::Init(hWnd, CRenderer::GetDevice());
	ImGui::StyleColorsClassic();

	// �L�[�{�[�h����������
	m_InputKeyboard = new CInputKeyboard;
	m_InputKeyboard->Init(hInstance, hWnd);

	// �}�E�X����������
	m_InputMouse = new CInputMouse;
	m_InputMouse->Init(hInstance, hWnd);

	CManager::m_Mode = new CModeMapMake();
	//CManager::m_Mode = new CModeGame();

	if (CManager::m_Mode != NULL)
	{
		CManager::m_Mode->Init();
	}

	CFade::Init();

	return true;
}

void CManager::Uninit()
{
	CFade::Uninit();

	CManager::m_Mode->Uninit();
	delete CManager::m_Mode;

	// �L�[�{�[�h�̊J������
	if (m_InputKeyboard)
	{
		m_InputKeyboard->Uninit();
		delete m_InputKeyboard;
		m_InputKeyboard = NULL;
	}

	// �}�E�X�̊J������
	if (m_InputMouse)
	{
		m_InputMouse->Uninit();
		delete m_InputMouse;
		m_InputMouse = NULL;
	}

	// �T�E���h�����
	CSound::Uninit();

	// �g�����V�F�[�_�[�����
	CShader::ReleaseAll();

	// imgui�̏I������
	CImGui::Uninit();

	// �����_���[�N���X�̏I������
	CRenderer::Uninit();
}

void CManager::Update()
{
	CImGui::BeginDraw();

	CFade::Update();

	// �L�[�{�[�h�X�V����
	if (m_InputKeyboard)
	{
		m_InputKeyboard->Update();
	}

	// �}�E�X�X�V����
	if (m_InputMouse)
	{
		m_InputMouse->Update();
	}

	m_Mode->Update();

	m_UsingCamera->Update();
}

void CManager::Draw()
{
	HRESULT hr;

	hr = CRenderer::DrawBegin();

	//Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(hr))
	{
		//�`��
		m_Mode->Draw();

		//CFade::Draw();
	}

	CRenderer::DrawEnd();
}

void CManager::SetMode(CMode* mode)
{
	if (CManager::m_Mode != NULL)
	{
		CManager::m_Mode->Uninit();

		delete CManager::m_Mode;
		CManager::m_Mode = NULL;
	}

	CManager::m_Mode = mode;

	if (CManager::m_Mode != NULL)
	{
		CManager::m_Mode->Init();
	}
}

