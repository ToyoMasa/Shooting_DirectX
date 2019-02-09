#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "fieldShadowShader.h"

CShaderFieldShadow *CShaderFieldShadow::m_Shader = NULL;

CShaderFieldShadow::CShaderFieldShadow() : CShader()
{
	bool sts;
	sts = VertexShaderCompile(
		SHADER_FILE[SH_ID_FIELD_SHADOW].c_str(),						// �V�F�[�_�[�t�@�C����
		"main",							// �G���g���[�֐���
		"vs_3_0");						// �o�[�W����

	if (!sts) {
		MessageBox(NULL, "�G���[", "�G���[", MB_OK);
	}

	// �s�N�Z���V�F�[�_�[�R���p�C��
	sts = PixelShaderCompile(
		SHADER_FILE[SH_ID_FIELD_SHADOW].c_str(),						// �V�F�[�_�[�t�@�C����
		"PS",							// �G���g���[�֐���
		"ps_3_0");						// �o�[�W����

	if (!sts) {
		MessageBox(NULL, "�ǂݍ��݃G���[", "�ǂݍ��݃G���[", MB_OK);
	}
}

CShaderFieldShadow* CShaderFieldShadow::GetShader()
{
	if (m_Shader == NULL)
	{
		m_Shader = new CShaderFieldShadow();
	}

	return m_Shader;
}

void CShaderFieldShadow::Destroy()
{
	if (m_Shader)
	{
		delete m_Shader;
	}
}

void CShaderFieldShadow::ShaderSet(const D3DXMATRIX& world)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ���̐ݒ���
	D3DXVECTOR4		cameraPos = D3DXVECTOR4(CManager::GetCamera()->GetPos(), 0.0f);

	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
	pDevice->SetVertexShader(m_VertexShader);
	pDevice->SetPixelShader(m_PixelShader);

	// �萔���Z�b�g(���_�V�F�[�_�[)
	m_VSConstantTable->SetMatrix(pDevice, "g_world", &world);
	m_VSConstantTable->SetMatrix(pDevice, "g_view", &CManager::GetCamera()->GetView());
	m_VSConstantTable->SetMatrix(pDevice, "g_projection", &CManager::GetCamera()->GetProjection());
	m_VSConstantTable->SetMatrix(pDevice, "g_view", &CManager::GetLightCamera());
	m_VSConstantTable->SetMatrix(pDevice, "g_projection", &CManager::GetLightProjection());
}

void CShaderFieldShadow::SetMaterial(D3DMATERIAL9 const&mat)
{

}
