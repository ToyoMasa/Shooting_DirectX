//======================================================================
//	3D���f���̕`��
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "scene3D.h"
#include "texture.h"

static const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL);

//======================================================================
//	���������֐�
//======================================================================
void CScene3D::Init(int texId, float meshSize, int sizeX, int sizeY, int numPrimitive, int numVertex, int numIndex)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	m_NumPrimitive = numPrimitive;
	m_NumVertex = numVertex;

	m_TexId = texId;
	CTexture::Load(m_TexId);

	HRESULT hr;

	// ���_�o�b�t�@							���傫�����ɂ͖��Ȃ�
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * numVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "���_�o�b�t�@�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}

	// �C���f�b�N�X�o�b�t�@					���傫�����ɂ͖��Ȃ�			��size��DWORD�Ȃ�32
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * numIndex, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "�C���f�b�N�X�o�b�t�@�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}

	VERTEX_3D* V;

	// ���_�o�b�t�@
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

	// �}�e���A���̐ݒ�
	m_Mat.Diffuse.r = 1.0f;
	m_Mat.Diffuse.g = 1.0f;
	m_Mat.Diffuse.b = 1.0f;
	m_Mat.Diffuse.a = 1.0f;
	// �A���r�G���g�J���[�i���j
	m_Mat.Ambient.r = 0.8f;
	m_Mat.Ambient.g = 0.7f;
	m_Mat.Ambient.b = 0.9f;
	m_Mat.Ambient.a = 1.0f;

	D3DXMatrixIdentity(&m_World);
}

void CScene3D::Init(int texId, VERTEX_3D* vertex, WORD* index, int numPrimitive, int numVertex, int numIndex)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	m_NumPrimitive = numPrimitive;
	m_NumVertex = numVertex;

	m_TexId = texId;
	CTexture::Load(m_TexId);

	HRESULT hr;

	// ���_�o�b�t�@							���傫�����ɂ͖��Ȃ�
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * numVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "���_�o�b�t�@�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}

	// �C���f�b�N�X�o�b�t�@					���傫�����ɂ͖��Ȃ�			��size��DWORD�Ȃ�32
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * numIndex, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "�C���f�b�N�X�o�b�t�@�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}

	VERTEX_3D* V;

	// ���_�o�b�t�@
	m_VertexBuffer->Lock(0, 0, (void**)&V, D3DLOCK_DISCARD);

	for (int i = 0; i < numVertex; i++)
	{
		V[i] = vertex[i];
	}

	m_VertexBuffer->Unlock();

	m_Vertex = new VERTEX_3D[numVertex];
	for (int i = 0; i < numVertex; i++)
	{
		m_Vertex[i] = vertex[i];
	}

	LPWORD Index;
	m_IndexBuffer->Lock(0, 0, (void**)&Index, D3DLOCK_DISCARD);

	for (int i = 0; i < numIndex; i++)
	{
		Index[i] = index[i];
	}

	m_IndexBuffer->Unlock();

	// �}�e���A���̐ݒ�
	m_Mat.Diffuse.r = 1.0f;
	m_Mat.Diffuse.g = 1.0f;
	m_Mat.Diffuse.b = 1.0f;
	m_Mat.Diffuse.a = 1.0f;
	// �A���r�G���g�J���[�i���j
	m_Mat.Ambient.r = 0.8f;
	m_Mat.Ambient.g = 0.7f;
	m_Mat.Ambient.b = 0.9f;
	m_Mat.Ambient.a = 1.0f;

	D3DXMatrixIdentity(&m_World);
}

//======================================================================
//	�I�������֐�
//======================================================================
void CScene3D::Uninit()
{
	// �e�N�X�`���̉��
	CTexture::Release(m_TexId);

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

	if (m_Vertex != NULL)
	{
		delete m_Vertex;
	}
}

//======================================================================
//	�X�V�֐�
//======================================================================
void CScene3D::Update()
{
}

//======================================================================
//	�`��֐�
//======================================================================
void CScene3D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// FVF(������g�p���钸�_���)�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
	pDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(m_IndexBuffer);

	//�e��s��̐ݒ�(�����̂�肽���Ƃ�);
	pDevice->SetMaterial(&m_Mat);

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTransform(D3DTS_WORLD, &m_World);

	pDevice->SetTexture(0, CTexture::GetTexture(m_TexId));

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_NumVertex, 0, m_NumPrimitive);
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