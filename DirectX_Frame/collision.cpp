//======================================================================
//	�����蔻�菈�� [collision.cpp]�@�i2017/12/05�j
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "collision.h"
#include <cmath>

//======================================================================
//	���Ƌ��̓����蔻��
//======================================================================
bool isCollisionSphere(Sphere &sphere1, Sphere &sphere2)
{
	return
		(sphere2.GetPos().x - sphere1.GetPos().x) * (sphere2.GetPos().x - sphere1.GetPos().x) + 
		(sphere2.GetPos().y - sphere1.GetPos().y) * (sphere2.GetPos().y - sphere1.GetPos().y) + 
		(sphere2.GetPos().z - sphere1.GetPos().z) * (sphere2.GetPos().z - sphere1.GetPos().z) <=
		(sphere1.GetRad() + sphere2.GetRad()) * (sphere1.GetRad() + sphere2.GetRad());
}

//======================================================================
//	OBB�Ɠ_�̋����𑪒�
//======================================================================
float LenOBBToPoint(OBB &obb, D3DXVECTOR3 &point)
{
	D3DXVECTOR3 Vec(0, 0, 0);   // �ŏI�I�ɒ��������߂�x�N�g��

	// �e���ɂ��Ă͂ݏo�������̃x�N�g�����Z�o
	for (int i = 0; i<3; i++)
	{
		float L = obb.GetLen(i);
		if (L <= 0) continue;  // L=0�͌v�Z�ł��Ȃ�
		float s = D3DXVec3Dot(&(point - obb.GetPos()), &obb.GetDirect(i)) / L;

		// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
		s = fabs(s);
		if (s > 1)
		{
			Vec += (1 - s)*L*obb.GetDirect(i);   // �͂ݏo���������̃x�N�g���Z�o
		}
	}

	return D3DXVec3Length(&Vec);   // �������o��
}

//======================================================================
//	OBB�Ƌ��̓����蔻��
//======================================================================
bool isCollisionOBBtoSphere(OBB &obb, Sphere &sphere)
{
	return LenOBBToPoint(obb, sphere.GetPos()) <= sphere.GetRad();
}

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = 0)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
	float r1 = fabs(D3DXVec3Dot(Sep, e1));
	float r2 = fabs(D3DXVec3Dot(Sep, e2));
	float r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}

//======================================================================
//	AABB�Ɠ_�̓����蔻��
//======================================================================
bool isCollisionAABBToPoint(AABB &aabb, D3DXVECTOR3 &point)
{
	if (point.x < aabb.GetMin().x) return false;
	if (point.x > aabb.GetMax().x) return false;
	if (point.y < aabb.GetMin().y) return false;
	if (point.y > aabb.GetMax().y) return false;
	if (point.z < aabb.GetMin().z) return false;
	if (point.z > aabb.GetMax().z) return false;

	return true;
}

//======================================================================
//	OBB���m�̓����蔻��
//======================================================================
bool isCollisionOBBs(OBB &obb1, OBB &obb2)
{
	// �e�����x�N�g���̊m��
	// �iN***:�W���������x�N�g���j
	D3DXVECTOR3 NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen(0);
	D3DXVECTOR3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen(1);
	D3DXVECTOR3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen(2);
	D3DXVECTOR3 NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen(0);
	D3DXVECTOR3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen(1);
	D3DXVECTOR3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen(2);
	D3DXVECTOR3 Interval = obb1.GetPos() - obb2.GetPos();

	// ������ : Ae1
	float rA = D3DXVec3Length(&Ae1);
	float rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	float L = fabs(D3DXVec3Dot(&Interval, &NAe1));
	if (L > rA + rB)
		return false; // �Փ˂��Ă��Ȃ�

					  // ������ : Ae2
	rA = D3DXVec3Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NAe2));
	if (L > rA + rB)
		return false;

	// ������ : Ae3
	rA = D3DXVec3Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NAe3));
	if (L > rA + rB)
		return false;

	// ������ : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be1);
	L = fabs(D3DXVec3Dot(&Interval, &NBe1));
	if (L > rA + rB)
		return false;

	// ������ : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be2);
	L = fabs(D3DXVec3Dot(&Interval, &NBe2));
	if (L > rA + rB)
		return false;

	// ������ : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NBe3));
	if (L > rA + rB)
		return false;

	// ������ : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
}

// �w�莲�ԍ��̕����x�N�g�����擾
D3DXVECTOR3 OBB::GetDirect(int elem)
{
	return m_NormalDirect[elem];
}

// �w�莲�����̒������擾
float OBB::GetLen(int elem)
{
	return m_fLength[elem];
}

