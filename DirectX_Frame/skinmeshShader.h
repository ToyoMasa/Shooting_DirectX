//======================================================================
//	スキンメッシュ対応シェーダー[skinmeshShader.h]
//
//======================================================================
#ifndef _SKINMESH_SHADER_H_
#define _SKINMESH_SHADER_H_

#include "shader.h"

class CShaderSkinmesh : public CShader
{
public:
	static CShaderSkinmesh* GetShader();
	static void Destroy();
	void ShaderSet(const D3DXMATRIX& world)override;
	void SetMaterial(const D3DMATERIAL9& mat)override;

private:
	CShaderSkinmesh();
	CShaderSkinmesh(const CShaderSkinmesh& ref) {};
	CShaderSkinmesh& operator = (const CShaderSkinmesh& ref) {};
	~CShaderSkinmesh() {}

	static CShaderSkinmesh* m_Shader;
};

#endif // !_SKINMESH_SHADER_H_
