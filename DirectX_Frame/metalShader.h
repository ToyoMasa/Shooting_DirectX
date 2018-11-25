//======================================================================
//	金属シェーダー[metalShader.h]
//
//======================================================================
#ifndef _METAL_SHADER_H_
#define _METAL_SHADER_H_

#include "shader.h"

class CShaderMetal : public CShader
{
public:
	static CShaderMetal* GetShader();
	static void Destroy();
	void ShaderSet(const D3DXMATRIX& world)override;
	void SetMaterial(const D3DMATERIAL9& mat)override;

private:
	CShaderMetal();
	CShaderMetal(const CShaderMetal& ref) {};
	CShaderMetal& operator = (const CShaderMetal& ref) {};
	~CShaderMetal() {}

	static CShaderMetal* m_Shader;
};

#endif // !_METAL_SHADER_H_
