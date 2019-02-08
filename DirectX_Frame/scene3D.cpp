//======================================================================
//	3Dモデルの描画
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "scene3D.h"
#include "texture.h"

//======================================================================
//	初期処理関数
//======================================================================
void CScene3D::Init(int texId, float meshSize, int sizeX, int sizeY, int numPrimitive, int numVertex, int numIndex)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	m_NumPrimitive = numPrimitive;
	m_NumIndex = numIndex;
	m_NumVertex = numVertex;

	m_TexId = texId;
	CTexture::Load(m_TexId);

	HRESULT hr;

	// 頂点バッファ							↓大きい分には問題ない
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * numVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファの読み込みに失敗しました", "エラー", MB_OK);
		return;
	}

	// インデックスバッファ					↓大きい分には問題ない			↓sizeがDWORDなら32
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * numIndex, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "インデックスバッファの読み込みに失敗しました", "エラー", MB_OK);
		return;
	}

	VERTEX_3D* V;

	// 頂点バッファ
	m_VertexBuffer->Lock(0, 0, (void**)&V, D3DLOCK_DISCARD);

	for (int j = 0; j < sizeY + 1; j++)
	{
		for (int i = 0; i < sizeX + 1; i++)
		{
			V[i + (sizeX + 1) * j] = { D3DXVECTOR3(-(meshSize * sizeX / 2) + (meshSize * i), 0.0f, (meshSize * sizeY / 2) - (meshSize * j)), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2((float)i ,  (float)j) };
		}
	}

	m_VertexBuffer->Unlock();

	LPWORD Index;
	m_IndexBuffer->Lock(0, 0, (void**)&Index, D3DLOCK_DISCARD);

	int numindex = 0;

	for (int j = 0; j < sizeY; j++)
	{
		for (int i = 0; i < sizeX + 1; i++)
		{
			Index[numindex] = j * (sizeX + 1) + i + sizeX + 1;
			numindex++;
			Index[numindex] = j * (sizeX + 1) + i;
			numindex++;

			if (i == sizeX && !(j == sizeY - 1))
			{
				Index[numindex] = j * (sizeX + 1) + i;
				numindex++;
				Index[numindex] = (j + 1) * (sizeX + 1) + sizeX + 1;
				numindex++;
			}
		}
	}

	m_IndexBuffer->Unlock();

	// マテリアルの設定
	m_Mat.Diffuse.r = 1.0f;
	m_Mat.Diffuse.g = 1.0f;
	m_Mat.Diffuse.b = 1.0f;
	m_Mat.Diffuse.a = 1.0f;
	// アンビエントカラー（紫）
	m_Mat.Ambient.r = 0.8f;
	m_Mat.Ambient.g = 0.7f;
	m_Mat.Ambient.b = 0.9f;
	m_Mat.Ambient.a = 1.0f;

	D3DXMatrixIdentity(&m_World);

	m_isUseShadow = true;
}

void CScene3D::Init(int texId, VERTEX_3D* vertex, WORD* index, int numPrimitive, int numVertex, int numIndex)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	m_NumPrimitive = numPrimitive;
	m_NumIndex = numIndex;
	m_NumVertex = numVertex;

	m_TexId = texId;
	CTexture::Load(m_TexId);

	HRESULT hr;

	// 頂点バッファ							↓大きい分には問題ない
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * numVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファの読み込みに失敗しました", "エラー", MB_OK);
		return;
	}

	// インデックスバッファ					↓大きい分には問題ない			↓sizeがDWORDなら32
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * numIndex, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "インデックスバッファの読み込みに失敗しました", "エラー", MB_OK);
		return;
	}

	VERTEX_3D* V;

	// 頂点バッファ
	m_VertexBuffer->Lock(0, 0, (void**)&V, D3DLOCK_DISCARD);

	for (int i = 0; i < numVertex; i++)
	{
		V[i] = vertex[i];
	}

	m_VertexBuffer->Unlock();

	m_Vertex = vertex;

	LPWORD Index;
	m_IndexBuffer->Lock(0, 0, (void**)&Index, D3DLOCK_DISCARD);

	for (int i = 0; i < numIndex; i++)
	{
		Index[i] = index[i];
	}

	m_IndexBuffer->Unlock();

	// マテリアルの設定
	m_Mat.Diffuse.r = 1.0f;
	m_Mat.Diffuse.g = 1.0f;
	m_Mat.Diffuse.b = 1.0f;
	m_Mat.Diffuse.a = 1.0f;
	// アンビエントカラー（紫）
	m_Mat.Ambient.r = 0.8f;
	m_Mat.Ambient.g = 0.7f;
	m_Mat.Ambient.b = 0.9f;
	m_Mat.Ambient.a = 1.0f;

	D3DXMatrixIdentity(&m_World);

	m_isUseShadow = true;
}

