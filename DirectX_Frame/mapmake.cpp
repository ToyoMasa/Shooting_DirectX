//======================================================================
//	�}�b�v�쐬�c�[�� [mapmake.cpp]�@�i2018/11/2�j
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "mode.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "sceneSkinMesh.h"
#include "billboard.h"
#include "skinMeshFile.h"
#include "camera.h"
#include "light.h"
#include "manager.h"
#include "field.h"
#include "texture.h"
#include "input.h"
#include "mapmake.h"
#include "mathutil.h"
#include "player.h"
#include "game.h"
#include "fade.h"
#include "metalShader.h"
#include "waypoint.h"
#include "astar.h"
#include <iostream>
#include <fstream>
#include <string>
#include "fog.h"
using namespace std;

static float CAMERA_SPEED = 0.1f;

enum BRUSH_TYPE
{
	BRUSH_NORMAL,
	BRUSH_FLAT_HIGH,
	BRUSH_FLAT_LOW,
	BRUSH_FLAT_RESET,
	BRUSH_COLOR,
};

enum WAYPOINT_CURSOL_TYPE
{
	WAYPOINT_CURSOL_ADD_WAYPOINT,
	WAYPOINT_CURSOL_ADD_NEARPOINT,
	WAYPOINT_CURSOL_ADD_SPAWNPOINT,
};

CCamera	*CModeMapMake::m_Camera = NULL;

