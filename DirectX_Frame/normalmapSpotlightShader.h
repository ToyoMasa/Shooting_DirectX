//======================================================================
//	ノーマルマップ対応シェーダー[normalmapSpotlightShader.h]
//
//======================================================================
#ifndef _NORMALMAP_SPOTLIGHT_SHADER_H_
#define _NORMALMAP_SPOTLIGHT_SHADER_H_

#include "shader.h"

class CShaderNormalmapSpotlight : public CShader
{
public:
	static CShaderNormalmapSpotlight* GetShader();
	static void Destroy();
	void ShaderSet(const D3DXMATRIX& world)override;
	void SetMaterial(const D3DMATERIAL9& mat)override;

private:
	CShaderNormalmapSpotlight();
	CShaderNormalmapSpotlight(const CShaderNormalmapSpotlight& ref) {};
	CShaderNormalmapSpotlight& operator = (const CShaderNormalmapSpotlight& ref) {};
	~CShaderNormalmapSpotlight() {}

	static CShaderNormalmapSpotlight* m_Shader;
};

#endif // !_NORMALMAP_SHADER_H_
