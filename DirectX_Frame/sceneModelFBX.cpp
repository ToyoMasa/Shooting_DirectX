//======================================================================
//	3D���f��(FBX)�̕`��
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "sceneModelFBX.h"

//======================================================================
//	���������֐�
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

	// �t�@�C���̏����o��

	// �O�p�|���S���փR���o�[�g
	FbxGeometryConverter geometryConverter(m_FbxManager);
	geometryConverter.Triangulate(m_Scene, true);

	// �m�[�h�̎擾
	m_NodeCount = m_Scene->GetNodeCount();
	m_RootNode = m_Scene->GetRootNode();
	if (m_RootNode != NULL)
	{
		//--- ���[�g�m�[�h�̎q�m�[�h�����擾 ---//
		int childCount = m_RootNode->GetChildCount();

		//--- �q�m�[�h�̐������T�������� ---//
		for (int i = 0; childCount > i; i++) 
		{
			GetMesh(m_RootNode->GetChild(i));
		}
	}

	m_FbxManager->Destroy();

	// �`�揀��
	//LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	//if (pDevice == NULL)
	//{
	//	return;
	//}

	//HRESULT hr;

	//// ���_�o�b�t�@							���傫�����ɂ͖��Ȃ�
	//hr = pDevice->CreateVertexBuffer(sizeof(VERTEX) * m_VertexCount, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	//if (FAILED(hr))
	//{
	//	MessageBox(NULL, "���_�o�b�t�@�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
	//	return;
	//}

	//// �C���f�b�N�X�o�b�t�@					���傫�����ɂ͖��Ȃ�			��size��DWORD�Ȃ�32
	//hr = pDevice->CreateIndexBuffer(sizeof(DWORD) * m_IndexCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_IndexBuffer, NULL);

	//if (FAILED(hr))
	//{
	//	MessageBox(NULL, "�C���f�b�N�X�o�b�t�@�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
	//	return;
	//}

}

//======================================================================
//	�I�������֐�
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

	//���_�o�b�t�@�̉��
	if (m_VertexBuffer != NULL)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if (m_IndexBuffer != NULL)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = NULL;
	}
}

//======================================================================
//	�X�V�֐�
//======================================================================
void CSceneModelFBX::Update()
{

}

//======================================================================
//	�`��֐�
//======================================================================
void CSceneModelFBX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	//VERTEX* V;

	//// ���_�o�b�t�@
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



	// FVF(������g�p���钸�_���)�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTransform(D3DTS_WORLD, &m_World);

	//// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
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
	//--- �m�[�h�̑������擾 ---//
	FbxNodeAttribute* attr = node->GetNodeAttribute();

	if (NULL != attr) {
		//--- �����̔��� ---//
		switch (attr->GetAttributeType()) {
			//--- ���b�V���m�[�h���� ---//
		case FbxNodeAttribute::eMesh:
			//----------------------------------------
			// ���b�V�������������̂ŕK�v�ȏ����擾
			//----------------------------------------
			FbxMesh* mesh = node->GetMesh();					// ���b�V�����擾
			GetPosition(mesh);
			GetNormal(mesh);
			GetVertexUV(mesh);
			GetTextureNames(mesh);
			GetVertexColor(mesh);

			break;
		}
	}

	//--- �q�m�[�h�̍ċA�T�� ---//
	int childCount = node->GetChildCount();
	for (int i = 0; childCount > i; i++) {
		GetMesh(node->GetChild(i));
	}
}

