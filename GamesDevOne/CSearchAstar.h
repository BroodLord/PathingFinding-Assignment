#pragma once
// Made by Danny Kavananagh
#include "Search.h"
#include "SearchFactory.h" // Factory declarations


class CSearchAstar : public ISearch
{
public:
	int mBlockChecker(TerrainMap& terrain, unique_ptr <SNode> &pCurrentNode, unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, // returns the new nodes NESW
		unique_ptr <SNode> &pWest, unique_ptr<SNode> &pGoal, NodeList& path, int maxX, int maxY, vector <vector <IModel*>> Tiles);
	int mPushAndPop(TerrainMap& terrain, unique_ptr <SNode> &pCurrentNode, unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest, int maxX, int maxY, I3DEngine* engine, vector <vector <IModel*>> Tiles, int &sortCount);
	// Pushes and pops values onto and off the open and closed List
	bool mOpenListCHecker(unique_ptr <SNode> &pCurrentNode, unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest, unique_ptr<SNode> &pGoal); // checks the openList
	bool mClosedListCHecker(unique_ptr <SNode> &pCurrentNode, unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest, unique_ptr<SNode> &pGoal); // checks the closedList
	int mPathPlotter(unique_ptr<SNode> &pGoal, unique_ptr <SNode> &pCurrentNode, NodeList& path); // returns a path
	bool FindPath(TerrainMap& terrain, unique_ptr<SNode> &pStart, unique_ptr<SNode> &pGoal, NodeList& path, int maxX, int maxY, vector <vector <IModel*>> Tiles, I3DEngine* engine); // returns the path to the main engine
	int Heuristic(unique_ptr <SNode> &pCurrentNode,unique_ptr<SNode> &pGoal); // calculates the Manhatten distance

private:
	NodeList openList; // used to sort values which haven't been checked
	NodeList closedList; // used to sort values which have been checked
};
