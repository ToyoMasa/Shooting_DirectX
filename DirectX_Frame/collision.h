//======================================================================
//	当たり判定ヘッダ[camera.h]
//
//======================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#define _OX_EPSILON_ 0.000001f // 誤差

// 3成分float
class Float3 
{
public:
	float x, y, z;

	Float3() {}
	Float3(float x, float y, float z) : x(x), y(y), z(z) {}
	~Float3() {}

	Float3 operator +(const Float3 &r) const 
	{
		return Float3(x + r.x, y + r.y, z + r.z);
	}

	Float3 operator -(const Float3 &r) const 
	{
		return Float3(x - r.x, y - r.y, z - r.z);
	}

	Float3 operator -() const 
	{
		return Float3(x * -1.0f, y * -1.0f, z * -1.0f);
	}

	Float3 operator *(const Float3 &r) const 
	{
		return Float3(x * r.x, y * r.y, z * r.z);
	}

	Float3 operator /(const Float3 &r) const
	{
		return Float3(x / r.x, y / r.y, z / r.z);
	}

	Float3 operator *(float r) const 
	{
		return Float3(x * r, y * r, z * r);
	}

	Float3 operator /(float r) const
	{
		return Float3(x / r, y / r, z / r);
	}

	friend Float3 operator *(float l, const Float3 &r)
	{
		return Float3(r.x * l, r.y * l, r.z * l);
	}

	friend Float3 operator /(float l, const Float3 &r) 
	{
		return Float3(r.x / l, r.y / l, r.z / l);
	}

	float dot(const Float3 &r) const 
	{
		return x * r.x + y * r.y + z * r.z;
	}

	Float3 cross(const Float3 &r) const
	{
		return Float3(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
	}

	float length() const
	{
		return sqrtf(lengthSq());
	}

	float lengthSq() const 
	{
		return x * x + y * y + z * z;
	}

	void norm() {
		const float len = length();
		if (len > 0.0f) 
		{
			x /= len;
			y /= len;
			z /= len;
		}
	}

	Float3 getNorm() const 
	{
		const float len = length();
		if (len > 0.0f) 
		{
			return Float3(x / len, y / len, z / len);
		}
		return Float3(0.0f, 0.0f, 0.0f);
	}
};

// 点
typedef Float3 Point;

// ベクトル
class Vec3 : public Float3 
{
public:
	Vec3() {}
	Vec3(float x, float y, float z) : Float3(x, y, z) {}
	Vec3(const Float3 &r) : Float3(r) {}
	~Vec3() {}

	Vec3& operator =(const Float3 &r) 
	{
		x = r.x;
		y = r.y;
		z = r.z;
		return *this;
	}

	// 標準化
	void norm()
	{
		const float len = length();
		if (len > 0.0f)
		{
			x /= len;
			y /= len;
			z /= len;
		}
	}

	// 垂直関係にある？
	bool isVertical(const Vec3 &r) const 
	{
		float d = dot(r);
		return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// 誤差範囲内なら垂直と判定
	}

	// 平行関係にある？
	bool isParallel(const Vec3 &r) const
	{
		float d = cross(r).lengthSq();
		return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// 誤差範囲内なら平行と判定
	}

	// 鋭角関係？
	bool isSharpAngle(const Vec3 &r) const 
	{
		return (dot(r) >= 0.0f);
	}
};

// 直線
class Line 
{
public:
	Point p;
	Vec3 v;		// 方向ベクトル
	Line() : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f) {}
	Line(const Point &p, const Vec3 &v) : p(p), v(v) {}
	~Line() {}

	// 点上の座標を取得
	//  ベクトルに掛け算する係数
	Point getPoint(float t) const 
	{
		return p + t * v;
	}
};

// 線分
class Segment : public Line 
{
public:
	Segment() {}
	Segment(const Point &p, const Vec3 &v) : Line(p, v) {}
	Segment(const Point &p1, const Point &p2) : Line(p1, (p2 - p1)) {}

	// 終点を取得
	Float3 getEndPoint() const 
	{
		return p + v;
	}
};

// カプセル
class Capsule
{
public:
	Segment s;
	float r;	// 半径
	Capsule() : r(0.5f) {}
	Capsule(const Segment &s, float r) : s(s), r(r) {}
	Capsule(const Point &p1, const Point &p2, float r) : s(p1, p2), r(r) {}
	~Capsule() {}

	void Set(const Point &p1, const Point &p2, float r)
	{
		Segment seg(p1, p2);
		s = seg;
		this->r = r;
	}
};

// 球
class Sphere
{
public:
	D3DXVECTOR3 pos;	// 中心座標
	float rad;			// 半径

	// コンストラクタ
	Sphere()
	{
		rad = 1.0f;
	}
};

