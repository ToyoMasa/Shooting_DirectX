//======================================================================
//	3Dモデル(FBX)の描画
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "sceneModelFBX.h"

//======================================================================
//	初期処理関数
//======================================================================
void CSceneModelFBX::InitFbxManager()
{
}

void CSceneModelFBX::Init(const std::string& modelName)
{
	m_FbxManager = FbxManager::Create();
	m_IOSettings = FbxIOSettings::Create(m_FbxManager, IOSROOT);
	m_FbxManager->SetIOSettings(m_IOSettings);
	m_FbxImporter = FbxImporter::Create(m_FbxManager, "");

	const char* str = modelName.c_str();
	bool result = m_FbxImporter->Initialize(str, -1, m_FbxManager->GetIOSettings());

	if (!result)
	{
		m_FbxImporter->GetStatus().GetErrorString();
	}
	else
	{
		m_Scene = FbxScene::Create(m_FbxManager, "myScene");
		m_FbxImporter->Import(m_Scene);
	}

	m_FbxImporter->Destroy();

	// ファイルの書き出し

	// 三角ポリゴンへコンバート
	FbxGeometryConverter geometryConverter(m_FbxManager);
	geometryConverter.Triangulate(m_Scene, true);

	// ノードの取得
	m_NodeCount = m_Scene->GetNodeCount();
	m_RootNode = m_Scene->GetRootNode();
	if (m_RootNode != NULL)
	{
		//--- ルートノードの子ノード数を取得 ---//
		int childCount = m_RootNode->GetChildCount();

		//--- 子ノードの数だけ探査をする ---//
		for (int i = 0; childCount > i; i++) 
		{
			GetMesh(m_RootNode->GetChild(i));
		}
	}

	m_FbxManager->Destroy();

	// 描画準備
	//LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	//if (pDevice == NULL)
	//{
	//	return;
	//}

	//HRESULT hr;

	//// 頂点バッファ							↓大きい分には問題ない
	//hr = pDevice->CreateVertexBuffer(sizeof(VERTEX) * m_VertexCount, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	//if (FAILED(hr))
	//{
	//	MessageBox(NULL, "頂点バッファの読み込みに失敗しました", "エラー", MB_OK);
	//	return;
	//}

	//// インデックスバッファ					↓大きい分には問題ない			↓sizeがDWORDなら32
	//hr = pDevice->CreateIndexBuffer(sizeof(DWORD) * m_IndexCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_IndexBuffer, NULL);

	//if (FAILED(hr))
	//{
	//	MessageBox(NULL, "インデックスバッファの読み込みに失敗しました", "エラー", MB_OK);
	//	return;
	//}

}

//======================================================================
//	終了処理関数
//======================================================================
void CSceneModelFBX::UninitFbxManager()
{

}

void CSceneModelFBX::Uninit()
{
	if (m_PolygonSize != NULL)
	{
		delete[] m_PolygonSize;
	}

	if (m_Vertex != NULL)
	{
		delete[] m_Vertex;
	}

	if (m_Index != NULL)
	{
		delete[] m_Index;
	}

	if (m_VertexColor != NULL)
	{
		delete[] m_VertexColor;
	}

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

//======================================================================
//	更新関数
//======================================================================
void CSceneModelFBX::Update()
{

}

//======================================================================
//	描画関数
//======================================================================
void CSceneModelFBX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	//VERTEX* V;

	//// 頂点バッファ
	//m_VertexBuffer->Lock(0, 0, (void**)&V, D3DLOCK_DISCARD);

	//for (int i = 0; i < m_VertexCount; i++)
	//{
	//	V[i] = m_Vertex[i];
	//}

	//m_VertexBuffer->Unlock();

	//LPWORD Index;
	//m_IndexBuffer->Lock(0, 0, (void**)&Index, D3DLOCK_DISCARD);

	//for (int i = 0; i < m_IndexCount; i++)
	//{
	//	Index[i] = m_Index[i];
	//}

	//m_IndexBuffer->Unlock();



	// FVF(今から使用する頂点情報)の設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTransform(D3DTS_WORLD, &m_World);

	//// 頂点バッファとインデックスバッファの設定
	//pDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX));
	//pDevice->SetIndices(m_IndexBuffer);
	//
	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_VertexCount, m_PolygonCount, m_Index, D3DFMT_INDEX32, m_Vertex, sizeof(VERTEX));
}

