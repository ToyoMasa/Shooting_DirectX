//======================================================================
//	壁[wall.cpp]　（2018/8/25）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "wall.h"

static const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL);

void CWall::Init(D3DXVECTOR3 pos, float x, float y, float z, int texID)
{
	m_Pos = pos;

	m_Box = new CBox();
	m_Box->Init(x, y, z, texID);

	D3DXMatrixTranslation(&m_World, pos.x, pos.y, pos.z);

	// 当たり判定の設定
	m_OBB.SetPos(m_Pos);
	m_OBB.SetLength(x / 2.0f, y / 2.0f, z / 2.0f);

	for (int i = 0; i < 6; i++)
	{
		m_NormalPos[i] = m_OBB.GetPos();
	}

	m_NormalPos[0].y += m_OBB.GetLen(1);
	m_NormalPos[1].y -= m_OBB.GetLen(1);
	m_NormalPos[2].x += m_OBB.GetLen(0);
	m_NormalPos[3].x -= m_OBB.GetLen(0);
	m_NormalPos[4].z += m_OBB.GetLen(2);
	m_NormalPos[5].z -= m_OBB.GetLen(2);

	m_Type = SCENE_TYPE_WALL;

}

void CWall::Uninit()
{
	if (m_Box != NULL)
	{
		m_Box->Uninit();
		delete m_Box;
	}
}

void CWall::Update()
{

}

void CWall::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// FVF(今から使用する頂点情報)の設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	
	m_Box->Draw(m_World);
}

void CWall::DrawWithShader()
{
	if (m_Shader == NULL)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

}

D3DXVECTOR3 CWall::GetNormal(int n)
{
	return m_Normal[n];
}

D3DXVECTOR3 CWall::GetNormalPos(int n)
{
	return m_NormalPos[n];
}

int CWall::FindHitPlane(Sphere sphere)
{
	for (int i = 0; i < 6; i++)
	{
		if (isCollisionSpheretoPlane(sphere, m_NormalPos[i], m_Normal[i]))
		{
			return i;
		}
	}

	return 0;
}

CWall* CWall::Create(D3DXVECTOR3 pos, float X, float Y, float Z, int texID)
{
	CWall* wall = new CWall(LAYER_BACKGROUND);
	wall->Init(pos, X, Y, Z, texID);

	return wall;
}
