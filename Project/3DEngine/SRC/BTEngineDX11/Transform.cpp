#include "pch.h"
#include "Transform.h"

Transform::Transform()
{
	Position.x = 0;	Position.y = 0;	Position.z = 0;
	Rotation.x = 0;	Rotation.y = 0;	Rotation.z = 0;
	Scale.x = 1;	Scale.y = 1;	Scale.z = 1;
}

Transform::~Transform()
{
}

// 미리 구해놔도 되나?
Vector3 Transform::GetForward()
{
	float rotY = Rotation.y;

	Vector3 radianRot = Rotation / MathHelper::Rad;
	Matrix rot = Matrix::CreateFromYawPitchRoll(radianRot.y, radianRot.x, radianRot.z);

	Vector3 forward = XMVector3TransformNormal(Vector3(0, 0, 1), rot);

	return forward;
}

Matrix Transform::GetTransformMatrix()
{
	Matrix trans;
	trans.Translation(Position);

	Vector3 radianRot = Rotation / MathHelper::Rad;

	////Test zxy
	//Matrix rotZ = Matrix::CreateFromAxisAngle(Vector3::Forward, radianRot.z);
	//Matrix rotX = Matrix::CreateFromAxisAngle(Vector3::Right, radianRot.x);
	//Matrix rotY = Matrix::CreateFromAxisAngle(Vector3::Up, radianRot.y);
	//Matrix rot = rotZ * rotX * rotY;

	Matrix rot = Matrix::CreateFromYawPitchRoll(radianRot.y, radianRot.x, radianRot.z);

	Matrix scale = Matrix::CreateScale(Scale);

	return scale  * rot * trans;
}
