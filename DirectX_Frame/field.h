//======================================================================
//	フィールド[field.h]
//
//======================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include <string>

class CScene3D;
class CCharacter;

static const int SEPARATE_NUM = 200;

class CField : public CScene
{
public:
	CField::CField(int priority) : CScene(priority)
	{
		m_MeshSize = 1.0f;
		m_SizeX = 1;
		m_SizeY = 1;
		m_TextureID = 0;
		m_NumPrimitive = 0;
		m_NumVertex = 0;
		m_NumIndex = 0;
		m_Scene3D = NULL;
		m_AreaID = NULL;
	}
	~CField(){}

	void Init(int texId, float meshSize, int sizeX, int sizeY);
	void Init(int texId, float meshSize, int sizeX, int sizeY, float** heights);
	void Uninit();
	void Update();
	void Draw();
	void DrawWithShader()override {}
	float GetHeight(D3DXVECTOR3& pos, CCharacter* chara);
	int GetBlockSize() { return m_SizeX; }
	int GetVertexNum() { return m_NumVertex; }
	int GetSizeX() { return m_SizeX; }
	int GetSizeY() { return m_SizeY; }
	void Change();
	VERTEX_3D* GetVertex() { return m_Scene3D->GetVertex(); }
	void SetVertex(VERTEX_3D* v) { m_Vertex = v; }
	void Save(std::string textname);
	void Load(std::string textname);
	static CField* Create(std::string textname);
	static CField* Create(int texId, float meshSize, int sizeX, int sizeY);
	static CField* Create(int texId, float meshSize, int sizeX, int sizeY, float** heights);

private:
	float m_MeshSize;
	int m_SizeX;
	int m_SizeY;
	int m_TextureID;
	int m_NumPrimitive;
	int m_NumVertex;
	int m_NumIndex;
	int* m_AreaID;
	float m_FieldHeight[5][5];
	VERTEX_3D* m_Vertex;
	WORD* m_Index;
	CScene3D* m_Scene3D;
};

#endif // !_FIELD_H_
