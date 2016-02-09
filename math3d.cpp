#include "math3d.h"

Vector3f calcNormal(Vector3f first, Vector3f second, Vector3f third) {
	Vector3f U, V, Normal;

	U = second - first;
	V = third - first;

	Normal.x = U.y * V.z - U.z * V.y;
	Normal.y = U.z * V.x - U.x * V.z;
	Normal.z = U.x * V.y - U.y * V.x;
	Normal.Normalize();

	return Normal;
}

void roundNormals(Vertex * v, unsigned int vertCount) {
	for (int i = 0; i < vertCount; i++) {
		v[i].m_normal = v[i].m_pos;
	}
}

void calcNormals(Vertex * v, unsigned int vertCount) {
	for (int i = 0; i < vertCount; i += 3) {
		v[i].m_normal = v[i + 1].m_normal = v[i + 2].m_normal = calcNormal(v[i].m_pos, v[i + 1].m_pos, v[i + 2].m_pos);
	}
}

Plane::Plane(Vector3f &v1, Vector3f &v2, Vector3f &v3) {
	set3Points(v1, v2, v3);
}
Plane::Plane() {}
Plane::~Plane() {}
void Plane::set3Points(Vector3f &v1, Vector3f &v2, Vector3f &v3) {
	Vector3f aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = aux2.Cross(aux1);

	normal.Normalize();
	point.copy(v2);
	d = -(normal.innerProduct(point));
}
void Plane::setNormalAndPoint(Vector3f &normal, Vector3f &point) {
	this->normal.copy(normal);
	this->normal.Normalize();
	d = -(this->normal.innerProduct(point));
}
void Plane::setCoefficients(float a, float b, float c, float d) {		//this is probably correct
	normal.set(a, b, c); // set the normal vector
	float l = normal.length(); //compute the length of the vector
	normal.set(a / l, b / l, c / l); // normalize the vector
	this->d = d / l; // and divide d by th length as well
}
float Plane::distance(Vector3f &p) {
	return (d + normal.innerProduct(p));
}
