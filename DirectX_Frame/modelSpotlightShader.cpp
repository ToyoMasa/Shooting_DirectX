#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "spotlightShader.h"
#include "modelSpotlightShader.h"

CShaderModelSpotlight *CShaderModelSpotlight::m_Shader = NULL;

CShaderModelSpotlight::CShaderModelSpotlight() : CShader()
{
	bool sts;
	sts = VertexShaderCompile(
		SHADER_FILE[SH_ID_MODEL_SPOTLIGHT].c_str(),						// �V�F�[�_�[�t�@�C����
		"main",							// �G���g���[�֐���
		"vs_3_0");						// �o�[�W����

	if (!sts)
	{
		MessageBox(NULL, "�G���[", "�G���[", MB_OK);
	}

	// �s�N�Z���V�F�[�_�[�R���p�C��
	sts = PixelShaderCompile(
		SHADER_FILE[SH_ID_MODEL_SPOTLIGHT].c_str(),						// �V�F�[�_�[�t�@�C����
		"PS",							// �G���g���[�֐���
		"ps_3_0");						// �o�[�W����

	if (!sts)
	{
		MessageBox(NULL, "�ǂݍ��݃G���[", "�ǂݍ��݃G���[", MB_OK);
	}
}

CShaderModelSpotlight* CShaderModelSpotlight::GetShader()
{
	if (m_Shader == NULL)
	{
		m_Shader = new CShaderModelSpotlight();
	}

	return m_Shader;
}

void CShaderModelSpotlight::Destroy()
{
	if (m_Shader != NULL)
	{
		delete m_Shader;
		m_Shader = NULL;
	}
}

void CShaderModelSpotlight::ShaderSet(const D3DXMATRIX& world)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ���̐ݒ���
	D3DXVECTOR4		lightDir(CManager::GetCamera()->GetFront(), 0.0f);		// ���̕���
	D3DXVECTOR4		diffuse(DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR);			// ���s�����̐F
	D3DXVECTOR4		ambient(DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR);			// ����
	D3DXVECTOR4		specular(1.0f, 1.0f, 1.0f, 0.01f);			// �X�y�L������
	D3DXVECTOR4		cameraPos = D3DXVECTOR4(CManager::GetCamera()->GetPos(), 0.0f);
	float			lightStrength = 30.0f;

	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
	pDevice->SetVertexShader(m_VertexShader);
	pDevice->SetPixelShader(m_PixelShader);

	// �萔���Z�b�g(���_�V�F�[�_�[)
	m_VSConstantTable->SetMatrix(pDevice, "g_world", &world);
	m_VSConstantTable->SetMatrix(pDevice, "g_view", &CManager::GetCamera()->GetView());
	m_VSConstantTable->SetMatrix(pDevice, "g_projection", &CManager::GetCamera()->GetProjection());

	m_PSConstantTable->SetVector(pDevice, "g_light_pos", &cameraPos);
	m_PSConstantTable->SetVector(pDevice, "g_light_dir", &lightDir);
	m_PSConstantTable->SetVector(pDevice, "g_light_diff", &diffuse);
	m_PSConstantTable->SetVector(pDevice, "g_light_specular", &specular);
	m_PSConstantTable->SetVector(pDevice, "g_light_ambient", &ambient);
	m_PSConstantTable->SetVector(pDevice, "g_falloff_param", &FALLOFF_PARAM);
	m_PSConstantTable->SetVector(pDevice, "g_light_param", &SPOTLIGHT_PARAM);

	m_PSConstantTable->SetVector(pDevice, "g_light_pos", &cameraPos);
	m_PSConstantTable->SetVector(pDevice, "g_camera_pos", &cameraPos);
}

void CShaderModelSpotlight::SetMaterial(const D3DMATERIAL9& mat)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	D3DXVECTOR4  tempcolor;

	//�����p�̃}�e���A�����Z�b�g
	tempcolor.x = 0.13f;
	tempcolor.y = 0.13f;
	tempcolor.z = 0.13f;
	tempcolor.w = 1.0f;
	m_PSConstantTable->SetVector(pDevice, "g_mat_ambient", &tempcolor);

	// �f�B�t���[�Y���p�̃}�e���A�����Z�b�g
	tempcolor.x = 0.6f;
	tempcolor.y = 0.6f;
	tempcolor.z = 0.6f;
	tempcolor.w = 1.0f;
	m_PSConstantTable->SetVector(pDevice, "g_mat_diffuse", &tempcolor);

	// �G�~�b�V�u���p�̃}�e���A�����Z�b�g
	tempcolor.x = 0.5f;
	tempcolor.y = 0.5f;
	tempcolor.z = 0.5f;
	tempcolor.w = 1.0f;
	m_PSConstantTable->SetVector(pDevice, "g_mat_emissive", &tempcolor);

	// �X�y�L�����[���p�̃}�e���A�����Z�b�g
	tempcolor.x = 0.05f;
	tempcolor.y = 0.05f;
	tempcolor.z = 0.05f;
	tempcolor.w = 1.0f;
	m_PSConstantTable->SetVector(pDevice, "g_mat_specular", &tempcolor);

	// �p���[�l���Z�b�g
	m_PSConstantTable->SetFloat(pDevice, "g_mat_power", 50.0f);
	m_PSConstantTable->SetFloat(pDevice, "g_alpha", 1.0f);
}