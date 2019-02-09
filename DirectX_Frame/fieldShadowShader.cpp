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
		SHADER_FILE[SH_ID_FIELD_SHADOW].c_str(),						// シェーダーファイル名
		"main",							// エントリー関数名
		"vs_3_0");						// バージョン

	if (!sts) {
		MessageBox(NULL, "エラー", "エラー", MB_OK);
	}

	// ピクセルシェーダーコンパイル
	sts = PixelShaderCompile(
		SHADER_FILE[SH_ID_FIELD_SHADOW].c_str(),						// シェーダーファイル名
		"PS",							// エントリー関数名
		"ps_3_0");						// バージョン

	if (!sts) {
		MessageBox(NULL, "読み込みエラー", "読み込みエラー", MB_OK);
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

	// 光の設定情報
	D3DXVECTOR4		cameraPos = D3DXVECTOR4(CManager::GetCamera()->GetPos(), 0.0f);

	// 頂点シェーダーとピクセルシェーダーをセット
	pDevice->SetVertexShader(m_VertexShader);
	pDevice->SetPixelShader(m_PixelShader);

	// 定数をセット(頂点シェーダー)
	m_VSConstantTable->SetMatrix(pDevice, "g_world", &world);
	m_VSConstantTable->SetMatrix(pDevice, "g_view", &CManager::GetCamera()->GetView());
	m_VSConstantTable->SetMatrix(pDevice, "g_projection", &CManager::GetCamera()->GetProjection());
	m_VSConstantTable->SetMatrix(pDevice, "g_view", &CManager::GetLightCamera());
	m_VSConstantTable->SetMatrix(pDevice, "g_projection", &CManager::GetLightProjection());
}

void CShaderFieldShadow::SetMaterial(D3DMATERIAL9 const&mat)
{

}
