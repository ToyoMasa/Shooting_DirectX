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

// �V�F�[�_�[�t�@�C�����X�g
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
	D3DXMATRIX m_World;	// �`��Ɏg���ϊ��s��

	CCamera* m_pCamera;

	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE m_hTech;			// ���݂̃e�N�j�b�N�̃n���h��
	D3DXHANDLE m_hTechNext;		// ���̃e�N�j�b�N�̃n���h��
};

#endif // !_SHADER_H_