// �ʒu���擾
D3DXVECTOR3 OBB::GetPos()
{
	return m_Pos;
}

//======================================================================
//	���ƕ��ʂ̓����蔻��(�Ώۂ̋�, ���ʏ�̓_�C���ʂ̖@���x�N�g��)
//======================================================================
bool isCollisionSpheretoPlane(Sphere &sphere, D3DXVECTOR3 &v, D3DXVECTOR3 &n)
{
	return distancePointToPlane(n, sphere.GetPos(), v) <= sphere.GetRad();
}

//======================================================================
//	�����ƕ��ʂ̓����蔻��(�����̎n�_�C�����̏I�_, ���ʂ̖@���x�N�g��)
//======================================================================
bool isCollisionLinetoPlane(D3DXVECTOR3 &v1, D3DXVECTOR3 &v2, D3DXVECTOR3 &n)
{
	return D3DXVec3Dot(&v1, &n) * D3DXVec3Dot(&v2, &n) <= 0;
}

//======================================================================
//	�_�ƕ��ʂ̋������擾(���ʂ̖@���x�N�g��, �_�̍��W, ���ʏ�̓_�̍��W)
//======================================================================
float distancePointToPlane(D3DXVECTOR3 &vn, D3DXVECTOR3 &p1, D3DXVECTOR3 &p2)
{
	D3DXVECTOR3 v = p2 - p1;
	return fabs(D3DXVec3Dot(&vn, &v));
}

//======================================================================
//	2D�p�~�Ɖ~�̓����蔻��
//======================================================================
bool isCollisionCircle2D(float x0, float y0, float r0, float x1, float y1, float r1)
{
	float x = x1 - x0;
	float y = y1 - y0;
	float l = x * x + y * y;	//�Εӂ̓��̒����i��擯�m�Ōv�Z����j

	return l < (r0 + r1) * (r0 + r1);
}

//======================================================================
//�@2D�p���Ɛ��̓����蔻��
//======================================================================
bool isCollisionLine2D(D3DXVECTOR3 &r1, D3DXVECTOR3 &r2, D3DXVECTOR3 &p1, D3DXVECTOR3 &p2)
{
	float t1, t2;

	//2�_r1-r2��ʂ钼���ɑ΂��Ap1�Ap2���ǂ̈ʒu�ɂ��邩�v�Z����
	t1 = (r1.x - r2.x) * (p1.y - r1.z) + (r1.y - r2.z) * (r1.x - p1.x);
	t2 = (r1.x - r2.x) * (p2.y - r1.z) + (r1.y - r2.z) * (r1.x - p2.x);

	//���ꂼ��̐������قȂ�i�ς����ɂȂ�j���A0�i�_��������ɂ���j
	//�Ȃ�N���X���Ă���
	if (t1 * t2 < 0 || t1 == 0 || t2 == 0)
	{
		return true; //�N���X���Ă���
	}
	else
	{
		return false; //�N���X���Ȃ�
	}
}

bool onPolygon(D3DXVECTOR3 &target, D3DXVECTOR3 &p0, D3DXVECTOR3 &p1, D3DXVECTOR3 &p2)
{
	float cross1, cross2, cross3;
	D3DXVECTOR3 V0P, V1P, V2P, V01, V12, V20;

	V0P = target - p0;
	V1P = target - p1;
	V2P = target - p2;
	V01 = p1 - p0;
	V12 = p2 - p1;
	V20 = p0 - p1;

	cross1 = V01.x * V0P.z - V01.z * V0P.x;
	cross2 = V12.x * V1P.z - V12.z * V1P.x;
	cross3 = V20.x * V2P.z - V20.z * V2P.x;

	if (cross1 <= 0.0f)
	{
		if (cross2 <= 0.0f)
		{
			if (cross3 <= 0.0f)
			{
				return true;
			}
		}
	}

	return false;
}

// �_�ƒ����̍ŒZ����
// p : �_
// l : ����
// h : �_���牺�낵�������̑��i�߂�l�j
// t :�x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcPointLineDist(const Point &p, const Line &l, Point &h, float &s) {

	float lenSqV = l.v.lengthSq();
	float t = 0.0f;
	if (lenSqV > 0.0f)
		t = l.v.dot(p - l.p) / lenSqV;

	h = l.p + s * l.v;
	return (h - p).length();
}

// ��p1p2p3�͉s�p�H
bool isSharpAngle(const Point &p1, const Point &p2, const Point &p3)
{
	Vec3 p12(p1 - p2);

	return p12.isSharpAngle(p3 - p2);
}

