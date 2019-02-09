#include "common.h"
#include "renderer.h"
#include "billboard.h"
#include "shadowShader.h"
#include "skinmeshShadowShader.h"
#include "fieldShadowShader.h"

//======================================================================
//	�ÓI�����o�ϐ��̏�����
//======================================================================
CScene *CScene::m_Scene[PRIORITY_MAX][OBJECT_MAX] = { NULL };

CScene::CScene(int priority)
{
	for (int i = 0; i < OBJECT_MAX; i++)
	{
		if (m_Scene[priority][i] == NULL)
		{
			m_Scene[priority][i] = this;
			m_Type = SCENE_TYPE_NONE;
			m_Visible = true;
			m_Shader = NULL;
			break;
		}
	}
}

void CScene::Set(const D3DXVECTOR3& pos)
{
	m_Pos = pos;
}

void CScene::Release()
{
	for (int j = 0; j < PRIORITY_MAX; j++)
	{
		for (int i = 0; i < OBJECT_MAX; i++)
		{
			if (m_Scene[j][i] == this)
			{
				this->m_Shader = NULL;
				Uninit();
				m_Scene[j][i] = NULL;
				delete this;
				break;
			}
		}
	}
}

void CScene::UpdateAll()
{
	for (int j = 0; j < PRIORITY_MAX; j++)
	{
		for (int i = 0; i < OBJECT_MAX; i++)
		{
			if (m_Scene[j][i] != NULL)
			{
				m_Scene[j][i]->Update();
			}
		}
	}
}

void CScene::DrawAll()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ���e�X�g(3�Z�b�g)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// ���e�X�g��ON/OFF
	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);					// ��2������0�`255�̍D���Ȓl
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// ��2�����͕s����(GREATER�͑�Ȃ�)�A��̒l���傫����΍��i

	for (int j = 0; j < PRIORITY_MAX; j++)
	{
		switch (j)
		{
		case LAYER_BACKGROUND:
			// FVF(������g�p���钸�_���)�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);
			break;
		case LAYER_OBJECT2D:
			pDevice->SetFVF(FVF_VERTEX_2D);
			break;
		case LAYER_OBJECT3D:
			pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
			CBillBoard::DrawAll();
			pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON

		default:
			// FVF(������g�p���钸�_���)�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			break;
		}
		for (int i = 0; i < OBJECT_MAX; i++)
		{
			if (m_Scene[j][i] != NULL)
			{
				if (m_Scene[j][i]->m_Visible)
				{  
					if (m_Scene[j][i]->m_Shader != NULL)
					{
						m_Scene[j][i]->DrawWithShader();
					}
					else
					{
						m_Scene[j][i]->Draw();
					}
				}
			}
		}
	}

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g��ON/OFF
}

void CScene::DrawShadowAll()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int j = 0; j < PRIORITY_MAX; j++)
	{
		switch (j)
		{
		case LAYER_BACKGROUND:
			// FVF(������g�p���钸�_���)�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);
			break;
		case LAYER_OBJECT2D:
			pDevice->SetFVF(FVF_VERTEX_2D);
			break;
		default:
			// FVF(������g�p���钸�_���)�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			break;
		}
		for (int i = 0; i < OBJECT_MAX; i++)
		{
			if (m_Scene[j][i] != NULL)
			{
				if (m_Scene[j][i]->m_Visible)
				{
					if (m_Scene[j][i]->GetUseShadow())
					{
						CShader* save = m_Scene[j][i]->GetShader();
						if (j == LAYER_SKINMESH)
						{
							m_Scene[j][i]->SetShader(CShaderSkinmeshShadow::GetShader());
						}
						else
						{
							m_Scene[j][i]->SetShader(CShaderShadow::GetShader());
						}
						m_Scene[j][i]->DrawShadow();
						m_Scene[j][i]->SetShader(save);
					}
				}
			}
		}
	}

	for (int j = 0; j < PRIORITY_MAX; j++)
	{
		if (j == LAYER_BACKGROUND)
		{
			for (int i = 0; i < OBJECT_MAX; i++)
			{
				if (m_Scene[j][i] != NULL)
				{
					if (m_Scene[j][i]->m_Visible)
					{
						if (m_Scene[j][i]->GetUseShadow())
						{
							CShader* save = m_Scene[j][i]->GetShader();
							m_Scene[j][i]->SetShader(CShaderFieldShadow::GetShader());
							m_Scene[j][i]->DrawShadow();
							m_Scene[j][i]->SetShader(save);
						}
					}
				}
			}
			break;
		}
	}

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g��ON/OFF
}

void CScene::ReleaseAll()
{
	for (int j = 0; j < PRIORITY_MAX; j++)
	{
		for (int i = 0; i < OBJECT_MAX; i++)
		{
			if (m_Scene[j][i] != NULL)
			{
				m_Scene[j][i]->Release();
			}
		}
	}
}