void CSceneModelFBX::SetWorld(D3DXMATRIX move)
{

}

void CSceneModelFBX::Move(D3DXVECTOR3 pos)
{

}

void CSceneModelFBX::Rotate(D3DXVECTOR3 rot)
{

}

void CSceneModelFBX::Rotate(D3DXMATRIX rot)
{

}

void CSceneModelFBX::Scale(D3DXVECTOR3 scale)
{

}

CSceneModelFBX* CSceneModelFBX::Create(const std::string& modelName)
{
	CSceneModelFBX* sceneModelFbx = new CSceneModelFBX(1);
	sceneModelFbx->Init(modelName);

	return sceneModelFbx;
}

//CSceneModelFBX* CSceneModelFBX::Create(const std::string& modelName, bool isIgnore)
//{
//	return;
//}

void CSceneModelFBX::GetMesh(FbxNode* node)
{
	//--- ノードの属性を取得 ---//
	FbxNodeAttribute* attr = node->GetNodeAttribute();

	if (NULL != attr) {
		//--- 属性の判別 ---//
		switch (attr->GetAttributeType()) {
			//--- メッシュノード発見 ---//
		case FbxNodeAttribute::eMesh:
			//----------------------------------------
			// メッシュが見つかったので必要な情報を取得
			//----------------------------------------
			FbxMesh* mesh = node->GetMesh();					// メッシュを取得
			GetPosition(mesh);
			GetNormal(mesh);
			GetVertexUV(mesh);
			GetTextureNames(mesh);
			GetVertexColor(mesh);

			break;
		}
	}

	//--- 子ノードの再帰探査 ---//
	int childCount = node->GetChildCount();
	for (int i = 0; childCount > i; i++) {
		GetMesh(node->GetChild(i));
	}
}

void CSceneModelFBX::GetPosition(FbxMesh* mesh)
{
	m_PolygonCount = mesh->GetPolygonCount();			// ポリゴン数
	m_VertexCount = mesh->GetControlPointsCount();		// 頂点数
	m_IndexCount = mesh->GetPolygonVertexCount();		// インデックスバッファ数
	FbxVector4* vertex = mesh->GetControlPoints();		// 頂点座標
	int* index = mesh->GetPolygonVertices();			// インデックスバッファ

	//--- ポリゴンサイズを取得 ---//
	m_PolygonSize = new int[m_PolygonCount];

	for (int i = 0; m_PolygonCount > i; i++) {
		m_PolygonSize[i] = mesh->GetPolygonSize(i);
	}

	// 頂点バッファのハードコピー
	m_Vertex = new Vertex[m_VertexCount];

	for (int i = 0; m_VertexCount > i; i++) {
		//--- i番目の頂点の座標Ｘを取得 ---//
		m_Vertex[i].posX = (float)vertex[i][0];

		//--- i番目の頂点の座標Ｙを取得 ---//
		m_Vertex[i].posY = (float)vertex[i][1];

		//--- i番目の頂点の座標Ｚを取得 ---//
		m_Vertex[i].posZ = (float)vertex[i][2];
	}

	// インデックスバッファのハードコピー
	m_Index = new DWORD[m_IndexCount];
	for (int i = 0; m_IndexCount > i; i++)
	{
		m_Index[i] = index[i];
	}
}

