//======================================================================
//	3D���f���w�b�_[sceneModelFBX]
//
//======================================================================
#ifndef _SCENEMODELFBX_H_
#define _SCENEMODELFBX_H_

#include <string>
#include <list>
#include <fbxsdk.h>

#pragma comment (lib, "libfbxsdk-mt.lib")

//-------------------------------------------------
// �\���̐錾
//-------------------------------------------------
typedef struct VERTEX
{
	float posX, posY, posZ;
	float normalX, normalY, normalZ;
} Vertex;

// UV
typedef struct Point2DF 
{
	float x, y;

	bool operator==(Point2DF& val) 
	{
		if (this->x == val.x && this->y == val.y) 
		{
			return true;
		}
		return false;
	}
} point2;

typedef struct __UV_SET__ 
{
	std::string uvSetName;
	std::list<std::string> textures;
	std::list<point2> uvBuffer;
} UvSet;

typedef struct COLOR_RGBAF {
	float r;
	float g;
	float b;
	float a;
} ColorRGBA;

// ���f���Ǘ��pID
typedef enum
{
	FBX_ID_XBOT = 0,		// 
} FBX_MODEL_ID;

// ���f���ǂݍ��ݗp�̃\�[�X
static const std::string FBX_MODEL_SOURCE[] =
{
	"data/models/Running.fbx",
};

class CSceneModelFBX : public CScene
{
public:
	//======================================================================
	//	�R���X�g���N�^
	//======================================================================
	CSceneModelFBX::CSceneModelFBX(int priority) : CScene(priority)
	{
		D3DXMatrixIdentity(&m_World);
		D3DXMatrixIdentity(&m_Move);
		D3DXMatrixIdentity(&m_Rotate);
		D3DXMatrixIdentity(&m_RotX);
		D3DXMatrixIdentity(&m_RotY);
		D3DXMatrixIdentity(&m_RotZ);
		D3DXMatrixIdentity(&m_Scale);

	}

	//======================================================================
	//	�f�X�g���N�^
	//======================================================================
	CSceneModelFBX::~CSceneModelFBX()
	{
	}

	static void InitFbxManager();
	static void UninitFbxManager();
	void Init(const std::string& modelName);
	void Uninit();
	void Update();
	void Draw();
	void SetWorld(D3DXMATRIX move);
	void Move(D3DXVECTOR3 pos);
	void Rotate(D3DXVECTOR3 rot);
	void Rotate(D3DXMATRIX rot);
	void Scale(D3DXVECTOR3 scale);
	static CSceneModelFBX* Create(const std::string& modelName);
	/*static CSceneModelFBX* Create(const std::string& modelName, bool isIgnore);*/
	void GetMesh(FbxNode* node);			// ���b�V�����擾
	void GetPosition(FbxMesh* mesh);		// ���_���W�擾
	void GetNormal(FbxMesh* mesh);			// �@�����擾
	void GetVertexUV(FbxMesh* mesh);		// UV���擾
	void GetTextureNames(FbxMesh* mesh);	// �e�N�X�`�����擾
	void GetVertexColor(FbxMesh* mesh);		// ���_�J���[�擾

private:
	D3DXMATRIX		m_World;		// ���[���h�ϊ��s��
	D3DXMATRIX		m_Move;			// ���s�ړ��s��
	D3DXMATRIX		m_Rotate;		// ��]�s��
	D3DXMATRIX		m_RotX;			// X����]�s��
	D3DXMATRIX		m_RotY;			// Y����]�s��
	D3DXMATRIX		m_RotZ;			// Z����]�s��
	D3DXMATRIX		m_Scale;		// �g��k���s��
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer; // ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9  m_IndexBuffer;  // �C���f�b�N�X�o�b�t�@

	FbxManager*		m_FbxManager;	// FbxManager�̐���
	FbxImporter*	m_FbxImporter;	// Importer�̐���
	FbxIOSettings*	m_IOSettings;	// ���o�͐ݒ�
	FbxScene*		m_Scene;		// �V�[���I�u�W�F�N�g
	FbxNode*		m_RootNode;		// ���[�g�m�[�h
	FbxNode*		m_ChildNode;	// �q�m�[�h
	int				m_NodeCount;	// �m�[�h��
	int				m_PolygonCount;	// �|���S����
	int*			m_PolygonSize;	// �|���S���T�C�Y
	VERTEX*			m_Vertex;		// ���_�o�b�t�@	
	int				m_VertexCount;	// ���_��
	int				m_IndexCount;	// �C���f�b�N�X�o�b�t�@��
	DWORD*			m_Index;		// �C���f�b�N�X�o�b�t�@
	UvSet			m_UVset;		// UV
	ColorRGBA*		m_VertexColor;	// ���_�J���[
};

#endif //!_SCENEMODELFBX_H_