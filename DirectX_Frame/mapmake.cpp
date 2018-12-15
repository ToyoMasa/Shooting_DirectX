//======================================================================
//	マップ作成ツール [mapmake.cpp]　（2018/11/2）
//	Author : 豊村 昌俊
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
#include "SkinMeshFile.h"
#include "camera.h"
#include "light.h"
#include "manager.h"
#include "field.h"
#include "texture.h"
#include "input.h"
#include "mapmake.h"
#include "mathutil.h"
#include "player.h"
#include "box.h"
#include "game.h"
#include "fade.h"
#include "metalShader.h"
#include "waypoint.h"
#include "astar.h"
#include <iostream>
#include <fstream>
#include <string>
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

	// テクスチャの初期化
	CTexture::Init();

	m_Camera = CCamera::Create(D3DXVECTOR3(0.0f, 10.0f, -15.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CManager::SetCamera(m_Camera);

	// ブラシの作成
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
	CWayPoint::Init();

	int i = CWayPoint::SearchShortestPoint(D3DXVECTOR3(88.0f, 0.0f, 22.0f));
	i = 0;
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
		break;
	case WAYPOINT_MAKE:
		WaypointMakeUpdate();
		break;
	case NONE:
		break;
	default:
		break;
	}

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
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ

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
	// 高さを変更
	// 以前のマップを解放
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

	// マップのサイズを更新
	m_NumBlock = m_NextNumBlock;

	// 新しいマップを作成
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

	// キーボード取得
	inputKeyboard = CManager::GetInputKeyboard();

	// マウス取得
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	// カメラ操作
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

		// 回転
		m_Camera->Rotation(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);
	}

	// マップ操作
	D3DXVECTOR3 mouse;

	mouse.x = inputMouse->GetPosWorld()->x - 128.0f;
	mouse.y = inputMouse->GetPosWorld()->y - 85.0f;
	mouse.z = 1.0f;

	// ブラシの位置を更新
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

				// スクリーン座標に変換
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

	// フィールドを更新
	m_Field->SetVertex(vertex);
	m_Field->Change();

	// マップ設定
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

	// マップツールボックス
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

	// キーボード取得
	inputKeyboard = CManager::GetInputKeyboard();

	// マウス取得
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	// カメラ操作
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

		// 回転
		m_Camera->Rotation(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);
	}

	// マップ操作
	D3DXVECTOR3 mouse;

	mouse.x = inputMouse->GetPosWorld()->x - 128.0f;
	mouse.y = inputMouse->GetPosWorld()->y - 85.0f;
	mouse.z = 1.0f;

	VERTEX_3D* vertex = m_Field->GetVertex();

	// ウェイポイントの設置
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

					// スクリーン座標に変換
					WorldToScreen(pDevice, CManager::GetCamera()->GetView(), CManager::GetCamera()->GetProjection(), vtx, screen);

					if (isCollisionCircle2D(mouse.x, mouse.y, 10.0f, screen.x, screen.y, 1.0f))
					{
						D3DXVECTOR3 vtx = vertex[j + m_NumBlock * i].pos;
						vtx.y = 0;

						// ウェイポイントを追加
						CWayPoint::Add(vtx);

						break;
					}
				}
			}
			break;

		case WAYPOINT_CURSOL_ADD_NEARPOINT:
			// 選択中のウェイポイントが有効なら
			if (m_SelectWayPoint != -1)
			{
				int addPoint = CWayPoint::GetIDonScreen(mouse);
				if (addPoint != -1)
				{
					CWayPoint::AddNearPoint(m_SelectWayPoint, addPoint);
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

	// ウェイポイントツールボックス
	ImGui::Begin("WaypointConfig", 0);
	ImGui::RadioButton("ADD WAYPOINT", &m_WaypointCursolType, WAYPOINT_CURSOL_ADD_WAYPOINT);
	ImGui::RadioButton("ADD NEARPOINT", &m_WaypointCursolType, WAYPOINT_CURSOL_ADD_NEARPOINT);
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

		// 初期化
		CWayPoint::Uninit();

		CWayPoint::Load(filename);
	}
	ImGui::End();

}
