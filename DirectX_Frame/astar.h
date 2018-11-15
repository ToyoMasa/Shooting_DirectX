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
	STATUS status; // �X�e�[�^�X
	int cost;        // �ړ��R�X�g
	int heuristic;   // �q���[���X�e�B�b�N�R�X�g
	AStar* parent;   // �eA*�̃|�C���^
	D3DXVECTOR2 pos;       // ���W
					 // �X�R�A�擾
	int GetScore()
	{
		return cost + heuristic;
	}
};

#endif // !_A_STAR_H_
