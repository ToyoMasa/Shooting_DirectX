//======================================================================
//	3Dモデルの描画
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "sceneModel.h"

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

	//// クローン先
	//LPD3DXMESH pCloneMesh;
	//D3DVERTEXELEMENT9 elements[MAXD3DDECLLENGTH + 1];
	//m_Mesh->GetDeclaration(elements);

	//// VRAM上にクローンする（コピーする）
	//hr = m_Mesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY | D3DXMESH_32BIT, elements, pDevice, &pCloneMesh);

	//if (FAILED(hr))
	//{
	//	MessageBox(NULL, "クローンに失敗しました", "エラー", MB_OK);
	//	return;
	//}
	//// クローンがいるので元のやつを解放してクローン先のアドレスをもらう
	//if (m_Mesh != NULL)
	//{
	//	m_Mesh->Release();
	//	m_Mesh = pCloneMesh;
	//}

	AddTangentSpace();

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

	for (int j = 0; j < (int)m_MaterialNum; j++)
	{
		m_Texture[j] = NULL;
	}

	for (int j = 0; j < (int)m_MaterialNum; j++)
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
	for (int i = 0; i < (int)m_MaterialNum; i++)
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

	// 法線マップテクスチャの解放
	if (m_NormalmapTexture != NULL)
	{
		m_NormalmapTexture->Release();
		m_NormalmapTexture = NULL;
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

	// ライトを無視するモデルならライトを無効化
	if (m_isIgnoreLight)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetMaterial(&pMaterials->MatD3D);

	int texnum = 0;

	for (int i = 0; i < (int)m_MaterialNum; i++)
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

	D3DXVECTOR4  tempcolor;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)m_Material->GetBufferPointer();

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_Shader->ShaderSet(m_World);

	// サブセットを描画する(マテリアル数分ループ)
	for (int i = 0; i < (int)m_MaterialNum; i++)
	{
		pDevice->SetMaterial(&pMaterials[i].MatD3D);
		m_Shader->SetMaterial(pMaterials[i].MatD3D);
		
		// テクスチャを使用するか否かをセット
		if (pMaterials[i].pTextureFilename != NULL)
		{
			m_Shader->GetPSTable()->SetBool(pDevice, "g_tex", TRUE);
			// テクスチャをサンプラーへセット
			int index = m_Shader->GetPSTable()->GetSamplerIndex("Sampler1");
			pDevice->SetTexture(index, m_Texture[i]);
			
			// 法線マップを持っているならシェーダーに渡す
			if (m_NormalmapTexture != NULL)
			{
				// テクスチャをサンプラーへセット
				index = m_Shader->GetPSTable()->GetSamplerIndex("NormalmapSampler1");
				pDevice->SetTexture(index, m_NormalmapTexture);
			}
		}
		else
		{
			m_Shader->GetPSTable()->SetBool(pDevice, "g_tex", FALSE);
			// テクスチャをサンプラーへセット
			int index = m_Shader->GetPSTable()->GetSamplerIndex("Sampler1");
			pDevice->SetTexture(index, NULL);

			if (m_NormalmapTexture != NULL)
			{
				// テクスチャをサンプラーへセット
				index = m_Shader->GetPSTable()->GetSamplerIndex("NormalmapSampler1");
				pDevice->SetTexture(index, NULL);
			}
		}

		m_Mesh->DrawSubset(i);					// サブセットの描画
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// 頂点シェーダーとピクセルシェーダーをリセット
	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);
}

void CSceneModel::SetWorld(D3DXMATRIX move)
{
	m_Target = move;
}

void CSceneModel::SetNormalMapTexture(const std::string& texName)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}
	char filename[256];
	ZeroMemory(filename, sizeof(filename));

	sprintf_s(filename, "data/textures/%s", texName.c_str());

	HRESULT hr;
	hr = D3DXCreateTextureFromFile(pDevice, filename, &m_NormalmapTexture);

	if (FAILED(hr))
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", "エラー", MB_OK);
	}
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

// 従法線、接ベクトルのないメッシュに従法線、接ベクトルを追加する
bool CSceneModel::AddTangentSpace() 
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return false;
	}

	LPD3DXMESH		tempMesh;
	LPD3DXMESH		outMesh;
	HRESULT			hr;

	const D3DVERTEXELEMENT9 Decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },				// 位置情報
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },				// テクスチャ座標
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },				// 法線
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },				// 接ベクトル
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },				// 従法線ベクトル
		D3DDECL_END()
	};

	// 指定フォーマットのクローンメッシュを作る
	hr = m_Mesh->CloneMesh(m_Mesh->GetOptions(), Decl, pDevice, &tempMesh);
	if (FAILED(hr)) {
		MessageBox(NULL, "ERROR!!", "CloneMeshエラー", MB_OK);
		return false;
	}

	// 法線ベクトルを計算
	hr = D3DXComputeNormals(tempMesh, NULL);
	if (FAILED(hr)) {
		tempMesh->Release();
		MessageBox(NULL, "ERROR!!", "D3DXComputeNormalsエラー", MB_OK);
		return false;
	}

	// 従法線、接ベクトルをもつメッシュを作成する
	hr = D3DXComputeTangentFrameEx(tempMesh,
		D3DDECLUSAGE_TEXCOORD, 0,		// 0番目のテクスチャ座標を使用する 
		D3DDECLUSAGE_TANGENT, 0,		// 0番目の接ベクトルを計算する
		D3DDECLUSAGE_BINORMAL, 0,		// 0番目の従法線ベクトルを計算する
		D3DDECLUSAGE_NORMAL, 0,			// 0番目の法線ベクトルを計算する
		0,								// 計算のオプション
		NULL,							// 
		-1.01f,							// 隣接する３角形の閾値
		-0.01f,							// 単独とみなされる頂点の閾値
		-1.01f,							// 法線の閾値
		&outMesh,						// 生成されたメッシュ
		NULL);							//  
	if (FAILED(hr)) {
		tempMesh->Release();
		MessageBox(NULL, "ERROR!!", "D3DXComputeTangentFrameExエラー", MB_OK);
		return false;
	}

	m_Mesh->Release();
	m_Mesh = outMesh;

	if (tempMesh)
	{
		tempMesh->Release();
	}

	return true;
}

CSceneModel* CSceneModel::Create(const std::string& modelName, bool isIgnore)
{
	CSceneModel* sceneModel = new CSceneModel(LAYER_OBJECT3D);
	sceneModel->Init(modelName);
	sceneModel->m_isIgnoreLight = isIgnore;

	return sceneModel;
}
