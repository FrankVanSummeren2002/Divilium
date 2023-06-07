#pragma once

namespace divil
{
	class RenderList;
	class TileManager;
	class Tile;
	class PerlinNoise;
	
	class TileMapManager
	{

		// tile size
		float mTileSizeX = 16;
		float mTileSizeY = 16;
		//map size
		int mMapX = 10;

		//perlin noise values
		float mPersistance = 2;
		float mFrequency = 0.01;
		int mOctaves = 4;
		//

		TileManager* mTileManager;
		PerlinNoise* mPerlinNoise;
		divil::Array<Tile*> mMap;

		TileMapManager(TileMapManager& other) = delete;
		void operator=(const TileMapManager&) = delete;

		TileMapManager();
		int ConvertToIndex(int X, int Y) { return X + Y * mMapX; }
	public:
		static TileMapManager* GetInstance();

		void GenerateMap();

		void RemoveMap();


		//Setters
		void SetTileSize(float aTile);

		//setters for perlin noise
		//Set the amount of octaves to use for the perlin noise
		//values below 1 will be set to one
		void SetOcaves(int aValue);
		//set the persistance of perlin noise
		// will be set to 0 of value is lower then 0
		void SetPersistance(float aValue);
		//set the frequency used by persistance
		// will be set to 0 of value is lower then 0
		void SetFrequency(float aValue);


		//Getters
		void DrawMap(RenderList* aRenderlist);
		void GetTileSize(float& aTileX, float& aTileY) { aTileX = mTileSizeX; aTileY = mTileSizeY; };
		TileManager* GetTileManager() { return mTileManager; };
		PerlinNoise* GetPerlinNoise() { return mPerlinNoise; };
		glm::vec2 GetMapXY();

		//returns a nullptr if it could not find the Tile
		Tile* GetTile(int aX, int aY);
	protected:
		static TileMapManager* mInstance;
	};
}