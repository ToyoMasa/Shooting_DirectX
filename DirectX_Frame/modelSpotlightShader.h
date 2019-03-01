//======================================================================
//	ノーマルマップ対応シェーダー[normalmapSpotlightShader.h]
//
//======================================================================
#ifndef _MODEL_SPOTLIGHT_SHADER_H_
#define _MODEL_SPOTLIGHT_SHADER_H_

#include "shader.h"

class CShaderModelSpotlight : public CShader
{
public:
	static CShaderModelSpotlight* GetShader();
	static void Destroy();
	void ShaderSet(const D3DXMATRIX& world)override;
	void SetMaterial(const D3DMATERIAL9& mat)override;

private:
	CShaderModelSpotlight();
	CShaderModelSpotlight(const CShaderModelSpotlight& ref) {};
	CShaderModelSpotlight& operator = (const CShaderModelSpotlight& ref) {};
	~CShaderModelSpotlight() {}

	static CShaderModelSpotlight* m_Shader;
};

#endif // !_MODEL_SPOTLIGHT_SHADER_H_