void CModeMapMake::Init()
{
	m_WaypointCursolType = WAYPOINT_CURSOL_ADD_WAYPOINT;
	m_SelectMode = MAP_MAKE;
	m_SelectWayPoint = -1;
	strcpy_s(m_FileName, "map.txt");
	m_VertexMode = false;
	m_Wire = false;

	// �e�N�X�`���̏�����
	CTexture::Init();

	m_Camera = CCamera::Create(D3DXVECTOR3(0.0f, 10.0f, -15.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CManager::SetCamera(m_Camera);

	// �u���V�̍쐬
	m_BrushType = BRUSH_NORMAL;
	m_BrushColor = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_BrushRange = 10.0f;
	m_BrushValue = 1.0f;
	m_Brush = CScene2D::Create(TEX_ID_CIRCLE_2D, m_BrushRange, m_BrushRange);
	m_Brush->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	m_Brush->SetColor(D3DCOLOR_RGBA(0, 255, 0, 255));

	m_NumBlock = 10;
	m_NextNumBlock = 10;
	m_Size = 1.0f;

	m_Light = CLight::Create(0);

	m_Field = CField::Create("data/output/map.txt");

	m_NumBlock = m_Field->GetBlockSize();

	m_Heights = new float*[m_NumBlock];
	m_isHit = new DWORD*[m_NumBlock];
	for (int i = 0; i < m_NumBlock; i++)
	{
		m_Heights[i] = new float[m_NumBlock];
		m_isHit[i] = new DWORD[m_NumBlock];
	}

	for (int i = 0; i < m_NumBlock; i++)
	{
		for (int j = 0; j < m_NumBlock; j++)
		{
			m_Heights[i][j] = 0.0f;
			m_isHit[i][j] = false;
		}
	}

	CBillBoard::Init();

	// �E�F�C�|�C���g�̓ǂݍ���
	//CWayPoint::Init();
	VERTEX_3D* vertex = m_Field->GetVertex();

	for (int i = 0; i < m_NumBlock; i++)
	{
		if (i % 2 == 0)
		{
			continue;
		}

		for (int j = 0; j < m_NumBlock; j++)
		{
			if (j % 2 == 0)
			{
				continue;
			}

			D3DXVECTOR3 vtx = vertex[j + m_NumBlock * i].pos;

			if (vtx.y == 0.0f)
			{
				CWayPoint::Add(vtx);
			}
		}
	}

	std::vector<CWayPoint*> waypoints = CWayPoint::GetWayPoints();
	for (unsigned int i = 0; i < waypoints.size(); i++)
	{
		for (unsigned int j = 0; j < waypoints.size(); j++)
		{
			D3DXVECTOR3 vec = waypoints[i]->GetPos() - waypoints[j]->GetPos();
			float len = D3DXVec3Length(&vec);
			if (len > 3.0f)
			{
				continue;
			}

			CWayPoint::AddNearPoint(i, j);
		}
	}

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	CFog fog;
	fog.Set(D3DCOLOR_RGBA(18, 18, 36, 255), 0.0f);

	CWayPoint::Debug(false);
	CInput::ChangeShowCursol(TRUE);
}

void CModeMapMake::Uninit()
{
	m_Camera->Release();
	m_Field->Release();
	m_Light->Release();

	for (int i = 0; i < m_NumBlock; i++)
	{
		delete[] m_Heights[i];
		delete[] m_isHit[i];
	}
	delete[] m_Heights;
	delete[] m_isHit;

	CScene::ReleaseAll();
	CBillBoard::ReleaseAll();
	CBillBoard::Uninit();

	CWayPoint::Uninit();
}

void CModeMapMake::Update()
{
	switch (m_SelectMode)
	{
	case MAP_MAKE:
		MapMakeUpdate();
		CWayPoint::Debug(false);
		break;
	case WAYPOINT_MAKE:
		WaypointMakeUpdate();
		CWayPoint::Debug(true);
		break;
	case NONE:
		break;
	default:
		break;
	}

	ImGui::SetNextWindowSize(ImVec2(120, 80), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(20.0f, 300), ImGuiSetCond_Once);
	ImGui::Begin("MakingMode", 0);
	ImGui::RadioButton("MAP", &m_SelectMode, MAP_MAKE);
	ImGui::RadioButton("WAYPOINT", &m_SelectMode, WAYPOINT_MAKE);
	ImGui::End();
}

void CModeMapMake::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O

	if (m_Wire)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	CScene::DrawAll();
	CBillBoard::DrawAll();

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_Brush->Draw();
}

void CModeMapMake::ChangeMap()
{
	// ������ύX
	// �ȑO�̃}�b�v�����
	for (int i = 0; i < m_NumBlock; i++)
	{
		delete[] m_Heights[i];
		delete[] m_isHit[i];
		m_Heights[i] = NULL;
		m_isHit[i] = NULL;
	}
	delete[] m_Heights;
	delete[] m_isHit;
	m_Heights = NULL;
	m_isHit = NULL;

	// �}�b�v�̃T�C�Y���X�V
	m_NumBlock = m_NextNumBlock;

	// �V�����}�b�v���쐬
	m_Heights = new float*[m_NumBlock];
	m_isHit = new DWORD*[m_NumBlock];
	for (int i = 0; i < m_NumBlock; i++)
	{
		m_Heights[i] = new float[m_NumBlock];
		m_isHit[i] = new DWORD[m_NumBlock];
	}

	for (int i = 0; i < m_NumBlock; i++)
	{
		for (int j = 0; j < m_NumBlock; j++)
		{
			m_Heights[i][j] = 0.0f;
			m_isHit[i][j] = false;
		}
	}

	m_Field->Release();
	m_Field = NULL;

	m_Field = CField::Create(TEX_ID_FIELD001, m_Size, m_NumBlock, m_NumBlock, m_Heights);
}

void CModeMapMake::MapMakeUpdate()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

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
	if (inputMouse->GetCenterPress())
	{
		D3DXVECTOR3 camerapos = m_Camera->GetPos();
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

		camerapos = camerapos + m_Camera->GetRight() * moveX * CAMERA_SPEED;
		camerapos = camerapos + m_Camera->GetUp() * moveY * CAMERA_SPEED;
		camerapos = camerapos + m_Camera->GetFront() * moveZ * CAMERA_SPEED;

		m_Camera->Move(camerapos - m_Camera->GetPos());

		// ��]
		m_Camera->Rotation(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);
	}

	// �}�b�v����
	D3DXVECTOR3 mouse;

	mouse.x = inputMouse->GetPosWorld()->x - 128.0f;
	mouse.y = inputMouse->GetPosWorld()->y - 85.0f;
	mouse.z = 1.0f;

	// �u���V�̈ʒu���X�V
	m_Brush->Set(mouse);
	m_Brush->SetSize(D3DXVECTOR2(m_BrushRange, m_BrushRange));

	VERTEX_3D* vertex = m_Field->GetVertex();

	if (inputMouse->GetRightPress())
	{
		for (int i = 0; i < m_NumBlock; i++)
		{
			for (int j = 0; j < m_NumBlock; j++)
			{
				D3DXVECTOR3 screen;
				D3DXVECTOR3 vtx = vertex[j + m_NumBlock * i].pos;

				if (!m_VertexMode)
				{
					vtx.y = 0.0f;
				}

				// �X�N���[�����W�ɕϊ�
				WorldToScreen(pDevice, CManager::GetCamera()->GetView(), CManager::GetCamera()->GetProjection(), vtx, screen);

				if (isCollisionCircle2D(mouse.x, mouse.y, m_BrushRange / 2.0f, screen.x, screen.y, 1.0f))
				{
					if (timeGetTime() - m_isHit[i][j] > 300.0)
					{
						switch (m_BrushType)
						{
						case BRUSH_NORMAL:
							vertex[j + m_NumBlock * i].pos.y += m_BrushValue;
							break;
						case BRUSH_FLAT_HIGH:
							if (i != 0 && (i + 1) < m_NumBlock)
							{
								if (vertex[j + m_NumBlock * i].pos.y < vertex[j + m_NumBlock * i - 1].pos.y)
								{
									vertex[j + m_NumBlock * i].pos.y = vertex[j + m_NumBlock * i - 1].pos.y;
								}
								if (vertex[j + m_NumBlock * i].pos.y < vertex[j + m_NumBlock * i + 1].pos.y)
								{
									vertex[j + m_NumBlock * i].pos.y = vertex[j + m_NumBlock * i + 1].pos.y;
								}
								if (vertex[j + m_NumBlock * i].pos.y < vertex[j + m_NumBlock * (i + 1)].pos.y)
								{
									vertex[j + m_NumBlock * i].pos.y = vertex[j + m_NumBlock * (i + 1)].pos.y;
								}
								if (vertex[j + m_NumBlock * i].pos.y < vertex[j + m_NumBlock * (i - 1)].pos.y)
								{
									vertex[j + m_NumBlock * i].pos.y = vertex[j + m_NumBlock * (i - 1)].pos.y;
								}
							}
							break;
						case BRUSH_FLAT_LOW:
							if (i != 0 && (i + 1) < m_NumBlock)
							{
								if (vertex[j + m_NumBlock * i].pos.y > vertex[j + m_NumBlock * i - 1].pos.y)
								{
									vertex[j + m_NumBlock * i].pos.y = vertex[j + m_NumBlock * i - 1].pos.y;
								}
								if (vertex[j + m_NumBlock * i].pos.y > vertex[j + m_NumBlock * i + 1].pos.y)
								{
									vertex[j + m_NumBlock * i].pos.y = vertex[j + m_NumBlock * i + 1].pos.y;
								}
								if (vertex[j + m_NumBlock * i].pos.y > vertex[j + m_NumBlock * (i + 1)].pos.y)
								{
									vertex[j + m_NumBlock * i].pos.y = vertex[j + m_NumBlock * (i + 1)].pos.y;
								}
								if (vertex[j + m_NumBlock * i].pos.y > vertex[j + m_NumBlock * (i - 1)].pos.y)
								{
									vertex[j + m_NumBlock * i].pos.y = vertex[j + m_NumBlock * (i - 1)].pos.y;
								}
							}
							break;
						case BRUSH_FLAT_RESET:
							vertex[j + m_NumBlock * i].pos.y = 0.0f;
							break;
						case BRUSH_COLOR:
							vertex[j + m_NumBlock * i].color = m_BrushColor;
							break;
						default:
							break;
						}
						m_isHit[i][j] = timeGetTime();
					}
				}
				else
				{
					m_isHit[i][j] = (DWORD)0.0;
				}
			}
		}
	}

	// �t�B�[���h���X�V
	m_Field->SetVertex(vertex);
	m_Field->Change();

	// �}�b�v�ݒ�
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(20.0f, 20), ImGuiSetCond_Once);
	ImGui::Begin("MapConfig", 0);
	ImGui::Checkbox("isWireframe", &m_Wire);
	ImGui::SliderInt("NumBlock", &m_NextNumBlock, 1, 1000);
	if (ImGui::Button("Reload"))
	{
		ChangeMap();
	}
	ImGui::InputText("FileName", m_FileName, sizeof(m_FileName));
	if (ImGui::Button("Save"))
	{
		char filename[256];
		sprintf_s(filename, "data/output/%s", m_FileName);
		m_Field->Save(filename);
	}
	if (ImGui::Button("Load"))
	{
		char filename[256];
		sprintf_s(filename, "data/output/%s", m_FileName);
		m_Field->Load(filename);
	}
	if (ImGui::Button("Play"))
	{
		CManager::SetMode(new CModeGame());
	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 320.0f, 20), ImGuiSetCond_Once);
	// �}�b�v�c�[���{�b�N�X
	ImGui::Begin("BrushConfig", 0);
	ImGui::Text("BrushType : %d", m_BrushType);
	ImGui::Checkbox("Vertex", &m_VertexMode);
	ImGui::RadioButton("HEIGHT", &m_BrushType, BRUSH_NORMAL);
	ImGui::RadioButton("FLAT_H", &m_BrushType, BRUSH_FLAT_HIGH);
	ImGui::RadioButton("FLAT_L", &m_BrushType, BRUSH_FLAT_LOW);
	ImGui::RadioButton("FLAT_R", &m_BrushType, BRUSH_FLAT_RESET);
	ImGui::RadioButton("COLOR", &m_BrushType, BRUSH_COLOR);
	ImGui::SliderFloat("BrashSize", &m_BrushRange, 1.0f, 1000.0f);
	ImGui::SliderFloat("BrashValue", &m_BrushValue, -10.0f, 10.0f);
	ImGui::SliderFloat("CameraSpeed", &CAMERA_SPEED, 0.0f, 5.0f);
	ImGui::ColorEdit4("BrushColor", m_TempColor);
	ImGui::Text("%.2f, %.2f, %.2f, %.2f", m_TempColor[0], m_TempColor[1], m_TempColor[2], m_TempColor[3]);
	ImGui::End();

	m_BrushColor = D3DCOLOR_RGBA((int)(m_TempColor[0] * 255), (int)(m_TempColor[1] * 255), (int)(m_TempColor[2] * 255), (int)(m_TempColor[3] * 255));
}