void CSceneModelFBX::GetNormal(FbxMesh* mesh) 
{
	//--- 法線セット数を取得 ---//
	int normalLayerCount = mesh->GetElementNormalCount();

	//--- レイヤー数だけ回る ---//
	for (int i = 0; normalLayerCount > i; i++) {
		//--- 法線セットを取得 ---//
		FbxGeometryElementNormal* normal = mesh->GetElementNormal(i);

		//--- マッピングモードの取得
		FbxGeometryElement::EMappingMode mapping = normal->GetMappingMode();
		//--- リファレンスモードの取得 ---//
		FbxGeometryElement::EReferenceMode reference = normal->GetReferenceMode();

		//--- マッピングモードの判別 ---//
		switch (mapping) {
		case FbxGeometryElement::eByControlPoint:
			//--- リファレンスモードの判別 ---//
			switch (reference) {
			case FbxGeometryElement::eDirect:
			{
				//--- 法線数を取得 ---//
				int normalCount = normal->GetDirectArray().GetCount();

				//-----------------------------------------------------------------------
				// eDirect の場合データは順番に格納されているのでそのまま保持
				//-----------------------------------------------------------------------
				for (int i = 0; normalCount > i; i++) 
				{
					//--- 法線の取得 ---//
					m_Vertex[i].normalX = (float)normal->GetDirectArray().GetAt(i)[0];
					m_Vertex[i].normalY = (float)normal->GetDirectArray().GetAt(i)[1];
					m_Vertex[i].normalZ = (float)normal->GetDirectArray().GetAt(i)[2];
				}
			}
			break;

			case FbxGeometryElement::eIndexToDirect:
				break;

			default:
				break;
			}

			break;
		case FbxGeometryElement::eByPolygonVertex:
			break;
		default:
			break;
		}
	}
}

void CSceneModelFBX::GetVertexUV(FbxMesh* mesh)
{
	//--- UVセット数を取得 ---//
	int UVLayerCount = mesh->GetElementUVCount();
	for (int i = 0; UVLayerCount > i; i++) {
		//--- UVバッファを取得 ---//
		FbxGeometryElementUV* UV = mesh->GetElementUV(i);

		//--- マッピングモードの取得
		FbxGeometryElement::EMappingMode mapping = UV->GetMappingMode();
		//--- リファレンスモードの取得 ---//
		FbxGeometryElement::EReferenceMode reference = UV->GetReferenceMode();

		//--- UV数を取得 ---//
		int uvCount = UV->GetDirectArray().GetCount();

		//--- マッピングモードの判別 ---//
		switch (mapping) {
		case FbxGeometryElement::eByControlPoint:
			break;

		case FbxGeometryElement::eByPolygonVertex:
			//--- リファレンスモードの判別 ---//
			switch (reference) {
			case FbxGeometryElement::eDirect:
				break;
			case FbxGeometryElement::eIndexToDirect:
			{
				FbxLayerElementArrayTemplate<int>* uvIndex = &UV->GetIndexArray();
				int uvIndexCount = uvIndex->GetCount();

				//--- UVを保持 ---// 
				point2 temp;
				for (int i = 0; uvIndexCount > i; i++) {

					temp.x = (float)UV->GetDirectArray().GetAt(uvIndex->GetAt(1))[0];

					temp.y = 1.0f - (float)UV->GetDirectArray().GetAt(uvIndex->GetAt(i))[1];

					m_UVset.uvBuffer.push_back(temp);
				}

				//--- UVSet名を取得 ---//
				m_UVset.uvSetName = UV->GetName();
			}
			break;
			default:
				break;
			}
			break;
		case FbxGeometryElement::eByEdge:
			break;
		case FbxGeometryElement::eByPolygon:
			break;
		default:
			break;
		}
	}
}

