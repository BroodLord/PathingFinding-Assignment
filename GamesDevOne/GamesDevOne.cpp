// GamesDevOne.cpp: A program using the TL-Engine
// Made by Danny Kavananagh
// Youtube Vid: https://youtu.be/NY-rwzOMOzI

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "Definitions.h"
#include "SearchFactory.h"
using namespace tle;

/* These are keys that will preform a certain part of the code */
const EKeyCode kAstar = Key_1;
const EKeyCode kBreathFirst = Key_2;
const EKeyCode kPlusLetter = Key_Right;
const EKeyCode kMinusLetter = Key_Left;
const EKeyCode kLoadMap = Key_Return;
const EKeyCode kStartPath = Key_Space;
const EKeyCode kReset = Key_R;
const EKeyCode kQuit = Key_Escape;


TerrainMap gmapTerrian; // Holds the map numbers
unique_ptr <SNode> gpStartNode(new SNode); // Will point to the start position on the map
unique_ptr <SNode> gpEndNode(new SNode); // Will point to the end position on the map
ESearchType SearchTypes; // Has the differnet search types held in the enum
NodeList gpath; // holds the correct path when found.

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
	myEngine->AddMediaFolder("D:\\DKavanagh2\\Documents\\GitHub\\PathingFinding-Assignment\\GamesDevOne\\Models");

	/**** Set up your scene here ****/
	ICamera* myCam; // Model for the camera
	IMesh* tilesMesh = myEngine->LoadMesh("cube.x");  // Loads the mesh for the tile
	IModel* skyBox; // Model for the skybox
	IModel* pathFinder; // Model for the moving model
	ESetUp setUp = setupStage; // Sets up the enum
	pathFinder = tilesMesh->CreateModel(-200.0f, 3.0f, 0.0f); // Creates the patherfinder model
	pathFinder->Scale(0.5f); // scales the pathfinder model down
	skyBox = tilesMesh->CreateModel(50.0f, -500.0f, 0.0f); // creates the skybox model
	skyBox->Scale(100.0f); // Scales the skybox up
	skyBox->SetSkin("black.png"); // Set the skin of the skybox
	myCam = myEngine->CreateCamera(kManual); // Creates the camrea
	myCam->SetLocalPosition(50.0f, 150.0f, 40.0f); // Sets the position of the camera
	myCam->RotateLocalX(90.0f); // Rotates the camrea
	int currentLetter = 0; // Used as a counter to go throught the Letter Array
	bool cleared = false; // Used to see why the map is cleared
	bool mapOpen = false; // Used to see if the map is able to load
	int mapX = 0; // This will store the max X of the loaded map
	int mapY = 0; // This will store the max Y of the loaded map
	int pathFinderStartX = 0; // This is the start position on the x for the patherfinder
	int pathFinderStartZ = 0; // This is the start position on the xyfor the patherfinder
	int xStartingPos = 0.0f; // This is the start position on the x for the titles
	int yStartingPos = 0.0f; // This is the start position on the y for the titles
	int cubeModifier = 10; // This will space the cubes out so they don't mesh together
	bool ready = false; // used to see when its ready to load the pathfinding algorithem
	bool loading = false;  // Used to deactivate buttons when the pathfinding algorithem is running
	IFont* uiFont = myEngine->LoadFont("Comic Sans MS", 32); // The font size and font type of the on screen text
	vector <vector <IModel*>> tiles; // Models for the tiles
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene

		myEngine->DrawScene();

		ISearch* SearchEngine = NewSearch(SearchTypes); // Loads the search engine to the selected algoritem
		switch (setUp) // Used to switch the different states of the programming, this splits up the code.
		{
		case setupStage:
		{   /*****************Screen Text***************************/
			uiFont->Draw("_____________________", 980, -20, kWhite);
			uiFont->Draw("Map Details", 980, 10, kWhite);
			uiFont->Draw("Current Map", 980, 60, kWhite);
			uiFont->Draw(LETTERARAY[currentLetter], 980, 100, kWhite);
			uiFont->Draw("Map.txt", 1000, 100, kWhite);
			uiFont->Draw("Current Coords", 980, 160, kWhite);
			uiFont->Draw(LETTERARAY[currentLetter], 980, 200, kWhite);
			uiFont->Draw("Coords.txt", 1000, 200, kWhite);
			uiFont->Draw("_____________________", 980, 210, kWhite);
			uiFont->Draw("_____________________", 980, 220, kWhite);
			uiFont->Draw("Current Algorithm", 980, 260, kWhite);
			uiFont->Draw("_____________________", 10, -20, kWhite);
			uiFont->Draw("KeyCodes", 10, 10, kWhite);
			uiFont->Draw("Press: 1 for Astar", 10, 50, kWhite);
			uiFont->Draw("Press: 2 for BreathFirst", 10, 90, kWhite);
			uiFont->Draw("Press: Enter to pick a map", 10, 130, kWhite);
			uiFont->Draw("Press: Space to start the search", 10, 170, kWhite);
			uiFont->Draw("Press: -> To \nIncrement The Map Letter", 10, 210, kWhite);
			uiFont->Draw("Press: <- To \nDecrement The Map Letter", 10, 270, kWhite);
			uiFont->Draw("_____________________", 10, 310, kWhite);
			uiFont->Draw("_____________________", 10, 330, kWhite);
			uiFont->Draw("Map Costs", 10, 360, kWhite);
			uiFont->Draw("Walls : 0", 10, 400, kWhite);
			uiFont->Draw("Dirt : 1", 10, 440, kWhite);
			uiFont->Draw("Wood : 2", 10, 480, kWhite);
			uiFont->Draw("Water : 3", 10, 520, kWhite);
			/*******************************************************/
			switch (SearchTypes)
			{
			case Astar:
			{
				uiFont->Draw("Astar", 980, 300, kWhite); // displays the name of the algorithem
				ISearch* SearchEngine = NewSearch(Astar); // sets the search type to load Astar
				break;
			}
			case BreathFirst:
			{
				uiFont->Draw("BreathFirst", 980, 300, kWhite); // displays the name of the algorithem
				ISearch* SearchEngine = NewSearch(BreathFirst); // sets the search type to load BreathFirst
				break;
			}
			}
			/* This will load an algorthiem depending on what button is pressed */
			if (myEngine->KeyHeld(kAstar)) 
			{
				SearchTypes = Astar;
			}
			if (myEngine->KeyHit(kBreathFirst))
			{
				SearchTypes = BreathFirst;
			}
			/*******************************************************************/

			/*Key Presses that will switch the current letter*/
			if (myEngine->KeyHit(kPlusLetter))
			{
				if (currentLetter <= LETTERMAX) // checks to see if its at max
				{
					currentLetter++; // incress the currentLetter to display another letter
				}
				if (currentLetter > LETTERMAX) // checks to see if its greater then max
				{
					currentLetter = 0; // sets currentletter to 0
				}
			}
			else if (myEngine->KeyHit(kMinusLetter))
			{
				if (currentLetter >= 0) // checks to see if its at max
				{
					currentLetter--; // decress the currentLetter to display another letter
				}
				if (currentLetter < 0) // checks to see if its less then max
				{
					currentLetter = LETTERMAX; // sets currentletter to 25
				}
			}
			if (myEngine->KeyHit(kLoadMap)) // When pressed it will load the current map
			{
				string mapLetter = LETTERARAY[currentLetter]; // Gets the current Letter
				string tmpMap = "mMap.txt"; // gets the default map 
				string tmpCoords = "mCoords.txt"; // gets the default coords
				tmpMap.replace(0, 1, mapLetter); // This will replace the first letter of map with mapLetter (Used to load any map)
				tmpCoords.replace(0, 1,mapLetter); // This will replace the first letter of coords to mapLetter (used to load any coords)
				const char* Map = tmpMap.c_str(); // This will set the map we open to tmpMap (c_str makes it so we can conver const char* to string)
				const char* coords = tmpCoords.c_str(); // This will set the coords we open to tmpcoords (c_str makes it so we can conver const char* to string)
				MapCheck(Map, mapOpen); // This will check to see if the map can be open, return mapOpen either true or false
				if (mapOpen == true)
				{
					if (cleared == false)
					{
						ClearMap(tiles, pathFinder, tilesMesh, gpStartNode, gmapTerrian, gpEndNode, mapX, mapY); // This will clear the map
					}
					LoadMeasurements(coords, gpStartNode, gpEndNode); // Loads the start and end coords
					LoadMap(Map, mapX, mapY, gmapTerrian); // Loads the currnet map (doesn't create it)
					CreateMap(tiles, pathFinder, tilesMesh, gpStartNode, gmapTerrian, gpEndNode, // This will take the what was loaded in loadmap and will create the map out of tiles
						cubeModifier, xStartingPos, yStartingPos, mapX, mapY);
					pathFinderStartX = tiles[gpStartNode->x][gpStartNode->y]->GetLocalX(); // This gets the starting x for the moving model (used for placement)
					pathFinderStartZ = tiles[gpStartNode->x][gpStartNode->y]->GetLocalZ(); // This gets the starting y for the moving model (used for placement)
					pathFinder->SetSkin("purple.png"); // Sets the skin
					pathFinder->SetLocalPosition(pathFinderStartX, 5.0f, pathFinderStartZ); // Sets the moving model onto the start node.
					cleared = false; // set to false so the map can be cleared again
					mapOpen = false; // sets to false so a new map can be tested
					ready = true; // set to true so that next part of the program can start
				}
				else
				{
					mapOpen = false; // set to false if the map can't be opened
				}
			}
			if (ready == true) // check to see if map has been loaded
			{
				if (myEngine->KeyHit(kStartPath)) // When pressed it will start the finding a path
				{
					setUp = runningStage; // Sets the next state to be ran
					ready = false; // Set to false so map can be reload after
					loading = true; // sets to true so the path can start being found
				}
				break;
			}
		}
		case runningStage:
		{
			/* More display text and position of it */
			uiFont->Draw("PRESS R TO RESET THE MAP", 490, 640, kWhite);
			uiFont->Draw("_____________________", 500, 650, kWhite);
			if (loading == true) // checks to see if the search can start
			{
				SearchEngine->FindPath(gmapTerrian, gpStartNode, gpEndNode, gpath, mapX, mapY, tiles, myEngine); // This will find our path on the map that was picked with the picked algorithem
				// used to return a path so it can be constructed
				for (auto & it : gpath) // ittorator to run through the path that was returned
				{
					tiles[it->x][it->y]->SetSkin("blue.png"); // sets skin
				}
				gpath.clear(); // clears all data on the list so it can be reused
				loading = false; // sets to false so it won't re-run the searchEngine
			}
			/* This is used to reset all the values which are used*/
			if (myEngine->KeyHit(kReset))
			{
				ClearMap(tiles, pathFinder, tilesMesh, gpStartNode, gmapTerrian, gpEndNode, mapX, mapY);
				gpStartNode.reset(new SNode);
				gpEndNode.reset(new SNode);
				mapX = 0;
				mapY = 0;
				gmapTerrian.clear();
				tiles.clear();
				cleared = false;
				setUp = setupStage;
			}
			/*******************************************************/
		}
		}
		/* Closes the program when pressed */
		if (myEngine->KeyHit(kQuit))
		{
			myEngine->Stop();
		}

		/**** Update your scene each frame here ****/

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
