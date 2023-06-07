#include "BackGround/TileMapManager.h"
#include "Entities/EntitySystem.h"
#include "BackGround/Tile.h"
#include "PerlinNoise/PerlinNoise.h"
#include "BackGround/TileManager.h"
#include "glm/glm.hpp"
#include "Pathfinding/PathManager.h"
#include "divilium.h"
using namespace divil;
TileMapManager* TileMapManager::mInstance = nullptr;
TileMapManager::TileMapManager()
{
	mTileManager = new TileManager();
	mPerlinNoise = new PerlinNoise();
}
TileMapManager* TileMapManager::GetInstance()
{
	if (!mInstance)
		mInstance = new TileMapManager();
	return mInstance;
}
void TileMapManager::GenerateMap()
{
	RemoveMap();
	mPerlinNoise->Randomize();
	glm::vec2 min;
	glm::vec2 max;
	
	divil::GetWorldMinMax(min, max);
	//make sure it has a bit more then required
	mMapX = (int)((max.x - min.x) / mTileSizeX);
	int MapY = (int)((max.y - min.y) / mTileSizeY);

	//amplitude should be 1 since the return value should be in that range;
	for (int y = 0; y < MapY + 1; y++)
	{
		for (int x = 0; x < mMapX; x++)
		{
			float value = (float)mPerlinNoise->OctavePerlin((double)x, (double)y, (double)0,mOctaves, (double)mPersistance, (double)mFrequency, 1.0);
			TileData data = mTileManager->GetTileData(value);
			Tile* tile = new Tile(data.mSpeed,min + glm::vec2(x * mTileSizeX, y * mTileSizeY),glm::vec2(mTileSizeX,mTileSizeY),data.mCol);
			mMap.Add(tile);
		}
		
	}
}
void TileMapManager::RemoveMap()
{
	for (int i = 0; i < mMap.Count(); i++)
	{
		delete mMap[i];
	}
	mMap.Clear();
}

void divil::TileMapManager::SetTileSize(float aTile)
{
	mTileSizeX = aTile;
	mTileSizeY = aTile;
	PathManager::GetInstance()->SetTileSize(mTileSizeX);
}

glm::vec2 divil::TileMapManager::GetMapXY()
{
	glm::vec2 min;
	glm::vec2 max;

	divil::GetWorldMinMax(min, max);
	//make sure it has a bit more then required
	mMapX = (int)((max.x - min.x) / mTileSizeX);
	int MapY = (int)((max.y - min.y) / mTileSizeY);
	return glm::vec2(mMapX, MapY);
}

Tile* TileMapManager::GetTile(int aX, int aY)
{
	int index = ConvertToIndex(aX, aY);
	if (index >= mMap.Count())
		return nullptr;
	return mMap[index];
}

void TileMapManager::SetOcaves(int aValue) 
{ 
	mOctaves = glm::max(1, aValue); 
}

void TileMapManager::SetPersistance(float aValue)
{
	mPersistance = glm::max(0.f, aValue); 
}

void TileMapManager::SetFrequency(float aValue)
{
	mFrequency = glm::max(0.f, aValue);
}

void TileMapManager::DrawMap(RenderList* aRenderlist)
{
	for (int i = 0; i < mMap.Count(); i++)
	{
		mMap[i]->Draw(aRenderlist);
	}
}
