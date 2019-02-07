// Made by Danny Kavananagh
#include "CSearchBreathFirst.h"
#include "SearchFactory.h"
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>

/*Boolen to see if a node is already on the open list*/
bool gBFNorthOnList = false;
bool gBFEastOnList = false;
bool gBFSouthOnList = false;
bool gBFWestOnList = false;


/* This memeber function is used to generate the new rules of the search (NESW) */
int CSearchBreathFirst::mBlockChecker(TerrainMap& terrain, unique_ptr <SNode> &pCurrentNode, unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth,
	                                  unique_ptr <SNode> &pWest, unique_ptr<SNode> &pGoal, NodeList& path, int maxX, int maxY, vector <vector <IModel*>> Tiles)
{
	//North (Sets the new node and sets the parent)
	pNorth->x = pCurrentNode->x;
	pNorth->y = pCurrentNode->y;
	pNorth->y++;
	pNorth->parent = pCurrentNode.get();
	//East (Sets the new node and sets the parent)
	pEast->x = pCurrentNode->x;
	pEast->y = pCurrentNode->y;
	pEast->x++;
	pEast->parent = pCurrentNode.get();
	//South (Sets the new node and sets the parent)
	pSouth->x = pCurrentNode->x;
	pSouth->y = pCurrentNode->y;
	pSouth->y--;
	pSouth->parent = pCurrentNode.get();
	//West (Sets the new node and sets the parent)
	pWest->x = pCurrentNode->x;
	pWest->y = pCurrentNode->y;
	pWest->x--;
	pWest->parent = pCurrentNode.get();
	return true;
};

