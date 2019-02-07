// Made by Danny Kavananagh
#include "SearchFactory.h" // Factory declarations
#include "CSearchAstar.h"
#include <deque>
#include <fstream>
#include <iterator>
#include <memory>
#include <algorithm>
#include <iostream>

/* Boolen that are used for the open and closed list checks */
bool gASNorthOnList = false;
bool gASEastOnList = false;
bool gASSouthOnList = false;
bool gASWestOnList = false;
bool gASNorthClosedList = false;
bool gASEastOnClosedList = false;
bool gASSouthOnClosedList = false;
bool gASWestOnClosedList = false;

/* This will calculate the manhatten distance */
int CSearchAstar::Heuristic(unique_ptr <SNode> &pCurrentNode, unique_ptr<SNode> &pGoal)
{
	int Heuristic = abs(pCurrentNode->x - pGoal->x) + abs(pCurrentNode->y - pGoal->y);
	return Heuristic;
}
/*This memeber function is used to generate the new rules of the search(NESW) */
int CSearchAstar::mBlockChecker(TerrainMap& terrain, unique_ptr <SNode> &pCurrentNode, unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth,
	unique_ptr <SNode> &pWest, unique_ptr<SNode> &pGoal, NodeList& path, int maxX, int maxY, vector <vector <IModel*>> Tiles)
{
	/* Each new node in Astar will have a parent, cost, Heuristic, score and totalScore, each node generates theses */

	//North (sets the new node up)
	pNorth->x = pCurrentNode->x;
	pNorth->y = pCurrentNode->y;
	pNorth->y++; 
	/* Chceck to see if its greater then the max size */
	if (pNorth->y == maxY)
	{
		pNorth->y = maxY - 1;
	}
	pNorth->parent = pCurrentNode.get();
	pNorth->cost = terrain[pNorth->x][pNorth->y];
	pNorth->Heuristic = CSearchAstar::Heuristic(pNorth, pGoal);
	pNorth->score = (pNorth->parent->score + pNorth->cost);
	pNorth->TotalCost = pNorth->score + pNorth->Heuristic;

	//East (Sets up new node)
	pEast->x = pCurrentNode->x;
	pEast->y = pCurrentNode->y;
	pEast->x++;
	/* Chceck to see if its greater then the max size */
	if (pEast->x == maxX)
	{
		pEast->x = maxX - 1;
	}
	pEast->parent = pCurrentNode.get();
	pEast->cost = terrain[pEast->x][pEast->y];
	pEast->Heuristic = CSearchAstar::Heuristic(pEast, pGoal);
	pEast->score = (pEast->parent->score + pEast->cost);
	pEast->TotalCost = pEast->score + pEast->Heuristic;

	//South (Sets up new node)
	pSouth->x = pCurrentNode->x;
	pSouth->y = pCurrentNode->y;
	pSouth->y--;
	/* Chceck to see if its less then the 0 */
	if (pSouth->y == -1)
	{
		pSouth->y = 0;
	}
	pSouth->parent = pCurrentNode.get();
	pSouth->cost = terrain[pSouth->x][pSouth->y];
	pSouth->Heuristic = CSearchAstar::Heuristic(pSouth, pGoal);
	pSouth->score = (pSouth->parent->score + pSouth->cost);
	pSouth->TotalCost = pSouth->score + pSouth->Heuristic;

	//West (Sets up New Node)
	pWest->x = pCurrentNode->x;
	pWest->y = pCurrentNode->y;
	pWest->x--;
	/* Chceck to see if its greater then the max size */
	if (pWest->x == -1)
	{
		pWest->x = 0;
	}
	pWest->parent = pCurrentNode.get();
	pWest->cost = terrain[pWest->x][pWest->y];
	pWest->Heuristic = CSearchAstar::Heuristic(pWest, pGoal);
	pWest->score = (pWest->parent->score + pWest->cost);
	pWest->TotalCost = pWest->score + pWest->Heuristic;
	return true;
}
/* This memeber function is used to check to see if the new nodes generated are already on the open list */
bool CSearchAstar::mOpenListCHecker(unique_ptr <SNode> &pCurrentNode, unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest, unique_ptr<SNode> &pGoal)
{
	for (auto p = openList.begin(); p != openList.end(); p++) // This ittorator will run through the openlist
	{
		if (pNorth->x == (*p)->x && pNorth->y == (*p)->y) // checks to see if the new state is already on the openlist
		{
			gASNorthOnList = true;
			/* This will test to see if the totalCost is greater then the orginal on the openList and if it is */
			/* then it will sets the parent to current, give it a new cost (cost = score) and will work out the total cost again */
			if (pNorth->TotalCost <= (*p)->TotalCost)
			{
				pNorth->parent = pCurrentNode.get();
				pNorth->cost = pNorth->score;
				pNorth->TotalCost = pNorth->cost + pNorth->Heuristic;
			}
		}
		if (pEast->x == (*p)->x && pEast->y == (*p)->y) // checks to see if the new state is already on the openlist
		{
			gASEastOnList = true;
			/* This will test to see if the totalCost is greater then the orginal on the openList and if it is */
			/* then it will sets the parent to current, give it a new cost (cost = score) and will work out the total cost again */
			if (pEast->TotalCost <= (*p)->TotalCost) 
			{
				pEast->parent = pCurrentNode.get();
				pEast->cost = pEast->score;
				pEast->TotalCost = pEast->cost + pEast->Heuristic;
			}
		}
		if (pSouth->x == (*p)->x && pSouth->y == (*p)->y) // checks to see if the new state is already on the openlist
		{
			gASSouthOnList = true;
			/* This will test to see if the totalCost is greater then the orginal on the openList and if it is */
			/* then it will sets the parent to current, give it a new cost (cost = score) and will work out the total cost again */
			if (pSouth->TotalCost <= (*p)->TotalCost)
			{
				pSouth->parent = pCurrentNode.get();
				pSouth->cost = pSouth->score;
				pSouth->TotalCost = pSouth->cost + pSouth->Heuristic;
			}
		}
		if (pWest->x == (*p)->x && pWest->y == (*p)->y) // checks to see if the new state is already on the openlist
		{
			gASWestOnList = true;
			/* This will test to see if the totalCost is greater then the orginal on the openList and if it is */
			/* then it will sets the parent to current, give it a new cost (cost = score) and will work out the total cost again */
			if (pWest->TotalCost <= (*p)->TotalCost)
			{
				pWest->parent = pCurrentNode.get();
				pWest->cost = pSouth->score;
				pWest->TotalCost = pWest->cost + pWest->Heuristic;
			}
		}

	}
	return true;

}

