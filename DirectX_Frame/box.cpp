//======================================================================
//	3Dポリゴンボックス描画[box.cpp]　（2018/8/23）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "box.h"
#include "texture.h"

#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)
#define NUM_VERTEX (24)
#define NUM_PRIMITIVE (12)

CBox::CBox()
{
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
}

CBox::~CBox()
{
}

bool CBox::Init(float X, float Y, float Z, int texID)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return false;
	}
	HRESULT hr;

	//		位置座標							法線ベクトル						色							UV座標
	//前の面
	m_v[0] = { D3DXVECTOR3(-X / 2,    Y, -Z / 2), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
	m_v[1] = { D3DXVECTOR3(X / 2,    Y, -Z / 2), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 0.0f) };
	m_v[2] = { D3DXVECTOR3(-X / 2, 0.0f, -Z / 2), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f ,0.5f) };
	m_v[3] = { D3DXVECTOR3(X / 2, 0.0f, -Z / 2), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 0.5f) };
	//右の面						  								   							 
	m_v[4] = { D3DXVECTOR3(X / 2,    Y, -Z / 2), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
	m_v[5] = { D3DXVECTOR3(X / 2,    Y,  Z / 2), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 0.0f) };
	m_v[6] = { D3DXVECTOR3(X / 2, 0.0f, -Z / 2), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f ,0.5f) };
	m_v[7] = { D3DXVECTOR3(X / 2, 0.0f,  Z / 2), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 0.5f) };
	//後ろの面						  								  							 
	m_v[8] = { D3DXVECTOR3(X / 2,    Y, Z / 2), D3DXVECTOR3(0.0f, 0.0f,  1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
	m_v[9] = { D3DXVECTOR3(-X / 2,    Y, Z / 2), D3DXVECTOR3(0.0f, 0.0f,  1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 0.0f) };
	m_v[10] = { D3DXVECTOR3(X / 2, 0.0f, Z / 2), D3DXVECTOR3(0.0f, 0.0f,  1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f ,0.5f) };
	m_v[11] = { D3DXVECTOR3(-X / 2, 0.0f, Z / 2), D3DXVECTOR3(0.0f, 0.0f,  1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 0.5f) };
	//左の面														    
	m_v[12] = { D3DXVECTOR3(-X / 2,    Y,  Z / 2), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
	m_v[13] = { D3DXVECTOR3(-X / 2,    Y, -Z / 2), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 0.0f) };
	m_v[14] = { D3DXVECTOR3(-X / 2, 0.0f,  Z / 2), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f ,0.5f) };
	m_v[15] = { D3DXVECTOR3(-X / 2, 0.0f, -Z / 2), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 0.5f) };
	//上の面														    
	m_v[16] = { D3DXVECTOR3(-X / 2, Y,  Z / 2), D3DXVECTOR3(0.0f,  1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.5f) };
	m_v[17] = { D3DXVECTOR3(X / 2, Y,  Z / 2), D3DXVECTOR3(0.0f,  1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 0.5f) };
	m_v[18] = { D3DXVECTOR3(-X / 2, Y, -Z / 2), D3DXVECTOR3(0.0f,  1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) };
	m_v[19] = { D3DXVECTOR3(X / 2, Y, -Z / 2), D3DXVECTOR3(0.0f,  1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 1.0f) };
	//下の面														    
	m_v[20] = { D3DXVECTOR3(-X / 2, 0.0f, -Z / 2), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.5f) };
	m_v[21] = { D3DXVECTOR3(X / 2, 0.0f, -Z / 2), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 0.5f) };
	m_v[22] = { D3DXVECTOR3(-X / 2, 0.0f,  Z / 2), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) };
	m_v[23] = { D3DXVECTOR3(X / 2, 0.0f,  Z / 2), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.5f, 1.0f) };

	//前の面
	m_index[0] = 0;
	m_index[1] = 1;
	m_index[2] = 2;
	m_index[3] = 1;
	m_index[4] = 3;
	m_index[5] = 2;
	//右の面
	m_index[6] = 4;
	m_index[7] = 5;
	m_index[8] = 6;
	m_index[9] = 5;
	m_index[10] = 7;
	m_index[11] = 6;
	//後ろの面
	m_index[12] = 8;
	m_index[13] = 9;
	m_index[14] = 10;
	m_index[15] = 9;
	m_index[16] = 11;
	m_index[17] = 10;
	//左の面
	m_index[18] = 12;
	m_index[19] = 13;
	m_index[20] = 14;
	m_index[21] = 13;
	m_index[22] = 15;
	m_index[23] = 14;
	//上の面
	m_index[24] = 16;
	m_index[25] = 17;
	m_index[26] = 18;
	m_index[27] = 17;
	m_index[28] = 19;
	m_index[29] = 18;
	//下の面
	m_index[30] = 20;
	m_index[31] = 21;
	m_index[32] = 22;
	m_index[33] = 21;
	m_index[34] = 23;
	m_index[35] = 22;

	// 頂点バッファ							↓大きい分には問題ない
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX3D_BOX) * 24, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファの読み込みに失敗しました", "エラー", MB_OK);
		return false;
	}

	// インデックスバッファ					↓大きい分には問題ない			↓sizeがDWORDなら32
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * 36, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファの読み込みに失敗しました", "エラー", MB_OK);
		return false;
	}

	VERTEX3D_BOX* pV;

	// 頂点バッファ
	m_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	memcpy(pV, &m_v[0], sizeof(m_v));

	m_pVertexBuffer->Unlock();

	LPWORD pIndex;
	m_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);

	memcpy(pIndex, &m_index[0], sizeof(m_index));

	m_pIndexBuffer->Unlock();

	m_mat.Diffuse.r = 1.0f;
	m_mat.Diffuse.g = 1.0f;
	m_mat.Diffuse.b = 1.0f;
	m_mat.Diffuse.a = 1.0f;
	// アンビエントカラー（紫）
	m_mat.Ambient.r = 0.8f;
	m_mat.Ambient.g = 0.7f;
	m_mat.Ambient.b = 0.9f;
	m_mat.Ambient.a = 1.0f;

	CTexture::Load(texID);
	m_TexID = texID;

	return true;
}

