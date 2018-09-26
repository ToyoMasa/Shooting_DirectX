//======================================================================
//	丸影　（2018/08/28)
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "scene.h"
#include "scene3D.h"
#include "texture.h"
#include "player.h"
#include "sceneShadow.h"

#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)

void CSceneShadow::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}
	HRESULT hr;

	CTexture::Load(TEX_ID_CIRCLE);

	// 頂点バッファ							↓大きい分には問題ない
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファの読み込みに失敗しました", "エラー", MB_OK);
		return;
	}

	// インデックスバッファ					↓大きい分には問題ない					↓sizeがDWORDなら32
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * 4, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "インデックスバッファの読み込みに失敗しました", "エラー", MB_OK);
		return;
	}

	VERTEX_3D* pV;

	// 頂点バッファ
	m_VertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	pV[0] = { D3DXVECTOR3(-0.5, 0,  0.5), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(64, 64, 64, 255), D3DXVECTOR2(0.0f, 0.0f) };
	pV[1] = { D3DXVECTOR3( 0.5, 0,  0.5), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(64, 64, 64, 255), D3DXVECTOR2(1.0f, 0.0f) };
	pV[2] = { D3DXVECTOR3(-0.5, 0, -0.5), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(64, 64, 64, 255), D3DXVECTOR2(0.0f, 1.0f) };
	pV[3] = { D3DXVECTOR3( 0.5, 0, -0.5), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(64, 64, 64, 255), D3DXVECTOR2(1.0f, 1.0f) };

	m_VertexBuffer->Unlock();

	// インデックスバッファ
	LPWORD pIndex;
	m_IndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);

	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 3;

	m_IndexBuffer->Unlock();

	D3DXMatrixIdentity(&m_World);
	D3DXMatrixIdentity(&m_Move);
	D3DXMatrixIdentity(&m_Rotate);
	D3DXMatrixIdentity(&m_Scale);
}

void CSceneShadow::Uninit()
{
	// テクスチャの解放
	CTexture::Release(TEX_ID_CIRCLE);

	//頂点バッファの解放
	if (m_VertexBuffer != NULL)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = NULL;
	}

	//インデックスバッファの解放
	if (m_IndexBuffer != NULL)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = NULL;
	}
}

void CSceneShadow::Update()
{

}

void CSceneShadow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	m_World = m_Rotate * m_Scale * m_Move;

	// ライティングを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// αテスト(3つセット)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// αテストのON/OFF
	pDevice->SetRenderState(D3DRS_ALPHAREF, 32);					// 第2引数は0～255の好きな値
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// 第2引数は不等号(GREATERは大なり)、上の値より大きければ合格

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// FVF(今から使用する頂点情報)の設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// 頂点バッファとインデックスバッファの設定
	pDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(m_IndexBuffer);

	// テクスチャのセット
	pDevice->SetTexture(0, CTexture::GetTexture(TEX_ID_CIRCLE));

	//各種行列の設定(自分のやりたいところでやる)
	pDevice->SetTransform(D3DTS_WORLD, &m_World);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストのON/OFF
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CSceneShadow::Move(D3DXVECTOR3 pos)
{
	D3DXMatrixTranslation(&m_Move, pos.x, pos.y + 0.01f, pos.z);

	m_World = m_Rotate * m_Scale * m_Move;
}

void CSceneShadow::Rotate(D3DXVECTOR3 rot)
{
	D3DXMatrixRotationX(&m_RotX, rot.x);
	D3DXMatrixRotationY(&m_RotY, rot.y);
	D3DXMatrixRotationZ(&m_RotZ, rot.z);

	m_Rotate = m_RotX * m_RotY * m_RotZ;
	m_World = m_Rotate * m_Scale * m_Move;
}

void CSceneShadow::Rotate(D3DXMATRIX rot)
{
	m_Rotate = rot;
	m_World = m_Rotate * m_Scale * m_Move;
}

void CSceneShadow::Scale(D3DXVECTOR3 scale)
{
	D3DXMatrixScaling(&m_Scale, scale.x, scale.y, scale.z);

	m_World = m_Rotate * m_Scale * m_Move;
}

CSceneShadow* CSceneShadow::Create()
{
	CSceneShadow* shadow = new CSceneShadow();
	shadow->Init();

	return shadow;
}
