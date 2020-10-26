#ifndef GLASSCORTE_H
#define GLASSCORTE_H

#include <stdio.h>

#include <algorithm>
#include "list"

using namespace std;

class Glass_Corte
{
public:
	Glass_Corte();
	virtual ~Glass_Corte();
	Glass_Corte(int b, int n, int x, int y, int w, int h, int t, int c, int p)
	{
		bin = b; node = n; x1 = x; y1 = y; width = w; height = h; type = t; cut = c; parent = p;
	}
	Glass_Corte(int b, int n, int x, int y, int w, int h, int t, int c)
	{
		bin = b; node = n; x1 = x; y1 = y; width = w; height = h; type = t; cut = c; parent = -1;
	}
	
	int bin;
	int node;
	int x1;
	int y1;
	int width;
	int height;
	int type;
	int cut;
	int parent;

}; 



#endif // GLASSCORTE_H