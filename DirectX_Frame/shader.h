#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>

class CCamera;
class CScene;

static const DWORD FVF_SHADER = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

typedef enum
{
	SHADER_FILE_001,
} SHADERFILE_LIST;

// シェーダーファイルリスト
static const std::string SHADER_FILE[] =
{
	"shader.fx",
};

class CShader
{
public:
	CShader()
	{
		m_hTech = NULL;
		D3DXMatrixIdentity(&m_World);
	}
	~CShader(){}

	void Init();
	void Uninit();
	void Draw(CScene* scene);

private:
	D3DXMATRIX m_World;	// 描画に使う変換行列

	CCamera* m_pCamera;

	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE m_hTech;			// 現在のテクニックのハンドル
	D3DXHANDLE m_hTechNext;		// 次のテクニックのハンドル
};

#endif // !_SHADER_H_
