//======================================================================
//	�J�������� [camera.cpp]�@�i2018/4/17�j
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "scene.h"
#include "debug.h"

typedef struct
{
	D3DXVECTOR3 pos;
	D3DCOLOR color;
}DEBUG_VERTEX;

static const DWORD FVF_VERTEX_DEBUG = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

static const int NUM_POLYGON = 16;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DCOLOR color;
} VERTEX3D_DEBUG;

//*********************************************************
//	���̃f�o�b�O�`��
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
	int i = 0;
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

	// ���C�g�̖�����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetTexture(0, NULL);

	pDevice->SetFVF(FVF_VERTEX_DEBUG);

	const float R_C = D3DX_PI * 2.0f / NUM_POLYGON;

	DEBUG_VERTEX v[NUM_POLYGON + 1];

	//*********************************************************
	//	Z�����S��]
	//*********************************************************
	for (int i = 0; i < NUM_POLYGON; i++)
	{
		v[i].pos.x = cosf(R_C * i) * m_Radius;
		v[i].pos.y = sinf(R_C * i) * m_Radius;
		v[i].pos.z = 0.0f;
		v[i].color = D3DCOLOR_RGBA(32, 255, 64, 255);
	}

	v[NUM_POLYGON] = v[0];

	// �|�W�V�����̍X�V
	D3DXMatrixTranslation(&m_World, m_Pos.x, m_Pos.y, m_Pos.z);
	pDevice->SetTransform(D3DTS_WORLD, &m_World);

	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, NUM_POLYGON, &v, sizeof(DEBUG_VERTEX));

	//*********************************************************
	//	X�����S��]
	//*********************************************************
	for (int i = 0; i < NUM_POLYGON; i++)
	{
		v[i].pos.z = cosf(R_C * i) * m_Radius;
		v[i].pos.y = sinf(R_C * i) * m_Radius;
		v[i].pos.x = 0.0f;
		v[i].color = D3DCOLOR_RGBA(32, 255, 64, 255);
	}

	v[NUM_POLYGON] = v[0];

	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, NUM_POLYGON, &v, sizeof(DEBUG_VERTEX));

	//*********************************************************
	//	Y�����S��]
	//*********************************************************
	for (int i = 0; i < NUM_POLYGON; i++)
	{
		v[i].pos.x = cosf(R_C * i) * m_Radius;
		v[i].pos.z = sinf(R_C * i) * m_Radius;
		v[i].pos.y = 0.0f;
		v[i].color = D3DCOLOR_RGBA(32, 255, 64, 255);
	}

	v[NUM_POLYGON] = v[0];

	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, NUM_POLYGON, &v, sizeof(DEBUG_VERTEX));
}

CDebugSphere* CDebugSphere::Create(D3DXVECTOR3 pos, float r)
{
	CDebugSphere* dSphere = new CDebugSphere(LAYER_OBJECT3D);
	dSphere->Init(pos, r);

	return dSphere;
}
