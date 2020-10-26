#ifndef GLASSSTRUCTURES_H
#define GLASSSTRUCTURES_H
/* This class is created
* to save the models
**/

//#pragma warning(disable: 4786)

#include <stdlib.h>
#include <vector>
#include <utility>

#include "GlassRectangle.h"

 class GlassRectangle;


//Definitions of typedef
typedef std::pair <int, int> ParInt;

typedef  std::list< GlassDefect > LGlassDefect;

typedef std::pair< std::pair< int, int >, std::pair< int, int > >  Coordinates;

typedef std::list< GlassRectangle > LRectangles;






#endif // GLASSSTRUCTURES_H
