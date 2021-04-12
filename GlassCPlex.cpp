#include "Glass.h"

#pragma warning(disable:4786)


#include <assert.h>
#include <sys/types.h>
#include <sys/timeb.h>


#include <ilcplex/ilocplex.h>
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
typedef IloArray < IloArray<IloArray<IloArray<IloIntVarArray> > > > NumVarMatrix5;
typedef IloArray < IloArray < IloArray<IloArray<IloArray<IloIntVarArray> > > > > NumVarMatrix6;

typedef IloBoolVarArray BoolVector;
typedef IloArray<IloBoolVarArray> BoolVarMatrix;
typedef IloArray<IloBoolVarArray> BoolVarMatrix2;
typedef IloArray<IloArray<IloBoolVarArray> > BoolVarMatrix3;
typedef IloArray<IloArray<IloArray<IloBoolVarArray> > > BoolVarMatrix4;
typedef IloArray < IloArray<IloArray<IloArray<IloBoolVarArray> > > > BoolVarMatrix5;
typedef IloArray < IloArray < IloArray<IloArray<IloArray<IloBoolVarArray> > > > > BoolVarMatrix6;



//#include "GraspCPlex3Indices.cpp"


extern  "C" int  bouknap(int n, int* p, int* w, int* m, int* x, int c);

int  Glass::CalcularPerdidaPieza(int pieza_elegida, int ancho, int largo)
{

	std::vector<int> areas;

	for (int i = 0; i < batch_items; i++)
	{
		if (i == pieza_elegida) continue;
		if ((items[i].Getitem_stack() == items[pieza_elegida].Getitem_stack()) && (items[i].Getitem_seq() < items[pieza_elegida].Getitem_seq()))
			continue;
		if ((items[i].Getitem_min() <= ancho && items[i].Getitem_max() <= (plate_h - largo))
			|| (items[i].Getitem_min() <= (plate_h - largo) && items[i].Getitem_max() <= ancho))
			areas.push_back(items[i].Getitem_area());


	}
	int Area_total = (plate_h - largo) * ancho;
	int piezas_caben = areas.size();

	int max = 0, todas = 0;
	int* profit = (int*)calloc(piezas_caben, sizeof(int));
	int* weight = (int*)calloc(piezas_caben, sizeof(int));
	int* m = (int*)calloc(piezas_caben, sizeof(int));
	int* x = (int*)calloc(piezas_caben, sizeof(int));
	int Area_posible = 0;
	int val_posible = 0;

	for (int i = 0; i < piezas_caben; i++)
	{

		int d = areas[i];
		profit[i] = areas[i];
		weight[i] = areas[i];
		//en este caso todos sería de 1
		m[i] = 1;
		Area_posible += profit[i];

	}
	if (Area_posible > Area_total)

		Area_posible = bouknap(piezas_caben, profit, weight, m, x, Area_total);


	return Area_total - Area_posible;


}


void Glass::knapsack_bouknap_tiras()
{

	//Se crean los vectores de enteros, profit es lo que ganamos
	//weight es el peso 
	//m cuántos de esa pieza
	//x sería la solución
	int max = 0, todas = 0;
	int* profit = (int*)calloc(2 * batch_items, sizeof(int));
	int* weight = (int*)calloc(2 * batch_items, sizeof(int));
	int* m = (int*)calloc(2 * batch_items, sizeof(int));
	int* x = (int*)calloc(2 * batch_items, sizeof(int));
	/*for (int i = 0; i < 150; i++)
	{
		weight[i] = 0;
		profit[i] = 0;
		m[i] = 0;
		x[i] = 0;
	}*/
	int sum_posible = 0;
	int val_posible = 0;
	for (int i = 0; i < batch_items; i++)
	{
		int d = items[i].Getitem_h();
		profit[i] = 1;
		weight[i] = CalcularPerdidaPieza(i, items[i].Getitem_h(), items[i].Getitem_w());
		//en este caso todos sería de 1
		m[i] = 1;
		val_posible += profit[i];
		sum_posible += weight[i];
	}
	for (int i = 0; i < batch_items; i++)
	{
		int d = items[i].Getitem_w();
		profit[i + batch_items] = 1;
		weight[i + batch_items] = CalcularPerdidaPieza(i, items[i].Getitem_w(), items[i].Getitem_h());
		//en este caso todos sería de 1
		m[i + batch_items] = 1;
		val_posible += profit[i + batch_items];
		sum_posible += weight[i + batch_items];

	}
	printf("Aqui 1");


	if (sum_posible <= plate_w || val_posible <= Best_Min_Perdida)
		maxTiras = val_posible;
	else maxTiras = bouknap(2 * batch_items, profit, weight, m, x, Best_Min_Perdida);

	if (P_maxnivelesw == 0 || P_maxnivelesw > maxTiras)
		P_maxnivelesw = maxTiras;

	printf("Aqui 2");
	//z es lo máximo
	//en x ahora tendría cuáles escoge


	printf("Máximo Tiras: Tiras=%d\t Pilas=%d \tPiezas=%d\n", maxTiras, maxPilas, maxPiezasPila);
}
int Glass::SubSetSumEntera(int Val)
{
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	NumVector x_l(env);
	NumVector y_l(env);
	/***************** INICIALIZO VARIABLES *******************/
/**********************************************************/
	//inicializo las variable y_l
	printf("Aqui21\n");
	char nombre[20];
	x_l = IloIntVarArray(env, batch_items, 0, 1);
	y_l = IloIntVarArray(env, batch_items, 0, 1);
	for (int l = 0; l < batch_items; l++)
	{
		sprintf(nombre, "X%d", l);
		x_l[l].setName(nombre);
		sprintf(nombre, "Y%d", l);
		y_l[l].setName(nombre);
	}


	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/
//	printf("Aqui22 Temp_Conjuntos %d\n", Temp_Conjuntos.size());

	//Suma menor o igual que el valor

	IloExpr v1(env);
	for (int i = 0; i < batch_items; i++)
	{
		v1 += x_l[i] * items[i].Getitem_w();
		v1 += y_l[i] * items[i].Getitem_h();
		IloExpr v2(env);
		v2 += x_l[i] + y_l[i];
		restr.add(IloRange(env, -IloInfinity, v2, 1));
		v2.end();
	}

	restr.add(IloRange(env, Val, v1, IloInfinity));
	v1.end();

	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
	model.add(restr);

	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);

	for (int i = 0; i < batch_items; i++)
	{
		//		Expression_fobj += x_l[i] * Temp_Conjuntos[i].number;
		Expression_fobj += x_l[i] * items[i].Getitem_w();
		Expression_fobj += y_l[i] * items[i].Getitem_h();
		//		printf("\n %d %d\n", i, Temp_Conjuntos[i].number);
	}
	//	printf("Aqui65\n");



	model.add(IloMinimize(env, Expression_fobj));
	Expression_fobj.end();
	//	printf("Aqui66\n");

		/***************************************************************/
		/*************** EJECUTAMOS EL MODELO **************************/
		/***************************************************************/
	try {
		//		printf("Aqui67\n");
		int value2 = 0;
		IloCplex cplex(model);
		printf("Aqui687\n");
		//Si queremos las lazy y tenemos alguna
//		if (Lazy==true && stack_nbr<batch_items)

	//	cplex.addLazyConstraints(restrLazy);
		//No hace el probing
		//cplex.setParam(IloCplex::Probe, -1);
		//1 CORE
		cplex.setParam(IloCplex::Threads, 8);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, G_Time); //TIEMPO EN SEGUNDOS

//		cplex.exportModel("RoadefNumberLevels.lp");

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

		//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();
		//		printf("Aqui68");

		IloBool result = cplex.solve();

		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;
		//		printf("Aqui666\n");

				//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


			//			FILE* f = fopen("./estadomodelo.txt", "a+");
						//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
				printf("Infeasible\n");
				//			cplex.exportModel("Roadef.lp");
			}
			else
			{
				if (cplex.getStatus() == IloAlgorithm::Unbounded) printf("Unbounded\n");
				printf("\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}

		}
		else
		{
			//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			value2 = cplex.getObjValue();
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible

			if (cplex.getSolnPoolNsolns() > 0)
			{

				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
//				printf("Aqui26\n");
				for (int i = 0; i < batch_items; i++)
				{



					if (cplex.isExtracted(x_l[i]) && cplex.getValue(x_l[i]) > 0.001)
					{
						printf("%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack(), i);
						//						fprintf(f, "%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
					}
					if (cplex.isExtracted(y_l[i]) && cplex.getValue(y_l[i]) > 0.001)
					{
						printf("%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack(), i);
						//						fprintf(f, "%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
					}

				}
				//				printf("Aqui27\n");






			}
			//			printf("Aqui288");
						//}
		}
		printf("Aqui299");
		cplex.end();
		model.end();
		env.end();
		return value2;
	}
	catch (IloException & e)
	{
		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
	printf("Aqui666\n");
}

int Glass::MultiKnapsackEntera(int Val)
{
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	NumVarMatrix2 x_ij(env, batch_items);
	NumVarMatrix2 y_ij(env, batch_items);

	NumVector x_i(env);
	NumVector y_i(env);
	NumVector px_i(env, batch_items);
	NumVector py_i(env, batch_items);

	/***************** INICIALIZO VARIABLES *******************/
/**********************************************************/
	//inicializo las variable y_l
	printf("Aqui21\n");
	char nombre[20];
	x_i = IloIntVarArray(env, batch_items, 0, 1);
	y_i = IloIntVarArray(env, batch_items, 0, 1);


	for (int l = 0; l < batch_items; l++)
	{
		px_i[l] = IloIntVar(env, 0, plate_h * items[l].Getitem_w());
		py_i[l] = IloIntVar(env, 0, plate_h * items[l].Getitem_h());
		sprintf(nombre, "X%d", l);
		x_i[l].setName(nombre);
		sprintf(nombre, "Y%d", l);
		y_i[l].setName(nombre);
		sprintf(nombre, "Px%d", l);
		px_i[l].setName(nombre);
		sprintf(nombre, "Py%d", l);
		py_i[l].setName(nombre);

	}
	for (int i = 0; i < batch_items; i++)
	{
		x_ij[i] = IloIntVarArray(env, batch_items);
		y_ij[i] = IloIntVarArray(env, batch_items);
		for (int l = 0; l < batch_items; l++)
		{
			x_ij[i][l] = IloIntVar(env, 0, 1);
			sprintf(nombre, "X_%d_%d", i, l);
			x_ij[i][l].setName(nombre);
			y_ij[i][l] = IloIntVar(env, 0, 1);

			sprintf(nombre, "Y_%d_%d", i, l);
			y_ij[i][l].setName(nombre);
		}

	}

	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/
//	printf("Aqui22 Temp_Conjuntos %d\n", Temp_Conjuntos.size());

	//Suma menor o igual que el valor

	IloExpr v1(env);
	IloExpr v3(env);
	for (int i = 0; i < batch_items; i++)
	{
		v1 += x_i[i] * items[i].Getitem_w();
		v1 += y_i[i] * items[i].Getitem_h();

		v3 += px_i[i];
		v3 += py_i[i];


		IloExpr v4(env);
		v4 += px_i[i] - (plate_h) * (plate_w)*x_i[i];
		restr.add(IloRange(env, -IloInfinity, v4, 0));
		v4.end();

		IloExpr v5(env);
		v5 += py_i[i] - (plate_h) * (plate_w)*y_i[i];
		restr.add(IloRange(env, -IloInfinity, v5, 0));
		v5.end();

	}
	restr.add(IloRange(env, -IloInfinity, v3, Best_Min_Perdida));
	v3.end();

	restr.add(IloRange(env, -IloInfinity, v1, Val));
	v1.end();

	for (int i = 0; i < batch_items; i++)
	{
		IloExpr v1(env);
		IloExpr v2(env);
		IloExpr v3(env);
		for (int j = 0; j < batch_items; j++)
		{

			if (i == j)
			{
				//				v1 +=  items[i].Getitem_area();
				//				v2 +=  items[i].Getitem_area();
				continue;
			}
			v3 += x_ij[j][i] + y_ij[j][i];
			if ((items[i].Getitem_stack() == items[j].Getitem_stack()) && (items[j].Getitem_seq() < items[i].Getitem_seq()))
				continue;
			if ((items[j].Getitem_min() <= items[i].Getitem_min() && items[j].Getitem_max() <= (plate_h - items[i].Getitem_h()))
				|| (items[j].Getitem_min() <= (plate_h - items[i].Getitem_h()) && items[j].Getitem_max() <= items[i].Getitem_w()))
				v1 += x_ij[i][j] * items[j].Getitem_area();
			if ((items[j].Getitem_min() <= items[i].Getitem_min() && items[j].Getitem_max() <= (plate_h - items[i].Getitem_w()))
				|| (items[j].Getitem_min() <= (plate_h - items[i].Getitem_w()) && items[j].Getitem_max() <= items[i].Getitem_h()))
				v2 += y_ij[i][j] * items[j].Getitem_area();

		}
		restr.add(IloRange(env, -IloInfinity, v3, 1));
		v3.end();
		v1 += (-1) * (plate_h - items[i].Getitem_h()) * items[i].Getitem_w() * x_i[i] + px_i[i];
		restr.add(IloRange(env, 0, v1, 0));
		v1.end();
		v2 += (-1) * (plate_h - items[i].Getitem_w()) * items[i].Getitem_h() * y_i[i] + py_i[i];
		restr.add(IloRange(env, 0, v2, 0));
		v2.end();

	}
	//Restricciones
	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
	model.add(restr);

	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);

	for (int i = 0; i < batch_items; i++)
	{
		//		Expression_fobj += x_l[i] * Temp_Conjuntos[i].number;
		Expression_fobj += x_i[i];
		Expression_fobj += y_i[i];
		//		printf("\n %d %d\n", i, Temp_Conjuntos[i].number);
	}
	//	printf("Aqui65\n");



	model.add(IloMaximize(env, Expression_fobj));
	Expression_fobj.end();
	//	printf("Aqui66\n");

		/***************************************************************/
		/*************** EJECUTAMOS EL MODELO **************************/
		/***************************************************************/
	try {
		//		printf("Aqui67\n");
		int value2 = 0;
		IloCplex cplex(model);
		printf("Aqui687\n");
		//Si queremos las lazy y tenemos alguna
//		if (Lazy==true && stack_nbr<batch_items)

	//	cplex.addLazyConstraints(restrLazy);
		//No hace el probing
		//cplex.setParam(IloCplex::Probe, -1);
		//1 CORE
		cplex.setParam(IloCplex::Threads, 8);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, G_Time); //TIEMPO EN SEGUNDOS

//		cplex.exportModel("Multiknapsack.lp");

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

		//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();
		//		printf("Aqui68");

		IloBool result = cplex.solve();

		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;
		//		printf("Aqui666\n");

				//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


			//			FILE* f = fopen("./estadomodelo.txt", "a+");
						//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
				printf("Infeasible\n");
				//			cplex.exportModel("Roadef.lp");
			}
			else
			{
				if (cplex.getStatus() == IloAlgorithm::Unbounded) printf("Unbounded\n");
				printf("\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}

		}
		else
		{
			//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			value2 = cplex.getObjValue();
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible

			if (cplex.getSolnPoolNsolns() > 0)
			{

				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
//				printf("Aqui26\n");
				for (int i = 0; i < batch_items; i++)
				{



					if (cplex.isExtracted(x_i[i]) && cplex.getValue(x_i[i]) > 0.001)
					{
						printf("%s %d w %d h %d s %d id %d\n", x_i[i].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack(), i);
						//						fprintf(f, "%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
					}
					if (cplex.isExtracted(y_i[i]) && cplex.getValue(y_i[i]) > 0.001)
					{
						printf("%s %d w %d h %d s %d id %d\n", y_i[i].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack(), i);
						//						fprintf(f, "%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
					}

				}
				//				printf("Aqui27\n");






			}
			//			printf("Aqui288");
						//}
		}
		printf("Aqui299");
		cplex.end();
		model.end();
		env.end();
		return value2;
	}
	catch (IloException & e)
	{
		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
	printf("Aqui666\n");
}

int Glass::BiKnapsackEntera(int Val)
{
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	NumVector x_l(env);
	NumVector y_l(env);
	/***************** INICIALIZO VARIABLES *******************/
/**********************************************************/
	//inicializo las variable y_l
	printf("Aqui21\n");
	char nombre[20];
	x_l = IloIntVarArray(env, batch_items, 0, 1);
	y_l = IloIntVarArray(env, batch_items, 0, 1);
	for (int l = 0; l < batch_items; l++)
	{
		sprintf(nombre, "X%d", l);
		x_l[l].setName(nombre);
		sprintf(nombre, "Y%d", l);
		y_l[l].setName(nombre);
	}


	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/
//	printf("Aqui22 Temp_Conjuntos %d\n", Temp_Conjuntos.size());

	//Suma menor o igual que el valor

	IloExpr v1(env);
	IloExpr v3(env);
	for (int i = 0; i < batch_items; i++)
	{
		v1 += x_l[i] * items[i].Getitem_w();
		v1 += y_l[i] * items[i].Getitem_h();

		v3 += x_l[i] * CalcularPerdidaPieza(i, items[i].Getitem_w(), items[i].Getitem_h());
		v3 += y_l[i] * CalcularPerdidaPieza(i, items[i].Getitem_h(), items[i].Getitem_w());

		IloExpr v2(env);
		v2 += x_l[i] + y_l[i];
		restr.add(IloRange(env, -IloInfinity, v2, 1));
		v2.end();
	}
	restr.add(IloRange(env, -IloInfinity, v3, Best_Min_Perdida));
	v3.end();

	restr.add(IloRange(env, -IloInfinity, v1, Val));
	v1.end();

	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
	model.add(restr);

	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);

	for (int i = 0; i < batch_items; i++)
	{
		//		Expression_fobj += x_l[i] * Temp_Conjuntos[i].number;
		Expression_fobj += x_l[i];
		Expression_fobj += y_l[i];
		//		printf("\n %d %d\n", i, Temp_Conjuntos[i].number);
	}
	//	printf("Aqui65\n");



	model.add(IloMaximize(env, Expression_fobj));
	Expression_fobj.end();
	//	printf("Aqui66\n");

		/***************************************************************/
		/*************** EJECUTAMOS EL MODELO **************************/
		/***************************************************************/
	try {
		//		printf("Aqui67\n");
		int value2 = 0;
		IloCplex cplex(model);
		printf("Aqui687\n");
		//Si queremos las lazy y tenemos alguna
//		if (Lazy==true && stack_nbr<batch_items)

	//	cplex.addLazyConstraints(restrLazy);
		//No hace el probing
		//cplex.setParam(IloCplex::Probe, -1);
		//1 CORE
		cplex.setParam(IloCplex::Threads, 8);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, G_Time); //TIEMPO EN SEGUNDOS

//		cplex.exportModel("Multiknapsack.lp");

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

		//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();
		//		printf("Aqui68");

		IloBool result = cplex.solve();

		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;
		//		printf("Aqui666\n");

				//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


			//			FILE* f = fopen("./estadomodelo.txt", "a+");
						//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
				printf("Infeasible\n");
				//			cplex.exportModel("Roadef.lp");
			}
			else
			{
				if (cplex.getStatus() == IloAlgorithm::Unbounded) printf("Unbounded\n");
				printf("\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}

		}
		else
		{
			//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			value2 = cplex.getObjValue();
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible

			if (cplex.getSolnPoolNsolns() > 0)
			{

				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
//				printf("Aqui26\n");
				for (int i = 0; i < batch_items; i++)
				{



					if (cplex.isExtracted(x_l[i]) && cplex.getValue(x_l[i]) > 0.001)
					{
						printf("%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack(), i);
						//						fprintf(f, "%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
					}
					if (cplex.isExtracted(y_l[i]) && cplex.getValue(y_l[i]) > 0.001)
					{
						printf("%s %d w %d h %d s %d id %d\n", y_l[i].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack(), i);
						//						fprintf(f, "%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
					}

				}
				//				printf("Aqui27\n");






			}
			//			printf("Aqui288");
						//}
		}
		printf("Aqui299");
		cplex.end();
		model.end();
		env.end();
		return value2;
	}
	catch (IloException & e)
	{
		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
	printf("Aqui666\n");
}

//Si el peso es el mismo que el ancho el knapsack es trivial
void Glass::knapsack_bouknap()
{

	//Se crean los vectores de enteros, profit es lo que ganamos
	//weight es el peso 
	//m cuántos de esa pieza
	//x sería la solución
	int max = 0, todas = 0;
	int* profit = (int*)calloc(batch_items, sizeof(int));
	int* weight = (int*)calloc(batch_items, sizeof(int));
	int* m = (int*)calloc(batch_items, sizeof(int));
	int* x = (int*)calloc(batch_items, sizeof(int));
	/*for (int i = 0; i < 150; i++)
	{
		weight[i] = 0;
		profit[i] = 0;
		m[i] = 0;
		x[i] = 0;
	}*/
	int sum_posible = 0;
	int val_posible = 0;
	for (int i = 0; i < batch_items; i++)
	{
		int d = items[i].Getitem_min();
		profit[i] = 1;
		weight[i] = (int)d;
		//en este caso todos sería de 1
		m[i] = 1;
		val_posible += profit[i];
		sum_posible += weight[i];

	}
	printf("Aqui 1");


	if (sum_posible <= plate_w)
		maxTiras = val_posible;
	else maxTiras = bouknap(batch_items, profit, weight, m, x, plate_w);
	if (sum_posible <= plate_h)
		maxPilas = val_posible;
	else maxPilas = bouknap(batch_items, profit, weight, m, x, plate_h);
	if (sum_posible <= max1Cut)
		maxPiezasPila = val_posible;
	else maxPiezasPila = bouknap(batch_items, profit, weight, m, x, max1Cut);
	if (P_maxnivelesh == 0 || P_maxnivelesh > maxPilas)
		P_maxnivelesh = maxPilas;
	if (P_maxnivelesw == 0 || P_maxnivelesw > maxTiras)
		P_maxnivelesw = maxTiras;
	if (P_maxnivelesm == 0 || P_maxnivelesm > maxPiezasPila)
		P_maxnivelesm = maxPiezasPila;
	printf("Aqui 2");
	//z es lo máximo
	//en x ahora tendría cuáles escoge


	printf("Máximo Tiras: Tiras=%d\t Pilas=%d \tPiezas=%d\n", maxTiras, maxPilas, maxPiezasPila);
}

void Glass::CalculoMaximoPilasNivelesEasy()
{
	//	CrearVectorOrdenado();

	std::vector<int> Alturas;
	std::vector<int> Anchuras;
	for (int i = 0; i < batch_items; i++)
	{
		if (P_WithoutRotation)
		{
			Alturas.push_back(items[i].Getitem_h());
			Anchuras.push_back(items[i].Getitem_w());
		}
		else
			Alturas.push_back(items[i].Getitem_min());
	}
	std::sort(Alturas.begin(), Alturas.end());
	std::sort(Anchuras.begin(), Anchuras.end());
	//Acumulado
	for (int i = 0; i < batch_items; i++)
	{
		int suma = 0;
		for (int i2 = 0; i2 <= i; i2++)
		{
			suma += Alturas[i2];
		}
		G_Acumulado_Min.push_back(suma);
	}
	//Cota trivial
	int sumah = 0;
	int sumaw = 0;
	int cuantosh = 0;
	int cuantosw = 0;
	int cuantosm = 0;
	std::set<int> ::iterator its;
	int Max = plate_w;
	if (Best_plates_nbr == 1) Max = Best_Width;
	for (int i = 0; i < batch_items; i++)
	{
		if (P_WithoutRotation)
		{
			sumah += Alturas[i];
			sumaw += Anchuras[i];
		}
		else
		{
			sumah += Alturas[i];
			sumaw += Alturas[i];
		}
		if (sumah <= plate_h)
		{
			//En altura está el minimo de esa pieza

			cuantosh++;
		}
		if (sumaw <= min(plate_w, Max))
		{

			cuantosw++;
		}
		if (sumaw <= min(max1Cut, Max))
		{

			cuantosm++;
		}
	}
	if (P_maxnivelesh == 0 || P_maxnivelesh > cuantosh)
		P_maxnivelesh = cuantosh;
	if (P_maxnivelesw == 0 || P_maxnivelesw > cuantosw)
		P_maxnivelesw = cuantosw;
	if (P_maxnivelesm == 0 || P_maxnivelesm > cuantosm)
		P_maxnivelesm = cuantosm;

}

void Glass::CalculoMaximoPilasNivelesKnapsack(int Val)
{
	//Crear 

	std::vector< Item_orden> Temp_Conjuntos;
	for (int i = 0; i < batch_items; i++)
	{
		Item_orden p;
		p.rotado = false;
		p.anchura = items[i].Getitem_w();
		p.altura = items[i].Getitem_min();
		p.item_id = i;
		p.stack_nbr = items[i].Getitem_stack();
		p.number = 1;
		Temp_Conjuntos.push_back(p);
		int altura = items[i].Getitem_min();
		int numero = 1;
		if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1)
		{
			for (int j = i + 1; j < batch_items && items[i].Getitem_stack() == items[j].Getitem_stack() && altura < Val; j++)
			{

				altura += items[j].Getitem_min();
				if (altura > Val)
					continue;
				numero += 1;
				Item_orden p1;
				p1.rotado = false;
				p1.anchura = altura;
				p1.altura = altura;
				p1.item_id = j + 1000;
				p1.stack_nbr = items[j].Getitem_stack();
				p1.number = numero;
				Temp_Conjuntos.push_back(p1);


			}
		}
	}
	printf("AquiKnp\n");
	//Cota más compleja mediante un knapsack
	//hay que poner una variable por pieza y por pares, triples, etc de consecutivos 
	//que quepan en la tira y añadir que de cada pila solamente puede haber uno

	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	NumVector x_l(env);
	/***************** INICIALIZO VARIABLES *******************/
/**********************************************************/
	//inicializo las variable y_l
	printf("Aqui21\n");
	char nombre[20];
	x_l = IloIntVarArray(env, Temp_Conjuntos.size(), 0, 1);
	for (int l = 0; l < Temp_Conjuntos.size(); l++)
	{
		sprintf(nombre, "X%d", l);
		x_l[l].setName(nombre);
	}


	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/
	printf("Aqui22 Temp_Conjuntos %d\n", Temp_Conjuntos.size());

	//Suma menor o igual que el valor

	IloExpr v1(env);
	for (int i = 0; i < Temp_Conjuntos.size(); i++)
	{
		v1 += x_l[i] * Temp_Conjuntos[i].altura;
	}

	restr.add(IloRange(env, -IloInfinity, v1, Val));
	v1.end();
	//Solamente una de cada grupo de piezas de cada stacks
	printf("Aqui23\n");

	for (int i = 0; i < stack_nbr; i++)
	{
		if (stacks[i].Getitem_nbr() > 1)
		{
			IloExpr v1(env);

			for (int j = 0; j < Temp_Conjuntos.size(); j++)
			{
				if (Temp_Conjuntos[j].stack_nbr == stacks[i].Getstack_id())
					v1 += x_l[j];
			}
			restr.add(IloRange(env, -IloInfinity, v1, 1));
			v1.end();
		}

	}
	printf("Aqui43\n");
	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
	model.add(restr);

	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);

	for (int i = 0; i < Temp_Conjuntos.size(); i++)
	{
		Expression_fobj += x_l[i] * Temp_Conjuntos[i].number;
		//		printf("\n %d %d\n", i, Temp_Conjuntos[i].number);
	}
	//	printf("Aqui65\n");



	model.add(IloMaximize(env, Expression_fobj));
	Expression_fobj.end();
	//	printf("Aqui66\n");

		/***************************************************************/
		/*************** EJECUTAMOS EL MODELO **************************/
		/***************************************************************/
	try {
		//		printf("Aqui67\n");
		IloCplex cplex(model);
		printf("Aqui687\n");
		//Si queremos las lazy y tenemos alguna
//		if (Lazy==true && stack_nbr<batch_items)

	//	cplex.addLazyConstraints(restrLazy);
		//No hace el probing
		//cplex.setParam(IloCplex::Probe, -1);
		//1 CORE
		cplex.setParam(IloCplex::Threads, 8);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, G_Time); //TIEMPO EN SEGUNDOS

//		cplex.exportModel("RoadefNumberLevels.lp");

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

		//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();
		//		printf("Aqui68");

		IloBool result = cplex.solve();

		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;
		//		printf("Aqui666\n");

				//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


			//			FILE* f = fopen("./estadomodelo.txt", "a+");
						//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
				printf("Infeasible\n");
				//			cplex.exportModel("Roadef.lp");
			}
			else
			{
				if (cplex.getStatus() == IloAlgorithm::Unbounded) printf("Unbounded\n");
				printf("\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}

		}
		else
		{
			//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			Best_Value_Formulation = cplex.getObjValue();
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible
			if (Val == plate_h)
				P_maxnivelesh = Best_Value_Formulation;

			if (Val == plate_w)
				P_maxnivelesw = Best_Value_Formulation;
			if (cplex.getSolnPoolNsolns() > 0)
			{
				int value2 = cplex.getObjValue();
				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
//				printf("Aqui26\n");
				for (int i = 0; i < Temp_Conjuntos.size(); i++)
				{



					if (cplex.isExtracted(x_l[i]) && cplex.getValue(x_l[i]) > 0.001)
					{
						printf("%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
						//						fprintf(f, "%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
					}


				}
				//				printf("Aqui27\n");






			}
			//			printf("Aqui288");
						//}
		}
		printf("Aqui299");
		cplex.end();
		model.end();
		env.end();
	}
	catch (IloException & e)
	{
		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
	printf("Aqui666\n");
}
void Glass::FormulationStagesOrig(short int nbins, short int nt)
{
	CrearVectorOrdenado();
	int kk = 9;
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	IloRangeArray restrLazy(env);
	int maxtime = G_Time;
	IloNum max_time = maxtime; //tiempo del modelo

	char nombre[20];

//	knapsack_bouknap();
	//	Creo las variables
	numtiras = P_maxnivelesw;
	int numpieces = batch_items;
	numbins = nbins;
//	if (P_initial_solution)
//		numbins = numbins * 2;
	P_pilas = true;
	int Tipo_objetivo = 2;
	if (Tipo_objetivo == 3)
		numbins = 1;
	NumVarMatrix2 x_ij(env, 2 * numpieces);
	//	NumVarMatrix2 xr_ij(env, 2 * numpieces);
	NumVarMatrix3 z_kli(env, numbins);


	NumVarMatrix2 y_kl(env, numbins);
	NumVarMatrix2 w_kl(env, numbins);
	NumVector y_l(env);
	NumVector w_l(env);

	IloNumVarArray vars(env);
	IloNumArray vals(env);
	int LastBinWidth = max((double)floor((Total_area - ((numbins - 1) * plate_h * plate_w)) / plate_h), (double)0);
	//	LastBinWidth = 0;
	int maxh = 0;
	IloIntVar NbrPilas;

	/**********************************************************/
	/***************** INICIALIZO VARIABLES *******************/
	/**********************************************************/
	//inicializo la variable x_ijkl
	for (int i = 0; i < 2 * numpieces; i++)
	{

		x_ij[i] = IloIntVarArray(env, 2 * numpieces);
		for (int j = i; j < 2 * numpieces; j++)
		{

			x_ij[i][j] = IloIntVar(env, 0, 1);
			sprintf(nombre, "x_%d_%d", i, j);
			x_ij[i][j].setName(nombre);

		}
	}

	//inicializo variables bin
	for (int i = 0; i < numbins; i++)
	{
		z_kli[i] = NumVarMatrix(env, numtiras);

		for (int l = 0; l < numtiras; l++)
		{
			//	{
			z_kli[i][l] = IloIntVarArray(env, 2 * numpieces);
			for (int j = 0; j < 2 * numpieces; j++)
			{
				z_kli[i][l][j] = IloIntVar(env, 0, 1);
				sprintf(nombre, "z_%d_%d_%d", i, l, j);
				z_kli[i][l][j].setName(nombre);

			}
		}
	}
	//inicializo la variable h_jkl
	//for (int i = 0; i < numpilas; i++)
	//{
	//	h_jkl[i] = NumVarMatrix(env, numtiras);

	//	for (int j = 0; j < numtiras; j++)
	//	{
	//		h_jkl[i][j] = IloIntVarArray(env, numbins);
	//		for (int l = 0; l < numbins; l++)
	//		{
	//			h_jkl[i][j][l] = IloIntVar(env, 0, maxh);
	//			sprintf(nombre, "h_%d_%d_%d", i, j,l);
	//			h_jkl[i][j][l].setName(nombre);
	//
	//		}

	//	}
	//}
	//inicializo la variable w_kl y y_kl
	for (int i = 0; i < numbins; i++)
	{
		w_kl[i] = IloIntVarArray(env, numtiras);
		y_kl[i] = IloIntVarArray(env, numtiras);
		for (int j = 0; j < numtiras; j++)
		{
			w_kl[i][j] = IloIntVar(env, 0, max1Cut);
			sprintf(nombre, "W_%d_%d", i, j);
			w_kl[i][j].setName(nombre);
			y_kl[i][j] = IloIntVar(env, 0, 1);
			sprintf(nombre, "Y_%d_%d", i, j);
			y_kl[i][j].setName(nombre);

		}
	}

	//inicializo las variable y_l
	y_l = IloIntVarArray(env, numbins, 0, 1);
	for (int l = 0; l < numbins; l++)
	{
		sprintf(nombre, "Y%d", l);
		y_l[l].setName(nombre);
	}
	if (Tipo_objetivo == 3)
	{
		//inicializo las variable y_l
		w_l = IloIntVarArray(env, P_maxnivelesw, 0, 1);
		for (int l = 0; l < P_maxnivelesw; l++)
		{
			sprintf(nombre, "W%d", l);
			w_l[l].setName(nombre);
		}
	}

	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/


	//Todos los productos colocados
	for (int j = 0; j < 2 * numpieces; j++)
	{
		int id = G_Vector_Ordenado_Altura[j].item_id;
		int counterpart = 0;
		for (int j1 = 0; j1 < 2 * numpieces; j1++)
		{
			if (j1 == j) continue;
			if (G_Vector_Ordenado_Altura[j1].item_id == id)
			{
				counterpart = j1;
			}
		}
		if (counterpart > j)
		{
			IloExpr v1(env);
			for (int i = 0; i <= j; i++)
			{

				v1 += x_ij[i][j];
				//			v1 += xr_ij[i][j];


			}

			for (int i = 0; i <= counterpart; i++)
			{

				v1 += x_ij[i][counterpart];
				//			v1 += xr_ij[i][j];


			}

			if (Tipo_objetivo != 3)
				restr.add(IloRange(env, 1, v1, 1));
			else
				restr.add(IloRange(env, 0, v1, 1));
			v1.end();
		}

	}

	//Todos los niveles asignados a una tira
	for (int j = 0; j < 2 * numpieces; j++)
	{
		IloExpr v1(env);
		for (int k = 0; k < numbins; k++)
		{

			for (int l = 0; l < numtiras; l++)
			{
				v1 += z_kli[k][l][j];
			}
		}
		v1 += (-1) * x_ij[j][j];
		//		v1 += (-1) * xr_ij[j][j];
		restr.add(IloRange(env, 0, v1, 0));
	}
	//anchuras de las tiras

	for (int i = 0; i < (2 * numpieces - 1); i++)
	{
		IloExpr v1(env);
		for (int j = i; j < 2 * numpieces; j++)
		{

			v1 += G_Vector_Ordenado_Altura[j].anchura * x_ij[i][j];
			//			v1 += xr_ij[i][j];


		}
		v1 += (-3500) * x_ij[i][i];
		//		if (Tipo_objetivo != 3)
		restr.add(IloRange(env, -IloInfinity, v1, 0));
		//		else
			//		restr.add(IloRange(env, 0, v1, 1));

		v1.end();
	}

	//anchuras de las tiras

	for (int k = 0; k < numbins; k++)
	{
		for (int l = 0; l < numtiras; l++)
		{
			IloExpr v2(env);
			IloExpr v3(env);
			for (int i = 0; i < (2 * numpieces); i++)
			{

				IloExpr v1(env);

				for (int j = i; j < 2 * numpieces; j++)
				{
					v1 += G_Vector_Ordenado_Altura[j].anchura * x_ij[i][j];
					//					v1 +=(-1)*  G_Vector_Ordenado_Altura[j].anchura ;
										//			
					//					v1 +=  G_Vector_Ordenado_Altura[j].anchura * z_kli[k][l][i];
					//						v1 += items[G_Vector_Ordenado_Altura[i].item_id].Getitem_h() * xr_ij[i][j];



				}

				v1 += (-1) * w_kl[k][l];
				v1 += (-1) * max1Cut;
				v1 += z_kli[k][l][i] * max1Cut;
				v2 += (-1) * z_kli[k][l][i];
				v3+= z_kli[k][l][i];
				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();

			}
			if (Tipo_objetivo == 3)
			{
				
				IloExpr v1(env);
				v1 += w_kl[k][l];
				v1 += (-1) * max1Cut * w_l[l];
				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
				if (P_tiras)
				{
					v2 += w_l[l];
					restr.add(IloRange(env, -IloInfinity, v2, 0));
					v2.end();
				}
			}

		}
	}

	if (Tipo_objetivo == 3)
	{
		IloExpr v1(env);

		for (int j = 0; j < 2 * numpieces; j++)
		{
			int id = G_Vector_Ordenado_Altura[j].item_id;
			int counterpart = 0;
			for (int j1 = 0; j1 < 2 * numpieces; j1++)
			{
				if (j1 == j) continue;
				if (G_Vector_Ordenado_Altura[j1].item_id == id)
				{
					counterpart = j1;
				}
			}
			if (counterpart > j)
			{
				for (int i = 0; i <= j; i++)
				{

					v1 += G_Vector_Ordenado_Altura[j].altura * G_Vector_Ordenado_Altura[j].anchura* x_ij[i][j];
					//			v1 += xr_ij[i][j];


				}

				for (int i = 0; i <= counterpart; i++)
				{

					v1 += G_Vector_Ordenado_Altura[j].altura* G_Vector_Ordenado_Altura[j].anchura *x_ij[i][counterpart];
					//			v1 += xr_ij[i][j];


				}

			}

		}
		if (Best_plates_nbr<=1)
		v1 += (-1) * (plate_h * Best_Width - Best_Min_Perdida);
		else
			v1 += (-1) * ((plate_h * plate_w) - Best_Min_Perdida);

		restr.add(IloRange(env, 0, v1, IloInfinity));
		v1.end();
	}
	IloExpr v1(env);
	for (int k = 0; k < numtiras; k++)
	{

		v1 += w_kl[numbins - 1][k];


	}

	v1 += (-1) * LastBinWidth;
	restr.add(IloRange(env, 0, v1, IloInfinity));
	v1.end();
	if (Tipo_objetivo == 3)
	{
	IloExpr v1(env);
	for (int k = 0; k < numtiras; k++)
	{

		v1 += w_kl[numbins - 1][k];


	}
	if (Best_plates_nbr>1)
	v1 += (-1) * plate_w;
	else
		v1 += (-1) * Best_Width;
	restr.add(IloRange(env, -IloInfinity, v1, 0));
	v1.end();
	}
	//Anchura de la tira
	for (int k = 0; k < numbins; k++)
	{
		IloExpr v1(env);
		for (int l = 0; l < numtiras; l++)
		{

			v1 += w_kl[k][l];

		}

		v1 += (-1) * plate_w * y_l[k];
		restr.add(IloRange(env, -IloInfinity, v1, 0));
		v1.end();
	}

	//Altura mínima
	for (int k = 0; k < numbins; k++)
	{
		for (int l = 0; l < numtiras; l++)
		{
			IloExpr v1(env);
			for (int i = 0; i < 2 * numpieces; i++)
			{
				v1 += G_Vector_Ordenado_Altura[i].altura * z_kli[k][l][i];

			}
			v1 += (-1) * plate_h;
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
		}

	}
	//Variable resto
	IloIntVar Rm(env, 0, plate_w);
	Rm.setName("Rm");
	if (P_NewObjective_Rm)
	{

		//orders bins
		for (int l = 0; l < numbins - 1; l++)
		{
			IloExpr v1(env);
			v1 += y_l[l];
			v1 += (-1) * y_l[l + 1];
			restr.add(IloRange(env, 0, v1, IloInfinity));
			v1.end();
		}
		//Variables relacionadas con la Rm
		for (int l = 0; l < numbins; l++)
		{
			IloExpr v1(env);
			v1 += (-1) * Rm;
			if (l < (numbins - 1))
				v1 += (-1) * plate_w * y_l[l + 1];
			for (int k = 0; k < numtiras; k++)
			{
				v1 += w_kl[l][k];
			}
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
		}
		IloExpr v3(env);
		for (int i = 0; i < numbins; i++)
		{
			v3 += plate_w * y_l[i];
		}
		v3 += (-1) * plate_w;
		v3 += Rm;
		restr.add(IloRange(env, floor(Total_area / plate_h), v3, IloInfinity));
		v3.end();
	}
/*
	if (P_orders)
	{
		for (int k = 0; k < numbins; k++)
		{

			for (int l = 0; l < numtiras; l++)
			{

				for (int i = numpieces - 1; i > 0; i--)
				{

					//Si es la primera de su stacks no hace falta
					if ((items[i].Getitem_seq() > 1) && ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1))
					{
						IloExpr v1(env);
						for (int i1 = 0; i1 < (2 * numpieces); i1++)
						{
							if (G_Vector_Ordenado_Altura[i1].item_id == items[i].Getitem_id())
								v1 += z_kli[k][l][i1];
						}


						int i2 = i - 1;
						if (i2 >= 0 && items[i2].Getitem_stack() == items[i].Getitem_stack())
						{

							for (int k2 = 0; k2 <= k; k2++)
							{
								for (int l2 = 0; l2 < numtiras; l2++)
								{
									if (k2 == k && l2 > l)
										break;
									for (int i1 = 0; i1 < (2 * numpieces); i1++)
									{
										if (G_Vector_Ordenado_Altura[i1].item_id == items[i2].Getitem_id())
											v1 +=(-1)* z_kli[k2][l2][i1];
									}
									
								}
							}
						}

						
						restr.add(IloRange(env, -IloInfinity, v1, 0));
						v1.end();
					}

				}
			}


		}
	}
	*/
	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
	model.add(restr);


	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);
	if (Tipo_objetivo == 0)//biobjective //estaría en diet.ccp
	{
		IloExpr Expression_fobj1(env);
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj1 += y_l[i];
		}
		IloExpr Expression_fobj2(env);

		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj2 += w_kl[numbins - 1][k];
		}
	}

	if (Tipo_objetivo == 1)//por bins
	{
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj += y_l[i];
		}
	}
	if (Tipo_objetivo == 2)//por tira del último bin
	{

		if (Tipo_objetivo == 2)//por tira del último bin
		{
			if (P_NewObjective_Rm)
			{
				for (int i = 0; i < numbins; i++)
				{
					Expression_fobj += plate_w * y_l[i];
				}
				Expression_fobj += (-1) * plate_w;
				Expression_fobj += Rm;

			}
			else
			{
				for (int k = 0; k < numtiras; k++)
				{
					Expression_fobj += w_kl[k][numbins - 1];
				}
			}
		}
	}
	if (Tipo_objetivo == 3)//máximo de tiras
	{
	/*	if (P_sections)
		{

			for (int i = 0; i < (2 * numpieces); i++)
			{
				Expression_fobj += z_kli[0][i][i];
			}

		}*/

		if (P_pilas)
		{

			for (int i = 0; i < (2 * numpieces); i++)
			{
				Expression_fobj += z_kli[0][0][i];
			}

		}
		if (P_tiras)
		{
			for (int k = 0; k < numtiras; k++)
			{
				Expression_fobj += w_l[k];
			}
		}
	}

	if (Tipo_objetivo != 0 && Tipo_objetivo!=3)
		model.add(IloMinimize(env, Expression_fobj));
	if (Tipo_objetivo==3)
		model.add(IloMaximize(env, Expression_fobj));
	Expression_fobj.end();


	/***************************************************************/
	/*************** EJECUTAMOS EL MODELO **************************/
	/***************************************************************/
	try {

		IloCplex cplex(model);
		//Si queremos las lazy y tenemos alguna
//		if (Lazy==true && stack_nbr<batch_items)
	//	cplex.addLazyConstraints(restrLazy);
		//No hace el probing
		//cplex.setParam(IloCplex::Probe, -1);
		//1 CORE
		cplex.setParam(IloCplex::Threads, nt);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo

		cplex.setParam(IloCplex::TiLim, max_time); //TIEMPO EN SEGUNDOS
		if (Tipo_objetivo == 2)
			cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, LastBinWidth);
		if (G_Dibujar)
			cplex.exportModel("RoadefStageOrig.lp");

			//CPXsetintparam(env,CPX_PARAM_THREADS,1);
		if (P_NewObjective_Rm)
			cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, floor(Total_area / plate_h));
		if (!P_initial_solution)
			cplex.setParam(IloCplex::MIPEmphasis, 1);
			//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();
		if (P_initial_solution)
		{
	
			FILE* file2;
			//	strcat(name_instance, "_solution.csv");

			file2 = fopen(P_file_to_write.c_str(), "r+");
			if (file2 == NULL)
			{
				printf("Problemas al leer el archivo %s, no se encuentra el archivo", P_file_to_write.c_str());
				exit(4);
			}
			int PLATE_ID = 0;
			int	NODE_ID = 0;
			int	X = 0;
			int Y = 0;
			int	WIDTH = 0;
			int	HEIGHT = 0;
			int TYPE = 0;
			int CUT = 0;
			int PARENT = 0;
			char kk[256];
			fscanf(file2, "%s\n", kk);
			fscanf(file2, "%d;%d;%d;%d;%d;%d;%d;%d;", &PLATE_ID, &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT);
			int section = -1;
			int level = -1;
			int stripe = -1;
			int plate = 0;
			std::set<int> items_nivel;
			while (!feof(file2) && CUT != (-3))
			{

				fscanf(file2, "%d;", &PLATE_ID);
				if (PLATE_ID == plate)
					fscanf(file2, "%d;%d;%d;%d;%d;%d;%d;%d", &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT, &PARENT);
				else
				{
					int inicio = -1;
					for (std::set<int> ::iterator it = items_nivel.begin(); it != items_nivel.end(); it++)
					{
						if (it == items_nivel.begin())
						{
							inicio = (*it);
							vars.add(x_ij[(*it)][(*it)]);
							printf("%s %d\n", x_ij[inicio][(*it)].getName(), 1);


							vals.add(1.0);
							//								vars.add(z_kli[][][(*it)]);
						}
						else
						{
							vars.add(x_ij[inicio][(*it)]);
							printf("%s %d\n", x_ij[inicio][(*it)].getName(), 1);

							vals.add(1.0);
							//								vars.add(z_kli[][][(*it)]);
						}

					}



					if (items_nivel.size() > 0)
					{
						vars.add(z_kli[plate][stripe][inicio]);

						vals.add(1.0);
						printf("%s %d\n", z_kli[plate][stripe][inicio].getName(), 1);
					}
					items_nivel.clear();
					plate = PLATE_ID;
					stripe = -1;
					fscanf(file2, "%s\n", kk);
					continue;
				}
				plate = PLATE_ID;
				if (CUT >= 4 && TYPE < 0) continue;
				if (CUT == 1)
				{
					int inicio = -1;
					for (std::set<int> ::iterator it = items_nivel.begin(); it != items_nivel.end(); it++)
					{
						if (it == items_nivel.begin())
						{
							inicio = (*it);
							vars.add(x_ij[(*it)][(*it)]);
							printf("%s %d\n", x_ij[inicio][(*it)].getName(), 1);

							vals.add(1.0);
							//								vars.add(z_kli[][][(*it)]);
						}
						else
						{
							vars.add(x_ij[inicio][(*it)]);
							printf("%s %d\n", x_ij[inicio][(*it)].getName(), 1);

							vals.add(1.0);
							//								vars.add(z_kli[][][(*it)]);
						}

					}

					

					if (items_nivel.size() > 0)
					{
						vars.add(z_kli[plate][stripe][inicio]);

						vals.add(1.0);
						printf("%s %d\n", z_kli[plate][stripe][inicio].getName(), 1);
					}
					items_nivel.clear();
					stripe++;
					level = -1;
					section = 0;
					if (TYPE < 0)
						continue;
				}
				if (CUT == 2)
				{
					int inicio = -1;
					for (std::set<int> ::iterator it = items_nivel.begin(); it != items_nivel.end(); it++)
					{
						if (it == items_nivel.begin())
						{
							inicio = (*it);
							vars.add(x_ij[(*it)][(*it)]);
							printf("%s %d\n", x_ij[(*it)][(*it)].getName(), 1);
							vals.add(1.0);
							//								vars.add(z_kli[][][(*it)]);
						}
						else
						{
							vars.add(x_ij[inicio][(*it)]);
							printf("%s %d\n", x_ij[inicio][(*it)].getName(), 1);

							vals.add(1.0);
							//								vars.add(z_kli[][][(*it)]);
						}

					}
					if (items_nivel.size() > 0)
					{
						vars.add(z_kli[plate][stripe][inicio]);

						vals.add(1.0);
						printf("%s %d\n", z_kli[plate][stripe][inicio].getName(), 1);

					}
					items_nivel.clear();


					level++;
					section = 0;
					if (TYPE < 0)
						continue;
				}
				if (CUT == 3)
				{

					if (TYPE < 0)
					{
						if (TYPE == (-10))//cambiado por 1
							section++;
						continue;
					}

				}
				items_nivel.insert(BuscarPosicion(TYPE, HEIGHT));
				if (CUT == 3 || CUT == 4)
				{

					section++;

					
				}
				//		fscanf(fin2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &W, &H, &plates, &piezas, &defectos, &plate_defectos, &obj, &width_total);
			}
			cplex.addMIPStart(vars, vals, cplex.MIPStartRepair);

		}
		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();


		IloBool result = cplex.solve();

		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;

		//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
//				fprintf(f, "Infeasible\n");
				//				cplex.exportModel("Roadef.lp");
			}
			else
			{
//				if (cplex.getStatus() == IloAlgorithm::Unbounded) fprintf(f, "Unbounded\n");
//				fprintf(f, "\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}
			LB_Best_Value_Formulation = cplex.getBestObjValue();
//			fclose(f);
		}
		else
		{
//			FILE* f = fopen("./estadomodelo.txt", "a+");
//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			Best_Value_Formulation = cplex.getObjValue();
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible
			if (cplex.getSolnPoolNsolns() > 0)
			{
				int value2 = cplex.getObjValue();
				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ij

				for (int i = 0; i < 2 * numpieces; i++)
				{
					for (int j = i; j < 2 * numpieces; j++)
					{


						if (cplex.isExtracted(x_ij[i][j]) && cplex.getValue(x_ij[i][j]) > 0.001)
						{
							printf("%s %d w %d h %d s %d id %d\n", x_ij[i][j].getName(), 1, items[G_Vector_Ordenado_Altura[i].item_id].Getitem_w(), items[G_Vector_Ordenado_Altura[i].item_id].Getitem_h(), items[G_Vector_Ordenado_Altura[i].item_id].Getitem_stack(), G_Vector_Ordenado_Altura[i].item_id);
//							fprintf(f, "%s %d w %d h %d s %d id %d\n", x_ij[i][j].getName(), 1, items[G_Vector_Ordenado_Altura[i].item_id].Getitem_w(), items[G_Vector_Ordenado_Altura[i].item_id].Getitem_h(), items[G_Vector_Ordenado_Altura[i].item_id].Getitem_stack(), G_Vector_Ordenado_Altura[i].item_id);
						}

					}

				}

				//Pinto las variables z_kli
				for (int k = 0; k < numbins; k++)
				{
					for (int l = 0; l < numtiras; l++)
					{
						for (int j = 0; j < 2 * numpieces; j++)
						{

							if (cplex.isExtracted(z_kli[k][l][j]) && cplex.getValue(z_kli[k][l][j]) > 0.001)
							{
								printf("%s %2.f\n", z_kli[k][l][j].getName(), cplex.getValue(z_kli[k][l][j]));
//								fprintf(f, "%s %2.f\n", z_kli[k][l][j].getName(), cplex.getValue(z_kli[k][l][j]));
							}

						}

					}
				}
				//Pinto las variables w_kl y y_kl
				for (int k = 0; k < numbins; k++)
				{
					for (int l = 0; l < numtiras; l++)
					{

						if (cplex.isExtracted(w_kl[k][l]) && cplex.getValue(w_kl[k][l]) > 0.001)
						{
							printf("%s %2.f\n", w_kl[k][l].getName(), cplex.getValue(w_kl[k][l]));
//							fprintf(f, "%s %2.f\n", w_kl[k][l].getName(), cplex.getValue(w_kl[k][l]));
						}
						if (cplex.isExtracted(y_kl[k][l]) && cplex.getValue(y_kl[k][l]) > 0.001)
						{
							printf("%s %d\n", y_kl[k][l].getName(), 1);
//							fprintf(f, "%s %d\n", y_kl[k][l].getName(), 1);
						}

					}
				}

				//Pinto las variable y_l
				for (int l = 0; l < numbins; l++)
				{
					if (cplex.isExtracted(y_l[l]) && cplex.getValue(y_l[l]) > 0.001)
					{
						printf("%s %d\n", y_l[l].getName(), 1);
//						fprintf(f, "%s %d\n", y_l[l].getName(), 1);
					}

				}
				//estoy en el optimo, leo las variables y continuo
				if (relativeGap == 0)
				{
					int kk = 9;
				}
				if (G_Dibujar || G_Draw_Exacto)
				{
					FILE* fin3;
					if (G_Draw_Exacto)
					{
						std::string str = file_idx + "_solution_" + std::to_string(G_Exacto) + "_" + std::to_string(Tipo_objetivo) + "_" + std::to_string(Duplicated) +".txt";
						fin3=fopen(P_file_to_write.c_str(), "w+");
//						fin3 = fopen(str.c_str(), "w+");
					}
					else
						fin3 = fopen("./temp2.txt", "w+");
					int ndefects = 0;
					int nbinsdefects = 0;
					//					fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, Best_Objective_function);
					fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, numpieces, ndefects, nbinsdefects, LB_Best_Value_Formulation, Best_Value_Formulation);
					//DibujarStageOrig
					for (int k = 0; k < numbins; k++)
					{
						Best_Solution_Exacto = 0;
						int pos_x = 0;
						int pos_y = 0;
						int val = 0;
						for (int l = 0; l < numtiras; l++)
						{
							bool primera = false;
							
							pos_y = 0;
							for (int j = 0; j < 2 * numpieces; j++)
							{
								if (cplex.isExtracted(z_kli[k][l][j]) && cplex.getValue(z_kli[k][l][j]) > 0.001)
								{
									//					items[G_Vector_Ordenado_Altura[i].item_id].Getitem_w(), items[G_Vector_Ordenado_Altura[i].item_id].Getitem_h(), items[G_Vector_Ordenado_Altura[i].item_id].Getitem_stack(), G_Vector_Ordenado_Altura[i].item_id
	//									pos_y2 += G_Vector_Ordenado_Altura[j].altura;
									val = G_Vector_Ordenado_Altura[j].altura;
								}
								else continue;

	//							pos_x = pos_x2;
	//							pos_y = pos_y2;
								//Primero la de abajo, 
								int val_alto = 0;
								int pos_x_tira = pos_x;
								for (int i = j; i < 2 * numpieces; i++)
								{
									//cambia la altura
									if (i == j)
									{
										val_alto= G_Vector_Ordenado_Altura[i].altura;
										pos_x_tira = pos_x;
										if (pos_x_tira > Best_Solution_Exacto)
											Best_Solution_Exacto = pos_x;
									}

									if (cplex.isExtracted(x_ij[j][i]) && cplex.getValue(x_ij[j][i]) > 0.001)
									{
										int pieza = G_Vector_Ordenado_Altura[i].item_id;
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", pieza, items[pieza].Getitem_stack(), items[pieza].Getitem_seq(), pos_x_tira, pos_y, pos_x_tira + G_Vector_Ordenado_Altura[i].anchura, pos_y + G_Vector_Ordenado_Altura[i].altura, k + 1);
										pos_x_tira+= G_Vector_Ordenado_Altura[i].anchura;
										if (pos_x_tira > Best_Solution_Exacto)
											Best_Solution_Exacto = pos_x;
									}
									
									

								}
								pos_y += val_alto;


							}
							if (cplex.isExtracted(w_kl[k][l]) && cplex.getValue(w_kl[k][l]) > 0.001)
							{
									pos_x += cplex.getValue(w_kl[k][l]);
									if (pos_x > Best_Solution_Exacto)
										Best_Solution_Exacto = pos_x;
							}


						}

					}
					fclose(fin3);
					DibujarOpenGL();


				}

				//}
			}

			cplex.end();
			model.end();
			env.end();
		}
	}
	catch (IloException & e)
	{
		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
}
int Glass::BuscarPosicion(int id, int height)
{

	for (int i = 0; i < 2*batch_items; i++)
	{
		if (G_Vector_Ordenado_Altura[i].item_id == id &&
			G_Vector_Ordenado_Altura[i].altura == height)
			return i;
	}
	return -1;
}
void Glass::FormulationStages(short int nbins, short int nt)
{
	int kk = 9;
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	IloRangeArray restrLazy(env);
	int maxtime = G_Time;
	IloNum max_time = maxtime; //tiempo del modelo

	char nombre[20];

	knapsack_bouknap();
	//	Creo las variables

	numtiras = 4;
	numpilas = 4;
	int numpieces = batch_items;
	numbins = nbins;

	int Tipo_objetivo = 2;

	NumVarMatrix4 x_ijkl(env, numpieces);
	NumVarMatrix4 xr_ijkl(env, numpieces);
	NumVarMatrix2 y_kl(env, numtiras);
	NumVarMatrix3 h_jkl(env, numpilas);
	NumVarMatrix2 w_kl(env, numtiras);
	NumVector y_l(env);
	//	int LastBinWidth = floor((Total_area - ((numbins - 1)*plate_h*plate_w)) / plate_h);
	//	LastBinWidth = 0;
	int LastBinWidth = max((double)floor((Total_area - ((numbins - 1) * plate_h * plate_w)) / plate_h), (double)0);
	int maxh = 0;

	/**********************************************************/
	/***************** INICIALIZO VARIABLES *******************/
	/**********************************************************/
	//inicializo la variable x_ijkl
	for (int i = 0; i < numpieces; i++)
	{
		if (items[i].Getitem_h() > maxh)
			maxh = items[i].Getitem_h();
		if (items[i].Getitem_w() > maxh)
			maxh = items[i].Getitem_w();
		x_ijkl[i] = NumVarMatrix3(env, numpilas);
		xr_ijkl[i] = NumVarMatrix3(env, numpilas);

		for (int j = 0; j < numpilas; j++)
		{
			x_ijkl[i][j] = NumVarMatrix(env, numtiras);
			xr_ijkl[i][j] = NumVarMatrix(env, numtiras);

			for (int k = 0; k < numtiras; k++)
			{
				x_ijkl[i][j][k] = IloIntVarArray(env, numbins);
				xr_ijkl[i][j][k] = IloIntVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					x_ijkl[i][j][k][l] = IloIntVar(env, 0, 1);
					sprintf(nombre, "x_%d_%d_%d_%d", i, j, k, l);
					x_ijkl[i][j][k][l].setName(nombre);

					xr_ijkl[i][j][k][l] = IloIntVar(env, 0, 1);
					sprintf(nombre, "xr_%d_%d_%d_%d", i, j, k, l);
					xr_ijkl[i][j][k][l].setName(nombre);

				}

			}
		}
	}
	//inicializo variables rotadas
	//inicializo la variable h_jkl
	for (int i = 0; i < numpilas; i++)
	{
		h_jkl[i] = NumVarMatrix(env, numtiras);

		for (int j = 0; j < numtiras; j++)
		{
			h_jkl[i][j] = IloIntVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				h_jkl[i][j][l] = IloIntVar(env, 0, maxh);
				sprintf(nombre, "h_%d_%d_%d", i, j, l);
				h_jkl[i][j][l].setName(nombre);

			}

		}
	}
	//inicializo la variable w_kl y y_kl
	for (int i = 0; i < numtiras; i++)
	{
		w_kl[i] = IloIntVarArray(env, numbins);
		y_kl[i] = IloIntVarArray(env, numbins);
		for (int j = 0; j < numbins; j++)
		{
			w_kl[i][j] = IloIntVar(env, 0, max1Cut);
			sprintf(nombre, "W_%d_%d", i, j);
			w_kl[i][j].setName(nombre);
			y_kl[i][j] = IloIntVar(env, 0, 1);
			sprintf(nombre, "Y_%d_%d", i, j);
			y_kl[i][j].setName(nombre);

		}
	}

	//inicializo las variable y_l
	y_l = IloIntVarArray(env, numbins, 0, 1);
	for (int l = 0; l < numbins; l++)
	{
		sprintf(nombre, "Y%d", l);
		y_l[l].setName(nombre);
	}


	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/


	//Todos los productos colocados
	for (int i = 0; i < numpieces; i++)
	{
		IloExpr v1(env);
		for (int j = 0; j < numpilas; j++)
		{

			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					v1 += x_ijkl[i][j][k][l];
					v1 += xr_ijkl[i][j][k][l];
				}
			}
		}
		if (Tipo_objetivo != 3)
			restr.add(IloRange(env, 1, v1, 1));
		else
			restr.add(IloRange(env, 0, v1, 1));

		v1.end();
	}
	//Altura mínima
	for (int i = 0; i < numpieces; i++)
	{

		for (int j = 0; j < numpilas; j++)
		{

			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					IloExpr v1(env);

					v1 += items[i].Getitem_h() * x_ijkl[i][j][k][l];
					v1 += items[i].Getitem_w() * xr_ijkl[i][j][k][l];
					v1 += (-1) * h_jkl[j][k][l];
					restr.add(IloRange(env, -IloInfinity, v1, 0));
					v1.end();

				}
			}
		}

	}
	//simetria de las h (VA PEOR)
/*	for (int l = 0; l < numbins; l++)
	{
		for (int k = 0; k < numtiras; k++)
		{


			for (int j = (numpilas-1); j >0; j--)
			{
				IloExpr v1(env);
				v1 += h_jkl[j][k][l];
				v1+= (-maxh)* h_jkl[j-1][k][l];
				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
			}

		}
	}
	*/
	//suma de las alturas de las pilas menor H


	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			IloExpr v1(env);
			for (int j = 0; j < numpilas; j++)
			{


				v1 += h_jkl[j][k][l];
			}
			v1 += (-1) * plate_h * y_kl[k][l];
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
		}
	}
	//for (register int i = 0; i < stack_nbr; i++)
	//{
	//	//If there are items in this stacks
	//	if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
	//	{
	//		//If can not fii in that hole
	//		if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].GetItem_w()
	//suma de los anchos de las tiras
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			for (int j = 0; j < numpilas; j++)
			{

				IloExpr v1(env);

				for (int i = 0; i < numpieces; i++)
				{
					v1 += items[i].Getitem_w() * x_ijkl[i][j][k][l];
					v1 += items[i].Getitem_h() * xr_ijkl[i][j][k][l];



				}

				v1 += (-1) * w_kl[k][l];
				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
			}

		}
	}
	//Suma  tiras menor o igual a W
	for (int l = 0; l < numbins; l++)
	{
		IloExpr v1(env);
		for (int k = 0; k < numtiras; k++)
		{

			v1 += w_kl[k][l];

		}

		v1 += (-1) * plate_w * y_l[l];
		restr.add(IloRange(env, -IloInfinity, v1, 0));
		v1.end();
	}
	//Cota inferior last bin
	/*
		IloExpr v1(env);
		for (int k = 0; k < numtiras; k++)
		{

			v1 += w_kl[k][numbins-1];

		}

		v1 += (-1) * LastBinWidth  ;
		restr.add(IloRange(env, 0, v1, IloInfinity));
		v1.end();

*/
//Orden de los stacks por parejas
	for (int l = 0; l < numbins; l++)
	{
		for (int k = 0; k < numtiras; k++)
		{

			for (int j = 0; j < numpilas; j++)
			{

				for (int i = numpieces - 1; i > 0; i--)
				{


					if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1)
					{
						IloExpr v1(env);
						v1 += x_ijkl[i][j][k][l];
						v1 += xr_ijkl[i][j][k][l];
						//solamente el penúltimo tendría valor
						if (items[i].Getitem_seq() == 1)
							continue;
						// ponemos las variables
						int i2 = i - 1;
						if (i2 >= 0 && items[i].Getitem_stack() == items[i2].Getitem_stack())
						{

							for (int l2 = 0; l2 <= l; l2++)
							{
								for (int k2 = 0; k2 < numtiras; k2++)
								{
									if (l2 == l && k2 > k) break;
									for (int j2 = 0; j2 < numpilas; j2++)
									{
										if (l2 == l && k2 == k && j2 > j)
											break;

										v1 += (-1) * x_ijkl[i2][j2][k2][l2];
										v1 += (-1) * xr_ijkl[i2][j2][k2][l2];

									}

								}
							}

						}
						if (Lazy == false)
							restr.add(IloRange(env, -IloInfinity, v1, 0));
						else
							restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
						v1.end();




					}



				}
			}
		}
	}

	//Orden de los stacks
	/*
	for (int l = 0; l < numbins; l++)
	{
		for (int k = 0; k < numtiras; k++)
		{

			for (int j = 0; j < numpilas; j++)
			{

				for (int i = numpieces-1; i>0; i--)
				{

					if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1)
					{

						//solamente el penúltimo tendría valor
						if (items[i].Getitem_seq() == 1)
							continue;
							// ponemos las variables
							IloExpr v1(env);
							v1 += ( items[i].Getitem_seq()-1)*x_ijkl[i][j][k][l];
							v1 += ( items[i].Getitem_seq()-1) * xr_ijkl[i][j][k][l];

							for (int l2 = 0; l2 <= l; l2++)
							{
								for (int k2 = 0; k2 < numtiras; k2++)
								{
									if (l2 == l && k2 > k) continue;
									for (int j2 = 0; j2 < numpilas ; j2++)
									{
										if (l2==l && k2 == k && j2 > j) continue;
										for (register int i2 = i-1; i2 >=0 && items[i-1].Getitem_stack()== items[i2].Getitem_stack(); i2--)
										{
											v1 += (-1) * x_ijkl[i2][j2][k2][l2];
											v1 += (-1) * xr_ijkl[i2][j2][k2][l2];

										}

									}
								}
							}
							restr.add(IloRange(env, -IloInfinity, v1, 0));
							v1.end();



					}


				}
			}
		}
	}

	//Orden de los stacks las siguientes

	for (int l = 0; l < numbins; l++)
	{
		for (int k = 0; k < numtiras; k++)
		{

			for (int j = 0; j < numpilas; j++)
			{
				for (int i = 0; i <(numpieces-1); i++)
				{

					if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1)
					{

						//solamente el penúltimo tendría valor
						if (items[i].Getitem_seq() == stacks[items[i].Getitem_stack()].Getitem_nbr())
							continue;
							// ponemos las variables
						IloExpr v1(env);
						v1 += (stacks[items[i].Getitem_stack()].Getitem_nbr()-(items[i].Getitem_seq()) ) * x_ijkl[i][j][k][l];
						v1 += (stacks[items[i].Getitem_stack()].Getitem_nbr()-(items[i].Getitem_seq()) ) * xr_ijkl[i][j][k][l];

						for (int l2 = l; l2 < numbins; l2++)
						{
							for (int k2 = 0; k2 < numtiras; k2++)
							{
								if (l2 == l && k2 < k) continue;
								for (int j2 = 0; j2 < numpilas; j2++)
								{
									if (l2==l && k2 == k && j2 > j) continue;

									for (register int i2 = i+1; i2 <numpieces && items[i+1 ].Getitem_stack() == items[i2].Getitem_stack(); i2++)
									{
										v1 += (-1) * x_ijkl[i2][j2][k2][l2];
										v1 += (-1) * xr_ijkl[i2][j2][k2][l2];

									}

								}
							}
						}
						restr.add(IloRange(env, -IloInfinity, v1, 0));
						v1.end();



					}


				}
			}
		}
	}
	*/
	/*
		//sumas hasta la mitad los primeros, luego los siguiente
		int cont = 0;
		for (int l = 0; l < numbins; l++)
		{
			for (int k = 0; k < numtiras; k++)
			{
				for (int j = 0; j < numpilas; j++)
				{
					cont++;


					if ((cont) <= ((numbins + numpilas + numtiras) / 2))
					{
						for (int i = numpieces - 1; i > 0; i--)
						{

							if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1)
							{

								//solamente el penúltimo tendría valor
								if (items[i].Getitem_seq() == 1)
									continue;
								// ponemos las variables
								IloExpr v1(env);
								v1 += (items[i].Getitem_seq() - 1) * x_ijkl[i][j][k][l];
								v1 += (items[i].Getitem_seq() - 1) * xr_ijkl[i][j][k][l];

								for (int l2 = 0; l2 <= l; l2++)
								{
									for (int k2 = 0; k2 < numtiras; k2++)
									{
										if (l2 == l && k2 > k) continue;
										for (int j2 = 0; j2 < numpilas; j2++)
										{
											if (l2==l && k2 == k && j2 > j) continue;

											for (register int i2 = i - 1; i2 >= 0 && items[i - 1].Getitem_stack() == items[i2].Getitem_stack(); i2--)
											{
												v1 += (-1) * x_ijkl[i2][j2][k2][l2];
												v1 += (-1) * xr_ijkl[i2][j2][k2][l2];

											}

										}
									}
								}
								restr.add(IloRange(env, -IloInfinity, v1, 0));
								v1.end();



							}
						}
					}
					else
					{
						for (int i = 0; i < (numpieces - 1); i++)
						{

							if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1)
							{

								//solamente el penúltimo tendría valor
								if (items[i].Getitem_seq() == stacks[items[i].Getitem_stack()].Getitem_nbr())
									continue;
								// ponemos las variables
								IloExpr v1(env);
								v1 += (stacks[items[i].Getitem_stack()].Getitem_nbr() - (items[i].Getitem_seq())) * x_ijkl[i][j][k][l];
								v1 += (stacks[items[i].Getitem_stack()].Getitem_nbr() - (items[i].Getitem_seq())) * xr_ijkl[i][j][k][l];

								for (int l2 = l; l2 < numbins; l2++)
								{
									for (int k2 = 0; k2 < numtiras; k2++)
									{
										if (l2 == l && k2 < k) continue;
										for (int j2 = 0; j2 < numpilas; j2++)
										{
											if (l2==l && k2 == k && j2 < j) continue;

											for (register int i2 = i + 1; i2 < numpieces && items[i + 1].Getitem_stack() == items[i2].Getitem_stack(); i2++)
											{
												v1 += (-1) * x_ijkl[i2][j2][k2][l2];
												v1 += (-1) * xr_ijkl[i2][j2][k2][l2];

											}

										}
									}
								}
								restr.add(IloRange(env, -IloInfinity, v1, 0));
								v1.end();



							}
						}
					}


				}
			}
		}
		*/
		//AÑADIMOS LAS RESTRICCIONES AL MODELO 
	model.add(restr);


	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);
	if (Tipo_objetivo == 0)//biobjective //estaría en diet.ccp
	{
		IloExpr Expression_fobj1(env);
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj1 += y_l[i];
		}
		IloExpr Expression_fobj2(env);
		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj2 += w_kl[k][numbins - 1];
		}
	}

	if (Tipo_objetivo == 1)//por bins
	{
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj += y_l[i];
		}
	}
	if (Tipo_objetivo == 2)//por tira del último bin
	{

		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj += w_kl[k][numbins - 1];
		}
	}
	if (Tipo_objetivo == 3)//por área de lo metido en el bin
	{
		Expression_fobj += plate_h * plate_w;
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int j = 0; j < numpilas; j++)
				{



					for (int i = 0; i < numpieces; i++)
					{
						Expression_fobj += (-1) * items[i].Getitem_area() * x_ijkl[i][j][k][l];
						Expression_fobj += (-1) * items[i].Getitem_area() * xr_ijkl[i][j][k][l];
					}
				}
			}
		}
	}

	if (Tipo_objetivo != 0)
		model.add(IloMinimize(env, Expression_fobj));
	Expression_fobj.end();


	/***************************************************************/
	/*************** EJECUTAMOS EL MODELO **************************/
	/***************************************************************/
	try {

		IloCplex cplex(model);
		//Si queremos las lazy y tenemos alguna
		if (Lazy == true && stack_nbr < batch_items)
			cplex.addLazyConstraints(restrLazy);
		//No hace el probing
		//cplex.setParam(IloCplex::Probe, -1);
		//1 CORE
		cplex.setParam(IloCplex::Threads, nt);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, max_time); //TIEMPO EN SEGUNDOS
		if (Tipo_objetivo == 2)
			cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, LastBinWidth);

		//		cplex.exportModel("Roadef.lp");

				//CPXsetintparam(env,CPX_PARAM_THREADS,1);

				//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();


		IloBool result = cplex.solve();

		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;

		//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
//				fprintf(f, "Infeasible\n");
				//				cplex.exportModel("Roadef.lp");
			}
			else
			{
//				if (cplex.getStatus() == IloAlgorithm::Unbounded) fprintf(f, "Unbounded\n");
//				fprintf(f, "\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}
//			fclose(f);
		}
		else
		{
//			FILE* f = fopen("./estadomodelo.txt", "a+");
//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			Best_Value_Formulation = cplex.getObjValue();
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible
			if (cplex.getSolnPoolNsolns() > 0)
			{
				int value2 = cplex.getObjValue();
				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{


							for (int i = 0; i < numpieces; i++)
							{


								if (cplex.isExtracted(x_ijkl[i][j][k][l]) && cplex.getValue(x_ijkl[i][j][k][l]) > 0.001)
								{
									printf("%s %d w %d h %d s %d\n", x_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
//									fprintf(f, "%s %d w %d h %d s %d\n", x_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
								}
								if (cplex.isExtracted(xr_ijkl[i][j][k][l]) && cplex.getValue(xr_ijkl[i][j][k][l]) > 0.001)
								{
									printf("%s %d w %d h %d s %d\n", xr_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
//									fprintf(f, "%s %d w %d h %d s %d\n", xr_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
								}

							}

						}
					}
				}
				//Pinto las variables h_jkl
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							if (cplex.isExtracted(h_jkl[j][k][l]) && cplex.getValue(h_jkl[j][k][l]) > 0.001)
							{
								printf("%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
//								fprintf(f, "%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
							}

						}

					}
				}
				//Pinto las variables w_kl y y_kl
				for (int j = 0; j < numbins; j++)
				{
					for (int i = 0; i < numtiras; i++)
					{

						if (cplex.isExtracted(w_kl[i][j]) && cplex.getValue(w_kl[i][j]) > 0.001)
						{
							printf("%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
//							fprintf(f, "%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
						}
						if (cplex.isExtracted(y_kl[i][j]) && cplex.getValue(y_kl[i][j]) > 0.001)
						{
							printf("%s %d\n", y_kl[i][j].getName(), 1);
//							fprintf(f, "%s %d\n", y_kl[i][j].getName(), 1);
						}

					}
				}

				//Pinto las variable y_l
				for (int l = 0; l < numbins; l++)
				{
					if (cplex.isExtracted(y_l[l]) && cplex.getValue(y_l[l]) > 0.001)
					{
						printf("%s %d\n", y_l[l].getName(), 1);
//						fprintf(f, "%s %d\n", y_l[l].getName(), 1);
					}

				}
				//estoy en el optimo, leo las variables y continuo
				if (relativeGap == 0)
				{
					int kk = 9;
				}


			}

			//}
		}

		cplex.end();
		model.end();
		env.end();
	}
	catch (IloException& e)
	{
		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
}

void Glass::FormulationStagesMinimoCorte(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes)
{
	int kk = 9;
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	IloRangeArray restrLazy(env);
	int maxtime = G_Time;
	IloNum max_time = maxtime; //tiempo del modelo

	char nombre[20];

	knapsack_bouknap();
	//	Creo las variables

	numtiras = 4;
	numpilas = 4;
	int numpieces = batch_items;
	numbins = nbins;

	int Tipo_objetivo = 2;
	//max_numero de defectos
	int max_nd = 10;
	//Variables defectos piezas
	NumVarMatrix5 zlr_dijkl(env, max_nd);
	NumVarMatrix5 zlrr_dijkl(env, max_nd);
	NumVarMatrix5 zll_dijkl(env, max_nd);
	NumVarMatrix5 zllr_dijkl(env, max_nd);

	NumVarMatrix5 zla_dijkl(env, max_nd);
	NumVarMatrix5 zlar_dijkl(env, max_nd);
	NumVarMatrix5 zle_dijkl(env, max_nd);
	NumVarMatrix5 zler_dijkl(env, max_nd);

	//Variables defectos
	NumVarMatrix4 ze_djkl(env, max_nd);
	NumVarMatrix3 zr_dkl(env, max_nd);
	//minimo defecto pilas


	NumVarMatrix3 es_jkl(env, numpilas);
	NumVarMatrix4 el_ijkl(env, numpieces);
	NumVarMatrix3 ml_jkl(env, numpilas);
	NumVarMatrix2 ms_kl(env, numtiras);

	NumVarMatrix4 x_ijkl(env, numpieces);
	NumVarMatrix4 xr_ijkl(env, numpieces);

	NumVarMatrix2 y_kl(env, numtiras);

	NumVarMatrix3 h_jkl(env, numpilas);
	NumVarMatrix2 w_kl(env, numtiras);
	NumVector y_l(env);
	//	int LastBinWidth = floor((Total_area - ((numbins - 1)*plate_h*plate_w)) / plate_h);
	//	LastBinWidth = 0;
	int LastBinWidth = max((double)floor((Total_area - ((numbins - 1) * plate_h * plate_w)) / plate_h), (double)0);
	int maxh = 0;
	Lazy = false;
	/**********************************************************/
	/***************** INICIALIZO VARIABLES *******************/
	/**********************************************************/
	//inicializo la variable x_ijkl
	for (int i = 0; i < numpieces; i++)
	{
		if (items[i].Getitem_h() > maxh)
			maxh = items[i].Getitem_h();
		if (items[i].Getitem_w() > maxh)
			maxh = items[i].Getitem_w();
		x_ijkl[i] = NumVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			x_ijkl[i][j] = NumVarMatrix(env, numtiras);

			for (int k = 0; k < numtiras; k++)
			{
				x_ijkl[i][j][k] = IloIntVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					x_ijkl[i][j][k][l] = IloIntVar(env, 0, 1);
					sprintf(nombre, "x_%d_%d_%d_%d", i, j, k, l);
					x_ijkl[i][j][k][l].setName(nombre);

				}

			}
		}
	}
	//inicializo variables rotadas
	for (int i = 0; i < numpieces; i++)
	{
		xr_ijkl[i] = NumVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			xr_ijkl[i][j] = NumVarMatrix(env, numtiras);

			for (int k = 0; k < numtiras; k++)
			{
				xr_ijkl[i][j][k] = IloIntVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					xr_ijkl[i][j][k][l] = IloIntVar(env, 0, 1);
					sprintf(nombre, "xr_%d_%d_%d_%d", i, j, k, l);
					xr_ijkl[i][j][k][l].setName(nombre);

				}

			}
		}
	}
	//inicializo la variable h_jkl
	for (int i = 0; i < numpilas; i++)
	{
		h_jkl[i] = NumVarMatrix(env, numtiras);

		for (int j = 0; j < numtiras; j++)
		{
			h_jkl[i][j] = IloIntVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				h_jkl[i][j][l] = IloIntVar(env, 0, maxh);
				sprintf(nombre, "h_%d_%d_%d", i, j, l);
				h_jkl[i][j][l].setName(nombre);

			}

		}
	}
	//inicializo la variable w_kl y y_kl
	for (int i = 0; i < numtiras; i++)
	{
		w_kl[i] = IloIntVarArray(env, numbins);
		y_kl[i] = IloIntVarArray(env, numbins);
		ms_kl[i] = IloIntVarArray(env, numbins);
		for (int j = 0; j < numbins; j++)
		{
			w_kl[i][j] = IloIntVar(env, 0, max1Cut);
			sprintf(nombre, "W_%d_%d", i, j);
			w_kl[i][j].setName(nombre);
			y_kl[i][j] = IloIntVar(env, 0, 1);
			sprintf(nombre, "Y_%d_%d", i, j);
			y_kl[i][j].setName(nombre);
			ms_kl[i][j] = IloIntVar(env, 0, 1);
			sprintf(nombre, "ms_%d_%d", i, j);
			ms_kl[i][j].setName(nombre);

		}
	}

	//inicializo las variable y_l
	y_l = IloIntVarArray(env, numbins, 0, 1);
	for (int l = 0; l < numbins; l++)
	{
		sprintf(nombre, "Y%d", l);
		y_l[l].setName(nombre);
	}

	//minimco corte
	//inicializo variables rotadas
	for (int i = 0; i < numpieces; i++)
	{
		el_ijkl[i] = NumVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			el_ijkl[i][j] = NumVarMatrix(env, numtiras);

			for (int k = 0; k < numtiras; k++)
			{
				el_ijkl[i][j][k] = IloIntVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					el_ijkl[i][j][k][l] = IloIntVar(env, 0, 1);
					sprintf(nombre, "Xel_%d_%d_%d_%d", i, j, k, l);
					el_ijkl[i][j][k][l].setName(nombre);

				}

			}
		}
	}
	//inicializo la variable es_jkl
	for (int i = 0; i < numpilas; i++)
	{
		es_jkl[i] = NumVarMatrix(env, numtiras);
		ml_jkl[i] = NumVarMatrix(env, numtiras);
		for (int j = 0; j < numtiras; j++)
		{
			es_jkl[i][j] = IloIntVarArray(env, numbins);
			ml_jkl[i][j] = IloIntVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				es_jkl[i][j][l] = IloIntVar(env, 0, 1);
				sprintf(nombre, "Xes_%d_%d_%d", i, j, l);
				es_jkl[i][j][l].setName(nombre);
				ml_jkl[i][j][l] = IloIntVar(env, 0, 1);
				sprintf(nombre, "ml_%d_%d_%d", i, j, l);
				ml_jkl[i][j][l].setName(nombre);

			}

		}
	}
	//inicializo las variables de los cortes

	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			zr_dkl[i] = NumVarMatrix(env, numtiras);

			for (int j = 0; j < numtiras; j++)
			{
				zr_dkl[i][j] = IloIntVarArray(env, 1);
				zr_dkl[i][j][l] = IloIntVar(env, 0, 1);
				sprintf(nombre, "zr_%d_%d_%d", i, j, l);
				zr_dkl[i][j][l].setName(nombre);

			}
		}
	}

	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();
		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			ze_djkl[i] = NumVarMatrix3(env, numpilas);

			for (int j = 0; j < numpilas; j++)
			{
				ze_djkl[i][j] = NumVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					ze_djkl[i][j][k] = IloIntVarArray(env, 1);
					ze_djkl[i][j][k][l] = IloIntVar(env, 0, 1);
					sprintf(nombre, "ze_%d_%d_%d_%d", i, j, k, l);
					ze_djkl[i][j][k][l].setName(nombre);
				}

			}
		}
	}

	//inicializo las variables de los defectos
	//derecha e izquierda
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();
		int d = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
		{
			zlr_dijkl[d] = NumVarMatrix4(env, numpieces);
			zlrr_dijkl[d] = NumVarMatrix4(env, numpieces);
			zll_dijkl[d] = NumVarMatrix4(env, numpieces);
			zllr_dijkl[d] = NumVarMatrix4(env, numpieces);

			for (int i = 0; i < numpieces; i++)
			{
				zlr_dijkl[d][i] = NumVarMatrix3(env, numpilas);
				zlrr_dijkl[d][i] = NumVarMatrix3(env, numpilas);
				zll_dijkl[d][i] = NumVarMatrix3(env, numpilas);
				zllr_dijkl[d][i] = NumVarMatrix3(env, numpilas);
				for (int j = 0; j < numpilas; j++)
				{
					zlr_dijkl[d][i][j] = NumVarMatrix(env, numtiras);
					zlrr_dijkl[d][i][j] = NumVarMatrix(env, numtiras);
					zll_dijkl[d][i][j] = NumVarMatrix(env, numtiras);
					zllr_dijkl[d][i][j] = NumVarMatrix(env, numtiras);
					for (int k = 0; k < numtiras; k++)
					{
						zlr_dijkl[d][i][j][k] = IloIntVarArray(env, 1);
						zlr_dijkl[d][i][j][k][l] = IloIntVar(env, 0, 1);
						sprintf(nombre, "zlr_%d_%d_%d_%d_%d", d, i, j, k, l);
						zlr_dijkl[d][i][j][k][l].setName(nombre);

						zlrr_dijkl[d][i][j][k] = IloIntVarArray(env, 1);
						zlrr_dijkl[d][i][j][k][l] = IloIntVar(env, 0, 1);
						sprintf(nombre, "zlrr_%d_%d_%d_%d_%d", d, i, j, k, l);
						zlrr_dijkl[d][i][j][k][l].setName(nombre);

						zll_dijkl[d][i][j][k] = IloIntVarArray(env, 1);
						zll_dijkl[d][i][j][k][l] = IloIntVar(env, 0, 1);
						sprintf(nombre, "zll_%d_%d_%d_%d_%d", d, i, j, k, l);
						zll_dijkl[d][i][j][k][l].setName(nombre);

						zllr_dijkl[d][i][j][k] = IloIntVarArray(env, 1);
						zllr_dijkl[d][i][j][k][l] = IloIntVar(env, 0, 1);
						sprintf(nombre, "zllr_%d_%d_%d_%d_%d", d, i, j, k, l);
						zllr_dijkl[d][i][j][k][l].setName(nombre);


					}

				}
			}
		}
	}

	//Arriba o abajo
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();
		int d = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
		{
			zla_dijkl[d] = NumVarMatrix4(env, numpieces);
			zlar_dijkl[d] = NumVarMatrix4(env, numpieces);
			zle_dijkl[d] = NumVarMatrix4(env, numpieces);
			zler_dijkl[d] = NumVarMatrix4(env, numpieces);
			for (int i = 0; i < numpieces; i++)
			{
				zla_dijkl[d][i] = NumVarMatrix3(env, numpilas);
				zlar_dijkl[d][i] = NumVarMatrix3(env, numpilas);
				zle_dijkl[d][i] = NumVarMatrix3(env, numpilas);
				zler_dijkl[d][i] = NumVarMatrix3(env, numpilas);
				for (int j = 0; j < numpilas; j++)
				{
					zla_dijkl[d][i][j] = NumVarMatrix(env, numtiras);
					zlar_dijkl[d][i][j] = NumVarMatrix(env, numtiras);
					zle_dijkl[d][i][j] = NumVarMatrix(env, numtiras);
					zler_dijkl[d][i][j] = NumVarMatrix(env, numtiras);
					for (int k = 0; k < numtiras; k++)
					{
						zla_dijkl[d][i][j][k] = IloIntVarArray(env, 1);
						zla_dijkl[d][i][j][k][l] = IloIntVar(env, 0, 1);
						sprintf(nombre, "zla_%d_%d_%d_%d_%d", d, i, j, k, l);
						zla_dijkl[d][i][j][k][l].setName(nombre);

						zlar_dijkl[d][i][j][k] = IloIntVarArray(env, 1);
						zlar_dijkl[d][i][j][k][l] = IloIntVar(env, 0, 1);
						sprintf(nombre, "zlar_%d_%d_%d_%d_%d", d, i, j, k, l);
						zlar_dijkl[d][i][j][k][l].setName(nombre);

						zle_dijkl[d][i][j][k] = IloIntVarArray(env, 1);
						zle_dijkl[d][i][j][k][l] = IloIntVar(env, 0, 1);
						sprintf(nombre, "zle_%d_%d_%d_%d_%d", d, i, j, k, l);
						zle_dijkl[d][i][j][k][l].setName(nombre);

						zler_dijkl[d][i][j][k] = IloIntVarArray(env, 1);
						zler_dijkl[d][i][j][k][l] = IloIntVar(env, 0, 1);
						sprintf(nombre, "zler_%d_%d_%d_%d_%d", d, i, j, k, l);
						zler_dijkl[d][i][j][k][l].setName(nombre);


					}

				}
			}
		}
	}
	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/

//	Lazy_minimo = false;
	//Todos los productos colocados
	for (int i = 0; i < numpieces; i++)
	{
		IloExpr v1(env);
		for (int j = 0; j < numpilas; j++)
		{

			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					v1 += x_ijkl[i][j][k][l];
					v1 += xr_ijkl[i][j][k][l];
				}
			}
		}
		if (Tipo_objetivo != 3)
			restr.add(IloRange(env, 1, v1, 1));
		else
			restr.add(IloRange(env, 0, v1, 1));

		v1.end();
	}

	//Altura mínima
	for (int i = 0; i < numpieces; i++)
	{

		for (int j = 0; j < numpilas; j++)
		{

			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					IloExpr v1(env);

					v1 += items[i].Getitem_h() * x_ijkl[i][j][k][l];
					v1 += items[i].Getitem_w() * xr_ijkl[i][j][k][l];
					v1 += (-1) * h_jkl[j][k][l];
					v1 += waste_min;
					v1 += (-1) * waste_min * (el_ijkl[i][j][k][l]);
					restr.add(IloRange(env, -IloInfinity, v1, 0));
					v1.end();

				}
			}
		}

	}

	//Altura maxima Candidatas lazy
	for (int i = 0; i < numpieces; i++)
	{

		for (int j = 0; j < numpilas; j++)
		{

			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					IloExpr v1(env);

					v1 += items[i].Getitem_h() * x_ijkl[i][j][k][l];
					v1 += items[i].Getitem_w() * xr_ijkl[i][j][k][l];
					v1 += (-1) * h_jkl[j][k][l];
					v1 += plate_h;
					v1 += (-1) * plate_h * el_ijkl[i][j][k][l];
					if (Lazy_minimo)
						restrLazy.add(IloRange(env, 0, v1, IloInfinity));
					else
						restr.add(IloRange(env, 0, v1, IloInfinity));

					v1.end();

				}
			}
		}

	}

	//suma de las alturas de las pilas menor H


	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			IloExpr v1(env);
			for (int j = 0; j < numpilas; j++)
			{


				v1 += h_jkl[j][k][l];
			}
			v1 += (-1) * plate_h * y_kl[k][l];
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
		}
	}
	//suma de los anchos de las tiras
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			for (int j = 0; j < numpilas; j++)
			{

				IloExpr v1(env);

				for (int i = 0; i < numpieces; i++)
				{
					v1 += items[i].Getitem_w() * x_ijkl[i][j][k][l];
					v1 += items[i].Getitem_h() * xr_ijkl[i][j][k][l];



				}

				v1 += (-1) * w_kl[k][l];
				v1 += waste_min;
				v1 += (-1) * waste_min * es_jkl[j][k][l];
				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
			}

		}
	}
	//minimos cortes
		//suma de las alturas de las pilas menor H

//	Lazy_minimo = true;
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{

			for (int j = 0; j < numpilas; j++)
			{
				//				Lazy_minimo = true;
				IloExpr v1(env);
				v1 += min2Cut * ml_jkl[j][k][l];

				v1 += (-1) * h_jkl[j][k][l];
				/*				if (Lazy_minimo)
								restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
								else*/
				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
				IloExpr v2(env);
				v2 += plate_h * ml_jkl[j][k][l];
				v2 += (-1) * h_jkl[j][k][l];
				if (Lazy_minimo)
					restrLazy.add(IloRange(env, 0, v2, IloInfinity));
				else
					restr.add(IloRange(env, 0, v2, IloInfinity));
				v2.end();
				//no crea pilas vacías y además ordenadas
				IloExpr v3(env);
				v3 += ml_jkl[j][k][l];
				for (int i = 0; i < numpieces; i++)
				{
					v3 += (-1) * x_ijkl[i][j][k][l];
					v3 += (-1) * xr_ijkl[i][j][k][l];
				}
				restr.add(IloRange(env, -IloInfinity, v3, 0));
				v3.end();

				if (j > 0)
				{
					IloExpr v4(env);
					v4 += ml_jkl[j][k][l];
					v4 += (-1) * ml_jkl[j - 1][k][l];
					restr.add(IloRange(env, -IloInfinity, v4, 0));
					v4.end();
				}




			}
			//			Lazy_minimo = false;
			IloExpr v1(env);
			v1 += min1Cut * ms_kl[k][l];

			v1 += (-1) * w_kl[k][l];
			/*			if (Lazy_minimo)
						restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
						else*/
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
			IloExpr v2(env);
			v2 += max1Cut * ms_kl[k][l];
			v2 += (-1) * w_kl[k][l];
			if (Lazy_minimo)
				restrLazy.add(IloRange(env, 0, v2, IloInfinity));
			else
				restr.add(IloRange(env, 0, v2, IloInfinity));
			v2.end();

			//no crea tiras vacías y además ordenadas
			IloExpr v3(env);
			v3 += ms_kl[k][l];
			for (int i = 0; i < numpieces; i++)
			{
				for (int j = 0; j < numpilas; j++)
				{
					v3 += (-1) * x_ijkl[i][j][k][l];
					v3 += (-1) * xr_ijkl[i][j][k][l];
				}
			}
			restr.add(IloRange(env, -IloInfinity, v3, 0));
			v3.end();
			if (k > 0)
			{
				IloExpr v4(env);
				v4 += ms_kl[k][l];
				v4 += (-1) * ms_kl[k - 1][l];
				restr.add(IloRange(env, -IloInfinity, v4, 0));
				v4.end();
			}
		}
	}
	//	Lazy_minimo = true;
		//suma de los anchos de las tiras minimo
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			for (int j = 0; j < numpilas; j++)
			{

				IloExpr v1(env);

				for (int i = 0; i < numpieces; i++)
				{
					v1 += items[i].Getitem_w() * x_ijkl[i][j][k][l];
					v1 += items[i].Getitem_h() * xr_ijkl[i][j][k][l];



				}

				v1 += (-1) * w_kl[k][l];
				v1 += max1Cut;
				v1 += (-1) * max1Cut * es_jkl[j][k][l];
				if (Lazy_minimo)
					restrLazy.add(IloRange(env, 0, v1, IloInfinity));

				else
					restr.add(IloRange(env, 0, v1, IloInfinity));

				v1.end();

			}

		}
	}
	//Suma  tiras menor o igual a W
	for (int l = 0; l < numbins; l++)
	{
		IloExpr v1(env);
		for (int k = 0; k < numtiras; k++)
		{

			v1 += w_kl[k][l];

		}

		v1 += (-1) * plate_w * y_l[l];
		restr.add(IloRange(env, -IloInfinity, v1, 0));
		v1.end();
	}

	//Orden de los stacks por parejas
	for (int l = 0; l < numbins; l++)
	{
		for (int k = 0; k < numtiras; k++)
		{

			for (int j = 0; j < numpilas; j++)
			{

				for (int i = numpieces - 1; i > 0; i--)
				{


					if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1)
					{
						IloExpr v1(env);
						v1 += x_ijkl[i][j][k][l];
						v1 += xr_ijkl[i][j][k][l];
						//solamente el penúltimo tendría valor
						if (items[i].Getitem_seq() == 1)
							continue;
						// ponemos las variables
						int i2 = i - 1;
						if (i2 >= 0 && items[i].Getitem_stack() == items[i2].Getitem_stack())
						{

							for (int l2 = 0; l2 <= l; l2++)
							{
								for (int k2 = 0; k2 < numtiras; k2++)
								{
									if (l2 == l && k2 > k) break;
									for (int j2 = 0; j2 < numpilas; j2++)
									{
										if (l2 == l && k2 == k && j2 > j)
											break;

										v1 += (-1) * x_ijkl[i2][j2][k2][l2];
										v1 += (-1) * xr_ijkl[i2][j2][k2][l2];

									}

								}
							}

						}
						if (Lazy == false)
							restr.add(IloRange(env, -IloInfinity, v1, 0));
						else
							restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
						v1.end();




					}



				}
			}
		}
	}
	//restricciones de los cortes verticales
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{

			for (int k1 = 0; k1 < numtiras; k1++)
			{
				IloExpr v1(env);
				IloExpr v2(env);
				for (int k2 = 0; k2 <= k1; k2++)
				{
					v1 += w_kl[k2][l];
					v2 += w_kl[k2][l];
				}
				v1 += (-1) * ((*itD).Getpos_x());

				v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1][l]);
				//			v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth());
				v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zr_dkl[i][k1][l]);
				restr.add(IloRange(env, -IloInfinity, v1, 0));

				if (Lazy_cortes)
					restrLazy.add(IloRange(env, 0, v2, IloInfinity));
				else
					restr.add(IloRange(env, 0, v2, IloInfinity));
				v1.end();
				v2.end();
				if (k1 > 0)
				{
					IloExpr v3(env);
					v3 += zr_dkl[i][k1][l];
					v3 += (-1) * zr_dkl[i][k1 - 1][l];
					restr.add(IloRange(env, -IloInfinity, v3, 0));
					v3.end();
				}
			}



		}
	}

	//restricciones de los cortes horizontales
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			for (int k = 0; k < numtiras; k++)
			{

				for (int j1 = 0; j1 < numpilas; j1++)
				{
					IloExpr v1(env);
					IloExpr v2(env);
					for (int j2 = 0; j2 <= j1; j2++)
					{

						v1 += h_jkl[j2][k][l];
						v2 += h_jkl[j2][k][l];
					}



					v1 += (-1) * ((*itD).Getpos_y());
					v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - ze_djkl[i][j1][k][l]);
					v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (zr_dkl[i][k][l]);
					if (k > 0)
					{
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zr_dkl[i][k - 1][l]);
					}
					restr.add(IloRange(env, -IloInfinity, v1, 0));
					//ahora falta la otra parte
					int rtyd = (*itD).Getpos_y() + (*itD).Getheight();
					v2 += (-1) * (rtyd) * (1 - ze_djkl[i][j1][k][l]);
					v2 += (rtyd) * (zr_dkl[i][k][l]);
					if (k > 0)
					{
						v2 += rtyd * (1 - zr_dkl[i][k - 1][l]);
					}

					if (Lazy_cortes)
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));
					else
						restr.add(IloRange(env, 0, v2, IloInfinity));
					v1.end();
					v2.end();
					if (j1 > 0)
					{
						IloExpr v3(env);
						v3 += ze_djkl[i][j1][k][l];
						v3 += (-1) * ze_djkl[i][j1 - 1][k][l];
						restr.add(IloRange(env, -IloInfinity, v3, 0));
						v3.end();


					}
					IloExpr v4(env);
					v4 += ze_djkl[i][j1][k][l];
					if (k > 0)
						v4 += (-1) * (zr_dkl[i][k - 1][l] - zr_dkl[i][k][l]);
					else
						v4 += (-1) * (1 - zr_dkl[i][k][l]);
					restr.add(IloRange(env, -IloInfinity, v4, 0));
					v4.end();


				}
				//orden de las variables
			}






		}
	}

	//restricciones de los defectos
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int d = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
		{

			for (int j = 0; j < numpilas; j++)
			{

				for (int k = 0; k < numtiras; k++)
				{
					IloExpr v1(env);
					IloExpr v2(env);
					for (int k2 = 0; k2 <= k; k2++)
					{
						if (k2 < k)
							v1 += w_kl[k2][l];
						v2 += w_kl[k2][l];
					}
					for (int i = 0; i < numpieces; i++)
					{
						v1 += items[i].Getitem_w() * zlr_dijkl[d][i][j][k][l];
						v1 += items[i].Getitem_h() * zlrr_dijkl[d][i][j][k][l];
						v2 += (-1) * items[i].Getitem_w() * zll_dijkl[d][i][j][k][l];
						v2 += (-1) * items[i].Getitem_h() * zllr_dijkl[d][i][j][k][l];

						/*				IloExpr v3(env);
										v3 += zlr_dijkl[d][i][j][k][l];
										v3 += zll_dijkl[d][i][j][k][l];
										v3 += (-1) * x_ijkl[i][j][k][l];
										if (Lazy_cortes)
											restrLazy.add(IloRange(env, -IloInfinity, v3, 0));
										else
											restr.add(IloRange(env, -IloInfinity, v3, 0));
										v3.end();
										IloExpr v4(env);
										v4 += zlrr_dijkl[d][i][j][k][l];
										v4 += zllr_dijkl[d][i][j][k][l];
										v4 += (-1) * xr_ijkl[i][j][k][l];
										if (Lazy_cortes)
											restrLazy.add(IloRange(env, -IloInfinity, v4, 0));
										else
											restr.add(IloRange(env, -IloInfinity, v4, 0));
										v4.end();
										*/

						IloExpr v5(env);
						IloExpr v6(env);
						v5 += zlrr_dijkl[d][i][j][k][l];
						v5 += zllr_dijkl[d][i][j][k][l];
						v5 += zlar_dijkl[d][i][j][k][l];
						v5 += zler_dijkl[d][i][j][k][l];
						v6 += zlr_dijkl[d][i][j][k][l];
						v6 += zll_dijkl[d][i][j][k][l];
						v6 += zla_dijkl[d][i][j][k][l];
						v6 += zle_dijkl[d][i][j][k][l];
						v5 += (-1) * xr_ijkl[i][j][k][l];
						v6 += (-1) * x_ijkl[i][j][k][l];
						//						if (Lazy_cortes)
						//							restrLazy.add(IloRange(env, 0, v5, 0));
						//						else
						restr.add(IloRange(env, 0, v5, 0));
						v5.end();
						restr.add(IloRange(env, 0, v6, 0));
						v6.end();
					}
					v1 += (-1) * (*itD).Getpos_x();
					v1 += (-1) * (plate_w - (*itD).Getpos_x()) * ze_djkl[d][j][k][l];
					v1 += (-1) * (plate_w - (*itD).Getpos_x()) * zr_dkl[d][k][l];
					if (j > 0)
						v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - ze_djkl[d][j - 1][k][l]);
					if (k > 0)
						v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[d][k - 1][l]);

					int rtxd = (*itD).Getpos_x() + (*itD).Getwidth();
					v2 += (-1) * rtxd;
					v2 += (rtxd)* ze_djkl[d][j][k][l];
					v2 += (rtxd)* zr_dkl[d][k][l];
					if (j > 0)
						v2 += (rtxd) * (1 - ze_djkl[d][j - 1][k][l]);
					if (k > 0)
						v2 += (rtxd) * (1 - zr_dkl[d][k - 1][l]);

					restr.add(IloRange(env, -IloInfinity, v1, 0));
					if (Lazy_cortes)
						restr.add(IloRange(env, 0, v2, IloInfinity));
					else
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));



					v1.end();
					v2.end();

					//orden de las variables



				}
				//orden de las variables
			}


		}



	}



	//restricciones de arriba y abajo
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int d = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
		{

			for (int j = 0; j < numpilas; j++)
			{

				for (int k = 0; k < numtiras; k++)
				{
					IloExpr v1(env);
					IloExpr v2(env);
					for (int j2 = 0; j2 <= j; j2++)
					{
						if (j2 < j)
							v1 += h_jkl[j2][k][l];
						v2 += h_jkl[j2][k][l];
					}
					for (int i = 0; i < numpieces; i++)
					{
						v1 += items[i].Getitem_h() * zle_dijkl[d][i][j][k][l];
						v1 += items[i].Getitem_w() * zler_dijkl[d][i][j][k][l];
						v2 += (-1) * items[i].Getitem_h() * zla_dijkl[d][i][j][k][l];
						v2 += (-1) * items[i].Getitem_w() * zlar_dijkl[d][i][j][k][l];
						/*
						IloExpr v3(env);
						v3 += zla_dijkl[d][i][j][k][l];
						v3 += zle_dijkl[d][i][j][k][l];
						v3 += (-1) * x_ijkl[i][j][k][l];
						//						if (Lazy_cortes)
						//							restrLazy.add(IloRange(env, -IloInfinity, v3, 0));
						//						else
						restr.add(IloRange(env, -IloInfinity, v3, 0));
						v3.end();
						IloExpr v4(env);
						v4 += zlar_dijkl[d][i][j][k][l];
						v4 += zler_dijkl[d][i][j][k][l];
						v4 += (-1) * xr_ijkl[i][j][k][l];
						//						if (Lazy_cortes)
						//							restrLazy.add(IloRange(env, -IloInfinity, v4, 0));
						//						else
						restr.add(IloRange(env, -IloInfinity, v4, 0));
						v4.end();*/
					}
					v1 += (-1) * (*itD).Getpos_y();
					v1 += (-1) * (plate_h - (*itD).Getpos_y()) * ze_djkl[d][j][k][l];
					v1 += (-1) * (plate_h - (*itD).Getpos_y()) * zr_dkl[d][k][l];
					if (j > 0)
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - ze_djkl[d][j - 1][k][l]);
					if (k > 0)
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zr_dkl[d][k - 1][l]);

					int rtyd = (*itD).Getpos_y() + (*itD).Getheight();
					v2 += (-1) * rtyd;
					v2 += (rtyd)* ze_djkl[d][j][k][l];
					v2 += (rtyd)* zr_dkl[d][k][l];
					if (j > 0)
						v2 += (rtyd) * (1 - ze_djkl[d][j - 1][k][l]);
					if (k > 0)
						v2 += (rtyd) * (1 - zr_dkl[d][k - 1][l]);

					restr.add(IloRange(env, -IloInfinity, v1, 0));
					if (Lazy_cortes)
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));
					else
						restr.add(IloRange(env, 0, v2, IloInfinity));



					v1.end();
					v2.end();


				}
				//orden de las variables
			}






		}
	}

	model.add(restr);
	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
//	model.add(restr);


	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);
	if (Tipo_objetivo == 0)//biobjective //estaría en diet.ccp
	{
		IloExpr Expression_fobj1(env);
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj1 += y_l[i];
		}
		IloExpr Expression_fobj2(env);
		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj2 += w_kl[k][numbins - 1];
		}
	}

	if (Tipo_objetivo == 1)//por bins
	{
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj += y_l[i];
		}
	}
	if (Tipo_objetivo == 2)//por tira del último bin
	{

		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj += w_kl[k][numbins - 1];
		}
	}
	if (Tipo_objetivo == 3)//por área de lo metido en el bin
	{
		Expression_fobj += plate_h * plate_w;
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int j = 0; j < numpilas; j++)
				{



					for (int i = 0; i < numpieces; i++)
					{
						Expression_fobj += (-1) * items[i].Getitem_area() * x_ijkl[i][j][k][l];
						Expression_fobj += (-1) * items[i].Getitem_area() * xr_ijkl[i][j][k][l];
					}
				}
			}
		}
	}

	if (Tipo_objetivo != 0)
		model.add(IloMinimize(env, Expression_fobj));
	Expression_fobj.end();


	/***************************************************************/
	/*************** EJECUTAMOS EL MODELO **************************/
	/***************************************************************/
	try {

		IloCplex cplex(model);
		//Si queremos las lazy y tenemos alguna
		if (Lazy_minimo)
			cplex.addLazyConstraints(restrLazy);
		//		cplex.exportModel("Roadef.lp");
				//No hace el probing
				//cplex.setParam(IloCplex::Probe, -1);
				//1 CORE
		cplex.setParam(IloCplex::Threads, nt);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, max_time); //TIEMPO EN SEGUNDOS
		if (Tipo_objetivo == 2)
			cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, LastBinWidth);
		if (G_Dibujar)
		cplex.exportModel("Roadef.lp");

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

		//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();


		IloBool result = cplex.solve();

		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;

		//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
//				fprintf(f, "Infeasible\n");
				//				cplex.exportModel("Roadef.lp");
			}
			else
			{
//				if (cplex.getStatus() == IloAlgorithm::Unbounded) fprintf(f, "Unbounded\n");
//				fprintf(f, "\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}
//			fclose(f);
		}
		else
		{
//			FILE* f = fopen("./estadomodelo.txt", "a+");
//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			Best_Value_Formulation = cplex.getObjValue();
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible
			if (cplex.getSolnPoolNsolns() > 0)
			{
				int value2 = cplex.getObjValue();
				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{


							for (int i = 0; i < numpieces; i++)
							{


								if (cplex.isExtracted(x_ijkl[i][j][k][l]) && cplex.getValue(x_ijkl[i][j][k][l]) > 0.001)
								{
									printf("%s %d w %d h %d s %d\n", x_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
//									fprintf(f, "%s %d w %d h %d s %d\n", x_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
								}
								if (cplex.isExtracted(xr_ijkl[i][j][k][l]) && cplex.getValue(xr_ijkl[i][j][k][l]) > 0.001)
								{
									printf("%s %d w %d h %d s %d\n", xr_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
//									fprintf(f, "%s %d w %d h %d s %d\n", xr_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
								}

							}

						}
					}
				}
				//Pinto las variables h_jkl
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							if (cplex.isExtracted(h_jkl[j][k][l]) && cplex.getValue(h_jkl[j][k][l]) > 0.001)
							{
								printf("%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
//								fprintf(f, "%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
							}

						}

					}
				}
				//Pinto las variables w_kl y y_kl
				for (int j = 0; j < numbins; j++)
				{
					for (int i = 0; i < numtiras; i++)
					{

						if (cplex.isExtracted(w_kl[i][j]) && cplex.getValue(w_kl[i][j]) > 0.001)
						{
							printf("%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
//							fprintf(f, "%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
						}
						if (cplex.isExtracted(y_kl[i][j]) && cplex.getValue(y_kl[i][j]) > 0.001)
						{
							printf("%s %d\n", y_kl[i][j].getName(), 1);
//							fprintf(f, "%s %d\n", y_kl[i][j].getName(), 1);
						}

					}
				}

				//Pinto las variable y_l
				for (int l = 0; l < numbins; l++)
				{
					if (cplex.isExtracted(y_l[l]) && cplex.getValue(y_l[l]) > 0.001)
					{
						printf("%s %d\n", y_l[l].getName(), 1);
//						fprintf(f, "%s %d\n", y_l[l].getName(), 1);
					}

				}
				//Pinto los cortes
				for (int l = 0; l < numbins; l++)
				{
					int nd = DefectsPlate[l].size();

					int d = 0;
					for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							for (int k = 0; k < numtiras; k++)
							{
								if (cplex.isExtracted(ze_djkl[d][j][k][l]) && cplex.getValue(ze_djkl[d][j][k][l]) > 0.001)
								{
									printf("%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
//									fprintf(f, "%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
								}
								if (j == 0)
								{
									if (cplex.isExtracted(zr_dkl[d][k][l]) && cplex.getValue(zr_dkl[d][k][l]) > 0.001)
									{
										printf("%s %d\n", zr_dkl[d][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zr_dkl[d][k][l].getName(), 1);
									}

								}



							}
						}
					}

				}
				//Pinto las variables zlr,zllr, zll,zlrr
				for (int l = 0; l < numbins; l++)
				{
					int nd = DefectsPlate[l].size();

					int d = 0;
					for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
					{
						for (int i = 0; i < numpieces; i++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								for (int k = 0; k < numtiras; k++)
								{
									if (cplex.isExtracted(zlr_dijkl[d][i][j][k][l]) && cplex.getValue(zlr_dijkl[d][i][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zlr_dijkl[d][i][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zlr_dijkl[d][i][j][k][l].getName(), 1);
									}
									if (cplex.isExtracted(zlrr_dijkl[d][i][j][k][l]) && cplex.getValue(zlrr_dijkl[d][i][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zlrr_dijkl[d][i][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zlrr_dijkl[d][i][j][k][l].getName(), 1);
									}
									if (cplex.isExtracted(zll_dijkl[d][i][j][k][l]) && cplex.getValue(zll_dijkl[d][i][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zll_dijkl[d][i][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zll_dijkl[d][i][j][k][l].getName(), 1);
									}
									if (cplex.isExtracted(zllr_dijkl[d][i][j][k][l]) && cplex.getValue(zllr_dijkl[d][i][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zllr_dijkl[d][i][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zllr_dijkl[d][i][j][k][l].getName(), 1);
									}
									if (cplex.isExtracted(zla_dijkl[d][i][j][k][l]) && cplex.getValue(zla_dijkl[d][i][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zla_dijkl[d][i][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zla_dijkl[d][i][j][k][l].getName(), 1);
									}
									if (cplex.isExtracted(zlar_dijkl[d][i][j][k][l]) && cplex.getValue(zlar_dijkl[d][i][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zlar_dijkl[d][i][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zlar_dijkl[d][i][j][k][l].getName(), 1);
									}
									if (cplex.isExtracted(zle_dijkl[d][i][j][k][l]) && cplex.getValue(zle_dijkl[d][i][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zle_dijkl[d][i][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zle_dijkl[d][i][j][k][l].getName(), 1);
									}
									if (cplex.isExtracted(zler_dijkl[d][i][j][k][l]) && cplex.getValue(zler_dijkl[d][i][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zler_dijkl[d][i][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zler_dijkl[d][i][j][k][l].getName(), 1);
									}

								}
							}
						}
					}
				}
				printf("\nVariables de mínimo\n");
				for (int i = 0; i < numpieces; i++)
				{

					for (int j = 0; j < numpilas; j++)
					{

						for (int k = 0; k < numtiras; k++)
						{
							for (int l = 0; l < numbins; l++)
							{
								if (cplex.isExtracted(el_ijkl[i][j][k][l]) && cplex.getValue(el_ijkl[i][j][k][l]) > 0.001)
								{
									printf("%s %d\n", el_ijkl[i][j][k][l].getName(), 1);
//									fprintf(f, "%s %d\n", el_ijkl[i][j][k][l].getName(), 1);
								}
								if (i == 0)
								{
									if (cplex.isExtracted(es_jkl[j][k][l]) && cplex.getValue(es_jkl[j][k][l]) > 0.001)
									{
										printf("%s %d\n", es_jkl[j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", es_jkl[j][k][l].getName(), 1);
									}

								}

							}
						}
					}
				}
				//Escribo la solución para dibujar
					//Inicio
				if (G_Dibujar || G_Draw_Exacto)
				{
					FILE* fin3;
					if (G_Draw_Exacto)
					{
						std::string str = file_idx + "_solution_" + std::to_string(G_Exacto) + "_" + std::to_string(Tipo_objetivo) + ".txt";
						fin3=fopen(P_file_to_write.c_str(), "w+");
//						fin3 = fopen(str.c_str(), "w+");
					}
					else
						fin3 = fopen("./temp2.txt", "w+");
					int ndefects = 0;
					int nbinsdefects = 0;
					for (int i = 0; i < numbins; i++)
					{
						if (DefectsPlate[i].size() >= 1)
							nbinsdefects++;
						for (std::list< GlassDefect > ::iterator it = DefectsPlate[i].begin(); it != DefectsPlate[i].end(); it++)
						{
							ndefects++;
						}
					}
					//					fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, Best_Objective_function);
					fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, numpieces, ndefects, nbinsdefects, LB_Best_Value_Formulation, Best_Value_Formulation);
					//defectos

					for (int i = 0; i < numbins; i++)
					{

						for (std::list< GlassDefect > ::iterator it = DefectsPlate[i].begin(); it != DefectsPlate[i].end(); it++)
						{
							// Get defect coordinates.
							int defect_x = (*it).Getpos_x();
							int defect_y = (*it).Getpos_y();
							int defect_width = (*it).Getwidth();
							int defect_height = (*it).Getheight();

							fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", defect_x, defect_y, defect_width, defect_height, i);
						}
					}
					for (int i = 0; i < numbins; i++)
					{
						if (DefectsPlate[i].size() > 0)
							fprintf(fin3, "%d\t%d\n", i, DefectsPlate[i].size());

					}
					for (int l = 0; l < numbins; l++)
					{
						for (int k = 0; k < numtiras; k++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								int pos_x = 0;
								for (int k1 = 0; k1 < k; k1++)
								{
									if (cplex.isExtracted(w_kl[k1][l]) && cplex.getValue(w_kl[k1][l]) > 0.001)
									{
										pos_x += cplex.getValue(w_kl[k1][l]);
									}
								}
								int pos_y = 0;
								for (int j1 = 0; j1 < j; j1++)
								{
									if (cplex.isExtracted(h_jkl[j1][k][l]) && cplex.getValue(h_jkl[j1][k][l]) > 0.001)
									{
										pos_y += cplex.getValue(h_jkl[j1][k][l]);
									}
								}
								for (int i = 0; i < numpieces; i++)
								{


									if (cplex.isExtracted(x_ijkl[i][j][k][l]) && cplex.getValue(x_ijkl[i][j][k][l]) > 0.001)
									{

										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x, pos_y, pos_x + items[i].Getitem_w(), pos_y + items[i].Getitem_h(), l + 1);
										pos_x += items[i].Getitem_w();

									}
									if (cplex.isExtracted(xr_ijkl[i][j][k][l]) && cplex.getValue(xr_ijkl[i][j][k][l]) > 0.001)
									{
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x, pos_y, pos_x + items[i].Getitem_h(), pos_y + items[i].Getitem_w(), l + 1);
										pos_x += items[i].Getitem_h();
									}

								}

							}
						}
					}

					fclose(fin3);
					DibujarOpenGL();
				}
				//estoy en el optimo, leo las variables y continuo
				if (relativeGap == 0)
				{
					int kk = 9;
				}


			}

			//}
		}

		cplex.end();
		model.end();
		env.end();
	}
	/*	catch (IloCplex::InvalidCutException& ex)
		{
			cout << "Concert error: " << ex.getCut() << endl;
		}*/
	catch (IloException& e)
	{


		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
}
void Glass::FormulationStagesIIMinimoCorte(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes)
{
	int kk = 9;
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	IloRangeArray restrLazy(env);
	int maxtime = G_Time;
	IloNum max_time = maxtime; //tiempo del modelo

	char nombre[20];

	knapsack_bouknap();
	//	Creo las variables

	numtiras = 3;
	numpilas = 3;
	int numpiezasmax = 3;
	int numpieces = batch_items;
	numbins = nbins;
	printf("Máximo Tiras: Tiras=%d\t Pilas=%d \tPiezas=%d\n", numtiras, numpilas, numpiezasmax);

	int Tipo_objetivo = 2;
	//max_numero de defectos
	int max_nd = 10;
	//Variables defectos piezas
	NumVarMatrix6 zlr_dimjkl(env, max_nd);
	NumVarMatrix6 zlrr_dimjkl(env, max_nd);
	NumVarMatrix6 zll_dimjkl(env, max_nd);
	NumVarMatrix6 zllr_dimjkl(env, max_nd);

	NumVarMatrix6 zla_dimjkl(env, max_nd);
	NumVarMatrix6 zlar_dimjkl(env, max_nd);
	NumVarMatrix6 zle_dimjkl(env, max_nd);
	NumVarMatrix6 zler_dimjkl(env, max_nd);

	//Variables defectos
	NumVarMatrix5 zr_dmjkl(env, max_nd);
	NumVarMatrix4 ze_djkl(env, max_nd);
	NumVarMatrix3 zr_dkl(env, max_nd);
	//minimo defecto pilas


	NumVarMatrix3 es_jkl(env, numpilas);
	NumVarMatrix4 el_ijkl(env, numpieces);
	NumVarMatrix5 es_mijkl(env, numpiezasmax);
	NumVarMatrix4 ms_mjkl(env, numpiezasmax);
	NumVarMatrix3 ml_jkl(env, numpilas);
	NumVarMatrix2 ms_kl(env, numtiras);

	NumVarMatrix5 x_imjkl(env, numpieces);
	NumVarMatrix5 xr_imjkl(env, numpieces);

	NumVarMatrix2 y_kl(env, numtiras);
	NumVarMatrix4 w_mjkl(env, numpiezasmax);
	NumVarMatrix3 h_jkl(env, numpilas);
	NumVarMatrix2 w_kl(env, numtiras);
	NumVector y_l(env);
	//	int LastBinWidth = floor((Total_area - ((numbins - 1)*plate_h*plate_w)) / plate_h);
	//	LastBinWidth = 0;
	int LastBinWidth = max((double)floor((Total_area - ((numbins - 1) * plate_h * plate_w)) / plate_h), (double)0);
	int maxh = 0;
	Lazy = false;
	/**********************************************************/
	/***************** INICIALIZO VARIABLES *******************/
	/**********************************************************/
	//inicializo la variable x_ijkl
	for (int i = 0; i < numpieces; i++)
	{
		if (items[i].Getitem_h() > maxh)
			maxh = items[i].Getitem_h();
		if (items[i].Getitem_w() > maxh)
			maxh = items[i].Getitem_w();
		x_imjkl[i] = NumVarMatrix4(env, numpiezasmax);
		for (int m = 0; m < numpiezasmax; m++)
		{
			x_imjkl[i][m] = NumVarMatrix3(env, numpilas);

			for (int j = 0; j < numpilas; j++)
			{
				x_imjkl[i][m][j] = NumVarMatrix(env, numtiras);

				for (int k = 0; k < numtiras; k++)
				{
					x_imjkl[i][m][j][k] = IloIntVarArray(env, numbins);
					for (int l = 0; l < numbins; l++)
					{
						x_imjkl[i][m][j][k][l] = IloIntVar(env, 0, 1);
						sprintf(nombre, "x_%d_%d_%d_%d_%d", i, m, j, k, l);
						x_imjkl[i][m][j][k][l].setName(nombre);

					}

				}
			}
		}
	}
	//inicializo variables rotadas
	for (int i = 0; i < numpieces; i++)
	{
		xr_imjkl[i] = NumVarMatrix4(env, numpiezasmax);
		for (int m = 0; m < numpiezasmax; m++)
		{
			xr_imjkl[i][m] = NumVarMatrix3(env, numpilas);

			for (int j = 0; j < numpilas; j++)
			{
				xr_imjkl[i][m][j] = NumVarMatrix(env, numtiras);

				for (int k = 0; k < numtiras; k++)
				{
					xr_imjkl[i][m][j][k] = IloIntVarArray(env, numbins);
					for (int l = 0; l < numbins; l++)
					{
						xr_imjkl[i][m][j][k][l] = IloIntVar(env, 0, 1);
						sprintf(nombre, "xr_%d_%d_%d_%d_%d", i, m, j, k, l);
						xr_imjkl[i][m][j][k][l].setName(nombre);

					}

				}
			}
		}
	}
	//inicializo las varialbes de cada tira
	for (int m = 0; m < numpiezasmax; m++)
	{
		w_mjkl[m] = NumVarMatrix3(env, numpilas);

		for (int j = 0; j < numpilas; j++)
		{
			w_mjkl[m][j] = NumVarMatrix(env, numtiras);

			for (int k = 0; k < numtiras; k++)
			{
				w_mjkl[m][j][k] = IloIntVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					w_mjkl[m][j][k][l] = IloIntVar(env, 0, max1Cut);
					sprintf(nombre, "w_%d_%d_%d_%d", m, j, k, l);
					w_mjkl[m][j][k][l].setName(nombre);

				}

			}
		}
	}
	//inicializo la variable h_jkl
	for (int i = 0; i < numpilas; i++)
	{
		h_jkl[i] = NumVarMatrix(env, numtiras);

		for (int j = 0; j < numtiras; j++)
		{
			h_jkl[i][j] = IloIntVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				h_jkl[i][j][l] = IloIntVar(env, 0, maxh);
				sprintf(nombre, "h_%d_%d_%d", i, j, l);
				h_jkl[i][j][l].setName(nombre);

			}

		}
	}
	//inicializo la variable w_kl y y_kl
	for (int i = 0; i < numtiras; i++)
	{
		w_kl[i] = IloIntVarArray(env, numbins);
		y_kl[i] = IloIntVarArray(env, numbins);
		ms_kl[i] = IloIntVarArray(env, numbins);
		for (int j = 0; j < numbins; j++)
		{
			w_kl[i][j] = IloIntVar(env, 0, max1Cut);
			sprintf(nombre, "W_%d_%d", i, j);
			w_kl[i][j].setName(nombre);
			y_kl[i][j] = IloIntVar(env, 0, 1);
			sprintf(nombre, "Y_%d_%d", i, j);
			y_kl[i][j].setName(nombre);
			ms_kl[i][j] = IloIntVar(env, 0, 1);
			sprintf(nombre, "ms_%d_%d", i, j);
			ms_kl[i][j].setName(nombre);

		}
	}

	//inicializo las variable y_l
	y_l = IloIntVarArray(env, numbins, 0, 1);
	for (int l = 0; l < numbins; l++)
	{
		sprintf(nombre, "Y%d", l);
		y_l[l].setName(nombre);
	}

	//minimco corte
	//inicializo variables rotadas
	for (int i = 0; i < numpieces; i++)
	{
		el_ijkl[i] = NumVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			el_ijkl[i][j] = NumVarMatrix(env, numtiras);

			for (int k = 0; k < numtiras; k++)
			{
				el_ijkl[i][j][k] = IloIntVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					el_ijkl[i][j][k][l] = IloIntVar(env, 0, 1);
					sprintf(nombre, "Xel_%d_%d_%d_%d", i, j, k, l);
					el_ijkl[i][j][k][l].setName(nombre);

				}

			}
		}
	}
	//inicializo la variable es_jkl
	for (int i = 0; i < numpilas; i++)
	{
		es_jkl[i] = NumVarMatrix(env, numtiras);
		ml_jkl[i] = NumVarMatrix(env, numtiras);
		for (int j = 0; j < numtiras; j++)
		{
			es_jkl[i][j] = IloIntVarArray(env, numbins);
			ml_jkl[i][j] = IloIntVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				es_jkl[i][j][l] = IloIntVar(env, 0, 1);
				sprintf(nombre, "Xes_%d_%d_%d", i, j, l);
				es_jkl[i][j][l].setName(nombre);
				ml_jkl[i][j][l] = IloIntVar(env, 0, 1);
				sprintf(nombre, "ml_%d_%d_%d", i, j, l);
				ml_jkl[i][j][l].setName(nombre);

			}

		}
	}

	//inicializo las variables de los cortes

	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			zr_dkl[i] = NumVarMatrix(env, numtiras);

			for (int j = 0; j < numtiras; j++)
			{
				zr_dkl[i][j] = IloIntVarArray(env, 1);
				zr_dkl[i][j][l] = IloIntVar(env, 0, 1);
				sprintf(nombre, "zr_%d_%d_%d", i, j, l);
				zr_dkl[i][j][l].setName(nombre);

			}
		}
	}
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			zr_dmjkl[i] = NumVarMatrix4(env, numpiezasmax);

			for (int m = 0; m < numpiezasmax; m++)
			{
				zr_dmjkl[i][m] = NumVarMatrix3(env, numpilas);

				for (int j = 0; j < numpilas; j++)
				{
					zr_dmjkl[i][m][j] = NumVarMatrix(env, numtiras);
					for (int k = 0; k < numtiras; k++)
					{

						zr_dmjkl[i][m][j][k] = IloIntVarArray(env, 1);
						zr_dmjkl[i][m][j][k][l] = IloIntVar(env, 0, 1);
						sprintf(nombre, "zr_%d_%d_%d_%d_%d", i, m, j, k, l);
						zr_dmjkl[i][m][j][k][l].setName(nombre);
					}

				}
			}
		}
	}
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();
		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			ze_djkl[i] = NumVarMatrix3(env, numpilas);

			for (int j = 0; j < numpilas; j++)
			{
				ze_djkl[i][j] = NumVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					ze_djkl[i][j][k] = IloIntVarArray(env, 1);
					ze_djkl[i][j][k][l] = IloIntVar(env, 0, 1);
					sprintf(nombre, "ze_%d_%d_%d_%d", i, j, k, l);
					ze_djkl[i][j][k][l].setName(nombre);
				}

			}
		}
	}

	//inicializo las variables de los defectos
	//derecha e izquierda
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();
		int d = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
		{
			zlr_dimjkl[d] = NumVarMatrix5(env, numpieces);
			zlrr_dimjkl[d] = NumVarMatrix5(env, numpieces);
			zll_dimjkl[d] = NumVarMatrix5(env, numpieces);
			zllr_dimjkl[d] = NumVarMatrix5(env, numpieces);
			for (int i = 0; i < numpieces; i++)
			{

				zlr_dimjkl[d][i] = NumVarMatrix4(env, numpiezasmax);
				zlrr_dimjkl[d][i] = NumVarMatrix4(env, numpiezasmax);
				zll_dimjkl[d][i] = NumVarMatrix4(env, numpiezasmax);
				zllr_dimjkl[d][i] = NumVarMatrix4(env, numpiezasmax);
				for (int m = 0; m < numpiezasmax; m++)
				{
					zlr_dimjkl[d][i][m] = NumVarMatrix3(env, numpilas);
					zlrr_dimjkl[d][i][m] = NumVarMatrix3(env, numpilas);
					zll_dimjkl[d][i][m] = NumVarMatrix3(env, numpilas);
					zllr_dimjkl[d][i][m] = NumVarMatrix3(env, numpilas);
					for (int j = 0; j < numpilas; j++)
					{
						zlr_dimjkl[d][i][m][j] = NumVarMatrix(env, numtiras);
						zlrr_dimjkl[d][i][m][j] = NumVarMatrix(env, numtiras);
						zll_dimjkl[d][i][m][j] = NumVarMatrix(env, numtiras);
						zllr_dimjkl[d][i][m][j] = NumVarMatrix(env, numtiras);
						for (int k = 0; k < numtiras; k++)
						{
							zlr_dimjkl[d][i][m][j][k] = IloIntVarArray(env, 1);
							zlr_dimjkl[d][i][m][j][k][l] = IloIntVar(env, 0, 1);
							sprintf(nombre, "zlr_%d_%d_%d_%d_%d_%d", d, i, m, j, k, l);
							zlr_dimjkl[d][i][m][j][k][l].setName(nombre);

							zlrr_dimjkl[d][i][m][j][k] = IloIntVarArray(env, 1);
							zlrr_dimjkl[d][i][m][j][k][l] = IloIntVar(env, 0, 1);
							sprintf(nombre, "zlrr_%d_%d_%d_%d_%d_%d", d, i, m, j, k, l);
							zlrr_dimjkl[d][i][m][j][k][l].setName(nombre);

							zll_dimjkl[d][i][m][j][k] = IloIntVarArray(env, 1);
							zll_dimjkl[d][i][m][j][k][l] = IloIntVar(env, 0, 1);
							sprintf(nombre, "zll_%d_%d_%d_%d_%d_%d", d, i, m, j, k, l);
							zll_dimjkl[d][i][m][j][k][l].setName(nombre);

							zllr_dimjkl[d][i][m][j][k] = IloIntVarArray(env, 1);
							zllr_dimjkl[d][i][m][j][k][l] = IloIntVar(env, 0, 1);
							sprintf(nombre, "zllr_%d_%d_%d_%d_%d_%d", d, i, m, j, k, l);
							zllr_dimjkl[d][i][m][j][k][l].setName(nombre);


						}

					}
				}
			}
		}
	}

	//Arriba o abajo
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();
		int d = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
		{
			zla_dimjkl[d] = NumVarMatrix5(env, numpieces);
			zlar_dimjkl[d] = NumVarMatrix5(env, numpieces);
			zle_dimjkl[d] = NumVarMatrix5(env, numpieces);
			zler_dimjkl[d] = NumVarMatrix5(env, numpieces);
			for (int i = 0; i < numpieces; i++)
			{
				zla_dimjkl[d][i] = NumVarMatrix4(env, numpiezasmax);
				zlar_dimjkl[d][i] = NumVarMatrix4(env, numpiezasmax);
				zle_dimjkl[d][i] = NumVarMatrix4(env, numpiezasmax);
				zler_dimjkl[d][i] = NumVarMatrix4(env, numpiezasmax);
				for (int m = 0; m < numpiezasmax; m++)
				{
					zla_dimjkl[d][i][m] = NumVarMatrix3(env, numpilas);
					zlar_dimjkl[d][i][m] = NumVarMatrix3(env, numpilas);
					zle_dimjkl[d][i][m] = NumVarMatrix3(env, numpilas);
					zler_dimjkl[d][i][m] = NumVarMatrix3(env, numpilas);
					for (int j = 0; j < numpilas; j++)
					{
						zla_dimjkl[d][i][m][j] = NumVarMatrix(env, numtiras);
						zlar_dimjkl[d][i][m][j] = NumVarMatrix(env, numtiras);
						zle_dimjkl[d][i][m][j] = NumVarMatrix(env, numtiras);
						zler_dimjkl[d][i][m][j] = NumVarMatrix(env, numtiras);
						for (int k = 0; k < numtiras; k++)
						{
							zla_dimjkl[d][i][m][j][k] = IloIntVarArray(env, 1);
							zla_dimjkl[d][i][m][j][k][l] = IloIntVar(env, 0, 1);
							sprintf(nombre, "zla_%d_%d_%d_%d_%d_%d", d, i, m, j, k, l);
							zla_dimjkl[d][i][m][j][k][l].setName(nombre);

							zlar_dimjkl[d][i][m][j][k] = IloIntVarArray(env, 1);
							zlar_dimjkl[d][i][m][j][k][l] = IloIntVar(env, 0, 1);
							sprintf(nombre, "zlar_%d_%d_%d_%d_%d_%d", d, i, m, j, k, l);
							zlar_dimjkl[d][i][m][j][k][l].setName(nombre);

							zle_dimjkl[d][i][m][j][k] = IloIntVarArray(env, 1);
							zle_dimjkl[d][i][m][j][k][l] = IloIntVar(env, 0, 1);
							sprintf(nombre, "zle_%d_%d_%d_%d_%d_%d", d, i, m, j, k, l);
							zle_dimjkl[d][i][m][j][k][l].setName(nombre);

							zler_dimjkl[d][i][m][j][k] = IloIntVarArray(env, 1);
							zler_dimjkl[d][i][m][j][k][l] = IloIntVar(env, 0, 1);
							sprintf(nombre, "zler_%d_%d_%d_%d_%d_%d", d, i, m, j, k, l);
							zler_dimjkl[d][i][m][j][k][l].setName(nombre);


						}

					}
				}
			}
		}
	}

	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/

//	Lazy_minimo = false;
	//Todos los productos colocados
	for (int i = 0; i < numpieces; i++)
	{
		IloExpr v1(env);
		for (int j = 0; j < numpilas; j++)
		{
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						v1 += x_imjkl[i][m][j][k][l];
						v1 += xr_imjkl[i][m][j][k][l];
					}
				}
			}
		}
		if (Tipo_objetivo != 3)
			restr.add(IloRange(env, 1, v1, 1));
		else
			restr.add(IloRange(env, -IloInfinity, v1, 1));

		v1.end();
	}
	//Si coloco la segunda pieza también la primera
	//solamente una por celda


	for (int j = 0; j < numpilas; j++)
	{
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					IloExpr v1(env);
					IloExpr v2(env);
					for (int i = 0; i < numpieces; i++)
					{

						v2 += x_imjkl[i][m][j][k][l];
						v2 += xr_imjkl[i][m][j][k][l];

						v1 += x_imjkl[i][m][j][k][l];
						v1 += xr_imjkl[i][m][j][k][l];
						if (m > 0)
						{
							v1 += (-1) * x_imjkl[i][m - 1][j][k][l];
							v1 += (-1) * xr_imjkl[i][m - 1][j][k][l];
						}

					}
					//V1 indica que si ponemos la pieza m, antes también tiene que haber piezas
					if (m > 0)
					{
						restr.add(IloRange(env, -IloInfinity, v1, 0));
						v1.end();
					}
					restr.add(IloRange(env, -IloInfinity, v2, 1));
					v2.end();



				}
			}
		}
	}


	//Altura mínima altura
	for (int i = 0; i < numpieces; i++)
	{
		for (int m = 0; m < numpiezasmax; m++)
		{
			for (int j = 0; j < numpilas; j++)
			{

				for (int k = 0; k < numtiras; k++)
				{
					for (int l = 0; l < numbins; l++)
					{
						IloExpr v1(env);

						v1 += items[i].Getitem_h() * x_imjkl[i][m][j][k][l];
						v1 += items[i].Getitem_w() * xr_imjkl[i][m][j][k][l];
						v1 += (-1) * h_jkl[j][k][l];
						v1 += waste_min * (1 - el_ijkl[i][j][k][l]);
						restr.add(IloRange(env, -IloInfinity, v1, 0));
						v1.end();
						IloExpr v2(env);

						v2 += items[i].Getitem_h() * x_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_w() * xr_imjkl[i][m][j][k][l];
						v2 += (-1) * h_jkl[j][k][l];
						v2 += plate_h * (1 - el_ijkl[i][j][k][l]);
						if (Lazy_minimo)
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						else
							restr.add(IloRange(env, 0, v2, IloInfinity));

						v2.end();

					}
				}
			}
		}

	}

	//Anchura de los cortes en cada pila


	for (int j = 0; j < numpilas; j++)
	{

		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					IloExpr v2(env);
					for (int i = 0; i < numpieces; i++)
					{

						v2 += items[i].Getitem_w() * x_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_h() * xr_imjkl[i][m][j][k][l];

					}
					v2 += (-1) * w_mjkl[m][j][k][l];

					restr.add(IloRange(env, -IloInfinity, v2, 0));
					v2.end();


				}
			}
		}
	}



	//suma de las alturas de las pilas menor H


	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			IloExpr v1(env);
			for (int j = 0; j < numpilas; j++)
			{


				v1 += h_jkl[j][k][l];
			}
			v1 += (-1) * plate_h * y_kl[k][l];
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
		}
	}


	//suma de minimo corte anchos de las tiras
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			for (int j = 0; j < numpilas; j++)
			{

				IloExpr v1(env);
				IloExpr v2(env);
				for (int m = 0; m < numpiezasmax; m++)
				{
					v1 += w_mjkl[m][j][k][l];
					v2 += w_mjkl[m][j][k][l];

				}

				v1 += (-1) * w_kl[k][l];
				v1 += (1 - es_jkl[j][k][l]) * waste_min;

				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();

				v2 += (-1) * w_kl[k][l];
				v2 += (1 - es_jkl[j][k][l]) * max1Cut;

				if (Lazy_minimo)
					restrLazy.add(IloRange(env, 0, v2, IloInfinity));

				else
					restr.add(IloRange(env, 0, v2, IloInfinity));
				v2.end();
			}

		}
	}
	//minimos cortes
		//suma de las alturas de las pilas menor H

//	Lazy_minimo = true;
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{

			for (int j = 0; j < numpilas; j++)
			{
				//				Lazy_minimo = true;
				IloExpr v1(env);
				v1 += min2Cut * ml_jkl[j][k][l];

				v1 += (-1) * h_jkl[j][k][l];

				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
				IloExpr v2(env);
				v2 += plate_h * ml_jkl[j][k][l];
				v2 += (-1) * h_jkl[j][k][l];
				if (Lazy_minimo)
					restrLazy.add(IloRange(env, 0, v2, IloInfinity));
				else
					restr.add(IloRange(env, 0, v2, IloInfinity));
				v2.end();
				//no crea pilas vacías y además ordenadas
				IloExpr v3(env);
				v3 += ml_jkl[j][k][l];
				for (int i = 0; i < numpieces; i++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						v3 += (-1) * x_imjkl[i][m][j][k][l];
						v3 += (-1) * xr_imjkl[i][m][j][k][l];
					}
				}
				restr.add(IloRange(env, -IloInfinity, v3, 0));
				v3.end();

				if (j > 0)
				{
					IloExpr v4(env);
					v4 += ml_jkl[j][k][l];
					v4 += (-1) * ml_jkl[j - 1][k][l];
					restr.add(IloRange(env, -IloInfinity, v4, 0));
					v4.end();
				}




			}
			//			Lazy_minimo = false;
			IloExpr v1(env);
			v1 += min1Cut * ms_kl[k][l];

			v1 += (-1) * w_kl[k][l];

			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
			IloExpr v2(env);
			v2 += max1Cut * ms_kl[k][l];
			v2 += (-1) * w_kl[k][l];
			if (Lazy_minimo)
				restrLazy.add(IloRange(env, 0, v2, IloInfinity));
			else
				restr.add(IloRange(env, 0, v2, IloInfinity));
			v2.end();

			//no crea tiras vacías y además ordenadas
			IloExpr v3(env);
			v3 += ms_kl[k][l];
			for (int i = 0; i < numpieces; i++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					for (int j = 0; j < numpilas; j++)
					{
						v3 += (-1) * x_imjkl[i][m][j][k][l];
						v3 += (-1) * xr_imjkl[i][m][j][k][l];
					}
				}
			}
			restr.add(IloRange(env, -IloInfinity, v3, 0));
			v3.end();
			if (k > 0)
			{
				IloExpr v4(env);
				v4 += ms_kl[k][l];
				v4 += (-1) * ms_kl[k - 1][l];
				restr.add(IloRange(env, -IloInfinity, v4, 0));
				v4.end();
			}
		}
	}

	//Suma  tiras menor o igual a W
	for (int l = 0; l < numbins; l++)
	{
		IloExpr v1(env);
		for (int k = 0; k < numtiras; k++)
		{

			v1 += w_kl[k][l];

		}

		v1 += (-1) * plate_w * y_l[l];
		restr.add(IloRange(env, -IloInfinity, v1, 0));
		v1.end();
	}

	//Orden de los stacks por parejas
	for (int l = 0; l < numbins; l++)
	{

		for (int k = 0; k < numtiras; k++)
		{

			for (int j = 0; j < numpilas; j++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					for (int i = numpieces - 1; i > 0; i--)
					{


						if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1 && (items[i].Getitem_seq() > 1))
						{
							int kk = stacks[items[i].Getitem_stack()].Getitem_nbr();
							IloExpr v1(env);
							v1 += x_imjkl[i][m][j][k][l];
							v1 += xr_imjkl[i][m][j][k][l];

							// ponemos las variables
							int i2 = i - 1;
							if (i2 >= 0 && items[i].Getitem_stack() == items[i2].Getitem_stack())
							{

								for (int l2 = 0; l2 <= l; l2++)
								{
									for (int k2 = 0; k2 < numtiras; k2++)
									{
										if (l2 == l && k2 > k)
											break;
										for (int j2 = 0; j2 < numpilas; j2++)
										{
											if (l2 == l && k2 == k && j2 > j)
												break;
											for (int m2 = 0; m2 < numpiezasmax; m2++)
											{
												if (l2 == l && k2 == k && j2 == j && m2 > m)
													break;

												v1 += (-1) * x_imjkl[i2][m2][j2][k2][l2];
												v1 += (-1) * xr_imjkl[i2][m2][j2][k2][l2];

											}
										}

									}
								}

							}
							if (Lazy)
								restrLazy.add(IloRange(env, -IloInfinity, v1, 0));

							else
								restr.add(IloRange(env, -IloInfinity, v1, 0));
							v1.end();




						}



					}
				}
			}
		}
	}


	//restricciones de los cortes verticales
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{

			for (int k1 = 0; k1 < numtiras; k1++)
			{
				IloExpr v1(env);
				IloExpr v2(env);
				for (int k2 = 0; k2 <= k1; k2++)
				{
					v1 += w_kl[k2][l];
					v2 += w_kl[k2][l];
				}
				v1 += (-1) * ((*itD).Getpos_x());

				v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1][l]);
				//			v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth());
				v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zr_dkl[i][k1][l]);


				if (Lazy_cortes)
				{
					restrLazy.add(IloRange(env, 0, v2, IloInfinity));
					restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
				}
				else
				{
					restr.add(IloRange(env, 0, v2, IloInfinity));
					restr.add(IloRange(env, -IloInfinity, v1, 0));
				}
				v1.end();
				v2.end();
				if (k1 > 0)
				{
					IloExpr v3(env);
					v3 += zr_dkl[i][k1][l];
					v3 += (-1) * zr_dkl[i][k1 - 1][l];
					restr.add(IloRange(env, -IloInfinity, v3, 0));
					v3.end();
				}
			}



		}
	}

	//restricciones de los cortes horizontales
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			for (int k = 0; k < numtiras; k++)
			{

				for (int j1 = 0; j1 < numpilas; j1++)
				{
					IloExpr v1(env);
					IloExpr v2(env);
					for (int j2 = 0; j2 <= j1; j2++)
					{

						v1 += h_jkl[j2][k][l];
						v2 += h_jkl[j2][k][l];
					}



					v1 += (-1) * ((*itD).Getpos_y());
					v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - ze_djkl[i][j1][k][l]);
					v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (zr_dkl[i][k][l]);
					if (k > 0)
					{
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zr_dkl[i][k - 1][l]);
					}

					//ahora falta la otra parte
					int rtyd = (*itD).Getpos_y() + (*itD).Getheight();
					v2 += (-1) * (rtyd) * (1 - ze_djkl[i][j1][k][l]);
					v2 += (rtyd) * (zr_dkl[i][k][l]);
					if (k > 0)
					{
						v2 += rtyd * (1 - zr_dkl[i][k - 1][l]);
					}

					if (Lazy_cortes)
					{
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
					}
					else
					{
						restr.add(IloRange(env, 0, v2, IloInfinity));
						restr.add(IloRange(env, -IloInfinity, v1, 0));
					}
					v1.end();
					v2.end();
					if (j1 > 0)
					{
						IloExpr v3(env);
						v3 += ze_djkl[i][j1][k][l];
						v3 += (-1) * ze_djkl[i][j1 - 1][k][l];
						restr.add(IloRange(env, -IloInfinity, v3, 0));
						v3.end();


					}
					IloExpr v4(env);
					v4 += ze_djkl[i][j1][k][l];
					if (k > 0)
						v4 += (-1) * (zr_dkl[i][k - 1][l] - zr_dkl[i][k][l]);
					else
						v4 += (-1) * (1 - zr_dkl[i][k][l]);
					restr.add(IloRange(env, -IloInfinity, v4, 0));
					v4.end();


				}
				//orden de las variables
			}






		}
	}


	//restricciones de los cortes verticales de segundo orden de defecto
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{

			for (int k1 = 0; k1 < numtiras; k1++)
			{
				for (int j = 0; j < numpilas; j++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						IloExpr v1(env);
						IloExpr v2(env);
						if (k1 > 0)
						{
							for (int k2 = 0; k2 < k1; k2++)
							{

								v1 += w_kl[k2][l];
								v2 += w_kl[k2][l];
							}
						}
						for (int m2 = 0; m2 <= m; m2++)
						{

							v1 += w_mjkl[m2][j][k1][l];
							v2 += w_mjkl[m2][j][k1][l];
						}
						v1 += (-1) * ((*itD).Getpos_x());
						v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dmjkl[i][m][j][k1][l]);
						v1 += (-1) * (plate_w - (*itD).Getpos_x()) * ze_djkl[i][j][k1][l];
						if (j > 0)
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - ze_djkl[i][j - 1][k1][l]);
						int rtxd = (*itD).Getpos_x() + (*itD).Getwidth();
						v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zr_dmjkl[i][m][j][k1][l]);
						v2 += (rtxd)* ze_djkl[i][j][k1][l];
						if (j > 0)
							v2 += (rtxd) * (1 - ze_djkl[i][j - 1][k1][l]);
						if (Lazy_cortes)
						{
							restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						}
						else
						{
							restr.add(IloRange(env, -IloInfinity, v1, 0));
							restr.add(IloRange(env, 0, v2, IloInfinity));
						}
						v1.end();
						v2.end();
						if (m > 0)
						{
							IloExpr v3(env);
							v3 += zr_dmjkl[i][m][j][k1][l];
							v3 += (-1) * zr_dmjkl[i][m - 1][j][k1][l];
							restr.add(IloRange(env, -IloInfinity, v3, 0));
							v3.end();
						}
						//if (m ==0)
						//{
						//	IloExpr v3(env);
						//	v3 += zr_dkl[i][k1][l];
						//	v3 += (-1) * zr_dmjkl[i][numpiezasmax-1][j][k1][l];
						//	restr.add(IloRange(env, -IloInfinity, v3, 0));
						//	v3.end();
						//}

						IloExpr v4(env);
						v4 += zr_dmjkl[i][m][j][k1][l];
						if (j > 0)
							v4 += (-1) * (ze_djkl[i][j - 1][k1][l] - ze_djkl[i][j][k1][l]);
						else
							v4 += (-1) * (1 - ze_djkl[i][j][k1][l]);
						restr.add(IloRange(env, -IloInfinity, v4, 0));
						v4.end();

					}
				}
			}



		}
	}



	//restricciones de los defectos
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int d = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
		{

			for (int j = 0; j < numpilas; j++)
			{

				for (int k = 0; k < numtiras; k++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						IloExpr v1(env);
						IloExpr v2(env);
						//						if (k2<k)
						//						v1 += w_kl[k1][l];
						//						v2 += w_kl[k1][l];
						if (k > 0)
						{
							for (int k2 = 0; k2 < k; k2++)
							{

								v1 += w_kl[k2][l];
								v2 += w_kl[k2][l];
							}
						}
						for (int m2 = 0; m2 <= m; m2++)
						{
							if (m2 < m)
								v1 += w_mjkl[m2][j][k][l];
							v2 += w_mjkl[m2][j][k][l];
						}
						for (int i = 0; i < numpieces; i++)
						{
							v1 += items[i].Getitem_w() * zlr_dimjkl[d][i][m][j][k][l];
							v1 += items[i].Getitem_h() * zlrr_dimjkl[d][i][m][j][k][l];


							v2 += (-1) * items[i].Getitem_w() * zll_dimjkl[d][i][m][j][k][l];
							v2 += (-1) * items[i].Getitem_h() * zllr_dimjkl[d][i][m][j][k][l];


							IloExpr v5(env);
							IloExpr v6(env);
							v5 += zlrr_dimjkl[d][i][m][j][k][l];
							v5 += zllr_dimjkl[d][i][m][j][k][l];
							v5 += zlar_dimjkl[d][i][m][j][k][l];
							v5 += zler_dimjkl[d][i][m][j][k][l];


							v6 += zlr_dimjkl[d][i][m][j][k][l];
							v6 += zll_dimjkl[d][i][m][j][k][l];
							v6 += zla_dimjkl[d][i][m][j][k][l];
							v6 += zle_dimjkl[d][i][m][j][k][l];
							v5 += (-1) * xr_imjkl[i][m][j][k][l];
							v6 += (-1) * x_imjkl[i][m][j][k][l];
							//						if (Lazy_cortes)
							//							restrLazy.add(IloRange(env, 0, v5, 0));
							//						else
							restr.add(IloRange(env, 0, v5, 1));
							v5.end();
							restr.add(IloRange(env, 0, v6, 1));
							v6.end();
						}
						v1 += (-1) * (*itD).Getpos_x();
						v1 += (-1) * (plate_w - (*itD).Getpos_x()) * ze_djkl[d][j][k][l];
						v1 += (-1) * (plate_w - (*itD).Getpos_x()) * zr_dmjkl[d][m][j][k][l];
						if (j > 0)
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - ze_djkl[d][j - 1][k][l]);
						if (m > 0)
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dmjkl[d][m - 1][j][k][l]);
						else
						{
							if (k > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[d][k - 1][l]);
						}
						int rtxd = (*itD).Getpos_x() + (*itD).Getwidth();
						v2 += (-1) * rtxd;
						v2 += (rtxd)* ze_djkl[d][j][k][l];
						v2 += (rtxd)* zr_dmjkl[d][m][j][k][l];
						if (j > 0)
							v2 += (rtxd) * (1 - ze_djkl[d][j - 1][k][l]);
						if (m > 0)
							v2 += (rtxd) * (1 - zr_dmjkl[d][m - 1][j][k][l]);
						else
						{
							if (k > 0)
								v2 += (rtxd) * (1 - zr_dkl[d][k - 1][l]);
						}
						//						if (m>0)
						//						restr.add(IloRange(env, -IloInfinity, v1, 0));
						restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
						if (Lazy_cortes)
						{


							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						}
						else
						{


							restr.add(IloRange(env, 0, v2, IloInfinity));
						}



						v1.end();
						v2.end();
					}
					//orden de las variables



				}
				//orden de las variables
			}


		}



	}




	//restricciones de arriba y abajo
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int d = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
		{

			for (int j = 0; j < numpilas; j++)
			{

				for (int k = 0; k < numtiras; k++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						IloExpr v1(env);
						IloExpr v2(env);
						for (int j2 = 0; j2 <= j; j2++)
						{
							if (j2 < j)
								v1 += h_jkl[j2][k][l];
							v2 += h_jkl[j2][k][l];
						}
						for (int i = 0; i < numpieces; i++)
						{
							v1 += items[i].Getitem_h() * zle_dimjkl[d][i][m][j][k][l];
							v1 += items[i].Getitem_w() * zler_dimjkl[d][i][m][j][k][l];
							v2 += (-1) * items[i].Getitem_h() * zla_dimjkl[d][i][m][j][k][l];
							v2 += (-1) * items[i].Getitem_w() * zlar_dimjkl[d][i][m][j][k][l];

						}
						v1 += (-1) * (*itD).Getpos_y();
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * ze_djkl[d][j][k][l];
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * zr_dkl[d][k][l];
						if (j > 0)
							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - ze_djkl[d][j - 1][k][l]);

						if (m > 0)
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dmjkl[d][m - 1][j][k][l]);
						else
						{
							if (k > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[d][k - 1][l]);
						}


						int rtyd = (*itD).Getpos_y() + (*itD).Getheight();
						v2 += (-1) * rtyd;
						v2 += (rtyd)* ze_djkl[d][j][k][l];
						v2 += (rtyd)* zr_dkl[d][k][l];
						if (j > 0)
							v2 += (rtyd) * (1 - ze_djkl[d][j - 1][k][l]);



						if (m > 0)
							v2 += (rtyd) * (1 - zr_dmjkl[d][m - 1][j][k][l]);
						else
						{
							if (k > 0)
								v2 += (rtyd) * (1 - zr_dkl[d][k - 1][l]);
						}
						restr.add(IloRange(env, -IloInfinity, v1, 0));
						if (Lazy_cortes)
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						else
							restr.add(IloRange(env, 0, v2, IloInfinity));



						v1.end();
						v2.end();
					}

				}
				//orden de las variables
			}






		}
	}


	model.add(restr);
	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
//	model.add(restr);


	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);
	if (Tipo_objetivo == 0)//biobjective //estaría en diet.ccp
	{
		IloExpr Expression_fobj1(env);
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj1 += y_l[i];
		}
		IloExpr Expression_fobj2(env);
		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj2 += w_kl[k][numbins - 1];
		}
	}

	if (Tipo_objetivo == 1)//por bins
	{
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj += y_l[i];
		}
	}
	if (Tipo_objetivo == 2)//por tira del último bin
	{

		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj += w_kl[k][numbins - 1];
		}
	}
	if (Tipo_objetivo == 3)//por área de lo metido en el bin
	{
		Expression_fobj += plate_h * plate_w;
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int j = 0; j < numpilas; j++)
				{

					for (int m = 0; m < numpiezasmax; m++)
					{

						for (int i = 0; i < numpieces; i++)
						{
							Expression_fobj += (-1) * items[i].Getitem_area() * x_imjkl[i][m][j][k][l];
							Expression_fobj += (-1) * items[i].Getitem_area() * xr_imjkl[i][m][j][k][l];
						}
					}
				}
			}
		}
	}

	if (Tipo_objetivo != 0)
		model.add(IloMinimize(env, Expression_fobj));
	Expression_fobj.end();


	/***************************************************************/
	/*************** EJECUTAMOS EL MODELO **************************/
	/***************************************************************/
	try
	{

		IloCplex cplex(model);
		//Si queremos las lazy y tenemos alguna
		if (Lazy_minimo)
			cplex.addLazyConstraints(restrLazy);
		//		cplex.exportModel("Roadef.lp");
				//No hace el probing
				//cplex.setParam(IloCplex::Probe, -1);
				//1 CORE
		cplex.setParam(IloCplex::Threads, nt);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, max_time); //TIEMPO EN SEGUNDOS
		if (Tipo_objetivo == 2)
			cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, LastBinWidth);
		if (G_Dibujar)
		cplex.exportModel("Roadef.lp");

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

		//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();


		IloBool result = cplex.solve();

		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;

		//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
//				fprintf(f, "Infeasible\n");
				//				cplex.exportModel("Roadef.lp");
			}
			else
			{
//				if (cplex.getStatus() == IloAlgorithm::Unbounded) fprintf(f, "Unbounded\n");
//				fprintf(f, "\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}
//			fclose(f);
		}
		else
		{
//			FILE* f = fopen("./estadomodelo.txt", "a+");
//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			Best_Value_Formulation = cplex.getObjValue();
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible
			if (cplex.getSolnPoolNsolns() > 0)
			{
				int value2 = cplex.getObjValue();
				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							for (int m = 0; m < numpiezasmax; m++)
							{
								for (int i = 0; i < numpieces; i++)
								{


									if (cplex.isExtracted(x_imjkl[i][m][j][k][l]) && cplex.getValue(x_imjkl[i][m][j][k][l]) > 0.001)
									{
										printf("%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
//										fprintf(f, "%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
									}
									if (cplex.isExtracted(xr_imjkl[i][m][j][k][l]) && cplex.getValue(xr_imjkl[i][m][j][k][l]) > 0.001)
									{
										printf("%s %d w %d h %d s %d\n", xr_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
//										fprintf(f, "%s %d w %d h %d s %d\n", xr_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
									}

								}
							}

						}
					}
				}
				//Pinto las variables w_mjkl
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{
							for (int m = 0; m < numpiezasmax; m++)
							{
								if (cplex.isExtracted(w_mjkl[m][j][k][l]) && cplex.getValue(w_mjkl[m][j][k][l]) > 0.001)
								{
									printf("%s %2.f\n", w_mjkl[m][j][k][l].getName(), cplex.getValue(w_mjkl[m][j][k][l]));
//									fprintf(f, "%s %2.f\n", w_mjkl[m][j][k][l].getName(), cplex.getValue(w_mjkl[m][j][k][l]));
								}
							}

						}

					}
				}
				//Pinto las variables h_jkl
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							if (cplex.isExtracted(h_jkl[j][k][l]) && cplex.getValue(h_jkl[j][k][l]) > 0.001)
							{
								printf("%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
//								fprintf(f, "%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
							}

						}

					}
				}
				//Pinto las variables w_kl y y_kl
				for (int j = 0; j < numbins; j++)
				{
					for (int i = 0; i < numtiras; i++)
					{

						if (cplex.isExtracted(w_kl[i][j]) && cplex.getValue(w_kl[i][j]) > 0.001)
						{
							printf("%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
//							fprintf(f, "%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
						}
						if (cplex.isExtracted(y_kl[i][j]) && cplex.getValue(y_kl[i][j]) > 0.001)
						{
							printf("%s %d\n", y_kl[i][j].getName(), 1);
//							fprintf(f, "%s %d\n", y_kl[i][j].getName(), 1);
						}

					}
				}

				//Pinto las variable y_l
				for (int l = 0; l < numbins; l++)
				{
					if (cplex.isExtracted(y_l[l]) && cplex.getValue(y_l[l]) > 0.001)
					{
						printf("%s %d\n", y_l[l].getName(), 1);
//						fprintf(f, "%s %d\n", y_l[l].getName(), 1);
					}

				}
				//Pinto los cortes
				for (int l = 0; l < numbins; l++)
				{
					int nd = DefectsPlate[l].size();

					int d = 0;
					for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							for (int k = 0; k < numtiras; k++)
							{
								for (int m = 0; m < numpiezasmax; m++)
								{
									if (cplex.isExtracted(zr_dmjkl[d][m][j][k][l]) && cplex.getValue(zr_dmjkl[d][m][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zr_dmjkl[d][m][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zr_dmjkl[d][m][j][k][l].getName(), 1);
									}
									if (m == 0)
									{
										if (cplex.isExtracted(ze_djkl[d][j][k][l]) && cplex.getValue(ze_djkl[d][j][k][l]) > 0.001)
										{
											printf("%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
										}
									}
									if (j == 0 && m == 0)
									{
										if (cplex.isExtracted(zr_dkl[d][k][l]) && cplex.getValue(zr_dkl[d][k][l]) > 0.001)
										{
											printf("%s %d\n", zr_dkl[d][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", zr_dkl[d][k][l].getName(), 1);
										}

									}

								}

							}
						}
					}

				}
				//Pinto las variables zlr,zllr, zll,zlrr
				printf("\nVariables para los defectos\n");

				for (int l = 0; l < numbins; l++)
				{
					int nd = DefectsPlate[l].size();

					int d = 0;
					for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
					{
						for (int i = 0; i < numpieces; i++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								for (int k = 0; k < numtiras; k++)
								{
									for (int m = 0; m < numpiezasmax; m++)
									{
										if (cplex.isExtracted(zlr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlr_dimjkl[d][i][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zlr_dimjkl[d][i][m][j][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", zlr_dimjkl[d][i][m][j][k][l].getName(), 1);
										}
										if (cplex.isExtracted(zlrr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlrr_dimjkl[d][i][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zlrr_dimjkl[d][i][m][j][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", zlrr_dimjkl[d][i][m][j][k][l].getName(), 1);
										}
										if (cplex.isExtracted(zll_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zll_dimjkl[d][i][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zll_dimjkl[d][i][m][j][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", zll_dimjkl[d][i][m][j][k][l].getName(), 1);
										}
										if (cplex.isExtracted(zllr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zllr_dimjkl[d][i][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zllr_dimjkl[d][i][m][j][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", zllr_dimjkl[d][i][m][j][k][l].getName(), 1);
										}
										if (cplex.isExtracted(zla_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zla_dimjkl[d][i][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zla_dimjkl[d][i][m][j][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", zla_dimjkl[d][i][m][j][k][l].getName(), 1);
										}
										if (cplex.isExtracted(zlar_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlar_dimjkl[d][i][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zlar_dimjkl[d][i][m][j][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", zlar_dimjkl[d][i][m][j][k][l].getName(), 1);
										}
										if (cplex.isExtracted(zle_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zle_dimjkl[d][i][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zle_dimjkl[d][i][m][j][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", zle_dimjkl[d][i][m][j][k][l].getName(), 1);
										}
										if (cplex.isExtracted(zler_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zler_dimjkl[d][i][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zler_dimjkl[d][i][m][j][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", zler_dimjkl[d][i][m][j][k][l].getName(), 1);
										}
									}

								}
							}
						}
					}
				}

				printf("\nVariables de mínimo\n");
				for (int i = 0; i < numpieces; i++)
				{

					for (int j = 0; j < numpilas; j++)
					{

						for (int k = 0; k < numtiras; k++)
						{
							for (int l = 0; l < numbins; l++)
							{
								if (cplex.isExtracted(el_ijkl[i][j][k][l]) && cplex.getValue(el_ijkl[i][j][k][l]) > 0.001)
								{
									printf("%s %d\n", el_ijkl[i][j][k][l].getName(), 1);
//									fprintf(f, "%s %d\n", el_ijkl[i][j][k][l].getName(), 1);
								}
								if (i == 0)
								{
									if (cplex.isExtracted(es_jkl[j][k][l]) && cplex.getValue(es_jkl[j][k][l]) > 0.001)
									{
										printf("%s %d\n", es_jkl[j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", es_jkl[j][k][l].getName(), 1);
									}

								}

							}
						}
					}
				}
			}
			//Escribo la solución para dibujar
				//Inicio
			if (G_Dibujar || G_Draw_Exacto)
			{
				FILE* fin3;
				if (G_Draw_Exacto)
				{
					std::string str = file_idx + "_solution_" + std::to_string(G_Exacto) + "_" + std::to_string(Tipo_objetivo) + ".txt";
					fin3=fopen(P_file_to_write.c_str(), "w+");
//					fin3 = fopen(str.c_str(), "w+");
				}
				else
					fin3 = fopen("./temp2.txt", "w+");
				int ndefects = 0;
				int nbinsdefects = 0;
				for (int i = 0; i < numbins; i++)
				{
					if (DefectsPlate[i].size() >= 1)
						nbinsdefects++;
					for (std::list< GlassDefect > ::iterator it = DefectsPlate[i].begin(); it != DefectsPlate[i].end(); it++)
					{
						ndefects++;
					}
				}
				//					fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, Best_Objective_function);
				fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, numpieces, ndefects, nbinsdefects, LB_Best_Value_Formulation, Best_Value_Formulation);
				//defectos

				for (int i = 0; i < numbins; i++)
				{

					for (std::list< GlassDefect > ::iterator it = DefectsPlate[i].begin(); it != DefectsPlate[i].end(); it++)
					{
						// Get defect coordinates.
						int defect_x = (*it).Getpos_x();
						int defect_y = (*it).Getpos_y();
						int defect_width = (*it).Getwidth();
						int defect_height = (*it).Getheight();

						fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", defect_x, defect_y, defect_width, defect_height, i);
					}
				}
				for (int i = 0; i < numbins; i++)
				{
					if (DefectsPlate[i].size() > 0)
						fprintf(fin3, "%d\t%d\n", i, DefectsPlate[i].size());

				}
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							int pos_x = 0;
							int pos_X = 0;
							int pos_Y = 0;
							for (int k1 = 0; k1 < k; k1++)
							{
								if (cplex.isExtracted(w_kl[k1][l]) && cplex.getValue(w_kl[k1][l]) > 0.001)
								{
									pos_x += cplex.getValue(w_kl[k1][l]);
								}
							}
							int pos_y = 0;
							for (int j1 = 0; j1 < j; j1++)
							{
								if (cplex.isExtracted(h_jkl[j1][k][l]) && cplex.getValue(h_jkl[j1][k][l]) > 0.001)
								{
									pos_y += cplex.getValue(h_jkl[j1][k][l]);
								}
							}
							for (int m = 0; m < numpiezasmax; m++)
							{
								if (m > 0)
								{
									if (cplex.isExtracted(w_mjkl[m - 1][j][k][l]) && cplex.getValue(w_mjkl[m - 1][j][k][l]) > 0.001)
									{
										pos_x += cplex.getValue(w_mjkl[m - 1][j][k][l]);
									}
								}
								if (m < (numpiezasmax - 1))
								{
									pos_X = pos_x + cplex.getValue(w_mjkl[m][j][k][l]);
								}
								else
									pos_X = pos_x + cplex.getValue(w_kl[k][l]);
								pos_Y = pos_y + cplex.getValue(h_jkl[j][k][l]);

								bool puesta = false;
								for (int i = 0; i < numpieces && puesta == false; i++)
								{
									//buscar defecto que está dentro de este rango
									int d = 0;
									bool pegado_izquierda = false;
									bool pegado_derecha = false;
									bool pegado_abajo = false;
									bool pegado_arriba = false;
									for (std::list< GlassDefect > ::iterator it = DefectsPlate[l].begin(); it != DefectsPlate[l].end(); it++, d++)
									{
										if (((*it).Getpos_x() > pos_X) || ((*it).Getpos_x() + (*it).Getwidth() < pos_x)
											|| ((*it).Getpos_y() > pos_Y) || ((*it).Getpos_y() + (*it).Getheight() < pos_y))
										{
											continue;
										}
										//este está aquí dentro
										if ((cplex.isExtracted(zlr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlr_dimjkl[d][i][m][j][k][l]) > 0.001) ||
											(cplex.isExtracted(zlrr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlrr_dimjkl[d][i][m][j][k][l]) > 0.001))
										{
											pegado_derecha = true;
										}
										if ((cplex.isExtracted(zll_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zll_dimjkl[d][i][m][j][k][l]) > 0.001) ||
											(cplex.isExtracted(zllr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zllr_dimjkl[d][i][m][j][k][l]) > 0.001))
										{
											pegado_izquierda = true;
										}
										if ((cplex.isExtracted(zla_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zla_dimjkl[d][i][m][j][k][l]) > 0.001) ||
											(cplex.isExtracted(zlar_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlar_dimjkl[d][i][m][j][k][l]) > 0.001))
										{
											pegado_arriba = true;
										}
										if ((cplex.isExtracted(zle_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zle_dimjkl[d][i][m][j][k][l]) > 0.001) ||
											(cplex.isExtracted(zler_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zler_dimjkl[d][i][m][j][k][l]) > 0.001))
										{
											pegado_abajo = true;
										}


									}

									if (cplex.isExtracted(x_imjkl[i][m][j][k][l]) && cplex.getValue(x_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_w();
										}
										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_h();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_w(), pos_y1 + items[i].Getitem_h(), l + 1);
										//										pos_x += items[i].Getitem_w();
										puesta = true;

									}
									if (cplex.isExtracted(xr_imjkl[i][m][j][k][l]) && cplex.getValue(xr_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_h();
										}

										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_w();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_h(), pos_y1 + items[i].Getitem_w(), l + 1);
										//										pos_x += items[i].Getitem_h();
										puesta = true;
									}

								}
							}
						}

					}
				}


				fclose(fin3);
				DibujarOpenGL();
			}



		}

		//}


		cplex.end();
		model.end();
		env.end();
	}
	catch (IloException& e)
	{


		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
}
void Glass::FormulationStagesIIIMinimoCorteTiras(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes)
{
	int kk = 9;
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	IloRangeArray restrLazy(env);
	int maxtime = G_Time;
	IloNum max_time = maxtime; //tiempo del modelo

	char nombre[20];

	knapsack_bouknap();
	//	Creo las variables

	numtiras = 3;
	numpilas = 3;
	int numpiezasmax = 3;
	int numpieces = batch_items;
	numbins = nbins;

	int Tipo_objetivo = 2;
	//max_numero de defectos
	int max_nd = 10;



	//Variables defectos
	BoolVarMatrix5 zr_dmjkl(env, max_nd);
	BoolVarMatrix5 zrp_dmjkl(env, max_nd);
	BoolVarMatrix5 zu_dmjkl(env, max_nd);
	BoolVarMatrix4 ze_djkl(env, max_nd);

	BoolVarMatrix3 zr_dkl(env, max_nd);
	//minimo defecto pilas


	BoolVarMatrix3 es_jkl(env, numpilas);
	BoolVarMatrix4 el_ijkl(env, numpieces);
	BoolVarMatrix5 es_mijkl(env, numpiezasmax);
	BoolVarMatrix4 ms_mjkl(env, numpiezasmax);
	BoolVarMatrix4 zu_mjkl(env, numpiezasmax);
	BoolVarMatrix3 ml_jkl(env, numpilas);
	BoolVarMatrix2 ms_kl(env, numtiras);

	BoolVarMatrix5 x_imjkl(env, numpieces);
	BoolVarMatrix5 xr_imjkl(env, numpieces);

	BoolVarMatrix5 za_imjkl(env, numpieces);
	BoolVarMatrix5 zar_imjkl(env, numpieces);

	BoolVarMatrix2 y_kl(env, numtiras);
	NumVarMatrix4 w_mjkl(env, numpiezasmax);
	BoolVarMatrix4 es_mjkl(env, numpiezasmax);
	NumVarMatrix3 h_jkl(env, numpilas);
	NumVarMatrix2 w_kl(env, numtiras);
	BoolVector y_l(env);
	//	int LastBinWidth = floor((Total_area - ((numbins - 1)*plate_h*plate_w)) / plate_h);
	//	LastBinWidth = 0;
	int LastBinWidth = max((double)floor((Total_area - ((numbins - 1) * plate_h * plate_w)) / plate_h), (double)0);
	int maxh = 0;
	Lazy = false;
	/**********************************************************/
	/***************** INICIALIZO VARIABLES *******************/
	/**********************************************************/
	//inicializo la variable x_ijkl
	for (int i = 0; i < numpieces; i++)
	{
		if (items[i].Getitem_h() > maxh)
			maxh = items[i].Getitem_h();
		if (items[i].Getitem_w() > maxh)
			maxh = items[i].Getitem_w();
		x_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		xr_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		za_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		zar_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		for (int m = 0; m < numpiezasmax; m++)
		{
			x_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			xr_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			za_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			zar_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			for (int j = 0; j < numpilas; j++)
			{
				x_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				xr_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				za_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				zar_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					x_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					xr_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					za_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					zar_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					for (int l = 0; l < numbins; l++)
					{
						x_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "x_%d_%d_%d_%d_%d", i, m, j, k, l);
						x_imjkl[i][m][j][k][l].setName(nombre);
						za_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "za_%d_%d_%d_%d_%d", i, m, j, k, l);
						za_imjkl[i][m][j][k][l].setName(nombre);
						zar_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "zar_%d_%d_%d_%d_%d", i, m, j, k, l);
						zar_imjkl[i][m][j][k][l].setName(nombre);
						xr_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "xr_%d_%d_%d_%d_%d", i, m, j, k, l);
						xr_imjkl[i][m][j][k][l].setName(nombre);

					}

				}
			}
		}
	}

	//inicializo las varialbes de cada tira
	for (int m = 0; m < numpiezasmax; m++)
	{
		w_mjkl[m] = NumVarMatrix3(env, numpilas);
		es_mjkl[m] = BoolVarMatrix3(env, numpilas);
		zu_mjkl[m] = BoolVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			w_mjkl[m][j] = NumVarMatrix(env, numtiras);
			es_mjkl[m][j] = BoolVarMatrix(env, numtiras);
			zu_mjkl[m][j] = BoolVarMatrix(env, numtiras);
			for (int k = 0; k < numtiras; k++)
			{
				w_mjkl[m][j][k] = IloIntVarArray(env, numbins);
				es_mjkl[m][j][k] = IloBoolVarArray(env, numbins);
				zu_mjkl[m][j][k] = IloBoolVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					w_mjkl[m][j][k][l] = IloIntVar(env, 0, max1Cut);
					sprintf(nombre, "w_%d_%d_%d_%d", m, j, k, l);
					w_mjkl[m][j][k][l].setName(nombre);
					es_mjkl[m][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "ms_%d_%d_%d_%d", m, j, k, l);
					es_mjkl[m][j][k][l].setName(nombre);
					zu_mjkl[m][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "zu_%d_%d_%d_%d", m, j, k, l);
					zu_mjkl[m][j][k][l].setName(nombre);
				}

			}
		}
	}
	//inicializo la variable h_jkl
	for (int i = 0; i < numpilas; i++)
	{
		h_jkl[i] = NumVarMatrix(env, numtiras);

		for (int j = 0; j < numtiras; j++)
		{
			h_jkl[i][j] = IloIntVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				h_jkl[i][j][l] = IloIntVar(env, 0, maxh);
				sprintf(nombre, "h_%d_%d_%d", i, j, l);
				h_jkl[i][j][l].setName(nombre);

			}

		}
	}
	//inicializo la variable w_kl y y_kl
	for (int i = 0; i < numtiras; i++)
	{
		w_kl[i] = IloIntVarArray(env, numbins);
		y_kl[i] = IloBoolVarArray(env, numbins);
		ms_kl[i] = IloBoolVarArray(env, numbins);
		for (int j = 0; j < numbins; j++)
		{
			w_kl[i][j] = IloIntVar(env, 0, max1Cut);
			sprintf(nombre, "W_%d_%d", i, j);
			w_kl[i][j].setName(nombre);
			y_kl[i][j] = IloBoolVar(env);
			sprintf(nombre, "Y_%d_%d", i, j);
			y_kl[i][j].setName(nombre);
			ms_kl[i][j] = IloBoolVar(env);
			sprintf(nombre, "ms_%d_%d", i, j);
			ms_kl[i][j].setName(nombre);

		}
	}

	//inicializo las variable y_l
	y_l = IloBoolVarArray(env, numbins);
	for (int l = 0; l < numbins; l++)
	{
		sprintf(nombre, "Y%d", l);
		y_l[l].setName(nombre);
	}

	//minimco corte
	//inicializo variables rotadas
	for (int i = 0; i < numpieces; i++)
	{
		el_ijkl[i] = BoolVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			el_ijkl[i][j] = BoolVarMatrix(env, numtiras);

			for (int k = 0; k < numtiras; k++)
			{
				el_ijkl[i][j][k] = IloBoolVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					el_ijkl[i][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "Xel_%d_%d_%d_%d", i, j, k, l);
					el_ijkl[i][j][k][l].setName(nombre);

				}

			}
		}
	}
	//inicializo la variable es_jkl
	for (int i = 0; i < numpilas; i++)
	{
		es_jkl[i] = BoolVarMatrix(env, numtiras);
		ml_jkl[i] = BoolVarMatrix(env, numtiras);
		for (int j = 0; j < numtiras; j++)
		{
			es_jkl[i][j] = IloBoolVarArray(env, numbins);
			ml_jkl[i][j] = IloBoolVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				es_jkl[i][j][l] = IloBoolVar(env);
				sprintf(nombre, "Xes_%d_%d_%d", i, j, l);
				es_jkl[i][j][l].setName(nombre);
				ml_jkl[i][j][l] = IloBoolVar(env);
				sprintf(nombre, "ml_%d_%d_%d", i, j, l);
				ml_jkl[i][j][l].setName(nombre);

			}

		}
	}

	//inicializo las variables de los cortes

	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			zr_dkl[i] = BoolVarMatrix(env, numtiras);

			for (int j = 0; j < numtiras; j++)
			{
				zr_dkl[i][j] = IloBoolVarArray(env, 1);
				zr_dkl[i][j][l] = IloBoolVar(env);
				sprintf(nombre, "zr_%d_%d_%d", i, j, l);
				zr_dkl[i][j][l].setName(nombre);

			}
		}
	}
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			zr_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
			zrp_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
			zu_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
			for (int m = 0; m < numpiezasmax; m++)
			{
				zr_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
				zrp_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
				zu_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
				for (int j = 0; j < numpilas; j++)
				{
					zr_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
					zrp_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
					zu_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
					for (int k = 0; k < numtiras; k++)
					{

						zr_dmjkl[i][m][j][k] = IloBoolVarArray(env, 1);
						zr_dmjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "zr_%d_%d_%d_%d_%d", i, m, j, k, l);
						zr_dmjkl[i][m][j][k][l].setName(nombre);
						zrp_dmjkl[i][m][j][k] = IloBoolVarArray(env, 1);
						zrp_dmjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "zrp_%d_%d_%d_%d_%d", i, m, j, k, l);
						zrp_dmjkl[i][m][j][k][l].setName(nombre);
						zu_dmjkl[i][m][j][k] = IloBoolVarArray(env, 1);
						zu_dmjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "zu_%d_%d_%d_%d_%d", i, m, j, k, l);
						zu_dmjkl[i][m][j][k][l].setName(nombre);

					}

				}
			}
		}
	}
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();
		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			ze_djkl[i] = BoolVarMatrix3(env, numpilas);

			for (int j = 0; j < numpilas; j++)
			{
				ze_djkl[i][j] = BoolVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					ze_djkl[i][j][k] = IloBoolVarArray(env, 1);
					ze_djkl[i][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "ze_%d_%d_%d_%d", i, j, k, l);
					ze_djkl[i][j][k][l].setName(nombre);
				}

			}
		}
	}

	//inicializo las variables de los defectos


	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/

//	Lazy_minimo = false;
	//Todos los productos colocados
	for (int i = 0; i < numpieces; i++)
	{
		IloExpr v1(env);
		for (int j = 0; j < numpilas; j++)
		{
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						v1 += x_imjkl[i][m][j][k][l];
						v1 += xr_imjkl[i][m][j][k][l];
					}
				}
			}
		}
		if (Tipo_objetivo != 3)
			restr.add(IloRange(env, 1, v1, 1));
		else
			restr.add(IloRange(env, -IloInfinity, v1, 1));

		v1.end();
	}
	//Si coloco la segunda pieza también la primera
	//solamente una por celda


	for (int j = 0; j < numpilas; j++)
	{
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					IloExpr v1(env);
					IloExpr v2(env);
					for (int i = 0; i < numpieces; i++)
					{

						v2 += x_imjkl[i][m][j][k][l];
						v2 += xr_imjkl[i][m][j][k][l];

						v1 += x_imjkl[i][m][j][k][l];
						v1 += xr_imjkl[i][m][j][k][l];
						if (m > 0)
						{
							v1 += (-1) * x_imjkl[i][m - 1][j][k][l];
							v1 += (-1) * xr_imjkl[i][m - 1][j][k][l];
						}

					}
					//V1 indica que si ponemos la pieza m, antes también tiene que haber piezas
					if (m > 0)
					{
						restr.add(IloRange(env, -IloInfinity, v1, 0));
						v1.end();
					}
					restr.add(IloRange(env, -IloInfinity, v2, 1));
					v2.end();



				}
			}
		}
	}


	//Altura mínima altura
	for (int i = 0; i < numpieces; i++)
	{
		for (int m = 0; m < numpiezasmax; m++)
		{
			for (int j = 0; j < numpilas; j++)
			{

				for (int k = 0; k < numtiras; k++)
				{
					for (int l = 0; l < numbins; l++)
					{
						IloExpr v1(env);

						v1 += items[i].Getitem_h() * x_imjkl[i][m][j][k][l];
						v1 += items[i].Getitem_w() * xr_imjkl[i][m][j][k][l];
						v1 += (-1) * h_jkl[j][k][l];
						v1 += waste_min * (1 - el_ijkl[i][j][k][l]);
						restr.add(IloRange(env, -IloInfinity, v1, 0));
						v1.end();
						IloExpr v2(env);

						v2 += items[i].Getitem_h() * x_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_w() * xr_imjkl[i][m][j][k][l];
						v2 += (-1) * h_jkl[j][k][l];
						v2 += plate_h * (1 - el_ijkl[i][j][k][l]);
						if (Lazy_minimo)
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						else
							restr.add(IloRange(env, 0, v2, IloInfinity));

						v2.end();

					}
				}
			}
		}

	}

	//Anchura de los cortes en cada pila


	for (int j = 0; j < numpilas; j++)
	{

		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					IloExpr v2(env);
					for (int i = 0; i < numpieces; i++)
					{

						v2 += items[i].Getitem_w() * x_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_h() * xr_imjkl[i][m][j][k][l];

					}
					v2 += (-1) * w_mjkl[m][j][k][l];

					restr.add(IloRange(env, -IloInfinity, v2, 0));
					v2.end();


				}
			}
		}
	}
	//minimo corte consecutivo en los de tres
	for (int j = 0; j < numpilas; j++)
	{

		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					IloExpr v1(env);
					IloExpr v2(env);

					for (int i = 0; i < numpieces; i++)
					{


						v1 += items[i].Getitem_w() * x_imjkl[i][m][j][k][l];
						v1 += items[i].Getitem_h() * xr_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_w() * x_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_h() * xr_imjkl[i][m][j][k][l];

					}
					v1 += (-1) * w_mjkl[m][j][k][l];
					v1 += (1 - es_mjkl[m][j][k][l]) * waste_min;
					v2 += (-1) * w_mjkl[m][j][k][l];
					v2 += (1 - es_mjkl[m][j][k][l]) * max1Cut;
					restr.add(IloRange(env, -IloInfinity, v1, 0));
					v1.end();
					if (Lazy_minimo)
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));
					else
						restr.add(IloRange(env, 0, v2, IloInfinity));
					v2.end();




				}
			}
		}
	}

	//suma de las alturas de las pilas menor H


	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			IloExpr v1(env);
			for (int j = 0; j < numpilas; j++)
			{


				v1 += h_jkl[j][k][l];
			}
			v1 += (-1) * plate_h * y_kl[k][l];
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
		}
	}


	//suma de minimo corte anchos de las tiras
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			for (int j = 0; j < numpilas; j++)
			{

				IloExpr v1(env);
				IloExpr v2(env);
				for (int m = 0; m < numpiezasmax; m++)
				{
					v1 += w_mjkl[m][j][k][l];
					v2 += w_mjkl[m][j][k][l];

				}

				v1 += (-1) * w_kl[k][l];
				v1 += (1 - es_jkl[j][k][l]) * waste_min;

				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();

				v2 += (-1) * w_kl[k][l];
				v2 += (1 - es_jkl[j][k][l]) * max1Cut;

				if (Lazy_minimo)
					restrLazy.add(IloRange(env, 0, v2, IloInfinity));

				else
					restr.add(IloRange(env, 0, v2, IloInfinity));
				v2.end();
			}

		}
	}
	//minimos cortes
		//suma de las alturas de las pilas menor H

//	Lazy_minimo = true;
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{

			for (int j = 0; j < numpilas; j++)
			{
				//				Lazy_minimo = true;
				IloExpr v1(env);
				v1 += min2Cut * ml_jkl[j][k][l];

				v1 += (-1) * h_jkl[j][k][l];

				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
				IloExpr v2(env);
				v2 += plate_h * ml_jkl[j][k][l];
				v2 += (-1) * h_jkl[j][k][l];
				if (Lazy_minimo)
					restrLazy.add(IloRange(env, 0, v2, IloInfinity));
				else
					restr.add(IloRange(env, 0, v2, IloInfinity));
				v2.end();
				//no crea pilas vacías y además ordenadas
				IloExpr v3(env);
				v3 += ml_jkl[j][k][l];
				for (int i = 0; i < numpieces; i++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						v3 += (-1) * x_imjkl[i][m][j][k][l];
						v3 += (-1) * xr_imjkl[i][m][j][k][l];
					}
				}
				restr.add(IloRange(env, -IloInfinity, v3, 0));
				v3.end();

				if (j > 0)
				{
					IloExpr v4(env);
					v4 += ml_jkl[j][k][l];
					v4 += (-1) * ml_jkl[j - 1][k][l];
					restr.add(IloRange(env, -IloInfinity, v4, 0));
					v4.end();
				}




			}
			//			Lazy_minimo = false;
			IloExpr v1(env);
			v1 += min1Cut * ms_kl[k][l];

			v1 += (-1) * w_kl[k][l];

			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
			IloExpr v2(env);
			v2 += max1Cut * ms_kl[k][l];
			v2 += (-1) * w_kl[k][l];
			if (Lazy_minimo)
				restrLazy.add(IloRange(env, 0, v2, IloInfinity));
			else
				restr.add(IloRange(env, 0, v2, IloInfinity));
			v2.end();

			//no crea tiras vacías y además ordenadas
			IloExpr v3(env);
			v3 += ms_kl[k][l];
			for (int i = 0; i < numpieces; i++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					for (int j = 0; j < numpilas; j++)
					{
						v3 += (-1) * x_imjkl[i][m][j][k][l];
						v3 += (-1) * xr_imjkl[i][m][j][k][l];
					}
				}
			}
			restr.add(IloRange(env, -IloInfinity, v3, 0));
			v3.end();
			if (k > 0)
			{
				IloExpr v4(env);
				v4 += ms_kl[k][l];
				v4 += (-1) * ms_kl[k - 1][l];
				restr.add(IloRange(env, -IloInfinity, v4, 0));
				v4.end();
			}
		}
	}

	//Suma  tiras menor o igual a W
	for (int l = 0; l < numbins; l++)
	{
		IloExpr v1(env);
		for (int k = 0; k < numtiras; k++)
		{

			v1 += w_kl[k][l];

		}

		v1 += (-1) * plate_w * y_l[l];
		restr.add(IloRange(env, -IloInfinity, v1, 0));
		v1.end();
	}

	//Orden de los stacks por parejas
	for (int l = 0; l < numbins; l++)
	{

		for (int k = 0; k < numtiras; k++)
		{

			for (int j = 0; j < numpilas; j++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					for (int i = numpieces - 1; i > 0; i--)
					{


						if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1 && (items[i].Getitem_seq() > 1))
						{
							int kk = stacks[items[i].Getitem_stack()].Getitem_nbr();
							IloExpr v1(env);
							v1 += x_imjkl[i][m][j][k][l];
							v1 += xr_imjkl[i][m][j][k][l];

							// ponemos las variables
							int i2 = i - 1;
							if (i2 >= 0 && items[i].Getitem_stack() == items[i2].Getitem_stack())
							{

								for (int l2 = 0; l2 <= l; l2++)
								{
									for (int k2 = 0; k2 < numtiras; k2++)
									{
										if (l2 == l && k2 > k)
											break;
										for (int j2 = 0; j2 < numpilas; j2++)
										{
											if (l2 == l && k2 == k && j2 > j)
												break;
											for (int m2 = 0; m2 < numpiezasmax; m2++)
											{
												if (l2 == l && k2 == k && j2 == j && m2 > m)
													break;

												v1 += (-1) * x_imjkl[i2][m2][j2][k2][l2];
												v1 += (-1) * xr_imjkl[i2][m2][j2][k2][l2];

											}
										}

									}
								}

							}
							if (Lazy)
								restrLazy.add(IloRange(env, -IloInfinity, v1, 0));

							else
								restr.add(IloRange(env, -IloInfinity, v1, 0));
							v1.end();




						}



					}
				}
			}
		}
	}


	//restricciones de los cortes verticales
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{

			for (int k1 = 0; k1 < numtiras; k1++)
			{
				IloExpr v1(env);
				IloExpr v2(env);
				for (int k2 = 0; k2 <= k1; k2++)
				{
					v1 += w_kl[k2][l];
					v2 += w_kl[k2][l];
				}
				v1 += (-1) * ((*itD).Getpos_x());

				v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1][l]);
				//			v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth());
				v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zr_dkl[i][k1][l]);


				if (Lazy_cortes)
				{
					restrLazy.add(IloRange(env, 0, v2, IloInfinity));
					restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
				}
				else
				{
					restr.add(IloRange(env, 0, v2, IloInfinity));
					restr.add(IloRange(env, -IloInfinity, v1, 0));
				}
				v1.end();
				v2.end();
				if (k1 > 0)
				{
					IloExpr v3(env);
					v3 += zr_dkl[i][k1][l];
					v3 += (-1) * zr_dkl[i][k1 - 1][l];
					restr.add(IloRange(env, -IloInfinity, v3, 0));
					v3.end();
				}
			}



		}
	}

	//restricciones de los cortes horizontales
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			for (int k = 0; k < numtiras; k++)
			{

				for (int j1 = 0; j1 < numpilas; j1++)
				{
					IloExpr v1(env);
					IloExpr v2(env);
					for (int j2 = 0; j2 <= j1; j2++)
					{

						v1 += h_jkl[j2][k][l];
						v2 += h_jkl[j2][k][l];
					}



					v1 += (-1) * ((*itD).Getpos_y());
					v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - ze_djkl[i][j1][k][l]);
					v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (zr_dkl[i][k][l]);
					if (k > 0)
					{
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zr_dkl[i][k - 1][l]);
					}

					//ahora falta la otra parte
					int rtyd = (*itD).Getpos_y() + (*itD).Getheight();
					v2 += (-1) * (rtyd) * (1 - ze_djkl[i][j1][k][l]);
					v2 += (rtyd) * (zr_dkl[i][k][l]);
					if (k > 0)
					{
						v2 += rtyd * (1 - zr_dkl[i][k - 1][l]);
					}

					if (Lazy_cortes)
					{
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
					}
					else
					{
						restr.add(IloRange(env, 0, v2, IloInfinity));
						restr.add(IloRange(env, -IloInfinity, v1, 0));
					}
					v1.end();
					v2.end();
					if (j1 > 0)
					{
						IloExpr v3(env);
						v3 += ze_djkl[i][j1][k][l];
						v3 += (-1) * ze_djkl[i][j1 - 1][k][l];
						restr.add(IloRange(env, -IloInfinity, v3, 0));
						v3.end();


					}
					IloExpr v4(env);
					v4 += ze_djkl[i][j1][k][l];
					if (k > 0)
						v4 += (-1) * (zr_dkl[i][k - 1][l] - zr_dkl[i][k][l]);
					else
						v4 += (-1) * (1 - zr_dkl[i][k][l]);
					restr.add(IloRange(env, -IloInfinity, v4, 0));
					v4.end();


				}
				//orden de las variables
			}






		}
	}


	//restricciones de los cortes verticales de segundo orden de defecto


	//cortes
	//restricciones de los cortes verticales de segundo orden de defecto
	//Izquierda
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{

			for (int k1 = 0; k1 < numtiras; k1++)
			{
				for (int j = 0; j < numpilas; j++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						IloExpr v1(env);
						IloExpr v2(env);
						if (k1 > 0)
						{
							for (int k2 = 0; k2 < k1; k2++)
							{

								v1 += w_kl[k2][l];
								v2 += w_kl[k2][l];
							}
						}
						for (int m2 = 0; m2 <= m; m2++)
						{
							
							v1 += w_mjkl[m2][j][k1][l];
							v2 += w_mjkl[m2][j][k1][l];
						}
						for (int i1 = 0; i1 < numpieces; i1++)
						{
							v2 += (-1) * items[i1].Getitem_w() * x_imjkl[i1][m][j][k1][l];
							v2 += (-1) * items[i1].Getitem_h() * xr_imjkl[i1][m][j][k1][l];
						}


						v1 += (-1) * ((*itD).Getpos_x());
						v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zrp_dmjkl[i][m][j][k1][l]);
						v1 += (-1) * (plate_w - (*itD).Getpos_x()) * ze_djkl[i][j][k1][l];
						if (j > 0)
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - ze_djkl[i][j - 1][k1][l]);
						int rtxd = (*itD).Getpos_x() + (*itD).Getwidth();
						v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zrp_dmjkl[i][m][j][k1][l]);
						v2 += (rtxd)* ze_djkl[i][j][k1][l];
						if (j > 0)
							v2 += (rtxd) * (1 - ze_djkl[i][j - 1][k1][l]);
						if (Lazy_cortes)
						{
							restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						}
						else
						{
							restr.add(IloRange(env, -IloInfinity, v1, 0));
							restr.add(IloRange(env, 0, v2, IloInfinity));
						}
						v1.end();
						v2.end();
						if (m > 0)
						{
							IloExpr v3(env);
							v3 += zrp_dmjkl[i][m][j][k1][l];
							v3 += (-1) * zrp_dmjkl[i][m - 1][j][k1][l];
							restr.add(IloRange(env, -IloInfinity, v3, 0));
							v3.end();
						}
						//if (m ==0)
						//{
						//	IloExpr v3(env);
						//	v3 += zr_dkl[i][k1][l];
						//	v3 += (-1) * zr_dmjkl[i][numpiezasmax-1][j][k1][l];
						//	restr.add(IloRange(env, -IloInfinity, v3, 0));
						//	v3.end();
						//}

						IloExpr v4(env);
						v4 += zrp_dmjkl[i][m][j][k1][l];
						if (j > 0)
							v4 += (-1) * (ze_djkl[i][j - 1][k1][l] - ze_djkl[i][j][k1][l]);
						else
							v4 += (-1) * (1 - ze_djkl[i][j][k1][l]);
						restr.add(IloRange(env, -IloInfinity, v4, 0));
						v4.end();

					}
				}
			}



		}
	}

	
	model.add(restr);
	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
//	model.add(restr);


	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);
	if (Tipo_objetivo == 0)//biobjective //estaría en diet.ccp
	{
		IloExpr Expression_fobj1(env);
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj1 += y_l[i];
		}
		IloExpr Expression_fobj2(env);
		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj2 += w_kl[k][numbins - 1];
		}
	}

	if (Tipo_objetivo == 1)//por bins
	{
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj += y_l[i];
		}
	}
	if (Tipo_objetivo == 2)//por tira del último bin
	{

		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj += w_kl[k][numbins - 1];
		}
	}
	if (Tipo_objetivo == 3)//por área de lo metido en el bin
	{
		Expression_fobj += plate_h * plate_w;
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int j = 0; j < numpilas; j++)
				{

					for (int m = 0; m < numpiezasmax; m++)
					{

						for (int i = 0; i < numpieces; i++)
						{
							Expression_fobj += (-1) * items[i].Getitem_area() * x_imjkl[i][m][j][k][l];
							Expression_fobj += (-1) * items[i].Getitem_area() * xr_imjkl[i][m][j][k][l];
						}
					}
				}
			}
		}
	}

	if (Tipo_objetivo != 0)
		model.add(IloMinimize(env, Expression_fobj));
	Expression_fobj.end();


	/***************************************************************/
	/*************** EJECUTAMOS EL MODELO **************************/
	/***************************************************************/
	try
	{

		IloCplex cplex(model);
		//Si queremos las lazy y tenemos alguna
		if (Lazy_minimo)
			cplex.addLazyConstraints(restrLazy);
		//		cplex.exportModel("Roadef.lp");
				//No hace el probing
				//cplex.setParam(IloCplex::Probe, -1);
				//1 CORE
		cplex.setParam(IloCplex::Threads, nt);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, max_time); //TIEMPO EN SEGUNDOS
		if (Tipo_objetivo == 2)
			cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, LastBinWidth);
		if (G_Dibujar)
		cplex.exportModel("Roadef.lp");

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

		//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();


		IloBool result = cplex.solve();

		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;

		//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
//				fprintf(f, "Infeasible\n");
				//				cplex.exportModel("Roadef.lp");
			}
			else
			{
//				if (cplex.getStatus() == IloAlgorithm::Unbounded) fprintf(f, "Unbounded\n");
//				fprintf(f, "\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}
//			fclose(f);
		}
		else
		{
//			FILE* f = fopen("./estadomodelo.txt", "a+");
//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			Best_Value_Formulation = cplex.getObjValue();
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible
			if (cplex.getSolnPoolNsolns() > 0)
			{
				int value2 = cplex.getObjValue();
				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							for (int m = 0; m < numpiezasmax; m++)
							{
								for (int i = 0; i < numpieces; i++)
								{


									if (cplex.isExtracted(x_imjkl[i][m][j][k][l]) && cplex.getValue(x_imjkl[i][m][j][k][l]) > 0.001)
									{
										printf("%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
//										fprintf(f, "%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
									}
									if (cplex.isExtracted(xr_imjkl[i][m][j][k][l]) && cplex.getValue(xr_imjkl[i][m][j][k][l]) > 0.001)
									{
										printf("%s %d w %d h %d s %d\n", xr_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
//										fprintf(f, "%s %d w %d h %d s %d\n", xr_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
									}

								}
							}

						}
					}
				}
				//Pinto las variables w_mjkl
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{
							for (int m = 0; m < numpiezasmax; m++)
							{
								if (cplex.isExtracted(w_mjkl[m][j][k][l]) && cplex.getValue(w_mjkl[m][j][k][l]) > 0.001)
								{
									printf("%s %2.f\n", w_mjkl[m][j][k][l].getName(), cplex.getValue(w_mjkl[m][j][k][l]));
//									fprintf(f, "%s %2.f\n", w_mjkl[m][j][k][l].getName(), cplex.getValue(w_mjkl[m][j][k][l]));
								}
							}

						}

					}
				}
				//Pinto las variables h_jkl
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							if (cplex.isExtracted(h_jkl[j][k][l]) && cplex.getValue(h_jkl[j][k][l]) > 0.001)
							{
								printf("%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
//								fprintf(f, "%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
							}

						}

					}
				}
				//Pinto las variables w_kl y y_kl
				for (int j = 0; j < numbins; j++)
				{
					for (int i = 0; i < numtiras; i++)
					{

						if (cplex.isExtracted(w_kl[i][j]) && cplex.getValue(w_kl[i][j]) > 0.001)
						{
							printf("%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
//							fprintf(f, "%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
						}
						if (cplex.isExtracted(y_kl[i][j]) && cplex.getValue(y_kl[i][j]) > 0.001)
						{
							printf("%s %d\n", y_kl[i][j].getName(), 1);
//							fprintf(f, "%s %d\n", y_kl[i][j].getName(), 1);
						}

					}
				}

				//Pinto las variable y_l
				for (int l = 0; l < numbins; l++)
				{
					if (cplex.isExtracted(y_l[l]) && cplex.getValue(y_l[l]) > 0.001)
					{
						printf("%s %d\n", y_l[l].getName(), 1);
//						fprintf(f, "%s %d\n", y_l[l].getName(), 1);
					}

				}
				//Pinto los cortes
				for (int l = 0; l < numbins; l++)
				{
					int nd = DefectsPlate[l].size();

					int d = 0;
					for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							for (int k = 0; k < numtiras; k++)
							{
								for (int m = 0; m < numpiezasmax; m++)
								{
									if (cplex.isExtracted(zr_dmjkl[d][m][j][k][l]) && cplex.getValue(zr_dmjkl[d][m][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zr_dmjkl[d][m][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zr_dmjkl[d][m][j][k][l].getName(), 1);
									}
									if (cplex.isExtracted(zrp_dmjkl[d][m][j][k][l]) && cplex.getValue(zrp_dmjkl[d][m][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zrp_dmjkl[d][m][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zrp_dmjkl[d][m][j][k][l].getName(), 1);
									}
									if (cplex.isExtracted(zu_dmjkl[d][m][j][k][l]) && cplex.getValue(zu_dmjkl[d][m][j][k][l]) > 0.001)
									{
										printf("%s %d\n", zu_dmjkl[d][m][j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", zu_dmjkl[d][m][j][k][l].getName(), 1);
									}
									if (m == 0)
									{
										if (cplex.isExtracted(ze_djkl[d][j][k][l]) && cplex.getValue(ze_djkl[d][j][k][l]) > 0.001)
										{
											printf("%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
										}
									}
									if (j == 0 && m == 0)
									{
										if (cplex.isExtracted(zr_dkl[d][k][l]) && cplex.getValue(zr_dkl[d][k][l]) > 0.001)
										{
											printf("%s %d\n", zr_dkl[d][k][l].getName(), 1);
//											fprintf(f, "%s %d\n", zr_dkl[d][k][l].getName(), 1);
										}

									}

								}

							}
						}
					}

				}
				//Pinto las variables zlr,zllr, zll,zlrr
				printf("\nVariables para los defectos\n");

				for (int l = 0; l < numbins; l++)
				{
					int nd = DefectsPlate[l].size();

					int d = 0;
					for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
					{
						for (int i = 0; i < numpieces; i++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								for (int k = 0; k < numtiras; k++)
								{
									for (int m = 0; m < numpiezasmax; m++)
									{
										if (d == 0)
										{
											if (cplex.isExtracted(zar_imjkl[i][m][j][k][l]) && cplex.getValue(zar_imjkl[i][m][j][k][l]) > 0.001)
											{
												printf("%s %d\n", zar_imjkl[i][m][j][k][l].getName(), 1);
//												fprintf(f, "%s %d\n", zar_imjkl[i][m][j][k][l].getName(), 1);
											}
											if (cplex.isExtracted(za_imjkl[i][m][j][k][l]) && cplex.getValue(za_imjkl[i][m][j][k][l]) > 0.001)
											{
												printf("%s %d\n", za_imjkl[i][m][j][k][l].getName(), 1);
//												fprintf(f, "%s %d\n", za_imjkl[i][m][j][k][l].getName(), 1);
											}
										}

									}

								}
							}
						}
					}
				}

				printf("\nVariables de mínimo\n");
				for (int i = 0; i < numpieces; i++)
				{

					for (int j = 0; j < numpilas; j++)
					{

						for (int k = 0; k < numtiras; k++)
						{
							for (int l = 0; l < numbins; l++)
							{
								if (cplex.isExtracted(el_ijkl[i][j][k][l]) && cplex.getValue(el_ijkl[i][j][k][l]) > 0.001)
								{
									printf("%s %d\n", el_ijkl[i][j][k][l].getName(), 1);
//									fprintf(f, "%s %d\n", el_ijkl[i][j][k][l].getName(), 1);
								}
								if (i == 0)
								{
									if (cplex.isExtracted(es_jkl[j][k][l]) && cplex.getValue(es_jkl[j][k][l]) > 0.001)
									{
										printf("%s %d\n", es_jkl[j][k][l].getName(), 1);
//										fprintf(f, "%s %d\n", es_jkl[j][k][l].getName(), 1);
									}

								}

							}
						}
					}
				}
			}
			//Escribo la solución para dibujar
				//Inicio
			if (G_Dibujar || G_Draw_Exacto)
			{
				FILE* fin3;
				if (G_Draw_Exacto)
				{
					std::string str = file_idx + "_solution_" + std::to_string(G_Exacto) + "_" + std::to_string(Tipo_objetivo) + ".txt";
					fin3=fopen(P_file_to_write.c_str(), "w+");
//					fin3 = fopen(str.c_str(), "w+");
				}
				else
					fin3 = fopen("./temp2.txt", "w+");
				int ndefects = 0;
				int nbinsdefects = 0;
				for (int i = 0; i < numbins; i++)
				{
					if (DefectsPlate[i].size() >= 1)
						nbinsdefects++;
					for (std::list< GlassDefect > ::iterator it = DefectsPlate[i].begin(); it != DefectsPlate[i].end(); it++)
					{
						ndefects++;
					}
				}
				//					fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, Best_Objective_function);
				fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, numpieces, ndefects, nbinsdefects, LB_Best_Value_Formulation, Best_Value_Formulation);
				//defectos

				for (int i = 0; i < numbins; i++)
				{

					for (std::list< GlassDefect > ::iterator it = DefectsPlate[i].begin(); it != DefectsPlate[i].end(); it++)
					{
						// Get defect coordinates.
						int defect_x = (*it).Getpos_x();
						int defect_y = (*it).Getpos_y();
						int defect_width = (*it).Getwidth();
						int defect_height = (*it).Getheight();

						fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", defect_x, defect_y, defect_width, defect_height, i);
					}
				}
				for (int i = 0; i < numbins; i++)
				{
					if (DefectsPlate[i].size() > 0)
						fprintf(fin3, "%d\t%d\n", i, DefectsPlate[i].size());

				}
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							int pos_x = 0;
							int pos_X = 0;
							int pos_Y = 0;
							for (int k1 = 0; k1 < k; k1++)
							{
								if (cplex.isExtracted(w_kl[k1][l]) && cplex.getValue(w_kl[k1][l]) > 0.001)
								{
									pos_X += cplex.getValue(w_kl[k1][l]);
								}
							}
							int pos_y = 0;
							for (int j1 = 0; j1 < j; j1++)
							{
								if (cplex.isExtracted(h_jkl[j1][k][l]) && cplex.getValue(h_jkl[j1][k][l]) > 0.001)
								{
									pos_y += cplex.getValue(h_jkl[j1][k][l]);
								}
							}
							for (int m = 0; m < numpiezasmax; m++)
							{

								pos_X = pos_X + cplex.getValue(w_mjkl[m][j][k][l]);


								pos_Y = pos_y + cplex.getValue(h_jkl[j][k][l]);

								bool puesta = false;
								for (int i = 0; i < numpieces && puesta == false; i++)
								{
									//buscar defecto que está dentro de este rango
									int d = 0;
									bool pegado_izquierda = false;
									bool pegado_derecha = false;
									bool pegado_abajo = false;
									bool pegado_arriba = true;

									///									for (std::list< GlassDefect > ::iterator it = DefectsPlate[l].begin(); it != DefectsPlate[l].end(); it++, d++)
									//									{
														/*					if (((*it).Getpos_x() > pos_X) || ((*it).Getpos_x() + (*it).Getwidth() < pos_x)
																				|| ((*it).Getpos_y() > pos_Y) || ((*it).Getpos_y() + (*it).Getheight() < pos_y))
																			{
																				continue;
																			}
																			//este está aquí dentro
																			if ((cplex.isExtracted(zlr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlr_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zlrr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlrr_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_derecha = true;
																			}
																			if ((cplex.isExtracted(zll_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zll_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zllr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zllr_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_izquierda = true;
																			}
																			if ((cplex.isExtracted(zla_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zla_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zlar_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlar_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_arriba = true;
																			}
																			if ((cplex.isExtracted(zle_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zle_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zler_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zler_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_abajo = true;
																			}

																			*/
																			//	if ((cplex.isExtracted(zu_dmjkl[d][m][j][k][l]) && cplex.getValue(zu_dmjkl[d][m][j][k][l]) > 0.001)
																			//		&& (cplex.isExtracted(zrp_dmjkl[d][m][j][k][l]) && cplex.getValue(zrp_dmjkl[d][m][j][k][l]) > 0.001) 
																			//		&& (cplex.isExtracted(zr_dmjkl[d][m][j][k][l]) && cplex.getValue(zr_dmjkl[d][m][j][k][l]) < 0.001))
																			//	{
																			//		pegado_arriba = false;
																			//	}

																			//}

									if (cplex.isExtracted(zu_mjkl[m][j][k][l]) && cplex.getValue(zu_mjkl[m][j][k][l]) > 0.001)
										pegado_arriba = false;
									pegado_izquierda = true;
									pegado_arriba = false;
									if (cplex.isExtracted(x_imjkl[i][m][j][k][l]) && cplex.getValue(x_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_w();
										}
										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_h();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_w(), pos_y1 + items[i].Getitem_h(), l + 1);
										//										pos_x += items[i].Getitem_w();
										puesta = true;

									}
									if (cplex.isExtracted(xr_imjkl[i][m][j][k][l]) && cplex.getValue(xr_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_h();
										}

										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_w();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_h(), pos_y1 + items[i].Getitem_w(), l + 1);
										//										pos_x += items[i].Getitem_h();
										puesta = true;
									}

								}
							}
						}

					}
				}


				fclose(fin3);
				DibujarOpenGL();
			}



		}

		//}


		cplex.end();
		model.end();
		env.end();
	}
	catch (IloException& e)
	{


		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
}
//Calcula los conjuntos para cada uno de los anteriores

//Tambien crea un conjunto de piezas que no tendré que definir
void Glass::ComputePiecesEquals()
{
	


	for (register int j = 0; j < batch_items; j++)
	{

		std::set<int> set_da;
		std::set<int> set_dar;
		std::set<int> set_sa;
		std::set<int> set_sar;
		for (register int i = 0; i < batch_items; i++)
		{
			if (i == j) continue;
			if ((items[i].Getitem_w() == items[j].Getitem_w()))
			{
				if ((items[i].Getitem_stack() == items[j].Getitem_stack() && (items[j].Getitem_seq() == (items[i].Getitem_seq() - 1))) || ((items[i].Getitem_stack() != items[j].Getitem_stack())))
				{
					set_da.insert(i);
					Dxx[i].insert(j);
					Ax[j].insert(i);
					P_set_xa[i] = true;
					P_set_xpa[j] = true;
				}
			}
			if ((items[i].Getitem_w() == items[j].Getitem_h()))
			{
				if ((items[i].Getitem_stack() == items[j].Getitem_stack() && (items[j].Getitem_seq() == (items[i].Getitem_seq() - 1))) || ((items[i].Getitem_stack() != items[j].Getitem_stack())))
				{
					Dyx[i].insert(j);
					P_set_xa[i] = true;
					Ax[j].insert(i);
					P_set_xrpa[j] = true;
				}
			}
			if ((items[i].Getitem_h() == items[j].Getitem_w()))
			{
				if ((items[i].Getitem_stack() == items[j].Getitem_stack() && (items[j].Getitem_seq() == (items[i].Getitem_seq() - 1))) || ((items[i].Getitem_stack() != items[j].Getitem_stack())))
				{
					Dxy[i].insert(j);
					P_set_xar[i] = true;
					P_set_xpa[j] = true;
					Ay[j].insert(i);
				}
			}
			if ((items[i].Getitem_h() == items[j].Getitem_h()))
			{
				if ((items[i].Getitem_stack() == items[j].Getitem_stack() && (items[j].Getitem_seq() == (items[i].Getitem_seq() - 1))) || ((items[i].Getitem_stack() != items[j].Getitem_stack())))
				{
					Dyy[i].insert(j);
					P_set_xar[i] = true;
					P_set_xrpa[j] = true;
					Ay[j].insert(i);
				}
			}
		}


	}
	for (register int j = 0; j < batch_items; j++)
	{
		P_piezas_juntas += (Dyy[j].size() + Dxy[j].size() + Dxx[j].size() + Dyx[j].size());
	}
	for (register int j = 0; j < batch_items; j++)
	{
//		if (P_set_xpa[j] == true || P_set_xrpa[j] == true)
//			P_piezas_juntas++;
		if (P_set_xar[j] == true || P_set_xa[j] == true)
			P_piezas_juntas_posibilidades++;
	}
}

//cambiar SAR por Dy
//cambiar SA por Dx
/*
void Glass::ComputePiecesEquals()
{

	for (register int i = 0; i < batch_items; i++)
	{
		P_set_xa.push_back(false);
		P_set_xar.push_back(false);
	}

	for (register int i = 0; i < batch_items ; i++)
	{

		std::set<int> set_a;
		std::set<int> set_ar;
		std::set<int> set_ra;
		std::set<int> set_rar;
		std::set<int> set_sa;
		std::set<int> set_sar;
		for (register int j = 0; j < batch_items  ; j++)
		{
			if (i == j) continue;
			if ((items[i].Getitem_w() == items[j].Getitem_w()))
			{
				if ((items[i].Getitem_stack() == items[j].Getitem_stack() && (items[j].Getitem_seq() == (items[i].Getitem_seq() + 1))) || ((items[i].Getitem_stack() != items[j].Getitem_stack())))
				{
					set_a.insert(j);
					set_sa.insert(j);
					P_set_xa[j]=true;
				}
			}
			if ((items[i].Getitem_w() == items[j].Getitem_h()))
			{
				if ((items[i].Getitem_stack() == items[j].Getitem_stack() && (items[j].Getitem_seq() == (items[i].Getitem_seq() + 1))) || ((items[i].Getitem_stack() != items[j].Getitem_stack())))
				{
					set_ar.insert(j);
					P_set_xar[j] = true;
					set_sar.insert(j);
					
				}
			}
			if ((items[i].Getitem_h() == items[j].Getitem_w()))
			{
				if ((items[i].Getitem_stack() == items[j].Getitem_stack() && (items[j].Getitem_seq() == (items[i].Getitem_seq() + 1))) || ((items[i].Getitem_stack() != items[j].Getitem_stack())))
				{
					set_ra.insert(j);
					P_set_xa[j] = true;
					set_sa.insert(j);
				}
			}
			if ((items[i].Getitem_h() == items[j].Getitem_h()))
			{
				if ((items[i].Getitem_stack() == items[j].Getitem_stack() && (items[j].Getitem_seq() == (items[i].Getitem_seq() + 1))) || ((items[i].Getitem_stack() != items[j].Getitem_stack())))
				{
					set_rar.insert(j);
					P_set_xar[j] = true;
					set_sar.insert(j);
				}
			}
		}
		Dx.push_back(set_sa);
		Dy.push_back(set_sar);
		A.push_back(set_a);
		AR.push_back(set_ar);
		RA.push_back(set_ra);
		RAR.push_back(set_rar);
	}
}
*/
void Glass::FormulationCompleta4Indices(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes)
{
	//Compute the number of pieces
	//P_Set_ar y P_Set_xar
	int trozo = plate_w;
	if (numbins <= 1)
	{
		trozo = Best_Width;
	}
	maxTiras = P_maxnivelesw;
//	int maxTiras=MultiKnapsackEntera(trozo);
	if (P_maxnivelesw > maxTiras)
		P_maxnivelesw = maxTiras;
	for (register int i = 0; i < batch_items; i++)
	{
		P_set_xa.push_back(false);
		P_set_xar.push_back(false);
		P_set_xpa.push_back(false);
		P_set_xrpa.push_back(false);
		std::set<int> set_da;
		std::set<int> set_dar;
		Dxx.push_back(set_da);
		Dyx.push_back(set_dar);
		Dxy.push_back(set_da);
		Dyy.push_back(set_dar);
		Ax.push_back(set_da);
		Ay.push_back(set_da);


	}


	if (P_trimming)
	ComputePiecesEquals();
	int kk = 9;
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	IloRangeArray restrLazy(env);
	int maxtime = G_Time;
	IloNum max_time = maxtime; //tiempo del modelo

	char nombre[20];
	//	Lazy = true;
	Lazy_minimo = false;
	Lazy_cortes = false;
	int Lazy_orden = false;
//	knapsack_bouknap_tiras();
	//	Creo las variables

	numtiras = P_maxnivelesw;
	numpilas = P_maxnivelesh;
	int numpiezasmax = P_maxnivelesm;
	int numpieces = batch_items;
	numbins = nbins;


//	numtiras = 5;
//	numpilas = 4;
//	numpiezasmax = 3;
	int Tipo_objetivo = 2;
	//max_numero de defectos
	int max_nd = 1;
	for (int i = 0; i < numbins; i++)
	{
		int kk = DefectsPlate[i].size();
		if (kk > max_nd)
		{
			max_nd = kk;
		}
	}

	printf("Aqui21");


	//Variables defectos
	BoolVarMatrix5 zr_dmjkl(env, max_nd);
	BoolVarMatrix5 zrp_dmjkl(env, max_nd);
	BoolVarMatrix5 zu_dmjkl(env, max_nd);
	BoolVarMatrix4 ze_djkl(env, max_nd);

	BoolVarMatrix3 zr_dkl(env, max_nd);
	//minimo defecto pilas


	BoolVarMatrix3 es_jkl(env, numpilas);
	BoolVarMatrix4 el_mjkl(env, numpiezasmax);
	BoolVarMatrix5 es_mijkl(env, numpiezasmax);
	BoolVarMatrix4 ms_mjkl(env, numpiezasmax);
	BoolVarMatrix4 zu_mjkl(env, numpiezasmax);
	BoolVarMatrix3 ml_jkl(env, numpilas);
	BoolVarMatrix2 ms_kl(env, numtiras);

	BoolVarMatrix5 x_imjkl(env, numpieces);
	BoolVarMatrix5 xr_imjkl(env, numpieces);

	//Variables para el trimming
	BoolVarMatrix5 xa_imjkl(env, numpieces);
	BoolVarMatrix5 xar_imjkl(env, numpieces);

	BoolVarMatrix5 za_imjkl(env, numpieces);
	BoolVarMatrix5 zar_imjkl(env, numpieces);

	BoolVarMatrix2 y_kl(env, numtiras);
	NumVarMatrix4 w_mjkl(env, numpiezasmax);
	BoolVarMatrix4 es_mjkl(env, numpiezasmax);
	NumVarMatrix3 h_jkl(env, numpilas);
	NumVarMatrix2 w_kl(env, numtiras);
	BoolVector y_l(env);
	NumVector W_K(env);

	IloNumVarArray vars(env);
	IloNumArray vals(env);
	

	//	int LastBinWidth = floor((Total_area - ((Boolbins - 1)*plate_h*plate_w)) / plate_h);
	//	LastBinWidth = 0;
	int LastBinWidth = max((double)floor((Total_area - ((numbins - 1) * plate_h * plate_w)) / plate_h), (double)0);
	int maxh = 0;

	/**********************************************************/
	/***************** INICIALIZO VARIABLES *******************/
	/**********************************************************/
	//inicializo la variable x_ijkl y xa_imjkl
	for (int i = 0; i < numpieces; i++)
	{
		bool variable_a = true;
		if (items[i].Getitem_h() > maxh)
			maxh = items[i].Getitem_h();
		if (items[i].Getitem_w() > maxh)
			maxh = items[i].Getitem_w();
		x_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		if (P_set_xa[i])
		{
			xa_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		}
		else
			variable_a = false;
		za_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		zar_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		for (int m = 0; m < numpiezasmax; m++)
		{
			x_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			if (variable_a)
				xa_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			za_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			zar_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			for (int j = 0; j < numpilas; j++)
			{
				x_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				if (variable_a)
					xa_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				za_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				zar_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					x_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					if (variable_a)
						xa_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					za_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					zar_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					for (int l = 0; l < numbins; l++)
					{
						x_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "x_%d_%d_%d_%d_%d", i, m, j, k, l);
						x_imjkl[i][m][j][k][l].setName(nombre);
						if (variable_a)
						{
							xa_imjkl[i][m][j][k][l] = IloBoolVar(env);
							sprintf(nombre, "xa_%d_%d_%d_%d_%d", i, m, j, k, l);
							xa_imjkl[i][m][j][k][l].setName(nombre);
						}
						za_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "za_%d_%d_%d_%d_%d", i, m, j, k, l);
						za_imjkl[i][m][j][k][l].setName(nombre);
						zar_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "zar_%d_%d_%d_%d_%d", i, m, j, k, l);
						zar_imjkl[i][m][j][k][l].setName(nombre);

					}

				}
			}
		}
	}
	//inicializo variables rotadas
	for (int i = 0; i < numpieces; i++)
	{
		bool variable_ar = true;
		xr_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);

		if (P_set_xar[i])
		{
			xar_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		}
		else
			variable_ar = false;
		for (int m = 0; m < numpiezasmax; m++)
		{
			xr_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			if (variable_ar)
				xar_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			for (int j = 0; j < numpilas; j++)
			{
				xr_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				if (variable_ar)
					xar_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					xr_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					if (variable_ar)
						xar_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					for (int l = 0; l < numbins; l++)
					{
						xr_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "xr_%d_%d_%d_%d_%d", i, m, j, k, l);
						xr_imjkl[i][m][j][k][l].setName(nombre);
						if (variable_ar)
						{
							xar_imjkl[i][m][j][k][l] = IloBoolVar(env);
							sprintf(nombre, "xar_%d_%d_%d_%d_%d", i, m, j, k, l);
							xar_imjkl[i][m][j][k][l].setName(nombre);
						}

					}

				}
			}
		}
	}
	//inicializo las varialbes de cada tira
	for (int m = 0; m < numpiezasmax; m++)
	{
		w_mjkl[m] = NumVarMatrix3(env, numpilas);
		es_mjkl[m] = BoolVarMatrix3(env, numpilas);
		zu_mjkl[m] = BoolVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			w_mjkl[m][j] = NumVarMatrix(env, numtiras);
			es_mjkl[m][j] = BoolVarMatrix(env, numtiras);
			zu_mjkl[m][j] = BoolVarMatrix(env, numtiras);
			for (int k = 0; k < numtiras; k++)
			{
				w_mjkl[m][j][k] = IloIntVarArray(env, numbins);
				es_mjkl[m][j][k] = IloBoolVarArray(env, numbins);
				zu_mjkl[m][j][k] = IloBoolVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					w_mjkl[m][j][k][l] = IloIntVar(env, 0, max1Cut);
					sprintf(nombre, "w_%d_%d_%d_%d", m, j, k, l);
					w_mjkl[m][j][k][l].setName(nombre);
					es_mjkl[m][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "ms_%d_%d_%d_%d", m, j, k, l);
					es_mjkl[m][j][k][l].setName(nombre);
					zu_mjkl[m][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "zu_%d_%d_%d_%d", m, j, k, l);
					zu_mjkl[m][j][k][l].setName(nombre);
				}

			}
		}
	}
	//inicializo la variable h_jkl
	for (int i = 0; i < numpilas; i++)
	{
		h_jkl[i] = NumVarMatrix(env, numtiras);

		for (int j = 0; j < numtiras; j++)
		{
			h_jkl[i][j] = IloIntVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				h_jkl[i][j][l] = IloIntVar(env, 0,plate_h);
				sprintf(nombre, "h_%d_%d_%d", i, j, l);
				h_jkl[i][j][l].setName(nombre);

			}

		}
	}
	//inicializo la variable w_kl y y_kl
	for (int i = 0; i < numtiras; i++)
	{
		w_kl[i] = IloIntVarArray(env, numbins);
		y_kl[i] = IloBoolVarArray(env, numbins);
		ms_kl[i] = IloBoolVarArray(env, numbins);
		for (int j = 0; j < numbins; j++)
		{
			w_kl[i][j] = IloIntVar(env, 0, max1Cut);
			sprintf(nombre, "W_%d_%d", i, j);
			w_kl[i][j].setName(nombre);
			y_kl[i][j] = IloBoolVar(env);
			sprintf(nombre, "Y_%d_%d", i, j);
			y_kl[i][j].setName(nombre);
			ms_kl[i][j] = IloBoolVar(env);
			sprintf(nombre, "ms_%d_%d", i, j);
			ms_kl[i][j].setName(nombre);

		}
	}

	//inicializo las variable y_l
	y_l = IloIntVarArray(env, numbins, 0, 1);
	for (int l = 0; l < numbins; l++)
	{
		sprintf(nombre, "Y%d", l);
		y_l[l].setName(nombre);
	}
	if (Tipo_objetivo == 4)
	{
		W_K = IloIntVarArray(env, numtiras, 0, 1);
		for (int l = 0; l < numtiras; l++)
		{
			sprintf(nombre, "W%d", l);
			W_K[l].setName(nombre);
		}
	}

	//minimco corte

		for (int i = 0; i < numpiezasmax; i++)
		{
			el_mjkl[i] = BoolVarMatrix3(env, numpilas);
			for (int j = 0; j < numpilas; j++)
			{
				el_mjkl[i][j] = BoolVarMatrix(env, numtiras);

				for (int k = 0; k < numtiras; k++)
				{
					el_mjkl[i][j][k] = IloBoolVarArray(env, numbins);
					for (int l = 0; l < numbins; l++)
					{
						el_mjkl[i][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "Xem_%d_%d_%d_%d", i, j, k, l);
						el_mjkl[i][j][k][l].setName(nombre);

					}

				}
			}
		}

	//inicializo la variable es_jkl
	for (int i = 0; i < numpilas; i++)
	{
		es_jkl[i] = BoolVarMatrix(env, numtiras);
		ml_jkl[i] = BoolVarMatrix(env, numtiras);
		for (int j = 0; j < numtiras; j++)
		{
			es_jkl[i][j] = IloBoolVarArray(env, numbins);
			ml_jkl[i][j] = IloBoolVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				es_jkl[i][j][l] = IloBoolVar(env);
				sprintf(nombre, "Xes_%d_%d_%d", i, j, l);
				es_jkl[i][j][l].setName(nombre);
				ml_jkl[i][j][l] = IloBoolVar(env);
				sprintf(nombre, "ml_%d_%d_%d", i, j, l);
				ml_jkl[i][j][l].setName(nombre);

			}

		}
	}

	//inicializo las variables de los cortes

		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{
				zr_dkl[i] = BoolVarMatrix(env, numtiras);

				for (int j = 0; j < numtiras; j++)
				{
					zr_dkl[i][j] = IloBoolVarArray(env, numbins);
					for (int kl = 0; kl < numbins; kl++)
					{

						zr_dkl[i][j][kl] = IloBoolVar(env);
						sprintf(nombre, "zr_%d_%d_%d", i, j, kl);
						zr_dkl[i][j][kl].setName(nombre);
					}
				}
			}
		}
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{
				zr_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
				zrp_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
				zu_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
				for (int m = 0; m < numpiezasmax; m++)
				{
					zr_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
					zrp_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
					zu_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
					for (int j = 0; j < numpilas; j++)
					{
						zr_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
						zrp_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
						zu_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
						for (int k = 0; k < numtiras; k++)
						{

							zr_dmjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
							zrp_dmjkl[i][m][j][k] = IloBoolVarArray(env, numbins);

							for (int kl = 0; kl < numbins; kl++)
							{
								zr_dmjkl[i][m][j][k][kl] = IloBoolVar(env);
								sprintf(nombre, "zr_%d_%d_%d_%d_%d", i, m, j, k, kl);
								zr_dmjkl[i][m][j][k][kl].setName(nombre);
								zrp_dmjkl[i][m][j][k][kl] = IloBoolVar(env);
								sprintf(nombre, "zrp_%d_%d_%d_%d_%d", i, m, j, k, kl);
								zrp_dmjkl[i][m][j][k][kl].setName(nombre);
							}
						}

					}
				}
			}
		}
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();
			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{
				ze_djkl[i] = BoolVarMatrix3(env, numpilas);

				for (int j = 0; j < numpilas; j++)
				{
					ze_djkl[i][j] = BoolVarMatrix(env, numtiras);
					for (int k = 0; k < numtiras; k++)
					{
						ze_djkl[i][j][k] = IloBoolVarArray(env, numbins);
						for (int kl = 0; kl < numbins; kl++)
						{
							ze_djkl[i][j][k][kl] = IloBoolVar(env);
							sprintf(nombre, "ze_%d_%d_%d_%d", i, j, k, kl);
							ze_djkl[i][j][k][kl].setName(nombre);
						}
					}

				}
			}
		}

	//inicializo las variables de los defectos


	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/

//	Lazy_minimo = false;
	//Todos los productos colocados
	for (int i = 0; i < numpieces; i++)
	{
		IloExpr v1(env);
		for (int j = 0; j < numpilas; j++)
		{
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						v1 += x_imjkl[i][m][j][k][l];
						v1 += xr_imjkl[i][m][j][k][l];
						if (P_set_xa[i])
						v1 += xa_imjkl[i][m][j][k][l];
						if (P_set_xar[i])
						v1 += xar_imjkl[i][m][j][k][l];
					}
				}
			}
		}
		if (Tipo_objetivo != 3)
			restr.add(IloRange(env, 1, v1, 1));
		else
			restr.add(IloRange(env, -IloInfinity, v1, 1));

		v1.end();
	}
	//Restricciones para el ORDEN de los trimming
	//CONSTRAINTS ORDERS TRIMMING
	if (P_trimming)
	{
		for (int j = 0; j < numpilas; j++)
		{
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{

						for (int i = 0; i < numpieces; i++)
						{


							if (P_set_xa[i])
							{
								IloExpr v1(env);
								v1 += xa_imjkl[i][m][j][k][l];

								for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
								{
									v1 += (-1) * x_imjkl[(*it)][m][j][k][l];
								}
								for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
								{
									v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];
								}
								restr.add(IloRange(env, -IloInfinity, v1, 0));
								v1.end();

							}
							if (P_set_xar[i])
							{
								IloExpr v1(env);
								v1 += xar_imjkl[i][m][j][k][l];

								for (std::set<int> ::iterator it = Dxy[i].begin(); it != Dxy[i].end(); it++)
								{
									v1 += (-1) * x_imjkl[(*it)][m][j][k][l];
								}
								for (std::set<int> ::iterator it = Dyy[i].begin(); it != Dyy[i].end(); it++)
								{
									v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];
								}
								restr.add(IloRange(env, -IloInfinity, v1, 0));
								v1.end();

							}

						}
					}
				}
			}
		}
	}
	//CONSTRAINTS ORDERS
		//Si coloco la segunda pieza también la primera
		//solamente una por celda
	if (P_orders)
	{

		for (int j = 0; j < numpilas; j++)
		{
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						IloExpr v1(env);
						IloExpr v2(env);
						IloExpr v3(env);
						for (int i = 0; i < numpieces; i++)
						{

							v2 += x_imjkl[i][m][j][k][l];
							v2 += xr_imjkl[i][m][j][k][l];
							if (P_set_xa[i])
								v3 += xa_imjkl[i][m][j][k][l];
							if (P_set_xar[i])
								v3 += xar_imjkl[i][m][j][k][l];

							v1 += x_imjkl[i][m][j][k][l];
							v1 += xr_imjkl[i][m][j][k][l];
							if (P_set_xa[i])
								v1 += xa_imjkl[i][m][j][k][l];
							if (P_set_xar[i])
								v1 += xar_imjkl[i][m][j][k][l];

							if (m > 0)
							{
								v1 += (-1) * x_imjkl[i][m - 1][j][k][l];
								v1 += (-1) * xr_imjkl[i][m - 1][j][k][l];
								if (P_set_xa[i])
									v1 += (-1) * xa_imjkl[i][m - 1][j][k][l];
								if (P_set_xar[i])
									v1 += (-1) * xar_imjkl[i][m - 1][j][k][l];
							}

						}
						//V1 indica que si ponemos la pieza m, antes también tiene que haber piezas
						if (m > 0)
						{
							//							restr.add(IloRange(env, -IloInfinity, v1, 0));
							//							v1.end();
						}
						restr.add(IloRange(env, -IloInfinity, v2, 1));
						v2.end();
						restr.add(IloRange(env, -IloInfinity, v3, 1));
						v3.end();



					}
				}
			}
		}
	}
		//	Lazy_minimo=false;
			//Altura mínima altura
	//CONSTRAINTS HEIGHT
		for (int j = 0; j < numpilas; j++)
		{

			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{

					for (int m = 0; m < numpiezasmax; m++)
					{
						IloExpr v1(env);
						IloExpr v2(env);

						for (int i = 0; i < numpieces; i++)
						{
							v1 += items[i].Getitem_h() * x_imjkl[i][m][j][k][l];
							v1 += items[i].Getitem_w() * xr_imjkl[i][m][j][k][l];
							v2 += items[i].Getitem_h() * x_imjkl[i][m][j][k][l];
							v2 += items[i].Getitem_w() * xr_imjkl[i][m][j][k][l];

							if (P_set_xa[i])
//							for (std::set<int>::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
							{
								v1 += items[i].Getitem_h() * xa_imjkl[i][m][j][k][l];
								v2 += items[i].Getitem_h() * xa_imjkl[i][m][j][k][l];
							}
							if (P_set_xar[i])
//							for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
							{
								v1 += items[i].Getitem_w() * xar_imjkl[i][m][j][k][l];
								v2 += items[i].Getitem_w() * xar_imjkl[i][m][j][k][l];
							}
							


						}
						v1 += (-1) * h_jkl[j][k][l];
						if (P_minimo_waste)	
							v1 += waste_min * (1 - el_mjkl[m][j][k][l]);

						restr.add(IloRange(env, -IloInfinity, v1, 0));
						v1.end();
						v2 += (-1) * h_jkl[j][k][l];
						if (P_minimo_waste)
						{
							v2 += plate_h * (1 - el_mjkl[m][j][k][l]);
							if (Lazy_minimo)
								restrLazy.add(IloRange(env, 0, v2, IloInfinity));
							else
								restr.add(IloRange(env, 0, v2, IloInfinity));
						}
						v2.end();

					}
				}
			}
		}
		//CONSTRAINTS MINIMIZAR TIRAS
		if (Tipo_objetivo == 4)
		{
			//Mete una que obligue a que si W_K vale algo es porque tiene algo
			for (int k = 0; k < numtiras; k++)
			{
				IloExpr v1(env);
				v1 += W_K[k];
				for (int i = 0; i < numpieces; i++)
				{
					for (int j = 0; j < numpilas; j++)
					{
						for (int m = 0; m < numpiezasmax; m++)
						{
							v1 += (-1) * x_imjkl[i][m][j][k][0];
							v1 += (-1) * xr_imjkl[i][m][j][k][0];
						}
					}
				}
				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
			}

		}

		// CONSTRAINT TRIMMING
		//Si pongo un triming la altura tiene que ser perfecta
		if (P_trimming)
		{
			for (int j = 0; j < numpilas; j++)
			{

				for (int k = 0; k < numtiras; k++)
				{
					for (int l = 0; l < numbins; l++)
					{

						for (int m = 0; m < numpiezasmax; m++)
						{

							IloExpr v1(env);
							IloExpr v2(env);
							for (int i = 0; i < numpieces; i++)
							{
								if (P_set_xa[i])
									//							for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
								{
									v1 += xa_imjkl[i][m][j][k][l];
									v2 += xa_imjkl[i][m][j][k][l];
								}
								if (P_set_xar[i])
									//							for (std::set<int>::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
								{
									v1 += xar_imjkl[i][m][j][k][l];
									v2 += xar_imjkl[i][m][j][k][l];
								}





							}
							v2 += (1 - zu_mjkl[m][j][k][l]);

							v1 += (-1) * el_mjkl[m][j][k][l];
							restr.add(IloRange(env, -IloInfinity, v1, 0));
							v1.end();
							restr.add(IloRange(env, -IloInfinity, v2, 1));
							v2.end();
						}
					}
				}
			}
		}
	//CONSTRAINT WIDTH
	
//	Lazy_minimo = false;
	//Anchura de los cortes en cada pila
	//minimo corte consecutivo en los de tres

	for (int j = 0; j < numpilas; j++)
	{

		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					IloExpr v1(env);
					IloExpr v2(env);

					for (int i = 0; i < numpieces; i++)
					{


						v1 += items[i].Getitem_w() * x_imjkl[i][m][j][k][l];
						v1 += items[i].Getitem_h() * xr_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_w() * x_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_h() * xr_imjkl[i][m][j][k][l];

					}
					v1 += (-1) * w_mjkl[m][j][k][l];
					if (P_minimo_waste)
					v1 += (1 - es_mjkl[m][j][k][l]) * waste_min;
					v2 += (-1) * w_mjkl[m][j][k][l];
					v2 += (1 - es_mjkl[m][j][k][l]) * max1Cut;
					
					restr.add(IloRange(env, -IloInfinity, v1, 0));
					v1.end();
					if (P_minimo_waste)
					{
						if (Lazy_minimo)
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						else
							restr.add(IloRange(env, 0, v2, IloInfinity));
					}
					v2.end();




				}
			}
		}
	}

	//CONSTRAINT HEIGHT
	//suma de las alturas de las pilas menor H


	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			IloExpr v1(env);
			for (int j = 0; j < numpilas; j++)
			{


				v1 += h_jkl[j][k][l];
			}
			v1 += (-1) * plate_h * y_kl[k][l];
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
		}
	}
	//CONSTRAINT WIDHT
	/**/
	//suma de minimo corte anchos de las tiras
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			for (int j = 0; j < numpilas; j++)
			{

				IloExpr v1(env);
				IloExpr v2(env);
				for (int m = 0; m < numpiezasmax; m++)
				{
					v1 += w_mjkl[m][j][k][l];
					v2 += w_mjkl[m][j][k][l];

				}

				v1 += (-1) * w_kl[k][l];
				if (P_minimo_waste)
				v1 += (1 - es_jkl[j][k][l]) * waste_min;
				
				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();

				v2 += (-1) * w_kl[k][l];
				v2 += (1 - es_jkl[j][k][l]) * max1Cut;
				if (P_minimo_waste)
				{
					if (Lazy_minimo)
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));

					else
						restr.add(IloRange(env, 0, v2, IloInfinity));
				}
				v2.end();
			}

		}
	}

	//minimos cortes
		//suma de las alturas de las pilas menor H
	
//	Lazy_minimo = true;
	
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{

			for (int j = 0; j < numpilas; j++)
			{
				//				Lazy_minimo = true;
				IloExpr v1(env);
				v1 += min2Cut * ml_jkl[j][k][l];

				v1 += (-1) * h_jkl[j][k][l];
				
				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
				IloExpr v2(env);
				v2 += plate_h * ml_jkl[j][k][l];
				v2 += (-1) * h_jkl[j][k][l];
				if (Lazy_minimo)
					restrLazy.add(IloRange(env, 0, v2, IloInfinity));
				else
					restr.add(IloRange(env, 0, v2, IloInfinity));
				v2.end();
				//no crea pilas vacías y además ordenadas
				if (P_order_tiras)
				{
					IloExpr v3(env);
					v3 += ml_jkl[j][k][l];
					for (int i = 0; i < numpieces; i++)
					{
						for (int m = 0; m < numpiezasmax; m++)
						{
							v3 += (-1) * x_imjkl[i][m][j][k][l];
							v3 += (-1) * xr_imjkl[i][m][j][k][l];
						}
					}
					restr.add(IloRange(env, -IloInfinity, v3, 0));
					v3.end();

					if (j > 0)
					{
						IloExpr v4(env);
						v4 += ml_jkl[j][k][l];
						v4 += (-1) * ml_jkl[j - 1][k][l];
						restr.add(IloRange(env, -IloInfinity, v4, 0));
						v4.end();
					}

				}


			}
			//			Lazy_minimo = false;
			IloExpr v1(env);
			v1 += min1Cut * ms_kl[k][l];

			v1 += (-1) * w_kl[k][l];

			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
			IloExpr v2(env);
			v2 += max1Cut * ms_kl[k][l];
			v2 += (-1) * w_kl[k][l];
			if (Lazy_minimo)
				restrLazy.add(IloRange(env, 0, v2, IloInfinity));
			else
				restr.add(IloRange(env, 0, v2, IloInfinity));
			v2.end();

			//no crea tiras vacías y además ordenadas
			if (P_order_tiras)
			{
				IloExpr v3(env);
				v3 += ms_kl[k][l];
				for (int i = 0; i < numpieces; i++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						for (int j = 0; j < numpilas; j++)
						{
							v3 += (-1) * x_imjkl[i][m][j][k][l];
							v3 += (-1) * xr_imjkl[i][m][j][k][l];
						}
					}
				}
				restr.add(IloRange(env, -IloInfinity, v3, 0));
				v3.end();
				if (k > 0)
				{
					IloExpr v4(env);
					v4 += ms_kl[k][l];
					v4 += (-1) * ms_kl[k - 1][l];
					restr.add(IloRange(env, -IloInfinity, v4, 0));
					v4.end();
				}
			}
		}
	}
	//CONSTRAINT WIDTH
	//Suma  tiras menor o igual a W
	for (int l = 0; l < numbins; l++)
	{
		IloExpr v1(env);
		for (int k = 0; k < numtiras; k++)
		{

			v1 += w_kl[k][l];

		}

		v1 += (-1) * plate_w * y_l[l];
		restr.add(IloRange(env, -IloInfinity, v1, 0));
		v1.end();
	}
	

	//CONSTRAINT ORDER STACKS
	//Orden de los stacks por parejas
	//******************

	//
	if (P_orders)
	{
		for (int l = 0; l < numbins; l++)
		{

			for (int k = 0; k < numtiras; k++)
			{

				for (int j = 0; j < numpilas; j++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						for (int i = numpieces - 1; i > 0; i--)
						{


							if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1 && (items[i].Getitem_seq() > 1))
							{
								int kk = stacks[items[i].Getitem_stack()].Getitem_nbr();
								IloExpr v1(env);
								v1 += x_imjkl[i][m][j][k][l];
								v1 += xr_imjkl[i][m][j][k][l];
								if (P_set_xa[i])
									v1 += xa_imjkl[i][m][j][k][l];
								if (P_set_xar[i])
									v1 += xar_imjkl[i][m][j][k][l];
								// ponemos las variables
								int i2 = i - 1;
								if (i2 >= 0 && items[i].Getitem_stack() == items[i2].Getitem_stack())
								{

									for (int l2 = 0; l2 <= l; l2++)
									{
										for (int k2 = 0; k2 < numtiras; k2++)
										{
											if (l2 == l && k2 > k)
												break;
											for (int j2 = 0; j2 < numpilas; j2++)
											{
												if (l2 == l && k2 == k && j2 > j)
													break;
												for (int m2 = 0; m2 < numpiezasmax; m2++)
												{
													if (l2 == l && k2 == k && j2 == j && m2 > m)
														break;

													v1 += (-1) * x_imjkl[i2][m2][j2][k2][l2];
													v1 += (-1) * xr_imjkl[i2][m2][j2][k2][l2];
													if (P_set_xa[i2])
														v1 += (-1) * xa_imjkl[i2][m2][j2][k2][l2];
													if (P_set_xar[i2])
														v1 += (-1) * xar_imjkl[i2][m2][j2][k2][l2];

												}
											}

										}
									}

								}
								for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
										v1 += (-1) * x_imjkl[(*it)][m][j][k][l];
								}
								for (std::set<int> ::iterator it = Dxy[i].begin(); it != Dxy[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
										v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];
								}
								for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
										v1 += (-1) * x_imjkl[(*it)][m][j][k][l];
								}
								for (std::set<int> ::iterator it = Dyy[i].begin(); it != Dyy[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
										v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];
								}
								if (Lazy_orden)
									restrLazy.add(IloRange(env, -IloInfinity, v1, 0));

								else
									restr.add(IloRange(env, -IloInfinity, v1, 0));
								v1.end();




							}



						}
					}
				}
			}
		}
	}
	//CONSTRAINTS CUT VERTICAL
	//restricciones de los cortes verticales
	if (P_defects)
	{
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{

				for (int k1 = 0; k1 < numtiras; k1++)
				{
					IloExpr v1(env);
					IloExpr v2(env);
					for (int k2 = 0; k2 <= k1; k2++)
					{
						v1 += w_kl[k2][l];
						v2 += w_kl[k2][l];
					}
					v1 += (-1) * ((*itD).Getpos_x());

					v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1][l]);
					//			v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth());
					v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zr_dkl[i][k1][l]);


					if (Lazy_cortes)
					{
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
					}
					else
					{
						restr.add(IloRange(env, 0, v2, IloInfinity));
						restr.add(IloRange(env, -IloInfinity, v1, 0));
					}
					v1.end();
					v2.end();
					if (k1 > 0)
					{
						IloExpr v3(env);
						v3 += zr_dkl[i][k1][l];
						v3 += (-1) * zr_dkl[i][k1 - 1][l];
						restr.add(IloRange(env, -IloInfinity, v3, 0));
						v3.end();
					}
				}



			}
		}
		//CONSTRAINTS CUT HORIZONTAL
		//restricciones de los cortes horizontales
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{
				for (int k = 0; k < numtiras; k++)
				{

					for (int j1 = 0; j1 < numpilas; j1++)
					{
						IloExpr v1(env);
						IloExpr v2(env);
						for (int j2 = 0; j2 <= j1; j2++)
						{

							v1 += h_jkl[j2][k][l];
							v2 += h_jkl[j2][k][l];
						}



						v1 += (-1) * ((*itD).Getpos_y());
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - ze_djkl[i][j1][k][l]);
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (zr_dkl[i][k][l]);
						if (k > 0)
						{
							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zr_dkl[i][k - 1][l]);
						}

						//ahora falta la otra parte
						int rtyd = (*itD).Getpos_y() + (*itD).Getheight();
						v2 += (-1) * (rtyd) * (1 - ze_djkl[i][j1][k][l]);
						v2 += (rtyd) * (zr_dkl[i][k][l]);
						if (k > 0)
						{
							v2 += rtyd * (1 - zr_dkl[i][k - 1][l]);
						}

						if (Lazy_cortes)
						{
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
							restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
						}
						else
						{
							restr.add(IloRange(env, 0, v2, IloInfinity));
							restr.add(IloRange(env, -IloInfinity, v1, 0));
						}
						v1.end();
						v2.end();
						if (j1 > 0)
						{
							IloExpr v3(env);
							v3 += ze_djkl[i][j1][k][l];
							v3 += (-1) * ze_djkl[i][j1 - 1][k][l];
							restr.add(IloRange(env, -IloInfinity, v3, 0));
							v3.end();


						}
						//Quitar y pongo las otras
		//				IloExpr v4(env);
		//				v4 += ze_djkl[i][j1][k][l];
		//				if (k > 0)
		//					v4 += (-1) * (zr_dkl[i][k - 1][l] - zr_dkl[i][k][l]);
		//				else
		//					v4 += (-1) * (1 - zr_dkl[i][k][l]);
		//				restr.add(IloRange(env, -IloInfinity, v4, 0));
		//				v4.end();
		//				

					}
					//orden de las variables
				}






			}
		}
		//CONSTRAINTS HORIZONTAL 2ORDER

		//restricciones de los cortes verticales de segundo orden de defecto

		//Deerecha
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{

				for (int k1 = 0; k1 < numtiras; k1++)
				{
					for (int j = 0; j < numpilas; j++)
					{
						for (int m = 0; m < numpiezasmax; m++)
						{
							IloExpr v1(env);
							IloExpr v2(env);
							if (k1 > 0)
							{
								for (int k2 = 0; k2 < k1; k2++)
								{

									v1 += w_kl[k2][l];
									v2 += w_kl[k2][l];
								}
							}
							for (int m2 = 0; m2 <= m; m2++)
							{

								v1 += w_mjkl[m2][j][k1][l];
								v2 += w_mjkl[m2][j][k1][l];
							}
							v1 += (-1) * ((*itD).Getpos_x());
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dmjkl[i][m][j][k1][l]);
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * ze_djkl[i][j][k1][l];
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (zr_dkl[i][k1][l]);
							if (k1 > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1 - 1][l]);
							if (j > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - ze_djkl[i][j - 1][k1][l]);
							int rtxd = (*itD).Getpos_x() + (*itD).Getwidth();
							v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zr_dmjkl[i][m][j][k1][l]);
							v2 += (rtxd)*ze_djkl[i][j][k1][l];
							v2 += (rtxd) * (zr_dkl[i][k1][l]);
							if (k1 > 0)
								v2 += (rtxd) * (1 - zr_dkl[i][k1 - 1][l]);

							if (j > 0)
								v2 += (rtxd) * (1 - ze_djkl[i][j - 1][k1][l]);
							if (Lazy_cortes)
							{
								restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
								restrLazy.add(IloRange(env, 0, v2, IloInfinity));
							}
							else
							{
								restr.add(IloRange(env, -IloInfinity, v1, 0));
								restr.add(IloRange(env, 0, v2, IloInfinity));
							}
							v1.end();
							v2.end();
							if (m > 0)
							{
								IloExpr v3(env);
								v3 += zr_dmjkl[i][m][j][k1][l];
								v3 += (-1) * zr_dmjkl[i][m - 1][j][k1][l];
								restr.add(IloRange(env, -IloInfinity, v3, 0));
								v3.end();
							}
							//if (m ==0)
							//{
							//	IloExpr v3(env);
							//	v3 += zr_dkl[i][k1][l];
							//	v3 += (-1) * zr_dmjkl[i][numpiezasmax-1][j][k1][l];
							//	restr.add(IloRange(env, -IloInfinity, v3, 0));
							//	v3.end();
							//}

							IloExpr v4(env);
							v4 += zr_dmjkl[i][m][j][k1][l];
							if (j > 0)
								v4 += (-1) * (ze_djkl[i][j - 1][k1][l] - ze_djkl[i][j][k1][l]);
							else
								v4 += (-1) * (1 - ze_djkl[i][j][k1][l]);
							restr.add(IloRange(env, -IloInfinity, v4, 0));
							v4.end();


						}
					}
				}



			}
		}

		//CONSTRAINTS HORIZONTAL 2ORDER LEFT
		//cortes
		//restricciones de los cortes verticales de segundo orden de defecto
		//Izquierda
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{

				for (int k1 = 0; k1 < numtiras; k1++)
				{
					for (int j = 0; j < numpilas; j++)
					{
						for (int m = 0; m < numpiezasmax; m++)
						{
							IloExpr v1(env);
							IloExpr v2(env);
							if (k1 > 0)
							{
								for (int k2 = 0; k2 < k1; k2++)
								{

									v1 += w_kl[k2][l];
									v2 += w_kl[k2][l];
								}
							}
							for (int m2 = 0; m2 <= m; m2++)
							{

								v1 += w_mjkl[m2][j][k1][l];
								v2 += w_mjkl[m2][j][k1][l];
							}
							for (int i1 = 0; i1 < numpieces; i1++)
							{
								v1 += (-1) * items[i1].Getitem_w() * x_imjkl[i1][m][j][k1][l];
								v1 += (-1) * items[i1].Getitem_h() * xr_imjkl[i1][m][j][k1][l];
								v2 += (-1) * items[i1].Getitem_w() * x_imjkl[i1][m][j][k1][l];
								v2 += (-1) * items[i1].Getitem_h() * xr_imjkl[i1][m][j][k1][l];
							}


							v1 += (-1) * ((*itD).Getpos_x());
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zrp_dmjkl[i][m][j][k1][l]);
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * ze_djkl[i][j][k1][l];
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * zr_dkl[i][k1][l];
							if (k1 > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1 - 1][l]);

							if (j > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - ze_djkl[i][j - 1][k1][l]);
							int rtxd = (*itD).Getpos_x() + (*itD).Getwidth();
							v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zrp_dmjkl[i][m][j][k1][l]);
							v2 += (rtxd)*ze_djkl[i][j][k1][l];
							v2 += (rtxd)*zr_dkl[i][k1][l];
							if (k1 > 0)
								v2 += (rtxd) * (1 - zr_dkl[i][k1 - 1][l]);

							if (j > 0)
								v2 += (rtxd) * (1 - ze_djkl[i][j - 1][k1][l]);
							if (Lazy_cortes)
							{
								restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
								restrLazy.add(IloRange(env, 0, v2, IloInfinity));
							}
							else
							{
								restr.add(IloRange(env, -IloInfinity, v1, 0));
								restr.add(IloRange(env, 0, v2, IloInfinity));
							}
							v1.end();
							v2.end();
							if (m > 0)
							{
								IloExpr v3(env);
								v3 += zrp_dmjkl[i][m][j][k1][l];
								v3 += (-1) * zrp_dmjkl[i][m - 1][j][k1][l];
								restr.add(IloRange(env, -IloInfinity, v3, 0));
								v3.end();
							}
							//if (m ==0)
							//{
							//	IloExpr v3(env);
							//	v3 += zr_dkl[i][k1][l];
							//	v3 += (-1) * zr_dmjkl[i][numpiezasmax-1][j][k1][l];
							//	restr.add(IloRange(env, -IloInfinity, v3, 0));
							//	v3.end();
							//}

							//IloExpr v4(env);
							//v4 += zrp_dmjkl[i][m][j][k1][l];
							//if (j > 0)
							//	v4 += (-1) * (ze_djkl[i][j - 1][k1][l] - ze_djkl[i][j][k1][l]);
							//else
							//	v4 += (-1) * (1 - ze_djkl[i][j][k1][l]);
							//restr.add(IloRange(env, -IloInfinity, v4, 0));
							//v4.end();

						}
					}
				}



			}
		}
		//CONSTRAINTS HORIZONTAL 2ORDER RIGHT
		//restricciones de los cortes horizontales de la pieza
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{
				for (int k = 0; k < numtiras; k++)
				{

					for (int j1 = 0; j1 < numpilas; j1++)
					{

						for (int m1 = 0; m1 < numpiezasmax; m1++)
						{
							IloExpr v1(env);
							IloExpr v2(env);
							for (int j2 = 0; j2 <= j1; j2++)
							{
								if (j2 < j1)
									v1 += h_jkl[j2][k][l];
								v2 += h_jkl[j2][k][l];
							}
							for (int i1 = 0; i1 < numpieces; i1++)
							{
								v1 += items[i1].Getitem_h() * (x_imjkl[i1][m1][j1][k][l]);
								v1 += items[i1].Getitem_w() * (xr_imjkl[i1][m1][j1][k][l]);
								v2 += (-1) * items[i1].Getitem_h() * x_imjkl[i1][m1][j1][k][l];
								v2 += (-1) * items[i1].Getitem_w() * xr_imjkl[i1][m1][j1][k][l];
							}


							v1 += (-1) * ((*itD).Getpos_y());
							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zu_mjkl[m1][j1][k][l]);

							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zrp_dmjkl[i][m1][j1][k][l]);
							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (zr_dmjkl[i][m1][j1][k][l]);

							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (ze_djkl[i][j1][k][l]);
							if (j1 > 0)
								v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - ze_djkl[i][j1 - 1][k][l]);
							//ahora falta la otra parte
							int rtyd = (*itD).Getpos_y() + (*itD).Getheight();
							v2 += (-1) * (rtyd) * (1 - zu_mjkl[m1][j1][k][l]);
							v2 += ((rtyd) * (1 - zrp_dmjkl[i][m1][j1][k][l]));
							v2 += ((rtyd)) * (zr_dmjkl[i][m1][j1][k][l]);


							v2 += ((rtyd)) * (ze_djkl[i][j1][k][l]);
							if (j1 > 0)
								v2 += (rtyd) * (1 - ze_djkl[i][j1 - 1][k][l]);
							restr.add(IloRange(env, -IloInfinity, v1, 0));
							if (Lazy_cortes)
							{
								restrLazy.add(IloRange(env, 0, v2, IloInfinity));

							}
							else
							{
								restr.add(IloRange(env, 0, v2, IloInfinity));

							}
							v1.end();
							v2.end();

							//if (j1 > 0)
							//{
							//	IloExpr v3(env);
							//	v3 += ze_djkl[i][j1][k][l];
							//	v3 += (-1) * ze_djkl[i][j1 - 1][k][l];
							//	restr.add(IloRange(env, -IloInfinity, v3, 0));
							//	v3.end();


							//}
							//IloExpr v4(env);
							//v4 +=2* zu_mjkl[m1][j1][k][l];

							//v4 += (-1) * zrp_dmjkl[i][m1][j1][k][l];
							//v4 += (-1) * (1-zr_dmjkl[i][m1][j1][k][l]);

							//restr.add(IloRange(env, -IloInfinity, v4, 0));
							//v4.end();


						}


					}
					//orden de las variables
				}






			}
		}
	}
	if (P_cota)
	{
		if ((Tipo_objetivo == 2 && ((P_TrozoArea < 6000) && (numbins == P_BinsCotaArea + 1))))
		{

			IloExpr v4(env);
			for (int k = 0; k < numtiras; k++)
			{



				v4 += w_kl[k][numbins - 1];
			}
			v4 += (-1) * P_TrozoArea;
			restr.add(IloRange(env, 0, v4, IloInfinity));
			v4.end();

		}
	}
	model.add(restr);
	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
//	model.add(restr);


	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);
	if (Tipo_objetivo == 4)
	{
//		IloExpr Expression_fobj(env);
		for (int i = 0; i < numtiras; i++)
		{
			Expression_fobj += W_K[i];
		}


	}
	if (Tipo_objetivo == 0)//biobjective //estaría en diet.ccp
	{
		IloExpr Expression_fobj1(env);
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj1 += y_l[i];
		}
		IloExpr Expression_fobj2(env);
		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj2 += w_kl[k][numbins - 1];
		}
	}

	if (Tipo_objetivo == 1)//por bins
	{
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj += y_l[i];
		}
	}
	if (Tipo_objetivo == 2)//por tira del último bin
	{

		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj += w_kl[k][numbins - 1];
		}
	}
	if (Tipo_objetivo == 3)//por área de lo metido en el bin
	{
		Expression_fobj += plate_h * plate_w;
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int j = 0; j < numpilas; j++)
				{

					for (int m = 0; m < numpiezasmax; m++)
					{

						for (int i = 0; i < numpieces; i++)
						{
							Expression_fobj += (-1) * items[i].Getitem_area() * x_imjkl[i][m][j][k][l];
							Expression_fobj += (-1) * items[i].Getitem_area() * xr_imjkl[i][m][j][k][l];
						}
					}
				}
			}
		}
	}

	if (Tipo_objetivo != 0 && Tipo_objetivo != 4)
		model.add(IloMinimize(env, Expression_fobj));
	if (Tipo_objetivo==4)
		model.add(IloMaximize(env, Expression_fobj));

	Expression_fobj.end();


	/***************************************************************/
	/*************** EJECUTAMOS EL MODELO **************************/
	/***************************************************************/
	try
	{
		printf("Aqui211");
		IloCplex cplex(model);
		printf("Versión %s", cplex.getVersion());

		//Si queremos las lazy y tenemos alguna
		if (Lazy_minimo)
		{
			cplex.addLazyConstraints(restrLazy);
		}
		printf("Aqui2112");
		//		cplex.exportModel("Roadef.lp");
				//No hace el probing
				//cplex.setParam(IloCplex::Probe, -1);
				//1 CORE
		cplex.setParam(IloCplex::Threads, nt);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, max_time); //TIEMPO EN SEGUNDOS
//		if (Tipo_objetivo == 2)
//			cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, LastBinWidth);

		if (G_Dibujar)
		cplex.exportModel("Roadef.lp");

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

		//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();

		if (P_initial_solution)
		{

			FILE* file2;
			//	strcat(name_instance, "_solution.csv");

			file2 = fopen(P_file_to_write.c_str(), "w+");
			if (file2 == NULL)
			{
				printf("Problemas al leer el archivo %s, no se encuentra el archivo", P_file_to_write.c_str());
				exit(4);
			}
			int PLATE_ID = 0;
			int	NODE_ID = 0;
			int	X = 0;
			int Y = 0;
			int	WIDTH = 0;
			int	HEIGHT = 0;
			int TYPE = 0;
			int CUT = 0;
			int PARENT = 0;
			char kk[256];
			fscanf(file2, "%s\n", kk);
			fscanf(file2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &PLATE_ID, &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT);
			int section = 0;
			int level = -1;
			int stripe = -1;
			int plate = 0;
			while (!feof(file2) && CUT != (-3))
			{

				fscanf(file2, "%d\t", &PLATE_ID);
				if (PLATE_ID == plate)
					fscanf(file2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT, &PARENT);
				else
					fscanf(file2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t", &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT);
				plate = PLATE_ID;
				if (CUT == 1)
				{
					stripe++;
					level = -1;
					section = 0;
					continue;
				}
				if (CUT == 2)
				{
					level++;
					section = 0;
					continue;
				}
				if (CUT == 3 && TYPE < 0)
				{
					section++;
					continue;
				}
				if (items[TYPE].Getitem_h() == HEIGHT)
				{
					//VAriable 
					vars.add(xr_imjkl[TYPE][section][level][stripe][plate]);
					vals.add(1.0);
				}

				//		fscanf(fin2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &W, &H, &plates, &piezas, &defectos, &plate_defectos, &obj, &width_total);
			}
			cplex.addMIPStart(vars, vals);

		}
		IloBool result = cplex.solve();
		printf("Aqui2113");
		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;

		//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
//				fprintf(f, "Infeasible\n");
				printf( "Infeasible\n");
				//				cplex.exportModel("Roadef.lp");
			}
			else
			{
				if (cplex.getStatus() == IloAlgorithm::Unbounded) 
//					fprintf(f, "Unbounded\n");
					printf("Unbounded\n");

				//				fprintf(f, "\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
				printf( "\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}
//			fclose(f);
		}
		else
		{
//			FILE* f = fopen("./estadomodelo.txt", "a+");
//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			Best_Value_Formulation = cplex.getObjValue();
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible
			if (cplex.getSolnPoolNsolns() > 0)
			{
				int value2 = cplex.getObjValue();
				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
				if (G_Dibujar )
				{
					for (int l = 0; l < numbins; l++)
					{
						for (int k = 0; k < numtiras; k++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								for (int m = 0; m < numpiezasmax; m++)
								{
									for (int i = 0; i < numpieces; i++)
									{


										if (cplex.isExtracted(x_imjkl[i][m][j][k][l]) && cplex.getValue(x_imjkl[i][m][j][k][l]) > 0.001)
										{
											printf("%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											//										fprintf(f, "%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
										}
										if (cplex.isExtracted(xr_imjkl[i][m][j][k][l]) && cplex.getValue(xr_imjkl[i][m][j][k][l]) > 0.001)
										{
											printf("%s %d w %d h %d s %d\n", xr_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
											//										fprintf(f, "%s %d w %d h %d s %d\n", xr_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
										}
										if (P_set_xa[i])
										{
											if (cplex.isExtracted(xa_imjkl[i][m][j][k][l]) && cplex.getValue(xa_imjkl[i][m][j][k][l]) > 0.001)
											{
												printf("%s %d w %d h %d s %d\n", xa_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
												//										fprintf(f, "%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											}
										}
										if (P_set_xar[i])
										{
											if (cplex.isExtracted(xar_imjkl[i][m][j][k][l]) && cplex.getValue(xar_imjkl[i][m][j][k][l]) > 0.001)
											{
												printf("%s %d w %d h %d s %d\n", xar_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
												//										fprintf(f, "%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											}
										}

									}
								}

							}
						}
					}
					//Pinto las variables w_mjkl
					for (int l = 0; l < numbins; l++)
					{
						for (int k = 0; k < numtiras; k++)
						{
							for (int j = 0; j < numpilas; j++)
							{
								for (int m = 0; m < numpiezasmax; m++)
								{
									if (cplex.isExtracted(w_mjkl[m][j][k][l]) && cplex.getValue(w_mjkl[m][j][k][l]) > 0.001)
									{
										printf("%s %2.f\n", w_mjkl[m][j][k][l].getName(), cplex.getValue(w_mjkl[m][j][k][l]));
										//								fprintf(f, "%s %2.f\n", w_mjkl[m][j][k][l].getName(), cplex.getValue(w_mjkl[m][j][k][l]));
									}
								}

							}

						}
					}
					//Pinto las variables h_jkl
					for (int l = 0; l < numbins; l++)
					{
						for (int k = 0; k < numtiras; k++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								if (cplex.isExtracted(h_jkl[j][k][l]) && cplex.getValue(h_jkl[j][k][l]) > 0.001)
								{
									printf("%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
									//								fprintf(f, "%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
								}

							}

						}
					}
					//Pinto las variables w_kl y y_kl
					for (int j = 0; j < numbins; j++)
					{
						for (int i = 0; i < numtiras; i++)
						{

							if (cplex.isExtracted(w_kl[i][j]) && cplex.getValue(w_kl[i][j]) > 0.001)
							{
								printf("%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
								//							fprintf(f, "%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
							}
							if (cplex.isExtracted(y_kl[i][j]) && cplex.getValue(y_kl[i][j]) > 0.001)
							{
								printf("%s %d\n", y_kl[i][j].getName(), 1);
								//							fprintf(f, "%s %d\n", y_kl[i][j].getName(), 1);
							}

						}
					}

					//Pinto las variable y_l
					for (int l = 0; l < numbins; l++)
					{
						if (cplex.isExtracted(y_l[l]) && cplex.getValue(y_l[l]) > 0.001)
						{
							printf("%s %d\n", y_l[l].getName(), 1);
							//						fprintf(f, "%s %d\n", y_l[l].getName(), 1);
						}

					}
					//Pinto los cortes
					for (int l = 0; l < numbins; l++)
					{
						int nd = DefectsPlate[l].size();

						int d = 0;
						for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								for (int k = 0; k < numtiras; k++)
								{
									for (int m = 0; m < numpiezasmax; m++)
									{
										if (cplex.isExtracted(zr_dmjkl[d][m][j][k][l]) && cplex.getValue(zr_dmjkl[d][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zr_dmjkl[d][m][j][k][l].getName(), 1);
											//										fprintf(f, "%s %d\n", zr_dmjkl[d][m][j][k][l].getName(), 1);
										}
										if (cplex.isExtracted(zrp_dmjkl[d][m][j][k][l]) && cplex.getValue(zrp_dmjkl[d][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zrp_dmjkl[d][m][j][k][l].getName(), 1);
											//										fprintf(f, "%s %d\n", zrp_dmjkl[d][m][j][k][l].getName(), 1);
										}
										if (m == 0)
										{
											if (cplex.isExtracted(ze_djkl[d][j][k][l]) && cplex.getValue(ze_djkl[d][j][k][l]) > 0.001)
											{
												printf("%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
												//											fprintf(f, "%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
											}
										}
										if (j == 0 && m == 0)
										{
											if (cplex.isExtracted(zr_dkl[d][k][l]) && cplex.getValue(zr_dkl[d][k][l]) > 0.001)
											{
												printf("%s %d\n", zr_dkl[d][k][l].getName(), 1);
												//											fprintf(f, "%s %d\n", zr_dkl[d][k][l].getName(), 1);
											}

										}

									}

								}
							}
						}

					}
					//Pinto las variables zlr,zllr, zll,zlrr
					printf("\nVariables para los defectos\n");

					for (int l = 0; l < numbins; l++)
					{
						int nd = DefectsPlate[l].size();

						int d = 0;
						for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
						{
							for (int i = 0; i < numpieces; i++)
							{
								for (int j = 0; j < numpilas; j++)
								{

									for (int k = 0; k < numtiras; k++)
									{
										for (int m = 0; m < numpiezasmax; m++)
										{
											if (d == 0)
											{
												if (cplex.isExtracted(zar_imjkl[i][m][j][k][l]) && cplex.getValue(zar_imjkl[i][m][j][k][l]) > 0.001)
												{
													printf("%s %d\n", zar_imjkl[i][m][j][k][l].getName(), 1);
													//												fprintf(f, "%s %d\n", zar_imjkl[i][m][j][k][l].getName(), 1);
												}
												if (cplex.isExtracted(za_imjkl[i][m][j][k][l]) && cplex.getValue(za_imjkl[i][m][j][k][l]) > 0.001)
												{
													printf("%s %d\n", za_imjkl[i][m][j][k][l].getName(), 1);
													//												fprintf(f, "%s %d\n", za_imjkl[i][m][j][k][l].getName(), 1);
												}
											}

										}

									}
								}
							}
						}
					}

					printf("\nVariables de mínimo\n");
					for (int i = 0; i < numpieces; i++)
					{

						for (int j = 0; j < numpilas; j++)
						{

							for (int k = 0; k < numtiras; k++)
							{
								for (int l = 0; l < numbins; l++)
								{
									if (i == 0)
									{
										for (int m = 0; m < numpiezasmax; m++)
										{
											if (cplex.isExtracted(el_mjkl[m][j][k][l]) && cplex.getValue(el_mjkl[m][j][k][l]) > 0.001)
											{
												printf("%s %d\n", el_mjkl[m][j][k][l].getName(), 1);
												//											fprintf(f, "%s %d\n", el_mjkl[m][j][k][l].getName(), 1);
											}
										}
									}
									if (i == 0)
									{
										if (cplex.isExtracted(es_jkl[j][k][l]) && cplex.getValue(es_jkl[j][k][l]) > 0.001)
										{
											printf("%s %d\n", es_jkl[j][k][l].getName(), 1);
											//										fprintf(f, "%s %d\n", es_jkl[j][k][l].getName(), 1);
										}

									}

								}
							}
						}
					}
				}
			}
			//Escribo la solución para dibujar
				//Inicio
			if (G_Dibujar || G_Draw_Exacto)
			{
				FILE* fin3;
				if (G_Draw_Exacto)
				{
					std::string str = file_idx + "_solution_" + std::to_string(G_Exacto) + "_" + std::to_string(Tipo_objetivo) + ".txt";
					fin3=fopen(P_file_to_write.c_str(), "w+");
//					fin3 = fopen(str.c_str(), "w+");
				}
				else
					fin3 = fopen("./temp2.txt", "w+");				int ndefects = 0;
				int nbinsdefects = 0;
				for (int i = 0; i < numbins; i++)
				{
					if (DefectsPlate[i].size() >= 1)
						nbinsdefects++;
					for (std::list< GlassDefect > ::iterator it = DefectsPlate[i].begin(); it != DefectsPlate[i].end(); it++)
					{
						ndefects++;
					}
				}
				//					fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, Best_Objective_function);
				fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, numpieces, ndefects, nbinsdefects, LB_Best_Value_Formulation, Best_Value_Formulation);
				//defectos

				for (int i = 0; i < numbins; i++)
				{

					for (std::list< GlassDefect > ::iterator it = DefectsPlate[i].begin(); it != DefectsPlate[i].end(); it++)
					{
						// Get defect coordinates.
						int defect_x = (*it).Getpos_x();
						int defect_y = (*it).Getpos_y();
						int defect_width = (*it).Getwidth();
						int defect_height = (*it).Getheight();

						fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", defect_x, defect_y, defect_width, defect_height, i);
					}
				}
				for (int i = 0; i < numbins; i++)
				{
					if (DefectsPlate[i].size() > 0)
						fprintf(fin3, "%d\t%d\n", i, DefectsPlate[i].size());

				}
				for (int l = 0; l < numbins; l++)
				{
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							int pos_x = 0;
							int pos_X = 0;
							int pos_Y = 0;
							for (int k1 = 0; k1 < k; k1++)
							{
								if (cplex.isExtracted(w_kl[k1][l]) && cplex.getValue(w_kl[k1][l]) > 0.001)
								{
									pos_X += cplex.getValue(w_kl[k1][l]);
								}
							}
							int pos_y = 0;
							for (int j1 = 0; j1 < j; j1++)
							{
								if (cplex.isExtracted(h_jkl[j1][k][l]) && cplex.getValue(h_jkl[j1][k][l]) > 0.001)
								{
									pos_y += cplex.getValue(h_jkl[j1][k][l]);
								}
							}
							for (int m = 0; m < numpiezasmax; m++)
							{

								pos_X = pos_X + cplex.getValue(w_mjkl[m][j][k][l]);


								pos_Y = pos_y + cplex.getValue(h_jkl[j][k][l]);

								bool puesta = false;
								for (int i = 0; i < numpieces && puesta == false; i++)
								{
									//buscar defecto que está dentro de este rango
									int d = 0;
									bool pegado_izquierda = false;
									bool pegado_derecha = false;
									bool pegado_abajo = false;
									bool pegado_arriba = true;

									///									for (std::list< GlassDefect > ::iterator it = DefectsPlate[l].begin(); it != DefectsPlate[l].end(); it++, d++)
									//									{
														/*					if (((*it).Getpos_x() > pos_X) || ((*it).Getpos_x() + (*it).Getwidth() < pos_x)
																				|| ((*it).Getpos_y() > pos_Y) || ((*it).Getpos_y() + (*it).Getheight() < pos_y))
																			{
																				continue;
																			}
																			//este está aquí dentro
																			if ((cplex.isExtracted(zlr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlr_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zlrr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlrr_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_derecha = true;
																			}
																			if ((cplex.isExtracted(zll_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zll_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zllr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zllr_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_izquierda = true;
																			}
																			if ((cplex.isExtracted(zla_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zla_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zlar_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlar_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_arriba = true;
																			}
																			if ((cplex.isExtracted(zle_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zle_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zler_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zler_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_abajo = true;
																			}

																			*/
																			//	if ((cplex.isExtracted(zu_dmjkl[d][m][j][k][l]) && cplex.getValue(zu_dmjkl[d][m][j][k][l]) > 0.001)
																			//		&& (cplex.isExtracted(zrp_dmjkl[d][m][j][k][l]) && cplex.getValue(zrp_dmjkl[d][m][j][k][l]) > 0.001) 
																			//		&& (cplex.isExtracted(zr_dmjkl[d][m][j][k][l]) && cplex.getValue(zr_dmjkl[d][m][j][k][l]) < 0.001))
																			//	{
																			//		pegado_arriba = false;
																			//	}

																			//}

									if (cplex.isExtracted(zu_mjkl[m][j][k][l]) && cplex.getValue(zu_mjkl[m][j][k][l]) > 0.001)
										pegado_arriba = false;
									pegado_izquierda = true;

									if (cplex.isExtracted(x_imjkl[i][m][j][k][l]) && cplex.getValue(x_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_w();
										}
										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_h();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_w(), pos_y1 + items[i].Getitem_h(), l + 1);
										//										pos_x += items[i].Getitem_w();
										puesta = true;

									}
									if (cplex.isExtracted(xr_imjkl[i][m][j][k][l]) && cplex.getValue(xr_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_h();
										}

										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_w();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_h(), pos_y1 + items[i].Getitem_w(), l + 1);
										//										pos_x += items[i].Getitem_h();
										puesta = true;
									}
								}
									puesta = false;
									//Ya se ha puesto alguna en la posición
									for (int i = 0; i < numpieces && puesta == false; i++)
									{
										bool pegado_izquierda = false;
										bool pegado_derecha = false;
										bool pegado_abajo = false;
										bool pegado_arriba = true;
										//No hay defecto
										pegado_arriba = true;
										pegado_izquierda = true;

										if (P_set_xa[i] && cplex.isExtracted(xa_imjkl[i][m][j][k][l]) && cplex.getValue(xa_imjkl[i][m][j][k][l]) > 0.001)
										{
											int pos_x1 = pos_x;
											int pos_y1 = pos_y;
											if (pegado_izquierda == true)
											{
												pos_x1 = pos_X - items[i].Getitem_w();
											}
											if (pegado_arriba == true)
											{
												pos_y1 = pos_Y - items[i].Getitem_h();
											}
											fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_w(), pos_y1 + items[i].Getitem_h(), l + 1);
											//										pos_x += items[i].Getitem_w();
											puesta = true;

										}
										if (P_set_xar[i] && cplex.isExtracted(xar_imjkl[i][m][j][k][l]) && cplex.getValue(xar_imjkl[i][m][j][k][l]) > 0.001)
										{
											int pos_x1 = pos_x;
											int pos_y1 = pos_y;
											if (pegado_izquierda == true)
											{
												pos_x1 = pos_X - items[i].Getitem_h();
											}

											if (pegado_arriba == true)
											{
												pos_y1 = pos_Y - items[i].Getitem_w();
											}
											fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_h(), pos_y1 + items[i].Getitem_w(), l + 1);
											//										pos_x += items[i].Getitem_h();
											puesta = true;
										}
									}

								
							}
						}

					}
				}


				fclose(fin3);
				DibujarOpenGL();
			}



		}

		//}


		cplex.end();
		model.end();
		env.end();
	}
	catch (IloException& e)
	{


		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
}


void Glass::FormulacionCompleta4IndicesDefectos(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes)
{
	if (P_maxnivelesh * P_maxnivelesm * P_maxnivelesw > 2500)
	{
		P_maxnivelesw-=4;
		P_maxnivelesm -= 4;
		P_maxnivelesh -= 4;
	}
//	P_maxnivelesh++;
	//Compute the number of pieces
	//P_Set_ar y P_Set_xar
	int trozo = plate_w;
	if (numbins <= 1)
	{
		trozo = Best_Width;
	}
	
	maxTiras = P_maxnivelesw;
	//	int maxTiras=MultiKnapsackEntera(trozo);
	if (P_maxnivelesw > maxTiras)
		P_maxnivelesw = maxTiras;
	for (register int i = 0; i < batch_items; i++)
	{
		P_set_xa.push_back(false);
		P_set_xar.push_back(false);
		P_set_xpa.push_back(false);
		P_set_xrpa.push_back(false);
		std::set<int> set_da;
		std::set<int> set_dar;
		Dxx.push_back(set_da);
		Dyx.push_back(set_dar);
		Dxy.push_back(set_da);
		Dyy.push_back(set_dar);
		Ax.push_back(set_da);
		Ay.push_back(set_da);

		
	}
//	if (!P_initial_solution)
//		numbins++;
	if (P_trimming && Type!=2)
		ComputePiecesEquals();
	int kk = 9;
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	IloRangeArray restrLazy(env);
	int maxtime = G_Time;
	IloNum max_time = maxtime; //tiempo del modelo

	char nombre[20];
	//	Lazy = true;
	Lazy_minimo = false;
	Lazy_cortes = false;
	int Lazy_orden = false;
	//	knapsack_bouknap_tiras();
		//	Creo las variables
	if (P_heur) CalculateMax();
	numtiras = P_maxnivelesw;
	numpilas = P_maxnivelesh;
	int numpiezasmax = P_maxnivelesm;
	if (P_heur)
	{
		numtiras = max((int) numtiras, 3);
		numpilas = max((int)numpilas, 3);
		numpiezasmax = max((int) numpiezasmax, 3);
	}
//	int numpiezasmax = 3;
	int numpieces = batch_items;
	numbins = nbins;
//	if (!P_initial_solution)
//		numbins =numbins*2;
//	numtiras = 5;
//	numpilas = 4;
//	numpiezasmax = 3;
	int Tipo_objetivo = 2;
	if (P_WithoutRotation == true)
	{
		P_minimo_waste = false;
		Tipo_objetivo = 4;
		numbins = 1;
		P_cota = false;

	}
	//max_numero de defectos
	int max_nd = 1;
	for (int i = 0; i < numbins; i++)
	{
		int kk = DefectsPlate[i].size();
		if (kk > max_nd)
		{
			max_nd = kk;
		}
	}

	printf("Aqui21");

	//Variables defectos
	BoolVarMatrix5 zr_dmjkl(env, max_nd);
	BoolVarMatrix5 zrp_dmjkl(env, max_nd);
	BoolVarMatrix5 zu_dmjkl(env, max_nd);
	BoolVarMatrix4 ze_djkl(env, max_nd);

	BoolVarMatrix3 zr_dkl(env, max_nd);
	//minimo defecto pilas


	BoolVarMatrix3 es_jkl(env, numpilas);
	BoolVarMatrix4 el_mjkl(env, numpiezasmax);
	BoolVarMatrix5 es_mijkl(env, numpiezasmax);
	BoolVarMatrix4 ms_mjkl(env, numpiezasmax);
	BoolVarMatrix4 zu_mjkl(env, numpiezasmax);
	BoolVarMatrix3 ml_jkl(env, numpilas);
	BoolVarMatrix2 ms_kl(env, numtiras);

		BoolVarMatrix5 x_imjkl(env, numpieces);
	BoolVarMatrix5 xr_imjkl(env, numpieces);

	//Variables para el trimming
	
	BoolVarMatrix5 xa_imjkl(env, numpieces);
	BoolVarMatrix5 xar_imjkl(env, numpieces);

	BoolVarMatrix5 za_imjkl(env, numpieces);
	BoolVarMatrix5 zar_imjkl(env, numpieces);

	BoolVarMatrix2 esH_kl(env, numtiras);
	BoolVarMatrix2 y_kl(env, numtiras);
	NumVarMatrix4 w_mjkl(env, numpiezasmax);
	BoolVarMatrix4 es_mjkl(env, numpiezasmax);
	NumVarMatrix3 h_jkl(env, numpilas);
	NumVarMatrix2 w_kl(env, numtiras);

	BoolVector esW_l(env);
	BoolVector y_l(env);
	
	NumVector W_K(env);


	IloNumVarArray vars(env);
	IloNumArray vals(env);
	int generals = 0;

	//	int LastBinWidth = floor((Total_area - ((Boolbins - 1)*plate_h*plate_w)) / plate_h);
	//	LastBinWidth = 0;
	int LastBinWidth = max((double)floor((Total_area - ((numbins - 1) * plate_h * plate_w)) / plate_h), (double)0);
	int maxh = 0;

	/**********************************************************/
	/***************** INICIALIZO VARIABLES *******************/
	/**********************************************************/
	//inicializo la variable x_ijkl y xa_imjkl

	for (int i = 0; i < numpieces; i++)
	{
		bool variable_a = true;
		if (items[i].Getitem_h() > maxh)
			maxh = items[i].Getitem_h();
		if (items[i].Getitem_w() > maxh)
			maxh = items[i].Getitem_w();
		x_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		if (P_set_xa[i])
		{
			xa_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		}
		else
			variable_a = false;
		za_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		zar_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		for (int m = 0; m < numpiezasmax; m++)
		{
			x_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			if (variable_a)
				xa_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			za_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			zar_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			for (int j = 0; j < numpilas; j++)
			{
				x_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				if (variable_a)
					xa_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				za_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				zar_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					x_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					if (variable_a)
						xa_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					za_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					zar_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					for (int l = 0; l < numbins; l++)
					{
						int max_pieces = 1;
							 
						x_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "x_%d_%d_%d_%d_%d", i, m, j, k, l);
						x_imjkl[i][m][j][k][l].setName(nombre);
						int maximo_tira = max1Cut;
						if (k > 0)
							maximo_tira = min(max1Cut, max(0, plate_w - G_Acumulado_Min[k - 1]));
						int maximo_seccion = maximo_tira;
						if (m > 0)
							maximo_seccion = min(maximo_tira, max(0, maximo_tira - G_Acumulado_Min[m - 1]));
						int maximo_h = plate_h;
						if (j > 0)
							maximo_h = min(plate_h, max(0, plate_h - G_Acumulado_Min[j - 1]));

/*						if (items[i].Getitem_w() > maximo_seccion || items[i].Getitem_h() > maximo_h)
						{
							IloExpr v1(env);
							v1 += x_imjkl[i][m][j][k][l];
							restr.add(IloRange(env, 0, v1, 0));
							v1.end();
						}*/
						if (variable_a)
						{
							xa_imjkl[i][m][j][k][l] = IloBoolVar(env);
							sprintf(nombre, "xa_%d_%d_%d_%d_%d", i, m, j, k, l);
							xa_imjkl[i][m][j][k][l].setName(nombre);
						}
						za_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "za_%d_%d_%d_%d_%d", i, m, j, k, l);
						za_imjkl[i][m][j][k][l].setName(nombre);
						zar_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "zar_%d_%d_%d_%d_%d", i, m, j, k, l);
						zar_imjkl[i][m][j][k][l].setName(nombre);

					}

				}
			}
		}
	}
	//inicializo variables rotadas
	
	for (int i = 0; i < numpieces; i++)
	{
		bool variable_ar = true;
		xr_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);

		if (P_set_xar[i])
		{
			xar_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		}
		else
			variable_ar = false;
		for (int m = 0; m < numpiezasmax; m++)
		{
			xr_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			if (variable_ar)
				xar_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			for (int j = 0; j < numpilas; j++)
			{
				xr_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				if (variable_ar)
					xar_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					xr_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					if (variable_ar)
						xar_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					for (int l = 0; l < numbins; l++)
					{
						xr_imjkl[i][m][j][k][l] = IloBoolVar(env);

						sprintf(nombre, "xr_%d_%d_%d_%d_%d", i, m, j, k, l);
						xr_imjkl[i][m][j][k][l].setName(nombre);
						int maximo_tira = max1Cut;
						if (k > 0)
							maximo_tira = min(max1Cut, max(0, plate_w - G_Acumulado_Min[k - 1]));
						int maximo_seccion = maximo_tira;
						if (m > 0)
							maximo_seccion = min(maximo_tira,max(0, maximo_tira - G_Acumulado_Min[m - 1]));
						int maximo_h = plate_h;
						if (j > 0)
							maximo_h = min(plate_h, max(0, plate_h - G_Acumulado_Min[j - 1]));

/*						if (items[i].Getitem_h() > maximo_seccion || items[i].Getitem_w() > maximo_h)
						{
							IloExpr v1(env);
							v1 += xr_imjkl[i][m][j][k][l];
							restr.add(IloRange(env, 0, v1, 0));
							v1.end();
						}*/
						if (variable_ar)
						{
							xar_imjkl[i][m][j][k][l] = IloBoolVar(env);
							sprintf(nombre, "xar_%d_%d_%d_%d_%d", i, m, j, k, l);
							xar_imjkl[i][m][j][k][l].setName(nombre);
								//IloExpr v1(env);
								//v1 += xar_imjkl[i][m][j][k][l];
								//restr.add(IloRange(env, 0, v1, 0));
								//v1.end();
						}

					}

				}
			}
		}
	}
	//inicializo las varialbes de cada tira
	for (int m = 0; m < numpiezasmax; m++)
	{
		w_mjkl[m] = NumVarMatrix3(env, numpilas);
		es_mjkl[m] = BoolVarMatrix3(env, numpilas);
		zu_mjkl[m] = BoolVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			w_mjkl[m][j] = NumVarMatrix(env, numtiras);
			es_mjkl[m][j] = BoolVarMatrix(env, numtiras);
			zu_mjkl[m][j] = BoolVarMatrix(env, numtiras);
			for (int k = 0; k < numtiras; k++)
			{
				int maximo_tira = max1Cut;

				if (k > 0)
					maximo_tira = min(max1Cut, max(0, plate_w - G_Acumulado_Min[k - 1]));

				w_mjkl[m][j][k] = IloIntVarArray(env, numbins);
				es_mjkl[m][j][k] = IloBoolVarArray(env, numbins);
				zu_mjkl[m][j][k] = IloBoolVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					if (P_initial_solution && l == (numbins-1))
					{
						maximo_tira = Best_Width;

						if (k > 0)
							maximo_tira = min(max1Cut, max(0, maximo_tira - G_Acumulado_Min[k - 1]));

					}
					int maximo_seccion = maximo_tira;
					if (m > 0)
						maximo_seccion =min(maximo_tira, max(0, maximo_tira - G_Acumulado_Min[m - 1]));

					if (maximo_seccion < G_Acumulado_Min[0])
						maximo_seccion = 0;
					if (maximo_seccion > 0)
						generals++;
					else
						int kk = 9;
					w_mjkl[m][j][k][l] = IloIntVar(env, 0, maximo_seccion);
					sprintf(nombre, "w_%d_%d_%d_%d", m, j, k, l);
					w_mjkl[m][j][k][l].setName(nombre);
					es_mjkl[m][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "ms_%d_%d_%d_%d", m, j, k, l);
					es_mjkl[m][j][k][l].setName(nombre);
					zu_mjkl[m][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "zu_%d_%d_%d_%d", m, j, k, l);
					zu_mjkl[m][j][k][l].setName(nombre);
				}

			}
		}
	}
	//inicializo la variable h_jkl
	for (int i = 0; i < numpilas; i++)
	{
		h_jkl[i] = NumVarMatrix(env, numtiras);

		for (int j = 0; j < numtiras; j++)
		{
			h_jkl[i][j] = IloIntVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				int maximo = plate_h;
				if (i > 0)
					maximo = min(plate_h, max(0, plate_h - G_Acumulado_Min[i - 1]));

				if (maximo < G_Acumulado_Min[0])
					maximo = 0;
				if (maximo > 0)
					generals++;
				h_jkl[i][j][l] = IloIntVar(env, 0, maximo);
				sprintf(nombre, "h_%d_%d_%d", i, j, l);
				h_jkl[i][j][l].setName(nombre);

			}

		}
	}
	//inicializo la variable w_kl y y_kl
	for (int i = 0; i < numtiras; i++)
	{
		w_kl[i] = IloIntVarArray(env, numbins);
		y_kl[i] = IloBoolVarArray(env, numbins);
		ms_kl[i] = IloBoolVarArray(env, numbins);
		esH_kl[i] = IloBoolVarArray(env, numbins);

		for (int j = 0; j < numbins; j++)
		{
			int maximo = max1Cut;
			if (i>0)
			maximo =min(max1Cut, max(0, plate_w - G_Acumulado_Min[i - 1]));
			if (P_initial_solution && j == (numbins-1))
			{
				maximo = Best_Width;

				if (i > 0)
					maximo = min(max1Cut, max(0, maximo - G_Acumulado_Min[i - 1]));

			}
			if (maximo < G_Acumulado_Min[0])
				maximo = 0;
			if (maximo > 0)
				generals++;
			w_kl[i][j] = IloIntVar(env, 0, maximo);
			sprintf(nombre, "W_%d_%d", i, j);
			w_kl[i][j].setName(nombre);
			y_kl[i][j] = IloBoolVar(env);
			sprintf(nombre, "Y_%d_%d", i, j);
			y_kl[i][j].setName(nombre);
			ms_kl[i][j] = IloBoolVar(env);
			sprintf(nombre, "ms_%d_%d", i, j);
			ms_kl[i][j].setName(nombre);
			esH_kl[i][j] = IloBoolVar(env);
			sprintf(nombre, "VesH_%d_%d", i, j);
			esH_kl[i][j].setName(nombre);

		}
	}

	//inicializo las variable y_l
	y_l = IloIntVarArray(env, numbins, 0, 1);
	esW_l = IloIntVarArray(env, numbins, 0, 1);
	for (int l = 0; l < numbins; l++)
	{
		sprintf(nombre, "Y%d", l);
		y_l[l].setName(nombre);
		sprintf(nombre, "VesW%d", l);
		esW_l[l].setName(nombre);
	}
	if (Tipo_objetivo == 4)
	{
		W_K = IloIntVarArray(env, numtiras, 0, 1);
		for (int l = 0; l < numtiras; l++)
		{
			sprintf(nombre, "W%d", l);
			W_K[l].setName(nombre);
		}
	}

	//minimco corte

	for (int i = 0; i < numpiezasmax; i++)
	{
		el_mjkl[i] = BoolVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			el_mjkl[i][j] = BoolVarMatrix(env, numtiras);

			for (int k = 0; k < numtiras; k++)
			{
				el_mjkl[i][j][k] = IloBoolVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					el_mjkl[i][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "Xem_%d_%d_%d_%d", i, j, k, l);
					el_mjkl[i][j][k][l].setName(nombre);

				}

			}
		}
	}

	//inicializo la variable es_jkl
	for (int i = 0; i < numpilas; i++)
	{
		es_jkl[i] = BoolVarMatrix(env, numtiras);
		ml_jkl[i] = BoolVarMatrix(env, numtiras);
		for (int j = 0; j < numtiras; j++)
		{
			es_jkl[i][j] = IloBoolVarArray(env, numbins);
			ml_jkl[i][j] = IloBoolVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				es_jkl[i][j][l] = IloBoolVar(env);
				sprintf(nombre, "Xes_%d_%d_%d", i, j, l);
				es_jkl[i][j][l].setName(nombre);
				ml_jkl[i][j][l] = IloBoolVar(env);
				sprintf(nombre, "ml_%d_%d_%d", i, j, l);
				ml_jkl[i][j][l].setName(nombre);

			}

		}
	}

	//inicializo las variables de los cortes

	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			zr_dkl[i] = BoolVarMatrix(env, numtiras);

			for (int j = 0; j < numtiras; j++)
			{
				zr_dkl[i][j] = IloBoolVarArray(env, numbins);
				for (int kl = 0; kl < numbins; kl++)
				{

					zr_dkl[i][j][kl] = IloBoolVar(env);
					sprintf(nombre, "zr_%d_%d_%d", i, j, kl);
					zr_dkl[i][j][kl].setName(nombre);
/*					if ((kl!=numbins-1) && (kl==l) && ((*itD).Getpos_x() + (*itD).Getwidth()) < G_Acumulado_Min[j ])
					{
						IloExpr v1(env);
						v1+=zr_dkl[i][j][kl];
						restr.add(IloRange(env, 0, v1, 0));
						v1.end();
					}*/
				}
			}
		}
	}
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			zr_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
			zrp_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
			zu_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
			for (int m = 0; m < numpiezasmax; m++)
			{
				zr_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
				zrp_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
				zu_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
				for (int j = 0; j < numpilas; j++)
				{
					zr_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
					zrp_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
					zu_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
					for (int k = 0; k < numtiras; k++)
					{

						zr_dmjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
						zrp_dmjkl[i][m][j][k] = IloBoolVarArray(env, numbins);

						for (int kl = 0; kl < numbins; kl++)
						{
							zr_dmjkl[i][m][j][k][kl] = IloBoolVar(env);
							sprintf(nombre, "zr_%d_%d_%d_%d_%d", i, m, j, k, kl);
							zr_dmjkl[i][m][j][k][kl].setName(nombre);
							zrp_dmjkl[i][m][j][k][kl] = IloBoolVar(env);
							sprintf(nombre, "zrp_%d_%d_%d_%d_%d", i, m, j, k, kl);
							zrp_dmjkl[i][m][j][k][kl].setName(nombre);
						}
					}

				}
			}
		}
	}
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();
		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			ze_djkl[i] = BoolVarMatrix3(env, numpilas);

			for (int j = 0; j < numpilas; j++)
			{
				ze_djkl[i][j] = BoolVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					ze_djkl[i][j][k] = IloBoolVarArray(env, numbins);
					for (int kl = 0; kl < numbins; kl++)
					{
						ze_djkl[i][j][k][kl] = IloBoolVar(env);
						sprintf(nombre, "ze_%d_%d_%d_%d", i, j, k, kl);
						ze_djkl[i][j][k][kl].setName(nombre);
					}
				}

			}
		}
	}

	//inicializo las variables de los defectos


	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/
	//Fijo
	

//	Lazy_minimo = false;
	//Todos los productos colocados
	for (int i = 0; i < numpieces; i++)
	{
		IloExpr v1(env);
		for (int j = 0; j < numpilas; j++)
		{
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						v1 += x_imjkl[i][m][j][k][l];
						v1 += xr_imjkl[i][m][j][k][l];
						if (P_set_xa[i])
							v1 += xa_imjkl[i][m][j][k][l];
						if (P_set_xar[i])
							v1 += xar_imjkl[i][m][j][k][l];
					}
				}
			}
		}
		if (Tipo_objetivo <3)
			restr.add(IloRange(env, 1, v1, 1));
		else
		{
				restr.add(IloRange(env, -IloInfinity, v1, 1));
		}

		v1.end();
	}
	//Restricciones para el ORDEN de los trimming
	//CONSTRAINTS ORDERS TRIMMING

	for (int j = 0; j < numpilas; j++)
	{
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{

					for (int i = 0; i < numpieces; i++)
					{
						if (P_set_xa[i])
						{
							IloExpr v1(env);
							v1 += xa_imjkl[i][m][j][k][l];

							for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
							{
								v1 += (-1) * x_imjkl[(*it)][m][j][k][l];
							}
							for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
							{
								v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];
							}
							restr.add(IloRange(env, -IloInfinity, v1, 0));
							v1.end();

						}
						if (P_set_xar[i])
						{
							IloExpr v1(env);
							v1 += xar_imjkl[i][m][j][k][l];

							for (std::set<int> ::iterator it = Dxy[i].begin(); it != Dxy[i].end(); it++)
							{
								v1 += (-1) * x_imjkl[(*it)][m][j][k][l];
							}
							for (std::set<int> ::iterator it = Dyy[i].begin(); it != Dyy[i].end(); it++)
							{
								v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];
							}
						
							restr.add(IloRange(env, -IloInfinity, v1, 0));
							v1.end();

						}

					}
				}
			}
		}
	}
	//Si coloco la segunda pieza también la primera
	//solamente una por celda

	if (Type != 2)
	{
		for (int j = 0; j < numpilas; j++)
		{
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						//						IloExpr v1(env);
						IloExpr v1(env);
						IloExpr v2(env);
						IloExpr v3(env);
						for (int i = 0; i < numpieces; i++)
						{
							//orden secciones
							if (m > 0)
							{
								v3 += x_imjkl[i][m][j][k][l];
								v3 += xr_imjkl[i][m][j][k][l];
								v3 += (-1) * x_imjkl[i][m - 1][j][k][l];
								v3 += (-1) * xr_imjkl[i][m - 1][j][k][l];
							}
							v1 += x_imjkl[i][m][j][k][l];
							v1 += xr_imjkl[i][m][j][k][l];
							if (P_set_xa[i])
								v2 += xa_imjkl[i][m][j][k][l];
							if (P_set_xar[i])
								v2 += xar_imjkl[i][m][j][k][l];



						}
						//V1 indica que si ponemos la pieza m, antes también tiene que haber piezas
						restr.add(IloRange(env, -IloInfinity, v3, 0));

						restr.add(IloRange(env, -IloInfinity, v1, 1));
						restr.add(IloRange(env, -IloInfinity, v2, 1));
						v1.end();
						v2.end();
						v3.end();



					}
				}
			}
		}
	}
	//	Lazy_minimo=false;
		//Altura mínima altura
//CONSTRAINTS HEIGHT
	for (int j = 0; j < numpilas; j++)
	{

		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{

				for (int m = 0; m < numpiezasmax; m++)
				{
					IloExpr v1(env);
					IloExpr v2(env);

					for (int i = 0; i < numpieces; i++)
					{
						v1 += items[i].Getitem_h() * x_imjkl[i][m][j][k][l];
						v1 += items[i].Getitem_w() * xr_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_h() * x_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_w() * xr_imjkl[i][m][j][k][l];

						if (P_set_xa[i])
							//							for (std::set<int>::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
						{
							v1 += items[i].Getitem_h() * xa_imjkl[i][m][j][k][l];
							v2 += items[i].Getitem_h() * xa_imjkl[i][m][j][k][l];
						}
						if (P_set_xar[i])
							//							for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
						{
							v1 += items[i].Getitem_w() * xar_imjkl[i][m][j][k][l];
							v2 += items[i].Getitem_w() * xar_imjkl[i][m][j][k][l];
						}



					}
					v1 += (-1) * h_jkl[j][k][l];
					if (P_minimo_waste)
						v1 += waste_min * (1 - el_mjkl[m][j][k][l]);

					restr.add(IloRange(env, -IloInfinity, v1, 0));
					v1.end();
					v2 += (-1) * h_jkl[j][k][l];
					if (P_minimo_waste)
					{
						v2 += plate_h * (1 - el_mjkl[m][j][k][l]);
						if (Lazy_minimo)
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						else
							restr.add(IloRange(env, 0, v2, IloInfinity));
					}
					v2.end();

				}
			}
		}
	}


	//CONSTRAINTS MINIMIZAR TIRAS
	if (Tipo_objetivo == 4)
	{
		//Mete una que obligue a que si W_K vale algo es porque tiene algo
		for (int k = 0; k < numtiras; k++)
		{
			IloExpr v1(env);
			v1 += W_K[k];
			for (int i = 0; i < numpieces; i++)
			{
				for (int j = 0; j < numpilas; j++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						v1 += (-1) * x_imjkl[i][m][j][k][0];
						v1 += (-1) * xr_imjkl[i][m][j][k][0];
					}
				}
			}
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
		}

	}

	// CONSTRAINT TRIMMING
	//Si pongo un triming la altura tiene que ser perfecta
	if (P_trimming && Type!=2)
	{
		for (int j = 0; j < numpilas; j++)
		{

			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{

					for (int m = 0; m < numpiezasmax; m++)
					{

						IloExpr v1(env);

						for (int i = 0; i < numpieces; i++)
						{
							if (P_set_xa[i])
								//							for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
							{
								v1 += 2*xa_imjkl[i][m][j][k][l];
//								v2 += 2* xa_imjkl[i][m][j][k][l];
							}
							if (P_set_xar[i])
								//							for (std::set<int>::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
							{
								v1 +=2* xar_imjkl[i][m][j][k][l];
//								v2 += 2* xar_imjkl[i][m][j][k][l];
							}





						}
						v1 += ((-1)* zu_mjkl[m][j][k][l]);

						v1 += (-1) * el_mjkl[m][j][k][l];
						restr.add(IloRange(env, -IloInfinity, v1, 0));
						v1.end();
//						restr.add(IloRange(env, -IloInfinity, v2, 1));
	//					v2.end();
					}
				}
			}
		}
	}
	//CONSTRAINT WIDTH

//	Lazy_minimo = false;
	//Anchura de los cortes en cada pila
	//minimo corte consecutivo en los de tres

	for (int j = 0; j < numpilas; j++)
	{

		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					IloExpr v1(env);
					IloExpr v2(env);

					for (int i = 0; i < numpieces; i++)
					{


						v1 += items[i].Getitem_w() * x_imjkl[i][m][j][k][l];
						v1 += items[i].Getitem_h() * xr_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_w() * x_imjkl[i][m][j][k][l];
						v2 += items[i].Getitem_h() * xr_imjkl[i][m][j][k][l];

					}
					v1 += (-1) * w_mjkl[m][j][k][l];
					if (P_minimo_waste)
						v1 += (1 - es_mjkl[m][j][k][l]) * waste_min;
					v2 += (-1) * w_mjkl[m][j][k][l];
					v2 += (1 - es_mjkl[m][j][k][l]) * max1Cut;

					restr.add(IloRange(env, -IloInfinity, v1, 0));
					v1.end();
					if (P_minimo_waste)
					{
						if (Lazy_minimo)
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						else
							restr.add(IloRange(env, 0, v2, IloInfinity));
					}
					v2.end();




				}
			}
		}
	}





	//CONSTRAINT WIDHT
	/**/
	//suma de minimo corte anchos de las tiras
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			for (int j = 0; j < numpilas; j++)
			{

				IloExpr v1(env);
				IloExpr v2(env);
				for (int m = 0; m < numpiezasmax; m++)
				{
					v1 += w_mjkl[m][j][k][l];
					v2 += w_mjkl[m][j][k][l];

				}

				v1 += (-1) * w_kl[k][l];
				if (P_minimo_waste)
					v1 += (1 - es_jkl[j][k][l]) * waste_min;

				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();

				v2 += (-1) * w_kl[k][l];
				v2 += (1 - es_jkl[j][k][l]) * max1Cut;
				if (P_minimo_waste)
				{
					if (Lazy_minimo)
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));

					else
						restr.add(IloRange(env, 0, v2, IloInfinity));
				}
				v2.end();
			}

		}
	}

	//minimos cortes
		//suma de las alturas de las pilas menor H
		//CONSTRAINT HEIGHT
		//CONSTRAINT HEIGHT
	//suma de las alturas de las pilas menor H


	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			IloExpr v1(env);
			IloExpr v2(env);
			IloExpr v3(env);
			for (int j = 0; j < numpilas; j++)
			{


				v1 += h_jkl[j][k][l];
				v2 += h_jkl[j][k][l];
				v3 += h_jkl[j][k][l];
			}
			v1 += (-1) * plate_h * ms_kl[k][l];
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
			v2 += (-1) * plate_h;
			v2+=(1-	esH_kl[k][l])* waste_min;
			if (P_minimo_waste)
			restr.add(IloRange(env, -IloInfinity, v2, 0));
			v2.end();
			v3 += (-1) * plate_h;
			v3 += (1 - esH_kl[k][l]) * plate_h;
			if (P_minimo_waste)
			restr.add(IloRange(env, 0, v3, IloInfinity));
			v3.end();
		}
	}
	//suma de las alturas de las pilas menor H
//	Lazy_minimo = true;

	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{

			for (int j = 0; j < numpilas; j++)
			{
				//				Lazy_minimo = true;
				IloExpr v1(env);
				v1 += min2Cut * ml_jkl[j][k][l];

				v1 += (-1) * h_jkl[j][k][l];

				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
				IloExpr v2(env);
				v2 += plate_h * ml_jkl[j][k][l];
				v2 += (-1) * h_jkl[j][k][l];
				if (Lazy_minimo)
					restrLazy.add(IloRange(env, 0, v2, IloInfinity));
				else
					restr.add(IloRange(env, 0, v2, IloInfinity));
				v2.end();
				//no crea pilas vacías y además ordenadas

					IloExpr v3(env);
					v3 += ml_jkl[j][k][l];
					for (int i = 0; i < numpieces; i++)
					{
						for (int m = 0; m < numpiezasmax; m++)
						{
							v3 += (-1) * x_imjkl[i][m][j][k][l];
							v3 += (-1) * xr_imjkl[i][m][j][k][l];
						}
					}
					restr.add(IloRange(env, -IloInfinity, v3, 0));
					v3.end();

					if (j > 0)
					{
						IloExpr v4(env);
						v4 += ml_jkl[j][k][l];
						v4 += (-1) * ml_jkl[j - 1][k][l];
						restr.add(IloRange(env, -IloInfinity, v4, 0));
						v4.end();
					}




			}
			//			Lazy_minimo = false;
			IloExpr v1(env);
			v1 += min1Cut * ms_kl[k][l];

			v1 += (-1) * w_kl[k][l];

			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
			IloExpr v2(env);
			v2 += max1Cut * ms_kl[k][l];
			v2 += (-1) * w_kl[k][l];
			if (Lazy_minimo)
				restrLazy.add(IloRange(env, 0, v2, IloInfinity));
			else
				restr.add(IloRange(env, 0, v2, IloInfinity));
			v2.end();

			//no crea tiras vacías y además ordenadas
				IloExpr v3(env);
				v3 += ms_kl[k][l];
				for (int i = 0; i < numpieces; i++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						for (int j = 0; j < numpilas; j++)
						{
							v3 += (-1) * x_imjkl[i][m][j][k][l];
							v3 += (-1) * xr_imjkl[i][m][j][k][l];
						}
					}
				}
				restr.add(IloRange(env, -IloInfinity, v3, 0));
				v3.end();
				if (k > 0)
				{
					IloExpr v4(env);
					v4 += ms_kl[k][l];
					v4 += (-1) * ms_kl[k - 1][l];
					restr.add(IloRange(env, -IloInfinity, v4, 0));
					v4.end();
				}

		}
	}
	//CONSTRAINT WIDTH
	//Suma  tiras menor o igual a W
	for (int l = 0; l < numbins; l++)
	{
		IloExpr v1(env);
		IloExpr v2(env);
		IloExpr v3(env);
		for (int k = 0; k < numtiras; k++)
		{

			v1 += w_kl[k][l];
			v2 += w_kl[k][l];
			v3 += w_kl[k][l];

		}

		v1 += (-1) * plate_w * y_l[l];
		restr.add(IloRange(env, -IloInfinity, v1, 0));
		v1.end();
		
		v2 += (-1) * plate_w;
		v2 += (1 - esW_l[l]) * waste_min;
		if (P_minimo_waste)
		restr.add(IloRange(env, -IloInfinity, v2, 0));
		v2.end();
		v3 += (-1) * plate_w;
		v3 += (1 - esW_l[l]) * plate_w;
		if (P_minimo_waste)
		restr.add(IloRange(env, 0, v3, IloInfinity));
		v3.end();
	}
	//Variable resto
	IloIntVar Rm(env, 0, plate_w);
	Rm.setName("Rm");
	if (P_NewObjective_Rm)
	{

		//orders bins
		for (int l = 0; l < numbins - 1; l++)
		{
			IloExpr v1(env);
			v1 += y_l[l];
			v1 += (-1) * y_l[l + 1];
			restr.add(IloRange(env, 0, v1, IloInfinity));
			v1.end();
		}
		//Variables relacionadas con la Rm
		for (int l = 0; l < numbins ; l++)
		{
			IloExpr v1(env);
			v1 +=(-1)* Rm;
			if (l<(numbins-1))
			v1 += (-1) * plate_w * y_l[l + 1];
			for (int k = 0; k < numtiras; k++)
			{
				v1 += w_kl[k][l];
			}
			restr.add(IloRange(env, -IloInfinity, v1,0 ));
			v1.end();
		}
		IloExpr v3(env);
		for (int i = 0; i < numbins; i++)
		{
			v3 += plate_w * y_l[i];
		}
		v3 += (-1) * plate_w;
		v3 += Rm;
		restr.add(IloRange(env, floor(Total_area / plate_h), v3, IloInfinity));
		v3.end();

	}
	//CONSTRAINT ORDER STACKS
	//Orden de los stacks por parejas
	//******************

	//
	if (P_orders)
	{
		for (int l = 0; l < numbins; l++)
		{

			for (int k = 0; k < numtiras; k++)
			{

				for (int j = 0; j < numpilas; j++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						for (int i = numpieces - 1; i > 0; i--)
						{


							if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1 && (items[i].Getitem_seq() > 1))
							{
								int kk = stacks[items[i].Getitem_stack()].Getitem_nbr();
								IloExpr v1(env);
								v1 += x_imjkl[i][m][j][k][l];
								v1 += xr_imjkl[i][m][j][k][l];
								if (P_set_xa[i])
									v1 += xa_imjkl[i][m][j][k][l];
								if (P_set_xar[i])
									v1 += xar_imjkl[i][m][j][k][l];
								// ponemos las variables
								int i2 = i - 1;
								if (i2 >= 0 && items[i].Getitem_stack() == items[i2].Getitem_stack())
								{

									for (int l2 = 0; l2 <= l; l2++)
									{
										for (int k2 = 0; k2 < numtiras; k2++)
										{
											if (l2 == l && k2 > k)
												break;
											for (int j2 = 0; j2 < numpilas; j2++)
											{
												if (l2 == l && k2 == k && j2 > j)
													break;
												for (int m2 = 0; m2 < numpiezasmax; m2++)
												{
													if (l2 == l && k2 == k && j2 == j && m2 >= m)
														break;

													v1 += (-1) * x_imjkl[i2][m2][j2][k2][l2];
													v1 += (-1) * xr_imjkl[i2][m2][j2][k2][l2];
													if (P_set_xa[i2])
														v1 += (-1) * xa_imjkl[i2][m2][j2][k2][l2];
													if (P_set_xar[i2])
														v1 += (-1) * xar_imjkl[i2][m2][j2][k2][l2];

												}
											}

										}
									}

								}
								for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
									{
										v1 += (-1) * x_imjkl[(*it)][m][j][k][l];

									}

								}
								for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
									{
										v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];

									}
								}
								for (std::set<int> ::iterator it = Dxy[i].begin(); it != Dxy[i].end() ; it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
										v1 += (-1) * x_imjkl[(*it)][m][j][k][l];
								}
								for (std::set<int> ::iterator it = Dyy[i].begin(); it != Dyy[i].end() ; it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
										v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];
								}
								if (Lazy_orden)
									restrLazy.add(IloRange(env, -IloInfinity, v1, 0));

								else
									restr.add(IloRange(env, -IloInfinity, v1, 0));
								v1.end();




							}



						}
					}
				}
			}
		}
	}
	//CONSTRAINTS CUT VERTICAL
	//restricciones de los cortes verticales
	if (P_defects)
	{
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{

				for (int k1 = 0; k1 < numtiras; k1++)
				{
					IloExpr v1(env);
					IloExpr v2(env);
					for (int k2 = 0; k2 <= k1; k2++)
					{
						v1 += w_kl[k2][l];
						v2 += w_kl[k2][l];
					}
					v1 += (-1) * ((*itD).Getpos_x());

					v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1][l]);
					//			v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth());
					v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zr_dkl[i][k1][l]);


					if (Lazy_cortes)
					{
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
					}
					else
					{
						restr.add(IloRange(env, 0, v2, IloInfinity));
						restr.add(IloRange(env, -IloInfinity, v1, 0));
					}
					v1.end();
					v2.end();
					if (k1 > 0)
					{
						IloExpr v3(env);
						v3 += zr_dkl[i][k1][l];
						v3 += (-1) * zr_dkl[i][k1 - 1][l];
						restr.add(IloRange(env, -IloInfinity, v3, 0));
						v3.end();
					}
					
					//consistencias r
					for (int j1 = 0; j1 < numpilas; j1++)
					{
						for (int m = 0; m < numpiezasmax; m++)
						{
							if (k1 > 0)
							{
								IloExpr v3(env);
								v3 += zr_dmjkl[i][m][j1][k1 ][l];
								v3 += (-1) * zr_dkl[i][k1-1][l];
								restr.add(IloRange(env, -IloInfinity, v3, 0));
								v3.end();
								IloExpr v4(env);
								v4 += zrp_dmjkl[i][m][j1][k1 ][l];
								v4 += (-1) * zr_dkl[i][k1-1][l];
								restr.add(IloRange(env, -IloInfinity, v4, 0));
								v4.end();
							}
						}
					}
				}



			}
		}
		//CONSTRAINTS CUT HORIZONTAL
		//restricciones de los cortes horizontales
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{
				for (int k = 0; k < numtiras; k++)
				{

					for (int j1 = 0; j1 < numpilas; j1++)
					{
						IloExpr v1(env);
						IloExpr v2(env);
						for (int j2 = 0; j2 <= j1; j2++)
						{

							v1 += h_jkl[j2][k][l];
							v2 += h_jkl[j2][k][l];
						}



						v1 += (-1) * ((*itD).Getpos_y());
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - ze_djkl[i][j1][k][l]);
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (zr_dkl[i][k][l]);
						if (k > 0)
						{
							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zr_dkl[i][k - 1][l]);
						}

						//ahora falta la otra parte
						int rtyd = (*itD).Getpos_y() + (*itD).Getheight();
						v2 += (-1) * (rtyd) * (1 - ze_djkl[i][j1][k][l]);
						v2 += (rtyd) * (zr_dkl[i][k][l]);
						if (k > 0)
						{
							v2 += rtyd * (1 - zr_dkl[i][k - 1][l]);
						}

						if (Lazy_cortes)
						{
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
							restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
						}
						else
						{
							restr.add(IloRange(env, 0, v2, IloInfinity));
							restr.add(IloRange(env, -IloInfinity, v1, 0));
						}
						v1.end();
						v2.end();
						if (j1 > 0)
						{
							IloExpr v3(env);
							v3 += ze_djkl[i][j1][k][l];
							v3 += (-1) * ze_djkl[i][j1 - 1][k][l];
							restr.add(IloRange(env, -IloInfinity, v3, 0));
							v3.end();


						}
						//Quitar y pongo las otras
						IloExpr v4(env);
						v4 += ze_djkl[i][j1][k][l];
						if (k > 0)
							v4 += (-1) * (zr_dkl[i][k - 1][l] - zr_dkl[i][k][l]);
						else
							v4 += (-1) * (1 - zr_dkl[i][k][l]);
						restr.add(IloRange(env, -IloInfinity, v4, 0));
						v4.end();
		//				

					}
					//orden de las variables
				}






			}
		}
		//CONSTRAINTS HORIZONTAL 2ORDER

		//restricciones de los cortes verticales de segundo orden de defecto

		//Deerecha
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{

				for (int k1 = 0; k1 < numtiras; k1++)
				{
					for (int j = 0; j < numpilas; j++)
					{
						for (int m = 0; m < numpiezasmax; m++)
						{
							IloExpr v1(env);
							IloExpr v2(env);
							if (k1 > 0)
							{
								for (int k2 = 0; k2 < k1; k2++)
								{

									v1 += w_kl[k2][l];
									v2 += w_kl[k2][l];
								}
							}
							for (int m2 = 0; m2 <= m; m2++)
							{

								v1 += w_mjkl[m2][j][k1][l];
								v2 += w_mjkl[m2][j][k1][l];
							}
							v1 += (-1) * ((*itD).Getpos_x());
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dmjkl[i][m][j][k1][l]);
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * ze_djkl[i][j][k1][l];
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (zr_dkl[i][k1][l]);
							if (k1 > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1 - 1][l]);
							if (j > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - ze_djkl[i][j - 1][k1][l]);
							int rtxd = (*itD).Getpos_x() + (*itD).Getwidth();
							v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zr_dmjkl[i][m][j][k1][l]);
							v2 += (rtxd)*ze_djkl[i][j][k1][l];
							v2 += (rtxd) * (zr_dkl[i][k1][l]);
							if (k1 > 0)
								v2 += (rtxd) * (1 - zr_dkl[i][k1 - 1][l]);

							if (j > 0)
								v2 += (rtxd) * (1 - ze_djkl[i][j - 1][k1][l]);
							if (Lazy_cortes)
							{
								restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
								restrLazy.add(IloRange(env, 0, v2, IloInfinity));
							}
							else
							{
								restr.add(IloRange(env, -IloInfinity, v1, 0));
								restr.add(IloRange(env, 0, v2, IloInfinity));
							}
							v1.end();
							v2.end();
							if (m > 0)
							{
								IloExpr v3(env);
								v3 += zr_dmjkl[i][m][j][k1][l];
								v3 += (-1) * zr_dmjkl[i][m - 1][j][k1][l];
								restr.add(IloRange(env, -IloInfinity, v3, 0));
								v3.end();
							}
							//if (m ==0)
							//{
							//	IloExpr v3(env);
							//	v3 += zr_dkl[i][k1][l];
							//	v3 += (-1) * zr_dmjkl[i][numpiezasmax-1][j][k1][l];
							//	restr.add(IloRange(env, -IloInfinity, v3, 0));
							//	v3.end();
							//}

							IloExpr v4(env);
							v4 += zr_dmjkl[i][m][j][k1][l];
							if (j > 0)
								v4 += (-1) * (ze_djkl[i][j - 1][k1][l] - ze_djkl[i][j][k1][l]);
							else
								v4 += (-1) * (1 - ze_djkl[i][j][k1][l]);
							restr.add(IloRange(env, -IloInfinity, v4, 0));
							v4.end();


						}
					}
				}



			}
		}

		//CONSTRAINTS HORIZONTAL 2ORDER LEFT
		//cortes
		//restricciones de los cortes verticales de segundo orden de defecto
		//Izquierda
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{

				for (int k1 = 0; k1 < numtiras; k1++)
				{
					for (int j = 0; j < numpilas; j++)
					{
						for (int m = 0; m < numpiezasmax; m++)
						{
							IloExpr v1(env);
							IloExpr v2(env);
							if (k1 > 0)
							{
								for (int k2 = 0; k2 < k1; k2++)
								{

									v1 += w_kl[k2][l];
									v2 += w_kl[k2][l];
								}
							}
							for (int m2 = 0; m2 <= m; m2++)
							{

								v1 += w_mjkl[m2][j][k1][l];
								v2 += w_mjkl[m2][j][k1][l];
							}
							for (int i1 = 0; i1 < numpieces; i1++)
							{
								v1 += (-1) * items[i1].Getitem_w() * x_imjkl[i1][m][j][k1][l];
								v1 += (-1) * items[i1].Getitem_h() * xr_imjkl[i1][m][j][k1][l];
								v2 += (-1) * items[i1].Getitem_w() * x_imjkl[i1][m][j][k1][l];
								v2 += (-1) * items[i1].Getitem_h() * xr_imjkl[i1][m][j][k1][l];
							}


							v1 += (-1) * ((*itD).Getpos_x());
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zrp_dmjkl[i][m][j][k1][l]);
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * ze_djkl[i][j][k1][l];
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * zr_dkl[i][k1][l];
							if (k1 > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1 - 1][l]);

							if (j > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - ze_djkl[i][j - 1][k1][l]);
							int rtxd = (*itD).Getpos_x() + (*itD).Getwidth();
							v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zrp_dmjkl[i][m][j][k1][l]);
							v2 += (rtxd)*ze_djkl[i][j][k1][l];
							v2 += (rtxd)*zr_dkl[i][k1][l];
							if (k1 > 0)
								v2 += (rtxd) * (1 - zr_dkl[i][k1 - 1][l]);

							if (j > 0)
								v2 += (rtxd) * (1 - ze_djkl[i][j - 1][k1][l]);
							if (Lazy_cortes)
							{
								restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
								restrLazy.add(IloRange(env, 0, v2, IloInfinity));
							}
							else
							{
								restr.add(IloRange(env, -IloInfinity, v1, 0));
								restr.add(IloRange(env, 0, v2, IloInfinity));
							}
							v1.end();
							v2.end();
							if (m > 0)
							{
								IloExpr v3(env);
								v3 += zrp_dmjkl[i][m][j][k1][l];
								v3 += (-1) * zrp_dmjkl[i][m - 1][j][k1][l];
								restr.add(IloRange(env, -IloInfinity, v3, 0));
								v3.end();
							}
							//if (m ==0)
							//{
							//	IloExpr v3(env);
							//	v3 += zr_dkl[i][k1][l];
							//	v3 += (-1) * zr_dmjkl[i][numpiezasmax-1][j][k1][l];
							//	restr.add(IloRange(env, -IloInfinity, v3, 0));
							//	v3.end();
							//}

							//31/03/2020
							IloExpr v4(env);
							v4 += zrp_dmjkl[i][m][j][k1][l];
							if (j > 0)
								v4 += (-1) * (ze_djkl[i][j - 1][k1][l] - ze_djkl[i][j][k1][l]);
							else
								v4 += (-1) * (1 - ze_djkl[i][j][k1][l]);
							restr.add(IloRange(env, -IloInfinity, v4, 0));
							v4.end();
							//IloExpr v4(env);
							//v4 += zrp_dmjkl[i][m][j][k1][l];
							//if (j > 0)
							//	v4 += (-1) * (ze_djkl[i][j - 1][k1][l] - ze_djkl[i][j][k1][l]);
							//else
							//	v4 += (-1) * (1 - ze_djkl[i][j][k1][l]);
							//restr.add(IloRange(env, -IloInfinity, v4, 0));
							//v4.end();

						}
					}
				}



			}
		}
		//
		//restricciones de los cortes horizontales de la pieza
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{
				for (int k = 0; k < numtiras; k++)
				{

					for (int j1 = 0; j1 < numpilas; j1++)
					{

						for (int m1 = 0; m1 < numpiezasmax; m1++)
						{
							IloExpr v1(env);
							IloExpr v2(env);
							for (int j2 = 0; j2 <= j1; j2++)
							{
								if (j2 < j1)
									v1 += h_jkl[j2][k][l];
								v2 += h_jkl[j2][k][l];
							}
							for (int i1 = 0; i1 < numpieces; i1++)
							{
								v1 += items[i1].Getitem_h() * (x_imjkl[i1][m1][j1][k][l]);
								v1 += items[i1].Getitem_w() * (xr_imjkl[i1][m1][j1][k][l]);
								v2 += (-1) * items[i1].Getitem_h() * x_imjkl[i1][m1][j1][k][l];
								v2 += (-1) * items[i1].Getitem_w() * xr_imjkl[i1][m1][j1][k][l];
							}


							v1 += (-1) * ((*itD).Getpos_y());
							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zu_mjkl[m1][j1][k][l]);

							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zrp_dmjkl[i][m1][j1][k][l]);
							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (zr_dmjkl[i][m1][j1][k][l]);

							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (ze_djkl[i][j1][k][l]);
							if (j1 > 0)
								v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - ze_djkl[i][j1 - 1][k][l]);
							//ahora falta la otra parte
							int rtyd = (*itD).Getpos_y() + (*itD).Getheight();
							v2 += (-1) * (rtyd) * (1 - zu_mjkl[m1][j1][k][l]);
							v2 += ((rtyd) * (1 - zrp_dmjkl[i][m1][j1][k][l]));
							v2 += ((rtyd)) * (zr_dmjkl[i][m1][j1][k][l]);


							v2 += ((rtyd)) * (ze_djkl[i][j1][k][l]);
							if (j1 > 0)
								v2 += (rtyd) * (1 - ze_djkl[i][j1 - 1][k][l]);
							restr.add(IloRange(env, -IloInfinity, v1, 0));
							if (Lazy_cortes)
							{
								restrLazy.add(IloRange(env, 0, v2, IloInfinity));

							}
							else
							{
								restr.add(IloRange(env, 0, v2, IloInfinity));

							}
							v1.end();
							v2.end();
							//1/04/2020

							IloExpr v3(env);
							v3 += el_mjkl[m1][j1][k][l];
							v3 += (-1);
							v3 +=  zrp_dmjkl[i][m1][j1][k][l];
							v3 += (-1) * zr_dmjkl[i][m1][j1][k][l];
							restr.add(IloRange(env, -IloInfinity, v3, 0));
							v3.end();
														//if (j1 > 0)
							//{
							//	IloExpr v3(env);
							//	v3 += ze_djkl[i][j1][k][l];
							//	v3 += (-1) * ze_djkl[i][j1 - 1][k][l];
							//	restr.add(IloRange(env, -IloInfinity, v3, 0));
							//	v3.end();


							//}

							//if (j1 > 0)
							//{
							//	IloExpr v3(env);
							//	v3 += ze_djkl[i][j1][k][l];
							//	v3 += (-1) * ze_djkl[i][j1 - 1][k][l];
							//	restr.add(IloRange(env, -IloInfinity, v3, 0));
							//	v3.end();


							//}
							//IloExpr v4(env);
							//v4 +=2* zu_mjkl[m1][j1][k][l];

							//v4 += (-1) * zrp_dmjkl[i][m1][j1][k][l];
							//v4 += (-1) * (1-zr_dmjkl[i][m1][j1][k][l]);

							//restr.add(IloRange(env, -IloInfinity, v4, 0));
							//v4.end();


						}


					}
					//orden de las variables
				}






			}
		}
	}
	if (P_cota)
	{
		if ((Tipo_objetivo == 2 && ((P_TrozoArea < 6000) && (numbins == P_BinsCotaArea + 1))))
		{
			IloExpr v4(env);
			for (int k = 0; k < numtiras; k++)
			{


				v4 += w_kl[k][numbins - 1];
			}
			v4 += (-1) * P_TrozoArea;
			restr.add(IloRange(env, 0, v4, IloInfinity));
			v4.end();
		}

		if (Type==2)
		{
			IloExpr v4(env);
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					for (int j = 0; j < numpilas; j++)
					{

						for (int m = 0; m < numpiezasmax; m++)
						{

							for (int i = 0; i < numpieces; i++)
							{
								v4 += items[i].Getitem_value() * x_imjkl[i][m][j][k][l];
							}
						}
					}
				}
			}
			v4 += (-1) * 181;
			restr.add(IloRange(env, -IloInfinity, v4, 0));
			v4.end();
		}
	}

	model.add(restr);
	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
//	model.add(restr);


	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);
	if (Tipo_objetivo == 5)
	{
		//		IloExpr Expression_fobj(env);
		for (int i = 0; i < numtiras; i++)
		{
			Expression_fobj += W_K[i];
		}


	}
	if (Tipo_objetivo == 0)//biobjective //estaría en diet.ccp
	{
		IloExpr Expression_fobj1(env);
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj1 += y_l[i];
		}
		IloExpr Expression_fobj2(env);
		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj2 += w_kl[k][numbins - 1];
		}
	}

	if (Tipo_objetivo == 1)//por bins
	{
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj += y_l[i];
		}
	}
	if (Tipo_objetivo == 2)//por tira del último bin
	{
		if (P_NewObjective_Rm)
		{
			for (int i = 0; i < numbins; i++)
			{
				Expression_fobj +=plate_w* y_l[i];
			}
			Expression_fobj +=(-1)* plate_w;
			Expression_fobj += Rm;

		}
		else
		{
			for (int k = 0; k < numtiras; k++)
			{
				Expression_fobj += w_kl[k][numbins - 1];
			}
		}
	}
	if (Tipo_objetivo == 3)//por área de lo metido en el bin
	{
		Expression_fobj += plate_h * plate_w;
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int j = 0; j < numpilas; j++)
				{

					for (int m = 0; m < numpiezasmax; m++)
					{

						for (int i = 0; i < numpieces; i++)
						{
							Expression_fobj += (-1) * items[i].Getitem_area() * x_imjkl[i][m][j][k][l];
							Expression_fobj += (-1) * items[i].Getitem_area() * xr_imjkl[i][m][j][k][l];
						}
					}
				}
			}
		}
	}
	if (Tipo_objetivo == 4)//por área de lo metido en el bin
	{
//		Expression_fobj += plate_h * plate_w;
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int j = 0; j < numpilas; j++)
				{

					for (int m = 0; m < numpiezasmax; m++)
					{

						for (int i = 0; i < numpieces; i++)
						{
							Expression_fobj += items[i].Getitem_value() * x_imjkl[i][m][j][k][l];
							Expression_fobj +=  items[i].Getitem_value() * xr_imjkl[i][m][j][k][l];
						}
					}
				}
			}
		}
	}
	if (Tipo_objetivo != 0 && Tipo_objetivo <4)
		model.add(IloMinimize(env, Expression_fobj));
	if (Tipo_objetivo >= 4)
		model.add(IloMaximize(env, Expression_fobj));

	Expression_fobj.end();


	/***************************************************************/
	/*************** EJECUTAMOS EL MODELO **************************/
	/***************************************************************/
	try
	{
		printf("Aqui211");
		IloCplex cplex(model);
		printf("Versión %s", cplex.getVersion());

		//Si queremos las lazy y tenemos alguna
		if (Lazy_minimo)
		{
			cplex.addLazyConstraints(restrLazy);
		}
//		printf("Aqui2112");
		if (G_Dibujar)
				cplex.exportModel("Roadef.lp");
				//No hace el probing
//		cplex.setParam(IloCplex::MIPEmphasis, 3);
	//			cplex.setParam(IloCplex::Probe, -1);
				//1 CORE
		cplex.setParam(IloCplex::Threads, nt);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, max_time); //TIEMPO EN SEGUNDOS

//		if (Tipo_objetivo == 2)
//			cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, LastBinWidth);
		if (P_NewObjective_Rm)
		cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, floor(Total_area / plate_h));
		if (!P_initial_solution)
		cplex.setParam(IloCplex::MIPEmphasis, 1);
		
		//
//		cplex.setParam(IloCplex::Param::Preprocessing::Presolve, 0);
//		cplex.setParam(IloCplex::LongParam::HeurFreq, -1);
//		cplex.setParam(IloCplex::Param::MIP::Display, 4);
//		cplex.setParam(IloCplex::Param::RootAlgorithm, 4);

		if (G_Dibujar)
		cplex.exportModel("Roadef.lp");

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

		//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();

		if (P_initial_solution)
		{


			FILE* file2;
			//	strcat(name_instance, "_solution.csv");

			file2 = fopen(P_file_to_write.c_str(), "r+");
			if (file2 == NULL)
			{
				printf("Problemas al leer el archivo %s, no se encuentra el archivo", P_file_to_write.c_str());
				exit(4);
			}
			int PLATE_ID = 0;
			int	NODE_ID = 0;
			int	X = 0;
			int Y = 0;
			int	WIDTH = 0;
			int	HEIGHT = 0;
			int TYPE = 0;
			int CUT = 0;
			int PARENT = 0;
			char kk[256];
			fscanf(file2, "%s\n",kk);
			fscanf(file2, "%d;%d;%d;%d;%d;%d;%d;%d;", &PLATE_ID, &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT);
			int section = -1;
			int level = -1;
			int stripe = -1;
			int plate = 0;
//			printf("Solución inicial\n");
			while (!feof(file2) && CUT != (-3))
			{

				fscanf(file2, "%d;", &PLATE_ID);
				if (PLATE_ID == plate)
					fscanf(file2, "%d;%d;%d;%d;%d;%d;%d;%d", &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT, &PARENT);
				else
				{
					plate = PLATE_ID;
					stripe = -1;
					fscanf(file2, "%s\n", kk);
					continue;
				}
				plate = PLATE_ID;
				if (CUT == 2 && TYPE == (-1)) continue;
				if (CUT == 1 && TYPE == (-1)) continue;
				if (CUT == 4 && TYPE<0) continue;
				if (CUT == 1)
				{
					stripe++;
					level = -1;
					section = 0;
					if (TYPE<0)
					continue;
				}
				if (CUT == 2)
				{
					level++; //change 4/3/2020
					section = 0;
					if (TYPE < 0)
					continue;
					//level++;//change 4/3/2020
				}
				if (CUT == 3 )
				{
					
					if (TYPE < 0)
					{
						if (TYPE==(-10))//cambiado por 1
						section++;
						continue;
					}
				//	else
				//	{
				//		if (level < 0) level = 0;//change 4/3/2020
				//	}
						
				}

				if (items[TYPE].Getitem_h() == HEIGHT)
				{
					//VAriable 
					vars.add(x_imjkl[TYPE][section][level][stripe][plate]);
					printf("%s %d w %d h %d s %d section %d level %d stripe %d plate %d\n", x_imjkl[TYPE][section][level][stripe][plate].getName(), 1, items[TYPE].Getitem_w(), items[TYPE].Getitem_h(), items[TYPE].Getitem_stack(), section,level,stripe,plate);
					vals.add(1.0);
				}
				else
				{
					//VAriable 
					vars.add(xr_imjkl[TYPE][section][level][stripe][plate]);
					printf("%s %d w %d h %d s %d section %d level %d stripe %d plate %d\n", xr_imjkl[TYPE][section][level][stripe][plate].getName(), 1, items[TYPE].Getitem_w(), items[TYPE].Getitem_h(), items[TYPE].Getitem_stack(),section, level, stripe, plate);
					vals.add(1.0);
				}
				if (CUT == 3 || CUT==4)
				{
					section++;
				}
				//		fscanf(fin2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &W, &H, &plates, &piezas, &defectos, &plate_defectos, &obj, &width_total);
			}
			fclose(file2);

			WriteSolution();
			DibujarOpenGL();
			cplex.addMIPStart(vars, vals);

		}
		IloBool result = cplex.solve();
//		printf("Aqui2113");
		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
//		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;

		//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


			//			FILE* f = fopen("./estadomodelo.txt", "a+");
						//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
				//				fprintf(f, "Infeasible\n");
//				printf("Infeasible\n");
				//				cplex.exportModel("Roadef.lp");
			}
			else
			{
				if (cplex.getStatus() == IloAlgorithm::Unbounded)
					//					fprintf(f, "Unbounded\n");
					printf("Unbounded\n");
				LB_Best_Value_Formulation = cplex.getBestObjValue();

				//				fprintf(f, "\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
//				printf("\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}
			//			fclose(f);
		}
		else
		{
			//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
//			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			Best_Value_Formulation = cplex.getObjValue()+0.001;
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible
			if (cplex.getSolnPoolNsolns() > 0)
			{
				int value2 = cplex.getObjValue();
				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
				if (G_Dibujar)
				{
					for (int l = 0; l < numbins; l++)
					{
						for (int k = 0; k < numtiras; k++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								for (int m = 0; m < numpiezasmax; m++)
								{
									for (int i = 0; i < numpieces; i++)
									{


										if (cplex.isExtracted(x_imjkl[i][m][j][k][l]) && cplex.getValue(x_imjkl[i][m][j][k][l]) > 0.001)
										{
											printf("%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											//										fprintf(f, "%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
										}
										if (cplex.isExtracted(xr_imjkl[i][m][j][k][l]) && cplex.getValue(xr_imjkl[i][m][j][k][l]) > 0.001)
										{
											printf("%s %d w %d h %d s %d\n", xr_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
											//										fprintf(f, "%s %d w %d h %d s %d\n", xr_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
										}
										if (P_set_xa[i])
										{
											if (cplex.isExtracted(xa_imjkl[i][m][j][k][l]) && cplex.getValue(xa_imjkl[i][m][j][k][l]) > 0.001)
											{
												printf("%s %d w %d h %d s %d\n", xa_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
												//										fprintf(f, "%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											}
										}
										if (P_set_xar[i])
										{
											if (cplex.isExtracted(xar_imjkl[i][m][j][k][l]) && cplex.getValue(xar_imjkl[i][m][j][k][l]) > 0.001)
											{
												printf("%s %d w %d h %d s %d\n", xar_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
												//										fprintf(f, "%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											}
										}

									}
								}

							}
						}
					}
					//Pinto las variables w_mjkl
					for (int l = 0; l < numbins; l++)
					{
						for (int k = 0; k < numtiras; k++)
						{
							for (int j = 0; j < numpilas; j++)
							{
								for (int m = 0; m < numpiezasmax; m++)
								{
									if (cplex.isExtracted(w_mjkl[m][j][k][l]) && cplex.getValue(w_mjkl[m][j][k][l]) > 0.001)
									{
										printf("%s %2.f\n", w_mjkl[m][j][k][l].getName(), cplex.getValue(w_mjkl[m][j][k][l]));
										//								fprintf(f, "%s %2.f\n", w_mjkl[m][j][k][l].getName(), cplex.getValue(w_mjkl[m][j][k][l]));
									}
								}

							}

						}
					}
					//Pinto las variables h_jkl
					for (int l = 0; l < numbins; l++)
					{
						for (int k = 0; k < numtiras; k++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								if (cplex.isExtracted(h_jkl[j][k][l]) && cplex.getValue(h_jkl[j][k][l]) > 0.001)
								{
									printf("%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
									//								fprintf(f, "%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
								}
								if (cplex.isExtracted(ml_jkl[j][k][l]) && cplex.getValue(ml_jkl[j][k][l]) > 0.001)
								{
									printf("%s %d\n", ml_jkl[j][k][l].getName(), 1);
									//							fprintf(f, "%s %d\n", y_kl[i][j].getName(), 1);
								}

							}

						}
					}
					//Pinto las variables w_kl y y_kl
					for (int j = 0; j < numbins; j++)
					{
						for (int i = 0; i < numtiras; i++)
						{

							if (cplex.isExtracted(w_kl[i][j]) && cplex.getValue(w_kl[i][j]) > 0.001)
							{
								printf("%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
								//							fprintf(f, "%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
							}
							if (cplex.isExtracted(ms_kl[i][j]) && cplex.getValue(ms_kl[i][j]) > 0.001)
							{
								printf("%s %d\n", ms_kl[i][j].getName(), 1);
								//							fprintf(f, "%s %d\n", y_kl[i][j].getName(), 1);
							}

						}
					}

					//Pinto las variable y_l
					for (int l = 0; l < numbins; l++)
					{
						if (cplex.isExtracted(y_l[l]) && cplex.getValue(y_l[l]) > 0.001)
						{
							printf("%s %d\n", y_l[l].getName(), 1);
							//						fprintf(f, "%s %d\n", y_l[l].getName(), 1);
						}

					}
					//Pinto los cortes
					for (int l = 0; l < numbins; l++)
					{
						int nd = DefectsPlate[l].size();

						int d = 0;
						for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								for (int k = 0; k < numtiras; k++)
								{
									for (int m = 0; m < numpiezasmax; m++)
									{
										if (cplex.isExtracted(zr_dmjkl[d][m][j][k][l]) && cplex.getValue(zr_dmjkl[d][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zr_dmjkl[d][m][j][k][l].getName(), 1);
											//										fprintf(f, "%s %d\n", zr_dmjkl[d][m][j][k][l].getName(), 1);
										}
										if (cplex.isExtracted(zrp_dmjkl[d][m][j][k][l]) && cplex.getValue(zrp_dmjkl[d][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zrp_dmjkl[d][m][j][k][l].getName(), 1);
											//										fprintf(f, "%s %d\n", zrp_dmjkl[d][m][j][k][l].getName(), 1);
										}
										if (m == 0)
										{
											if (cplex.isExtracted(ze_djkl[d][j][k][l]) && cplex.getValue(ze_djkl[d][j][k][l]) > 0.001)
											{
												printf("%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
												//											fprintf(f, "%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
											}
										}
										if (j == 0 && m == 0)
										{
											if (cplex.isExtracted(zr_dkl[d][k][l]) && cplex.getValue(zr_dkl[d][k][l]) > 0.001)
											{
												printf("%s %d\n", zr_dkl[d][k][l].getName(), 1);
												//											fprintf(f, "%s %d\n", zr_dkl[d][k][l].getName(), 1);
											}

										}

									}

								}
							}
						}

					}
					//Pinto las variables zlr,zllr, zll,zlrr
					printf("\nVariables para los defectos\n");

					for (int l = 0; l < numbins; l++)
					{
						int nd = DefectsPlate[l].size();

						int d = 0;
						for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
						{
							for (int i = 0; i < numpieces; i++)
							{
								for (int j = 0; j < numpilas; j++)
								{

									for (int k = 0; k < numtiras; k++)
									{
										for (int m = 0; m < numpiezasmax; m++)
										{
											if (d == 0)
											{
												if (cplex.isExtracted(zar_imjkl[i][m][j][k][l]) && cplex.getValue(zar_imjkl[i][m][j][k][l]) > 0.001)
												{
													printf("%s %d\n", zar_imjkl[i][m][j][k][l].getName(), 1);
													//												fprintf(f, "%s %d\n", zar_imjkl[i][m][j][k][l].getName(), 1);
												}
												if (cplex.isExtracted(za_imjkl[i][m][j][k][l]) && cplex.getValue(za_imjkl[i][m][j][k][l]) > 0.001)
												{
													printf("%s %d\n", za_imjkl[i][m][j][k][l].getName(), 1);
													//												fprintf(f, "%s %d\n", za_imjkl[i][m][j][k][l].getName(), 1);
												}
											}

										}

									}
								}
							}
						}
					}

					printf("\nVariables de mínimo\n");
					for (int i = 0; i < numpieces; i++)
					{

						for (int j = 0; j < numpilas; j++)
						{

							for (int k = 0; k < numtiras; k++)
							{
								for (int l = 0; l < numbins; l++)
								{
									if (i == 0)
									{
										for (int m = 0; m < numpiezasmax; m++)
										{
											if (cplex.isExtracted(el_mjkl[m][j][k][l]) && cplex.getValue(el_mjkl[m][j][k][l]) > 0.001)
											{
												printf("%s %d\n", el_mjkl[m][j][k][l].getName(), 1);
												//											fprintf(f, "%s %d\n", el_mjkl[m][j][k][l].getName(), 1);
											}
										}
									}
									if (i == 0)
									{
										if (cplex.isExtracted(es_jkl[j][k][l]) && cplex.getValue(es_jkl[j][k][l]) > 0.001)
										{
											printf("%s %d\n", es_jkl[j][k][l].getName(), 1);
											//										fprintf(f, "%s %d\n", es_jkl[j][k][l].getName(), 1);
										}

									}

								}
							}
						}
					}
				}
			}
			//Escribo la solución para dibujar
				//Inicio
			if (G_Dibujar || G_Draw_Exacto)
			{
				FILE* fin3;
				if (G_Draw_Exacto)
				{
					 std::string str = file_idx + "_solution_" + std::to_string(G_Exacto) + "_" + std::to_string(Tipo_objetivo) + "_" + std::to_string(Duplicated) +".txt";
					 fin3=fopen(P_file_to_write.c_str(), "w+");
//					fin3 = fopen(str.c_str(), "w+");
				}
				else
					fin3 = fopen("./temp2.txt", "w+");
				int ndefects = 0;
				int nbinsdefects = 0;
				for (int i = 0; i < numbins; i++)
				{
					if (DefectsPlate[i+1].size() >= 1)
						nbinsdefects++;
					for (std::list< GlassDefect > ::iterator it = DefectsPlate[i+1].begin(); it != DefectsPlate[i+1].end(); it++)
					{
						ndefects++;
					}
				}
				//					fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, Best_Objective_function);
				fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, numpieces, ndefects, nbinsdefects, LB_Best_Value_Formulation, Best_Value_Formulation);
				//defectos

				for (int i = 0; i < numbins; i++)
				{
					fprintf(fin3, "%d\t%d\n", i+1, DefectsPlate[i+1].size());
					for (std::list< GlassDefect > ::iterator it = DefectsPlate[i+1].begin(); it != DefectsPlate[i+1].end(); it++)
					{
						// Get defect coordinates.
						int defect_x = (*it).Getpos_x();
						int defect_y = (*it).Getpos_y();
						int defect_width = (*it).Getwidth();
						int defect_height = (*it).Getheight();

						fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", defect_x, defect_y, defect_width, defect_height, i+1);
					}
				}

				for (int l = 0; l < numbins; l++)
				{
					Best_Solution_Exacto = 0;
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{
							
							int pos_x = 0;
							int pos_X = 0;
							int pos_Y = 0;
							for (int k1 = 0; k1 < k; k1++)
							{
								if (cplex.isExtracted(w_kl[k1][l]) && cplex.getValue(w_kl[k1][l]) > 0.001)
								{
									pos_X += cplex.getValue(w_kl[k1][l]);
									if (pos_X > Best_Solution_Exacto)
										Best_Solution_Exacto = pos_X;
								}
							}
							int pos_y = 0;
							for (int j1 = 0; j1 < j; j1++)
							{
								if (cplex.isExtracted(h_jkl[j1][k][l]) && cplex.getValue(h_jkl[j1][k][l]) > 0.001)
								{
									pos_y += cplex.getValue(h_jkl[j1][k][l]);
								}
							}
							for (int m = 0; m < numpiezasmax; m++)
							{

								pos_X = pos_X + cplex.getValue(w_mjkl[m][j][k][l]);
								if (pos_X > Best_Solution_Exacto)
									Best_Solution_Exacto = pos_X;

								pos_Y = pos_y + cplex.getValue(h_jkl[j][k][l]);

								bool puesta = false;
								for (int i = 0; i < numpieces && puesta == false; i++)
								{
									//buscar defecto que está dentro de este rango
									int d = 0;
									bool pegado_izquierda = false;
									bool pegado_derecha = false;
									bool pegado_abajo = false;
									bool pegado_arriba = true;

									///									for (std::list< GlassDefect > ::iterator it = DefectsPlate[l].begin(); it != DefectsPlate[l].end(); it++, d++)
									//									{
														/*					if (((*it).Getpos_x() > pos_X) || ((*it).Getpos_x() + (*it).Getwidth() < pos_x)
																				|| ((*it).Getpos_y() > pos_Y) || ((*it).Getpos_y() + (*it).Getheight() < pos_y))
																			{
																				continue;
																			}
																			//este está aquí dentro
																			if ((cplex.isExtracted(zlr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlr_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zlrr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlrr_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_derecha = true;
																			}
																			if ((cplex.isExtracted(zll_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zll_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zllr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zllr_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_izquierda = true;
																			}
																			if ((cplex.isExtracted(zla_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zla_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zlar_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlar_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_arriba = true;
																			}
																			if ((cplex.isExtracted(zle_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zle_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zler_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zler_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_abajo = true;
																			}

																			*/
																			//	if ((cplex.isExtracted(zu_dmjkl[d][m][j][k][l]) && cplex.getValue(zu_dmjkl[d][m][j][k][l]) > 0.001)
																			//		&& (cplex.isExtracted(zrp_dmjkl[d][m][j][k][l]) && cplex.getValue(zrp_dmjkl[d][m][j][k][l]) > 0.001) 
																			//		&& (cplex.isExtracted(zr_dmjkl[d][m][j][k][l]) && cplex.getValue(zr_dmjkl[d][m][j][k][l]) < 0.001))
																			//	{
																			//		pegado_arriba = false;
																			//	}

																			//}

									if (cplex.isExtracted(zu_mjkl[m][j][k][l]) && cplex.getValue(zu_mjkl[m][j][k][l]) > 0.001)
										pegado_arriba = false;
									pegado_izquierda = true;

									if (cplex.isExtracted(x_imjkl[i][m][j][k][l]) && cplex.getValue(x_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_w();
										}
										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_h();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_w(), pos_y1 + items[i].Getitem_h(), l + 1);
										//										pos_x += items[i].Getitem_w();
										puesta = true;

									}
									if (cplex.isExtracted(xr_imjkl[i][m][j][k][l]) && cplex.getValue(xr_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_h();
										}

										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_w();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_h(), pos_y1 + items[i].Getitem_w(), l + 1);
										//										pos_x += items[i].Getitem_h();
										puesta = true;
									}
								}
								puesta = false;
								//Ya se ha puesto alguna en la posición
								for (int i = 0; i < numpieces && puesta == false; i++)
								{
									bool pegado_izquierda = false;
									bool pegado_derecha = false;
									bool pegado_abajo = false;
									bool pegado_arriba = true;
									//No hay defecto
									pegado_arriba = true;
									pegado_izquierda = true;

									if (P_set_xa[i] && cplex.isExtracted(xa_imjkl[i][m][j][k][l]) && cplex.getValue(xa_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_w();
										}
										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_h();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_w(), pos_y1 + items[i].Getitem_h(), l + 1);
										//										pos_x += items[i].Getitem_w();
										puesta = true;

									}
									if (P_set_xar[i] && cplex.isExtracted(xar_imjkl[i][m][j][k][l]) && cplex.getValue(xar_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_h();
										}

										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_w();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_h(), pos_y1 + items[i].Getitem_w(), l + 1);
										//										pos_x += items[i].Getitem_h();
										puesta = true;
									}
								}


							}
						}

					}
				}


				fclose(fin3);
				DibujarOpenGL();
			}



		}

		//}


		cplex.end();
		model.end();
		env.end();
	}
	catch (IloException & e)
	{


		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
}

void Glass::FormulacionCompleta4IndicesDefectosPilas(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes)
{
	if (P_maxnivelesh * P_maxnivelesm * P_maxnivelesw > 2500)
	{
		P_maxnivelesw -= 4;
		P_maxnivelesm -= 4;
		P_maxnivelesh -= 4;
	}
	P_maxnivelesh++;
	//Compute the number of pieces
	//P_Set_ar y P_Set_xar
	int trozo = plate_w;
	if (numbins <= 1)
	{
		trozo = Best_Width;
	}

	maxTiras = P_maxnivelesw;
	//	int maxTiras=MultiKnapsackEntera(trozo);
	if (P_maxnivelesw > maxTiras)
		P_maxnivelesw = maxTiras;
	for (register int i = 0; i < batch_items; i++)
	{
		P_set_xa.push_back(false);
		P_set_xar.push_back(false);
		P_set_xpa.push_back(false);
		P_set_xrpa.push_back(false);
		std::set<int> set_da;
		std::set<int> set_dar;
		Dxx.push_back(set_da);
		Dyx.push_back(set_dar);
		Dxy.push_back(set_da);
		Dyy.push_back(set_dar);
		Ax.push_back(set_da);
		Ay.push_back(set_da);


	}
	if (P_trimming && Type != 2)
		ComputePiecesEquals();
	int kk = 9;
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	IloRangeArray restrLazy(env);
	int maxtime = G_Time;
	IloNum max_time = maxtime; //tiempo del modelo

	char nombre[20];
	//	Lazy = true;
	Lazy_minimo = false;
	Lazy_cortes = false;
	int Lazy_orden = false;
	//	knapsack_bouknap_tiras();
		//	Creo las variables
	if (P_heur) CalculateMax();
	numtiras = P_maxnivelesw;
	numpilas = P_maxnivelesh;
	int numpiezasmax = P_maxnivelesm;
	if (P_heur)
	{
		numtiras = max((int)numtiras, 3);
		numpilas = max((int)numpilas, 3);
		numpiezasmax = max((int)numpiezasmax, 3);
	}
	//	int numpiezasmax = 3;
	int numpieces = batch_items;
	numbins = nbins;


	//	numtiras = 5;
	//	numpilas = 4;
	//	numpiezasmax = 3;
	int Tipo_objetivo = 1;
	if (P_WithoutRotation == true)
	{
		P_minimo_waste = false;
		Tipo_objetivo = 4;
		numbins = 1;
		P_cota = false;

	}
	//max_numero de defectos
	int max_nd = 1;
	for (int i = 0; i < numbins; i++)
	{
		int kk = DefectsPlate[i].size();
		if (kk > max_nd)
		{
			max_nd = kk;
		}
	}

	printf("Aqui21");


	//Variables defectos
	BoolVarMatrix5 zr_dmjkl(env, max_nd);
	BoolVarMatrix5 zrp_dmjkl(env, max_nd);
	BoolVarMatrix5 zu_dmjkl(env, max_nd);
	BoolVarMatrix4 ze_djkl(env, max_nd);

	BoolVarMatrix3 zr_dkl(env, max_nd);
	//minimo defecto pilas


	BoolVarMatrix3 es_jkl(env, numpilas);
	BoolVarMatrix4 el_mjkl(env, numpiezasmax);
	BoolVarMatrix5 es_mijkl(env, numpiezasmax);
	BoolVarMatrix4 ms_mjkl(env, numpiezasmax);
	BoolVarMatrix4 zu_mjkl(env, numpiezasmax);
	BoolVarMatrix3 ml_jkl(env, numpilas);
	BoolVarMatrix2 ms_kl(env, numtiras);

	NumVarMatrix5 x_imjkl(env, numpieces);
	NumVarMatrix5 h_imjkl(env, numpieces);
	BoolVarMatrix5 xr_imjkl(env, numpieces);

	//Variables para el trimming

	BoolVarMatrix5 xa_imjkl(env, numpieces);
	BoolVarMatrix5 xar_imjkl(env, numpieces);

	BoolVarMatrix5 za_imjkl(env, numpieces);
	BoolVarMatrix5 zar_imjkl(env, numpieces);

	BoolVarMatrix2 esH_kl(env, numtiras);
	BoolVarMatrix2 y_kl(env, numtiras);
	NumVarMatrix4 w_mjkl(env, numpiezasmax);
	BoolVarMatrix4 es_mjkl(env, numpiezasmax);
	NumVarMatrix3 h_jkl(env, numpilas);
	NumVarMatrix2 w_kl(env, numtiras);

	BoolVector esW_l(env);
	BoolVector y_l(env);

	NumVector W_K(env);


	IloNumVarArray vars(env);
	IloNumArray vals(env);


	//	int LastBinWidth = floor((Total_area - ((Boolbins - 1)*plate_h*plate_w)) / plate_h);
	//	LastBinWidth = 0;
	int LastBinWidth = max((double)floor((Total_area - ((numbins - 1) * plate_h * plate_w)) / plate_h), (double)0);
	int maxh = 0;

	/**********************************************************/
	/***************** INICIALIZO VARIABLES *******************/
	/**********************************************************/
	//inicializo la variable x_ijkl y xa_imjkl

	for (int i = 0; i < numpieces; i++)
	{
		bool variable_a = true;
		if (items[i].Getitem_h() > maxh)
			maxh = items[i].Getitem_h();
		if (items[i].Getitem_w() > maxh)
			maxh = items[i].Getitem_w();
		x_imjkl[i] = NumVarMatrix4(env, numpiezasmax);
		h_imjkl[i] = NumVarMatrix4(env, numpiezasmax);
		if (P_set_xa[i])
		{
			xa_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		}
		else
			variable_a = false;
		za_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		zar_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		for (int m = 0; m < numpiezasmax; m++)
		{
			x_imjkl[i][m] = NumVarMatrix3(env, numpilas);
			h_imjkl[i][m] = NumVarMatrix3(env, numpilas);
			if (variable_a)
				xa_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			za_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			zar_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			for (int j = 0; j < numpilas; j++)
			{
				x_imjkl[i][m][j] = NumVarMatrix(env, numtiras);
				h_imjkl[i][m][j] = NumVarMatrix(env, numtiras);
				if (variable_a)
					xa_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				za_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				zar_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					x_imjkl[i][m][j][k] = IloIntVarArray(env, numbins);
					h_imjkl[i][m][j][k] = IloIntVarArray(env, numbins);
					if (variable_a)
						xa_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					za_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					zar_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					for (int l = 0; l < numbins; l++)
					{
						int max_pieces = 1;

						x_imjkl[i][m][j][k][l] = IloIntVar(env, 0, max_pieces);
						sprintf(nombre, "x_%d_%d_%d_%d_%d", i, m, j, k, l);
						x_imjkl[i][m][j][k][l].setName(nombre);
						h_imjkl[i][m][j][k][l] = IloIntVar(env, 0, plate_h/items[i].Getitem_h());
						sprintf(nombre, "h_%d_%d_%d_%d_%d", i, m, j, k, l);
						h_imjkl[i][m][j][k][l].setName(nombre);
						if (variable_a)
						{
							xa_imjkl[i][m][j][k][l] = IloBoolVar(env);
							sprintf(nombre, "xa_%d_%d_%d_%d_%d", i, m, j, k, l);
							xa_imjkl[i][m][j][k][l].setName(nombre);
						}
						za_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "za_%d_%d_%d_%d_%d", i, m, j, k, l);
						za_imjkl[i][m][j][k][l].setName(nombre);
						zar_imjkl[i][m][j][k][l] = IloBoolVar(env);
						sprintf(nombre, "zar_%d_%d_%d_%d_%d", i, m, j, k, l);
						zar_imjkl[i][m][j][k][l].setName(nombre);

					}

				}
			}
		}
	}
	//inicializo variables rotadas

	for (int i = 0; i < numpieces; i++)
	{
		bool variable_ar = true;
		xr_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);

		if (P_set_xar[i])
		{
			xar_imjkl[i] = BoolVarMatrix4(env, numpiezasmax);
		}
		else
			variable_ar = false;
		for (int m = 0; m < numpiezasmax; m++)
		{
			xr_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			if (variable_ar)
				xar_imjkl[i][m] = BoolVarMatrix3(env, numpilas);
			for (int j = 0; j < numpilas; j++)
			{
				xr_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				if (variable_ar)
					xar_imjkl[i][m][j] = BoolVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					xr_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					if (variable_ar)
						xar_imjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
					for (int l = 0; l < numbins; l++)
					{
						xr_imjkl[i][m][j][k][l] = IloBoolVar(env);

						sprintf(nombre, "xr_%d_%d_%d_%d_%d", i, m, j, k, l);
						xr_imjkl[i][m][j][k][l].setName(nombre);

						if (variable_ar)
						{
							xar_imjkl[i][m][j][k][l] = IloBoolVar(env);

							sprintf(nombre, "xar_%d_%d_%d_%d_%d", i, m, j, k, l);
							xar_imjkl[i][m][j][k][l].setName(nombre);

						}

					}

				}
			}
		}
	}
	//inicializo las varialbes de cada tira
	for (int m = 0; m < numpiezasmax; m++)
	{
		w_mjkl[m] = NumVarMatrix3(env, numpilas);
		es_mjkl[m] = BoolVarMatrix3(env, numpilas);
		zu_mjkl[m] = BoolVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			w_mjkl[m][j] = NumVarMatrix(env, numtiras);
			es_mjkl[m][j] = BoolVarMatrix(env, numtiras);
			zu_mjkl[m][j] = BoolVarMatrix(env, numtiras);
			for (int k = 0; k < numtiras; k++)
			{
				w_mjkl[m][j][k] = IloIntVarArray(env, numbins);
				es_mjkl[m][j][k] = IloBoolVarArray(env, numbins);
				zu_mjkl[m][j][k] = IloBoolVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					w_mjkl[m][j][k][l] = IloIntVar(env, 0, max1Cut);
					sprintf(nombre, "w_%d_%d_%d_%d", m, j, k, l);
					w_mjkl[m][j][k][l].setName(nombre);
					es_mjkl[m][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "ms_%d_%d_%d_%d", m, j, k, l);
					es_mjkl[m][j][k][l].setName(nombre);
					zu_mjkl[m][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "zu_%d_%d_%d_%d", m, j, k, l);
					zu_mjkl[m][j][k][l].setName(nombre);
				}

			}
		}
	}
	//inicializo la variable h_jkl
	for (int i = 0; i < numpilas; i++)
	{
		h_jkl[i] = NumVarMatrix(env, numtiras);

		for (int j = 0; j < numtiras; j++)
		{
			h_jkl[i][j] = IloIntVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				h_jkl[i][j][l] = IloIntVar(env, 0, plate_h);
				sprintf(nombre, "h_%d_%d_%d", i, j, l);
				h_jkl[i][j][l].setName(nombre);

			}

		}
	}
	//inicializo la variable w_kl y y_kl
	for (int i = 0; i < numtiras; i++)
	{
		w_kl[i] = IloIntVarArray(env, numbins);
		y_kl[i] = IloBoolVarArray(env, numbins);
		ms_kl[i] = IloBoolVarArray(env, numbins);
		esH_kl[i] = IloBoolVarArray(env, numbins);

		for (int j = 0; j < numbins; j++)
		{
			w_kl[i][j] = IloIntVar(env, 0, max1Cut);
			sprintf(nombre, "W_%d_%d", i, j);
			w_kl[i][j].setName(nombre);
			y_kl[i][j] = IloBoolVar(env);
			sprintf(nombre, "Y_%d_%d", i, j);
			y_kl[i][j].setName(nombre);
			ms_kl[i][j] = IloBoolVar(env);
			sprintf(nombre, "ms_%d_%d", i, j);
			ms_kl[i][j].setName(nombre);
			esH_kl[i][j] = IloBoolVar(env);
			sprintf(nombre, "VesH_%d_%d", i, j);
			esH_kl[i][j].setName(nombre);

		}
	}

	//inicializo las variable y_l
	y_l = IloIntVarArray(env, numbins, 0, 1);
	esW_l = IloIntVarArray(env, numbins, 0, 1);
	for (int l = 0; l < numbins; l++)
	{
		sprintf(nombre, "Y%d", l);
		y_l[l].setName(nombre);
		sprintf(nombre, "VesW%d", l);
		esW_l[l].setName(nombre);
	}
	if (Tipo_objetivo == 4)
	{
		W_K = IloIntVarArray(env, numtiras, 0, 1);
		for (int l = 0; l < numtiras; l++)
		{
			sprintf(nombre, "W%d", l);
			W_K[l].setName(nombre);
		}
	}

	//minimco corte

	for (int i = 0; i < numpiezasmax; i++)
	{
		el_mjkl[i] = BoolVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			el_mjkl[i][j] = BoolVarMatrix(env, numtiras);

			for (int k = 0; k < numtiras; k++)
			{
				el_mjkl[i][j][k] = IloBoolVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					el_mjkl[i][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "Xem_%d_%d_%d_%d", i, j, k, l);
					el_mjkl[i][j][k][l].setName(nombre);

				}

			}
		}
	}

	//inicializo la variable es_jkl
	for (int i = 0; i < numpilas; i++)
	{
		es_jkl[i] = BoolVarMatrix(env, numtiras);
		ml_jkl[i] = BoolVarMatrix(env, numtiras);
		for (int j = 0; j < numtiras; j++)
		{
			es_jkl[i][j] = IloBoolVarArray(env, numbins);
			ml_jkl[i][j] = IloBoolVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				es_jkl[i][j][l] = IloBoolVar(env);
				sprintf(nombre, "Xes_%d_%d_%d", i, j, l);
				es_jkl[i][j][l].setName(nombre);
				ml_jkl[i][j][l] = IloBoolVar(env);
				sprintf(nombre, "ml_%d_%d_%d", i, j, l);
				ml_jkl[i][j][l].setName(nombre);

			}

		}
	}

	//inicializo las variables de los cortes

	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			zr_dkl[i] = BoolVarMatrix(env, numtiras);

			for (int j = 0; j < numtiras; j++)
			{
				zr_dkl[i][j] = IloBoolVarArray(env, numbins);
				for (int kl = 0; kl < numbins; kl++)
				{

					zr_dkl[i][j][kl] = IloBoolVar(env);
					sprintf(nombre, "zr_%d_%d_%d", i, j, kl);
					zr_dkl[i][j][kl].setName(nombre);
				}
			}
		}
	}
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();

		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			zr_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
			zrp_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
			zu_dmjkl[i] = BoolVarMatrix4(env, numpiezasmax);
			for (int m = 0; m < numpiezasmax; m++)
			{
				zr_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
				zrp_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
				zu_dmjkl[i][m] = BoolVarMatrix3(env, numpilas);
				for (int j = 0; j < numpilas; j++)
				{
					zr_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
					zrp_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
					zu_dmjkl[i][m][j] = BoolVarMatrix(env, numtiras);
					for (int k = 0; k < numtiras; k++)
					{

						zr_dmjkl[i][m][j][k] = IloBoolVarArray(env, numbins);
						zrp_dmjkl[i][m][j][k] = IloBoolVarArray(env, numbins);

						for (int kl = 0; kl < numbins; kl++)
						{
							zr_dmjkl[i][m][j][k][kl] = IloBoolVar(env);
							sprintf(nombre, "zr_%d_%d_%d_%d_%d", i, m, j, k, kl);
							zr_dmjkl[i][m][j][k][kl].setName(nombre);
							zrp_dmjkl[i][m][j][k][kl] = IloBoolVar(env);
							sprintf(nombre, "zrp_%d_%d_%d_%d_%d", i, m, j, k, kl);
							zrp_dmjkl[i][m][j][k][kl].setName(nombre);
						}
					}

				}
			}
		}
	}
	for (int l = 0; l < numbins; l++)
	{
		int nd = DefectsPlate[l].size();
		int i = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
		{
			ze_djkl[i] = BoolVarMatrix3(env, numpilas);

			for (int j = 0; j < numpilas; j++)
			{
				ze_djkl[i][j] = BoolVarMatrix(env, numtiras);
				for (int k = 0; k < numtiras; k++)
				{
					ze_djkl[i][j][k] = IloBoolVarArray(env, numbins);
					for (int kl = 0; kl < numbins; kl++)
					{
						ze_djkl[i][j][k][kl] = IloBoolVar(env);
						sprintf(nombre, "ze_%d_%d_%d_%d", i, j, k, kl);
						ze_djkl[i][j][k][kl].setName(nombre);
					}
				}

			}
		}
	}

	//inicializo las variables de los defectos


	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/
	//Fijo


//	Lazy_minimo = false;
	//Todos los productos colocados
	for (int i = 0; i < numpieces; i++)
	{
		IloExpr v1(env);
		for (int j = 0; j < numpilas; j++)
		{
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						v1 += x_imjkl[i][m][j][k][l];
						if (!P_WithoutRotation)
						v1 += xr_imjkl[i][m][j][k][l];
						if (!P_WithoutPiles)
						{
							IloExpr v2(env);
							v2 += (-1) * (plate_h / items[i].Getitem_h()) * x_imjkl[i][m][j][k][l];
							if (!P_WithoutRotation)
								v2 += (-1) * (plate_h / items[i].Getitem_w()) * xr_imjkl[i][m][j][k][l];
							v2 += h_imjkl[i][m][j][k][l];
							restr.add(IloRange(env, -IloInfinity, v2, 0));
							v2.end();
						}
						if (P_set_xa[i])
							v1 += xa_imjkl[i][m][j][k][l];
						if (P_set_xar[i])
							v1 += xar_imjkl[i][m][j][k][l];
					}
				}
			}
		}
		if (Tipo_objetivo < 3)
			restr.add(IloRange(env, 1, v1, 1));
		else
		{
			restr.add(IloRange(env, -IloInfinity, v1, 1));
		}

		v1.end();
	}
	//Restricciones para el ORDEN de los trimming
	//CONSTRAINTS ORDERS TRIMMING

	for (int j = 0; j < numpilas; j++)
	{
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{

					for (int i = 0; i < numpieces; i++)
					{
						if (P_set_xa[i])
						{
							IloExpr v1(env);
							v1 += xa_imjkl[i][m][j][k][l];

							for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
							{
								v1 += (-1) * x_imjkl[(*it)][m][j][k][l];
							}
							for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
							{
								v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];
							}
							restr.add(IloRange(env, -IloInfinity, v1, 0));
							v1.end();

						}
						if (P_set_xar[i])
						{
							IloExpr v1(env);
							v1 += xar_imjkl[i][m][j][k][l];

							for (std::set<int> ::iterator it = Dxy[i].begin(); it != Dxy[i].end(); it++)
							{
								v1 += (-1) * x_imjkl[(*it)][m][j][k][l];
							}
							for (std::set<int> ::iterator it = Dyy[i].begin(); it != Dyy[i].end(); it++)
							{
								v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];
							}

							restr.add(IloRange(env, -IloInfinity, v1, 0));
							v1.end();

						}

					}
				}
			}
		}
	}
	//Si coloco la segunda pieza también la primera
	//solamente una por celda

	if (Type != 2)
	{
		for (int j = 0; j < numpilas; j++)
		{
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						//						IloExpr v1(env);
						IloExpr v1(env);
						IloExpr v2(env);
						IloExpr v3(env);
						for (int i = 0; i < numpieces; i++)
						{
							//orden secciones
							if (m > 0)
							{
								v3 += x_imjkl[i][m][j][k][l];
								v3 += xr_imjkl[i][m][j][k][l];
								v3 += (-1) * x_imjkl[i][m - 1][j][k][l];
								v3 += (-1) * xr_imjkl[i][m - 1][j][k][l];
							}
							v1 += x_imjkl[i][m][j][k][l];
							v1 += xr_imjkl[i][m][j][k][l];
							if (P_set_xa[i])
								v2 += xa_imjkl[i][m][j][k][l];
							if (P_set_xar[i])
								v2 += xar_imjkl[i][m][j][k][l];



						}
						//V1 indica que si ponemos la pieza m, antes también tiene que haber piezas
						restr.add(IloRange(env, -IloInfinity, v3, 0));

						restr.add(IloRange(env, -IloInfinity, v1, 1));
						restr.add(IloRange(env, -IloInfinity, v2, 1));
						v1.end();
						v2.end();
						v3.end();



					}
				}
			}
		}
	}
	//	Lazy_minimo=false;
		//Altura mínima altura
//CONSTRAINTS HEIGHT
	for (int j = 0; j < numpilas; j++)
	{

		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{

				for (int m = 0; m < numpiezasmax; m++)
				{
					IloExpr v1(env);
					IloExpr v2(env);

					for (int i = 0; i < numpieces; i++)
					{
						if (P_WithoutPiles)
						{
							v1 += items[i].Getitem_h() * x_imjkl[i][m][j][k][l];
							v2 += items[i].Getitem_h() * x_imjkl[i][m][j][k][l];
						}
						else
						{
							v1 += items[i].Getitem_h() * h_imjkl[i][m][j][k][l];
							v2 += items[i].Getitem_h() * h_imjkl[i][m][j][k][l];
						}
						if (!P_WithoutRotation)
						v1 += items[i].Getitem_w() * xr_imjkl[i][m][j][k][l];

						if (!P_WithoutRotation)
						v2 += items[i].Getitem_w() * xr_imjkl[i][m][j][k][l];

						if (P_set_xa[i])
							//							for (std::set<int>::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
						{
							v1 += items[i].Getitem_h() * xa_imjkl[i][m][j][k][l];
							v2 += items[i].Getitem_h() * xa_imjkl[i][m][j][k][l];
						}
						if (P_set_xar[i])
							//							for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
						{
							v1 += items[i].Getitem_w() * xar_imjkl[i][m][j][k][l];
							v2 += items[i].Getitem_w() * xar_imjkl[i][m][j][k][l];
						}



					}
					v1 += (-1) * h_jkl[j][k][l];
					if (P_minimo_waste)
						v1 += waste_min * (1 - el_mjkl[m][j][k][l]);

					restr.add(IloRange(env, -IloInfinity, v1, 0));
					v1.end();
					v2 += (-1) * h_jkl[j][k][l];
					if (P_minimo_waste)
					{
						v2 += plate_h * (1 - el_mjkl[m][j][k][l]);
						if (Lazy_minimo)
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						else
							restr.add(IloRange(env, 0, v2, IloInfinity));
					}
					v2.end();

				}
			}
		}
	}


	//CONSTRAINTS MINIMIZAR TIRAS
	if (Tipo_objetivo == 4 && Type!=2)
	{
		//Mete una que obligue a que si W_K vale algo es porque tiene algo
		for (int k = 0; k < numtiras; k++)
		{
			IloExpr v1(env);
			v1 += W_K[k];
			for (int i = 0; i < numpieces; i++)
			{
				for (int j = 0; j < numpilas; j++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						v1 += (-1) * x_imjkl[i][m][j][k][0];
						if (P_WithoutRotation)
						v1 += (-1) * xr_imjkl[i][m][j][k][0];
					}
				}
			}
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
		}

	}

	// CONSTRAINT TRIMMING
	//Si pongo un triming la altura tiene que ser perfecta
	if (P_trimming )
	{
		for (int j = 0; j < numpilas; j++)
		{

			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{

					for (int m = 0; m < numpiezasmax; m++)
					{

						IloExpr v1(env);

						for (int i = 0; i < numpieces; i++)
						{
							if (P_set_xa[i])
								//							for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
							{
								v1 += 2 * xa_imjkl[i][m][j][k][l];
								//								v2 += 2* xa_imjkl[i][m][j][k][l];
							}
							if (P_set_xar[i])
								//							for (std::set<int>::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
							{
								v1 += 2 * xar_imjkl[i][m][j][k][l];
								//								v2 += 2* xar_imjkl[i][m][j][k][l];
							}





						}
						v1 += ((-1) * zu_mjkl[m][j][k][l]);

						v1 += (-1) * el_mjkl[m][j][k][l];
						restr.add(IloRange(env, -IloInfinity, v1, 0));
						v1.end();
						//						restr.add(IloRange(env, -IloInfinity, v2, 1));
							//					v2.end();
					}
				}
			}
		}
	}
	//CONSTRAINT WIDTH

//	Lazy_minimo = false;
	//Anchura de los cortes en cada pila
	//minimo corte consecutivo en los de tres

	for (int j = 0; j < numpilas; j++)
	{

		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					IloExpr v1(env);
					IloExpr v2(env);

					for (int i = 0; i < numpieces; i++)
					{


						v1 += items[i].Getitem_w() * x_imjkl[i][m][j][k][l];
						if (!P_WithoutRotation)
						v1 += items[i].Getitem_h() * xr_imjkl[i][m][j][k][l];
						
						v2 += items[i].Getitem_w() * x_imjkl[i][m][j][k][l];
						if (!P_WithoutRotation)
						v2 += items[i].Getitem_h() * xr_imjkl[i][m][j][k][l];

					}
					v1 += (-1) * w_mjkl[m][j][k][l];
					if (P_minimo_waste)
						v1 += (1 - es_mjkl[m][j][k][l]) * waste_min;
					v2 += (-1) * w_mjkl[m][j][k][l];
					v2 += (1 - es_mjkl[m][j][k][l]) * max1Cut;

					restr.add(IloRange(env, -IloInfinity, v1, 0));
					v1.end();
					if (P_minimo_waste)
					{
						if (Lazy_minimo)
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						else
							restr.add(IloRange(env, 0, v2, IloInfinity));
					}
					v2.end();




				}
			}
		}
	}





	//CONSTRAINT WIDHT
	/**/
	//suma de minimo corte anchos de las tiras
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			for (int j = 0; j < numpilas; j++)
			{

				IloExpr v1(env);
				IloExpr v2(env);
				for (int m = 0; m < numpiezasmax; m++)
				{
					v1 += w_mjkl[m][j][k][l];
					v2 += w_mjkl[m][j][k][l];

				}

				v1 += (-1) * w_kl[k][l];
				if (P_minimo_waste)
					v1 += (1 - es_jkl[j][k][l]) * waste_min;

				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();

				v2 += (-1) * w_kl[k][l];
				v2 += (1 - es_jkl[j][k][l]) * max1Cut;
				if (P_minimo_waste)
				{
					if (Lazy_minimo)
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));

					else
						restr.add(IloRange(env, 0, v2, IloInfinity));
				}
				v2.end();
			}

		}
	}

	//minimos cortes
		//suma de las alturas de las pilas menor H
		//CONSTRAINT HEIGHT
		//CONSTRAINT HEIGHT
	//suma de las alturas de las pilas menor H


	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			IloExpr v1(env);
			IloExpr v2(env);
			IloExpr v3(env);
			for (int j = 0; j < numpilas; j++)
			{


				v1 += h_jkl[j][k][l];
				v2 += h_jkl[j][k][l];
				v3 += h_jkl[j][k][l];
			}
			v1 += (-1) * plate_h * ms_kl[k][l];
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
			v2 += (-1) * plate_h;
			v2 += (1 - esH_kl[k][l]) * waste_min;
			if (P_minimo_waste)
				restr.add(IloRange(env, -IloInfinity, v2, 0));
			v2.end();
			v3 += (-1) * plate_h;
			v3 += (1 - esH_kl[k][l]) * plate_h;
			if (P_minimo_waste)
				restr.add(IloRange(env, 0, v3, IloInfinity));
			v3.end();
		}
	}
	//suma de las alturas de las pilas menor H
//	Lazy_minimo = true;

	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{

			for (int j = 0; j < numpilas; j++)
			{
				//				Lazy_minimo = true;
				IloExpr v1(env);
				v1 += min2Cut * ml_jkl[j][k][l];

				v1 += (-1) * h_jkl[j][k][l];

				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
				IloExpr v2(env);
				v2 += plate_h * ml_jkl[j][k][l];
				v2 += (-1) * h_jkl[j][k][l];
				if (Lazy_minimo)
					restrLazy.add(IloRange(env, 0, v2, IloInfinity));
				else
					restr.add(IloRange(env, 0, v2, IloInfinity));
				v2.end();
				//no crea pilas vacías y además ordenadas

				IloExpr v3(env);
				v3 += ml_jkl[j][k][l];
				for (int i = 0; i < numpieces; i++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						v3 += (-1) * x_imjkl[i][m][j][k][l];
						if (!P_WithoutRotation)
						v3 += (-1) * xr_imjkl[i][m][j][k][l];
					}
				}
				restr.add(IloRange(env, -IloInfinity, v3, 0));
				v3.end();

				if (j > 0)
				{
					IloExpr v4(env);
					v4 += ml_jkl[j][k][l];
					v4 += (-1) * ml_jkl[j - 1][k][l];
					restr.add(IloRange(env, -IloInfinity, v4, 0));
					v4.end();
				}




			}
			//			Lazy_minimo = false;
			IloExpr v1(env);
			v1 += min1Cut * ms_kl[k][l];

			v1 += (-1) * w_kl[k][l];

			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
			IloExpr v2(env);
			v2 += max1Cut * ms_kl[k][l];
			v2 += (-1) * w_kl[k][l];
			if (Lazy_minimo)
				restrLazy.add(IloRange(env, 0, v2, IloInfinity));
			else
				restr.add(IloRange(env, 0, v2, IloInfinity));
			v2.end();

			//no crea tiras vacías y además ordenadas
			IloExpr v3(env);
			v3 += ms_kl[k][l];
			for (int i = 0; i < numpieces; i++)
			{
				for (int m = 0; m < numpiezasmax; m++)
				{
					for (int j = 0; j < numpilas; j++)
					{
						v3 += (-1) * x_imjkl[i][m][j][k][l];
						if (!P_WithoutRotation)
						v3 += (-1) * xr_imjkl[i][m][j][k][l];
					}
				}
			}
			restr.add(IloRange(env, -IloInfinity, v3, 0));
			v3.end();
			if (k > 0)
			{
				IloExpr v4(env);
				v4 += ms_kl[k][l];
				v4 += (-1) * ms_kl[k - 1][l];
				restr.add(IloRange(env, -IloInfinity, v4, 0));
				v4.end();
			}

		}
	}
	//CONSTRAINT WIDTH
	//Suma  tiras menor o igual a W
	for (int l = 0; l < numbins; l++)
	{
		IloExpr v1(env);
		IloExpr v2(env);
		IloExpr v3(env);
		for (int k = 0; k < numtiras; k++)
		{

			v1 += w_kl[k][l];
			v2 += w_kl[k][l];
			v3 += w_kl[k][l];

		}

		v1 += (-1) * plate_w * y_l[l];
		restr.add(IloRange(env, -IloInfinity, v1, 0));
		v1.end();

		v2 += (-1) * plate_w;
		v2 += (1 - esW_l[l]) * waste_min;
		if (P_minimo_waste)
			restr.add(IloRange(env, -IloInfinity, v2, 0));
		v2.end();
		v3 += (-1) * plate_w;
		v3 += (1 - esW_l[l]) * plate_w;
		if (P_minimo_waste)
			restr.add(IloRange(env, 0, v3, IloInfinity));
		v3.end();
	}


	//CONSTRAINT ORDER STACKS
	//Orden de los stacks por parejas
	//******************

	//
	if (P_orders)
	{
		for (int l = 0; l < numbins; l++)
		{

			for (int k = 0; k < numtiras; k++)
			{

				for (int j = 0; j < numpilas; j++)
				{
					for (int m = 0; m < numpiezasmax; m++)
					{
						for (int i = numpieces - 1; i > 0; i--)
						{


							if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1 && (items[i].Getitem_seq() > 1))
							{
								int kk = stacks[items[i].Getitem_stack()].Getitem_nbr();
								IloExpr v1(env);
								v1 += x_imjkl[i][m][j][k][l];
								v1 += xr_imjkl[i][m][j][k][l];
								if (P_set_xa[i])
									v1 += xa_imjkl[i][m][j][k][l];
								if (P_set_xar[i])
									v1 += xar_imjkl[i][m][j][k][l];
								// ponemos las variables
								int i2 = i - 1;
								if (i2 >= 0 && items[i].Getitem_stack() == items[i2].Getitem_stack())
								{

									for (int l2 = 0; l2 <= l; l2++)
									{
										for (int k2 = 0; k2 < numtiras; k2++)
										{
											if (l2 == l && k2 > k)
												break;
											for (int j2 = 0; j2 < numpilas; j2++)
											{
												if (l2 == l && k2 == k && j2 > j)
													break;
												for (int m2 = 0; m2 < numpiezasmax; m2++)
												{
													if (l2 == l && k2 == k && j2 == j && m2 >= m)
														break;

													v1 += (-1) * x_imjkl[i2][m2][j2][k2][l2];
													v1 += (-1) * xr_imjkl[i2][m2][j2][k2][l2];
													if (P_set_xa[i2])
														v1 += (-1) * xa_imjkl[i2][m2][j2][k2][l2];
													if (P_set_xar[i2])
														v1 += (-1) * xar_imjkl[i2][m2][j2][k2][l2];

												}
											}

										}
									}

								}
								for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
									{
										v1 += (-1) * x_imjkl[(*it)][m][j][k][l];

									}

								}
								for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
									{
										v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];

									}
								}
								for (std::set<int> ::iterator it = Dxy[i].begin(); it != Dxy[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
										v1 += (-1) * x_imjkl[(*it)][m][j][k][l];
								}
								for (std::set<int> ::iterator it = Dyy[i].begin(); it != Dyy[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
										v1 += (-1) * xr_imjkl[(*it)][m][j][k][l];
								}
								if (Lazy_orden)
									restrLazy.add(IloRange(env, -IloInfinity, v1, 0));

								else
									restr.add(IloRange(env, -IloInfinity, v1, 0));
								v1.end();




							}



						}
					}
				}
			}
		}
	}
	//CONSTRAINTS CUT VERTICAL
	//restricciones de los cortes verticales
	if (P_defects)
	{
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{

				for (int k1 = 0; k1 < numtiras; k1++)
				{
					IloExpr v1(env);
					IloExpr v2(env);
					for (int k2 = 0; k2 <= k1; k2++)
					{
						v1 += w_kl[k2][l];
						v2 += w_kl[k2][l];
					}
					v1 += (-1) * ((*itD).Getpos_x());

					v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1][l]);
					//			v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth());
					v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zr_dkl[i][k1][l]);


					if (Lazy_cortes)
					{
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));
						restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
					}
					else
					{
						restr.add(IloRange(env, 0, v2, IloInfinity));
						restr.add(IloRange(env, -IloInfinity, v1, 0));
					}
					v1.end();
					v2.end();
					if (k1 > 0)
					{
						IloExpr v3(env);
						v3 += zr_dkl[i][k1][l];
						v3 += (-1) * zr_dkl[i][k1 - 1][l];
						restr.add(IloRange(env, -IloInfinity, v3, 0));
						v3.end();
					}

					//consistencias r
					for (int j1 = 0; j1 < numpilas; j1++)
					{
						for (int m = 0; m < numpiezasmax; m++)
						{
							if (k1 > 0)
							{
								IloExpr v3(env);
								v3 += zr_dmjkl[i][m][j1][k1][l];
								v3 += (-1) * zr_dkl[i][k1 - 1][l];
								restr.add(IloRange(env, -IloInfinity, v3, 0));
								v3.end();
								IloExpr v4(env);
								v4 += zrp_dmjkl[i][m][j1][k1][l];
								v4 += (-1) * zr_dkl[i][k1 - 1][l];
								restr.add(IloRange(env, -IloInfinity, v4, 0));
								v4.end();
							}
						}
					}
				}



			}
		}
		//CONSTRAINTS CUT HORIZONTAL
		//restricciones de los cortes horizontales
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{
				for (int k = 0; k < numtiras; k++)
				{

					for (int j1 = 0; j1 < numpilas; j1++)
					{
						IloExpr v1(env);
						IloExpr v2(env);
						for (int j2 = 0; j2 <= j1; j2++)
						{

							v1 += h_jkl[j2][k][l];
							v2 += h_jkl[j2][k][l];
						}



						v1 += (-1) * ((*itD).Getpos_y());
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - ze_djkl[i][j1][k][l]);
						v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (zr_dkl[i][k][l]);
						if (k > 0)
						{
							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zr_dkl[i][k - 1][l]);
						}

						//ahora falta la otra parte
						int rtyd = (*itD).Getpos_y() + (*itD).Getheight();
						v2 += (-1) * (rtyd) * (1 - ze_djkl[i][j1][k][l]);
						v2 += (rtyd) * (zr_dkl[i][k][l]);
						if (k > 0)
						{
							v2 += rtyd * (1 - zr_dkl[i][k - 1][l]);
						}

						if (Lazy_cortes)
						{
							restrLazy.add(IloRange(env, 0, v2, IloInfinity));
							restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
						}
						else
						{
							restr.add(IloRange(env, 0, v2, IloInfinity));
							restr.add(IloRange(env, -IloInfinity, v1, 0));
						}
						v1.end();
						v2.end();
						if (j1 > 0)
						{
							IloExpr v3(env);
							v3 += ze_djkl[i][j1][k][l];
							v3 += (-1) * ze_djkl[i][j1 - 1][k][l];
							restr.add(IloRange(env, -IloInfinity, v3, 0));
							v3.end();


						}
						//Quitar y pongo las otras
						IloExpr v4(env);
						v4 += ze_djkl[i][j1][k][l];
						if (k > 0)
							v4 += (-1) * (zr_dkl[i][k - 1][l] - zr_dkl[i][k][l]);
						else
							v4 += (-1) * (1 - zr_dkl[i][k][l]);
						restr.add(IloRange(env, -IloInfinity, v4, 0));
						v4.end();
						//				

					}
					//orden de las variables
				}






			}
		}
		//CONSTRAINTS HORIZONTAL 2ORDER

		//restricciones de los cortes verticales de segundo orden de defecto

		//Deerecha
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{

				for (int k1 = 0; k1 < numtiras; k1++)
				{
					for (int j = 0; j < numpilas; j++)
					{
						for (int m = 0; m < numpiezasmax; m++)
						{
							IloExpr v1(env);
							IloExpr v2(env);
							if (k1 > 0)
							{
								for (int k2 = 0; k2 < k1; k2++)
								{

									v1 += w_kl[k2][l];
									v2 += w_kl[k2][l];
								}
							}
							for (int m2 = 0; m2 <= m; m2++)
							{

								v1 += w_mjkl[m2][j][k1][l];
								v2 += w_mjkl[m2][j][k1][l];
							}
							v1 += (-1) * ((*itD).Getpos_x());
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dmjkl[i][m][j][k1][l]);
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * ze_djkl[i][j][k1][l];
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (zr_dkl[i][k1][l]);
							if (k1 > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1 - 1][l]);
							if (j > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - ze_djkl[i][j - 1][k1][l]);
							int rtxd = (*itD).Getpos_x() + (*itD).Getwidth();
							v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zr_dmjkl[i][m][j][k1][l]);
							v2 += (rtxd)*ze_djkl[i][j][k1][l];
							v2 += (rtxd) * (zr_dkl[i][k1][l]);
							if (k1 > 0)
								v2 += (rtxd) * (1 - zr_dkl[i][k1 - 1][l]);

							if (j > 0)
								v2 += (rtxd) * (1 - ze_djkl[i][j - 1][k1][l]);
							if (Lazy_cortes)
							{
								restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
								restrLazy.add(IloRange(env, 0, v2, IloInfinity));
							}
							else
							{
								restr.add(IloRange(env, -IloInfinity, v1, 0));
								restr.add(IloRange(env, 0, v2, IloInfinity));
							}
							v1.end();
							v2.end();
							if (m > 0)
							{
								IloExpr v3(env);
								v3 += zr_dmjkl[i][m][j][k1][l];
								v3 += (-1) * zr_dmjkl[i][m - 1][j][k1][l];
								restr.add(IloRange(env, -IloInfinity, v3, 0));
								v3.end();
							}
							//if (m ==0)
							//{
							//	IloExpr v3(env);
							//	v3 += zr_dkl[i][k1][l];
							//	v3 += (-1) * zr_dmjkl[i][numpiezasmax-1][j][k1][l];
							//	restr.add(IloRange(env, -IloInfinity, v3, 0));
							//	v3.end();
							//}

							IloExpr v4(env);
							v4 += zr_dmjkl[i][m][j][k1][l];
							if (j > 0)
								v4 += (-1) * (ze_djkl[i][j - 1][k1][l] - ze_djkl[i][j][k1][l]);
							else
								v4 += (-1) * (1 - ze_djkl[i][j][k1][l]);
							restr.add(IloRange(env, -IloInfinity, v4, 0));
							v4.end();


						}
					}
				}



			}
		}

		//CONSTRAINTS HORIZONTAL 2ORDER LEFT
		//cortes
		//restricciones de los cortes verticales de segundo orden de defecto
		//Izquierda
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{

				for (int k1 = 0; k1 < numtiras; k1++)
				{
					for (int j = 0; j < numpilas; j++)
					{
						for (int m = 0; m < numpiezasmax; m++)
						{
							IloExpr v1(env);
							IloExpr v2(env);
							if (k1 > 0)
							{
								for (int k2 = 0; k2 < k1; k2++)
								{

									v1 += w_kl[k2][l];
									v2 += w_kl[k2][l];
								}
							}
							for (int m2 = 0; m2 <= m; m2++)
							{

								v1 += w_mjkl[m2][j][k1][l];
								v2 += w_mjkl[m2][j][k1][l];
							}
							for (int i1 = 0; i1 < numpieces; i1++)
							{
								v1 += (-1) * items[i1].Getitem_w() * x_imjkl[i1][m][j][k1][l];
								if (!P_WithoutRotation)
								v1 += (-1) * items[i1].Getitem_h() * xr_imjkl[i1][m][j][k1][l];
								v2 += (-1) * items[i1].Getitem_w() * x_imjkl[i1][m][j][k1][l];
								if (!P_WithoutRotation)
								v2 += (-1) * items[i1].Getitem_h() * xr_imjkl[i1][m][j][k1][l];
							}


							v1 += (-1) * ((*itD).Getpos_x());
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zrp_dmjkl[i][m][j][k1][l]);
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * ze_djkl[i][j][k1][l];
							v1 += (-1) * (plate_w - (*itD).Getpos_x()) * zr_dkl[i][k1][l];
							if (k1 > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - zr_dkl[i][k1 - 1][l]);

							if (j > 0)
								v1 += (-1) * (plate_w - (*itD).Getpos_x()) * (1 - ze_djkl[i][j - 1][k1][l]);
							int rtxd = (*itD).Getpos_x() + (*itD).Getwidth();
							v2 += (-1) * ((*itD).Getpos_x() + (*itD).Getwidth()) * (1 - zrp_dmjkl[i][m][j][k1][l]);
							v2 += (rtxd)*ze_djkl[i][j][k1][l];
							v2 += (rtxd)*zr_dkl[i][k1][l];
							if (k1 > 0)
								v2 += (rtxd) * (1 - zr_dkl[i][k1 - 1][l]);

							if (j > 0)
								v2 += (rtxd) * (1 - ze_djkl[i][j - 1][k1][l]);
							if (Lazy_cortes)
							{
								restrLazy.add(IloRange(env, -IloInfinity, v1, 0));
								restrLazy.add(IloRange(env, 0, v2, IloInfinity));
							}
							else
							{
								restr.add(IloRange(env, -IloInfinity, v1, 0));
								restr.add(IloRange(env, 0, v2, IloInfinity));
							}
							v1.end();
							v2.end();
							if (m > 0)
							{
								IloExpr v3(env);
								v3 += zrp_dmjkl[i][m][j][k1][l];
								v3 += (-1) * zrp_dmjkl[i][m - 1][j][k1][l];
								restr.add(IloRange(env, -IloInfinity, v3, 0));
								v3.end();
							}
							//if (m ==0)
							//{
							//	IloExpr v3(env);
							//	v3 += zr_dkl[i][k1][l];
							//	v3 += (-1) * zr_dmjkl[i][numpiezasmax-1][j][k1][l];
							//	restr.add(IloRange(env, -IloInfinity, v3, 0));
							//	v3.end();
							//}

							//31/03/2020
							IloExpr v4(env);
							v4 += zrp_dmjkl[i][m][j][k1][l];
							if (j > 0)
								v4 += (-1) * (ze_djkl[i][j - 1][k1][l] - ze_djkl[i][j][k1][l]);
							else
								v4 += (-1) * (1 - ze_djkl[i][j][k1][l]);
							restr.add(IloRange(env, -IloInfinity, v4, 0));
							v4.end();
							//IloExpr v4(env);
							//v4 += zrp_dmjkl[i][m][j][k1][l];
							//if (j > 0)
							//	v4 += (-1) * (ze_djkl[i][j - 1][k1][l] - ze_djkl[i][j][k1][l]);
							//else
							//	v4 += (-1) * (1 - ze_djkl[i][j][k1][l]);
							//restr.add(IloRange(env, -IloInfinity, v4, 0));
							//v4.end();

						}
					}
				}



			}
		}
		//
		//restricciones de los cortes horizontales de la pieza
		for (int l = 0; l < numbins; l++)
		{
			int nd = DefectsPlate[l].size();

			int i = 0;
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, i++)
			{
				for (int k = 0; k < numtiras; k++)
				{

					for (int j1 = 0; j1 < numpilas; j1++)
					{

						for (int m1 = 0; m1 < numpiezasmax; m1++)
						{
							IloExpr v1(env);
							IloExpr v2(env);
							for (int j2 = 0; j2 <= j1; j2++)
							{
								if (j2 < j1)
									v1 += h_jkl[j2][k][l];
								v2 += h_jkl[j2][k][l];
							}
							for (int i1 = 0; i1 < numpieces; i1++)
							{
								if (P_WithoutPiles)
								{
									v1 += items[i1].Getitem_h() * (x_imjkl[i1][m1][j1][k][l]);
									v2 += (-1) * items[i1].Getitem_h() * x_imjkl[i1][m1][j1][k][l];
	
								}
								else
								{
									v1 += items[i1].Getitem_h() * (h_imjkl[i1][m1][j1][k][l]);
									v2 += (-1) * items[i1].Getitem_h() * h_imjkl[i1][m1][j1][k][l];

								}
								if (!P_WithoutRotation)
								v1 += items[i1].Getitem_w() * (xr_imjkl[i1][m1][j1][k][l]);
								
								if (!P_WithoutRotation)
								v2 += (-1) * items[i1].Getitem_w() * xr_imjkl[i1][m1][j1][k][l];
							}


							v1 += (-1) * ((*itD).Getpos_y());
							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zu_mjkl[m1][j1][k][l]);

							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - zrp_dmjkl[i][m1][j1][k][l]);
							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (zr_dmjkl[i][m1][j1][k][l]);

							v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (ze_djkl[i][j1][k][l]);
							if (j1 > 0)
								v1 += (-1) * (plate_h - (*itD).Getpos_y()) * (1 - ze_djkl[i][j1 - 1][k][l]);
							//ahora falta la otra parte
							int rtyd = (*itD).Getpos_y() + (*itD).Getheight();
							v2 += (-1) * (rtyd) * (1 - zu_mjkl[m1][j1][k][l]);
							v2 += ((rtyd) * (1 - zrp_dmjkl[i][m1][j1][k][l]));
							v2 += ((rtyd)) * (zr_dmjkl[i][m1][j1][k][l]);


							v2 += ((rtyd)) * (ze_djkl[i][j1][k][l]);
							if (j1 > 0)
								v2 += (rtyd) * (1 - ze_djkl[i][j1 - 1][k][l]);
							restr.add(IloRange(env, -IloInfinity, v1, 0));
							if (Lazy_cortes)
							{
								restrLazy.add(IloRange(env, 0, v2, IloInfinity));

							}
							else
							{
								restr.add(IloRange(env, 0, v2, IloInfinity));

							}
							v1.end();
							v2.end();
							//1/04/2020

							IloExpr v3(env);
							v3 += el_mjkl[m1][j1][k][l];
							v3 += (-1);
							v3 += zrp_dmjkl[i][m1][j1][k][l];
							v3 += (-1) * zr_dmjkl[i][m1][j1][k][l];
							restr.add(IloRange(env, -IloInfinity, v3, 0));
							v3.end();
							//if (j1 > 0)
//{
//	IloExpr v3(env);
//	v3 += ze_djkl[i][j1][k][l];
//	v3 += (-1) * ze_djkl[i][j1 - 1][k][l];
//	restr.add(IloRange(env, -IloInfinity, v3, 0));
//	v3.end();


//}

//if (j1 > 0)
//{
//	IloExpr v3(env);
//	v3 += ze_djkl[i][j1][k][l];
//	v3 += (-1) * ze_djkl[i][j1 - 1][k][l];
//	restr.add(IloRange(env, -IloInfinity, v3, 0));
//	v3.end();


//}
//IloExpr v4(env);
//v4 +=2* zu_mjkl[m1][j1][k][l];

//v4 += (-1) * zrp_dmjkl[i][m1][j1][k][l];
//v4 += (-1) * (1-zr_dmjkl[i][m1][j1][k][l]);

//restr.add(IloRange(env, -IloInfinity, v4, 0));
//v4.end();


						}


					}
					//orden de las variables
				}






			}
		}
	}
	if (P_cota)
	{
		if ((Tipo_objetivo == 2 && ((P_TrozoArea < 6000) && (numbins == P_BinsCotaArea + 1))))
		{
			IloExpr v4(env);
			for (int k = 0; k < numtiras; k++)
			{


				v4 += w_kl[k][numbins - 1];
			}
			v4 += (-1) * P_TrozoArea;
			restr.add(IloRange(env, 0, v4, IloInfinity));
			v4.end();
		}

		if (Type == 2)
		{
			IloExpr v4(env);
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{
					for (int j = 0; j < numpilas; j++)
					{

						for (int m = 0; m < numpiezasmax; m++)
						{

							for (int i = 0; i < numpieces; i++)
							{
								v4 += items[i].Getitem_value() * h_imjkl[i][m][j][k][l];
							}
						}
					}
				}
			}
			v4 += (-1) * 181;
			restr.add(IloRange(env, -IloInfinity, v4, 0));
			v4.end();
		}
	}
	model.add(restr);
	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
//	model.add(restr);


	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);
	if (Tipo_objetivo == 5)
	{
		//		IloExpr Expression_fobj(env);
		for (int i = 0; i < numtiras; i++)
		{
			Expression_fobj += W_K[i];
		}


	}
	if (Tipo_objetivo == 0)//biobjective //estaría en diet.ccp
	{
		IloExpr Expression_fobj1(env);
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj1 += y_l[i];
		}
		IloExpr Expression_fobj2(env);
		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj2 += w_kl[k][numbins - 1];
		}
	}

	if (Tipo_objetivo == 1)//por bins
	{
		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj += y_l[i];
		}
	}
	if (Tipo_objetivo == 2)//por tira del último bin
	{

		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj += w_kl[k][numbins - 1];
		}
	}
	if (Tipo_objetivo == 3)//por área de lo metido en el bin
	{
		Expression_fobj += plate_h * plate_w;
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int j = 0; j < numpilas; j++)
				{

					for (int m = 0; m < numpiezasmax; m++)
					{

						for (int i = 0; i < numpieces; i++)
						{
							Expression_fobj += (-1) * items[i].Getitem_area() * x_imjkl[i][m][j][k][l];
							Expression_fobj += (-1) * items[i].Getitem_area() * xr_imjkl[i][m][j][k][l];
						}
					}
				}
			}
		}
	}
	if (Tipo_objetivo == 4)//por área de lo metido en el bin
	{
		//		Expression_fobj += plate_h * plate_w;
		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				for (int j = 0; j < numpilas; j++)
				{

					for (int m = 0; m < numpiezasmax; m++)
					{

						for (int i = 0; i < numpieces; i++)
						{
							if (P_WithoutPiles)
								Expression_fobj += items[i].Getitem_value() * x_imjkl[i][m][j][k][l];
							else
								Expression_fobj += items[i].Getitem_value() * h_imjkl[i][m][j][k][l];
//							Expression_fobj += items[i].Getitem_value() * xr_imjkl[i][m][j][k][l];
						}
					}
				}
			}
		}
	}
	if (Tipo_objetivo != 0 && Tipo_objetivo < 4)
		model.add(IloMinimize(env, Expression_fobj));
	if (Tipo_objetivo >= 4)
		model.add(IloMaximize(env, Expression_fobj));

	Expression_fobj.end();


	/***************************************************************/
	/*************** EJECUTAMOS EL MODELO **************************/
	/***************************************************************/
	try
	{
		printf("Aqui211");
		IloCplex cplex(model);
		printf("Versión %s", cplex.getVersion());

		//Si queremos las lazy y tenemos alguna
		if (Lazy_minimo)
		{
			cplex.addLazyConstraints(restrLazy);
		}
		//		printf("Aqui2112");
		if (G_Dibujar)
			cplex.exportModel("Roadef.lp");
		//No hace el probing
		//cplex.setParam(IloCplex::Probe, -1);
		//1 CORE
		cplex.setParam(IloCplex::Threads, nt);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, max_time); //TIEMPO EN SEGUNDOS
//		cplex.setParam(IloCplex::MIPEmphasis, 1);//
//		cplex.setParam(IloCplex::Param::Preprocessing::Presolve, 0);
//		if (Tipo_objetivo == 2)
//			cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, LastBinWidth);

		if (G_Dibujar)
			cplex.exportModel("Roadef.lp");

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

		//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();

		if (P_initial_solution)
		{


			FILE* file2;
			//	strcat(name_instance, "_solution.csv");

			file2 = fopen(P_file_to_write.c_str(), "r+");
			if (file2 == NULL)
			{
				printf("Problemas al leer el archivo %s, no se encuentra el archivo", P_file_to_write.c_str());
				exit(4);
			}
			int PLATE_ID = 0;
			int	NODE_ID = 0;
			int	X = 0;
			int Y = 0;
			int	WIDTH = 0;
			int	HEIGHT = 0;
			int TYPE = 0;
			int CUT = 0;
			int PARENT = 0;
			char kk[256];
			fscanf(file2, "%s\n", kk);
			fscanf(file2, "%d;%d;%d;%d;%d;%d;%d;%d;", &PLATE_ID, &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT);
			int section = -1;
			int level = -1;
			int stripe = -1;
			int plate = 0;
			//			printf("Solución inicial\n");
			while (!feof(file2) && CUT != (-3))
			{

				fscanf(file2, "%d;", &PLATE_ID);
				if (PLATE_ID == plate)
					fscanf(file2, "%d;%d;%d;%d;%d;%d;%d;%d", &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT, &PARENT);
				else
				{
					plate = PLATE_ID;
					stripe = -1;
					fscanf(file2, "%s\n", kk);
					continue;
				}
				plate = PLATE_ID;
				if (CUT == 2 && TYPE == (-1)) continue;
				if (CUT == 1 && TYPE == (-1)) continue;
				if (CUT == 4 && TYPE < 0) continue;
				if (CUT == 1)
				{
					stripe++;
					level = -1;
					section = 0;
					if (TYPE < 0)
						continue;
				}
				if (CUT == 2)
				{
					level++; //change 4/3/2020
					section = 0;
					if (TYPE < 0)
						continue;
					//level++;//change 4/3/2020
				}
				if (CUT == 3)
				{

					if (TYPE < 0)
					{
						if (TYPE == (-10))//cambiado por 1
							section++;
						continue;
					}
					//	else
					//	{
					//		if (level < 0) level = 0;//change 4/3/2020
					//	}

				}

				if (items[TYPE].Getitem_h() == HEIGHT)
				{
					//VAriable 
					vars.add(x_imjkl[TYPE][section][level][stripe][plate]);
					printf("%s %d w %d h %d s %d section %d level %d stripe %d plate %d\n", x_imjkl[TYPE][section][level][stripe][plate].getName(), 1, items[TYPE].Getitem_w(), items[TYPE].Getitem_h(), items[TYPE].Getitem_stack(), section, level, stripe, plate);
					vals.add(1.0);
				}
				else
				{
					//VAriable 
					vars.add(xr_imjkl[TYPE][section][level][stripe][plate]);
					printf("%s %d w %d h %d s %d section %d level %d stripe %d plate %d\n", xr_imjkl[TYPE][section][level][stripe][plate].getName(), 1, items[TYPE].Getitem_w(), items[TYPE].Getitem_h(), items[TYPE].Getitem_stack(), section, level, stripe, plate);
					vals.add(1.0);
				}
				if (CUT == 3 || CUT == 4)
				{
					section++;
				}
				//		fscanf(fin2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &W, &H, &plates, &piezas, &defectos, &plate_defectos, &obj, &width_total);
			}
			fclose(file2);

			WriteSolution();
			DibujarOpenGL();
			cplex.addMIPStart(vars, vals);

		}
		IloBool result = cplex.solve();
		//		printf("Aqui2113");
		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		//		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;

		//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


			//			FILE* f = fopen("./estadomodelo.txt", "a+");
						//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
				//				fprintf(f, "Infeasible\n");
//				printf("Infeasible\n");
				//				cplex.exportModel("Roadef.lp");
			}
			else
			{
				if (cplex.getStatus() == IloAlgorithm::Unbounded)
					//					fprintf(f, "Unbounded\n");
					printf("Unbounded\n");

				//				fprintf(f, "\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
//				printf("\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}
			//			fclose(f);
		}
		else
		{
			//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
//			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			Best_Value_Formulation = cplex.getObjValue() + 0.001;
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible
			if (cplex.getSolnPoolNsolns() > 0)
			{
				int value2 = cplex.getObjValue();
				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
				if (G_Dibujar)
				{
					for (int l = 0; l < numbins; l++)
					{
						for (int k = 0; k < numtiras; k++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								for (int m = 0; m < numpiezasmax; m++)
								{
									for (int i = 0; i < numpieces; i++)
									{


										if (cplex.isExtracted(x_imjkl[i][m][j][k][l]) && cplex.getValue(x_imjkl[i][m][j][k][l]) > 0.001)
										{
											printf("%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											//										fprintf(f, "%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
										}
										if (cplex.isExtracted(xr_imjkl[i][m][j][k][l]) && cplex.getValue(xr_imjkl[i][m][j][k][l]) > 0.001)
										{
											printf("%s %d w %d h %d s %d\n", xr_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
											//										fprintf(f, "%s %d w %d h %d s %d\n", xr_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
										}
										if (P_set_xa[i])
										{
											if (cplex.isExtracted(xa_imjkl[i][m][j][k][l]) && cplex.getValue(xa_imjkl[i][m][j][k][l]) > 0.001)
											{
												printf("%s %d w %d h %d s %d\n", xa_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
												//										fprintf(f, "%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											}
										}
										if (P_set_xar[i])
										{
											if (cplex.isExtracted(xar_imjkl[i][m][j][k][l]) && cplex.getValue(xar_imjkl[i][m][j][k][l]) > 0.001)
											{
												printf("%s %d w %d h %d s %d\n", xar_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
												//										fprintf(f, "%s %d w %d h %d s %d\n", x_imjkl[i][m][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											}
										}

									}
								}

							}
						}
					}
					//Pinto las variables w_mjkl
					for (int l = 0; l < numbins; l++)
					{
						for (int k = 0; k < numtiras; k++)
						{
							for (int j = 0; j < numpilas; j++)
							{
								for (int m = 0; m < numpiezasmax; m++)
								{
									if (cplex.isExtracted(w_mjkl[m][j][k][l]) && cplex.getValue(w_mjkl[m][j][k][l]) > 0.001)
									{
										printf("%s %2.f\n", w_mjkl[m][j][k][l].getName(), cplex.getValue(w_mjkl[m][j][k][l]));
										//								fprintf(f, "%s %2.f\n", w_mjkl[m][j][k][l].getName(), cplex.getValue(w_mjkl[m][j][k][l]));
									}
								}

							}

						}
					}
					//Pinto las variables h_jkl
					for (int l = 0; l < numbins; l++)
					{
						for (int k = 0; k < numtiras; k++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								if (cplex.isExtracted(h_jkl[j][k][l]) && cplex.getValue(h_jkl[j][k][l]) > 0.001)
								{
									printf("%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
									//								fprintf(f, "%s %2.f\n", h_jkl[j][k][l].getName(), cplex.getValue(h_jkl[j][k][l]));
								}
								if (cplex.isExtracted(ml_jkl[j][k][l]) && cplex.getValue(ml_jkl[j][k][l]) > 0.001)
								{
									printf("%s %d\n", ml_jkl[j][k][l].getName(), 1);
									//							fprintf(f, "%s %d\n", y_kl[i][j].getName(), 1);
								}

							}

						}
					}
					//Pinto las variables w_kl y y_kl
					for (int j = 0; j < numbins; j++)
					{
						for (int i = 0; i < numtiras; i++)
						{

							if (cplex.isExtracted(w_kl[i][j]) && cplex.getValue(w_kl[i][j]) > 0.001)
							{
								printf("%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
								//							fprintf(f, "%s %2.f\n", w_kl[i][j].getName(), cplex.getValue(w_kl[i][j]));
							}
							if (cplex.isExtracted(ms_kl[i][j]) && cplex.getValue(ms_kl[i][j]) > 0.001)
							{
								printf("%s %d\n", ms_kl[i][j].getName(), 1);
								//							fprintf(f, "%s %d\n", y_kl[i][j].getName(), 1);
							}

						}
					}

					//Pinto las variable y_l
					for (int l = 0; l < numbins; l++)
					{
						if (cplex.isExtracted(y_l[l]) && cplex.getValue(y_l[l]) > 0.001)
						{
							printf("%s %d\n", y_l[l].getName(), 1);
							//						fprintf(f, "%s %d\n", y_l[l].getName(), 1);
						}

					}
					//Pinto los cortes
					for (int l = 0; l < numbins; l++)
					{
						int nd = DefectsPlate[l].size();

						int d = 0;
						for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
						{
							for (int j = 0; j < numpilas; j++)
							{

								for (int k = 0; k < numtiras; k++)
								{
									for (int m = 0; m < numpiezasmax; m++)
									{
										if (cplex.isExtracted(zr_dmjkl[d][m][j][k][l]) && cplex.getValue(zr_dmjkl[d][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zr_dmjkl[d][m][j][k][l].getName(), 1);
											//										fprintf(f, "%s %d\n", zr_dmjkl[d][m][j][k][l].getName(), 1);
										}
										if (cplex.isExtracted(zrp_dmjkl[d][m][j][k][l]) && cplex.getValue(zrp_dmjkl[d][m][j][k][l]) > 0.001)
										{
											printf("%s %d\n", zrp_dmjkl[d][m][j][k][l].getName(), 1);
											//										fprintf(f, "%s %d\n", zrp_dmjkl[d][m][j][k][l].getName(), 1);
										}
										if (m == 0)
										{
											if (cplex.isExtracted(ze_djkl[d][j][k][l]) && cplex.getValue(ze_djkl[d][j][k][l]) > 0.001)
											{
												printf("%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
												//											fprintf(f, "%s %d\n", ze_djkl[d][j][k][l].getName(), 1);
											}
										}
										if (j == 0 && m == 0)
										{
											if (cplex.isExtracted(zr_dkl[d][k][l]) && cplex.getValue(zr_dkl[d][k][l]) > 0.001)
											{
												printf("%s %d\n", zr_dkl[d][k][l].getName(), 1);
												//											fprintf(f, "%s %d\n", zr_dkl[d][k][l].getName(), 1);
											}

										}

									}

								}
							}
						}

					}
					//Pinto las variables zlr,zllr, zll,zlrr
					printf("\nVariables para los defectos\n");

					for (int l = 0; l < numbins; l++)
					{
						int nd = DefectsPlate[l].size();

						int d = 0;
						for (std::list < GlassDefect > ::iterator itD = DefectsPlate[l].begin(); itD != DefectsPlate[l].end(); itD++, d++)
						{
							for (int i = 0; i < numpieces; i++)
							{
								for (int j = 0; j < numpilas; j++)
								{

									for (int k = 0; k < numtiras; k++)
									{
										for (int m = 0; m < numpiezasmax; m++)
										{
											if (d == 0)
											{
												if (cplex.isExtracted(zar_imjkl[i][m][j][k][l]) && cplex.getValue(zar_imjkl[i][m][j][k][l]) > 0.001)
												{
													printf("%s %d\n", zar_imjkl[i][m][j][k][l].getName(), 1);
													//												fprintf(f, "%s %d\n", zar_imjkl[i][m][j][k][l].getName(), 1);
												}
												if (cplex.isExtracted(za_imjkl[i][m][j][k][l]) && cplex.getValue(za_imjkl[i][m][j][k][l]) > 0.001)
												{
													printf("%s %d\n", za_imjkl[i][m][j][k][l].getName(), 1);
													//												fprintf(f, "%s %d\n", za_imjkl[i][m][j][k][l].getName(), 1);
												}
											}

										}

									}
								}
							}
						}
					}

					printf("\nVariables de mínimo\n");
					for (int i = 0; i < numpieces; i++)
					{

						for (int j = 0; j < numpilas; j++)
						{

							for (int k = 0; k < numtiras; k++)
							{
								for (int l = 0; l < numbins; l++)
								{
									if (i == 0)
									{
										for (int m = 0; m < numpiezasmax; m++)
										{
											if (cplex.isExtracted(el_mjkl[m][j][k][l]) && cplex.getValue(el_mjkl[m][j][k][l]) > 0.001)
											{
												printf("%s %d\n", el_mjkl[m][j][k][l].getName(), 1);
												//											fprintf(f, "%s %d\n", el_mjkl[m][j][k][l].getName(), 1);
											}
										}
									}
									if (i == 0)
									{
										if (cplex.isExtracted(es_jkl[j][k][l]) && cplex.getValue(es_jkl[j][k][l]) > 0.001)
										{
											printf("%s %d\n", es_jkl[j][k][l].getName(), 1);
											//										fprintf(f, "%s %d\n", es_jkl[j][k][l].getName(), 1);
										}

									}

								}
							}
						}
					}
				}
			}
			//Escribo la solución para dibujar
				//Inicio
			if (G_Dibujar || G_Draw_Exacto)
			{
				FILE* fin3;
				if (G_Draw_Exacto)
				{
					std::string str = file_idx + "_solution_" + std::to_string(G_Exacto) + "_"+ std::to_string(Tipo_objetivo) + ".txt";
					fin3=fopen(P_file_to_write.c_str(), "w+");
//					fin3 = fopen(str.c_str(), "w+");
				}
				else
					fin3 = fopen("./temp2.txt", "w+");
				int ndefects = 0;
				int nbinsdefects = 0;
				for (int i = 0; i < numbins; i++)
				{
					if (DefectsPlate[i].size() >= 1)
						nbinsdefects++;
					for (std::list< GlassDefect > ::iterator it = DefectsPlate[i].begin(); it != DefectsPlate[i].end(); it++)
					{
						ndefects++;
					}
				}
				//					fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, Best_Objective_function);
				fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, numbins, numpieces, ndefects, nbinsdefects, LB_Best_Value_Formulation, Best_Value_Formulation);
				//defectos

				for (int i = 0; i < numbins; i++)
				{

					for (std::list< GlassDefect > ::iterator it = DefectsPlate[i].begin(); it != DefectsPlate[i].end(); it++)
					{
						// Get defect coordinates.
						int defect_x = (*it).Getpos_x();
						int defect_y = (*it).Getpos_y();
						int defect_width = (*it).Getwidth();
						int defect_height = (*it).Getheight();

						fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", defect_x, defect_y, defect_width, defect_height, i);
					}
				}
				for (int i = 0; i < numbins; i++)
				{
					if (DefectsPlate[i].size() > 0)
						fprintf(fin3, "%d\t%d\n", i, DefectsPlate[i].size());

				}
				for (int l = 0; l < numbins; l++)
				{
					Best_Solution_Exacto = 0;
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{

							int pos_x = 0;
							int pos_X = 0;
							int pos_Y = 0;
							for (int k1 = 0; k1 < k; k1++)
							{
								if (cplex.isExtracted(w_kl[k1][l]) && cplex.getValue(w_kl[k1][l]) > 0.001)
								{
									pos_X += cplex.getValue(w_kl[k1][l]);
									if (pos_X > Best_Solution_Exacto)
										Best_Solution_Exacto = pos_X;
								}
							}
							int pos_y = 0;
							for (int j1 = 0; j1 < j; j1++)
							{
								if (cplex.isExtracted(h_jkl[j1][k][l]) && cplex.getValue(h_jkl[j1][k][l]) > 0.001)
								{
									pos_y += cplex.getValue(h_jkl[j1][k][l]);
								}
							}
							for (int m = 0; m < numpiezasmax; m++)
							{

								pos_X = pos_X + cplex.getValue(w_mjkl[m][j][k][l]);
								if (pos_X > Best_Solution_Exacto)
									Best_Solution_Exacto = pos_X;

								pos_Y = pos_y + cplex.getValue(h_jkl[j][k][l]);

								bool puesta = false;
								for (int i = 0; i < numpieces && puesta == false; i++)
								{
									//buscar defecto que está dentro de este rango
									int d = 0;
									bool pegado_izquierda = false;
									bool pegado_derecha = false;
									bool pegado_abajo = false;
									bool pegado_arriba = true;

									///									for (std::list< GlassDefect > ::iterator it = DefectsPlate[l].begin(); it != DefectsPlate[l].end(); it++, d++)
									//									{
														/*					if (((*it).Getpos_x() > pos_X) || ((*it).Getpos_x() + (*it).Getwidth() < pos_x)
																				|| ((*it).Getpos_y() > pos_Y) || ((*it).Getpos_y() + (*it).Getheight() < pos_y))
																			{
																				continue;
																			}
																			//este está aquí dentro
																			if ((cplex.isExtracted(zlr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlr_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zlrr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlrr_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_derecha = true;
																			}
																			if ((cplex.isExtracted(zll_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zll_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zllr_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zllr_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_izquierda = true;
																			}
																			if ((cplex.isExtracted(zla_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zla_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zlar_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zlar_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_arriba = true;
																			}
																			if ((cplex.isExtracted(zle_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zle_dimjkl[d][i][m][j][k][l]) > 0.001) ||
																				(cplex.isExtracted(zler_dimjkl[d][i][m][j][k][l]) && cplex.getValue(zler_dimjkl[d][i][m][j][k][l]) > 0.001))
																			{
																				pegado_abajo = true;
																			}

																			*/
																			//	if ((cplex.isExtracted(zu_dmjkl[d][m][j][k][l]) && cplex.getValue(zu_dmjkl[d][m][j][k][l]) > 0.001)
																			//		&& (cplex.isExtracted(zrp_dmjkl[d][m][j][k][l]) && cplex.getValue(zrp_dmjkl[d][m][j][k][l]) > 0.001) 
																			//		&& (cplex.isExtracted(zr_dmjkl[d][m][j][k][l]) && cplex.getValue(zr_dmjkl[d][m][j][k][l]) < 0.001))
																			//	{
																			//		pegado_arriba = false;
																			//	}

																			//}

									if (cplex.isExtracted(zu_mjkl[m][j][k][l]) && cplex.getValue(zu_mjkl[m][j][k][l]) > 0.001)
										pegado_arriba = false;
									pegado_izquierda = true;

									if (P_WithoutPiles)
									{
										if (cplex.isExtracted(x_imjkl[i][m][j][k][l]) && cplex.getValue(x_imjkl[i][m][j][k][l]) > 0.001)
										{
											int pos_x1 = pos_x;
											int pos_y1 = pos_y;
											if (pegado_izquierda == true)
											{
												pos_x1 = pos_X - items[i].Getitem_w();
											}
											if (pegado_arriba == true)
											{
												pos_y1 = pos_Y - items[i].Getitem_h();
											}
											fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_w(), pos_y1 + items[i].Getitem_h(), l + 1);
											//										pos_x += items[i].Getitem_w();
											puesta = true;

										}

									}
									else
									{
										if (cplex.isExtracted(h_imjkl[i][m][j][k][l]) && cplex.getValue(h_imjkl[i][m][j][k][l]) > 0.001)
										{
											int pos_x1 = pos_x;
											int pos_y1 = pos_y;
											if (pegado_izquierda == true)
											{
												pos_x1 = pos_X - items[i].Getitem_w();
											}
											if (pegado_arriba == true)
											{
												pos_y1 = pos_Y - cplex.getValue(h_imjkl[i][m][j][k][l]) * items[i].Getitem_h();
											}
											for (int i3 = 0; i3 < cplex.getValue(h_imjkl[i][m][j][k][l]); i3++)
											{
												fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_w(), pos_y1 + items[i].Getitem_h(), l + 1);
												pos_y1 += items[i].Getitem_h();
											}
											//										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_w(), pos_y1 + items[i].Getitem_h(), l + 1);
																					//										pos_x += items[i].Getitem_w();
											puesta = true;

										}
									}
									if (cplex.isExtracted(xr_imjkl[i][m][j][k][l]) && cplex.getValue(xr_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_h();
										}

										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_w();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_h(), pos_y1 + items[i].Getitem_w(), l + 1);
										//										pos_x += items[i].Getitem_h();
										puesta = true;
									}
								}
								puesta = false;
								//Ya se ha puesto alguna en la posición
								for (int i = 0; i < numpieces && puesta == false; i++)
								{
									bool pegado_izquierda = false;
									bool pegado_derecha = false;
									bool pegado_abajo = false;
									bool pegado_arriba = true;
									//No hay defecto
									pegado_arriba = true;
									pegado_izquierda = true;

									if (P_set_xa[i] && cplex.isExtracted(xa_imjkl[i][m][j][k][l]) && cplex.getValue(xa_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_w();
										}
										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_h();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_w(), pos_y1 + items[i].Getitem_h(), l + 1);
										//										pos_x += items[i].Getitem_w();
										puesta = true;

									}
									if (P_set_xar[i] && cplex.isExtracted(xar_imjkl[i][m][j][k][l]) && cplex.getValue(xar_imjkl[i][m][j][k][l]) > 0.001)
									{
										int pos_x1 = pos_x;
										int pos_y1 = pos_y;
										if (pegado_izquierda == true)
										{
											pos_x1 = pos_X - items[i].Getitem_h();
										}

										if (pegado_arriba == true)
										{
											pos_y1 = pos_Y - items[i].Getitem_w();
										}
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_h(), pos_y1 + items[i].Getitem_w(), l + 1);
										//										pos_x += items[i].Getitem_h();
										puesta = true;
									}
								}


							}
						}

					}
				}


				fclose(fin3);
				DibujarOpenGL();
			}



		}

		//}


		cplex.end();
		model.end();
		env.end();
	}
	catch (IloException& e)
	{


		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
}

////
void Glass::CalcularAreaDefectosBin()
{
	if (G_Area_Defectos_Bin.size() != 0) return;
	for (int i = 0; i < PLATES_NBR_LIMIT; i++)
	{
		int area = 0;
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[i].begin(); itD != DefectsPlate[i].end(); itD++)
		{
			area += (*itD).Getwidth() * (*itD).Getheight();
		}
		G_Area_Defectos_Bin.push_back(area);
	}
}
void Glass::CotaArea()
{
	P_BinsCotaArea = (Total_area / plate_w) / plate_h;
	double temp = (((((double)Total_area / (double)plate_w) / (double)plate_h)) - (double)P_BinsCotaArea);
	P_TrozoArea = temp * plate_h;
	int area_defects = 0;
	for (int i = 0; i <= P_BinsCotaArea; i++)
	{

		if (i < P_BinsCotaArea)
		{
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[i].begin(); itD != DefectsPlate[i].end(); itD++)
			{
				area_defects += (*itD).Getwidth() * (*itD).Getheight();
			}
		}
		else
		{
			for (std::list < GlassDefect > ::iterator itD = DefectsPlate[i].begin(); itD != DefectsPlate[i].end(); itD++)
			{
				if (((*itD).Getpos_x()) < P_TrozoArea)
					area_defects += (min((*itD).Getpos_x() + (*itD).Getwidth(), P_TrozoArea) - (*itD).Getpos_x()) * (*itD).Getheight();
			}
		}

	}

	P_BinsCotaArea = ((Total_area + area_defects) / plate_w) / plate_h;
	temp = ((((double)(Total_area + area_defects) / (double)plate_w) / (double)plate_h) - (double)P_BinsCotaArea);

	P_TrozoArea = temp * plate_w;
	int valor = 0;
	if (P_BinsCotaArea == (Best_plates_nbr - 1))
	{
//		valor = SubSetSumEntera(P_TrozoArea);
		if (valor > P_TrozoArea)
			P_TrozoArea = valor;
	}
	else
		P_TrozoArea = 0;
	//Para mejorar esta cota hago un subset que me diga la mejor combinación de piezas
	//Por encima de ese valor

}
void Glass::FormulationMultiKnapsack(short int nbins, short int nt)
{

	int kk = 9;
	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	IloRangeArray restrLazy(env);
	int maxtime = G_Time;
	IloNum max_time = maxtime; //tiempo del modelo

	char nombre[20];
	CalcularAreaDefectosBin();

	int numpieces = batch_items;
	numbins = nbins;

	int Tipo_objetivo = 0;
	if (numbins != numpieces)
	{
		Tipo_objetivo = 1;

	}
	else
		numbins = min(numpieces, PLATES_NBR_LIMIT - 1);

	NumVarMatrix2 x_ij(env, numpieces);

	NumVector y_l(env);
	int LastBinWidth = max((double)floor((Total_area - ((numbins - 1) * plate_h * plate_w)) / plate_h), (double)0);
	//	LastBinWidth = 0;
	int maxh = 0;

	/**********************************************************/
	/***************** INICIALIZO VARIABLES *******************/
	/**********************************************************/
	//inicializo la variable x_ij
	for (int i = 0; i < numpieces; i++)
	{

		x_ij[i] = IloIntVarArray(env, numbins);
		for (int j = 0; j < numbins; j++)
		{

			x_ij[i][j] = IloIntVar(env, 0, 1);
			sprintf(nombre, "x_%d_%d", i, j);
			x_ij[i][j].setName(nombre);

		}
	}



	//inicializo las variable y_l
	y_l = IloIntVarArray(env, numbins, 0, 1);
	for (int l = 0; l < numbins; l++)
	{
		sprintf(nombre, "Y%d", l);
		y_l[l].setName(nombre);
	}


	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/


	//Todos los productos colocados
	for (int i = 0; i < numpieces; i++)
	{
		IloExpr v1(env);
		for (int j = 0; j < numbins; j++)
		{




			v1 += x_ij[i][j];
			//			v1 += xr_ij[i][j];


		}
		restr.add(IloRange(env, 1, v1, 1));

		v1.end();


	}
	//Area de la piezas

	for (int j = 0; j < numbins; j++)
	{
		IloExpr v1(env);
		for (int i = 0; i < numpieces; i++)
		{





			v1 += items[i].Getitem_area() * x_ij[i][j];
			//			v1 += xr_ij[i][j];


		}
		v1 += (-1) * (plate_h * plate_w - G_Area_Defectos_Bin[j]) * y_l[j];
		restr.add(IloRange(env, -IloInfinity, v1, 0));

		v1.end();


	}
	//orden de los items

	for (int j = 0; j < numbins; j++)
	{
		for (int i = 0; i < numpieces; i++)
		{

			if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1)
			{
				IloExpr v1(env);
				v1 += x_ij[i][j];
				//solamente el penúltimo tendría valor
				if (items[i].Getitem_seq() == 1)
					continue;
				// ponemos las variables
				int i2 = i - 1;
				if (i2 >= 0 && items[i].Getitem_stack() == items[i2].Getitem_stack())
				{

					for (int j2 = 0; j2 <= j; j2++)
					{
						if (j2 == j && i2 > i) break;
						v1 += (-1) * x_ij[i2][j2];
					}

				}
				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();


			}


		}
	}

	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
	model.add(restr);


	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/

	IloExpr Expression_fobj(env);
	if (Tipo_objetivo == 0)//biobjective //estaría en diet.ccp
	{

		for (int i = 0; i < numbins; i++)
		{
			Expression_fobj += y_l[i];
		}

	}

	if (Tipo_objetivo == 1)//por bins
	{
		for (int i = 0; i < numpieces; i++)
		{
			Expression_fobj += items[i].Getitem_area() * x_ij[i][numbins - 1];
		}
	}




	model.add(IloMinimize(env, Expression_fobj));
	Expression_fobj.end();


	/***************************************************************/
	/*************** EJECUTAMOS EL MODELO **************************/
	/***************************************************************/
	try {

		IloCplex cplex(model);
		//Si queremos las lazy y tenemos alguna
//		if (Lazy==true && stack_nbr<batch_items)
	//	cplex.addLazyConstraints(restrLazy);
		//No hace el probing
		//cplex.setParam(IloCplex::Probe, -1);
		//1 CORE
		cplex.setParam(IloCplex::Threads, nt);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, max_time); //TIEMPO EN SEGUNDOS
		if (Tipo_objetivo == 2)
			cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, LastBinWidth);

		
		if (G_Dibujar)
		cplex.exportModel("RoadefKP.lp");

				//CPXsetintparam(env,CPX_PARAM_THREADS,1);

				//numero de filas y columnas del modelo
		int nr = cplex.getNrows();
		int nc = cplex.getNcols();

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();


		IloBool result = cplex.solve();

		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;

		//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if (result == false)
		{


//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//fprintf(f, "%s ", nom);
			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
//				fprintf(f, "Infeasible\n");
				//			cplex.exportModel("Roadef.lp");
			}
			else
			{
//				if (cplex.getStatus() == IloAlgorithm::Unbounded) fprintf(f, "Unbounded\n");
//				fprintf(f, "\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}
//			fclose(f);
		}
		else
		{
//			FILE* f = fopen("./estadomodelo.txt", "a+");
//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			Best_Value_Formulation = cplex.getObjValue();
			LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible
			if (cplex.getSolnPoolNsolns() > 0)
			{
				int value2 = cplex.getObjValue();
				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl

				for (int i = 0; i < numpieces; i++)
				{
					for (int j = 0; j < numbins; j++)
					{


						if (cplex.isExtracted(x_ij[i][j]) && cplex.getValue(x_ij[i][j]) > 0.001)
						{
							printf("%s %d w %d h %d s %d id %d\n", x_ij[i][j].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack(), i);
//							fprintf(f, "%s %d w %d h %d s %d id %d\n", x_ij[i][j].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack(), i);
						}

					}

				}



				//Pinto las variable y_l
				for (int l = 0; l < numbins; l++)
				{
					if (cplex.isExtracted(y_l[l]) && cplex.getValue(y_l[l]) > 0.001)
					{
						printf("%s %d\n", y_l[l].getName(), 1);
//						fprintf(f, "%s %d\n", y_l[l].getName(), 1);
					}

				}

				//estoy en el optimo, leo las variables y continuo
				if (relativeGap == 0)
				{
					int kk = 9;
				}


			}

			//}
		}

		cplex.end();
		model.end();
		env.end();
	}
	catch (IloException& e)
	{
		printf("EXCEPCION");
		std::cerr << "IloException: " << e << "Status" << e;
	}
}
void Glass::WriteSolutionCPlex(string path)
{
	/*
	FILE* file;
	//	strcat(name_instance, "_solution.csv");

	file = fopen(path.c_str(), "w+");
	if (file == NULL)
	{
		printf("Problemas al leer el archivo %s, no se encuentra el archivo", name_instance.c_str());
		exit(4);
	}
	int PLATE_ID = 0;
	int	NODE_ID = 0;
	int	X = 0;
	int Y = 0;
	int	WIDTH = 0;
	int	HEIGHT = 0;
	int TYPE = 0;
	int CUT = 0;
	int PARENT = 0;
	fscanf(fin2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &PLATE_ID, &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT);
	int section = 0;
	int level = -1;
	int stripe = -1;
	int plate = 0;
	while (!feof(file) && CUT != (-3))
	{
		
		fscanf(fin2, "%d\t", &PLATE_ID);
		if (PLATE_ID==plate)
		fscanf(fin2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t",  &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT, &PARENT);
		else
			fscanf(fin2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t", &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT);
		plate = PLATE_ID;
		if (CUT == 1)
		{
			stripe++;
			level = -1;
			section = 0;
			continue;
		}
		if (CUT == 2)
		{
			level++;
			section = 0;
			continue;
		}
		if (CUT == 3 && TYPE < 0)
		{
			section++;
			continue;
		}
		if (items[TYPE].Getitem_h == HEIGHT)
		{
			//VAriable 
			vars.add(xr_imjkl[TYPE][section][level][stripe][plate]);

		}

//		fscanf(fin2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &W, &H, &plates, &piezas, &defectos, &plate_defectos, &obj, &width_total);
	}*/
}
void Glass::CalculateMax(void)
{
	P_maxnivelesm = 0;
	P_maxnivelesh = 0;
	P_maxnivelesw = 0;
	FILE* file2;
	//	strcat(name_instance, "_solution.csv");

	file2 = fopen(P_file_to_write.c_str(), "r+");
	if (file2 == NULL)
	{
		printf("Problemas al leer el archivo %s, no se encuentra el archivo", P_file_to_write.c_str());
		exit(4);
	}
	int PLATE_ID = 0;
	int	NODE_ID = 0;
	int	X = 0;
	int Y = 0;
	int	WIDTH = 0;
	int	HEIGHT = 0;
	int TYPE = 0;
	int CUT = 0;
	int PARENT = 0;
	char kk[256];
	fscanf(file2, "%s\n", kk);
	fscanf(file2, "%d;%d;%d;%d;%d;%d;%d;%d;", &PLATE_ID, &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT);
	int section = -1;
	int level = -1;
	int stripe = -1;
	int plate = 0;

	printf("Solución inicial\n");
	while (!feof(file2) && CUT != (-3))
	{

		fscanf(file2, "%d;", &PLATE_ID);
		if (PLATE_ID == plate)
			fscanf(file2, "%d;%d;%d;%d;%d;%d;%d;%d", &NODE_ID, &X, &Y, &WIDTH, &HEIGHT, &TYPE, &CUT, &PARENT);
		else
		{
			plate = PLATE_ID;
			stripe = -1;
			fscanf(file2, "%s\n", kk);
			continue;
		}
		plate = PLATE_ID;
		if (CUT == 4 && TYPE < 0) continue;
		if (CUT == 1)
		{
			stripe++;
			level = -1;
			section = 0;
			if (TYPE < 0)
				continue;
		}
		if (CUT == 2)
		{
			level++;
			section = 0;
			if (TYPE < 0)
				continue;
		}
		if (CUT == 3)
		{

			if (TYPE < 0)
			{
				if (TYPE == (-10))//cambiado por 1
					section++;
				continue;
			}

		}
	
			if (section > P_maxnivelesm)
				P_maxnivelesm = section;
			if (level > P_maxnivelesh)
				P_maxnivelesh = level;
			if (stripe > P_maxnivelesw)
				P_maxnivelesw = stripe;

		if (CUT == 3 || CUT == 4)
		{
			section++;
		}
		//		fscanf(fin2, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &W, &H, &plates, &piezas, &defectos, &plate_defectos, &obj, &width_total);
	}
	P_maxnivelesm ++;
	P_maxnivelesh ++;
	P_maxnivelesw ++;
	fclose(file2);

//	WriteSolution();
//	DibujarOpenGL();
//	cplex.addMIPStart(vars, vals);
}