//======================================================================
//	終了処理関数
//======================================================================
void CScene3D::Uninit()
{
	// テクスチャの解放
	CTexture::Release(m_TexId);

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

	if (m_Vertex != NULL)
	{
		delete[] m_Vertex;
		m_Vertex = NULL;
	}

	m_Shader = NULL;
}

//======================================================================
//	更新関数
//======================================================================
void CScene3D::Update()
{
}

//======================================================================
//	描画関数
//======================================================================
void CScene3D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// 頂点バッファとインデックスバッファの設定
	pDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(m_IndexBuffer);

	//各種行列の設定(自分のやりたいとこ);
	pDevice->SetTransform(D3DTS_WORLD, &m_World);

	pDevice->SetMaterial(&m_Mat);

	pDevice->SetTexture(0, CTexture::GetTexture(m_TexId));

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_NumVertex, 0, m_NumPrimitive);
}

void CScene3D::DrawWithShader()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// 頂点フォーマットの指定
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	// シェーダーのセット
	m_Shader->ShaderSet(m_World);

	//各種行列の設定(自分のやりたいとこ);
	pDevice->SetTransform(D3DTS_WORLD, &m_World);

	pDevice->SetMaterial(&m_Mat);
	m_Shader->SetMaterial(m_Mat);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// テクスチャをサンプラーへセット
	m_Shader->GetPSTable()->SetBool(pDevice, "g_tex", TRUE);
	int index = m_Shader->GetPSTable()->GetSamplerIndex("Sampler1");
	pDevice->SetTexture(index, CTexture::GetTexture(m_TexId));

	// 頂点バッファとインデックスバッファの設定
	pDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(m_IndexBuffer);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_NumVertex, 0, m_NumPrimitive);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 頂点シェーダーとピクセルシェーダーをリセット
	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);
}

CScene3D* CScene3D::Create(int texId, float meshSize, int sizeX, int sizeY, int numPrimitive, int numVertex, int numIndex)
{
	CScene3D* scene3D = new CScene3D(LAYER_OBJECT3D);
	scene3D->Init(texId, meshSize, sizeX, sizeY, numPrimitive, numVertex, numIndex);

	return scene3D;
}

CScene3D* CScene3D::Create(int texId, VERTEX_3D* vertex, WORD* index, int numPrimitive, int numVertex, int numIndex)
{
	CScene3D* scene3D = new CScene3D(LAYER_OBJECT3D);
	scene3D->Init(texId, vertex, index, numPrimitive, numVertex, numIndex);

	return scene3D;
}

CScene3D* CScene3D::Create(int texId, float meshSize, int sizeX, int sizeY, int numPrimitive, int numVertex, int numIndex, LAYER layer)
{
	CScene3D* scene3D = new CScene3D(layer);
	scene3D->Init(texId, meshSize, sizeX, sizeY, numPrimitive, numVertex, numIndex);

	return scene3D;
}

CScene3D* CScene3D::Create(int texId, VERTEX_3D* vertex, WORD* index, int numPrimitive, int numVertex, int numIndex, LAYER layer)
{
	CScene3D* scene3D = new CScene3D(layer);
	scene3D->Init(texId, vertex, index, numPrimitive, numVertex, numIndex);

	return scene3D;
}

void CScene3D::Change(VERTEX_3D* vertex, WORD* index)
{
	m_Vertex = vertex;

	VERTEX_3D* V;

	// 頂点バッファ
	m_VertexBuffer->Lock(0, 0, (void**)&V, D3DLOCK_DISCARD);

	for (int i = 0; i < m_NumVertex; i++)
	{
		V[i] = vertex[i];
	}

	m_VertexBuffer->Unlock();

}
