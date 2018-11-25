//======================================================================
//	3D���f���̕`��
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "sceneModel.h"

//======================================================================
//	���������֐�
//======================================================================
void CSceneModel::Init(const std::string& modelName)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	HRESULT hr;

	LPD3DXBUFFER pAdjacency = NULL;	// �אڏ��(�œK���ɕK�v)

	hr = D3DXLoadMeshFromX(modelName.c_str(), D3DXMESH_SYSTEMMEM, pDevice, &pAdjacency, &m_Material, NULL, &m_MaterialNum, &m_Mesh);

	if (FAILED(hr))
	{
		MessageBox(NULL, "x���f���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}

	//X�t�@�C���ɖ@�����Ȃ��ꍇ�́A�@������������
	if (!(m_Mesh->GetFVF() & D3DFVF_NORMAL)) {

		ID3DXMesh* pTempMesh = NULL;

		m_Mesh->CloneMeshFVF(m_Mesh->GetOptions(),
			m_Mesh->GetFVF() | D3DFVF_NORMAL, pDevice, &pTempMesh);

		D3DXComputeNormals(pTempMesh, NULL);
		m_Mesh->Release();
		m_Mesh = pTempMesh;
	}

	// ���b�V���̍œK��
	hr = m_Mesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacency->GetBufferPointer(),
		NULL, NULL, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "�œK���Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}

	//// �N���[����
	//LPD3DXMESH pCloneMesh;
	//D3DVERTEXELEMENT9 elements[MAXD3DDECLLENGTH + 1];
	//m_Mesh->GetDeclaration(elements);

	//// VRAM��ɃN���[������i�R�s�[����j
	//hr = m_Mesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY | D3DXMESH_32BIT, elements, pDevice, &pCloneMesh);

	//if (FAILED(hr))
	//{
	//	MessageBox(NULL, "�N���[���Ɏ��s���܂���", "�G���[", MB_OK);
	//	return;
	//}
	//// �N���[��������̂Ō��̂��������ăN���[����̃A�h���X�����炤
	//if (m_Mesh != NULL)
	//{
	//	m_Mesh->Release();
	//	m_Mesh = pCloneMesh;
	//}

	AddTangentSpace();

	// ���������
	if (pAdjacency != NULL)
	{
		pAdjacency->Release();
		pAdjacency = NULL;
	}

	//	pMaterials[i].MatD3D�Ń}�e���A���^���擾�ł���i�c�[���ɂ���ă}�e���A���̎�ނ��Ⴄ���߉��H���K�v�ȏꍇ������j
	//	pMaterials[i].pTextureFilename�Ƀt�@�C�����������Ă�(�p�X��ǉ����ăe�N�X�`�����[�h�֐��ցANULL��������e�N�X�`���Ȃ�)
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
				MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
			}
		}
	}
}

//======================================================================
//	�I�������֐�
//======================================================================
void CSceneModel::Uninit()
{
	for (int i = 0; i < (int)m_MaterialNum; i++)
	{
		// �e�N�X�`���̉��
		if (m_Texture[i] != NULL)
		{
			m_Texture[i]->Release();
			m_Texture[i] = NULL;
		}
	}

	delete m_Texture;

	// ���b�V���̉��
	if (m_Mesh != NULL)
	{
		m_Mesh->Release();
		m_Mesh = NULL;
	}

	// �}�e���A���̉��
	if (m_Material != NULL)
	{
		m_Material->Release();
		m_Material = NULL;
	}

	// �@���}�b�v�e�N�X�`���̉��
	if (m_NormalmapTexture != NULL)
	{
		m_NormalmapTexture->Release();
		m_NormalmapTexture = NULL;
	}
}

//======================================================================
//	�X�V�֐�
//======================================================================
void CSceneModel::Update()
{
}

