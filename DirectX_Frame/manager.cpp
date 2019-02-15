//======================================================================
//	�Q�[���Ǘ�
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
#include "title.h"
#include "game.h"
#include "mapmake.h"
#include "fade.h"
#include "sound.h"
#include "shader.h"
#include "waypoint.h"

static const int TEXMAP_SIZE = 2048;
static float CAMERA_SPEED = 0.1f;

//======================================================================
//	�ÓI�����o�ϐ�
//======================================================================
CInputKeyboard			*CManager::m_InputKeyboard = NULL;		// �L�[�{�[�h�ւ̃|�C���^
CInputMouse				*CManager::m_InputMouse = NULL;			// �}�E�X�ւ̃|�C���^
CController				*CManager::m_Controller = NULL;
CLight					*CManager::m_Light;
CMode					*CManager::m_Mode = NULL;
CCamera					*CManager::m_UsingCamera = NULL;
CCamera					*CManager::m_DebugCamera = NULL;
CCamera					*CManager::m_TempCamera = NULL;
bool					CManager::m_isDebug = false;
bool					CManager::m_isFogEnable = true;
float					CManager::m_SkinMeshDrawDist = DRAW_DIST;
LPDIRECT3DTEXTURE9		CManager::m_ShadowTex = nullptr;				// ���e�V���h�E�e�N�X�`��
LPDIRECT3DSURFACE9		CManager::m_ShadowSurface = nullptr;			// ���e�V���h�E�e�N�X�`���T�[�t�F�X	
LPDIRECT3DSURFACE9		CManager::m_ShadowZbufferSurface = nullptr;		// ���e�e�N�X�`���փ����_�����O����ۂ̂y�o�b�t�@�i�[�x�o�b�t�@�j
D3DXMATRIX				CManager::m_LightCamera;
D3DXMATRIX				CManager::m_LightProjection;

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

	m_Controller = new CController;
	m_Controller->Init();

	// �X�L�����b�V���̈ꊇ���[�h
	CSceneSkinMesh::LoadAll();

	// �t�F�[�h����
	CFade::Init();

	//CManager::m_Mode = new CModeMapMake();
	//CManager::m_Mode = new CModeGame();
	CManager::m_Mode = new CModeTitle();

	// �f�o�b�O�J�����̃Z�b�g
	m_DebugCamera = CCamera::Create();

	if (CManager::m_Mode != NULL)
	{
		CManager::m_Mode->Init();
	}

	// �X�L�����b�V�����f���̕`�拗��
	m_SkinMeshDrawDist = DRAW_DIST;

	// ���e�e�N�X�`���I�u�W�F�N�g(�����_�[�^�[�Q�b�g�y�т���ɍ��킹���y�o�b�t�@)�̍쐬
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return false;
	}

	HRESULT	hr;
	// �e�N�X�`�����쐬����
	hr = pDevice->CreateTexture(
		TEXMAP_SIZE, TEXMAP_SIZE, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_ShadowTex,
		nullptr);

	if (FAILED(hr))
	{
		MessageBox(nullptr, "ERROR!!", "�e�N�X�`��(�����_�[�^�[�Q�b�g)�����G���[", MB_OK);
		return false;
	}

	hr = m_ShadowTex->GetSurfaceLevel(0, &m_ShadowSurface);
	if (FAILED(hr)) {
		MessageBox(nullptr, "ERROR!!", "�T�[�t�F�X(�����_�[�^�[�Q�b�g)�擾�G���[", MB_OK);
		return false;
	}

	// �����_�[�^�[�Q�b�g�ɍ��킹���y�o�b�t�@���쐬
	hr = pDevice->CreateDepthStencilSurface(TEXMAP_SIZE, TEXMAP_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_ShadowZbufferSurface, nullptr);
	if (FAILED(hr)) {
		MessageBox(nullptr, "ERROR!!", "�y�o�b�t�@�����G���[", MB_OK);
		return false;
	}

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

	// �R���g���[���[�̉������
	if (m_Controller)
	{
		m_Controller->Uninit();
		delete m_Controller;
		m_Controller = NULL;
	}

	// �T�E���h�����
	CSound::Uninit();

	// �g�����V�F�[�_�[�����
	CShader::ReleaseAll();

	// imgui�̏I������
	CImGui::Uninit();

	if (m_ShadowTex != nullptr) {
		m_ShadowTex->Release();
		m_ShadowTex = nullptr;
	}

	if (m_ShadowSurface != nullptr) {
		m_ShadowSurface->Release();
		m_ShadowSurface = nullptr;
	}

	if (m_ShadowZbufferSurface != nullptr) {
		m_ShadowZbufferSurface->Release();
		m_ShadowZbufferSurface = nullptr;
	}

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

	m_Controller->Update();

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

	// �L�[�{�[�h�擾
	CInputKeyboard *inputKeyboard;
	inputKeyboard = CManager::GetInputKeyboard();

	if (inputKeyboard->GetKeyTrigger(DIK_M))
	{
		CInput::ChangeShowCursol();
	}

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

		DrawShadow();

		pDevice->SetRenderState(D3DRS_FOGENABLE, m_isFogEnable); //�t�H�O

		//�`��
		m_Mode->Draw();

		// �V���h�E�}�b�v�e�N�X�`��
		//DrawQuad(pDevice, 10, 10, 200, 200, m_ShadowTex);

		if (CManager::GetDebug())
		{
			ImGui::Begin("Debug Window");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		CImGui::EndDraw();

		CFade::Draw();
	}

	CRenderer::DrawEnd();
}

