//======================================================================
//	ウェイポイント　（2018/11/17)
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "texture.h"
#include "billboard.h"
#include "waypoint.h"
#include "mathutil.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

std::vector<CWayPoint*> CWayPoint::m_WayPonits; 
std::vector<CBillBoard*> CWayPoint::m_PointsDebug;
int **CWayPoint::m_EdgeCost;
int **CWayPoint::m_ShortestPath;
int	CWayPoint::m_EdgeCostSize = 0;
int CWayPoint::m_ShortestPathSize = 0;

void CWayPoint::Init()
{
	Load("data/output/waypoints.txt");
}

void CWayPoint::Uninit()
{
	if (m_EdgeCost != NULL)
	{
		for (int i = 0; i < m_EdgeCostSize; ++i)
		{
			if (m_EdgeCost[i] != NULL)
			{
				delete[] m_EdgeCost[i];
				m_EdgeCost[i] = NULL;
			}
		}
		delete[] m_EdgeCost;
		m_EdgeCost = NULL;
	}

	if (m_ShortestPath != NULL)
	{
		for (int i = 0; i < m_ShortestPathSize; ++i)
		{
			delete[] m_ShortestPath[i];
			m_ShortestPath[i] = NULL;
		}
		delete[] m_ShortestPath;
		m_ShortestPath = NULL;
	}

	for (int i = 0; i != m_PointsDebug.size(); ++i)
	{
		m_PointsDebug[i]->Release();
	}

	for (int i = 0; i != m_WayPonits.size(); ++i)
	{
		if (m_WayPonits[i] != NULL)
		{
			m_WayPonits[i]->m_NearPoints.clear();
			delete m_WayPonits[i];
		}
	}

	m_WayPonits.clear();
	m_PointsDebug.clear();
}

void CWayPoint::Add(D3DXVECTOR3 pos)
{
	CWayPoint* waypoint = new CWayPoint(pos);
	m_WayPonits.push_back(waypoint);

	CBillBoard* billboard = CBillBoard::Create(TEX_ID_NORMAL_CIRCLE);
	billboard->Set(waypoint->m_Pos, 2.0f, BILLBOARDTYPE_NORMAL);
	//billboard->SetVisible(false);
	m_PointsDebug.push_back(billboard);
}

void CWayPoint::AddNearPoint(int id, int addid)
{
	// 追加するIDと追加先IDが同じなら戻る
	if (id == addid)
	{
		return;
	}
	// すでに追加するIDを持っていたら戻る
	for (int i = 0; i != m_WayPonits[id]->m_NearPoints.size(); i++)
	{
		if (m_WayPonits[id]->m_NearPoints[i] == addid)
		{
			return;
		}
	}

	m_WayPonits[id]->SetRecentPoint(addid);
	m_WayPonits[addid]->SetRecentPoint(id);

	SetSelectDebugColor(id);
}

void CWayPoint::SetRecentPoint(int id)
{
	m_NearPoints.push_back(id);
}

//グラフ生成.
void CWayPoint::CreateGraph()
{
	//エッジを設定.
	{
		m_EdgeCost = new int*[m_WayPonits.size()];
		m_EdgeCostSize = m_WayPonits.size();
		m_ShortestPath = new int*[m_WayPonits.size()];
		m_ShortestPathSize = m_WayPonits.size();
		for (int i = 0; i != m_WayPonits.size(); ++i)
		{
			m_EdgeCost[i] = new int[m_WayPonits.size()];
			m_ShortestPath[i] = new int[m_WayPonits.size()];
		}

		// コストを大きい値で初期化
		for (int i = 0; i != m_WayPonits.size(); ++i) 
		{
			for (int j = 0; j != m_WayPonits.size(); ++j) 
			{
				m_EdgeCost[i][j] = INIT_COST;
			}
		}

		// 隣接地点との距離をコストに
		for (int i = 0; i != m_WayPonits.size(); ++i)
		{
			for (int j = 0; j != m_WayPonits[i]->m_NearPoints.size(); ++j)
			{
				float len = D3DXVec3Length(&(m_WayPonits[i]->m_Pos - m_WayPonits[m_WayPonits[i]->m_NearPoints[j]]->m_Pos));
				m_EdgeCost[m_WayPonits[i]->m_ID][m_WayPonits[i]->m_NearPoints[j]] = (int)len;
			}
		}

		// 自分のコストを0に
		for (int i = 0; i != m_WayPonits.size(); ++i) 
		{
			m_EdgeCost[i][i] = 0; 
		}
	}

	// 全点対最短経路を求める
	{
		// エッジの状態をまんまコピー
		for (int i = 0; i != m_WayPonits.size(); ++i) {
			for (int j = 0; j != m_WayPonits.size(); ++j) {
				m_ShortestPath[i][j] = m_EdgeCost[i][j];
			}
		}

		// ワーシャルフロイド
		for (int k = 0; k != m_WayPonits.size(); k++) {
			for (int i = 0; i != m_WayPonits.size(); i++) {
				for (int j = 0; j != m_WayPonits.size(); j++) {
					const int newValue = m_ShortestPath[i][k] + m_ShortestPath[k][j];
					m_ShortestPath[i][j] = min(m_ShortestPath[i][j], newValue);
				}
			}
		}
	}
}

