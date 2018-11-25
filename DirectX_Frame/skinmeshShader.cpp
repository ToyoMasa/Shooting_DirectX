#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "skinmeshShader.h"

CShaderSkinmesh *CShaderSkinmesh::m_Shader = NULL;

CShaderSkinmesh::CShaderSkinmesh() : CShader()
{
	bool sts;
	sts = VertexShaderCompile(
		SHADER_FILE[SH_ID_SKINMESH].c_str(),						// シェーダーファイル名
		"main",							// エントリー関数名
		"vs_3_0");						// バージョン

	if (!sts) {
		MessageBox(NULL, "エラー", "エラー", MB_OK);
	}

	// ピクセルシェーダーコンパイル
	sts = PixelShaderCompile(
		SHADER_FILE[SH_ID_SKINMESH].c_str(),						// シェーダーファイル名
		"PS",							// エントリー関数名
		"ps_3_0");						// バージョン

	if (!sts) {
		MessageBox(NULL, "読み込みエラー", "読み込みエラー", MB_OK);
	}
}

CShaderSkinmesh* CShaderSkinmesh::GetShader()
{
	if (m_Shader == NULL)
	{
		m_Shader = new CShaderSkinmesh();
	}

	return m_Shader;
}

void CShaderSkinmesh::Destroy()
{
	if (m_Shader != NULL)
	{
		delete m_Shader;
		m_Shader = NULL;
	}
}

void CShaderSkinmesh::ShaderSet(const D3DXMATRIX& world)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// 光の設定情報
	D3DXVECTOR4		light_dir(-1.0f, -1.0f, 1.0f, 0.0f);		// 光の方向
//	D3DXVECTOR4		diffuse(DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR);			// 平行光源の色
	D3DXVECTOR4		diffuse(1.0f, 1.0f, 1.0f, 1.0f);			// 平行光源の色
	D3DXVECTOR4		ambient(DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR);			// 環境光
	D3DXVECTOR4		specular(1.0f, 1.0f, 1.0f, 0.01f);			// スペキュラ光
	D3DXVECTOR4		cameraPos = D3DXVECTOR4(CManager::GetCamera()->GetPos().x, CManager::GetCamera()->GetPos().y, CManager::GetCamera()->GetPos().z, 0.0f);

	// 頂点シェーダーとピクセルシェーダーをセット
	pDevice->SetVertexShader(m_VertexShader);
	pDevice->SetPixelShader(m_PixelShader);

	// 定数をセット(頂点シェーダー)
	m_VSConstantTable->SetMatrix(pDevice, "g_world", &world);
	m_VSConstantTable->SetMatrix(pDevice, "g_view", &CManager::GetCamera()->GetView());
	m_VSConstantTable->SetMatrix(pDevice, "g_projection", &CManager::GetCamera()->GetProjection());

	m_VSConstantTable->SetVector(pDevice, "g_diffuse", &diffuse);
	m_VSConstantTable->SetVector(pDevice, "g_ambient", &ambient);
	m_VSConstantTable->SetVector(pDevice, "g_specular", &specular);
	m_VSConstantTable->SetVector(pDevice, "g_light_dir", &light_dir);

}

void CShaderSkinmesh::SetMaterial(D3DMATERIAL9 const&mat)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	D3DXVECTOR4  tempcolor;

	// 環境光用のマテリアルをセット
	tempcolor = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 0.2f);
	m_VSConstantTable->SetVector(pDevice, "g_ambient_mat", &tempcolor);

	// ディフューズ光用のマテリアルをセット
	tempcolor = D3DXVECTOR4(0.7f, 0.7f, 0.7f, 0.7f);
	m_VSConstantTable->SetVector(pDevice, "g_diffuse_mat", &tempcolor);

	// エミッシブ光用のマテリアルをセット
	tempcolor.x = mat.Emissive.r;
	tempcolor.y = mat.Emissive.g;
	tempcolor.z = mat.Emissive.b;
	tempcolor.w = mat.Emissive.a;
	m_VSConstantTable->SetVector(pDevice, "g_emissive_mat", &tempcolor);

	// スペキュラー光用のマテリアルをセット
	tempcolor.x = mat.Specular.r;
	tempcolor.y = mat.Specular.g;
	tempcolor.z = mat.Specular.b;
	tempcolor.w = mat.Specular.a;
	m_VSConstantTable->SetVector(pDevice, "g_specular_mat", &tempcolor);

	// パワー値をセット
	m_VSConstantTable->SetFloat(pDevice, "g_power", mat.Power);
}