void CBox::Uninit(void)
{
	//頂点バッファの解放
	if (m_pVertexBuffer != NULL)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}

	//インデックスバッファの解放
	if (m_pIndexBuffer != NULL)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = NULL;
	}

	CTexture::Release(m_TexID);
}

void CBox::Draw(D3DXMATRIX mtxWorld)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	if (m_TexID != -1)
	{
		pDevice->SetTexture(0, CTexture::GetTexture(m_TexID));
	}
	else
	{
		pDevice->SetTexture(0, NULL);
	}

	// 頂点バッファとインデックスバッファの設定
	pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VERTEX3D_BOX));
	pDevice->SetIndices(m_pIndexBuffer);

	//各種行列の設定(自分のやりたいところでやる)
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->SetMaterial(&m_mat);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NUM_VERTEX, 0, NUM_PRIMITIVE);
}

void CBox::DrawWithShader(D3DXMATRIX mtxWorld, Shader* shader)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// FVF(今から使用する頂点情報)の設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXVECTOR4  tempcolor;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	shader->ShaderSet(mtxWorld);

//	// 環境光用のマテリアルをセット
//	tempcolor.x = m_mat.Ambient.r;
//	tempcolor.y = m_mat.Ambient.g;
//	tempcolor.z = m_mat.Ambient.b;
//	tempcolor.w = m_mat.Ambient.a;
//	shader->GetVSTable()->SetVector(pDevice, "g_ambient_mat", &tempcolor);
//
//	// ディフューズ光用のマテリアルをセット
//	tempcolor.x = m_mat.Diffuse.r;
//	tempcolor.y = m_mat.Diffuse.g;
//	tempcolor.z = m_mat.Diffuse.b;
//	tempcolor.w = m_mat.Diffuse.a;
//	shader->GetVSTable()->SetVector(pDevice, "g_diffuse_mat", &tempcolor);
//
//	// エミッシブ光用のマテリアルをセット
//	tempcolor.x = m_mat.Emissive.r;
//	tempcolor.y = m_mat.Emissive.g;
//	tempcolor.z = m_mat.Emissive.b;
//	tempcolor.w = m_mat.Emissive.a;
//	shader->GetVSTable()->SetVector(pDevice, "g_emissive_mat", &tempcolor);
//
////	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//
//	// スペキュラー光用のマテリアルをセット
//	tempcolor.x = m_mat.Specular.r;
//	tempcolor.y = m_mat.Specular.g;
//	tempcolor.z = m_mat.Specular.b;
//	tempcolor.w = m_mat.Specular.a;
//	shader->GetVSTable()->SetVector(pDevice, "g_specular_mat", &tempcolor);
//
//	// パワー値をセット
//	shader->GetVSTable()->SetFloat(pDevice, "g_power", m_mat.Power);
//
	// テクスチャを使用するか否かをセット
	if (CTexture::GetTexture(m_TexID)) {
		shader->GetPSTable()->SetBool(pDevice, "g_tex", TRUE);
	}
	else 
	{
		shader->GetPSTable()->SetBool(pDevice, "g_tex", FALSE);
	}
	// テクスチャをサンプラーへセット
	int index = shader->GetPSTable()->GetSamplerIndex("Sampler1");
	pDevice->SetTexture(index, CTexture::GetTexture(m_TexID));

	// 頂点バッファとインデックスバッファの設定
	pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VERTEX3D_BOX));
	pDevice->SetIndices(m_pIndexBuffer);

	//各種行列の設定(自分のやりたいところでやる)
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->SetMaterial(&m_mat);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NUM_VERTEX, 0, NUM_PRIMITIVE);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// 頂点シェーダーとピクセルシェーダーをセット
	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}