// OBB
class OBB
{
private:
	D3DXVECTOR3 m_Pos;					// 位置
	D3DXVECTOR3 m_NormalDirect[3];		// 方向ベクトル
	float m_fLength[3];					// 各軸方向の長さ

public:
	D3DXVECTOR3 GetDirect(int elem);	// 指定軸番号の方向ベクトルを取得
	float GetLen(int elem);				// 指定軸方向の長さを取得
	D3DXVECTOR3 GetPos();				// 位置を取得

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetNormal(int index, D3DXVECTOR3 normal) { m_NormalDirect[index] = normal; }
	void SetLength(float x, float y, float z)
	{
		m_fLength[0] = x;
		m_fLength[1] = y;
		m_fLength[2] = z;
	}

	OBB()
	{
		m_NormalDirect[0] = D3DXVECTOR3(1, 0, 0);
		m_NormalDirect[1] = D3DXVECTOR3(0, 1, 0);
		m_NormalDirect[2] = D3DXVECTOR3(0, 0, 1);
	}
};

// 点と直線の最短距離
// p : 点
// l : 直線
// h : 点から下ろした垂線の足（戻り値）
// t :ベクトル係数（戻り値）
// 戻り値: 最短距離
float calcPointLineDist(const Point &p, const Line &l, Point &h, float &s);

// ∠p1p2p3は鋭角？
bool isSharpAngle(const Point &p1, const Point &p2, const Point &p3);

// 点と線分の最短距離
// p : 点
// seg : 線分
// h : 最短距離となる端点（戻り値）
// t : 端点位置（ t < 0: 始点の外, 0 <= t <= 1: 線分内, t > 1: 終点の外 ） 
// 戻り値: 最短距離
float calcPointSegmentDist(const Point &p, const Segment &seg, Point &h, float &t);

// 2直線の最短距離
// l1 : L1
// l2 : L2
// p1 : L1側の垂線の足（戻り値）
// p2 : L2側の垂線の足（戻り値）
// t1 : L1側のベクトル係数（戻り値）
// t2 : L2側のベクトル係数（戻り値）
// 戻り値: 最短距離
float calcLineLineDist(const Line &l1, const Line &l2, Point &p1, Point &p2, float &t1, float &t2);

// 0～1の間にクランプ
void clamp01(float &v);

// 2線分の最短距離
// s1 : S1(線分1)
// s2 : S2(線分2)
// p1 : S1側の垂線の足（戻り値）
// p2 : S2側の垂線の足（戻り値）
// t1 : S1側のベクトル係数（戻り値）
// t2 : S2側のベクトル係数（戻り値）
// 戻り値: 最短距離
float calcSegmentSegmentDist(const Segment &s1, const Segment &s2, Point &p1, Point &p2, float &t1, float &t2);

// カプセル同士の衝突判定
// c1 : S1(線分1)
// c2 : S2(線分2)
// 戻り値: 衝突していたらtrue
bool isCollisionCapsule(const Capsule &c1, const Capsule &c2);

//************************************************************************
//	円と円の当たり判定
//	第一引数、第二引数　1つ目の円の中心座標
//	第三引数		  　1つ目の円の半径
//	第四引数、第五引数　2つ目の円の中心座標
//　第六引数			2つ目の円の半径
//	第八引数　サイズ
//************************************************************************
bool isCollisionCircle2D(float x0, float y0, float r0, float x1, float y1, float r1);

//************************************************************************
//	線と線の当たり判定
//	第一引数　1つ目の線の始点
//	第二引数　1つ目の線の終点
//　第三引数　2つ目の線の始点
//　第四引数　2つ目の線の終点	
//************************************************************************
bool isCollisionLine2D(D3DXVECTOR3 &r1, D3DXVECTOR3 &r2, D3DXVECTOR3 &p1, D3DXVECTOR3 &p2);

//************************************************************************
//	球と球の当たり判定
//************************************************************************
bool isCollisionSphere(Sphere &sphere1, Sphere &sphere2);

//************************************************************************
//	OBBと点の当たり判定
//************************************************************************
float LenOBBToPoint(OBB &obb, D3DXVECTOR3 &point);

//************************************************************************
//	OBBと球の当たり判定
//************************************************************************
bool isCollisionOBBtoSphere(OBB &obb, Sphere &sphere);

//************************************************************************
//	OBBとOBBの当たり判定
//************************************************************************
bool isCollisionOBBs(OBB &obb1, OBB &obb2);

//************************************************************************
//	球と平面の当たり判定
//************************************************************************
bool isCollisionSpheretoPlane(Sphere &sphere, D3DXVECTOR3 &v, D3DXVECTOR3 &n);

//************************************************************************
//	線と平面の当たり判定
//************************************************************************
bool isCollisionLinetoPlane(D3DXVECTOR3 &v1, D3DXVECTOR3 &v2, D3DXVECTOR3 &n);

//************************************************************************
//	点と平面の距離を測定
//************************************************************************
float distancePointToPlane(D3DXVECTOR3 &vn, D3DXVECTOR3 &p1, D3DXVECTOR3 &p2);

//************************************************************************
//	衝突ポリゴン判定
//************************************************************************
bool onPolygon(D3DXVECTOR3 &target, D3DXVECTOR3 &p1, D3DXVECTOR3 &p2, D3DXVECTOR3 &p3);

#endif // !_COLLISION_H_
