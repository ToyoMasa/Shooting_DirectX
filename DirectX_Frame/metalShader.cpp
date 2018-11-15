#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "metalShader.h"

CShaderMetal *CShaderMetal::m_Shader = NULL;

CShaderMetal::CShaderMetal() : CShader()
{
	bool sts;
	sts = VertexShaderCompile(
		"data/shaders/metal.fx",						// �V�F�[�_�[�t�@�C����
		"main",							// �G���g���[�֐���
		"vs_3_0");						// �o�[�W����

	if (!sts) {
		MessageBox(NULL, "�G���[", "�G���[", MB_OK);
	}

	// �s�N�Z���V�F�[�_�[�R���p�C��
	sts = PixelShaderCompile(
		"data/shaders/metal.fx",						// �V�F�[�_�[�t�@�C����
		"PS",							// �G���g���[�֐���
		"ps_3_0");						// �o�[�W����

	if (!sts) {
		MessageBox(NULL, "�ǂݍ��݃G���[", "�ǂݍ��݃G���[", MB_OK);
	}
}

CShaderMetal* CShaderMetal::GetShader()
{
	if (m_Shader == NULL)
	{
		m_Shader = new CShaderMetal();
	}

	return m_Shader;
}

void CShaderMetal::Destroy()
{
	if (m_Shader)
	{
		delete m_Shader;
	}
}

void CShaderMetal::ShaderSet(D3DXMATRIX world)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ���̐ݒ���
	D3DXVECTOR4		light_dir(-1.0f, -1.0f, 1.0f, 0.0f);		// ���̕���
	D3DXVECTOR4		diffuse(DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR);			// ���s�����̐F
	D3DXVECTOR4		ambient(DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR);			// ����
	D3DXVECTOR4		specular(1.0f, 1.0f, 1.0f, 0.01f);			// �X�y�L������
	D3DXVECTOR4		cameraPos = D3DXVECTOR4(CManager::GetCamera()->GetPos().x, CManager::GetCamera()->GetPos().y, CManager::GetCamera()->GetPos().z, 0.0f);

	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
	pDevice->SetVertexShader(m_VertexShader);
	pDevice->SetPixelShader(m_PixelShader);

	// �萔���Z�b�g(���_�V�F�[�_�[)
	m_VSConstantTable->SetMatrix(pDevice, "g_world", &world);
	m_VSConstantTable->SetMatrix(pDevice, "g_view", &CManager::GetCamera()->GetView());
	m_VSConstantTable->SetMatrix(pDevice, "g_projection", &CManager::GetCamera()->GetProjection());	

	m_PSConstantTable->SetVector(pDevice, "g_diffuse", &diffuse);
	m_PSConstantTable->SetVector(pDevice, "g_ambient", &ambient);
	m_PSConstantTable->SetVector(pDevice, "g_specular", &specular);
	m_PSConstantTable->SetVector(pDevice, "g_light_dir", &light_dir);

	m_PSConstantTable->SetVector(pDevice, "g_camerapos", &cameraPos);
	m_PSConstantTable->SetInt(pDevice, "g_specularpower", 200);
}

void CShaderMetal::SetMaterial(D3DMATERIAL9 const&mat)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	D3DXVECTOR4  tempcolor;

	// �����p�̃}�e���A�����Z�b�g
	//tempcolor.x = 0.8f;
	//tempcolor.y = 0.8f;
	//tempcolor.z = 0.8f;
	//tempcolor.w = 1.0f;
	tempcolor.x = mat.Ambient.r;
	tempcolor.y = mat.Ambient.g;
	tempcolor.z = mat.Ambient.b;
	tempcolor.w = mat.Ambient.a;
	m_PSConstantTable->SetVector(pDevice, "g_ambient_mat", &tempcolor);

	// �f�B�t���[�Y���p�̃}�e���A�����Z�b�g
	tempcolor.x = mat.Diffuse.r;
	tempcolor.y = mat.Diffuse.g;
	tempcolor.z = mat.Diffuse.b;
	tempcolor.w = mat.Diffuse.a;
	m_PSConstantTable->SetVector(pDevice, "g_diffuse_mat", &tempcolor);

	// �G�~�b�V�u���p�̃}�e���A�����Z�b�g
	tempcolor.x = mat.Emissive.r;
	tempcolor.y = mat.Emissive.g;
	tempcolor.z = mat.Emissive.b;
	tempcolor.w = mat.Emissive.a;
	m_PSConstantTable->SetVector(pDevice, "g_emissive_mat", &tempcolor);

	// �X�y�L�����[���p�̃}�e���A�����Z�b�g
	tempcolor.x = mat.Specular.r;
	tempcolor.y = mat.Specular.g;
	tempcolor.z = mat.Specular.b;
	tempcolor.w = mat.Specular.a;
	m_PSConstantTable->SetVector(pDevice, "g_specular_mat", &tempcolor);

	// �p���[�l���Z�b�g
	m_PSConstantTable->SetFloat(pDevice, "g_power", mat.Power);
}
