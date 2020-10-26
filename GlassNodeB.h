#ifndef GLASSNODEB_H
#define GLASSNODEB_H

#include <ostream>
#include <vector>
#include <list>
#include "GlassRsol.h"
/* This class is created
* to model a solution node instance with its all attributes
**/
class GlassNodeB {
public:
	GlassNodeB();
	GlassNodeB(int  Inicio, int  Plates, int  Total_area,	int  Total_area_used,int  Total_width_bin,	int  Objective_function,
	int  Partial_objective_function,	int  MinDimensionPieza, std::vector< std::vector < int > > Matrix_Items,	std::vector< int > Vector_Stacks,
	std::vector< bool > Vector_Items,	std::list< GlassRsol> Sol_Items);
	virtual ~GlassNodeB();
	double Utilization() {
		return _Utilization;
	};
	double Utilization2() {
		return _Utilization2;
	};
	int  Inicio() {
		return _Inicio;
	};
	int  Plates() {
		return _Plates;
	};
	int  Total_area() {
		return _Total_area;
	}; // Sum of total area of items.
	int  Total_area_used()
	{
		return _Total_area_used;
	};//Suma of used area partial
	int  Total_width_bin()
	{
		return _Total_width_bin;
	};
	int  Max_First_Bin()
	{
		return _Max_First_Bin;
	};
	int  Objective_function()
	{
		return _Objective_function;
	};
	int  Partial_objective_function()
	{
		return _Partial_objective_function;
	};// Percentaje per thousand
	int  MinDimensionPieza()
	{
		return _MinDimensionPieza;
	};
	int  MaxMinDimensionPieza()
	{
		return _MaxMinDimensionPieza;
	};
	std::vector< std::vector < int > > &Matrix_Items()
	{
		return _Matrix_Items;
	};
	// the size of this vector is the number of stacks, the first number is 
	//which is the first item possible to place
	std::vector< int > &Vector_Stacks(){
		return _Vector_Stacks;
	};
	std::vector< bool > &Vector_Items(){
		return _Vector_Items;
	};
	
	std::list< GlassRsol> &Sol_Items(){
		return _Sol_Items;
	};
	//set functions
	void Inicio(int  a)
	{
		_Inicio = a;
	};
	void Plates(int  a)
	{
		_Plates = a;
	};
	void Total_area(int  a) {
		_Total_area=a;
	}; // Sum of total area of items.
	void Total_area_used(int  a)
	{
		_Total_area_used=a;
	};//Suma of used area partial
	void Total_width_bin(int  a)
	{
		_Total_width_bin=a;
	};
	void Max_First_Bin(int  a)
	{
		_Max_First_Bin = a;
	};
	void Objective_function(int  a)
	{
		_Objective_function=a;
	};
	void Utilization(double a)
	{
		_Utilization = a;
	};
	void Utilization2(double a)
	{
		_Utilization2 = a;
	};
	void Partial_objective_function(int  a)
	{
		_Partial_objective_function=a;
	};// Percentaje per thousand
	void MinDimensionPieza(int  a)
	{
		_MinDimensionPieza=a;
	};
	void MaxMinDimensionPieza(int  a)
	{
		_MaxMinDimensionPieza = a;
	};
	void Matrix_Items(std::vector< std::vector < int > > a)
	{
		_Matrix_Items=a;
	};
	// the size of this vector is the number of stacks, the first number is 
	//which is the first item possible to place
	void Vector_Stacks(std::vector< int > a) {
		_Vector_Stacks=a;
	};
	void Vector_Items(std::vector< bool > a) {
		_Vector_Items=a;
	};

	void Sol_Items(std::list< GlassRsol> a) {
		_Sol_Items=a;
	};
/*	friend std::ostream & operator<<(std::ostream& os, GlassNode node) {
		os << "Node (p,id,x,y,w,h,t,s) = (" << node._plateId << "," << node._id << "," << node._x << "," << node._y << "," << node._w << "," << node._h << "," << node._nodeType << "," << node.cut << ")";
		return os;
	}*/

//public:
//	GlassNode * successor; // Successor nodes list.
protected:
private:
	double _Utilization;
	double _Utilization2=0;
	int  _Inicio;
	int  _Plates;
	int  _Total_area; // Sum of total area of items.
	int  _Total_area_used;//Suma of used area partial
	int  _Total_width_bin;
	int  _Max_First_Bin=0;
	int  _Objective_function;
	int  _Partial_objective_function;// Percentaje per thousand
	int  _MinDimensionPieza;
	int  _MaxMinDimensionPieza;
	std::vector< std::vector < int > > _Matrix_Items;
	// the size of this vector is the number of stacks, the first number is 
	//which is the first item possible to place
	std::vector< int > _Vector_Stacks;
	std::vector< bool > _Vector_Items;

	std::list< GlassRsol> _Sol_Items;
};

#endif // GLASSNODEB_H
