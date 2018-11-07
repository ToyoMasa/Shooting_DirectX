//======================================================================
//	�r���{�[�h�`��@�i2018/04/25�j
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "camera.h"
#include "scene3D.h"
#include "texture.h"
#include "billboard.h"

//======================================================================
//	�ÓI�����o�ϐ��̏�����
//======================================================================
D3DMATERIAL9			CBillBoard::m_Mat;					// ���f��1�����ɂ�1��
LPDIRECT3DVERTEXBUFFER9	CBillBoard::m_VertexBuffer = NULL;	// ���_�o�b�t�@
LPDIRECT3DINDEXBUFFER9	CBillBoard::m_IndexBuffer = NULL;	// �C���f�b�N�X�o�b�t�@
D3DXMATRIX				CBillBoard::m_World;				// ���[���h�ϊ��s��
D3DXMATRIX				CBillBoard::m_Move;					// ���W�ϊ��s��
D3DXMATRIX				CBillBoard::m_Scale;				// �g��k���s��
CBillBoard*				CBillBoard::m_BillBoards[BILLBOARD_MAX] = { NULL };

//======================================================================
//	�O���[�o���ϐ�
//======================================================================

void CBillBoard::Init()
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
	
	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
	pV[1] = { D3DXVECTOR3( 0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f) };
	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) };
	pV[3] = { D3DXVECTOR3( 0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f) };
	
	m_VertexBuffer->Unlock();
	
	// �C���f�b�N�X�o�b�t�@
	LPWORD pIndex;
	m_IndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
	
	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 3;
	
	m_IndexBuffer->Unlock();
	
	// ���[���h���W�s��̏�����
	D3DXMatrixIdentity(&m_World);
	D3DXMatrixIdentity(&m_Move);
	D3DXMatrixIdentity(&m_Scale);
}

void CBillBoard::Uninit()
{
	CTexture::Release(TEX_ID_CIRCLE);
	CBillBoard::ReleaseAll();

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

void CBillBoard::UpdateAll()
{

}

void CBillBoard::Draw(int textureId, D3DXVECTOR3 vPos, float scale, CCamera* camera)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ���[���h���W�s����Z�b�g
	D3DXMatrixTranslation(&m_Move, vPos.x, vPos.y, vPos.z);
	D3DXMatrixScaling(&m_Scale, scale, scale, scale);

	m_World = m_Scale * m_Move;

	D3DXMATRIX mtxViewRotInv = camera->GetView();

	// �r���[�s��̋t�s����쐬
	// ���s�ړ��𖳌��ɂ���
	mtxViewRotInv._41 = 0.0f;
	mtxViewRotInv._42 = 0.0f;
	mtxViewRotInv._43 = 0.0f;

	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);

	m_World = mtxViewRotInv * m_World;

	pDevice->SetTexture(0, CTexture::GetTexture(textureId));

	//�e��s��̐ݒ�(�����̂�肽���Ƃ���ł��)
	pDevice->SetTransform(D3DTS_WORLD, &m_World);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
}

void CBillBoard::DrawOne(CCamera* camera)
{
	if (m_isVisible)
	{
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
		if (pDevice == NULL)
		{
			return;
		}

		// ���[���h���W�s����Z�b�g
		D3DXMatrixTranslation(&m_Move, m_Pos.x, m_Pos.y, m_Pos.z);
		D3DXMatrixScaling(&m_Scale, m_ScaleX, m_ScaleY, m_ScaleZ);

		m_World = m_Scale * m_Move;

		D3DXMATRIX mtxViewRotInv = camera->GetView();

		// �r���[�s��̋t�s����쐬
		// m_DrawType�Őؑ�
		switch (m_DrawType)
		{
		case 0:
			mtxViewRotInv._41 = 0.0f;
			mtxViewRotInv._42 = 0.0f;
			mtxViewRotInv._43 = 0.0f;
			break;

		case 1:
			mtxViewRotInv._21 = 0.0f;
			mtxViewRotInv._32 = 0.0f;
			mtxViewRotInv._12 = 0.0f;
			mtxViewRotInv._23 = 0.0f;
			mtxViewRotInv._41 = 0.0f;
			mtxViewRotInv._42 = 0.0f;
			mtxViewRotInv._43 = 0.0f;

		default:
			break;
		}

		VERTEX_3D* pV;

		m_VertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

		pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), m_Color, D3DXVECTOR2(0.0f, 0.0f) };
		pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), m_Color, D3DXVECTOR2(1.0f, 0.0f) };
		pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), m_Color, D3DXVECTOR2(0.0f, 1.0f) };
		pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), m_Color, D3DXVECTOR2(1.0f, 1.0f) };

		m_VertexBuffer->Unlock();

		D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);

		// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
		pDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX_3D));
		pDevice->SetIndices(m_IndexBuffer);

		m_World = mtxViewRotInv * m_World;

		pDevice->SetTexture(0, CTexture::GetTexture(m_TextureId));

		//�e��s��̐ݒ�(�����̂�肽���Ƃ���ł��)
		pDevice->SetTransform(D3DTS_WORLD, &m_World);
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
	}
}

