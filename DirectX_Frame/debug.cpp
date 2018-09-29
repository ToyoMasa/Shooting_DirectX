//======================================================================
//	カメラ処理 [camera.cpp]　（2018/4/17）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "scene.h"
#include "debug.h"

typedef struct
{
	D3DXVECTOR3 pos;
	D3DCOLOR color;
} VERTEX3D_DEBUG;

//*********************************************************
//	球のデバッグ描画
//*********************************************************
void CDebugSphere::Init(D3DXVECTOR3 pos, float r)
{
	m_Pos = pos;
	m_Radius = r;

	D3DXMatrixTranslation(&m_World, pos.x, pos.y, pos.z);

	if (r <= 0.0f)
	{
		Release();
	}
}

void CDebugSphere::Uninit()
{

}

void CDebugSphere::Update()
{

}

void CDebugSphere::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ライトの無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ポジションの更新
	D3DXMatrixTranslation(&m_World, m_Pos.x, m_Pos.y, m_Pos.z);

	VERTEX3D_DEBUG v[16];


}

CDebugSphere* CDebugSphere::Create(D3DXVECTOR3 pos, float r)
{
	CDebugSphere* dSphere = new CDebugSphere(LAYER_OBJECT3D);
	dSphere->Init(pos, r);

	return dSphere;
}