/*This works like the memeber function above and will check to see if already on the closedList*/
bool CSearchAstar::mClosedListCHecker(unique_ptr <SNode> &pCurrentNode, unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest, unique_ptr<SNode> &pGoal)
{
	if (!closedList.empty()) // checks to see if the closedList is empty
	{
		for (auto p = closedList.begin(); p != closedList.end(); p++) // itterators that runs through the closed list
		{
			if (pNorth->x == (*p)->x && pNorth->y == (*p)->y) // checks to see if the new state is already on the closed
			{
				gASNorthClosedList = true;
				/* This will test to see if the totalCost is greater then the orginal on the openList and if it is */
			    /* then it will sets the parent to current, give it a new cost (cost = score) and will work out the total cost again */
				if (pNorth->TotalCost <= (*p)->TotalCost)
				{
					if (gASNorthOnList == false) // checks to see if the bool is false
					{
						pNorth->parent = pCurrentNode.get();
						pNorth->cost = pNorth->score;
						pNorth->TotalCost = pNorth->cost + pNorth->Heuristic;
					}
					/* This will set up a new unique pointer and make it the same as the value of p and then will erase p*/
					unique_ptr <SNode> tmpNode(new SNode);
					tmpNode->x = pNorth->x;
					tmpNode->y = pNorth->y;
					tmpNode->score = pNorth->score;
					tmpNode->cost = pNorth->cost;
					p = closedList.erase(p);
					if (gASNorthOnList == false) // If this is true it will push the tmpNode onto the openList
					{
						openList.push_back(move(tmpNode));
					}
					else
					{
						tmpNode.reset(new SNode); // Resets the tmpNode
					}
				}
			}
			if (pEast->x == (*p)->x && pEast->y == (*p)->y) // checks to see if the new state is already on the closed
			{
				gASEastOnClosedList = true;
				/* This will test to see if the totalCost is greater then the orginal on the openList and if it is */
			    /* then it will sets the parent to current, give it a new cost (cost = score) and will work out the total cost again */
				if (pEast->TotalCost <= (*p)->TotalCost)
				{
					if (gASEastOnList == false)
					{
						pEast->parent = pCurrentNode.get();
						pEast->cost = pEast->score;
						pEast->TotalCost = pEast->cost + pEast->Heuristic;
					}
					/* This will set up a new unique pointer and make it the same as the value of p and then will erase p*/
					unique_ptr <SNode> tmpNode(new SNode);
					tmpNode->x = (*p)->x;
					tmpNode->y = (*p)->y;
					tmpNode->score = (*p)->score;
					tmpNode->cost = (*p)->cost;
					p = closedList.erase(p);
					if (gASEastOnList == false) // If this is true it will push the tmpNode onto the openList
					{
						openList.push_back(move(tmpNode)); 
					}
					else
					{
						tmpNode.reset(new SNode); // Resets the tmpNode
					}
				}
			}
			if (pSouth->x == (*p)->x && pSouth->y == (*p)->y) // checks to see if the new state is already on the closed
			{
				gASSouthOnClosedList = true;
				/* This will test to see if the totalCost is greater then the orginal on the openList and if it is */
			/* then it will sets the parent to current, give it a new cost (cost = score) and will work out the total cost again */
				if (pSouth->TotalCost <= (*p)->TotalCost)
				{
					if (gASSouthOnList == false)
					{
						pSouth->parent = pCurrentNode.get();
						pSouth->cost = pSouth->score;
						pSouth->TotalCost = pSouth->cost + pSouth->Heuristic;
					}
					gASSouthOnList = true;
					/* This will set up a new unique pointer and make it the same as the value of p and then will erase p*/
					unique_ptr <SNode> tmpNode(new SNode);
					tmpNode->x = (*p)->x;
					tmpNode->y = (*p)->y;
					tmpNode->score = (*p)->score;
					tmpNode->cost = (*p)->cost;
					p = closedList.erase(p);
					if (gASSouthOnList == false) // If this is true it will push the tmpNode onto the openList
					{
						openList.push_back(move(tmpNode));
					}
					else
					{
						tmpNode.reset(new SNode); // Resets the tmpNode
					}
				}
			}
			if (pWest->x == (*p)->x && pWest->y == (*p)->y) // checks to see if the new state is already on the closed
			{
				gASWestOnClosedList = true;
				/* This will test to see if the totalCost is greater then the orginal on the openList and if it is */
			/* then it will sets the parent to current, give it a new cost (cost = score) and will work out the total cost again */
				if (pWest->TotalCost <= (*p)->TotalCost)
				{
					if (gASWestOnList == false)
					{
						pWest->parent = pCurrentNode.get();
						pWest->cost = pSouth->score;
						pWest->TotalCost = pWest->cost + pWest->Heuristic;
					}
					/* This will set up a new unique pointer and make it the same as the value of p and then will erase p*/
					unique_ptr <SNode> tmpNode(new SNode);
					tmpNode->x = (*p)->x;
					tmpNode->y = (*p)->y;
					tmpNode->score = (*p)->score;
					tmpNode->cost = (*p)->cost;
					tmpNode->TotalCost = (*p)->TotalCost;
					p = closedList.erase(p);
					if (gASWestOnList == false) // If this is true it will push the tmpNode onto the openList
					{
						openList.push_back(move(tmpNode));
					}
					else
					{
						tmpNode.reset(new SNode); // Resets the tmpNode
					}
				}
			}
		}
	}
	return true;
}

