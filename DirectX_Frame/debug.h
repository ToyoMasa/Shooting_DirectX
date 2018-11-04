//======================================================================
//	デバッグヘッダ[debug.h]
//
//======================================================================
#ifndef _DEBUG_H_
#define _DEBUG_H_

class CDebug : public CScene
{
public:
	CDebug(int priority) :CScene(priority) {}
	~CDebug() {}

protected:
	D3DXMATRIX m_World;
};

class CDebugSphere : public CDebug
{
public:
	CDebugSphere(int priority) :CDebug(priority) {}
	~CDebugSphere() {}

	void Init(D3DXVECTOR3 pos, float r);
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void DrawWithShader()override {}

	static CDebugSphere* Create(D3DXVECTOR3 pos, float r);

private:
	float m_Radius;
};

#endif // !_DEBUG_H_
