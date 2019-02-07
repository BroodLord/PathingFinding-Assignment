#pragma once
// Made by Danny Kavananagh
#include "SearchFactory.h"


class CSearchBreathFirst : public ISearch
{
public:
	int mBlockChecker(TerrainMap& terrain, unique_ptr <SNode> &pCurrentNode, unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, // return NESW values
		              unique_ptr <SNode> &pWest , unique_ptr<SNode> &pGoal, NodeList& path, int maxX, int maxY, vector <vector <IModel*>> Tiles);
	int mPathPlotter(unique_ptr<SNode> &pGoal, unique_ptr <SNode> &pCurrentNode ,NodeList& path); // returns the path
	bool mOpenListCHecker(unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest); // returns bools to see if NESW can be pushed onto openlist
	bool mClosedListCHecker(unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest); // returns bools to see if NESW can be pushed onto closed
	int mPushAndPop(TerrainMap& terrain, unique_ptr <SNode> &pCurrentNode,unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest,int maxX, int maxY, I3DEngine* engine, vector <vector <IModel*>> Tiles, int &sortCount);
	// Pushes and pop items onto the closedList and openList
	bool FindPath(TerrainMap& terrain, unique_ptr<SNode> &pStart, unique_ptr<SNode> &pGoal, NodeList& path, int maxX, int maxY, vector <vector <IModel*>> Tiles, I3DEngine* engine); // This will return the path to the main code
	

private:
	NodeList openList; // This is used to hold NESW rules that are vaild for the openList
	NodeList closedList; // This is used to hold used NESW rules
};