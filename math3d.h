#ifndef MATH3D_H
#define MATH3D_H

#ifdef WIN32
#define _USE_MATH_DEFINES 
#include <cmath>
#else
#include <math.h>
#endif

#define ToRadian(x) (float)(((x) * 3.14159265358979323846 / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / 3.14159265358979323846))

struct Vector2f{
public:
	float x, y;

	Vector2f() {}
	Vector2f(float _x, float _y) { x = _x; y = _y; }
	
	Vector2f operator+(const Vector2f& r) {
		Vector2f temp;
		temp.x = x + r.x;
		temp.y = y + r.y;
		return temp;
	}
	Vector2f& operator+=(const Vector2f& a) {
		x += a.x;
		y += a.y;
		return *this;
	}
};

class Vector3f {
public:
	float x, y, z;

	Vector3f() { x = 0.0, y = 0.0, z = 0.0; }
	Vector3f(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	Vector3f& operator+=(const Vector3f& a) {
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	Vector3f& operator-=(const Vector3f& a) {
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}
	Vector3f& operator*=(float a) {
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
	bool operator==(const Vector3f &a) {
		if (x == a.x && y == a.y && z == a.z) return true;
		else return false;
	}

	Vector3f Cross(Vector3f& v) {
		float _x = y * v.z - z * v.y;
		float _y = z * v.x - x * v.z;
		float _z = x * v.y - y * v.x;

		return Vector3f(_x, _y, _z);
	}
	float Vector3f::length() {
		return((float)sqrt(x*x + y*y + z*z));
	}
	float Vector3f::innerProduct(Vector3f &v) {
		return (x * v.x + y * v.y + z * v.z);
	}
	void Vector3f::set(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void Vector3f::copy(const Vector3f &v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vector3f& Normalize() {
		float Length = sqrtf(x * x + y * y + z * z);

		x /= Length; y /= Length; z /= Length;

		return *this;
	}
	Vector3f Vector3f::toPoint() {
		Vector3f res;
		res.x = sin(y*0.0174532925)*cos(x*0.0174532925);
		res.y = sin(x*0.0174532925);
		res.z = cos(y*0.0174532925)*cos(x*0.0174532925);
		return res;
	}
	Vector3f Vector3f::toPointUp() {
		Vector3f res;
		res.x = sin(y*0.0174532925)*-sin(x*0.0174532925);
		res.y = cos(x*0.0174532925);
		res.z = cos(y*0.0174532925)*-sin(x*0.0174532925);
		return res;
	}

	Vector3f operator+(Vector3f& r) {
		Vector3f temp;
		temp.x = x + r.x;
		temp.y = y + r.y;
		temp.z = z + r.z;
		return temp;
	}
	Vector3f operator-(Vector3f& r) {
		Vector3f temp;
		temp.x = x - r.x;
		temp.y = y - r.y;
		temp.z = z - r.z;
		return temp;
	}
	Vector3f operator*(float a) {
		Vector3f temp;
		temp.x = x * a;
		temp.y = y * a;
		temp.z = z * a;
		return temp;
	}
	Vector3f operator/(float a) {
		Vector3f temp;
		temp.x = x / a;
		temp.y = y / a;
		temp.z = z / a;
		return temp;
	}
};

struct Vertex {
	Vector3f m_pos;
	Vector2f m_tex;
	Vector3f m_normal;

	Vertex() {}
	Vertex(Vector3f pos, Vector2f tex) {
		m_pos = pos;
		m_tex = tex;
		m_normal = Vector3f(0.0f, 0.0f, 0.0f);
	}
	Vertex(Vector3f pos, Vector2f tex, Vector3f norm) {
		m_pos = pos;
		m_tex = tex;
		m_normal = norm;
	}
};

class Plane{			//planes are used for frustums
public:
	Vector3f normal, point;
	float d;

	Plane::Plane(Vector3f &v1, Vector3f &v2, Vector3f &v3);
	Plane::Plane(void);
	Plane::~Plane();

	void set3Points(Vector3f &v1, Vector3f &v2, Vector3f &v3);
	void setNormalAndPoint(Vector3f &normal, Vector3f &point);
	void setCoefficients(float a, float b, float c, float d);
	float distance(Vector3f &p);
	void print();
};

Vector3f calcNormal(Vector3f first, Vector3f second, Vector3f third);
void roundNormals(Vertex * v, unsigned int vertCount);

void calcNormals(Vertex * v, unsigned int vertCount);
#endif