int CSearchAstar::mPushAndPop(TerrainMap& terrain, unique_ptr <SNode> &pCurrentNode, unique_ptr <SNode> &pNorth, unique_ptr <SNode> &pEast, unique_ptr <SNode> &pSouth, unique_ptr <SNode> &pWest, int maxX, int maxY, I3DEngine* engine, vector <vector <IModel*>> Tiles, int &sortCount)
{
	if (gASNorthOnList == false && gASNorthClosedList == false) // Checks to see weather its on the open or closedList
	{
		if (terrain[pNorth->x][pNorth->y] != 0) // checks to see weather the new state is a wall or not
		{
			/* This will push the new state onto the openList and reset the state afterwards */
			openList.push_back(move(pNorth));
			pNorth.reset();
		}
		else
		{
			pNorth.reset();
		}
	}
	if (gASEastOnList == false && gASEastOnClosedList == false) // Checks to see weather its on the open or closedList
	{
		if (terrain[pEast->x][pEast->y] != 0) // checks to see weather the new state is a wall or not
		{
			/* This will push the new state onto the openList and reset the state afterwards */
			openList.push_back(move(pEast));
			pEast.reset();
		}
		else
		{
			pEast.reset();
		}
	}
	if (gASSouthOnList == false && gASSouthOnClosedList == false) // Checks to see weather its on the open or closedList
	{
		if (terrain[pSouth->x][pSouth->y] != 0) // checks to see weather the new state is a wall or not
		{
			/* This will push the new state onto the openList and reset the state afterwards */
			openList.push_back(move(pSouth));
			pSouth.reset();
		}
		else
		{
			pSouth.reset();

		}
	}
	if (gASWestOnList == false && gASWestOnClosedList == false) // Checks to see weather its on the open or closedList
	{
		if (terrain[pWest->x][pWest->y] != 0) // checks to see weather the new state is a wall or not
		{
			/* This will push the new state onto the openList and reset the state afterwards */
			openList.push_back(move(pWest));
			pWest.reset();
		}
		else
		{
			pWest.reset();

		}
	}
	closedList.push_front(move(pCurrentNode)); // Pushes current onto tbe openList
	sort(openList.begin(), openList.end(), [](unique_ptr<SNode>& lhs, // This sorts the openList and will put the state with the lowest cost at the start
		unique_ptr<SNode>& rhs) { return lhs->TotalCost < rhs->TotalCost; });
	sortCount++; // count amount of sorts and itterations it goes through
	/* If the openList isn't empty it will make currentNode equal to the front of the openList else it will return false */
	if (!openList.empty())
	{
		pCurrentNode = move(openList.front());
	}
	else
	{
		/* The will reset all of the boolen */
		gASNorthOnList = false;
		gASEastOnList = false;
		gASSouthOnList = false;
		gASWestOnList = false;
		gASNorthClosedList = false;
		gASEastOnClosedList = false;
		gASSouthOnClosedList = false;
		gASWestOnClosedList = false;
		return false;
	}
	/* The will reset all of the boolen */
	gASNorthOnList = false;
	gASEastOnList = false;
	gASSouthOnList = false;
	gASWestOnList = false;
	gASNorthClosedList = false;
	gASEastOnClosedList = false;
	gASSouthOnClosedList = false;
	gASWestOnClosedList = false;

	float frameTime = engine->Timer(); // This will get the timer function
	int countDown = 25; // counDown for the real time rep

	while (countDown > 0)
	{
		countDown -= frameTime;
		for (auto p = openList.begin(); p != openList.end(); p++)
		{
			if ((*p) != 0)
			{
				Tiles[(*p)->x][(*p)->y]->SetSkin("grey.png");
			}
		}
		for (auto o = closedList.begin(); o != closedList.end(); o++)
		{
			Tiles[(*o)->x][(*o)->y]->SetSkin("black.png");
		}
		Tiles[pCurrentNode->x][pCurrentNode->y]->SetSkin("blue.png");
	}

	engine->DrawScene(); // Will draw the scence to show the real time rep
	return true;
}

