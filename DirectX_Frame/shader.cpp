#include "common.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "sceneModel.h"
#include "box.h"

CCamera		*CShader::m_pCamera = NULL;
CShader		*CShader::m_Shaders[SHADER_FILE_MAX] = { NULL };

void CShader::Init(int id)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	m_pCamera = CManager::GetCamera();

	D3DXCreateEffectFromFile(
		pDevice,
		SHADER_FILE[id].c_str(),
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		&m_pEffect,
		NULL
	);

	HRESULT hr;

	do {
		hr = m_pEffect->FindNextValidTechnique(m_hTech, &m_hTechNext);
		if (FAILED(hr))
		{
			return;	// �G���[
		}

		if (m_hTechNext)
		{
			// �e�N�j�b�N�̃n���h��(m_hTechNext)���擾
			D3DXTECHNIQUE_DESC TechDesc;
			m_pEffect->GetTechniqueDesc(m_hTechNext, &TechDesc); // �e�N�j�b�N�̖��O���擾
																 // TechDesc.Name�Ƀe�N�j�b�N���������Ă�
		}
		m_hTech = m_hTechNext;
	} while (m_hTech != NULL);
}

void CShader::Uninit()
{
	if (m_pEffect)
	{
		m_pEffect->Release();
	}

}

void CShader::Draw(CScene* scene)
{
	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, scene->GetPos().x, scene->GetPos().y, scene->GetPos().z);
	m_World = world;

	if (m_pCamera != NULL)
	{
		m_pEffect->SetMatrix("World", &m_World);
		m_pEffect->SetMatrix("View", &m_pCamera->GetView());
		m_pEffect->SetMatrix("Proj", &m_pCamera->GetProjection());

		D3DXHANDLE hTVPShader;		// �e�N�j�b�N�uTVPShader�v�̃n���h��
		hTVPShader = m_pEffect->GetTechniqueByName("TVPShader");
		if (hTVPShader == NULL)
		{
			return;	// �e�N�j�b�N�uTVPShader�v��������Ȃ�����
		}

		m_pEffect->SetTechnique(hTVPShader);	// �n���h�����g���ăe�N�j�b�N�uTVPShader�v��I������

		UINT numPass;

		HRESULT hr;

		hr = m_pEffect->Begin(&numPass, 0);

		if (FAILED(hr))
		{
			MessageBox(NULL, "�e�N�j�b�N�̊J�n�Ɏ��s���܂���", "�G���[", MB_OK);
			return;	// �e�N�j�b�N�̊J�n�Ɏ��s
		}

		for (UINT iPass = 0; iPass < numPass; iPass++)
		{
			hr = m_pEffect->BeginPass(iPass);	// iPass�Ԗڂ̃p�X�̎��s���J�n
			if (FAILED(hr))
			{
				MessageBox(NULL, "�p�X�̎��s�Ɏ��s���܂���", "�G���[", MB_OK);
				break;	// �p�X�̎��s�Ɏ��s
			}

			// ��BeginPass�̌��ID3DXEffect::Set�`���\�b�h���g�����ꍇ�K�v�ɂȂ�
			// m_pEffect->CommitChanges(); // (���̃R�[�h��ł͂Ȃ�)

			// �����Ŏ��ۂɃ��b�V����`�悷��
			scene->Draw();

			m_pEffect->EndPass();
		}

		hr = m_pEffect->End();
		if (FAILED(hr))
		{

		}
	}
}

void CShader::Draw(CScene* scene, LPD3DXMESH mesh, DWORD id)
{
	D3DXMATRIX world;
	CSceneModel* temp = (CSceneModel*)scene;

	if (m_pCamera != NULL)
	{
		m_pEffect->SetMatrix("View", &m_pCamera->GetView());
		m_pEffect->SetMatrix("Proj", &m_pCamera->GetProjection());

		D3DXHANDLE hTVPShader;		// �e�N�j�b�N�uTVPShader�v�̃n���h��
		hTVPShader = m_pEffect->GetTechniqueByName("TVPShader");
		if (hTVPShader == NULL)
		{
			return;	// �e�N�j�b�N�uTVPShader�v��������Ȃ�����
		}

		m_pEffect->SetTechnique(hTVPShader);	// �n���h�����g���ăe�N�j�b�N�uTVPShader�v��I������

		UINT numPass;

		HRESULT hr;

		hr = m_pEffect->Begin(&numPass, 0);

		if (FAILED(hr))
		{
			MessageBox(NULL, "�e�N�j�b�N�̊J�n�Ɏ��s���܂���", "�G���[", MB_OK);
			return;	// �e�N�j�b�N�̊J�n�Ɏ��s
		}

		for (UINT iPass = 0; iPass < numPass; iPass++)
		{
			hr = m_pEffect->BeginPass(iPass);	// iPass�Ԗڂ̃p�X�̎��s���J�n
			if (FAILED(hr))
			{
				MessageBox(NULL, "�p�X�̎��s�Ɏ��s���܂���", "�G���[", MB_OK);
				break;	// �p�X�̎��s�Ɏ��s
			}

			// ��BeginPass�̌��ID3DXEffect::Set�`���\�b�h���g�����ꍇ�K�v�ɂȂ�
			// m_pEffect->CommitChanges(); // (���̃R�[�h��ł͂Ȃ�)

			// �����Ŏ��ۂɃ��b�V����`�悷��
			mesh->DrawSubset(id);

			m_pEffect->EndPass();
		}

		hr = m_pEffect->End();
		if (FAILED(hr))
		{
			MessageBox(NULL, "�V�F�[�_�[�̓K�p�Ɏ��s���܂���", "�G���[", MB_OK);
		}
	}
}

void CShader::SetWorld(D3DXMATRIX world)
{
	m_pEffect->SetMatrix("World", &world);
}

void CShader::SetTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_pEffect->SetTexture(0, texture);
}

void CShader::LoadShader()
{
	// �V�F�[�_�[�t�@�C���ǂݍ���
	for (int i = 0; i < SHADER_FILE_MAX; i++)
	{
		if (m_Shaders[i] == NULL)
		{
			m_Shaders[i] = new CShader();
			m_Shaders[i]->Init(i);
		}
	}
}

void CShader::ReleaseShader()
{
	// �V�F�[�_�[�t�@�C���̉��
	for (int i = 0; i < SHADER_FILE_MAX; i++)
	{
		if (m_Shaders[i] != NULL)
		{
			m_Shaders[i]->Uninit();
			delete m_Shaders[i];
			m_Shaders[i] = NULL;
		}
	}
}
