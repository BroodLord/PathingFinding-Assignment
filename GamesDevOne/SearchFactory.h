// Made by Danny Kavananagh
// SearchFactory.h
// ===============
//
// Factory function declaration to create CSearchXXX objects 
//

#pragma once
#include "Search.h" // Search interface class
#include "CSearchBreathFirst.h" // BreathFirst Algothrim
#include "CSearchAstar.h" // Astar Algothrim

// List of implemented seach algorithms
enum ESearchType
{
	Astar,
	BreathFirst,
};

// Factory function to create CSearchXXX object where XXX is the given search type
ISearch* NewSearch(ESearchType search);