void CSceneModelFBX::GetPosition(FbxMesh* mesh)
{
	m_PolygonCount = mesh->GetPolygonCount();			// �|���S����
	m_VertexCount = mesh->GetControlPointsCount();		// ���_��
	m_IndexCount = mesh->GetPolygonVertexCount();		// �C���f�b�N�X�o�b�t�@��
	FbxVector4* vertex = mesh->GetControlPoints();		// ���_���W
	int* index = mesh->GetPolygonVertices();			// �C���f�b�N�X�o�b�t�@

	//--- �|���S���T�C�Y���擾 ---//
	m_PolygonSize = new int[m_PolygonCount];

	for (int i = 0; m_PolygonCount > i; i++) {
		m_PolygonSize[i] = mesh->GetPolygonSize(i);
	}

	// ���_�o�b�t�@�̃n�[�h�R�s�[
	m_Vertex = new Vertex[m_VertexCount];

	for (int i = 0; m_VertexCount > i; i++) {
		//--- i�Ԗڂ̒��_�̍��W�w���擾 ---//
		m_Vertex[i].posX = (float)vertex[i][0];

		//--- i�Ԗڂ̒��_�̍��W�x���擾 ---//
		m_Vertex[i].posY = (float)vertex[i][1];

		//--- i�Ԗڂ̒��_�̍��W�y���擾 ---//
		m_Vertex[i].posZ = (float)vertex[i][2];
	}

	// �C���f�b�N�X�o�b�t�@�̃n�[�h�R�s�[
	m_Index = new DWORD[m_IndexCount];
	for (int i = 0; m_IndexCount > i; i++)
	{
		m_Index[i] = index[i];
	}
}