/* This memeber function is used to check to see if the new nodes generated are already on the open list */
bool CSearchBreathFirst::mOpenListCHecker(unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest)
{
	for (auto p = openList.begin(); p != openList.end(); p++) // This ittorator will run through the openlist
	{
		if ((*p) != 0) // checks to see if the ittorater isn't empty or finished
		{
			/* The boolens are used to make it skip the rest of the code (This makes it so it won't be checked on the closed list and won't be pushed onto openlist) */
			//North ((*p) is the current part of the ittorater)
			if (pNorth->x == (*p)->x && pNorth->y == (*p)->y) // checks to see if the new state is already on the openlist
			{
				gBFNorthOnList = true; 
			}
			if (pEast->x == (*p)->x && pEast->y == (*p)->y) // checks to see if the new state is already on the openlist
			{
				gBFEastOnList = true;
			}
			if (pSouth->x == (*p)->x && pSouth->y == (*p)->y) // checks to see if the new state is already on the openlist
			{
				gBFSouthOnList = true;
			}
			if (pWest->x == (*p)->x && pWest->y == (*p)->y) // checks to see if the new state is already on the openlist
			{
				gBFWestOnList == true;
			}
			/*******************************************************************************************/
		}
		
	}
	return true;

}
/*This works like the memeber function above and will check to see if already on the closedList*/
bool CSearchBreathFirst::mClosedListCHecker(unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest)
{
	/* The boolens are used to make it skip the rest of the code (This makes it so it won't be checked on the closed list and won't be pushed onto openlist) */
	for (auto p = closedList.begin(); p != closedList.end(); p++) // This ittorator will run through the closedlist
	{
		if (gBFNorthOnList != true) // check to see if its already on the openList
		{
			if (pNorth->x == (*p)->x && pNorth->y == (*p)->y) // checks to see if the new state is already on the closedlist
			{
				gBFNorthOnList = true;
			}
		}
		if (gBFEastOnList != true) // // check to see if its already on the openList
		{
			if (pEast->x == (*p)->x && pEast->y == (*p)->y) // checks to see if the new state is already on the closedlist
			{
				gBFEastOnList = true;
			}
		}
		if (gBFSouthOnList != true) // // check to see if its already on the openList
		{
			if (pSouth->x == (*p)->x && pSouth->y == (*p)->y) // checks to see if the new state is already on the closedlist
			{
				gBFSouthOnList = true;
			}
		}
		if (gBFWestOnList != true) // // check to see if its already on the openList
		{
			if (pWest->x == (*p)->x && pWest->y == (*p)->y) // checks to see if the new state is already on the closedlist
			{
				gBFWestOnList = true;
			}
		}
	}
	return true;
}
int CSearchBreathFirst::mPushAndPop(TerrainMap& terrain, unique_ptr <SNode> &pCurrentNode,unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, 
	                                 unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest, int maxX, int maxY, I3DEngine* engine, vector <vector <IModel*>> Tiles, int &sortCount)
{
	//North
	if (gBFNorthOnList != true) // Checks to see if its not on the open or closed list
	{
		if (pNorth->x >= 0 && pNorth->x != maxX && pNorth->y >= 0 && pNorth->y != maxY) // check to see if the new node isn't greater or less then the mapSize
		{
			if (terrain[pNorth->x][pNorth->y] != 0) // check to see if the new node is a wall
			{
				openList.push_back(move(pNorth)); // pushes the new state onto the openlist
			}
		}
	}
	//East
	if (gBFEastOnList != true) // Checks to see if its not on the open or closed list
	{
		if (pEast->x >= 0 && pEast->x != maxX && pEast->y >= 0 && pEast->y != maxY) // check to see if the new node isn't greater or less then the mapSize
		{
			if (terrain[pEast->x][pEast->y] != 0) // check to see if the new node is a wall
			{
				openList.push_back(move(pEast)); // pushes the new state onto the openlist
			}
		}
	}
	//South
	if (gBFSouthOnList != true) // Checks to see if its not on the open or closed list
	{
		if (pSouth->x >= 0 && pSouth->x != maxX && pSouth->y >= 0 && pSouth->y != maxY) // check to see if the new node isn't greater or less then the mapSize
		{
			if (terrain[pSouth->x][pSouth->y] != 0) // check to see if the new node is a wall
			{
				openList.push_back(move(pSouth)); // pushes the new state onto the openlist
			}
		}
	}
	// West
	if (gBFWestOnList != true) // Checks to see if its not on the open or closed list
	{
		if (pWest->x >= 0 && pWest->x != maxX && pWest->y >= 0 && pWest->y != maxY) // check to see if the new node isn't greater or less then the mapSize
		{
			if (terrain[pWest->x][pWest->y] != 0) // check to see if the new node is a wall
			{
				openList.push_back(move(pWest)); // pushes the new state onto the openlist
			}
		}
	}
	closedList.push_front(move(pCurrentNode)); // pushes the current state onto the closedlist
	openList.pop_front(); // removes the first element of the openlist (will be an empty element)
	/* This will check to see if there is no more tiles to check and will return false if there isn't */
	if (!openList.empty())
	{
		pCurrentNode = move(openList.front());
	}
	else
	{
		/*Reset the bools to false*/
		gBFNorthOnList = false;
		gBFEastOnList = false;
		gBFSouthOnList = false;
		gBFWestOnList = false;
		return false;
	}
	/*****************************************************************************/
	/*Reset the bools to false*/
	gBFNorthOnList = false;
	gBFEastOnList = false;
	gBFSouthOnList = false;
	gBFWestOnList = false;
	sortCount++; // This is used to keep track of sorts of itterations
	int countDown = 25; // countDown used in real time visual
	float frameTime = engine->Timer(); // This will get the timer function of the engine

	while (countDown > 0)
	{
		countDown -= frameTime; // will decrease the countdown by secs
		for (auto p = openList.begin(); p != openList.end(); p++) // standard itteration will run through the openlist
		{
			if ((*p) != 0)
			{
				Tiles[(*p)->x][(*p)->y]->SetSkin("grey.png"); // set skins
			}
		}
		for (auto o = closedList.begin(); o != closedList.end(); o++) // standard itteration will run through the closedList
		{
			Tiles[(*o)->x][(*o)->y]->SetSkin("black.png"); //set Skin
		}
		Tiles[pCurrentNode->x][pCurrentNode->y]->SetSkin("blue.png"); // sets skin
	}

	engine->DrawScene(); // This will draw the scene for the real time rep
	return true;
}

