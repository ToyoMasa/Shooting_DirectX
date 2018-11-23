//======================================================================
//	シェーダー[shader.h]
//
//======================================================================
#ifndef _SPOTLIGHT_SHADER_H_
#define _SPOTLIGHT_SHADER_H_

#include "shader.h"

class CShaderSpotlight : public CShader
{
public:
	static CShaderSpotlight* GetShader();
	static void Destroy();
	void ShaderSet(D3DXMATRIX world)override;
	void SetMaterial(D3DMATERIAL9 const&mat)override;

private:
	CShaderSpotlight();
	CShaderSpotlight(const CShaderSpotlight& ref) {};
	CShaderSpotlight& operator = (const CShaderSpotlight& ref) {};
	~CShaderSpotlight() {}

	static CShaderSpotlight* m_Shader;
};

#endif // !_SPOTLIGHT_SHADER_H_