void CSceneModelFBX::GetNormal(FbxMesh* mesh) 
{
	//--- �@���Z�b�g�����擾 ---//
	int normalLayerCount = mesh->GetElementNormalCount();

	//--- ���C���[��������� ---//
	for (int i = 0; normalLayerCount > i; i++) {
		//--- �@���Z�b�g���擾 ---//
		FbxGeometryElementNormal* normal = mesh->GetElementNormal(i);

		//--- �}�b�s���O���[�h�̎擾
		FbxGeometryElement::EMappingMode mapping = normal->GetMappingMode();
		//--- ���t�@�����X���[�h�̎擾 ---//
		FbxGeometryElement::EReferenceMode reference = normal->GetReferenceMode();

		//--- �}�b�s���O���[�h�̔��� ---//
		switch (mapping) {
		case FbxGeometryElement::eByControlPoint:
			//--- ���t�@�����X���[�h�̔��� ---//
			switch (reference) {
			case FbxGeometryElement::eDirect:
			{
				//--- �@�������擾 ---//
				int normalCount = normal->GetDirectArray().GetCount();

				//-----------------------------------------------------------------------
				// eDirect �̏ꍇ�f�[�^�͏��ԂɊi�[����Ă���̂ł��̂܂ܕێ�
				//-----------------------------------------------------------------------
				for (int i = 0; normalCount > i; i++) 
				{
					//--- �@���̎擾 ---//
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
	//--- UV�Z�b�g�����擾 ---//
	int UVLayerCount = mesh->GetElementUVCount();
	for (int i = 0; UVLayerCount > i; i++) {
		//--- UV�o�b�t�@���擾 ---//
		FbxGeometryElementUV* UV = mesh->GetElementUV(i);

		//--- �}�b�s���O���[�h�̎擾
		FbxGeometryElement::EMappingMode mapping = UV->GetMappingMode();
		//--- ���t�@�����X���[�h�̎擾 ---//
		FbxGeometryElement::EReferenceMode reference = UV->GetReferenceMode();

		//--- UV�����擾 ---//
		int uvCount = UV->GetDirectArray().GetCount();

		//--- �}�b�s���O���[�h�̔��� ---//
		switch (mapping) {
		case FbxGeometryElement::eByControlPoint:
			break;

		case FbxGeometryElement::eByPolygonVertex:
			//--- ���t�@�����X���[�h�̔��� ---//
			switch (reference) {
			case FbxGeometryElement::eDirect:
				break;
			case FbxGeometryElement::eIndexToDirect:
			{
				FbxLayerElementArrayTemplate<int>* uvIndex = &UV->GetIndexArray();
				int uvIndexCount = uvIndex->GetCount();

				//--- UV��ێ� ---// 
				point2 temp;
				for (int i = 0; uvIndexCount > i; i++) {

					temp.x = (float)UV->GetDirectArray().GetAt(uvIndex->GetAt(1))[0];

					temp.y = 1.0f - (float)UV->GetDirectArray().GetAt(uvIndex->GetAt(i))[1];

					m_UVset.uvBuffer.push_back(temp);
				}

				//--- UVSet�����擾 ---//
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
	//--- ���b�V������m�[�h���擾 ---//
	FbxNode* node = mesh->GetNode();

	//--- �}�e���A���̐����擾 ---//
	int materialCount = node->GetMaterialCount();

	//--- �}�e���A���̐������J��Ԃ� ---//
	for (int i = 0; materialCount > i; i++) {

		//--- �}�e���A�����擾 ---//
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

		//--- FbxLayeredTexture �̐����擾 ---//
		int layeredTextureCount = prop.GetSrcObjectCount();

		//--- �A�^�b�`���ꂽ�e�N�X�`���� FbxLayeredTexture �̏ꍇ ---//
		if (0 < layeredTextureCount) {

			//--- �A�^�b�`���ꂽ�e�N�X�`���̐������J��Ԃ� ---//
			for (int j = 0; layeredTextureCount > j; j++) {

				//--- �e�N�X�`�����擾 ---//
				FbxLayeredTexture* layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(j);
				//--- ���C���[�����擾 ---//
				int textureCount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();

				//--- ���C���[�������J��Ԃ� ---//
				for (int k = 0; textureCount > k; k++) {
					//--- �e�N�X�`�����擾 ---//
					FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(k);

					if (texture) {
						//--- �e�N�X�`�������擾 ---//
						//std::string textureName = texture->GetName();
						std::string textureName = texture->GetRelativeFileName();

						//--- UVSet�����擾 ---//
						std::string UVSetName = texture->UVSet.Get().Buffer();

						//--- UVSet�����r���Ή����Ă���e�N�X�`���Ȃ�ێ� ---//
						if (m_UVset.uvSetName == UVSetName) {
							m_UVset.textures.push_back(textureName);
						}
					}
				}
			}
		}
		else
		{
			//--- �e�N�X�`�������擾 ---//
			int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

			if (0 < fileTextureCount)
			{
				//--- �e�N�X�`���̐������J��Ԃ� ---//
				for (int j = 0; fileTextureCount > j; j++) 
				{
					//--- �e�N�X�`�����擾 ---//
					FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(j);
					if (texture) {
						//--- �e�N�X�`�������擾 ---//
						//std::string textureName = texture->GetName();
						std::string textureName = texture->GetRelativeFileName();

						//--- UVSet�����擾 ---//
						std::string UVSetName = texture->UVSet.Get().Buffer();

						//--- UVSet�����r���Ή����Ă���e�N�X�`���Ȃ�ێ� ---//
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
	//--- ���_�J���[�Z�b�g�����擾 ---//
	int vColorLayerCount = mesh->GetElementVertexColorCount();

	//--- ���C���[��������� ---//
	for (int i = 0; vColorLayerCount > i; i++) 
	{
		//--- �@���Z�b�g���擾 ---//
		FbxGeometryElementVertexColor* color = mesh->GetElementVertexColor(i);

		//--- �}�b�s���O���[�h�̎擾
		FbxGeometryElement::EMappingMode mappingMode = color->GetMappingMode();
		//--- ���t�@�����X���[�h�̎擾 ---//
		FbxGeometryElement::EReferenceMode referenceMode = color->GetReferenceMode();

		//--- �}�b�s���O���[�h�̔��� ---//
		switch (mappingMode)
		{
		case FbxGeometryElement::eByControlPoint:
			break;
		case FbxGeometryElement::eByPolygon:
			break;
		case FbxGeometryElement::eByPolygonVertex:
			//--- ���t�@�����X���[�h�̔��� ---//
			switch (referenceMode)
			{
			case FbxGeometryElement::eIndexToDirect:
			{
				FbxLayerElementArrayTemplate<int>* index = &color->GetIndexArray();
				int indexCount = index->GetCount();

				m_VertexColor = new ColorRGBA[indexCount];

				//--- ���_�̐��������_�J���[���擾 ---//
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
