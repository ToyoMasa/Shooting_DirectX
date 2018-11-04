//======================================================================
//	3Dモデルの描画
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "sceneModel.h"

static const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL);

//======================================================================
//	初期処理関数
//======================================================================
void CSceneModel::Init(const std::string& modelName)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	HRESULT hr;

	LPD3DXBUFFER pAdjacency = NULL;	// 隣接情報(最適化に必要)

	hr = D3DXLoadMeshFromX(modelName.c_str(), D3DXMESH_SYSTEMMEM, pDevice, &pAdjacency, &m_Material, NULL, &m_MaterialNum, &m_Mesh);

	if (FAILED(hr))
	{
		MessageBox(NULL, "xモデルの読み込みに失敗しました", "エラー", MB_OK);
		return;
	}

	//Xファイルに法線がない場合は、法線を書き込む
	if (!(m_Mesh->GetFVF() & D3DFVF_NORMAL)) {

		ID3DXMesh* pTempMesh = NULL;

		m_Mesh->CloneMeshFVF(m_Mesh->GetOptions(),
			m_Mesh->GetFVF() | D3DFVF_NORMAL, pDevice, &pTempMesh);

		D3DXComputeNormals(pTempMesh, NULL);
		m_Mesh->Release();
		m_Mesh = pTempMesh;
	}

	// メッシュの最適化
	hr = m_Mesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacency->GetBufferPointer(),
		NULL, NULL, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "最適化に失敗しました", "エラー", MB_OK);
		return;
	}

	// クローン先
	LPD3DXMESH pCloneMesh;
	D3DVERTEXELEMENT9 elements[MAXD3DDECLLENGTH + 1];
	m_Mesh->GetDeclaration(elements);

	// VRAM上にクローンする（コピーする）
	hr = m_Mesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY | D3DXMESH_32BIT, elements, pDevice, &pCloneMesh);

	if (FAILED(hr))
	{
		MessageBox(NULL, "クローンに失敗しました", "エラー", MB_OK);
		return;
	}
	// クローンがいるので元のやつを解放してクローン先のアドレスをもらう
	if (m_Mesh != NULL)
	{
		m_Mesh->Release();
		m_Mesh = pCloneMesh;
	}

	// メモリ解放
	if (pAdjacency != NULL)
	{
		pAdjacency->Release();
		pAdjacency = NULL;
	}

	//	pMaterials[i].MatD3Dでマテリアル型を取得できる（ツールによってマテリアルの種類が違うため加工が必要な場合もある）
	//	pMaterials[i].pTextureFilenameにファイル名が入ってる(パスを追加してテクスチャロード関数へ、NULLだったらテクスチャなし)
	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)m_Material->GetBufferPointer();

	m_Texture = new LPDIRECT3DTEXTURE9[m_MaterialNum];

	for (int j = 0; j < m_MaterialNum; j++)
	{
		m_Texture[j] = NULL;
	}

	for (int j = 0; j < m_MaterialNum; j++)
	{
		if (pMaterials[j].pTextureFilename != NULL)
		{
			char filename[256];
			ZeroMemory(filename, sizeof(filename));

			sprintf_s(filename, "data/textures/%s", pMaterials[j].pTextureFilename);

			hr = D3DXCreateTextureFromFile(pDevice, filename, &m_Texture[j]);

			m_TextureNum++;

			if (FAILED(hr))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", "エラー", MB_OK);
			}
		}
	}
}

//======================================================================
//	終了処理関数
//======================================================================
void CSceneModel::Uninit()
{
	for (int i = 0; i < m_MaterialNum; i++)
	{
		// テクスチャの解放
		if (m_Texture[i] != NULL)
		{
			m_Texture[i]->Release();
			m_Texture[i] = NULL;
		}
	}

	delete m_Texture;

	// メッシュの解放
	if (m_Mesh != NULL)
	{
		m_Mesh->Release();
		m_Mesh = NULL;
	}

	// マテリアルの解放
	if (m_Material != NULL)
	{
		m_Material->Release();
		m_Material = NULL;
	}
}

//======================================================================
//	更新関数
//======================================================================
void CSceneModel::Update()
{
}

