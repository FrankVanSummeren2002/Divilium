#include "BackGround/Tile.h"
#include "rendering/renderList.h"
using namespace divil;
Tile::Tile(float aSpeed, glm::vec2 aPos, glm::vec2 aSize, glm::vec3 aColor)
{
	mSpeed = aSpeed;
	mPos = aPos;
	mSize = aSize;
	mColor = aColor;
}

Tile::Tile(float aSpeed, glm::vec2 aPos, glm::vec2 aSize, const char* aTexture)
{
	mSpeed = aSpeed;
	mPos = aPos;
	mSize = aSize;
	mTexture = aTexture;
}

void Tile::Draw(RenderList* )
{
//	aRenderList->DrawSprite(mSize.x, mSize.y,
//		glm::vec2(mPos.x, mPos.y), 0, mColor,SpriteLayers::Background);
}
