//======================================================================
//	アイテムクラス[item.h]
//
//======================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

static const int ITEM_MAX = 50;

enum ITEM_TYPE
{
	ITEM_TYPE_NONE,
	ITEM_TYPE_CAPSULE,
	ITEM_TYPE_MAX
};

class CItem
{
public:
	CItem();
	~CItem() {}

	virtual void Init() { m_Type = ITEM_TYPE_NONE; }
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	static void UpdateAll();
	void Release();
	D3DXVECTOR3 GetPos() { return m_Pos; }
	ITEM_TYPE GetType() { return m_Type; }
	static CItem* GetItem(int id) { return m_Items[id]; }
	static void ReleaseAll();

private:
	static CItem*	m_Items[ITEM_MAX];
protected:
	D3DXVECTOR3		m_Pos;
	D3DXVECTOR3		m_Rot;
	ITEM_TYPE		m_Type;
};

#endif // !_ITEM_H_
