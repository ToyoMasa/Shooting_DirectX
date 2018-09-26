//======================================================================
//	ルート[root.h]
//
//======================================================================
#ifndef _ROOT_H_
#define _ROOT_H_

#include <map>
#include<d3d9.h>
#include<d3dx9.h>
//
//static std::map<int, D3DXVECTOR3> ROOT_POINT;	// AIの移動ルート
//static std::map<int, int> ROOT_MAP;				// 経路マップ

class CRoot
{
public:
	CRoot() {}
	~CRoot() {}

	static void Set();
	static D3DXVECTOR3 GetRootPos(int id) { return m_RootPos[id]; }
	static int GetNextRoot(int id) { return m_NextRoot[id]; }

private:
	static D3DXVECTOR3 m_RootPos[100];
	static int m_NextRoot[100];
};

#endif // !_ROOT_H_

