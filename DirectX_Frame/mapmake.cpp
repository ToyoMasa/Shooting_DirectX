
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
#include "field.h"
#include "texture.h"
#include "input.h"
#include "mapmake.h"
#include "mathutil.h"
#include "player.h"
#include "box.h"
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

CCamera	*CModeMapMake::m_Camera = NULL;
CScene2D* g_Brush;
CLight* g_Light;
CField* g_Field;
float** g_Heights;
DWORD** g_isHit;
int g_NumBlock;
int g_NextNumBlock;
int g_BrushType;
float g_Size;
float g_BrushRange;
float g_BrushValue;
float g_TempColor[4];
DWORD g_BrushColor;
double g_TestText;
char g_FileName[256] = "map.txt";
bool g_VertexMode = false;

static CSceneModel* testufo;
static CBox* testbox;
static Shader* testshader;

bool g_Wire = false;

void CModeMapMake::Init()
{
	// テクスチャの初期化
	CTexture::Init();

	m_Camera = new CCamera;
	m_Camera->Init(D3DXVECTOR3(0.0f, 10.0f, -15.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CManager::SetCamera(m_Camera);

	// ブラシの作成
	g_BrushType = BRUSH_NORMAL;
	g_BrushColor = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_BrushRange = 10.0f;
	g_BrushValue = 1.0f;
	g_Brush = CScene2D::Create(TEX_ID_CIRCLE_2D, g_BrushRange, g_BrushRange);
	g_Brush->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	g_Brush->SetColor(D3DCOLOR_RGBA(0, 255, 0, 255));

	g_NumBlock = 10;
	g_NextNumBlock = 10;
	g_Size = 1.0f;

	g_Light = CLight::Create(0);

	//g_Heights = new float*[g_NumBlock];
	//g_isHit = new DWORD*[g_NumBlock];
	//for (int i = 0; i < g_NumBlock; i++)
	//{
	//	g_Heights[i] = new float[g_NumBlock];
	//	g_isHit[i] = new DWORD[g_NumBlock];
	//}

	//for (int i = 0; i < g_NumBlock; i++)
	//{
	//	for (int j = 0; j < g_NumBlock; j++)
	//	{
	//		g_Heights[i][j] = 0.0f;
	//		g_isHit[i][j] = false;
	//	}
	//}

	//g_Field = CField::Create(TEX_ID_FIELD001, g_Size, g_NumBlock, g_NumBlock, g_Heights);
	g_Field = CField::Create("data/output/map.txt");

	g_NumBlock = g_Field->GetBlockSize();

	g_Heights = new float*[g_NumBlock];
	g_isHit = new DWORD*[g_NumBlock];
	for (int i = 0; i < g_NumBlock; i++)
	{
		g_Heights[i] = new float[g_NumBlock];
		g_isHit[i] = new DWORD[g_NumBlock];
	}

	for (int i = 0; i < g_NumBlock; i++)
	{
		for (int j = 0; j < g_NumBlock; j++)
		{
			g_Heights[i][j] = 0.0f;
			g_isHit[i][j] = false;
		}
	}

	bool sts;

	testshader = new Shader();
	sts = testshader->VertexShaderCompile(
		"data/shaders/shader.fx",						// シェーダーファイル名
		"VertexShader_Main",							// エントリー関数名
		"vs_3_0");						// バージョン

	if (!sts) {
		MessageBox(NULL, "エラー", "エラー", MB_OK);
	}

	// ピクセルシェーダーコンパイル
	sts = testshader->PixelShaderCompile(
		"data/shaders/shader.fx",						// シェーダーファイル名
		"PixelShader_Main",							// エントリー関数名
		"ps_3_0");						// バージョン

	if (!sts) {
		MessageBox(NULL, "読み込みエラー", "読み込みエラー", MB_OK);
	}

	testbox = new CBox();
	testbox->Init(2.0f, 2.0f, 2.0f, TEX_ID_FIELD001);
	testufo = CSceneModel::Create(MODEL_SOURCE[MODEL_ID_RIFLE]);
	testufo->Move(D3DXVECTOR3(0.0f, 1.0f, -5.0f));
	testufo->SetShader(testshader);
}

void CModeMapMake::Uninit()
{
	m_Camera->Release();
	g_Field->Release();
	g_Light->Release();

	for (int i = 0; i < g_NumBlock; i++)
	{
		delete[] g_Heights[i];
		delete[] g_isHit[i];
	}
	delete[] g_Heights;
	delete[] g_isHit;

	CScene::ReleaseAll();

	testbox->Uninit();
	delete testbox;
	delete testshader;
}

void CModeMapMake::Update()
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
	g_Brush->Set(mouse);
	g_Brush->SetSize(D3DXVECTOR2(g_BrushRange, g_BrushRange));

	VERTEX_3D* vertex = g_Field->GetVertex();

	if (inputMouse->GetRightPress())
	{
		for (int i = 0; i < g_NumBlock; i++)
		{
			for (int j = 0; j < g_NumBlock; j++)
			{
				D3DXVECTOR3 screen;
				D3DXVECTOR3 vtx = vertex[j + g_NumBlock * i].pos;

				if (!g_VertexMode)
				{
					vtx.y = 0.0f;
				}

				// スクリーン座標に変換
				WorldToScreen(pDevice, CManager::GetCamera()->GetView(), CManager::GetCamera()->GetProjection(), vtx, screen);

				if (isCollisionCircle2D(mouse.x, mouse.y, g_BrushRange / 2.0f, screen.x, screen.y, 1.0f))
				{
					if (timeGetTime() - g_isHit[i][j] > 300.0)
					{
						switch (g_BrushType)
						{
						case BRUSH_NORMAL:
							vertex[j + g_NumBlock * i].pos.y += g_BrushValue;
							break;
						case BRUSH_FLAT_HIGH:
							if (i != 0 && (i + 1) < g_NumBlock)
							{
								if (vertex[j + g_NumBlock * i].pos.y < vertex[j + g_NumBlock * i - 1].pos.y)
								{
									vertex[j + g_NumBlock * i].pos.y = vertex[j + g_NumBlock * i - 1].pos.y;
								}
								if (vertex[j + g_NumBlock * i].pos.y < vertex[j + g_NumBlock * i + 1].pos.y)
								{
									vertex[j + g_NumBlock * i].pos.y = vertex[j + g_NumBlock * i + 1].pos.y;
								}
								if (vertex[j + g_NumBlock * i].pos.y < vertex[j + g_NumBlock * (i + 1)].pos.y)
								{
									vertex[j + g_NumBlock * i].pos.y = vertex[j + g_NumBlock * (i + 1)].pos.y;
								}
								if (vertex[j + g_NumBlock * i].pos.y < vertex[j + g_NumBlock * (i - 1)].pos.y)
								{
									vertex[j + g_NumBlock * i].pos.y = vertex[j + g_NumBlock * (i - 1)].pos.y;
								}
							}
							break;
						case BRUSH_FLAT_LOW:
							if (i != 0 && (i + 1) < g_NumBlock)
							{
								if (vertex[j + g_NumBlock * i].pos.y > vertex[j + g_NumBlock * i - 1].pos.y)
								{
									vertex[j + g_NumBlock * i].pos.y = vertex[j + g_NumBlock * i - 1].pos.y;
								}
								if (vertex[j + g_NumBlock * i].pos.y > vertex[j + g_NumBlock * i + 1].pos.y)
								{
									vertex[j + g_NumBlock * i].pos.y = vertex[j + g_NumBlock * i + 1].pos.y;
								}
								if (vertex[j + g_NumBlock * i].pos.y > vertex[j + g_NumBlock * (i + 1)].pos.y)
								{
									vertex[j + g_NumBlock * i].pos.y = vertex[j + g_NumBlock * (i + 1)].pos.y;
								}
								if (vertex[j + g_NumBlock * i].pos.y > vertex[j + g_NumBlock * (i - 1)].pos.y)
								{
									vertex[j + g_NumBlock * i].pos.y = vertex[j + g_NumBlock * (i - 1)].pos.y;
								}
							}
							break;
						case BRUSH_FLAT_RESET:
							vertex[j + g_NumBlock * i].pos.y = 0.0f;
							break;
						case BRUSH_COLOR:
							vertex[j + g_NumBlock * i].color = g_BrushColor;
							break;
						default:
							break;
						}
						g_isHit[i][j] = timeGetTime();
					}
				}
				else
				{
					g_isHit[i][j] = 0.0;
				}
			}
		}
	}

	// フィールドを更新
	g_Field->SetVertex(vertex);
	g_Field->Change();

	ImGui::Begin("MapConfig", 0);
	ImGui::Checkbox("isWireframe", &g_Wire);
	ImGui::SliderInt("NumBlock", &g_NextNumBlock, 1, 1000);
	if (ImGui::Button("Reload"))
	{
		ChangeMap();
	}
	ImGui::InputText("FileName", g_FileName, sizeof(g_FileName));
	if (ImGui::Button("Save"))
	{
		char filename[256];
		sprintf_s(filename, "data/output/%s", g_FileName);
		g_Field->Save(filename);
	}
	if (ImGui::Button("Load"))
	{
		char filename[256];
		sprintf_s(filename, "data/output/%s", g_FileName);
		g_Field->Load(filename);
	}
	ImGui::End();

	ImGui::Begin("BrushConfig", 0);

	ImGui::Text("BrushType : %d", g_BrushType);
	ImGui::Checkbox("Vertex", &g_VertexMode);
	ImGui::RadioButton("HEIGHT", &g_BrushType, BRUSH_NORMAL); 
	ImGui::RadioButton("FLAT_H", &g_BrushType, BRUSH_FLAT_HIGH);
	ImGui::RadioButton("FLAT_L", &g_BrushType, BRUSH_FLAT_LOW);
	ImGui::RadioButton("FLAT_R", &g_BrushType, BRUSH_FLAT_RESET);
	ImGui::RadioButton("COLOR", &g_BrushType, BRUSH_COLOR);
	ImGui::SliderFloat("BrashSize", &g_BrushRange, 1.0f, 1000.0f);
	ImGui::SliderFloat("BrashValue", &g_BrushValue, -10.0f, 10.0f);
	ImGui::SliderFloat("CameraSpeed", &CAMERA_SPEED, -2.0f, 2.0f);
	ImGui::ColorEdit4("BrushColor", g_TempColor);
	ImGui::Text("%.2f, %.2f, %.2f, %.2f", g_TempColor[0], g_TempColor[1], g_TempColor[2], g_TempColor[3]);
	ImGui::End();

	g_BrushColor = D3DCOLOR_RGBA((int)(g_TempColor[0] * 255), (int)(g_TempColor[1] * 255), (int)(g_TempColor[2] * 255), (int)(g_TempColor[3] * 255));
}

void CModeMapMake::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	if (g_Wire)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	CScene::DrawAll();

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	//testbox->Draw(mat);
	testbox->DrawWithShader(mat, testshader);

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	g_Brush->Draw();

	CImGui::EndDraw();
}

void CModeMapMake::ChangeMap()
{
	// 高さを変更
	// 以前のマップを解放
	for (int i = 0; i < g_NumBlock; i++)
	{
		delete[] g_Heights[i];
		delete[] g_isHit[i];
		g_Heights[i] = NULL;
		g_isHit[i] = NULL;
	}
	delete[] g_Heights;
	delete[] g_isHit;
	g_Heights = NULL;
	g_isHit = NULL;

	// マップのサイズを更新
	g_NumBlock = g_NextNumBlock;

	// 新しいマップを作成
	g_Heights = new float*[g_NumBlock];
	g_isHit = new DWORD*[g_NumBlock];
	for (int i = 0; i < g_NumBlock; i++)
	{
		g_Heights[i] = new float[g_NumBlock];
		g_isHit[i] = new DWORD[g_NumBlock];
	}

	for (int i = 0; i < g_NumBlock; i++)
	{
		for (int j = 0; j < g_NumBlock; j++)
		{
			g_Heights[i][j] = 0.0f;
			g_isHit[i][j] = false;
		}
	}

	g_Field->Release();
	g_Field = NULL;

	g_Field = CField::Create(TEX_ID_FIELD001, g_Size, g_NumBlock, g_NumBlock, g_Heights);
}
