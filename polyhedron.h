#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "matrix.h"
#include <vector>
#include <set>
#include <iostream>

const double tri_height = sqrt(.75);	//height of equilateral triangle, assuming side length of 1
static const float golden_ratio = (1.0 + sqrt(5.0)) / 2.0;
//static const Matrix4f tilt = matrixRotate(Vector3f(0.0f, 0.0f, 0.0f));
static const Matrix4f tilt = matrixRotate(Vector3f(0.0f, 0.0f, 31.7175f));

static const Vector2f textureGrid[100] = {
	Vector2f(0.0, 0.9), Vector2f(0.1, 0.9), Vector2f(0.2, 0.9), Vector2f(0.3, 0.9), Vector2f(0.4, 0.9), Vector2f(0.5, 0.9), Vector2f(0.6, 0.9), Vector2f(0.7, 0.9), Vector2f(0.8, 0.9), Vector2f(0.9, 0.9),
	Vector2f(0.0, 0.8), Vector2f(0.1, 0.8), Vector2f(0.2, 0.8), Vector2f(0.3, 0.8), Vector2f(0.4, 0.8), Vector2f(0.5, 0.8), Vector2f(0.6, 0.8), Vector2f(0.7, 0.8), Vector2f(0.8, 0.8), Vector2f(0.9, 0.8),
	Vector2f(0.0, 0.7), Vector2f(0.1, 0.7), Vector2f(0.2, 0.7), Vector2f(0.3, 0.7), Vector2f(0.4, 0.7), Vector2f(0.5, 0.7), Vector2f(0.6, 0.7), Vector2f(0.7, 0.7), Vector2f(0.8, 0.7), Vector2f(0.9, 0.7),
	Vector2f(0.0, 0.6), Vector2f(0.1, 0.6), Vector2f(0.2, 0.6), Vector2f(0.3, 0.6), Vector2f(0.4, 0.6), Vector2f(0.5, 0.6), Vector2f(0.6, 0.6), Vector2f(0.7, 0.6), Vector2f(0.8, 0.6), Vector2f(0.9, 0.6),
	Vector2f(0.0, 0.5), Vector2f(0.1, 0.5), Vector2f(0.2, 0.5), Vector2f(0.3, 0.5), Vector2f(0.4, 0.5), Vector2f(0.5, 0.5), Vector2f(0.6, 0.5), Vector2f(0.7, 0.5), Vector2f(0.8, 0.5), Vector2f(0.9, 0.5),
	Vector2f(0.0, 0.4), Vector2f(0.1, 0.4), Vector2f(0.2, 0.4), Vector2f(0.3, 0.4), Vector2f(0.4, 0.4), Vector2f(0.5, 0.4), Vector2f(0.6, 0.4), Vector2f(0.7, 0.4), Vector2f(0.8, 0.4), Vector2f(0.9, 0.4),
	Vector2f(0.0, 0.3), Vector2f(0.1, 0.3), Vector2f(0.2, 0.3), Vector2f(0.3, 0.3), Vector2f(0.4, 0.3), Vector2f(0.5, 0.3), Vector2f(0.6, 0.3), Vector2f(0.7, 0.3), Vector2f(0.8, 0.3), Vector2f(0.9, 0.3),
	Vector2f(0.0, 0.2), Vector2f(0.1, 0.2), Vector2f(0.2, 0.2), Vector2f(0.3, 0.2), Vector2f(0.4, 0.2), Vector2f(0.5, 0.2), Vector2f(0.6, 0.2), Vector2f(0.7, 0.2), Vector2f(0.8, 0.2), Vector2f(0.9, 0.2),
	Vector2f(0.0, 0.1), Vector2f(0.1, 0.1), Vector2f(0.2, 0.1), Vector2f(0.3, 0.1), Vector2f(0.4, 0.1), Vector2f(0.5, 0.1), Vector2f(0.6, 0.1), Vector2f(0.7, 0.1), Vector2f(0.8, 0.1), Vector2f(0.9, 0.1),
	Vector2f(0.0, 0.0), Vector2f(0.1, 0.0), Vector2f(0.2, 0.0), Vector2f(0.3, 0.0), Vector2f(0.4, 0.0), Vector2f(0.5, 0.0), Vector2f(0.6, 0.0), Vector2f(0.7, 0.0), Vector2f(0.8, 0.0), Vector2f(0.9, 0.0)
};

class Point : public Vector3f {
public:
	int ID;
	int neighbours;
	float distFromOrigin;
	bool inFocus;									//true if the spot on the map is facing the player
	std::vector<Point *> neighbour;					//I want this to be in clockwise order
	Point() { ID = -1; x = 0.0; y = 0.0; z = 0.0;};
	Point(float _x, float _y, float _z) : Vector3f(_x, _y, _z) { ID = -1; neighbours = 6; };
	Point(Vector3f vect) {
		ID = -1;
		x = vect.x;
		y = vect.y;
		z = vect.z;
		neighbours = 6;						//number of connected points
	};
	
