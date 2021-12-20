#include "pch.h"
#include "TextRenderer.h"

TextRenderer::TextRenderer()
{
	mColor = Vector4(1, 1, 1, 1);
	mSize = 1;
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::Render(Camera* pCamera)
{
	Vector3 pos = gameObject->mTransform->Position;
	float rot = gameObject->mTransform->Rotation.z / MathHelper::Rad;
	DXTKFont* mFont = BTGameEngine::GetInstance()->GetRender()->GetFont();

	mFont->DrawTextColor(pos.x, pos.y, rot, mSize, mColor, mText);
}


void TextRenderer::SetText(std::wstring text, ...)
{
	va_list vl;
	va_start(vl, text);
	_vstprintf(mText, 1024, text.c_str(), vl);
	va_end(vl);

	SetSortOrder(100);
}

void TextRenderer::SetColor(Vector4 color)
{
	mColor = color;
}

void TextRenderer::SetSize(float size)
{
	mSize = size;
}
