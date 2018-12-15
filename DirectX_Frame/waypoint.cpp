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
	// ウェイポイントを手動で設定
	//Add(D3DXVECTOR3(-55.0f, 0.0f, -73.0f));
	//Add(D3DXVECTOR3(-38.5, 0.0f, -74.0f));
	//Add(D3DXVECTOR3(-38.5f, 0.0f, -88.0f));
	//Add(D3DXVECTOR3(-13.3f, 0.0f, -84.0f));
	//Add(D3DXVECTOR3(5.0f, 0.0f, -84.0f));

	//Add(D3DXVECTOR3(6.8f, 0.0f, -77.6f));
	//Add(D3DXVECTOR3(-40.0f, 0.0f, -45.7f));
	//Add(D3DXVECTOR3(-31.5f, 0.0f, -28.5f));
	//Add(D3DXVECTOR3(-73.0f, 0.0f, -28.0f));
	//Add(D3DXVECTOR3(-78.3f, 0.0f, -6.7f));

	//Add(D3DXVECTOR3(-55.0f, 0.0f, 1.0f));
	//Add(D3DXVECTOR3(-82.3f, 0.0f, 56.2f));
	//Add(D3DXVECTOR3(-68.5f, 0.0f, 63.7f));
	//Add(D3DXVECTOR3(-63.0f, 0.0f, 78.7f));
	//Add(D3DXVECTOR3(-47.0f, 0.0f, 79.2f));

	//Add(D3DXVECTOR3(-30.7f, 0.0f, 60.0f));
	//Add(D3DXVECTOR3(-1.0f, 0.0f, 84.6f));
	//Add(D3DXVECTOR3(11.0f, 0.0f, 59.8f));
	//Add(D3DXVECTOR3(0.0f, 0.0f, 49.3f));
	//Add(D3DXVECTOR3(-7.4f, 0.0f, 9.6f));

	//Add(D3DXVECTOR3(-9.7f, 0.0f, -8.2f));
	//Add(D3DXVECTOR3(-22.0f, 0.0f, 3.7f));
	//Add(D3DXVECTOR3(26.2f, 0.0f, 37.6f));
	//Add(D3DXVECTOR3(47.5f, 0.0f, 42.3f));
	//Add(D3DXVECTOR3(47.0f, 0.0f, 72.4f));

	//Add(D3DXVECTOR3(34.7f, 0.0f, 67.9f));
	//Add(D3DXVECTOR3(56.7f, 0.0f, 73.1f));
	//Add(D3DXVECTOR3(61.5f, 0.0f, 58.8f));
	//Add(D3DXVECTOR3(76.3f, 0.0f, 74.6f));
	//Add(D3DXVECTOR3(82.5f, 0.0f, 53.3f));

	//Add(D3DXVECTOR3(82.4f, 0.0f, 33.9f));
	//Add(D3DXVECTOR3(86.2f, 0.0f, 20.3f));
	//Add(D3DXVECTOR3(71.7f, 0.0f, -40.4f));
	//Add(D3DXVECTOR3(61.2f, 0.0f, -40.5f));
	//Add(D3DXVECTOR3(48.1f, 0.0f, -44.5f));

	//Add(D3DXVECTOR3(67.5f, 0.0f, -78.7f));
	//Add(D3DXVECTOR3(53.6f, 0.0f, -13.7f));
	//Add(D3DXVECTOR3(35.8f, 0.0f, -29.5f));
	//Add(D3DXVECTOR3(20.8f, 0.0f, -28.4f));
	//Add(D3DXVECTOR3(2.3f, 0.0f, -35.9f));

	//Add(D3DXVECTOR3(3.5f, 0.0f, -50.1f));
	//Add(D3DXVECTOR3(26.0f, 0.0f, -49.1f));
	//Add(D3DXVECTOR3(25.7f, 0.0f, -68.7f));

	//// 管理用IDを割り振り
	//for (int i = 0; i != m_WayPonits.size(); ++i)
	//{
	//	m_WayPonits[i]->m_ID = i;
	//}

	// 各隣接点の設定
	//m_WayPonits[0]->SetRecentPoint(1);

	//m_WayPonits[1]->SetRecentPoint(0);
	//m_WayPonits[1]->SetRecentPoint(2);

	//m_WayPonits[2]->SetRecentPoint(1);
	//m_WayPonits[2]->SetRecentPoint(3);

	//m_WayPonits[3]->SetRecentPoint(2);
	//m_WayPonits[3]->SetRecentPoint(4);

	//m_WayPonits[4]->SetRecentPoint(3);
	//m_WayPonits[4]->SetRecentPoint(5);
	//m_WayPonits[4]->SetRecentPoint(42);

	//m_WayPonits[5]->SetRecentPoint(4);
	//m_WayPonits[5]->SetRecentPoint(6);
	//m_WayPonits[5]->SetRecentPoint(42);

	//m_WayPonits[6]->SetRecentPoint(5);
	//m_WayPonits[6]->SetRecentPoint(7);
	//m_WayPonits[6]->SetRecentPoint(8);

	//m_WayPonits[7]->SetRecentPoint(6);
	//m_WayPonits[7]->SetRecentPoint(10);

	//m_WayPonits[8]->SetRecentPoint(6);
	//m_WayPonits[8]->SetRecentPoint(9);

	//m_WayPonits[9]->SetRecentPoint(8);
	//m_WayPonits[9]->SetRecentPoint(10);
	//m_WayPonits[9]->SetRecentPoint(11);

	//m_WayPonits[10]->SetRecentPoint(7);
	//m_WayPonits[10]->SetRecentPoint(9);

	//m_WayPonits[11]->SetRecentPoint(9);
	//m_WayPonits[11]->SetRecentPoint(12);
	//m_WayPonits[11]->SetRecentPoint(13);

	//m_WayPonits[12]->SetRecentPoint(11);
	//m_WayPonits[12]->SetRecentPoint(13);
	//m_WayPonits[12]->SetRecentPoint(21);

	//m_WayPonits[13]->SetRecentPoint(11);
	//m_WayPonits[13]->SetRecentPoint(12);
	//m_WayPonits[13]->SetRecentPoint(14);

	//m_WayPonits[14]->SetRecentPoint(13);
	//m_WayPonits[14]->SetRecentPoint(15);

	//m_WayPonits[15]->SetRecentPoint(14);
	//m_WayPonits[15]->SetRecentPoint(16);

	//m_WayPonits[16]->SetRecentPoint(15);
	//m_WayPonits[16]->SetRecentPoint(17);

	//m_WayPonits[17]->SetRecentPoint(16);
	//m_WayPonits[17]->SetRecentPoint(18);
	//m_WayPonits[17]->SetRecentPoint(22);

	//m_WayPonits[18]->SetRecentPoint(17);
	//m_WayPonits[18]->SetRecentPoint(19);

	//m_WayPonits[19]->SetRecentPoint(18);
	//m_WayPonits[19]->SetRecentPoint(20);
	//m_WayPonits[19]->SetRecentPoint(21);

	//m_WayPonits[20]->SetRecentPoint(19);
	//m_WayPonits[20]->SetRecentPoint(21);

	//m_WayPonits[21]->SetRecentPoint(19);
	//m_WayPonits[21]->SetRecentPoint(20);
	//m_WayPonits[21]->SetRecentPoint(12);

	//m_WayPonits[22]->SetRecentPoint(17);
	//m_WayPonits[22]->SetRecentPoint(23);
	//m_WayPonits[22]->SetRecentPoint(36);

	//m_WayPonits[23]->SetRecentPoint(22);
	//m_WayPonits[23]->SetRecentPoint(24);
	//m_WayPonits[23]->SetRecentPoint(36);

	//m_WayPonits[24]->SetRecentPoint(23);
	//m_WayPonits[24]->SetRecentPoint(25);
	//m_WayPonits[24]->SetRecentPoint(26);

	//m_WayPonits[25]->SetRecentPoint(24);

	//m_WayPonits[26]->SetRecentPoint(24);
	//m_WayPonits[26]->SetRecentPoint(27);
	//m_WayPonits[26]->SetRecentPoint(28);

	//m_WayPonits[27]->SetRecentPoint(26);
	//m_WayPonits[27]->SetRecentPoint(29);

	//m_WayPonits[28]->SetRecentPoint(26);
	//m_WayPonits[28]->SetRecentPoint(29);

	//m_WayPonits[29]->SetRecentPoint(27);
	//m_WayPonits[29]->SetRecentPoint(28);
	//m_WayPonits[29]->SetRecentPoint(30);

	//m_WayPonits[30]->SetRecentPoint(29);
	//m_WayPonits[30]->SetRecentPoint(31);

	//m_WayPonits[31]->SetRecentPoint(30);
	//m_WayPonits[31]->SetRecentPoint(32);

	//m_WayPonits[32]->SetRecentPoint(31);
	//m_WayPonits[32]->SetRecentPoint(33);
	//m_WayPonits[32]->SetRecentPoint(34);
	//m_WayPonits[32]->SetRecentPoint(35);

	//m_WayPonits[33]->SetRecentPoint(32);
	//m_WayPonits[33]->SetRecentPoint(34);
	//m_WayPonits[33]->SetRecentPoint(35);
	//m_WayPonits[33]->SetRecentPoint(36);

	//m_WayPonits[34]->SetRecentPoint(32);
	//m_WayPonits[34]->SetRecentPoint(33);
	//m_WayPonits[34]->SetRecentPoint(35);
	//m_WayPonits[34]->SetRecentPoint(37);

	//m_WayPonits[35]->SetRecentPoint(32);
	//m_WayPonits[35]->SetRecentPoint(33);
	//m_WayPonits[35]->SetRecentPoint(34);

	//m_WayPonits[36]->SetRecentPoint(22);
	//m_WayPonits[36]->SetRecentPoint(23);
	//m_WayPonits[36]->SetRecentPoint(33);

	//m_WayPonits[37]->SetRecentPoint(34);
	//m_WayPonits[37]->SetRecentPoint(38);

	//m_WayPonits[38]->SetRecentPoint(37);
	//m_WayPonits[38]->SetRecentPoint(39);

	//m_WayPonits[39]->SetRecentPoint(38);
	//m_WayPonits[39]->SetRecentPoint(40);

	//m_WayPonits[40]->SetRecentPoint(39);
	//m_WayPonits[40]->SetRecentPoint(41);

	//m_WayPonits[41]->SetRecentPoint(40);
	//m_WayPonits[41]->SetRecentPoint(42);

	//m_WayPonits[42]->SetRecentPoint(4);
	//m_WayPonits[42]->SetRecentPoint(5);
	//m_WayPonits[42]->SetRecentPoint(41);

	//CreateGraph();
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
