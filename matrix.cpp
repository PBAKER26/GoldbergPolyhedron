#include "matrix.h"

struct projectionStruct mProjection;
struct cameraStruct mCamera;
Matrix4f mPerspective;

Matrix4f matrixScale(Vector3f scale) {
	Matrix4f mScale;

	mScale.m[0][0] = scale.x; mScale.m[0][1] = 0.0f; mScale.m[0][2] = 0.0f; mScale.m[0][3] = 0.0f;
	mScale.m[1][0] = 0.0f; mScale.m[1][1] = scale.y; mScale.m[1][2] = 0.0f; mScale.m[1][3] = 0.0f;
	mScale.m[2][0] = 0.0f; mScale.m[2][1] = 0.0f; mScale.m[2][2] = scale.z; mScale.m[2][3] = 0.0f;
	mScale.m[3][0] = 0.0f; mScale.m[3][1] = 0.0f; mScale.m[3][2] = 0.0f; mScale.m[3][3] = 1.0f;

	return mScale;
}

Matrix4f matrixRotate(Vector3f rotate) {
	Matrix4f mRotateX, mRotateY, mRotateZ;
	float x = ToRadian(rotate.x);
	float y = ToRadian(rotate.y);
	float z = ToRadian(rotate.z);

	mRotateX.m[0][0] = 1.0f;		mRotateX.m[0][1] = 0.0f;		mRotateX.m[0][2] = 0.0f;		mRotateX.m[0][3] = 0.0f;
	mRotateX.m[1][0] = 0.0f;		mRotateX.m[1][1] = cosf(x);		mRotateX.m[1][2] = -sinf(x);	mRotateX.m[1][3] = 0.0f;
	mRotateX.m[2][0] = 0.0f;		mRotateX.m[2][1] = sinf(x);		mRotateX.m[2][2] = cosf(x);		mRotateX.m[2][3] = 0.0f;
	mRotateX.m[3][0] = 0.0f;		mRotateX.m[3][1] = 0.0f;		mRotateX.m[3][2] = 0.0f;		mRotateX.m[3][3] = 1.0f;

	mRotateY.m[0][0] = cosf(y);		mRotateY.m[0][1] = 0.0f;		mRotateY.m[0][2] = -sinf(y);	mRotateY.m[0][3] = 0.0f;
	mRotateY.m[1][0] = 0.0f;		mRotateY.m[1][1] = 1.0f;		mRotateY.m[1][2] = 0.0f;		mRotateY.m[1][3] = 0.0f;
	mRotateY.m[2][0] = sinf(y);		mRotateY.m[2][1] = 0.0f;		mRotateY.m[2][2] = cosf(y);		mRotateY.m[2][3] = 0.0f;
	mRotateY.m[3][0] = 0.0f;		mRotateY.m[3][1] = 0.0f;		mRotateY.m[3][2] = 0.0f;		mRotateY.m[3][3] = 1.0f;

	mRotateZ.m[0][0] = cosf(z);		mRotateZ.m[0][1] = -sinf(z);	mRotateZ.m[0][2] = 0.0f;		mRotateZ.m[0][3] = 0.0f;
	mRotateZ.m[1][0] = sinf(z);		mRotateZ.m[1][1] = cosf(z);		mRotateZ.m[1][2] = 0.0f;		mRotateZ.m[1][3] = 0.0f;
	mRotateZ.m[2][0] = 0.0f;		mRotateZ.m[2][1] = 0.0f;		mRotateZ.m[2][2] = 1.0f;		mRotateZ.m[2][3] = 0.0f;
	mRotateZ.m[3][0] = 0.0f;		mRotateZ.m[3][1] = 0.0f;		mRotateZ.m[3][2] = 0.0f;		mRotateZ.m[3][3] = 1.0f;

	//return mRotateZ * mRotateY * mRotateX;
	return mRotateZ * mRotateX * mRotateY;
}

Matrix4f matrixEulerRotate(float r, Vector3f vector) {
	Matrix4f mRotate;

	vector = vector.Normalize();
	float X = vector.x;
	float Y = vector.y;
	float Z = vector.z;

	r = ToRadian(r);
	float s = sin(r);
	float c = cos(r);
	float oc = 1.0 - c;

	mRotate.m[0][0] = oc*X*X + c;			mRotate.m[0][1] = oc*X*Y - s*Z;			mRotate.m[0][2] = oc*X*Z + s*Y;			mRotate.m[0][3] = 0.0f;
	mRotate.m[1][0] = oc*X*Y + s*Z;			mRotate.m[1][1] = oc*Y*Y + c;			mRotate.m[1][2] = oc*Y*Z - s*X;			mRotate.m[1][3] = 0.0f;
	mRotate.m[2][0] = oc*X*Z - s*Y;			mRotate.m[2][1] = oc*Y*Z + s*X;			mRotate.m[2][2] = oc*Z*Z + c;			mRotate.m[2][3] = 0.0f;
	mRotate.m[3][0] = 0.0f;					mRotate.m[3][1] = 0.0f;					mRotate.m[3][2] = 0.0f;					mRotate.m[3][3] = 1.0f;

	return mRotate;
}

