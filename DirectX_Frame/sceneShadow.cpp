//======================================================================
//	�ۉe�@�i2018/08/28)
//	Author : �L�� ���r
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

	// ���_�o�b�t�@							���傫�����ɂ͖��Ȃ�
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "���_�o�b�t�@�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}

	// �C���f�b�N�X�o�b�t�@					���傫�����ɂ͖��Ȃ�					��size��DWORD�Ȃ�32
	hr = pDevice->CreateIndexBuffer(sizeof(WORD) * 4, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "�C���f�b�N�X�o�b�t�@�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}

	VERTEX_3D* pV;

	// ���_�o�b�t�@
	m_VertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	pV[0] = { D3DXVECTOR3(-0.5, 0,  0.5), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(64, 64, 64, 255), D3DXVECTOR2(0.0f, 0.0f) };
	pV[1] = { D3DXVECTOR3( 0.5, 0,  0.5), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(64, 64, 64, 255), D3DXVECTOR2(1.0f, 0.0f) };
	pV[2] = { D3DXVECTOR3(-0.5, 0, -0.5), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(64, 64, 64, 255), D3DXVECTOR2(0.0f, 1.0f) };
	pV[3] = { D3DXVECTOR3( 0.5, 0, -0.5), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(64, 64, 64, 255), D3DXVECTOR2(1.0f, 1.0f) };

	m_VertexBuffer->Unlock();

	// �C���f�b�N�X�o�b�t�@
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
	// �e�N�X�`���̉��
	CTexture::Release(TEX_ID_CIRCLE);

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

	// ���C�e�B���O�𖳌�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���e�X�g(3�Z�b�g)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// ���e�X�g��ON/OFF
	pDevice->SetRenderState(D3DRS_ALPHAREF, 32);					// ��2������0�`255�̍D���Ȓl
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// ��2�����͕s����(GREATER�͑�Ȃ�)�A��̒l���傫����΍��i

	// ���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// FVF(������g�p���钸�_���)�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
	pDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(m_IndexBuffer);

	// �e�N�X�`���̃Z�b�g
	pDevice->SetTexture(0, CTexture::GetTexture(TEX_ID_CIRCLE));

	//�e��s��̐ݒ�(�����̂�肽���Ƃ���ł��)
	pDevice->SetTransform(D3DTS_WORLD, &m_World);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g��ON/OFF
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