//======================================================================
//	�`��֐�
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

	// ���C�g�𖳎����郂�f���Ȃ烉�C�g�𖳌���
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

	// �T�u�Z�b�g��`�悷��(�}�e���A���������[�v)
	for (int i = 0; i < (int)m_MaterialNum; i++)
	{
		pDevice->SetMaterial(&pMaterials[i].MatD3D);
		m_Shader->SetMaterial(pMaterials[i].MatD3D);
		
		// �e�N�X�`�����g�p���邩�ۂ����Z�b�g
		if (pMaterials[i].pTextureFilename != NULL)
		{
			m_Shader->GetPSTable()->SetBool(pDevice, "g_tex", TRUE);
			// �e�N�X�`�����T���v���[�փZ�b�g
			int index = m_Shader->GetPSTable()->GetSamplerIndex("Sampler1");
			pDevice->SetTexture(index, m_Texture[i]);
			
			// �@���}�b�v�������Ă���Ȃ�V�F�[�_�[�ɓn��
			if (m_NormalmapTexture != NULL)
			{
				// �e�N�X�`�����T���v���[�փZ�b�g
				index = m_Shader->GetPSTable()->GetSamplerIndex("NormalmapSampler1");
				pDevice->SetTexture(index, m_NormalmapTexture);
			}
		}
		else
		{
			m_Shader->GetPSTable()->SetBool(pDevice, "g_tex", FALSE);
			// �e�N�X�`�����T���v���[�փZ�b�g
			int index = m_Shader->GetPSTable()->GetSamplerIndex("Sampler1");
			pDevice->SetTexture(index, NULL);

			if (m_NormalmapTexture != NULL)
			{
				// �e�N�X�`�����T���v���[�փZ�b�g
				index = m_Shader->GetPSTable()->GetSamplerIndex("NormalmapSampler1");
				pDevice->SetTexture(index, NULL);
			}
		}

		m_Mesh->DrawSubset(i);					// �T�u�Z�b�g�̕`��
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�����Z�b�g
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
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
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

// �]�@���A�ڃx�N�g���̂Ȃ����b�V���ɏ]�@���A�ڃx�N�g����ǉ�����
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
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },				// �ʒu���
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },				// �e�N�X�`�����W
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },				// �@��
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },				// �ڃx�N�g��
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },				// �]�@���x�N�g��
		D3DDECL_END()
	};

	// �w��t�H�[�}�b�g�̃N���[�����b�V�������
	hr = m_Mesh->CloneMesh(m_Mesh->GetOptions(), Decl, pDevice, &tempMesh);
	if (FAILED(hr)) {
		MessageBox(NULL, "ERROR!!", "CloneMesh�G���[", MB_OK);
		return false;
	}

	// �@���x�N�g�����v�Z
	hr = D3DXComputeNormals(tempMesh, NULL);
	if (FAILED(hr)) {
		tempMesh->Release();
		MessageBox(NULL, "ERROR!!", "D3DXComputeNormals�G���[", MB_OK);
		return false;
	}

	// �]�@���A�ڃx�N�g���������b�V�����쐬����
	hr = D3DXComputeTangentFrameEx(tempMesh,
		D3DDECLUSAGE_TEXCOORD, 0,		// 0�Ԗڂ̃e�N�X�`�����W���g�p���� 
		D3DDECLUSAGE_TANGENT, 0,		// 0�Ԗڂ̐ڃx�N�g�����v�Z����
		D3DDECLUSAGE_BINORMAL, 0,		// 0�Ԗڂ̏]�@���x�N�g�����v�Z����
		D3DDECLUSAGE_NORMAL, 0,			// 0�Ԗڂ̖@���x�N�g�����v�Z����
		0,								// �v�Z�̃I�v�V����
		NULL,							// 
		-1.01f,							// �אڂ���R�p�`��臒l
		-0.01f,							// �P�ƂƂ݂Ȃ���钸�_��臒l
		-1.01f,							// �@����臒l
		&outMesh,						// �������ꂽ���b�V��
		NULL);							//  
	if (FAILED(hr)) {
		tempMesh->Release();
		MessageBox(NULL, "ERROR!!", "D3DXComputeTangentFrameEx�G���[", MB_OK);
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
