//======================================================================
//	�r���{�[�h�����w�b�_[billboard.h]
//
//======================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include <string>

class CCamera;

static const int BILLBOARD_MAX = 30000;

enum DRAW_TYPE
{
	BILLBOARDTYPE_NORMAL = 0,
	BILLBOARDTYPE_ADD,
	BILLBOARDTYPE_FIXED_Y,
};

class CBillBoard
{
public:
	CBillBoard() 
	{
		for (int i = 0; i < BILLBOARD_MAX; i++)
		{
			if (m_BillBoards[i] == NULL)
			{
				this->SetId(i);
				m_BillBoards[i] = this;
				m_BillBoards[i]->m_DrawType = 0;
				m_BillBoards[i]->m_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
				m_BillBoards[i]->m_isVisible = true;
				break;
			}
		}
	}
	~CBillBoard() {}
	static void Init();
	static void Uninit();
	static void UpdateAll();
	void Draw();
	static void DrawBegin();
	static void DrawEnd();
	static void DrawAll();
	static void Set(int id, int texId, D3DXVECTOR3 pos, float scale, int drawtype);
	void Set(int texId, D3DXVECTOR3 pos, float scale, int drawtype);
	void Set(D3DXVECTOR3 pos, float scale, int drawtype);
	void Set(int texId, D3DXVECTOR3 pos, float scale, int drawtype, D3DCOLOR color);
	void SetId(int Id) { m_Id = Id; }
	void SetVisible(bool visible) { m_isVisible = visible; }
	void SetColor(D3DCOLOR color);
	void Release();
	static void ReleaseAll();
	static CBillBoard* Create(int texId);

private:
	int								m_Id;					// �Ǘ��ԍ�
	int								m_DrawType;				// �r���{�[�h�̕`��^�C�v
	int								m_TextureId;			// �e�N�X�`���ԍ�
	D3DXVECTOR3						m_Pos;					// �ꏊ
	D3DCOLOR						m_Color;				// �F
	float							m_ScaleX;				// �T�C�Y
	float							m_ScaleY;				// �T�C�Y
	float							m_ScaleZ;				// �T�C�Y
	bool							m_isVisible;			// �`�悪�L�����ǂ���
	static D3DMATERIAL9				m_Mat;					// ���f��1�����ɂ�1��
	static LPDIRECT3DVERTEXBUFFER9	m_VertexBuffer;			// ���_�o�b�t�@
	static LPDIRECT3DINDEXBUFFER9	m_IndexBuffer;			// �C���f�b�N�X�o�b�t�@
	static D3DXMATRIX				m_World;				// ���[���h�ϊ��s��
	static D3DXMATRIX				m_Move;					// ���W�ϊ��s��
	static D3DXMATRIX				m_Scale;				// �g��k���s��
	static CBillBoard*				m_BillBoards[BILLBOARD_MAX];		// �r���{�[�h
};


#endif // !_BILLBOARD_H_