void CBillBoard::DrawFixedY(int textureId, D3DXVECTOR3 vPos, float scale, CCamera* camera)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ���[���h���W�s����Z�b�g
	D3DXMatrixTranslation(&m_Move, vPos.x, vPos.y, vPos.z);
	D3DXMatrixScaling(&m_Scale, scale, scale, scale);

	m_World = m_Scale * m_Move;

	D3DXMATRIX mtxViewRotInv = camera->GetView();

	// �r���[�s��̋t�s����쐬
	// ���s�ړ���Y���ȊO�̉�]�𖳌��ɂ���
	mtxViewRotInv._21 = 0.0f;
	mtxViewRotInv._32 = 0.0f;
	mtxViewRotInv._12 = 0.0f;
	mtxViewRotInv._23 = 0.0f;
	mtxViewRotInv._41 = 0.0f;
	mtxViewRotInv._42 = 0.0f;
	mtxViewRotInv._43 = 0.0f;

	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);

	m_World = mtxViewRotInv * m_World;

	pDevice->SetTexture(0, CTexture::GetTexture(textureId));

	//�e��s��̐ݒ�(�����̂�肽���Ƃ���ł��)
	pDevice->SetTransform(D3DTS_WORLD, &m_World);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
}

void CBillBoard::DrawBegin()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ���C�e�B���O�g���Ƃ��ɊO��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// ���e�X�g(3�Z�b�g)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// ���e�X�g��ON/OFF
	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);					// ��2������0�`255�̍D���Ȓl
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// ��2�����͕s����(GREATER�͑�Ȃ�)�A��̒l���傫����΍��i

	// ���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// FVF(������g�p���钸�_���)�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
}

void CBillBoard::DrawEnd()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CBillBoard::DrawAll(CCamera* camera)
{
	CBillBoard::DrawBegin();

	for (int i = 0; i < BILLBOARD_MAX; i++)
	{
		if (CBillBoard::m_BillBoards[i] != NULL)
		{
			CBillBoard::m_BillBoards[i]->DrawOne(camera);
		}
	}
	CBillBoard::DrawEnd();
}

void CBillBoard::Set(int id, int texId, D3DXVECTOR3 pos, float scale, int drawtype)
{
	CBillBoard::m_BillBoards[id]->m_TextureId = texId;
	CBillBoard::m_BillBoards[id]->m_Pos = pos;
	CBillBoard::m_BillBoards[id]->m_ScaleX = scale;
	CBillBoard::m_BillBoards[id]->m_ScaleY = scale;
	CBillBoard::m_BillBoards[id]->m_ScaleZ = scale;
	CBillBoard::m_BillBoards[id]->m_DrawType = drawtype;

	if (drawtype == FIXED_Y)
	{
		CBillBoard::m_BillBoards[id]->m_Pos.y += 0.5f * scale;
	}
}

void CBillBoard::Set(int texId, D3DXVECTOR3 pos, float scale, int drawtype)
{
	m_TextureId = texId;
	m_Pos = pos;
	m_ScaleX = scale;
	m_ScaleY = scale;
	m_ScaleZ = scale;
	m_DrawType = drawtype;

	if (drawtype == FIXED_Y)
	{
		m_Pos.y += 0.5f * scale;
	}
}