void CManager::DrawShadow()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	LPDIRECT3DSURFACE9 oldsurface;
	LPDIRECT3DSURFACE9 oldzbuffer;
	D3DVIEWPORT9	oldviewport;

	// ���݂̃����_�����O�^�[�Q�b�g�A�y�o�b�t�@�A�r���[�|�[�g���擾�i��Ŗ߂����߁j
	GetCurrentRendertarget(oldsurface, oldzbuffer, oldviewport);

	// ���e�V���h�E�쐬�i�����̈ʒu�ɃJ������u���āA�e�N�X�`���Ƀ����_�����O�j
	CreateShadow();

	// �����_�[�^�[�Q�b�g�����ɖ߂�
	SetRenderTarget(oldsurface, oldzbuffer, oldviewport);
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

void CManager::GetCurrentRendertarget(
	LPDIRECT3DSURFACE9& currentsurface,		// ���݂̕`��T�[�t�F�X��ۑ����邽�߂̃T�[�t�F�X
	LPDIRECT3DSURFACE9& currentzbuffer, 	// ���݂̂y�o�b�t�@��ۑ����邽�߂̃T�[�t�F�X
	D3DVIEWPORT9& currentvp) 				// ���݂̃r���[�|�[�g��ۑ����邽�߂̂���
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ���݂̃T�[�t�F�X���擾
	pDevice->GetRenderTarget(0, &currentsurface);

	// ���݂̃r���[�|�[�g���擾
	pDevice->GetViewport(&currentvp);

	// ���݂̂y�o�b�t�@���擾
	pDevice->GetDepthStencilSurface(&currentzbuffer);
}

void CManager::SetRenderTarget(
	LPDIRECT3DSURFACE9 rendersurface,	// �����_�[�^�[�Q�b�g
	LPDIRECT3DSURFACE9 zbuffer,			// �y�o�b�t�@
	D3DVIEWPORT9&		vp) 			// �r���[�|�[�g
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ���e�e�N�X�`���쐬�p�Ƀ����_�����O�^�[�Q�b�g���Z�b�g
	pDevice->SetRenderTarget(0, rendersurface);

	// ���e�e�N�X�`���쐬�p�ɂy�o�b�t�@�Z�b�g
	pDevice->SetDepthStencilSurface(zbuffer);

	// ���e�e�N�X�`���쐬�p�Ɂ@�r���[�|�[�g�Z�b�g
	pDevice->SetViewport(&vp);
}

void CManager::CreateShadow()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// �r���[�|�[�g
	D3DVIEWPORT9 vp = { 0, 0, TEXMAP_SIZE, TEXMAP_SIZE, 0.0f, 1.0f };

	// �����_�[�^�[�Q�b�g�ݒ�
	SetRenderTarget(m_ShadowSurface, m_ShadowZbufferSurface, vp);
	// �^�[�Q�b�g�o�b�t�@�̃N���A�A�y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);

	D3DXVECTOR3	lightPos = m_UsingCamera->GetPos();
	D3DXVECTOR3 at = lightPos;
	D3DXVECTOR3 front = m_UsingCamera->GetFront();
	//lightPos -= front * 5.0f;
	/*
	if (lightPos.y < 0.1f)
	{
		lightPos.y = 0.1f;
	}*/

	front.y = 0;
	D3DXVec3Normalize(&front, &front);
	lightPos -= front * 3.0f;
	at += front * 30.0f;
	at.y = 0;
	D3DXVECTOR3 up(0, 1, 0);
	lightPos.y += 0.5f;

	D3DXMatrixLookAtLH(&m_LightCamera, &lightPos, &at, &up);
	D3DXMatrixPerspectiveFovLH(&m_LightProjection, D3DXToRadian(m_UsingCamera->GetFov()), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.001f, 100.0f);

	// �V���h�E�{�����[���`��
	CScene::DrawShadowAll();
}

void CManager::DrawQuad(LPDIRECT3DDEVICE9 device, float x, float y, float width, float height, LPDIRECT3DTEXTURE9 tex) {

	// ���_�t�H�[�}�b�g�̒�`
	struct	VERTEX {
		float		x, y, z;
		float		rhw;
		float		tu, tv;
	};

	static VERTEX vertex[4];

	vertex[0].x = (float)x;
	vertex[0].y = (float)y;
	vertex[1].x = (float)(vertex[0].x + width);
	vertex[1].y = (float)vertex[0].y;
	vertex[2].x = (float)(vertex[0].x + width);
	vertex[2].y = (float)(vertex[0].y + height);
	vertex[3].x = (float)vertex[0].x;
	vertex[3].y = (float)(vertex[0].y + height);

	vertex[0].rhw = 1;
	vertex[1].rhw = 1;
	vertex[2].rhw = 1;
	vertex[3].rhw = 1;

	// �\���e�N�X�`�����W�Z�b�g
	vertex[0].tu = 0.0f;
	vertex[0].tv = 0.0f;
	vertex[1].tu = 1.0f;
	vertex[1].tv = 0.0f;
	vertex[2].tu = 1.0f;
	vertex[2].tv = 1.0f;
	vertex[3].tu = 0.0f;
	vertex[3].tv = 1.0f;

	// �e�N�X�`�����Z�b�g
	device->SetTexture(0, tex);

	// ���_�t�H�[�}�b�g���Z�b�g
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

	// �S�p�`��`��
	device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		vertex,
		sizeof(struct VERTEX));
}