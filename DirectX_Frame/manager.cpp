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
#include "waypoint.h"

static float CAMERA_SPEED = 0.1f;

//======================================================================
//	�ÓI�����o�ϐ�
//======================================================================
CInputKeyboard	*CManager::m_InputKeyboard = NULL;		// �L�[�{�[�h�ւ̃|�C���^
CInputMouse		*CManager::m_InputMouse = NULL;			// �}�E�X�ւ̃|�C���^
CLight			*CManager::m_Light;
CMode			*CManager::m_Mode = NULL;
CCamera			*CManager::m_UsingCamera = NULL;
CCamera			*CManager::m_DebugCamera = NULL;
CCamera			*CManager::m_TempCamera = NULL;
bool			CManager::m_isDebug = false;
bool			CManager::m_isFogEnable = true;
float			CManager::m_SkinMeshDrawDist = DRAW_DIST;

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

	// �X�L�����b�V���̈ꊇ���[�h
	CSceneSkinMesh::LoadAll();

	// �t�F�[�h����
	CFade::Init();

	CManager::m_Mode = new CModeMapMake();
	//CManager::m_Mode = new CModeGame();
	//CManager::m_Mode = new CModeTitle();

	// �f�o�b�O�J�����̃Z�b�g
	m_DebugCamera = CCamera::Create();

	if (CManager::m_Mode != NULL)
	{
		CManager::m_Mode->Init();
	}

	// �X�L�����b�V�����f���̕`�拗��
	m_SkinMeshDrawDist = DRAW_DIST;

	return true;
}

void CManager::Uninit()
{
	// �X�L�����b�V���t�@�C���̈ꊇ�����[�X
	CSceneSkinMesh::ReleaseFileAll();

	CFade::Uninit();

	CManager::m_Mode->Uninit();
	delete CManager::m_Mode;

	// �f�o�b�O�J�����̉��
	m_DebugCamera->Release();

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

	if (m_InputKeyboard->GetKeyTrigger(DIK_AT))
	{
		ChangeDebugMode();
	}

	m_Mode->Update();

	// �f�o�b�O���̓f�o�b�O�J������L����
	if (m_isDebug)
	{
		DebugCameraControll();
	}

	m_UsingCamera->Update();
}

void CManager::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	HRESULT hr;

	hr = CRenderer::DrawBegin();

	//Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(hr))
	{
		//�e��s��̐ݒ�
		pDevice->SetTransform(D3DTS_VIEW, &m_UsingCamera->GetView());
		pDevice->SetTransform(D3DTS_PROJECTION, &m_UsingCamera->GetProjection());

		pDevice->SetRenderState(D3DRS_FOGENABLE, m_isFogEnable); //�t�H�O

		//�`��
		m_Mode->Draw();

		CImGui::EndDraw();

		CFade::Draw();
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

	m_isDebug = false;
	m_TempCamera = NULL;
}

void CManager::ChangeDebugMode()
{
	m_isDebug = !m_isDebug; 

	CWayPoint::Debug();

	if (m_isDebug)
	{
		m_TempCamera = m_UsingCamera;
		m_UsingCamera = m_DebugCamera;
	}
	else
	{
		if (m_TempCamera)
		{
			m_UsingCamera = m_TempCamera;
		}
	}
}

void CManager::DebugCameraControll()
{
	ImGui::Begin("Debug");
	ImGui::Checkbox("FogEnable", &m_isFogEnable);
	ImGui::SliderFloat("CameraSpeed", &CAMERA_SPEED, 0.0f, 5.0f);
	ImGui::End();

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

	// �J��������
	if (inputMouse->GetRightPress())
	{
		D3DXVECTOR3 camerapos = m_DebugCamera->GetPos();
		float moveX = 0.0f, moveY = 0.0f, moveZ = 0.0f;
		if (inputKeyboard->GetKeyPress(DIK_A))
		{
			moveX = -1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_D))
		{
			moveX = 1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_Q))
		{
			moveY = -1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_E))
		{
			moveY = 1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_W))
		{
			moveZ = 1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_S))
		{
			moveZ = -1.0f;
		}

		camerapos = camerapos + m_DebugCamera->GetRight() * moveX * CAMERA_SPEED;
		camerapos = camerapos + m_DebugCamera->GetUp() * moveY * CAMERA_SPEED;
		camerapos = camerapos + m_DebugCamera->GetFront() * moveZ * CAMERA_SPEED;

		m_DebugCamera->Move(camerapos - m_DebugCamera->GetPos());

		// ��]
		m_DebugCamera->Rotation(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);
	}
}