Matrix4f matrixTrans(Vector3f trans, bool neg) {
	Matrix4f mTrans;

	mTrans.m[0][0] = 1.0f; mTrans.m[0][1] = 0.0f; mTrans.m[0][2] = 0.0f; mTrans.m[0][3] = trans.x;
	mTrans.m[1][0] = 0.0f; mTrans.m[1][1] = 1.0f; mTrans.m[1][2] = 0.0f; mTrans.m[1][3] = trans.y;
	mTrans.m[2][0] = 0.0f; mTrans.m[2][1] = 0.0f; mTrans.m[2][2] = 1.0f; mTrans.m[2][3] = trans.z;
	mTrans.m[3][0] = 0.0f; mTrans.m[3][1] = 0.0f; mTrans.m[3][2] = 0.0f; mTrans.m[3][3] = 1.0f;

	if (neg) { mTrans.m[0][3] = -trans.x; mTrans.m[1][3] = -trans.y; mTrans.m[2][3] = -trans.z; }

	return mTrans;
}

Matrix4f matrixPerspective() {
	const float ar = mProjection.Width / mProjection.Height;
	const float zNear = mProjection.zNear;
	const float zFar = mProjection.zFar;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(ToRadian(mProjection.FOV / 2.0f));

	Matrix4f mPers;
	mPers.m[0][0] = 1.0f / (tanHalfFOV*ar); mPers.m[0][1] = 0.0f;            mPers.m[0][2] = 0.0f;				  mPers.m[0][3] = 0.0;
	mPers.m[1][0] = 0.0f;                 mPers.m[1][1] = 1.0f / tanHalfFOV; mPers.m[1][2] = 0.0f;				  mPers.m[1][3] = 0.0;
	mPers.m[2][0] = 0.0f;                 mPers.m[2][1] = 0.0f;            mPers.m[2][2] = (-zNear - zFar) / zRange; mPers.m[2][3] = 2.0f * zFar*zNear / zRange;
	mPers.m[3][0] = 0.0f;                 mPers.m[3][1] = 0.0f;            mPers.m[3][2] = 1.0f;				  mPers.m[3][3] = 0.0;
	return mPers;
}

Matrix4f cameraTrans(Vector3f rotate) {
	Matrix4f mRotateX, mRotateY;
	float x = ToRadian(rotate.x);
	float y = ToRadian(rotate.y);

	mRotateX.m[0][0] = 1.0f; mRotateX.m[0][1] = 0.0f; mRotateX.m[0][2] = 0.0f; mRotateX.m[0][3] = 0.0f;
	mRotateX.m[1][0] = 0.0f; mRotateX.m[1][1] = cosf(x); mRotateX.m[1][2] = -sinf(x); mRotateX.m[1][3] = 0.0f;
	mRotateX.m[2][0] = 0.0f; mRotateX.m[2][1] = sinf(x); mRotateX.m[2][2] = cosf(x); mRotateX.m[2][3] = 0.0f;
	mRotateX.m[3][0] = 0.0f; mRotateX.m[3][1] = 0.0f; mRotateX.m[3][2] = 0.0f; mRotateX.m[3][3] = 1.0f;

	mRotateY.m[0][0] = cosf(y); mRotateY.m[0][1] = 0.0f; mRotateY.m[0][2] = -sinf(y); mRotateY.m[0][3] = 0.0f;
	mRotateY.m[1][0] = 0.0f; mRotateY.m[1][1] = 1.0f; mRotateY.m[1][2] = 0.0f; mRotateY.m[1][3] = 0.0f;
	mRotateY.m[2][0] = sinf(y); mRotateY.m[2][1] = 0.0f; mRotateY.m[2][2] = cosf(y); mRotateY.m[2][3] = 0.0f;
	mRotateY.m[3][0] = 0.0f; mRotateY.m[3][1] = 0.0f; mRotateY.m[3][2] = 0.0f; mRotateY.m[3][3] = 1.0f;

	return mRotateX * mRotateY;
}

void initPerspective(float FOV, float Width, float Height, float zNear, float zFar) {
	mProjection.FOV = FOV;
	mProjection.Width = Width;
	mProjection.Height = Height;
	mProjection.zNear = zNear;
	mProjection.zFar = zFar;

	mPerspective = matrixPerspective();
}

void cameraLook(Vector3f Pos, Vector3f Target, Vector3f Up) {
	mCamera.trans = matrixTrans(Pos, true);
	mCamera.rotate = cameraTrans(Target);
}

Matrix4f worldTransform(Vector3f scale, Vector3f rotate, Vector3f trans) {
	return matrixTrans(trans, false) * matrixRotate(rotate) * matrixScale(scale);
}

Matrix4f matrixTransform(Vector3f scale, Vector3f rotate, Vector3f trans) {
	return mPerspective * mCamera.rotate * mCamera.trans * worldTransform(scale, rotate, trans);
}

Matrix4f cameraTransform() {
	return mPerspective * mCamera.rotate;
}
