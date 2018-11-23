#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "spotlightShader.h"

CShaderSpotlight *CShaderSpotlight::m_Shader = NULL;

CShaderSpotlight::CShaderSpotlight() : CShader()
{
	bool sts;
	sts = VertexShaderCompile(
		SHADER_FILE[SH_ID_SPOTLIGHT].c_str(),						// シェーダーファイル名
		"main",							// エントリー関数名
		"vs_3_0");						// バージョン

	if (!sts) {
		MessageBox(NULL, "エラー", "エラー", MB_OK);
	}

	// ピクセルシェーダーコンパイル
	sts = PixelShaderCompile(
		SHADER_FILE[SH_ID_METAL].c_str(),						// シェーダーファイル名
		"PS",							// エントリー関数名
		"ps_3_0");						// バージョン

	if (!sts) {
		MessageBox(NULL, "読み込みエラー", "読み込みエラー", MB_OK);
	}
}

CShaderSpotlight* CShaderSpotlight::GetShader()
{
	if (m_Shader == NULL)
	{
		m_Shader = new CShaderSpotlight();
	}

	return m_Shader;
}

void CShaderSpotlight::Destroy()
{
	if (m_Shader)
	{
		delete m_Shader;
	}
}

void CShaderSpotlight::ShaderSet(D3DXMATRIX world)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// 光の設定情報
	D3DXVECTOR4		lightDir(CManager::GetCamera()->GetFront().x, CManager::GetCamera()->GetFront().y, CManager::GetCamera()->GetFront().z, 0.0f);		// 光の方向
	D3DXVECTOR4		diffuse(DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR);			// 平行光源の色
	D3DXVECTOR4		ambient(DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR);			// 環境光
	D3DXVECTOR4		specular(1.0f, 1.0f, 1.0f, 0.01f);			// スペキュラ光
	D3DXVECTOR4		cameraPos = D3DXVECTOR4(CManager::GetCamera()->GetPos().x, CManager::GetCamera()->GetPos().y, CManager::GetCamera()->GetPos().z, 0.0f);
	float			lightStrength = 30.0f;

	// 頂点シェーダーとピクセルシェーダーをセット
	pDevice->SetVertexShader(m_VertexShader);
	pDevice->SetPixelShader(m_PixelShader);

	// 定数をセット(頂点シェーダー)
	m_VSConstantTable->SetMatrix(pDevice, "g_world", &world);
	m_VSConstantTable->SetMatrix(pDevice, "g_view", &CManager::GetCamera()->GetView());
	m_VSConstantTable->SetMatrix(pDevice, "g_projection", &CManager::GetCamera()->GetProjection());

	m_VSConstantTable->SetVector(pDevice, "g_lightposition", &cameraPos);
	m_PSConstantTable->SetFloat(pDevice, "g_lightstrength", lightStrength);
	m_PSConstantTable->SetVector(pDevice, "g_lightdirection", &lightDir);
	m_PSConstantTable->SetFloat(pDevice, "g_lightcos", D3DXToRadian(45.0f));
	m_PSConstantTable->SetFloat(pDevice, "g_lightdecay", 1.0f);
}

void CShaderSpotlight::SetMaterial(D3DMATERIAL9 const&mat)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	D3DXVECTOR4  tempcolor;

	// 環境光用のマテリアルをセット
	//tempcolor.x = 0.8f;
	//tempcolor.y = 0.8f;
	//tempcolor.z = 0.8f;
	//tempcolor.w = 1.0f;
	//tempcolor.x = mat.Ambient.r;
	//tempcolor.y = mat.Ambient.g;
	//tempcolor.z = mat.Ambient.b;
	//tempcolor.w = mat.Ambient.a;
	//m_PSConstantTable->SetVector(pDevice, "g_ambient_mat", &tempcolor);

	//// ディフューズ光用のマテリアルをセット
	//tempcolor.x = mat.Diffuse.r;
	//tempcolor.y = mat.Diffuse.g;
	//tempcolor.z = mat.Diffuse.b;
	//tempcolor.w = mat.Diffuse.a;
	//m_PSConstantTable->SetVector(pDevice, "g_diffuse_mat", &tempcolor);

	//// エミッシブ光用のマテリアルをセット
	//tempcolor.x = mat.Emissive.r;
	//tempcolor.y = mat.Emissive.g;
	//tempcolor.z = mat.Emissive.b;
	//tempcolor.w = mat.Emissive.a;
	//m_PSConstantTable->SetVector(pDevice, "g_emissive_mat", &tempcolor);

	//// スペキュラー光用のマテリアルをセット
	//tempcolor.x = mat.Specular.r;
	//tempcolor.y = mat.Specular.g;
	//tempcolor.z = mat.Specular.b;
	//tempcolor.w = mat.Specular.a;
	//m_PSConstantTable->SetVector(pDevice, "g_specular_mat", &tempcolor);

	//// パワー値をセット
	//m_PSConstantTable->SetFloat(pDevice, "g_power", mat.Power);
}
