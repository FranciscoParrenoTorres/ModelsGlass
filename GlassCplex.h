#ifndef _GLASSCPLEX_H_
#define _GLASSCPLEX_H_


#include "ilcplex/ilocplex.h"
#include <ilconcert/ilomodel.h>
ILOSTLBEGIN

#pragma warning(disable:4786)
#pragma warning(disable:4996)


using namespace std;
/*
	IloIntVarArray  variable entera
	IloNumVarArray  variable real
*/
typedef IloIntVarArray NumVector;
typedef IloArray<IloNumVarArray> NumVarMatrixF;
typedef IloArray<IloIntVarArray> NumVarMatrix;
typedef IloArray<IloNumVarArray> NumVarMatrix2F;
typedef IloArray<IloIntVarArray> NumVarMatrix2;
typedef IloArray<IloArray<IloNumVarArray> > NumVarMatrix3F;
typedef IloArray<IloArray<IloIntVarArray> > NumVarMatrix3;
typedef IloArray<IloArray<IloArray<IloIntVarArray> > > NumVarMatrix4;
typedef IloArray<IloBoolVarArray> BoolVarMatrix;



class GlassCplex
{

	char m_nombre[200];

	IloEnv env;
	IloModel model;
	IloCplex cplex;
	IloEnv env2;
	IloModel model2;
	IloCplex cplex2;

public:


	void FormulationStages();
};

#endif