	bool operator==(const Point &other) const {
		if (distanceFrom((Point)other) < 0.00000960) return true;
		else return false;
	};
	bool operator!=(const Point &other) const {
		if (*this == other) return false;
		else return true;
	};
	bool operator<(const Point &other) const;
	bool operator>(const Point &other) const;
	float distanceFrom(Point &other) const {		//is this correct?
		return hypot(hypot(x-other.x, y-other.y), z-other.z);
	};
	void setDistFromOrigin();
};

static const Point icoPoints[12] = {
	Point(tilt * Vector3f(-1, golden_ratio, 0)), Point(tilt * Vector3f(1, golden_ratio, 0)), Point(tilt * Vector3f(-1, -golden_ratio, 0)), Point(tilt * Vector3f(1, -golden_ratio, 0)),
	Point(tilt * Vector3f(0, -1, golden_ratio)), Point(tilt * Vector3f(0, 1, golden_ratio)), Point(tilt * Vector3f(0, -1, -golden_ratio)), Point(tilt * Vector3f(0, 1, -golden_ratio)),
	Point(tilt * Vector3f(golden_ratio, 0, -1)), Point(tilt * Vector3f(golden_ratio, 0, 1)), Point(tilt * Vector3f(-golden_ratio, 0, -1)), Point(tilt * Vector3f(-golden_ratio, 0, 1)) };
static Point * origin = new Point(0, 3, 0);

class Face {		//these will be sorted by ID. Any pointers to them made before they are sorted may be messed up
public:
	int ID;
	std::vector<Vector3f> corners;
	unsigned n;						//number of corners. 5 for pentagon, 6 for hexagon
	Vector3f center;
	Face(int num, int _ID) : n(num), ID(_ID) {};
};

class Triangle {
public:
	Vector3f center, a, b, c;
	int ID;
	Triangle() {};
	Triangle(Vector3f _a, Vector3f _b, Vector3f _c) : a(_a), b(_b), c(_c)  {
		center = (a + b + c) / 3;
	}
	Triangle(int num, Vector3f _a, Vector3f _b, Vector3f _c) : ID(num), a(_a), b(_b), c(_c)  {
		center = (a + b + c) / 3;
	}
};

class Polyhedron {
public:
	Triangle icoTriangles[20];
	std::vector<Point> points;
	std::vector<Face> faces;
	std::vector<Face> masks;			//slightly larger than faces, displayed if faces are on the horizon
	GLuint VBO;
	GLsizei	numV;
	float m, n;
	float radius;						//used by the renderer
	Polyhedron(int _m, int _n) : m(_m), n(_n) {
		for (int i = 0; i < 12; i++) points.push_back(icoPoints[i]);

		icoTriangles[0]  = Triangle(points[0],  points[11], points[5]);
		icoTriangles[1]  = Triangle(points[0],  points[5],  points[1]);
		icoTriangles[2]  = Triangle(points[0],  points[1],  points[7]);
		icoTriangles[3]  = Triangle(points[0],  points[7],  points[10]);
		icoTriangles[4]  = Triangle(points[0],  points[10], points[11]);
		icoTriangles[5]  = Triangle(points[1],  points[5],  points[9]);
		icoTriangles[6]  = Triangle(points[5],  points[11], points[4]);
		icoTriangles[7]  = Triangle(points[11], points[10], points[2]);
		icoTriangles[8]  = Triangle(points[10], points[7],  points[6]);
		icoTriangles[9]  = Triangle(points[7],  points[1],  points[8]);
		icoTriangles[10] = Triangle(points[3],  points[9],  points[4]);
		icoTriangles[11] = Triangle(points[3],  points[4],  points[2]);
		icoTriangles[12] = Triangle(points[3],  points[2],  points[6]);
		icoTriangles[13] = Triangle(points[3],  points[6],  points[8]);
		icoTriangles[14] = Triangle(points[3],  points[8],  points[9]);
		icoTriangles[15] = Triangle(points[4],  points[9],  points[5]);
		icoTriangles[16] = Triangle(points[2],  points[4],  points[11]);
		icoTriangles[17] = Triangle(points[6],  points[2],  points[10]);
		icoTriangles[18] = Triangle(points[8],  points[6],  points[7]);
		icoTriangles[19] = Triangle(points[9],  points[8],  points[1]);

		initPolyhedron(m, n);
	};
	//void initVBO();

private:
	void initPolyhedron(int _m, int _n);
};

#endif
