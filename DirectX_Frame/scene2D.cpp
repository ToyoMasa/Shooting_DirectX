//======================================================================
//	2D���f���̕`��
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "scene2D.h"
#include "texture.h"

//======================================================================
//	���������֐�
//======================================================================
void CScene2D::Init(int texId, float texW, float texH)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	m_TexId = texId;
	CTexture::Load(m_TexId);

	m_isUseShadow = false;

	m_TexSize = D3DXVECTOR2(texW, texH);
}

//======================================================================
//	�I�������֐�
//======================================================================
void CScene2D::Uninit()
{
	// �e�N�X�`���̉��
	CTexture::Release(m_TexId);
}

//======================================================================
//	�X�V�֐�
//======================================================================
void CScene2D::Update()
{
}

//======================================================================
//	�`��֐�
//======================================================================
void CScene2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	VERTEX_2D vertex[4];
	vertex[0].pos = D3DXVECTOR4(m_Pos.x - m_TexSize.x / 2 - 0.5f, m_Pos.y - m_TexSize.y / 2, 0, 1);
	vertex[1].pos = D3DXVECTOR4(m_Pos.x + m_TexSize.x / 2 - 0.5f, m_Pos.y - m_TexSize.y / 2, 0, 1);
	vertex[2].pos = D3DXVECTOR4(m_Pos.x - m_TexSize.x / 2 - 0.5f, m_Pos.y + m_TexSize.y / 2, 0, 1);
	vertex[3].pos = D3DXVECTOR4(m_Pos.x + m_TexSize.x / 2 - 0.5f, m_Pos.y + m_TexSize.y / 2, 0, 1);

	vertex[0].texcoord = D3DXVECTOR2(m_TexCoordX1, m_TexCoordY1);
	vertex[1].texcoord = D3DXVECTOR2(m_TexCoordX2, m_TexCoordY1);
	vertex[2].texcoord = D3DXVECTOR2(m_TexCoordX1, m_TexCoordY2);
	vertex[3].texcoord = D3DXVECTOR2(m_TexCoordX2, m_TexCoordY2);

	vertex[0].color =
		vertex[1].color =
		vertex[2].color =
		vertex[3].color = m_Color;

	pDevice->SetTexture(0, CTexture::GetTexture(m_TexId));

	// ���e�X�g(3�Z�b�g)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// ���e�X�g��ON
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ��2������0�`255�̍D���Ȓl
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// ��2�����͕s����(GREATER�͑�Ȃ�)�A��̒l���傫����΍��i
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vertex[0], sizeof(VERTEX_2D));

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// ���e�X�g��OFF
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CScene2D::SetTexCoord(float texCoordx1, float texCoordx2, float texCoordy1, float texCoordy2)
{
	m_TexCoordX1 = texCoordx1;
	m_TexCoordX2 = texCoordx2;
	m_TexCoordY1 = texCoordy1;
	m_TexCoordY2 = texCoordy2;
}

//======================================================================
//	�t�@�N�g��
//======================================================================
CScene2D* CScene2D::Create(int texid, float texW, float texH)
{
	CScene2D *scene2D = new CScene2D(LAYER_OBJECT2D);
	scene2D->Init(texid, texW, texH);

	return scene2D;
}
