//======================================================================
//	シェーダー[shader.h]
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
	void ShaderSet(D3DXMATRIX world)override;
	void SetMaterial(D3DMATERIAL9 const&mat)override;

private:
	CShaderMetal();
	CShaderMetal(const CShaderMetal& ref) {};
	CShaderMetal& operator = (const CShaderMetal& ref) {};
	~CShaderMetal() {}

	static CShaderMetal* m_Shader;
};

#endif // !_METAL_SHADER_H_