//次に行くウェイポイントを取得.
int CWayPoint::GetNextPoint(const int s, const int e)
{
	for (int i = 0; i != m_WayPonits.size(); i++) 
	{
		if (i == s) { continue; }
		if (m_EdgeCost[s][i] + m_ShortestPath[i][e] == m_ShortestPath[s][e]) {
			return i;
		}
	}
	return -1;
}

//指定位置から一番近いウェイポイントを探す.
int CWayPoint::SearchShortestPoint(const D3DXVECTOR3& pos)
{
	// ウェイポイントがない時
	if (m_WayPonits.size() == 0) { return -1; }

	// 一番近いポイントを探す
	int     nextPoint = 0;
	float   minDistance = (float)(10000);

	for (int i = 0; i != m_WayPonits.size(); ++i) 
	{
		const float distance = D3DXVec3Length(&(m_WayPonits[i]->m_Pos - pos));
		if (distance < minDistance)
		{
			minDistance = distance;
			nextPoint = i;
		}
	}

	return nextPoint;
}

// ウェイポイントを取得
D3DXVECTOR3 CWayPoint::GetWayPointPos(const int& index)
{
	if (index < 0 || index >= (int)m_WayPonits.size())
	{
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	return m_WayPonits[index]->m_Pos;
}

void CWayPoint::Debug()
{
	for (int i = 0; i != m_PointsDebug.size(); ++i)
	{
		m_PointsDebug[i]->SetVisible(CManager::GetDebug());
	}
}

void CWayPoint::Debug(bool debug)
{
	for (int i = 0; i != m_PointsDebug.size(); ++i)
	{
		m_PointsDebug[i]->SetVisible(debug);
	}
}

// バイナリファイルに書き出し
void CWayPoint::Save(string textname)
{
	ofstream outputFile(textname, ios::out | ios::binary);

	if (m_WayPonits.size() > 0)
	{
		int size = m_WayPonits.size();
		outputFile.write((char*)&size, sizeof(int));
	}

	for (int i = 0; i != m_WayPonits.size(); ++i)
	{
		outputFile.write((char*)&m_WayPonits[i]->m_ID, sizeof(int));
		outputFile.write((char*)&m_WayPonits[i]->m_Pos, sizeof(D3DXVECTOR3));
		int numnear = m_WayPonits[i]->m_NearPoints.size();
		outputFile.write((char*)&numnear, sizeof(int));

		for (int j = 0; j != m_WayPonits[i]->m_NearPoints.size(); j++)
		{
			outputFile.write((char*)&m_WayPonits[i]->m_NearPoints[j], sizeof(int));
		}
	}

	outputFile.close();
}

void CWayPoint::Load(string textname)
{
	ifstream inputFile(textname, ios::in | ios::binary);
	if (!inputFile)
	{
		MessageBox(NULL, "ウェイポイントファイルの読み込みに失敗しました", "エラー", MB_OK);
	}

	int size;
	inputFile.read((char*)&size, sizeof(int));

	for (int i = 0; i < size; i++)
	{
		int id;
		inputFile.read((char*)&id, sizeof(int));
		D3DXVECTOR3 pos;
		inputFile.read((char*)&pos, sizeof(D3DXVECTOR3));
		int numnear;
		inputFile.read((char*)&numnear, sizeof(int));

		CWayPoint* waypoint = new CWayPoint(pos);
		waypoint->m_ID = i;
		for (int j = 0; j < numnear; j++)
		{
			int point;
			inputFile.read((char*)&point, sizeof(int));
			waypoint->m_NearPoints.push_back(point);
		}

		m_WayPonits.push_back(waypoint);

		m_PointsDebug.push_back(CBillBoard::Create(TEX_ID_NORMAL_CIRCLE));
		m_PointsDebug[i]->Set(m_WayPonits[i]->m_Pos, 2.0f, BILLBOARDTYPE_NORMAL);
	}

	inputFile.close();

	CreateGraph();
}

int CWayPoint::GetIDonScreen(D3DXVECTOR3 mouse)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return -1;
	}

	for (int i = 0; i != m_WayPonits.size(); i++)
	{
		D3DXVECTOR3 screen;
		// スクリーン座標に変換
		WorldToScreen(pDevice, CManager::GetCamera()->GetView(), CManager::GetCamera()->GetProjection(), m_WayPonits[i]->m_Pos, screen);

		if (isCollisionCircle2D(mouse.x, mouse.y, 10.0f, screen.x, screen.y, 10.0f))
		{
			return m_WayPonits[i]->m_ID;
		}
	}

	return -1;
}

void CWayPoint::DebugColorReset()
{
	for (int i = 0; i != m_PointsDebug.size(); i++)
	{
		m_PointsDebug[i]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}

void CWayPoint::SetSelectDebugColor(int id)
{
	DebugColorReset();
	m_PointsDebug[id]->SetColor(D3DCOLOR_RGBA(255, 16, 16, 255));

	for (int i = 0; i != m_WayPonits[id]->m_NearPoints.size(); i++)
	{
		int nearId = m_WayPonits[id]->m_NearPoints[i];
		m_PointsDebug[nearId]->SetColor(D3DCOLOR_RGBA(16, 255, 16, 255));
	}

}

void CWayPoint::ChangeDebugColor(int id, D3DCOLOR color)
{
	m_PointsDebug[id]->SetColor(color);
}
