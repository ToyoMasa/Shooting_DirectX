//======================================================================
//	アイテムクラス[item.h]
//
//======================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

static const int ITEM_MAX = 50;

class CItem
{
public:
	CItem();
	~CItem() {}

	virtual void Init() {}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	static void UpdateAll();
	void Release();
	static void ReleaseAll();

private:
	static CItem*	m_Items[ITEM_MAX];
protected:
	D3DXVECTOR3		m_Pos;
	D3DXVECTOR3		m_Rot;
};

#endif // !_ITEM_H_