void CModeMapMake::WaypointMakeUpdate()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

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
	if (inputMouse->GetCenterPress())
	{
		D3DXVECTOR3 camerapos = m_Camera->GetPos();
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

		camerapos = camerapos + m_Camera->GetRight() * moveX * CAMERA_SPEED;
		camerapos = camerapos + m_Camera->GetUp() * moveY * CAMERA_SPEED;
		camerapos = camerapos + m_Camera->GetFront() * moveZ * CAMERA_SPEED;

		m_Camera->Move(camerapos - m_Camera->GetPos());

		// ��]
		m_Camera->Rotation(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);
	}

	// �}�b�v����
	D3DXVECTOR3 mouse;

	mouse.x = inputMouse->GetPosWorld()->x - 128.0f;
	mouse.y = inputMouse->GetPosWorld()->y - 85.0f;
	mouse.z = 1.0f;

	VERTEX_3D* vertex = m_Field->GetVertex();

	// �E�F�C�|�C���g�̐ݒu
	if (inputMouse->GetRightTrigger())
	{
		switch (m_WaypointCursolType)
		{
		case WAYPOINT_CURSOL_ADD_WAYPOINT:
			for (int i = 0; i < m_NumBlock; i++)
			{
				for (int j = 0; j < m_NumBlock; j++)
				{
					D3DXVECTOR3 screen;
					D3DXVECTOR3 vtx = vertex[j + m_NumBlock * i].pos;

					if (!m_VertexMode)
					{
						vtx.y = 0.0f;
					}

					// �X�N���[�����W�ɕϊ�
					WorldToScreen(pDevice, CManager::GetCamera()->GetView(), CManager::GetCamera()->GetProjection(), vtx, screen);

					if (isCollisionCircle2D(mouse.x, mouse.y, 10.0f, screen.x, screen.y, 1.0f))
					{
						D3DXVECTOR3 vtx = vertex[j + m_NumBlock * i].pos;
						vtx.y = 0;

						// �E�F�C�|�C���g��ǉ�
						CWayPoint::Add(vtx);

						break;
					}
				}
			}
			break;

		case WAYPOINT_CURSOL_ADD_NEARPOINT:
			// �I�𒆂̃E�F�C�|�C���g���L���Ȃ�
			if (m_SelectWayPoint != -1)
			{
				int addPoint = CWayPoint::GetIDonScreen(mouse);
				if (addPoint != -1)
				{
					CWayPoint::AddNearPoint(m_SelectWayPoint, addPoint);
				}
			}
			break;

		case WAYPOINT_CURSOL_ADD_SPAWNPOINT:
			// �I�𒆂̃E�F�C�|�C���g���L���Ȃ�
			if (m_SelectWayPoint != -1)
			{
				int addPoint = CWayPoint::GetIDonScreen(mouse);
				if (addPoint != -1)
				{
					CWayPoint::AddSpawnPoint(m_SelectWayPoint, addPoint);
				}
			}
			break;

		default:
			break;
		}
	}
	if (inputMouse->GetLeftTrigger())
	{
		m_SelectWayPoint = CWayPoint::GetIDonScreen(mouse);
		if (m_SelectWayPoint != -1)
		{
			CWayPoint::SetSelectDebugColor(m_SelectWayPoint);
		}
	}

	// �E�F�C�|�C���g�c�[���{�b�N�X
	ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(20.0f, 20), ImGuiSetCond_Once);
	ImGui::Begin("WaypointConfig", 0);
	ImGui::RadioButton("ADD WAYPOINT", &m_WaypointCursolType, WAYPOINT_CURSOL_ADD_WAYPOINT);
	ImGui::RadioButton("ADD NEARPOINT", &m_WaypointCursolType, WAYPOINT_CURSOL_ADD_NEARPOINT);
	ImGui::RadioButton("ADD SPAWNPOINT", &m_WaypointCursolType, WAYPOINT_CURSOL_ADD_SPAWNPOINT);
	if (ImGui::Button("Save"))
	{
		char filename[256];
		sprintf_s(filename, "data/output/waypoints.txt");
		CWayPoint::Save(filename);
	}
	if (ImGui::Button("Load"))
	{
		char filename[256];
		sprintf_s(filename, "data/output/waypoints.txt");

		// ������
		CWayPoint::Uninit();

		CWayPoint::Load(filename);
	}
	ImGui::End();

}