/* This is used when the end goal has been found */
int CSearchAstar::mPathPlotter(unique_ptr<SNode> &pGoal, unique_ptr <SNode> &pCurrentNode, NodeList& path)
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

bool CSearchAstar::FindPath(TerrainMap& terrain, unique_ptr<SNode> &pStart, unique_ptr<SNode> &pGoal, NodeList& path, int maxX, int maxY, vector <vector <IModel*>> Tiles, I3DEngine* engine)
{
	std::ofstream outputFile; // Used to output to file
	pStart->parent = nullptr; // sets parent to NULL
	pStart->cost = 0; // sets cost
	pStart->score = 1; // sets score
	pStart->Heuristic = CSearchAstar::Heuristic(pStart, pGoal); // sets the Heuristic
	pStart->TotalCost = pStart->score + pStart->Heuristic; // sets the total cost
	openList.push_back(move(pStart)); // pushes startNode onto the openList
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
		openList.pop_front(); // removes the first element
		if (pCurrentNode->x == pGoal->x && pCurrentNode->y == pGoal->y) // If this is true it means the path is found
		{
			pathWayFound = true;
		}
		if (pathWayFound == true)
		{
			CSearchAstar::mPathPlotter(pGoal, pCurrentNode, path); // returns the path
			outputFile.open("outputFileAstar.txt"); // opens the output file
			outputFile << "X _ Y" << endl; // outputs to text file
			for (auto p = path.begin(); p != path.end(); p++)
			{
				outputFile << (*p)->x << "_" << (*p)->y << endl; // This will output the current point the itterator is at
			}
			outputFile << "Number of Itterations: " << itterationsCount << endl; // outputs number of itterations
			outputFile << "Number of Sorts: " << itterationsCount << endl; // outputs number of sorts
			outputFile.close(); // closes the file
			return true;
		}
		CSearchAstar::mBlockChecker(terrain, pCurrentNode, pNorth, pEast, pSouth, pWest, pGoal, path, mapMaxX, mapMaxY, Tiles); // generates the new rules
		CSearchAstar::mOpenListCHecker(pCurrentNode, pNorth, pEast, pSouth, pWest, pGoal); // checks to see if the new states are on the openList
		CSearchAstar::mClosedListCHecker(pCurrentNode, pNorth, pEast, pSouth, pWest, pGoal); // checks to see if the new states are on the closedList
		CSearchAstar::mPushAndPop(terrain, pCurrentNode, pNorth, pEast, pSouth, pWest, mapMaxX, mapMaxY, engine, Tiles, itterationsCount); //pushes and pop values on the open and closedList
		/* Resets the new nodes */
		pNorth.reset(new SNode);
		pEast.reset(new SNode);
		pSouth.reset(new SNode);
		pWest.reset(new SNode);
	}
	openList.clear(); // clears the openList
	closedList.clear(); // clears the closedList
	outputFile.open("outputFileAstar.txt"); // Opens the output file
	outputFile << "PATH NOT FOUND" << endl; // output to the file
	outputFile.close(); // closes file
	return false;

}