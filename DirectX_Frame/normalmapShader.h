//======================================================================
//	ノーマルマップ対応シェーダー[normalmapShader.h]
//
//======================================================================
#ifndef _NORMALMAP_SHADER_H_
#define _NORMALMAP_SHADER_H_

#include "shader.h"

class CShaderNormalmap : public CShader
{
public:
	static CShaderNormalmap* GetShader();
	static void Destroy();
	void ShaderSet(const D3DXMATRIX& world)override;
	void SetMaterial(const D3DMATERIAL9& mat)override;

private:
	CShaderNormalmap();
	CShaderNormalmap(const CShaderNormalmap& ref) {};
	CShaderNormalmap& operator = (const CShaderNormalmap& ref) {};
	~CShaderNormalmap() {}

	static CShaderNormalmap* m_Shader;
};

#endif // !_NORMALMAP_SHADER_H_