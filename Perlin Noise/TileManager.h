#pragma once
#include <vector>
#include "glm/glm.hpp"
namespace divil
{
	struct TileData
	{
		std::string mName;
		float mSpeed = 1;
		glm::vec3 mCol = glm::vec3(1, 1, 1);
		const char* mTexture = "None";
		TileData() {}
		TileData(std::string aName, float aSpeed, glm::vec3 aCol) { mName = aName; mSpeed = aSpeed; mCol = aCol; }
		TileData(std::string aName, float aSpeed, const char* aText) { mName = aName; mSpeed = aSpeed; mTexture = aText; }
	};
	struct TileSpot
	{
		float mBegin;
		TileSpot* mNext;
		TileSpot* mPrev;
		TileData mData;
	};

	class TileManager
	{
		TileData mDefault;
		TileSpot* mTileList;
		void PlaceNew(TileSpot* aPrev, TileSpot* aNew);
		void FindNewLocation(TileSpot* aSpot);

	public:
		//if their is no TileData found between 0 and 1 it will use this default
		void setDefault(TileData aData);
		TileData GetTileData(float aPoint);
		//placing a new type of tile in the array 
		// TileData name will need to be unqiue
		//if the tile cant be fit between 0 and 1 it will be set to -1 and placed at the end of the array
		//same goes with an existing tile in between the begin and end
		//returns 0 if it was succesfull
		int AddNewTile(TileData aData, float aBegin);
		const TileSpot* getTileList() { return mTileList; }
		bool RemoveSpot(TileSpot* aSpot);
		void ReplaceSPot(TileSpot* aSpot, float aNewBegin);
	};
}