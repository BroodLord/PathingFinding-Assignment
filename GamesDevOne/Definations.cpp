// Made by Danny Kavananagh
#include "Definitions.h"
#include <iostream>
#include <fstream>

bool MapCheck(const char* map, bool &open) // This is used to see if a map can be open or not
/* This block of code will try to open the map which is past over and if it can open it will set the bool to true and return true */
{
	std::ifstream testMap;
	testMap.open(map);
	if (testMap.is_open())
	{
		open = true;
		return true;
	}
	testMap.close();
	return false; //return false if it can't be opened
}
/**********************************************************************************************************************************/

bool LoadMeasurements(const char* coordsFile, unique_ptr <SNode> &start, unique_ptr <SNode> &finish) // This is used to load the map start and end coords
{
	std::ifstream dReadInCoords;
	dReadInCoords.open(coordsFile);
	if (dReadInCoords.is_open())
	{
		/*This will place the numbers in dReadInCoords into the follow data*/
		dReadInCoords >> start->x;
		dReadInCoords >> start->y;
		dReadInCoords >> finish->x;
		dReadInCoords >> finish->y;
		dReadInCoords.close();
		return true; // turns true if it can open and tranfered
	}
	else
	{
		return false; // return false if it can't be opened
	}
}
int LoadMap(const char* mapSize,int &mapSizeX, int &mapSizeY, TerrainMap &mapTerrian) // This is used to load in the map
{
	std::ifstream mapInput;
	mapInput.open(mapSize);
	if (mapInput.is_open())
	{
		mapInput >> mapSizeX; // loads in the max X of the map into the variable
		mapInput >> mapSizeY; // loads in the max Y of the map into the variable
		char tempChar; // used to store the current number in the map
		mapTerrian.resize(mapSizeX); // resizes the mapTerrian to mapSizeX

		for (int x = 0; x < mapSizeX; x++) // This will run until its greater the the max X of the map
		{
			mapTerrian[x].resize(mapSizeY); // resizes the mapTerrian to mapSizeY
		}

		for (int x = mapSizeY - 1; x >= 0; x--)
		{
			vector <ETerrainCost> tempMapRow; // used to push the tempchar onto
			for (int y = 0; y < mapSizeX; y++)
			{
				mapInput >> tempChar; // Puts the current number into the tmpChar
				/* A char is used so that it will load a single number at a time*/
				mapTerrian[y][x] = (ETerrainCost(tempChar - '0')); // This will make mapTerrian[x][y] have a cost of what ever tmpChar is, tempChar - 0 is used to remove the ascci value.

			}
			mapTerrian.push_back(tempMapRow); // Pushes back the current row onto the mapTerrian

		}
		return true;
	}
	else
	{
		return false;
	}

}

void ClearMap(vector <vector <IModel*>> &tiles, IModel* pathFinder, IMesh* tilesMesh, unique_ptr <SNode> &pStartNode, TerrainMap &mapTerrian, unique_ptr <SNode> &pEndNode, int mapSizeX, int mapSizeY)
{
	/* This will run through the map and will remove every tile which has been placed on the visual display */
	for (int y = 0; y < mapSizeY; y++)
	{
		for (int x = 0; x < mapSizeX; x++)
		{
			tilesMesh->RemoveModel(tiles[x][y]);
		}
	}
	pStartNode.reset(new SNode); // Resets the startnode so it can be reused
	pEndNode.reset(new SNode); // Resets the endnode so it can be reused
	mapTerrian.clear(); // clears the mapTerrian
	tiles.clear(); // clears all the tiles
	mapSizeX = 0; // sets the mapSizex to 0 so it can be reused
	mapSizeY = 0; // sets the mapSizey to 0 so it can be reused
}

void CreateMap(vector <vector <IModel*>> &Tiles, IModel* pathFinder, IMesh* TilesMesh, unique_ptr <SNode> &pStartNode, TerrainMap &mapTerrian, unique_ptr <SNode> &pEndNode,
	int cubeModifier, int xStartingPos, int yStartingPos, int mapSizeX, int mapSizeY)
{
	/*This will go through the resizing process again for the titles so it can be used with the mapTerrian*/
	Tiles.resize(mapSizeX);
	for (int x = 0; x < mapSizeX; x++)
	{
		Tiles[x].resize(mapSizeY);
	}
	/*******************************************************************************************************/
	for (int y = 0; y < mapSizeY; y++)
	{
		for (int x = 0; x < mapSizeX; x++)
		{
			Tiles[x][y] = (TilesMesh->CreateModel(xStartingPos + (x * cubeModifier), 0.2f, yStartingPos + (y * cubeModifier))); // Creates the tiles (x * cubeModifer is used to split the tiles to stop them meshing together)
			/* This will set the skins of the tiles to match the mapTerrian so that its obververse to the user what is what*/
			if (mapTerrian[x][y] == 0)
			{
				Tiles[x][y]->SetSkin("Wall.png");
			}
			else if (mapTerrian[x][y] == 1)
			{
				Tiles[x][y]->SetSkin("dirt.png");
			}
			else if (mapTerrian[x][y] == 2)
			{
				Tiles[x][y]->SetSkin("water.png");
			}
			else if (mapTerrian[x][y] == 3)
			{
				Tiles[x][y]->SetSkin("Forest.png");
			}
			/***************************************************************************/
			/* These two checks will see if the current tile[x][y] = the start or end node and will set there skins to match*/
			if (pStartNode->x == x && pStartNode->y == y)
			{
				Tiles[x][y]->SetSkin("green.png");
			}
			if (pEndNode->x == x && pEndNode->y == y)
			{
				Tiles[x][y]->SetSkin("red.PNG");
			}
		}
	}
}