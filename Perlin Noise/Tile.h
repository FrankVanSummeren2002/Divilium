#pragma once
namespace divil
{
	class RenderList;

	class Tile
	{
		float mSpeed;
		const char* mTexture;
		glm::vec3 mColor;
		glm::vec2 mSize;
		glm::vec2 mPos;
	public:
		Tile(float aSpeed, glm::vec2 aPos, glm::vec2 aSize, glm::vec3 aColor);
		Tile(float aSpeed, glm::vec2 aPos, glm::vec2 aSize, const char* aTexture);
		void Update(float) {};

		void Draw(RenderList* aRenderList);
		float GetSpeed() { return mSpeed; }
	};
}