// �_�Ɛ����̍ŒZ����
// p : �_
// seg : ����
// h : �ŒZ�����ƂȂ�[�_�i�߂�l�j
// t : �[�_�ʒu�i t < 0: �n�_�̊O, 0 <= t <= 1: ������, t > 1: �I�_�̊O �j 
// �߂�l: �ŒZ����
float calcPointSegmentDist(const Point &p, const Segment &seg, Point &h, float &t) {

	const Point e = seg.getEndPoint();

	// �����̒����A�����̑��̍��W�y��t���Z�o
	float len = calcPointLineDist(p, Line(seg.p, e - seg.p), h, t);

	if (isSharpAngle(p, seg.p, e) == false) {
		// �n�_���̊O��
		h = seg.p;
		return (seg.p - p).length();
	}
	else if (isSharpAngle(p, e, seg.p) == false) {
		// �I�_���̊O��
		h = e;
		return (e - p).length();
	}

	return len;
}

// 2�����̍ŒZ����
// l1 : L1
// l2 : L2
// p1 : L1���̐����̑��i�߂�l�j
// p2 : L2���̐����̑��i�߂�l�j
// t1 : L1���̃x�N�g���W���i�߂�l�j
// t2 : L2���̃x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcLineLineDist(const Line &l1, const Line &l2, Point &p1, Point &p2, float &t1, float &t2) {

	// 2���������s�H
	if (l1.v.isParallel(l2.v) == true) {

		// �_P11�ƒ���L2�̍ŒZ�����̖��ɋA��
		float len = calcPointLineDist(l1.p, l2, p2, t2);
		p1 = l1.p;
		t1 = 0.0f;

		return len;
	}

	// 2�����͂˂���֌W
	float DV1V2 = l1.v.dot(l2.v);
	float DV1V1 = l1.v.lengthSq();
	float DV2V2 = l2.v.lengthSq();
	Vec3 P21P11 = l1.p - l2.p;
	t1 = (DV1V2 * l2.v.dot(P21P11) - DV2V2 * l1.v.dot(P21P11)) / (DV1V1 * DV2V2 - DV1V2 * DV1V2);
	p1 = l1.getPoint(t1);
	t2 = l2.v.dot(p1 - l2.p) / DV2V2;
	p2 = l2.getPoint(t2);

	return (p2 - p1).length();
}

// 0�`1�̊ԂɃN�����v
void clamp01(float &v) {
	if (v < 0.0f)
		v = 0.0f;
	else if (v > 1.0f)
		v = 1.0f;
}

// 2�����̍ŒZ����
// s1 : S1(����1)
// s2 : S2(����2)
// p1 : S1���̐����̑��i�߂�l�j
// p2 : S2���̐����̑��i�߂�l�j
// t1 : S1���̃x�N�g���W���i�߂�l�j
// t2 : S2���̃x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
float calcSegmentSegmentDist(const Segment &s1, const Segment &s2, Point &p1, Point &p2, float &t1, float &t2) 
{

	// S1���k�ނ��Ă���H
	if (s1.v.lengthSq() < _OX_EPSILON_) {
		// S2���k�ށH
		if (s2.v.lengthSq() < _OX_EPSILON_) {
			// �_�Ɠ_�̋����̖��ɋA��
			float len = (s2.p - s1.p).length();
			p1 = s1.p;
			p2 = s2.p;
			t1 = t2 = 0.0f;
			return len;
		}
		else {
			// S1�̎n�_��S2�̍ŒZ���ɋA��
			float len = calcPointSegmentDist(s1.p, s2, p2, t2);
			p1 = s1.p;
			t1 = 0.0f;
			clamp01(t2);
			return len;
		}
	}

	// S2���k�ނ��Ă���H
	else if (s2.v.lengthSq() < _OX_EPSILON_) {
		// S2�̎n�_��S1�̍ŒZ���ɋA��
		float len = calcPointSegmentDist(s2.p, s1, p1, t1);
		p2 = s2.p;
		clamp01(t1);
		t2 = 0.0f;
		return len;
	}

	/* �������m */

	// 2���������s�������琂���̒[�_�̈��P1�ɉ�����
	if (s1.v.isParallel(s2.v) == true) {
		t1 = 0.0f;
		p1 = s1.p;
		float len = calcPointSegmentDist(s1.p, s2, p2, t2);
		if (0.0f <= t2 && t2 <= 1.0f)
			return len;
	}
	else {
		// �����͂˂���̊֌W
		// 2�����Ԃ̍ŒZ���������߂ĉ���t1,t2�����߂�
		float len = calcLineLineDist(s1, s2, p1, p2, t1, t2);
		if (
			0.0f <= t1 && t1 <= 1.0f &&
			0.0f <= t2 && t2 <= 1.0f
			) {
			return len;
		}
	}

	// �����̑����O�ɂ��鎖������
	// S1����t1��0�`1�̊ԂɃN�����v���Đ������~�낷
	clamp01(t1);
	p1 = s1.getPoint(t1);
	float len = calcPointSegmentDist(p1, s2, p2, t2);
	if (0.0f <= t2 && t2 <= 1.0f)
		return len;

	// S2�����O�������̂�S2�����N�����v�AS1�ɐ������~�낷
	clamp01(t2);
	p2 = s2.getPoint(t2);
	len = calcPointSegmentDist(p2, s1, p1, t1);
	if (0.0f <= t1 && t1 <= 1.0f)
		return len;

	// �o���̒[�_���ŒZ�Ɣ���
	clamp01(t1);
	p1 = s1.getPoint(t1);
	return (p2 - p1).length();
}