//======================================================================
//	描画関数
//======================================================================
void CSceneModel::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	m_Rotate = m_RotX * m_RotY * m_RotZ;
	m_World = m_Scale * m_Rotate * m_Move * m_Target;
	pDevice->SetTransform(D3DTS_WORLD, &m_World);

	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)m_Material->GetBufferPointer();

	// FVF(今から使用する頂点情報)の設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ライトを無視するモデルならライトを無効化
	if (m_isIgnoreLight)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetMaterial(&pMaterials->MatD3D);

	int texnum = 0;

	for (int i = 0; i < m_MaterialNum; i++)
	{
		pDevice->SetMaterial(&pMaterials[i].MatD3D);

		if (pMaterials[i].pTextureFilename != NULL)
		{
			pDevice->SetTexture(0, m_Texture[i]);
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}

		m_Mesh->DrawSubset(i);
	}

	if (m_isIgnoreLight)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CSceneModel::DrawWithShader()
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

	m_Rotate = m_RotX * m_RotY * m_RotZ;
	m_World = m_Scale * m_Rotate * m_Move * m_Target;
	pDevice->SetTransform(D3DTS_WORLD, &m_World);

	// FVF(今から使用する頂点情報)の設定
	pDevice->SetFVF(FVF_VERTEX_SHADER);

	D3DXVECTOR4  tempcolor;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)m_Material->GetBufferPointer();

	m_Shader->ShaderSet(m_World);

	// サブセットを描画する(マテリアル数分ループ)
	for (int i = 0; i<m_MaterialNum; i++)
	{
		// 環境光用のマテリアルをセット
		tempcolor.x = pMaterials[i].MatD3D.Ambient.r;
		tempcolor.y = pMaterials[i].MatD3D.Ambient.g;
		tempcolor.z = pMaterials[i].MatD3D.Ambient.b;
		tempcolor.w = pMaterials[i].MatD3D.Ambient.a;
		m_Shader->GetVSTable()->SetVector(pDevice, "g_ambient_mat", &tempcolor);

		// ディフューズ光用のマテリアルをセット
		tempcolor.x = pMaterials[i].MatD3D.Diffuse.r;
		tempcolor.y = pMaterials[i].MatD3D.Diffuse.g;
		tempcolor.z = pMaterials[i].MatD3D.Diffuse.b;
		tempcolor.w = pMaterials[i].MatD3D.Diffuse.a;
		m_Shader->GetVSTable()->SetVector(pDevice, "g_diffuse_mat", &tempcolor);

		// エミッシブ光用のマテリアルをセット
		tempcolor.x = pMaterials[i].MatD3D.Emissive.r;
		tempcolor.y = pMaterials[i].MatD3D.Emissive.g;
		tempcolor.z = pMaterials[i].MatD3D.Emissive.b;
		tempcolor.w = pMaterials[i].MatD3D.Emissive.a;
		m_Shader->GetVSTable()->SetVector(pDevice, "g_emissive_mat", &tempcolor);

		// スペキュラー光用のマテリアルをセット
		tempcolor.x = pMaterials[i].MatD3D.Specular.r;
		tempcolor.y = pMaterials[i].MatD3D.Specular.g;
		tempcolor.z = pMaterials[i].MatD3D.Specular.b;
		tempcolor.w = pMaterials[i].MatD3D.Specular.a;
		m_Shader->GetVSTable()->SetVector(pDevice, "g_specular_mat", &tempcolor);

		// パワー値をセット
		m_Shader->GetVSTable()->SetFloat(pDevice, "g_power", pMaterials[i].MatD3D.Power);

		// テクスチャを使用するか否かをセット
		if (m_Texture[i]) {
			m_Shader->GetPSTable()->SetBool(pDevice, "g_tex", TRUE);
		}
		else {
			m_Shader->GetPSTable()->SetBool(pDevice, "g_tex", FALSE);
		}
		// テクスチャをサンプラーへセット
		int index = m_Shader->GetPSTable()->GetSamplerIndex("Sampler1");
		pDevice->SetTexture(index, m_Texture[i]);

		m_Mesh->DrawSubset(i);					// サブセットの描画
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// 頂点シェーダーとピクセルシェーダーをセット
	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);
}

void CSceneModel::SetWorld(D3DXMATRIX move)
{
	m_Target = move;
}

void CSceneModel::Move(D3DXVECTOR3 pos)
{
	m_Pos = pos;
	D3DXMatrixTranslation(&m_Move, pos.x, pos.y, pos.z);
}

void CSceneModel::Rotate(D3DXVECTOR3 rot)
{
	D3DXMatrixRotationX(&m_RotX, D3DXToRadian(rot.x));
	D3DXMatrixRotationY(&m_RotY, D3DXToRadian(rot.y));
	D3DXMatrixRotationZ(&m_RotZ, D3DXToRadian(rot.z));
}

void CSceneModel::Rotate(D3DXMATRIX rot)
{
	m_Rotate = rot;
}

void CSceneModel::Scale(D3DXVECTOR3 scale)
{
	D3DXMatrixScaling(&m_Scale, scale.x, scale.y, scale.z);
}

CSceneModel* CSceneModel::Create(const std::string& modelName)
{
	CSceneModel* sceneModel = new CSceneModel(LAYER_OBJECT3D);
	sceneModel->Init(modelName);

	return sceneModel;
}

CSceneModel* CSceneModel::Create(const std::string& modelName, bool isIgnore)
{
	CSceneModel* sceneModel = new CSceneModel(LAYER_OBJECT3D);
	sceneModel->Init(modelName);
	sceneModel->m_isIgnoreLight = isIgnore;

	return sceneModel;
}