/* This is used when the end goal has been found */
int CSearchBreathFirst::mPathPlotter(unique_ptr<SNode> &pGoal, unique_ptr <SNode> &pCurrentNode, NodeList& path)
{
	vector<SNode*> parents; // this will hold the node with parents
	parents.push_back(pCurrentNode->parent); // Pushes the currentNode(EndNode) onto the parentList
	bool parentFinder = false;
	/* While this bool is false it will push all the nodes with parents onto the parent list*/
	while (parentFinder == false)
	{
		if (parents.back()->parent == NULL)
		{
			break;
		}
		parents.push_back(parents.back()->parent); // pushes node parents onto the parent list
	}

	unique_ptr <SNode> tmpPrt; // temp prt to hold a Node that matches a closed list node

	/* This will compare each parent on parent list to each node on the closed list */
	for (auto & outterIt : parents)
	{
		for (auto & innerIt : closedList)
		{
			/* If the parent matches the on one on the closed list it will set the tmpPrt to it and will push tmpPrt on the path */
			if ((outterIt)->x == (innerIt)->x && (outterIt)->y == (innerIt)->y)
			{
				tmpPrt.reset(new SNode);
				tmpPrt->x = (outterIt)->x;
				tmpPrt->y = (outterIt)->y;
				path.push_back(move(tmpPrt));
			}
		}
	}
	openList.clear(); // clears the openList
	closedList.clear(); // clears the closedList
	path.push_front(move(pGoal)); // pushes the Goal onto the path
	return true;
}

/* This is the main function memeber of the class as all the member function run through this */
bool CSearchBreathFirst::FindPath(TerrainMap& terrain, unique_ptr<SNode> &pStart, unique_ptr<SNode> &pGoal, NodeList& path, int maxX, int maxY, vector <vector <IModel*>> Tiles, I3DEngine* engine)
{
	std::ofstream outputFile; // used to output to txt file 
	openList.push_back(move(pStart)); // pushes the start on the open list
	int itterationsCount = 0; // this will keep count of itterations
	int mapMaxX = maxX; // sets the maxMaxX to maxX
	int mapMaxY = maxY; // sets the maxMaxY to maxY
	/* This sets up new points which are used for NESW */
	unique_ptr <SNode> pNorth(new SNode);
	unique_ptr <SNode> pEast(new SNode);
	unique_ptr <SNode> pSouth(new SNode);
	unique_ptr <SNode> pWest(new SNode);
	unique_ptr <SNode> pCurrentNode(new SNode);
	/***********************************************/
	bool pathWayFound = false; // bool to turn true when path is found
	pCurrentNode = move(openList.front()); // Makes currentNode equal to start
	while (!openList.empty()) // Runs while the openList isn't empty
	{
		if (pCurrentNode->x == pGoal->x && pCurrentNode->y == pGoal->y) // If this is true it means the path is found
		{
			pathWayFound = true;
		}
		if (pathWayFound == true)
		{
			outputFile.open("outputFileBreathFirst.txt"); // Opens the output file
			outputFile << "X _ Y" << endl; // reads into the output file
			CSearchBreathFirst::mPathPlotter(pGoal, pCurrentNode, path); // This will return the path
			for (auto p = path.begin(); p != path.end(); p++)
			{
				outputFile << (*p)->x << "_" << (*p)->y << endl; // This will output the current point the itterator is at
			}
			outputFile << "Number of Itterations: " << itterationsCount << endl; // output the number of Itterations
			outputFile.close(); // closes the file
			return true;
		}
		CSearchBreathFirst::mBlockChecker(terrain, pCurrentNode, pNorth, pEast, pSouth, pWest, pGoal, path, mapMaxX, mapMaxY, Tiles); // This will generate the new rules (NESW)
		CSearchBreathFirst::mOpenListCHecker(pNorth, pEast, pSouth, pWest); // This will check to see if NESW are on the closedList
		CSearchBreathFirst::mClosedListCHecker(pNorth, pEast, pSouth, pWest); // This will check to see if NESW are on the openList
		CSearchBreathFirst::mPushAndPop(terrain, pCurrentNode, pNorth, pEast, pSouth, pWest, mapMaxX, mapMaxY, engine, Tiles, itterationsCount); // This will push NESW onto the open list aswell as pushing current on the closedList and poping the openList
		/* Resets the new Nodes */
		pNorth.reset(new SNode);
		pEast.reset(new SNode);
		pSouth.reset(new SNode);
		pWest.reset(new SNode);
	}
	return false;
}