#include "GlassPlate.h"

GlassPlate::GlassPlate()
{
    //ctor
    waste = 0.0;
    useful = 0.0;
    successor_nbr = 0;
    defect_nbr = 0;
    residual.x(0);
    residual.y(0);
}
GlassPlate::GlassPlate(int nodoant, GlassRectangle &R, int nplate, int  *nodos)
{
	type = -2;
	cut = 0;
	pos_x = R.Getpos_x();
	pos_y = R.Getpos_y();
	width = R.Get_w();
	height = R.Get_h();
	waste = 0.0;
	useful = 0.0;
	successor_nbr = 0;
	defect_nbr = 0;
	plate_id = nplate;
	node_id = *nodos;
	node_parent = nodoant;
	(*nodos)++;
	residual.x(0);
	residual.y(0);
}
GlassPlate::GlassPlate(int nodoant, std::vector<int> &vecp , int nplate, int  *nodos, int ntype, int  ncut,int cant)
{
	type = ntype;
	cut = ncut;
	pos_x = vecp[1];
	pos_y = vecp[2];
	width = vecp[3]- vecp[1];
	height = vecp[4] - vecp[2];
	if (type == (-2))
		height = cant;
	waste = 0.0;
	useful = 0.0;
	successor_nbr = 0;
	defect_nbr = 0;
	plate_id = nplate;
	node_id = *nodos;
	node_parent = nodoant;
	(*nodos)++;
	residual.x(0);
	residual.y(0);
}
GlassPlate::GlassPlate(GlassRectangle &R,int nplate,int  *nodos)
{
	//ctor
	pos_x = R.Getpos_x();
	pos_y = R.Getpos_y();
	width = R.Get_w();
	height = R.Get_h();
	waste = 0.0;
	useful = 0.0;
	successor_nbr = 0;
	defect_nbr = 0;
	plate_id = nplate;
	node_id = *nodos;
	if (node_id == 0)
	{
		type = -2;
		cut = 0;
	}
	node_parent = 0;
	(*nodos)++;
	residual.x(0);
	residual.y(0);
}
GlassPlate::~GlassPlate()
{
    //dtor
}
