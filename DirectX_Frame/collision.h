//======================================================================
//	�����蔻��w�b�_[camera.h]
//
//======================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#define _OX_EPSILON_ 0.000001f // �덷

// 3����float
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

// �_
typedef Float3 Point;

// �x�N�g��
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

	// �W����
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

	// �����֌W�ɂ���H
	bool isVertical(const Vec3 &r) const 
	{
		float d = dot(r);
		return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// �덷�͈͓��Ȃ琂���Ɣ���
	}

	// ���s�֌W�ɂ���H
	bool isParallel(const Vec3 &r) const
	{
		float d = cross(r).lengthSq();
		return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// �덷�͈͓��Ȃ畽�s�Ɣ���
	}

	// �s�p�֌W�H
	bool isSharpAngle(const Vec3 &r) const 
	{
		return (dot(r) >= 0.0f);
	}
};

// ����
class Line 
{
public:
	Point p;
	Vec3 v;		// �����x�N�g��
	Line() : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f) {}
	Line(const Point &p, const Vec3 &v) : p(p), v(v) {}
	~Line() {}

	// �_��̍��W���擾
	//  �x�N�g���Ɋ|���Z����W��
	Point getPoint(float t) const 
	{
		return p + t * v;
	}
};

// ����
class Segment : public Line 
{
public:
	Segment() {}
	Segment(const Point &p, const Vec3 &v) : Line(p, v) {}
	Segment(const Point &p1, const Point &p2) : Line(p1, (p2 - p1)) {}

	// �I�_���擾
	Float3 getEndPoint() const 
	{
		return p + v;
	}
};

// �J�v�Z��
class Capsule
{
public:
	Segment s;
	float r;	// ���a
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

// ��
class Sphere
{
public:
	D3DXVECTOR3 pos;	// ���S���W
	float rad;			// ���a

	// �R���X�g���N�^
	Sphere()
	{
		rad = 1.0f;
	}
};

// OBB
class OBB
{
private:
	D3DXVECTOR3 m_Pos;					// �ʒu
	D3DXVECTOR3 m_NormalDirect[3];		// �����x�N�g��
	float m_fLength[3];					// �e�������̒���

public:
	D3DXVECTOR3 GetDirect(int elem);	// �w�莲�ԍ��̕����x�N�g�����擾
	float GetLen(int elem);				// �w�莲�����̒������擾
	D3DXVECTOR3 GetPos();				// �ʒu���擾

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

// �_�ƒ����̍ŒZ����
// p : �_
// l : ����
// h : �_���牺�낵�������̑��i�߂�l�j
// t :�x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcPointLineDist(const Point &p, const Line &l, Point &h, float &s);

// ��p1p2p3�͉s�p�H
bool isSharpAngle(const Point &p1, const Point &p2, const Point &p3);

// �_�Ɛ����̍ŒZ����
// p : �_
// seg : ����
// h : �ŒZ�����ƂȂ�[�_�i�߂�l�j
// t : �[�_�ʒu�i t < 0: �n�_�̊O, 0 <= t <= 1: ������, t > 1: �I�_�̊O �j 
// �߂�l: �ŒZ����
float calcPointSegmentDist(const Point &p, const Segment &seg, Point &h, float &t);

// 2�����̍ŒZ����
// l1 : L1
// l2 : L2
// p1 : L1���̐����̑��i�߂�l�j
// p2 : L2���̐����̑��i�߂�l�j
// t1 : L1���̃x�N�g���W���i�߂�l�j
// t2 : L2���̃x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcLineLineDist(const Line &l1, const Line &l2, Point &p1, Point &p2, float &t1, float &t2);

// 0�`1�̊ԂɃN�����v
void clamp01(float &v);

// 2�����̍ŒZ����
// s1 : S1(����1)
// s2 : S2(����2)
// p1 : S1���̐����̑��i�߂�l�j
// p2 : S2���̐����̑��i�߂�l�j
// t1 : S1���̃x�N�g���W���i�߂�l�j
// t2 : S2���̃x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcSegmentSegmentDist(const Segment &s1, const Segment &s2, Point &p1, Point &p2, float &t1, float &t2);

// �J�v�Z�����m�̏Փ˔���
// c1 : S1(����1)
// c2 : S2(����2)
// �߂�l: �Փ˂��Ă�����true
bool isCollisionCapsule(const Capsule &c1, const Capsule &c2);

//************************************************************************
//	�~�Ɖ~�̓����蔻��
//	�������A�������@1�ڂ̉~�̒��S���W
//	��O����		  �@1�ڂ̉~�̔��a
//	��l�����A��܈����@2�ڂ̉~�̒��S���W
//�@��Z����			2�ڂ̉~�̔��a
//	�攪�����@�T�C�Y
//************************************************************************
bool isCollisionCircle2D(float x0, float y0, float r0, float x1, float y1, float r1);

//************************************************************************
//	���Ɛ��̓����蔻��
//	�������@1�ڂ̐��̎n�_
//	�������@1�ڂ̐��̏I�_
//�@��O�����@2�ڂ̐��̎n�_
//�@��l�����@2�ڂ̐��̏I�_	
//************************************************************************
bool isCollisionLine2D(D3DXVECTOR3 &r1, D3DXVECTOR3 &r2, D3DXVECTOR3 &p1, D3DXVECTOR3 &p2);

//************************************************************************
//	���Ƌ��̓����蔻��
//************************************************************************
bool isCollisionSphere(Sphere &sphere1, Sphere &sphere2);

//************************************************************************
//	OBB�Ɠ_�̓����蔻��
//************************************************************************
float LenOBBToPoint(OBB &obb, D3DXVECTOR3 &point);

//************************************************************************
//	OBB�Ƌ��̓����蔻��
//************************************************************************
bool isCollisionOBBtoSphere(OBB &obb, Sphere &sphere);

//************************************************************************
//	OBB��OBB�̓����蔻��
//************************************************************************
bool isCollisionOBBs(OBB &obb1, OBB &obb2);

//************************************************************************
//	���ƕ��ʂ̓����蔻��
//************************************************************************
bool isCollisionSpheretoPlane(Sphere &sphere, D3DXVECTOR3 &v, D3DXVECTOR3 &n);

//************************************************************************
//	���ƕ��ʂ̓����蔻��
//************************************************************************
bool isCollisionLinetoPlane(D3DXVECTOR3 &v1, D3DXVECTOR3 &v2, D3DXVECTOR3 &n);

//************************************************************************
//	�_�ƕ��ʂ̋����𑪒�
//************************************************************************
float distancePointToPlane(D3DXVECTOR3 &vn, D3DXVECTOR3 &p1, D3DXVECTOR3 &p2);

//************************************************************************
//	�Փ˃|���S������
//************************************************************************
bool onPolygon(D3DXVECTOR3 &target, D3DXVECTOR3 &p1, D3DXVECTOR3 &p2, D3DXVECTOR3 &p3);

#endif // !_COLLISION_H_
