#ifndef _A_STAR_H_
#define _A_STAR_H_

#include <d3dx9.h>

struct AStar
{
	enum STATUS
	{
		NONE,
		OPEN,
		CLOSED,
	};
	STATUS status; // ステータス
	int cost;        // 移動コスト
	int heuristic;   // ヒューリスティックコスト
	AStar* parent;   // 親A*のポインタ
	D3DXVECTOR2 pos;       // 座標
					 // スコア取得
	int GetScore()
	{
		return cost + heuristic;
	}
};

#endif // !_A_STAR_H_
