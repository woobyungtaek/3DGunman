#include "pch.h"
#include "RectTransform.h"

RectTransform::RectTransform()
{
	mRect = Vector4(0.f, 0.f, 0.f, 0.f);
	mNearHeight = 1.f;
}

RectTransform::~RectTransform()
{
}

Matrix RectTransform::GetTransformMatrix()
{

	Matrix rot = Matrix::CreateFromYawPitchRoll(Rotation.y, Rotation.x, Rotation.z);

	Vector3 size;
	Vector3 pos;

	float minX = mScreenSize.x * mRect.x;
	float maxX = mScreenSize.x * (1.f - mRect.y);
	size.x = maxX - minX;

	float minY = mScreenSize.y * mRect.z;
	float maxY = mScreenSize.y * (1.f - mRect.w);
	size.y = maxY - minY;

	pos = size / 2;
	pos.x += minX;
	pos.y += minY;

	pos.x -= mScreenSize.x / 2;
	pos.y = (mScreenSize.y / 2) - pos.y;

	float heightAspect = mFarHeight / mScreenSize.y;
	pos *= heightAspect;
	pos.z = 1;
	pos += Position;

	size *= heightAspect;

	size.x *= Scale.x;
	size.y *= Scale.y;

	Matrix scaleTM = Matrix::CreateScale(size);

	Matrix trans;
	trans.Translation(pos);

	return scaleTM * rot * trans;
}

RECT RectTransform::GetRectSize()
{
	RECT rect;
	rect.left = mRect.x * mScreenSize.x;
	rect.right = (1.f- mRect.y) * mScreenSize.x;
	rect.top = mRect.z * mScreenSize.y;
	rect.bottom = (1.f - mRect.w) * mScreenSize.y;

	return rect;
}

void RectTransform::SetRectByOriginal()
{
	mRect.x = 0;
	mRect.z = 0;

	mRect.y = mScreenSize.x / mImageSize.x;
	mRect.w = mScreenSize.y / mImageSize.y;;
}
