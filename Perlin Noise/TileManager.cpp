#include "BackGround/TileManager.h"
using namespace divil;
void TileManager::PlaceNew(TileSpot* aPrev, TileSpot* aNew)
{
	aNew->mPrev = aPrev;
	aNew->mNext = aPrev->mNext;
	aPrev->mNext = aNew;
}
void TileManager::FindNewLocation(TileSpot* aSpot)
{
	//if it is bigger then 1 place it in the back 
	if (aSpot->mBegin > 1)
		aSpot->mBegin = -1;

	TileSpot* tempData = mTileList;
	while (tempData)
	{
		if (!tempData->mNext)
		{
			PlaceNew(tempData, aSpot);
		}
		//if it fits in here place it in the back
		else if (aSpot->mBegin > tempData->mBegin && aSpot->mBegin < tempData->mNext->mBegin)
		{
			PlaceNew(tempData, aSpot);
		}
		//two places have the same location so push one a little bit back and try again
		else if (aSpot->mBegin == tempData->mBegin)
		{
			aSpot->mBegin += 0.0001;
			FindNewLocation(aSpot);
		}

		tempData = tempData->mNext;
	}
}
void TileManager::setDefault(TileData aData)
{
	mDefault = aData;
}

TileData TileManager::GetTileData(float aPoint)
{
	TileSpot* tempSpot = mTileList;
	if (aPoint > 1)
		return mDefault;
	while (tempSpot)
	{
		if (aPoint >= tempSpot->mBegin)
		{
			if (!tempSpot->mNext)
			{
				return tempSpot->mData;
			}
			else if ((aPoint < tempSpot->mNext->mBegin))
			{
				return tempSpot->mData;
			}
		}
		tempSpot = tempSpot->mNext;
	}
	return mDefault;
}

int TileManager::AddNewTile(TileData aData, float aBegin)
{

	//check if name is overlapping
	TileSpot* tempData = mTileList;
	while (tempData)
	{
		if (tempData->mData.mName == aData.mName)
		{
			return 1;
		}
		tempData = tempData->mNext;
	}

	TileSpot* newSpot = new TileSpot();
	newSpot->mBegin = glm::min(aBegin, 1.f);
	newSpot->mData = aData;
	//if their is no Tile yet mat this one the first one
	if (mTileList == nullptr)
	{
		newSpot->mPrev = nullptr;
		newSpot->mNext = nullptr;
		mTileList = newSpot;
	}
	else
	{
		FindNewLocation(newSpot);
	}
	
	return 0;
}

bool TileManager::RemoveSpot(TileSpot* aSpot)
{
	TileSpot* tempSpot = mTileList;
	while (tempSpot)
	{
		if (tempSpot->mData.mName == aSpot->mData.mName)
		{
			tempSpot->mPrev->mNext = tempSpot->mNext;
			tempSpot->mNext->mPrev = tempSpot->mPrev;
			delete tempSpot;
			return true;
		}
		tempSpot = tempSpot->mNext;
	}
	return false;
}

void TileManager::ReplaceSPot(TileSpot* aSpot, float aNewBegin)
{
	TileSpot* tempSpot = mTileList;
	//get the spot out of the array
	bool found = false;
	while (tempSpot)
	{
		if (tempSpot->mData.mName == aSpot->mData.mName)
		{
			tempSpot->mPrev->mNext = tempSpot->mNext;
			tempSpot->mNext->mPrev = tempSpot->mPrev;
			found = true;
		}
		tempSpot = tempSpot->mNext;
	}
	if (found)
	{
		aSpot->mBegin = aNewBegin;
		FindNewLocation(aSpot);
	}

	return;
}