void CSceneModelFBX::GetTextureNames(FbxMesh* mesh)
{
	//--- メッシュからノードを取得 ---//
	FbxNode* node = mesh->GetNode();

	//--- マテリアルの数を取得 ---//
	int materialCount = node->GetMaterialCount();

	//--- マテリアルの数だけ繰り返す ---//
	for (int i = 0; materialCount > i; i++) {

		//--- マテリアルを取得 ---//
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

		//--- FbxLayeredTexture の数を取得 ---//
		int layeredTextureCount = prop.GetSrcObjectCount();

		//--- アタッチされたテクスチャが FbxLayeredTexture の場合 ---//
		if (0 < layeredTextureCount) {

			//--- アタッチされたテクスチャの数だけ繰り返す ---//
			for (int j = 0; layeredTextureCount > j; j++) {

				//--- テクスチャを取得 ---//
				FbxLayeredTexture* layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(j);
				//--- レイヤー数を取得 ---//
				int textureCount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();

				//--- レイヤー数だけ繰り返す ---//
				for (int k = 0; textureCount > k; k++) {
					//--- テクスチャを取得 ---//
					FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(k);

					if (texture) {
						//--- テクスチャ名を取得 ---//
						//std::string textureName = texture->GetName();
						std::string textureName = texture->GetRelativeFileName();

						//--- UVSet名を取得 ---//
						std::string UVSetName = texture->UVSet.Get().Buffer();

						//--- UVSet名を比較し対応しているテクスチャなら保持 ---//
						if (m_UVset.uvSetName == UVSetName) {
							m_UVset.textures.push_back(textureName);
						}
					}
				}
			}
		}
		else
		{
			//--- テクスチャ数を取得 ---//
			int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

			if (0 < fileTextureCount)
			{
				//--- テクスチャの数だけ繰り返す ---//
				for (int j = 0; fileTextureCount > j; j++) 
				{
					//--- テクスチャを取得 ---//
					FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(j);
					if (texture) {
						//--- テクスチャ名を取得 ---//
						//std::string textureName = texture->GetName();
						std::string textureName = texture->GetRelativeFileName();

						//--- UVSet名を取得 ---//
						std::string UVSetName = texture->UVSet.Get().Buffer();

						//--- UVSet名を比較し対応しているテクスチャなら保持 ---//
						if (m_UVset.uvSetName == UVSetName) {
							m_UVset.textures.push_back(textureName);
						}
					}
				}
			}
		}
	}
}

void CSceneModelFBX::GetVertexColor(FbxMesh* mesh)
{
	//--- 頂点カラーセット数を取得 ---//
	int vColorLayerCount = mesh->GetElementVertexColorCount();

	//--- レイヤー数だけ回る ---//
	for (int i = 0; vColorLayerCount > i; i++) 
	{
		//--- 法線セットを取得 ---//
		FbxGeometryElementVertexColor* color = mesh->GetElementVertexColor(i);

		//--- マッピングモードの取得
		FbxGeometryElement::EMappingMode mappingMode = color->GetMappingMode();
		//--- リファレンスモードの取得 ---//
		FbxGeometryElement::EReferenceMode referenceMode = color->GetReferenceMode();

		//--- マッピングモードの判別 ---//
		switch (mappingMode)
		{
		case FbxGeometryElement::eByControlPoint:
			break;
		case FbxGeometryElement::eByPolygon:
			break;
		case FbxGeometryElement::eByPolygonVertex:
			//--- リファレンスモードの判別 ---//
			switch (referenceMode)
			{
			case FbxGeometryElement::eIndexToDirect:
			{
				FbxLayerElementArrayTemplate<int>* index = &color->GetIndexArray();
				int indexCount = index->GetCount();

				m_VertexColor = new ColorRGBA[indexCount];

				//--- 頂点の数だけ頂点カラーを取得 ---//
				for (int j = 0; indexCount > j; j++)
				{
					m_VertexColor[j].r = (float)(color->GetDirectArray().GetAt(index->GetAt(j))[0]);
					m_VertexColor[j].g = (float)(color->GetDirectArray().GetAt(index->GetAt(j))[1]);
					m_VertexColor[j].b = (float)(color->GetDirectArray().GetAt(index->GetAt(j))[2]);
					m_VertexColor[j].a = (float)(color->GetDirectArray().GetAt(index->GetAt(j))[3]);
				}
			}
			break;
			case FbxGeometryElement::eDirect:
				break;
			case FbxGeometryElement::eIndex:
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}
