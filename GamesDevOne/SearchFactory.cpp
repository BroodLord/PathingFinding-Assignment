// Made by Danny Kavananagh
// SearchFactory.cpp
// =================
//
// Factory function implementation to create CSearchXXX objects 
//

#include "SearchFactory.h" // Factory declarations
#include "Search.h"
#include <deque>

/* This is used to load the selected enum type and then will load the correct algorithem */
ISearch* NewSearch(ESearchType search)
{
	switch (search)
	{
	case Astar:
	{
		return new CSearchAstar; // This will return the Astar Algorithem
		break;
	}
	case BreathFirst:
	{
		return new CSearchBreathFirst; // This will return the Breathfirst Algorithem
		break;
	}
	}
	return 0;
}

