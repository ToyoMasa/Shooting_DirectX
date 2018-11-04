//======================================================================
//	フィールド　（2018/05/23)
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "scene.h"
#include "scene3D.h"
#include "texture.h"
#include "billboard.h"
#include "field.h"

float fieldHeight[21][21] =
{
	{ 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 1.5f, 1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
																														
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
																														
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.5f, 1.0f, 0.5f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 2.0f, 2.0f, 1.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 2.0f, 1.5f, 0.5f, 0.0f, 0.0f, 3.0f },
																														
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 1.5f, 1.0f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },
	{ 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f },

	{ 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f },
};

void CField::Init(int texId, float meshSize, int sizeX, int sizeY)
{
	m_TextureID = texId;
	m_MeshSize = meshSize;

	m_NumVertex = (sizeX + 1) * (sizeY + 1);				// 頂点数の計算
	m_NumPrimitive = sizeX * 2 * sizeY + 4 * (sizeY - 1);	// プリティブ数の計算
	m_NumIndex = m_NumPrimitive + 2;						// インデックス数の計算

	m_Scene3D = CScene3D::Create(texId, meshSize, sizeX, sizeY, m_NumPrimitive, m_NumVertex, m_NumIndex, LAYER_BACKGROUND);
}

void CField::Init(int texId, float meshSize, int sizeX, int sizeY, bool isHeight)
{
	m_TextureID = texId;
	m_MeshSize = meshSize;

	m_NumVertex = (sizeX + 1) * (sizeY + 1);				// 頂点数の計算
	m_NumPrimitive = sizeX * 2 * sizeY + 4 * (sizeY - 1);	// プリティブ数の計算
	m_NumIndex = m_NumPrimitive + 2;						// インデックス数の計算

	VERTEX_3D* V = new VERTEX_3D[m_NumVertex];
	for (int j = 0; j < sizeY + 1; j++)
	{
		for (int i = 0; i < sizeX + 1; i++)
		{
			V[i + (sizeX + 1) * j] = { D3DXVECTOR3(-(meshSize * sizeX / 2) + (meshSize * i), 0.0f, (meshSize * sizeY / 2) - (meshSize * j)), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2((float)i ,  (float)j) };
		}
	}

	// テスト
	for (int j = 0; j < 21; j++)
	{
		for (int i = 0; i < 21; i++)
		{
			V[i + 21 * j].pos.y = fieldHeight[j][i];
		}
	}

	// 法線ベクトルの計算
	for (int z = 1; z < sizeY + 1; z++)
	{
		for (int x = 1; x < sizeX + 1; x++)
		{
			D3DXVECTOR3 vx, vz, nx, nz, n;

			vx = V[z * sizeY + x + 1].pos - V[z * sizeY + x - 1].pos;
			nx.x = -vx.y;
			nx.y = vx.x;
			nx.z = 0.0f;

			vz = V[z * sizeY + x - 1].pos - V[z * sizeY + x].pos;
			nz.x = -vz.y;
			nz.y = vz.x;
			nz.z = 0.0f;

			n = nx + nz;
			D3DXVec3Normalize(&n, &n);

			V[z * sizeY + x].NV = n;
		}
	}

	LPWORD Index = new WORD[m_NumIndex];

	int numindex = 0;

	for (int j = 0; j < sizeY; j++)
	{
		for (int i = 0; i < sizeX + 1; i++)
		{
			Index[numindex] = j * (sizeX + 1) + i + sizeX + 1;
			numindex++;
			Index[numindex] = j * (sizeX + 1) + i;
			numindex++;

			if (i == sizeX && !(j == sizeY - 1))
			{
				Index[numindex] = j * (sizeX + 1) + i;
				numindex++;
				Index[numindex] = (j + 1) * (sizeX + 1) + sizeX + 1;
				numindex++;
			}
		}
	}

	m_Scene3D = CScene3D::Create(texId, V, Index, m_NumPrimitive, m_NumVertex, m_NumIndex);

	delete V;
	delete Index;
}

void CField::Uninit()
{

}

void CField::Update()
{

}

void CField::Draw()
{

}

float CField::GetHeight(D3DXVECTOR3& pos)
{
	float cross1, cross2, cross3;
	D3DXVECTOR3 V0P, V1P, V2P, V01, V12, V20;
	D3DXVECTOR3 P0, P1, P2;

	VERTEX_3D* vertex = m_Scene3D->GetVertex();

	for (int z = 0; z < m_SizeY + 1; z++)
	{
		for (int x = 0; x < m_SizeX + 1; x++)
		{
			P0 = vertex[(z + 1) * (m_SizeY + 1) + x].pos;
			P1 = vertex[z * (m_SizeY + 1) + x].pos;
			P2 = vertex[(z + 1) * (m_SizeY + 1) + (x + 1)].pos;

			V01 = P1 - P0;
			V12 = P2 - P1;
			V20 = P0 - P2;

			V0P = pos - P0;
			V1P = pos - P1;
			V2P = pos - P2;
			
			cross1 = V01.x * V0P.z - V01.z * V0P.x;
			cross2 = V12.x * V1P.z - V12.z * V1P.x;
			cross3 = V20.x * V2P.z - V20.z * V2P.x;

			if (cross1 <= 0.0f && cross2 <= 0.0f && cross3 <= 0.0f)
			{
				D3DXVECTOR3 n;
				D3DXVec3Cross(&n, &V01, &V20);

				D3DXVec3Normalize(&n, &n);

				float y = P0.y - (n.x * (pos.x - P0.x) + n.z * (pos.z - P0.z)) / n.y;
				return y;
			}
		}
	
		for (int x = 0; x < m_SizeX + 1; x++)
		{
			P0 = vertex[z * (m_SizeY + 1)+(x + 1)].pos;
			P1 = vertex[(z + 1) * (m_SizeY + 1)+(x + 1)].pos;
			P2 = vertex[z * (m_SizeY + 1) + x].pos;

			V01 = P1 - P0;
			V12 = P2 - P1;
			V20 = P0 - P2;

			V0P = pos - P0;
			V1P = pos - P1;
			V2P = pos - P2;

			cross1 = V01.x * V0P.z - V01.z * V0P.x;
			cross2 = V12.x * V1P.z - V12.z * V1P.x;
			cross3 = V20.x * V2P.z - V20.z * V2P.x;

			if (cross1 <= 0.0f && cross2 <= 0.0f && cross3 <= 0.0f)
			{
				D3DXVECTOR3 n;
				D3DXVec3Cross(&n, &V20, &V01);

				D3DXVec3Normalize(&n, &n);

				float y = P0.y - (n.x * (pos.x - P0.x) + n.z * (pos.z - P0.z)) / n.y;
				return y;
			}
		}
	}

	return 0.0f;
}

CField* CField::Create(int texId, float meshSize, int sizeX, int sizeY)
{
	CField* field = new CField(LAYER_BACKGROUND);
	field->Init(texId, meshSize, sizeX, sizeY);
	field->m_SizeX = sizeX;
	field->m_SizeY = sizeY;

	return field;
}

CField* CField::Create(int texId, float meshSize, int sizeX, int sizeY, bool isHeight)
{
	CField* field = new CField(LAYER_BACKGROUND);
	field->Init(texId, meshSize, sizeX, sizeY, isHeight);
	field->m_SizeX = sizeX;
	field->m_SizeY = sizeY;

	return field;
}
