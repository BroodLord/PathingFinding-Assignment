// Made by Danny Kavananagh
// Definitions.h
// =============
//
// Type definitions to support path finding 
//

#pragma once

#include <vector>
#include <deque>
#include <memory>
#include <TL-Engine.h>

using namespace std;
using namespace tle;
// Represents a node in the search tree.
struct SNode
{
  int x;            // x coordinate
  int y;            // y coordinate
  int score;        // used in more complex algorithms
  int cost;         // cost of the current title
  int TotalCost; // combained cost of all titles
  int Heuristic;  // Manhattion distance
  SNode* parent = 0; // note use of raw pointer here
};

/* Used to initlize the main loop states */
enum ESetUp
{
	setupStage,
	runningStage
};

// Lists of nodes (e.g Open, Closed and Paths) are implemented as double-ended queues
using NodeList = deque<unique_ptr<SNode>>;

// Terrain costs for map squares
enum ETerrainCost
{
  Clear = 1,
  Water = 3,
  Wood  = 2,
  Wall  = 0
};

/* LetterArray circled through so the user can load a map and is a const because it will never change */
const string LETTERARAY[26] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n",
							  "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" };
const int LETTERMAX = 25; // This is the max the letters can go up to.

// Maps of any size are implemented as 2D vectors
using TerrainMap = vector<vector<ETerrainCost>>;

bool LoadMeasurements(const char* coordsFile, unique_ptr <SNode> &start, unique_ptr <SNode> &finish); // Returns the start locations of the map
bool MapCheck(const char* map, bool &open); // Return a bool to see if the map can be opened
int LoadMap(const char* mapSize, int &mapSizeX, int &mapSizeY, TerrainMap &mapTerrian); // Resizes the map and returns the terrainMap of the current map
/* This will clear the map so it can be reused */
void ClearMap(vector <vector <IModel*>> &Tiles, IModel* pathFinder, IMesh* tilesMesh, unique_ptr <SNode> &pStartNode, TerrainMap &mapTerrian, unique_ptr <SNode> &pEndNode, int mapSizeX, int mapSizeY);
/* This will resize and spawn the tiles while setting their skins to max */
void CreateMap(vector <vector <IModel*>> &Tiles, IModel* pathFinder, IMesh* TilesMesh, unique_ptr <SNode> &pStartNode, TerrainMap &mapTerrian, unique_ptr <SNode> &pEndNode,
	int cubeModifier, int xStartingPos, int yStartingPos, int mapSizeX, int mapSizeY);