// �J�v�Z�����m�̏Փ˔���
// c1 : S1(����1)
// c2 : S2(����2)
// �߂�l: �Փ˂��Ă�����true
bool isCollisionCapsule(Capsule c1, Capsule c2) 
{
	Point p1, p2;
	float t1, t2;
	float d = calcSegmentSegmentDist(c1.GetSegment(), c2.GetSegment(), p1, p2, t1, t2);
	return (d <= c1.GetRad() + c2.GetRad());
}

// lx, ly, lz : ���C�̎n�_
// vx, vy, vz : ���C�̕����x�N�g��
// px, py, pz : ���̒��S�_�̍��W
// r : ���̔��a
// q1x, q1y, q1z: �ՓˊJ�n�_�i�߂�l�j
// q2x, q2y, q2z: �ՓˏI���_�i�߂�l�j
bool calcRaySphere(
	D3DXVECTOR3 ray,
	D3DXVECTOR3 vec,
	D3DXVECTOR3 pos,
	float r,
	D3DXVECTOR3& q1,
	D3DXVECTOR3& q2)
{
	pos = pos - ray;

	float A = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	float B = vec.x * pos.x + vec.y * pos.y + vec.z * pos.z;
	float C = pos.x * pos.x + pos.y * pos.y + pos.z * pos.z - r * r;

	if (A == 0.0f)
		return false; // ���C�̒�����0

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // �Փ˂��Ă��Ȃ�

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
		return false; // ���C�̔��΂ŏՓ�

	q1.x = ray.x + a1 * vec.x;
	q1.y = ray.y + a1 * vec.y;
	q1.z = ray.z + a1 * vec.z;
	q2.x = ray.x + a2 * vec.x;
	q2.y = ray.y + a2 * vec.y;
	q2.z = ray.z + a2 * vec.z;

	return true;
}

//	lx, ly, lz : ���C�̎n�_
//	vx, vy, vz : ���C�̕����x�N�g��
//	p1x, p1y, p1z: �~������1�_
//	p2x, p2y, p2z: �~�����̂���1�_
//	r : �~���̔��a
//	q1x, q1y, q1z: �ђʊJ�n�_�i�߂�l�j
//	q2x, q2y, q2z: �ђʏI���_�i�߂�l�j
bool calcRayInfCilinder(
	D3DXVECTOR3 ray,
	D3DXVECTOR3 vec,
	D3DXVECTOR3 pos1,
	D3DXVECTOR3 pos2,
	float r,
	D3DXVECTOR3& q1,
	D3DXVECTOR3& q2)
{
	float px = pos1.x - ray.x;
	float py = pos1.y - ray.y;
	float pz = pos1.z - ray.z;
	pos2.x = pos2.x - ray.x;
	pos2.y = pos2.y - ray.y;
	pos2.z = pos2.z - ray.z;
	float sx = pos2.x - px;
	float sy = pos2.y - py;
	float sz = pos2.z - pz;

	// �e����ϒl
	float Dvv = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	float Dsv = sx * vec.x + sy * vec.y + sz * vec.z;
	float Dpv = px * vec.x + py * vec.y + pz * vec.z;
	float Dss = sx * sx + sy * sy + sz * sz;
	float Dps = px * sx + py * sy + pz * sz;
	float Dpp = px * px + py * py + pz * pz;
	float rr = r * r;

	if (Dss == 0.0f)
		return false; // �~������`����Ȃ�

	float A = Dvv - Dsv * Dsv / Dss;
	float B = Dpv - Dps * Dsv / Dss;
	float C = Dpp - Dps * Dps / Dss - rr;

	if (A == 0.0f)
		return false;

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // ���C���~���ƏՓ˂��Ă��Ȃ�
	s = sqrtf(s);

	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	q1.x = ray.x + a1 * vec.x;
	q1.y = ray.y + a1 * vec.y;
	q1.z = ray.z + a1 * vec.z;
	q2.x = ray.x + a2 * vec.x;
	q2.y = ray.y + a2 * vec.y;
	q2.z = ray.z + a2 * vec.z;

	return true;
}