void CBillBoard::Set(int texId, D3DXVECTOR3 pos, float scale, int drawtype, D3DCOLOR color)
{
	m_TextureId = texId;
	m_Pos = pos;
	m_ScaleX = scale;
	m_ScaleY = scale;
	m_ScaleZ = scale;
	m_DrawType = drawtype;
	m_Color = color;

	if (drawtype == FIXED_Y)
	{
		m_Pos.y += 0.5f * scale;
	}
}

void CBillBoard::Release()
{
	for (int i = 0; i < BILLBOARD_MAX; i++)
	{
		if (m_BillBoards[i] == this)
		{
			m_BillBoards[i] = NULL;
			delete this;
			break;
		}
	}
}

void CBillBoard::ReleaseAll()
{
	for (int i = 0; i < BILLBOARD_MAX; i++)
	{
		if (m_BillBoards[i] != NULL)
		{
			m_BillBoards[i]->Release();
		}
	}
}

CBillBoard* CBillBoard::Create(int texId)
{
	CBillBoard* billboard = new CBillBoard();
	billboard->Set(texId, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f, 0);

	CTexture::Load(texId);


	return billboard;
}

//typedef struct
//{
//	D3DXVECTOR3 pos;
//	D3DXVECTOR3 NV; // �@��
//	D3DCOLOR color;
//	D3DXVECTOR2 texcoord;
//} VERTEX3D;
//
//static LPDIRECT3DTEXTURE9 g_pTextures[BB_TEXTURE_MAX];
//static D3DMATERIAL9 g_mat; // ���f��1�����ɂ�1��
//static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; //���_�o�b�t�@
//static LPDIRECT3DINDEXBUFFER9  g_pIndexBuffer = NULL;  //�C���f�b�N�X�o�b�t�@
//static D3DXMATRIX g_mtxWorld;			//���[���h�ϊ��s��
//static D3DXMATRIX g_mtxMove;			//���W�ϊ��s��
//static D3DXMATRIX g_mtxScale;			//�g��k���s��
//
//void BillBoardUpdate(void)			// �r���{�[�h�̍X�V
//{
//
//}
//
//void BillBoardDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale)			// �r���{�[�h�̕`��
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	// ���[���h���W�s����Z�b�g
//	D3DXMatrixTranslation(&g_mtxMove, vPos.x, vPos.y, vPos.z);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// �r���[�s��̋t�s����쐬
//	// ���s�ړ���Y���ȊO�̉�]�𖳌��ɂ���
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// �e�N�X�`���̃Z�b�g
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(������g�p���钸�_���)�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//�e��s��̐ݒ�(�����̂�肽���Ƃ���ł��)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//}
//void BillBoardDrawStand(int texNum, D3DXVECTOR3 vPos, float scale)
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	// ���[���h���W�s����Z�b�g
//	D3DXMatrixTranslation(&g_mtxMove, vPos.x, vPos.y + scale * 0.5f, vPos.z);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// �r���[�s��̋t�s����쐬
//	// ���s�ړ���Y���ȊO�̉�]�𖳌��ɂ���
//	mtxViewRotInv._21 = 0.0f;
//	mtxViewRotInv._32 = 0.0f;
//	mtxViewRotInv._12 = 0.0f;
//	mtxViewRotInv._23 = 0.0f;
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// ���C�e�B���O�g���Ƃ��ɊO��
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// ���e�X�g(3�Z�b�g)
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// ���e�X�g��ON/OFF
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);					// ��2������0�`255�̍D���Ȓl
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// ��2�����͕s����(GREATER�͑�Ȃ�)�A��̒l���傫����΍��i
//
//																	// �e�N�X�`���̃Z�b�g
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(������g�p���钸�_���)�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//�e��s��̐ݒ�(�����̂�肽���Ƃ���ł��)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g��ON/OFF
//}
//
//void BillBoardDrawStand(int texNum, float posX, float posY, float posZ, float scale)
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	// ���[���h���W�s����Z�b�g
//	D3DXMatrixTranslation(&g_mtxMove, posX, posY + scale * 0.5f, posZ);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// �r���[�s��̋t�s����쐬
//	// ���s�ړ���Y���ȊO�̉�]�𖳌��ɂ���
//	mtxViewRotInv._21 = 0.0f;
//	mtxViewRotInv._32 = 0.0f;
//	mtxViewRotInv._12 = 0.0f;
//	mtxViewRotInv._23 = 0.0f;
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// ���C�e�B���O�g���Ƃ��ɊO��
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// ���e�X�g(3�Z�b�g)
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// ���e�X�g��ON/OFF
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);					// ��2������0�`255�̍D���Ȓl
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// ��2�����͕s����(GREATER�͑�Ȃ�)�A��̒l���傫����΍��i
//
//																	// �e�N�X�`���̃Z�b�g
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(������g�p���钸�_���)�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//�e��s��̐ݒ�(�����̂�肽���Ƃ���ł��)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g��ON/OFF
//}
//
//// �A�j���[�V�����r���{�[�h�̕`��
//void BillBoardAnimDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale, int tcx, int tcy, int tcw, int tch)
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	int tw = g_aBBTextureFileName[texNum].width;
//	int th = g_aBBTextureFileName[texNum].height;
//	float u0 = (float)tcx / tw;					//�e�N�X�`���̐؂���
//	float v0 = (float)tcy / th;					//
//	float u1 = (float)(tcx + tcw) / tw;
//	float v1 = (float)(tcy + tch) / th;
//
//	VERTEX3D* pV;
//
//	// ���_�o�b�t�@
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u0, v0) };
//	pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u1, v0) };
//	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u0, v1) };
//	pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u1, v1) };
//
//	g_pVertexBuffer->Unlock();
//
//	// ���[���h���W�s����Z�b�g
//	D3DXMatrixTranslation(&g_mtxMove, vPos.x, vPos.y, vPos.z);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// �r���[�s��̋t�s����쐬
//	// ���s�ړ���Y���ȊO�̉�]�𖳌��ɂ���
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// ���C�e�B���O�g���Ƃ��ɊO��
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// ���e�X�g(3�Z�b�g)
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// ���e�X�g��ON/OFF
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 16);					// ��2������0�`255�̍D���Ȓl
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// ��2�����͕s����(GREATER�͑�Ȃ�)�A��̒l���傫����΍��i
//
//																	// �e�N�X�`���̃Z�b�g
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(������g�p���钸�_���)�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//�e��s��̐ݒ�(�����̂�肽���Ƃ���ł��)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g��ON/OFF
//
//																// ���_�o�b�t�@
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
//	pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f) };
//	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) };
//	pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f) };
//
//	g_pVertexBuffer->Unlock();
//}
//
//// �F�ς��r���{�[�h�̕`��
//void BillBoardColorDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale, D3DCOLOR color)
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	VERTEX3D* pV;
//
//	// ���_�o�b�t�@
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), color, D3DXVECTOR2(0.0f, 0.0f) };
//	pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), color, D3DXVECTOR2(1.0f, 0.0f) };
//	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), color, D3DXVECTOR2(0.0f, 1.0f) };
//	pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), color, D3DXVECTOR2(1.0f, 1.0f) };
//
//	g_pVertexBuffer->Unlock();
//
//	// ���[���h���W�s����Z�b�g
//	D3DXMatrixTranslation(&g_mtxMove, vPos.x, vPos.y, vPos.z);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// �r���[�s��̋t�s����쐬
//	// ���s�ړ���Y���ȊO�̉�]�𖳌��ɂ���
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// ���C�e�B���O�g���Ƃ��ɊO��
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// ���e�X�g(3�Z�b�g)
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// ���e�X�g��ON/OFF
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// ��2������0�`255�̍D���Ȓl
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// ��2�����͕s����(GREATER�͑�Ȃ�)�A��̒l���傫����΍��i
//
//																	// �e�N�X�`���̃Z�b�g
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(������g�p���钸�_���)�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//�e��s��̐ݒ�(�����̂�肽���Ƃ���ł��)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g��ON/OFF
//
//																// ���_�o�b�t�@
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
//	pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f) };
//	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) };
//	pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f) };
//
//	g_pVertexBuffer->Unlock();
//}
//
//// �A�j���[�V�����r���{�[�h�̕`��
//void BillBoardAnimDrawCircle(int texNum, float width, float height, D3DXVECTOR3 vPos, float scale, int tcx, int tcy, int tcw, int tch)
//{
//	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
//	if (pDevice == NULL)
//	{
//		return;
//	}
//
//	int tw = g_aBBTextureFileName[texNum].width;
//	int th = g_aBBTextureFileName[texNum].height;
//	float u0 = (float)tcx / tw;					//�e�N�X�`���̐؂���
//	float v0 = (float)tcy / th;					//
//	float u1 = (float)(tcx + tcw) / tw;
//	float v1 = (float)(tcy + tch) / th;
//
//	VERTEX3D* pV;
//
//	// ���_�o�b�t�@
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-width / 2,  height / 2, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u0, v0) };
//	pV[1] = { D3DXVECTOR3(width / 2,  height / 2, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u1, v0) };
//	pV[2] = { D3DXVECTOR3(-width / 2, -height / 2, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u0, v1) };
//	pV[3] = { D3DXVECTOR3(width / 2, -height / 2, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(u1, v1) };
//
//	g_pVertexBuffer->Unlock();
//
//	// ���[���h���W�s����Z�b�g
//	D3DXMatrixTranslation(&g_mtxMove, vPos.x, vPos.y, vPos.z);
//	D3DXMatrixScaling(&g_mtxScale, scale, scale, scale);
//
//	g_mtxWorld = g_mtxScale * g_mtxMove;
//
//	D3DXMATRIX mtxViewRotInv = GetView();
//
//	// �r���[�s��̋t�s����쐬
//	// ���s�ړ���Y���ȊO�̉�]�𖳌��ɂ���
//	mtxViewRotInv._41 = 0.0f;
//	mtxViewRotInv._42 = 0.0f;
//	mtxViewRotInv._43 = 0.0f;
//
//	D3DXMatrixTranspose(&mtxViewRotInv, &mtxViewRotInv);
//
//	g_mtxWorld = mtxViewRotInv * g_mtxWorld;
//
//	// ���C�e�B���O�g���Ƃ��ɊO��
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// ���e�X�g(3�Z�b�g)
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// ���e�X�g��ON/OFF
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 16);					// ��2������0�`255�̍D���Ȓl
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// ��2�����͕s����(GREATER�͑�Ȃ�)�A��̒l���傫����΍��i
//
//																	// �e�N�X�`���̃Z�b�g
//	pDevice->SetTexture(0, g_pTextures[texNum]);
//
//	// FVF(������g�p���钸�_���)�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐ݒ�
//	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(VERTEX3D));
//	pDevice->SetIndices(g_pIndexBuffer);
//
//	//�e��s��̐ݒ�(�����̂�肽���Ƃ���ł��)
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
//
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g��ON/OFF
//
//																// ���_�o�b�t�@
//	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);
//
//	pV[0] = { D3DXVECTOR3(-0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f) };
//	pV[1] = { D3DXVECTOR3(0.5,  0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f) };
//	pV[2] = { D3DXVECTOR3(-0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f) };
//	pV[3] = { D3DXVECTOR3(0.5, -0.5, 0), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f) };
//
//	g_pVertexBuffer->Unlock();
//}
//
//// ��]���Ȃ��r���{�[�h�̕`��
//void BillBoardStaticDrawCircle(int texNum, D3DXVECTOR3 vPos, float scale)
//{
//}
//
//CBillBoard::CBillBoard()
//{
//	m_Texture = NULL;
//	m_VertexBuffer = NULL;
//	m_IndexBuffer = NULL;
//}
//
//CBillBoard::~CBillBoard()
//{
//}
//
//void CBillBoard::Init()
//{
//
//}
//
//void CBillBoard::Uninit()
//{
//	//���_�o�b�t�@�̉��
//	if (m_VertexBuffer != NULL)
//	{
//		m_VertexBuffer->Release();
//		m_VertexBuffer = NULL;
//	}
//
//	//�C���f�b�N�X�o�b�t�@�̉��
//	if (m_IndexBuffer != NULL)
//	{
//		m_IndexBuffer->Release();
//		m_IndexBuffer = NULL;
//	}
//}
