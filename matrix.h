#ifndef MATRIX_H
#define MATRIX_H

#include "math3d.h"

class Matrix4f {
public:
	float m[4][4];

	Matrix4f() {}
	inline void InitIdentity() {
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}
	inline Matrix4f operator*(const Matrix4f& Right) const {
		Matrix4f Ret;

		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++) {
				Ret.m[i][j] = m[i][0] * Right.m[0][j] +
					m[i][1] * Right.m[1][j] +
					m[i][2] * Right.m[2][j] +
					m[i][3] * Right.m[3][j];
			}
		}
		return Ret;
	}
	inline Vector3f operator*(const Vector3f& Right) const {
		Vector3f Ret;
		
		Ret.x = m[0][0] * Right.x + m[0][1] * Right.y + m[0][2] * Right.z + m[0][3];
		Ret.y = m[1][0] * Right.x + m[1][1] * Right.y + m[1][2] * Right.z + m[1][3];
		Ret.z = m[2][0] * Right.x + m[2][1] * Right.y + m[2][2] * Right.z + m[2][3];
		return Ret;
	}
};

struct projectionStruct { float FOV, Width, Height, zNear, zFar; };
extern struct projectionStruct mProjection;
struct cameraStruct { Matrix4f trans, rotate; };
extern struct cameraStruct mCamera;
extern Matrix4f mPerspective;

Matrix4f matrixScale(Vector3f scale);
Matrix4f matrixRotate(Vector3f rotate);
Matrix4f matrixEulerRotate(float r, Vector3f vector);
Matrix4f matrixTrans(Vector3f trans, bool neg);
Matrix4f matrixPerspective();
Matrix4f cameraTrans(Vector3f rotate);
void initPerspective(float FOV, float Width, float Height, float zNear, float zFar);
void cameraLook(Vector3f Pos, Vector3f Target, Vector3f Up);
Matrix4f worldTransform(Vector3f scale, Vector3f rotate, Vector3f trans);
Matrix4f matrixTransform(Vector3f scale, Vector3f rotate, Vector3f trans);
Matrix4f cameraTransform();

#endif
