//======================================================================
//	3D���f���̕`��
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "sceneModel.h"

static const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL);

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

	// �N���[����
	LPD3DXMESH pCloneMesh;
	D3DVERTEXELEMENT9 elements[MAXD3DDECLLENGTH + 1];
	m_Mesh->GetDeclaration(elements);

	// VRAM��ɃN���[������i�R�s�[����j
	hr = m_Mesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY | D3DXMESH_32BIT, elements, pDevice, &pCloneMesh);

	if (FAILED(hr))
	{
		MessageBox(NULL, "�N���[���Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}
	// �N���[��������̂Ō��̂��������ăN���[����̃A�h���X�����炤
	if (m_Mesh != NULL)
	{
		m_Mesh->Release();
		m_Mesh = pCloneMesh;
	}

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
	for (int i = 0; i < m_MaterialNum; i++)
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

	// FVF(������g�p���钸�_���)�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���C�g�𖳎����郂�f���Ȃ烉�C�g�𖳌���
	if (m_isIgnoreLight)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

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