// ��P1P2P3�̓��ς��Z�o
// x1, y1, z1: �_P1
// x2, y2, z2: �_P2
// x3, y3, z3: �_P3
float checkDot(const float& x1, const float& y1, const float& z1, const float& x2, const float& y2, const float& z2, const float& x3, const float& y3, const float& z3) {
	return ((x1 - x2) * (x3 - x2) + (y1 - y2) * (y3 - y2) + (z1 - z2) * (z3 - z2));
}
float checkDot(const D3DXVECTOR3& vec1, const D3DXVECTOR3& vec2, const D3DXVECTOR3& vec3) {
	return (vec1.x - vec2.x) * (vec3.x - vec2.x) + (vec1.y - vec2.y) * (vec3.y - vec2.y) + (vec1.z - vec2.z) * (vec3.z - vec2.z);
}

// ���C�ƃJ�v�Z���̊ђʓ_���Z�o
//	lx, ly, lz : ���C�̎n�_
//	vx, vy, vz : ���C�̕����x�N�g��
//	p1x, p1y, p1z: �J�v�Z�����̒[�_P1
//	p2x, p2y, p2z: �J�v�Z�����̒[�_P2
//	r : �J�v�Z���̔��a
//	q1x, q1y, q1z: �ђʊJ�n�_�i�߂�l�j
//	q2x, q2y, q2z: �ђʏI���_�i�߂�l�j
bool calcRayCapsule(
	D3DXVECTOR3 ray,
	D3DXVECTOR3 vec,
	D3DXVECTOR3 pos1,
	D3DXVECTOR3 pos2,
	float r,
	D3DXVECTOR3& q1,
	D3DXVECTOR3& q2)
{
	bool Q1inP1 = false;
	bool Q1inP2 = false;
	bool Q1inCld = false;

	// Q1�̌���
	if (
		calcRaySphere(ray, vec, pos1, r, q1, q2) == true &&
		checkDot(pos2, pos1, q1) <= 0.0f
		) {
		Q1inP1 = true; // Q1�͋���P1��ɂ���

	}
	else if (
		calcRaySphere(ray, vec, pos2, r, q1, q2) == true &&
		checkDot(pos1, pos2, q1) <= 0.0f
		) {
		Q1inP2 = true; // Q1�͋���P2��ɂ���

	}
	else if (
		calcRayInfCilinder(ray, vec, pos1, pos2, r, q1, q2) == true &&
		checkDot(pos1, pos2, q1) > 0.0f &&
		checkDot(pos2, pos1, q1) > 0.0f
		) {
		Q1inCld = true; // Q1�͉~���ʂɂ���

	}
	else
		return false; // ���C�͏Փ˂��Ă��Ȃ�

					  // Q2�̌���
	D3DXVECTOR3 t; // �_�~�[
	if (Q1inP1 && checkDot(pos2, pos1, q2) <= 0.0f) {
		// Q1�AQ2����P1��ɂ���
		return true;

	}
	else if (Q1inP2 && checkDot(pos1, pos2, q2) <= 0.0f) {
		// Q1�AQ2����P2��ɂ���
		return true;

	}
	else if (
		Q1inCld &&
		checkDot(pos1, pos2, q2) > 0.0f &&
		checkDot(pos2, pos1, q2) > 0.0f
		) {
		// Q1�AQ2�����~���ʂɂ���
		return true;

	}
	else if (
		calcRaySphere(ray, vec, pos1, r, t, q2) == true &&
		checkDot(pos2, pos1, q2) <= 0.0f
		) {
		// Q2�͋�P1��ɂ���
		return true;

	}
	else if (
		calcRaySphere(ray, vec, pos2, r, t, q2) == true &&
		checkDot(pos1, pos2, q2) <= 0.0f
		) {
		// Q2�͋�P2��ɂ���
		return true;

	}

	// Q2���~����ɂ��鎖���m��
	calcRayInfCilinder(ray, vec, pos1, pos2, r, t, q2);

	return true;
}