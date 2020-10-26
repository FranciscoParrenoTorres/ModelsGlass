#include "Glass.h"

#pragma warning(disable:4786)


#include <assert.h>
#include <sys/types.h>
#include <sys/timeb.h>


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

void Glass::FormulacionCompleta3Indices(short int nbins, short int nt, bool Lazy_inimo, bool Lazy_cortes)
{

	//Compute the number of pieces
	//P_Set_ar y P_Set_xar
	int trozo = plate_w;
	if (numbins <= 1)
	{
		trozo = Best_Width;
	}
	maxTiras = P_maxnivelesw;
//	int maxTiras = MultiKnapsackEntera(trozo);
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
	Lazy_inimo = false;
	Lazy_cortes = false;
	int Lazy_orden = false;
	//	knapsack_bouknap_tiras();
		//	Creo las variables

	numtiras = P_maxnivelesw;
	numpilas = P_maxnivelesh;
	int numpiezasmax = P_maxnivelesm;
	int numpieces = batch_items;
	numbins = nbins;



	int Tipo_objetivo = 2;
	//max_numero de defectos
	int max_nd = 1;

	printf("Aqui21");



	//inimo defecto pilas


	BoolVarMatrix4 el_ijkl(env, numpieces);
	BoolVarMatrix3 el_jkl(env, numpilas);
	BoolVarMatrix4 es_ijkl(env, numpieces);
	BoolVarMatrix3 ms_jkl(env, numpilas);

	BoolVarMatrix3 ml_jkl(env, numpilas);
	BoolVarMatrix2 ms_kl(env, numtiras);

	NumVarMatrix3 h_jkl(env, numpilas);

	BoolVarMatrix4 x_ijkl(env, numpieces);
	BoolVarMatrix4 xr_ijkl(env, numpieces);

	//Variables para el triming
	BoolVarMatrix5 xa_i1i2jkl(env, numpieces);
	BoolVarMatrix5 xar_i1i2jkl(env, numpieces);


	BoolVarMatrix2 y_kl(env, numtiras);
	BoolVarMatrix3 es_jkl(env, numpilas);

	BoolVarMatrix2 esH_kl(env, numtiras);
	BoolVector esW_l(env);

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
	//inicializo la variable x_ijkl 
	//inicializo variables rotadas

	
	for (int i = 0; i < numpieces; i++)
	{
		x_ijkl[i] = BoolVarMatrix3(env, numpilas);
		xr_ijkl[i] = BoolVarMatrix3(env, numpilas);
		el_ijkl[i] = BoolVarMatrix3(env, numpilas);
		for (int j = 0; j < numpilas; j++)
		{
			x_ijkl[i][j] = BoolVarMatrix(env, numtiras);
			xr_ijkl[i][j] = BoolVarMatrix(env, numtiras);
			el_ijkl[i][j] = BoolVarMatrix(env, numtiras);
			for (int k = 0; k < numtiras; k++)
			{
				x_ijkl[i][j][k] = IloBoolVarArray(env, numbins);
				xr_ijkl[i][j][k] = IloBoolVarArray(env, numbins);
				el_ijkl[i][j][k] = IloBoolVarArray(env, numbins);
				for (int l = 0; l < numbins; l++)
				{
					x_ijkl[i][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "x_%d_%d_%d_%d", i, j, k, l);
					x_ijkl[i][j][k][l].setName(nombre);

					xr_ijkl[i][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "xr_%d_%d_%d_%d", i, j, k, l);
					xr_ijkl[i][j][k][l].setName(nombre);

					el_ijkl[i][j][k][l] = IloBoolVar(env);
					sprintf(nombre, "Xel_%d_%d_%d_%d", i, j, k, l);
					el_ijkl[i][j][k][l].setName(nombre);

				}


			}
		}
	}
	if (P_trimming)
	{
		//Variables trimming
		int m = 0;
		for (int i1 = 0; i1 < numpieces; i1++)
		{

			xa_i1i2jkl[i1] = BoolVarMatrix4(env, numpieces);
			xar_i1i2jkl[i1] = BoolVarMatrix4(env, numpieces);


			for (int i2 = 0; i2 < numpieces; i2++)
			{


				xa_i1i2jkl[i1][i2] = BoolVarMatrix3(env, numpilas);
				xar_i1i2jkl[i1][i2] = BoolVarMatrix3(env, numpilas);

				for (int j = 0; j < numpilas; j++)
				{
					xa_i1i2jkl[i1][i2][j] = BoolVarMatrix(env, numtiras);
					xar_i1i2jkl[i1][i2][j] = BoolVarMatrix(env, numtiras);
					for (int k = 0; k < numtiras; k++)
					{
						xa_i1i2jkl[i1][i2][j][k] = IloBoolVarArray(env, numbins);
						xar_i1i2jkl[i1][i2][j][k] = IloBoolVarArray(env, numbins);

						for (int l = 0; l < numbins; l++)
						{
							xa_i1i2jkl[i1][i2][j][k][l] = IloBoolVar(env);
							xar_i1i2jkl[i1][i2][j][k][l] = IloBoolVar(env);
							sprintf(nombre, "xa_%d_%d_%d_%d_%d", i1, i2, j, k, l);
							xa_i1i2jkl[i1][i2][j][k][l].setName(nombre);
							sprintf(nombre, "xar_%d_%d_%d_%d_%d", i1, i2, j, k, l);
							xar_i1i2jkl[i1][i2][j][k][l].setName(nombre);

						}


					}
				}
			}
		}
	}
	//inicializo las variables de cada tira
	for (int j = 0; j < numpilas; j++)
	{

		es_jkl[j] = BoolVarMatrix(env, numtiras);
		el_jkl[j] = BoolVarMatrix(env, numtiras);
		for (int k = 0; k < numtiras; k++)
		{

			es_jkl[j][k] = IloBoolVarArray(env, numbins);
			el_jkl[j][k] = IloBoolVarArray(env, numbins);
			for (int l = 0; l < numbins; l++)
			{
				int m = 0;
				es_jkl[j][k][l] = IloBoolVar(env);
				sprintf(nombre, "ms_%d_%d_%d_%d", m, j, k, l);
				es_jkl[j][k][l].setName(nombre);
				el_jkl[j][k][l] = IloBoolVar(env);
				sprintf(nombre, "Xel_%d_%d_%d_%d", m, j, k, l);
				el_jkl[j][k][l].setName(nombre);
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

	//inimco corte


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



	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/

	//	Lazy_inimo = false;
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
					if (P_set_xa[i])
					{
						for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
						{
							int i2 = (*it);
							v1 += xa_i1i2jkl[i][i2][j][k][l];
						}
						for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
						{
							int i2 = (*it);
							v1 += xa_i1i2jkl[i][i2][j][k][l];
						}
					}
					if (P_set_xar[i])
					{
						for (std::set<int> ::iterator it = Dxy[i].begin(); it != Dxy[i].end(); it++)
						{
							int i2 = (*it);
							v1 += xar_i1i2jkl[i][i2][j][k][l];
						}
						for (std::set<int> ::iterator it = Dyy[i].begin(); it != Dyy[i].end(); it++)
						{
							int i2 = (*it);
							v1 += xar_i1i2jkl[i][i2][j][k][l];
						}
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
	//Restricciones para el ORDEN de los triming
	//CONSTRAINTS ORDERS TRIMiNG
	if (P_trimming)
	{
		for (int j = 0; j < numpilas; j++)
		{
			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{

					for (int i = 0; i < numpieces; i++)
					{


						if (P_set_xa[i])
						{
							for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
							{
								int i2 = (*it);
								IloExpr v1(env);
								v1 += xa_i1i2jkl[i][i2][j][k][l];
								v1 += (-1) * x_ijkl[i2][j][k][l];

								restr.add(IloRange(env, -IloInfinity, v1, 0));
								v1.end();
							}
							for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
							{
								int i2 = (*it);
								IloExpr v1(env);
								v1 += xa_i1i2jkl[i][i2][j][k][l];
								v1 += (-1) * xr_ijkl[i2][j][k][l];

								restr.add(IloRange(env, -IloInfinity, v1, 0));
								v1.end();
							}

						}
						if (P_set_xar[i])
						{
							for (std::set<int> ::iterator it = Dxy[i].begin(); it != Dxy[i].end(); it++)
							{
								int i2 = (*it);
								IloExpr v1(env);
								v1 += xar_i1i2jkl[i][i2][j][k][l];
								v1 += (-1) * x_ijkl[i2][j][k][l];

								restr.add(IloRange(env, -IloInfinity, v1, 0));
								v1.end();
							}
							for (std::set<int> ::iterator it = Dyy[i].begin(); it != Dyy[i].end(); it++)
							{
								int i2 = (*it);
								IloExpr v1(env);
								v1 += xar_i1i2jkl[i][i2][j][k][l];
								v1 += (-1) * xr_ijkl[i2][j][k][l];

								restr.add(IloRange(env, -IloInfinity, v1, 0));
								v1.end();
							}

						}


					}
				}
			}

		}
	}
		//solamente una por celda
	//	Lazy_inimo=false;
		//Altura mínima altura
	//CONSTRAINTS HEIGHT
	for (int j = 0; j < numpilas; j++)
	{

		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{




				for (int i = 0; i < numpieces; i++)
				{
					IloExpr v1(env);
					IloExpr v2(env);
					v1 += items[i].Getitem_h() * x_ijkl[i][j][k][l];
					v1 += items[i].Getitem_w() * xr_ijkl[i][j][k][l];
					v2 += items[i].Getitem_h() * x_ijkl[i][j][k][l];
					v2 += items[i].Getitem_w() * xr_ijkl[i][j][k][l];

					if (P_set_xpa[i] || P_set_xrpa[i])
					{
						for (std::set<int> ::iterator it = Ax[i].begin(); it != Ax[i].end(); it++)
						{
							
							v1 += items[(*it)].Getitem_h() * xa_i1i2jkl[(*it)][i][j][k][l];
							v2 += items[(*it)].Getitem_h() * xa_i1i2jkl[(*it)][i][j][k][l];

						}
	
						for (std::set<int> ::iterator it = Ay[i].begin(); it != Ay[i].end(); it++)
						{
	
							v1 += items[(*it)].Getitem_w() * xar_i1i2jkl[(*it)][i][j][k][l];
							v2 += items[(*it)].Getitem_w() * xar_i1i2jkl[(*it)][i][j][k][l];
						}

					}



					v1 += (-1) * h_jkl[j][k][l];
					if (P_minimo_waste)
						v1 += waste_min * (1 - el_ijkl[i][j][k][l]);

					restr.add(IloRange(env, -IloInfinity, v1, 0));
					v1.end();
					v2 += (-1) * h_jkl[j][k][l];
					if (P_minimo_waste)
					{
						v2 += plate_h * (1 - el_ijkl[i][j][k][l]);
						if (Lazy_inimo)
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
			IloExpr v2(env);
			IloExpr v3(env);
			for (int j = 0; j < numpilas; j++)
			{


				v1 += h_jkl[j][k][l];
				v2 += h_jkl[j][k][l];
				v3 += h_jkl[j][k][l];
			}
			v1 += (-1) * plate_h * y_kl[k][l];
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
			v2 += (-1) * plate_h;
			v2 += (1- esH_kl[k][l])*waste_min;
			restr.add(IloRange(env, -IloInfinity, v2, 0));
			v2.end();
			v3+= (-1) * plate_h;
			v3 += (1 - esH_kl[k][l]) * plate_h;
			restr.add(IloRange(env, 0, v3, IloInfinity));
			v3.end();
		}
	}
	//CONSTRAINTS iNIiZAR TIRAS
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
					v1 += (-1) * x_ijkl[i][j][k][0];
					v1 += (-1) * xr_ijkl[i][j][k][0];

				}
			}
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
		}

	}

	// CONSTRAINT TRIMiNG
	//Si pongo un triing la altura tiene que ser perfecta
	if (P_trimming)
	{
		for (int j = 0; j < numpilas; j++)
		{

			for (int k = 0; k < numtiras; k++)
			{
				for (int l = 0; l < numbins; l++)
				{


					

					for (int i = 0; i < numpieces; i++)
					{
						IloExpr v1(env);
						IloExpr v2(env);
						bool meter = false;
						if (P_set_xpa[i] || P_set_xrpa[i])
						{
							for (std::set<int> ::iterator it = Ax[i].begin(); it != Ax[i].end(); it++)
							{
								meter = true;
								v1 +=  xa_i1i2jkl[(*it)][i][j][k][l];
								v2 += xa_i1i2jkl[(*it)][i][j][k][l];

							}

							for (std::set<int> ::iterator it = Ay[i].begin(); it != Ay[i].end(); it++)
							{
								meter = true;
								v1 +=  xar_i1i2jkl[(*it)][i][j][k][l];
								v2 += xar_i1i2jkl[(*it)][i][j][k][l];

							}

						}


						if (meter == true)
						{
							v1 += (-1) * el_ijkl[i][j][k][l];
							restr.add(IloRange(env, -IloInfinity, v1, 0));
							
							v1.end();
							restr.add(IloRange(env, -IloInfinity, v2, 1));

							v2.end();
						}

						//Como máximo una de trimming

					}



				}
			}
		}

	}
	//CONSTRAINT WIDTH

	//	Lazy_inimo = false;
		//Anchura de los cortes en cada pila
		//inimo corte consecutivo en los de tres
	/*
	for (int j = 0; j < numpilas; j++)
	{

		for (int k = 0; k < numtiras; k++)
		{
			for (int l = 0; l < numbins; l++)
			{
				IloExpr v1(env);
				IloExpr v2(env);

				for (int i = 0; i < numpieces; i++)
				{


					v1 += items[i].Getitem_w() * x_ijkl[i][j][k][l];
					v1 += items[i].Getitem_h() * xr_ijkl[i][j][k][l];
					v2 += items[i].Getitem_w() * x_ijkl[i][j][k][l];
					v2 += items[i].Getitem_h() * xr_ijkl[i][j][k][l];

				}
				v1 += (-1) * w_jkl[j][k][l];
				if (P_minimo_waste)
					v1 += (1 - es_jkl[j][k][l]) * waste_min;
				v2 += (-1) * w_jkl[j][k][l];
				v2 += (1 - es_jkl[j][k][l]) * max1Cut;

				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
				if (P_minimo_waste)
				{
					if (Lazy_inimo)
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));
					else
						restr.add(IloRange(env, 0, v2, IloInfinity));
				}
				v2.end();




			}
		}

	}
	*/
	//CONSTRAINT WIDHT
/**/
//suma de inimo corte anchos de las tiras
	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{
			for (int j = 0; j < numpilas; j++)
			{

				IloExpr v1(env);
				IloExpr v2(env);
//				v1 += w_jkl[j][k][l];
//				v2 += w_jkl[j][k][l];
				for (int i = 0; i < numpieces; i++)
				{


					v1 += items[i].Getitem_w() * x_ijkl[i][j][k][l];
					v1 += items[i].Getitem_h() * xr_ijkl[i][j][k][l];
					v2 += items[i].Getitem_w() * x_ijkl[i][j][k][l];
					v2 += items[i].Getitem_h() * xr_ijkl[i][j][k][l];

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
					if (Lazy_inimo)
						restrLazy.add(IloRange(env, 0, v2, IloInfinity));

					else
						restr.add(IloRange(env, 0, v2, IloInfinity));
				}
				v2.end();
			}

		}
	}
	


	//inimos cortes
		//suma de las alturas de las pilas menor H

	//	Lazy_inimo = true;

	for (int k = 0; k < numtiras; k++)
	{
		for (int l = 0; l < numbins; l++)
		{

			for (int j = 0; j < numpilas; j++)
			{
				//				Lazy_inimo = true;
				IloExpr v1(env);
				v1 += min2Cut * ml_jkl[j][k][l];

				v1 += (-1) * h_jkl[j][k][l];

				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
				IloExpr v2(env);
				v2 += plate_h * ml_jkl[j][k][l];
				v2 += (-1) * h_jkl[j][k][l];
				if (Lazy_inimo)
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


			}
			//			Lazy_inimo = false;
			IloExpr v1(env);
			v1 += min1Cut * ms_kl[k][l];

			v1 += (-1) * w_kl[k][l];

			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
			IloExpr v2(env);
			v2 += max1Cut * ms_kl[k][l];
			v2 += (-1) * w_kl[k][l];
			if (Lazy_inimo)
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
		v2 += (-1) * plate_w ;
		v2 += (1-  esW_l[l])*waste_min;
		restr.add(IloRange(env, -IloInfinity, v2, 0));
		v2.end();
		v3 += (-1) * plate_w;
		v3 += (1 - esW_l[l]) * plate_w;
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

					for (int i = numpieces - 1; i > 0; i--)
					{


						if ((stacks[items[i].Getitem_stack()].Getitem_nbr()) > 1 && (items[i].Getitem_seq() > 1))
						{
							int kk = stacks[items[i].Getitem_stack()].Getitem_nbr();
							IloExpr v1(env);
							v1 += x_ijkl[i][j][k][l];
							v1 += xr_ijkl[i][j][k][l];
							if (P_set_xa[i])
							{
								for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
								{

										v1 += xa_i1i2jkl[i][(*it)][j][k][l];
								}
								for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
								{

									v1 += xa_i1i2jkl[i][(*it)][j][k][l];
								}
							}
							if (P_set_xar[i])
							{
								for (std::set<int> ::iterator it = Dxy[i].begin(); it != Dxy[i].end(); it++)
								{

									v1 += xa_i1i2jkl[i][(*it)][j][k][l];
								}
								for (std::set<int> ::iterator it = Dyy[i].begin(); it != Dyy[i].end(); it++)
								{

									v1 += xar_i1i2jkl[i][(*it)][j][k][l];
								}
							}



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
							

												v1 += (-1) * x_ijkl[i2][j2][k2][l2];
												v1 += (-1) * xr_ijkl[i2][j2][k2][l2];
												if (P_set_xa[i2])
												{
													for (std::set<int> ::iterator it = Dxx[i2].begin(); it != Dxx[i2].end(); it++)
													{

														
															v1 += (-1) * xa_i1i2jkl[i2][(*it)][j2][k2][l2];
													}
													for (std::set<int> ::iterator it = Dyx[i2].begin(); it != Dyx[i2].end(); it++)
													{


														v1 += (-1) * xar_i1i2jkl[i2][(*it)][j2][k2][l2];
													}
												}
												if (P_set_xar[i2])
												{
													for (std::set<int> ::iterator it = Dxy[i2].begin(); it != Dxy[i2].end(); it++)
													{


														v1 += (-1) * xa_i1i2jkl[i2][(*it)][j2][k2][l2];
													}
													for (std::set<int> ::iterator it = Dyy[i2].begin(); it != Dyy[i2].end(); it++)
													{


														v1 += (-1) * xar_i1i2jkl[i2][(*it)][j2][k2][l2];
													}
												}

											}
										}

	
								}
/*
								for (std::set<int> ::iterator it = Dx[i].begin(); it != Dx[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
										v1 += (-1) * x_ijkl[(*it)][j][k][l];
								}
								for (std::set<int> ::iterator it = Dy[i].begin(); it != Dy[i].end(); it++)
								{
									if ((*it) == (i - 1) && items[i].Getitem_stack() == items[(*it)].Getitem_stack())
										v1 += (-1) * xr_ijkl[(*it)][j][k][l];
								}
*/
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
	bool P_Mejorar_Sol = false;

	if (P_cota)
	{
		if ((Tipo_objetivo == 2 && ((P_TrozoArea < 6000) && (numbins==P_BinsCotaArea+1))))
		{
			IloExpr v4(env);
			IloExpr v5(env);
			for (int k = 0; k < numtiras; k++)
			{



				v4 += w_kl[k][numbins - 1];
				v5 += w_kl[k][numbins - 1];

			}
			v4 += (-1) * P_TrozoArea;
			restr.add(IloRange(env, 0, v4, IloInfinity));
			v4.end();
			if (P_Mejorar_Sol)
			{
				v5 += (-1) * Best_Width;
				restr.add(IloRange(env, -IloInfinity, v5, 0));
				v5.end();
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


					for (int i = 0; i < numpieces; i++)
					{
						Expression_fobj += (-1) * items[i].Getitem_area() * x_ijkl[i][j][k][l];
						Expression_fobj += (-1) * items[i].Getitem_area() * xr_ijkl[i][j][k][l];
					}
				}
			}
		}
	}

	if (Tipo_objetivo != 0 && Tipo_objetivo != 4)
		model.add(IloMinimize(env, Expression_fobj));
	if (Tipo_objetivo == 4)
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
		if (Lazy_inimo)
		{
			cplex.addLazyConstraints(restrLazy);
		}
		printf("Aqui2112");
//				cplex.exportModel("Roadef.lp");
				//No hace el probing
//				cplex.setParam(IloCplex::Probe, -1);
				//1 CORE
		cplex.setParam(IloCplex::Threads, nt);
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//liite de tiempo
		cplex.setParam(IloCplex::TiLim, max_time); //TIEMPO EN SEGUNDOS
	//		if (Tipo_objetivo == 2)
	//			cplex.setParam(IloCplex::Param::Simplex::Liits::LowerObj, LastBinWidth);
//		cplex.exportModel("Roadef.lp");
		if (G_Dibujar)
			cplex.exportModel("Roadef.lp");

			//CPXsetintparam(env,CPX_PARAM_THREADS,1);

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
				if (CUT >= 4 && TYPE < 0) continue;
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
				if (items[TYPE].Getitem_h() == HEIGHT)
				{
					//VAriable 
					vars.add(x_ijkl[TYPE][level][stripe][plate]);
					vals.add(1.0);
				}
				else
				{
					//VAriable 
					vars.add(xr_ijkl[TYPE][level][stripe][plate]);
					vals.add(1.0);
				}
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
		printf("Aqui2113");
		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		printf("%2f \n", secs);

		int vol = 0;
		//		int invol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
		int candidato = 0;

		//Estado del algoritmo	al terinar
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
				printf("Infeasible\n");
				//				cplex.exportModel("Roadef.lp");
			}
			else
			{
				if (cplex.getStatus() == IloAlgorithm::Unbounded)
					//					fprintf(f, "Unbounded\n");
					printf("Unbounded\n");

				//				fprintf(f, "\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
				printf("\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}
			//			fclose(f);
		}
		else
		{
			//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			Best_Value_Formulation = cplex.getObjValue()+0.0001;
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

								for (int i = 0; i < numpieces; i++)
								{


									if (cplex.isExtracted(x_ijkl[i][j][k][l]) && cplex.getValue(x_ijkl[i][j][k][l]) > 0.001)
									{
										printf("%s %d w %d h %d s %d\n", x_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
										//										fprintf(f, "%s %d w %d h %d s %d\n", x_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
									}
									if (cplex.isExtracted(xr_ijkl[i][j][k][l]) && cplex.getValue(xr_ijkl[i][j][k][l]) > 0.001)
									{
										printf("%s %d w %d h %d s %d\n", xr_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
										//										fprintf(f, "%s %d w %d h %d s %d\n", xr_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack());
									}
									if (P_set_xa[i])
									{
										for (std::set<int> ::iterator it = Dxx[i].begin(); it != Dxx[i].end(); it++)
										{
											if (cplex.isExtracted(xa_i1i2jkl[i][(*it)][j][k][l]) && cplex.getValue(xa_i1i2jkl[i][(*it)][j][k][l]) > 0.001)
											{
												printf("%s %d w %d h %d s %d\n", xa_i1i2jkl[i][(*it)][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
												//										fprintf(f, "%s %d w %d h %d s %d\n", x_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											}
										}
										for (std::set<int> ::iterator it = Dyx[i].begin(); it != Dyx[i].end(); it++)
										{
											if (cplex.isExtracted(xa_i1i2jkl[i][(*it)][j][k][l]) && cplex.getValue(xa_i1i2jkl[i][(*it)][j][k][l]) > 0.001)
											{
												printf("%s %d w %d h %d s %d\n", xa_i1i2jkl[i][(*it)][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
												//										fprintf(f, "%s %d w %d h %d s %d\n", x_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											}
										}
									}
									if (P_set_xar[i])
									{
										for (std::set<int> ::iterator it = Dxy[i].begin(); it != Dxy[i].end(); it++)
										{
											if (cplex.isExtracted(xar_i1i2jkl[i][(*it)][j][k][l]) && cplex.getValue(xar_i1i2jkl[i][(*it)][j][k][l]) > 0.001)
											{
												printf("%s %d w %d h %d s %d\n", xar_i1i2jkl[i][(*it)][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
												//										fprintf(f, "%s %d w %d h %d s %d\n", x_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											}
										}
										for (std::set<int> ::iterator it = Dyy[i].begin(); it != Dyy[i].end(); it++)
										{
											if (cplex.isExtracted(xar_i1i2jkl[i][(*it)][j][k][l]) && cplex.getValue(xar_i1i2jkl[i][(*it)][j][k][l]) > 0.001)
											{
												printf("%s %d w %d h %d s %d\n", xar_i1i2jkl[i][(*it)][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
												//										fprintf(f, "%s %d w %d h %d s %d\n", x_ijkl[i][j][k][l].getName(), 1, items[i].Getitem_w(), items[i].Getitem_h(), items[i].Getitem_stack());
											}
										}
									}

								}

							}
						}
					}
					//Pinto las variables w_jkl
	/*				for (int l = 0; l < numbins; l++)
					{
						for (int k = 0; k < numtiras; k++)
						{
							for (int j = 0; j < numpilas; j++)
							{
								if (cplex.isExtracted(w_jkl[j][k][l]) && cplex.getValue(w_jkl[j][k][l]) > 0.001)
								{
									printf("%s %2.f\n", w_jkl[j][k][l].getName(), cplex.getValue(w_jkl[j][k][l]));
									//								fprintf(f, "%s %2.f\n", w_jkl[j][k][l].getName(), cplex.getValue(w_jkl[j][k][l]));
								}

							}

						}
					}*/
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
										if (cplex.isExtracted(el_jkl[j][k][l]) && cplex.getValue(el_jkl[j][k][l]) > 0.001)
										{
											printf("%s %d\n", el_jkl[j][k][l].getName(), 1);
											//											fprintf(f, "%s %d\n", el_jkl[j][k][l].getName(), 1);
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
					fin3 = fopen(str.c_str(), "w+");
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
					int width = 0;
					for (int k1 = 0; k1 < numtiras; k1++)
					{
						if (cplex.isExtracted(w_kl[k1][l]) && cplex.getValue(w_kl[k1][l]) > 0.001)
						{
							width += cplex.getValue(w_kl[k1][l]);
							if (width > Best_Solution_Exacto)
								Best_Solution_Exacto = width;
						}
					}
					for (int k = 0; k < numtiras; k++)
					{
						for (int j = 0; j < numpilas; j++)
						{
							//PAra dibujar primero hay que ver cúantos en el mismo hueco
							//comparten pila estos por orden, si no hay pilas repetidas, da igual
							//el primero es su lugar
							bool mas2stack = false;
							std::vector< std::set< int> > temp_stacks;
							for (int i = 0; i < stack_nbr; i++)
							{
								std::set<int> temp;
								temp_stacks.push_back(temp);
							}
							for (int i = 0; i < numpieces; i++)
							{
								if (((cplex.isExtracted(x_ijkl[i][j][k][l]) && cplex.getValue(x_ijkl[i][j][k][l]) > 0.001)) ||
									((cplex.isExtracted(xr_ijkl[i][j][k][l]) && cplex.getValue(xr_ijkl[i][j][k][l]) > 0.001)) )
				
								temp_stacks[items[i].Getitem_stack()].insert(items[i].Getitem_id());
								for (std::set<int> ::iterator it = Ax[i].begin(); it != Ax[i].end(); it++)
								{
									if ((cplex.isExtracted(xa_i1i2jkl[(*it)][i][j][k][l]) && cplex.getValue(xa_i1i2jkl[(*it)][i][j][k][l]) > 0.001))
										temp_stacks[items[(*it)].Getitem_stack()].insert(items[(*it)].Getitem_id());


								}
								for (std::set<int> ::iterator it = Ay[i].begin(); it != Ay[i].end(); it++)
								{
									if ((cplex.isExtracted(xar_i1i2jkl[(*it)][i][j][k][l]) && cplex.getValue(xar_i1i2jkl[(*it)][i][j][k][l]) > 0.001))
										temp_stacks[items[(*it)].Getitem_stack()].insert(items[(*it)].Getitem_id());

								}
								if (temp_stacks[items[i].Getitem_stack()].size() > 1)
									mas2stack = true;
							}
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
							pos_x = pos_X;
							
							pos_X = pos_X + cplex.getValue(w_kl[k][l]);


							pos_Y = pos_y + cplex.getValue(h_jkl[j][k][l]);

							bool puesta = false;
							std::set<int> set_puestas;

							for (std::vector< std::set <int > > ::iterator it = temp_stacks.begin(); it != temp_stacks.end(); it++)
							{
								for (std::set<int> ::iterator it2 = (*it).begin(); it2 != (*it).end(); it2++)
								{
									int i = (*it2);
									bool pegado_izquierda = false;
									bool pegado_derecha = false;
									bool pegado_abajo = false;
									bool pegado_arriba = true;

									pegado_arriba = false;
									pegado_izquierda = false;

									//Primero la de abajo, 
									if (cplex.isExtracted(x_ijkl[i][j][k][l]) && cplex.getValue(x_ijkl[i][j][k][l]) > 0.001)
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

										//mirar si hay trimming
										for (std::set<int> ::iterator it = Ax[(*it2)].begin(); it != Ax[(*it2)].end(); it++)
										{
											if (cplex.isExtracted(xa_i1i2jkl[(*it)][i][j][k][l]) && cplex.getValue(xa_i1i2jkl[(*it)][i][j][k][l]) > 0.001)
											{
												int pos_y2= pos_Y - items[(*it)].Getitem_h();
												fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it), items[(*it)].Getitem_stack(), items[(*it)].Getitem_seq(), pos_x1, pos_y2, pos_x1 + items[(*it)].Getitem_w(), pos_Y, l + 1);
												temp_stacks[items[(*it)].Getitem_stack()].erase(items[(*it)].Getitem_id());

											}

										}
										for (std::set<int> ::iterator it = Ay[(*it2)].begin(); it != Ay[(*it2)].end(); it++)
										{
											if (cplex.isExtracted(xar_i1i2jkl[(*it)][i][j][k][l]) && cplex.getValue(xar_i1i2jkl[(*it)][i][j][k][l]) > 0.001)
											{
												int pos_y2 = pos_Y - items[(*it)].Getitem_w();
												fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it), items[(*it)].Getitem_stack(), items[(*it)].Getitem_seq(), pos_x1, pos_y2, pos_x1 + items[(*it)].Getitem_h(), pos_Y, l + 1);
												temp_stacks[items[(*it)].Getitem_stack()].erase(items[(*it)].Getitem_id());
											}

										}
										if (pegado_izquierda==false)
											pos_x+= items[i].Getitem_w();
										continue;
										//										pos_x += items[i].Getitem_w();
									}
									if (cplex.isExtracted(xr_ijkl[i][j][k][l]) && cplex.getValue(xr_ijkl[i][j][k][l]) > 0.001)
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
										for (std::set<int> ::iterator it = Ax[(*it2)].begin(); it != Ax[(*it2)].end(); it++)
										{
											if (cplex.isExtracted(xa_i1i2jkl[(*it)][i][j][k][l]) && cplex.getValue(xa_i1i2jkl[(*it)][i][j][k][l]) > 0.001)
											{
												int pos_y2 = pos_Y - items[(*it)].Getitem_h();
										
												fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it), items[(*it)].Getitem_stack(), items[(*it)].Getitem_seq(), pos_x1, pos_y2, pos_x1 + items[(*it)].Getitem_w(), pos_Y, l + 1);
												temp_stacks[items[(*it)].Getitem_stack()].erase(items[(*it)].Getitem_id());
											}

										}
										for (std::set<int> ::iterator it = Ay[(*it2)].begin(); it != Ay[(*it2)].end(); it++)
										{
											if (cplex.isExtracted(xar_i1i2jkl[(*it)][i][j][k][l]) && cplex.getValue(xar_i1i2jkl[(*it)][i][j][k][l]) > 0.001)
											{
												int pos_y2 = pos_Y - items[(*it)].Getitem_w();
												fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it), items[(*it)].Getitem_stack(), items[(*it)].Getitem_seq(), pos_x1, pos_y2, pos_x1 + items[(*it)].Getitem_h(), pos_Y, l + 1);
												temp_stacks[items[(*it)].Getitem_stack()].erase(items[(*it)].Getitem_id());
											}

										}
										
										if (pegado_izquierda == false)
											pos_x += items[i].Getitem_h();
										continue;

									}
									//La de arriba, 
									for (std::set<int> ::iterator it = Ax[(*it2)].begin(); it != Ax[(*it2)].end(); it++)
									{

										if (cplex.isExtracted(xa_i1i2jkl[i][(*it)][j][k][l]) && cplex.getValue(xa_i1i2jkl[i][(*it)][j][k][l]) > 0.001)
										{
											int pos_x1 = pos_x;
											int pos_y1 = pos_y;
											pegado_arriba = true;
											if (pegado_izquierda == true)
											{
												pos_x1 = pos_X - items[i].Getitem_w();
											}

											if (pegado_arriba == true)
											{
												pos_y1 = pos_Y - items[i].Getitem_h();
											}
											fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_w(), pos_y1 + items[i].Getitem_h(), l + 1);

											//mirar si hay trimming
												if (cplex.isExtracted(x_ijkl[(*it)][j][k][l]) && cplex.getValue(x_ijkl[(*it)][j][k][l]) > 0.001)
												{
													int pos_y1 = pos_y;
													fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it), items[(*it)].Getitem_stack(), items[(*it)].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[(*it)].Getitem_w(), pos_y1 + items[(*it)].Getitem_h(), l + 1);
													temp_stacks[items[(*it)].Getitem_stack()].erase(items[(*it)].Getitem_id());
												}
												if (cplex.isExtracted(xr_ijkl[(*it)][j][k][l]) && cplex.getValue(xr_ijkl[(*it)][j][k][l]) > 0.001)
												{
													int pos_y1 = pos_y;
													fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it), items[(*it)].Getitem_stack(), items[(*it)].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[(*it)].Getitem_h(), pos_y1 + items[(*it)].Getitem_w(), l + 1);
													temp_stacks[items[(*it)].Getitem_stack()].erase(items[(*it)].Getitem_id());
												}

						
											if (pegado_izquierda == false)
												pos_x += items[i].Getitem_w();
											//										pos_x += items[i].Getitem_w();
										}
										for (std::set<int> ::iterator it = Ay[(*it2)].begin(); it != Ay[(*it2)].end(); it++)
										{

											if (cplex.isExtracted(xar_i1i2jkl[i][(*it)][j][k][l]) && cplex.getValue(xar_i1i2jkl[i][(*it)][j][k][l]) > 0.001)
											{
												int pos_x1 = pos_x;
												int pos_y1 = pos_y;
												pegado_arriba = true;
												if (pegado_izquierda == true)
												{
													pos_x1 = pos_X - items[i].Getitem_h();
												}

												if (pegado_arriba == true)
												{
													pos_y1 = pos_Y - items[i].Getitem_w();
												}
												fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i, items[i].Getitem_stack(), items[i].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[i].Getitem_h(), pos_y1 + items[i].Getitem_w(), l + 1);

												//mirar si hay trimming
												if (cplex.isExtracted(x_ijkl[(*it)][j][k][l]) && cplex.getValue(x_ijkl[(*it)][j][k][l]) > 0.001)
												{
													int pos_y1 = pos_y;
													fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it), items[(*it)].Getitem_stack(), items[(*it)].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[(*it)].Getitem_w(), pos_y1 + items[(*it)].Getitem_h(), l + 1);
													temp_stacks[items[(*it)].Getitem_stack()].erase(items[(*it)].Getitem_id());
												}
												if (cplex.isExtracted(xr_ijkl[(*it)][j][k][l]) && cplex.getValue(xr_ijkl[(*it)][j][k][l]) > 0.001)
												{
													int pos_y1 = pos_y;
													fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it), items[(*it)].Getitem_stack(), items[(*it)].Getitem_seq(), pos_x1, pos_y1, pos_x1 + items[(*it)].Getitem_h(), pos_y1 + items[(*it)].Getitem_w(), l + 1);
													temp_stacks[items[(*it)].Getitem_stack()].erase(items[(*it)].Getitem_id());
												}


												if (pegado_izquierda == false)
													pos_x += items[i].Getitem_h();
												//										pos_x += items[i].Getitem_w();
											}
										}

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
void Glass::FormulationStagesOrigWastes(short int nbins, short int nt)
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
	P_pilas = true;
	int Tipo_objetivo = 2;
	if (Tipo_objetivo == 3)
		numbins = 1;
	NumVarMatrix2 x_ij(env, 2 * numpieces);
	//	NumVarMatrix2 xr_ij(env, 2 * numpieces);
	NumVarMatrix3 z_kli(env, numbins);
//	NumVarMatrix4 z_klji(env, numbins);
	NumVarMatrix3 h_kli(env, numbins);

	NumVarMatrix2 y_kl(env, numbins);
	NumVarMatrix2 esH_kl(env, numbins);
	NumVarMatrix2 w_kl(env, numbins);
	NumVector y_l(env);
	NumVector esW_l(env);
	NumVector w_l(env);

	//Variables 
	NumVarMatrix2 el_ij(env, 2*numpieces);
	NumVector es_i(env,2* numpieces);

	

	NumVector H_i(env, 2*numpieces);
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
//		z_klji[i] = NumVarMatrix2(env, numtiras);
		z_kli[i] = NumVarMatrix(env, numtiras);
		h_kli[i] = NumVarMatrix(env, numtiras);
		for (int l = 0; l < numtiras; l++)
		{
			//	{
//			z_klji[i][l] = NumVarMatrix(env, 2 * numpieces);
			z_kli[i][l] = IloIntVarArray(env, 2 * numpieces);
			h_kli[i][l] = IloIntVarArray(env, 2 * numpieces);
			for (int j = 0; j < 2 * numpieces; j++)
			{
//				z_klji[i][l][j] = IloIntVarArray(env, 2 * numpieces);
/*				for (int j2 = 0; j2 < 2 * numpieces; j2++)
				{
					z_klji[i][l][j][j2] = IloIntVar(env, 0, 1);
					sprintf(nombre, "z_%d_%d_%d_%d", i, l, j,j2);
					z_klji[i][l][j][j2].setName(nombre);
				}*/
				z_kli[i][l][j] = IloIntVar(env, 0, 1);
				sprintf(nombre, "z_%d_%d_%d", i, l, j);
				z_kli[i][l][j].setName(nombre);
				h_kli[i][l][j] = IloIntVar(env, 0, plate_h);
				sprintf(nombre, "h_%d_%d_%d", i, l, j);
				h_kli[i][l][j].setName(nombre);

			}
		}
	}


	for (int j = 0; j < 2 * numpieces; j++)
	{
		el_ij[j]= IloIntVarArray(env, 2 * numpieces);
		H_i[j]= IloIntVar(env, 0, plate_h);
		sprintf(nombre, "H_%d", j);
		H_i[j].setName(nombre);
		es_i[j] = IloIntVar(env, 0, 1);
		sprintf(nombre, "Xes_%d", j);
		es_i[j].setName(nombre);
		for (int j2 = j; j2 < 2 * numpieces; j2++)
		{

			el_ij[j][j2] = IloIntVar(env, 0, 1);
			sprintf(nombre, "Xel_%d_%d",j, j2);
			el_ij[j][j2].setName(nombre);
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
		esH_kl[i]= IloIntVarArray(env, numtiras);
		for (int j = 0; j < numtiras; j++)
		{
			w_kl[i][j] = IloIntVar(env, 0, max1Cut);
			sprintf(nombre, "W_%d_%d", i, j);
			w_kl[i][j].setName(nombre);
			y_kl[i][j] = IloIntVar(env, 0, 1);
			sprintf(nombre, "Y_%d_%d", i, j);
			y_kl[i][j].setName(nombre);
			esH_kl[i][j] = IloIntVar(env, 0, 1);
			sprintf(nombre, "XesH_%d_%d", i, j);
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
		sprintf(nombre, "XesW%d", l);
		esW_l[l].setName(nombre);
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
	//nuevos alturas
	for (int i = 0; i < 2 * numpieces; i++)
	{
		IloExpr v3(env);

		v3 += x_ij[i][i] * min2Cut;
	

		v3 += (-1)*H_i[i];

		restr.add(IloRange(env, -IloInfinity, v3, 0));
		v3.end();
		for (int k = 0; k < numbins; k++)
		{
			for (int l = 0; l < numtiras; l++)
			{
				IloExpr v4(env);
				v4 += x_ij[i][i] * min1Cut;
				v4 += (-1) * w_kl[k][l];
				v4 += (-1) * (1 - z_kli[k][l][i]) * max1Cut;
				restr.add(IloRange(env, -IloInfinity, v4, 0));
				v4.end();
			}
		}

		for (int j = i; j < 2 * numpieces; j++)
		{
			IloExpr v1(env);
			IloExpr v2(env);

			v1 += G_Vector_Ordenado_Altura[j].altura*x_ij[i][j];
			v2 += G_Vector_Ordenado_Altura[j].altura* x_ij[i][j];
			v1 += (-1)*H_i[i];
			v1 += (1 - el_ij[i][j])*waste_min;
			
			v2 += (-1)*H_i[i];
			v2 += (1 - el_ij[i][j])*plate_h;
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();

			restr.add(IloRange(env, 0, v2, IloInfinity));
			v2.end();
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
		v1.end();
	}
	//Para el orden
	/*
	if (P_orders)
	{
		//Variables que me indican donde
		for (int j = 0; j < 2 * numpieces; j++)
		{

			for (int j2 = j; j2 < 2 * numpieces; j2++)
			{
				IloExpr v1(env);
				v1 += (-1) * x_ij[j][j2];
				for (int k = 0; k < numbins; k++)
				{

					for (int l = 0; l < numtiras; l++)
					{
						v1 += z_kli[k][l][j][j2];
					}
				}

				//		v1 += (-1) * xr_ij[j][j];
				restr.add(IloRange(env, 0, v1, 0));
				v1.end();
			}
		}
		for (int k = 0; k < numbins; k++)
		{

			for (int l = 0; l < numtiras; l++)
			{
				for (int j = 0; j < 2 * numpieces; j++)
				{
					for (int j2 = j; j2 < 2 * numpieces; j2++)
					{
						int id = G_Vector_Ordenado_Altura[j2].item_id;
						IloExpr v1(env);
						v1 += z_klji[k][l][j][j2];
						if ((stacks[id].Getitem_nbr()) > 1 && (items[id].Getitem_seq() > 1))
						{

							// buscamos el id anterior
							int id_anterior = -1;
							for (int ant1 = 0; ant1 < 2 * numpieces && id_anterior<0; ant1++)
							{
								if (items[ant1].Getitem_stack() == items[id].Getitem_stack() &&
									((items[ant1].Getitem_seq() + 1) == items[id].Getitem_seq()))
								{
									id_anterior = ant1;
								}
							}
							int i2 = id - 1;
							if (i2 >= 0 && items[id].Getitem_stack() == items[i2].Getitem_stack())
							{

								for (int k2 = 0; k2 <= k; k2++)
								{
									for (int l2 = 0; l2 < numtiras; k2++)
									{
										if (k2 == k && l2 > l)
											break;
										for (int j3 = 0; j3 < 2 * numpieces; j3++)
										{
											if (k2 == k && l2 == l && j3 > j)
												break;

												v1 += z_klji[k2][l2][j3][j2-1];



										}
									}
								}
							}
						}

					}
				}
			}
		}
	}
	*/
	//anchuras de las tiras

	for (int i = 0; i < (2 * numpieces - 1); i++)
	{
		IloExpr v1(env);
		for (int j = i; j < 2 * numpieces; j++)
		{

			v1 += G_Vector_Ordenado_Altura[j].anchura * x_ij[i][j];
			//			v1 += xr_ij[i][j];


		}
		v1 +=(-1)* (max1Cut) * x_ij[i][i];
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
				IloExpr v4(env);
				for (int j = i; j < 2 * numpieces; j++)
				{
					v1 += G_Vector_Ordenado_Altura[j].anchura * x_ij[i][j];
					v4 += G_Vector_Ordenado_Altura[j].anchura * x_ij[i][j];
					//					v1 +=(-1)*  G_Vector_Ordenado_Altura[j].anchura ;
										//			
					//					v1 +=  G_Vector_Ordenado_Altura[j].anchura * z_kli[k][l][i];
					//						v1 += items[G_Vector_Ordenado_Altura[i].item_id].Getitem_h() * xr_ij[i][j];



				}

				v1 += (-1) * w_kl[k][l];

				v1 += (-1)*(1- z_kli[k][l][i]) * max1Cut;

				v1 += (1 - es_i[i])*waste_min;

				v4 += (-1) * w_kl[k][l];

				v4 += (1 - z_kli[k][l][i]) * max1Cut;

				v4 += (1 - es_i[i])*max1Cut;
				restr.add(IloRange(env, -IloInfinity, v1, 0));
				v1.end();
				restr.add(IloRange(env, 0, v4, IloInfinity));
				v4.end();
				v2 += (-1) * z_kli[k][l][i];
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

					v1 += G_Vector_Ordenado_Altura[j].altura * G_Vector_Ordenado_Altura[j].anchura * x_ij[i][j];
					//			v1 += xr_ij[i][j];


				}

				for (int i = 0; i <= counterpart; i++)
				{

					v1 += G_Vector_Ordenado_Altura[j].altura * G_Vector_Ordenado_Altura[j].anchura * x_ij[i][counterpart];
					//			v1 += xr_ij[i][j];


				}

			}

		}
		if (Best_plates_nbr <= 1)
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
		if (Best_plates_nbr > 1)
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
		IloExpr v2(env);
		IloExpr v3(env);
		for (int l = 0; l < numtiras; l++)
		{

			v1 += w_kl[k][l];
			v2 += w_kl[k][l];
			v3 += w_kl[k][l];

		}

		v1 += (-1) * plate_w * y_l[k];
		restr.add(IloRange(env, -IloInfinity, v1, 0));
		v1.end();
		v2 += (-1) * plate_w;
		v2 += (1- esW_l[k])*waste_min;
		restr.add(IloRange(env, -IloInfinity, v2, 0));
		v2.end();
		v3 += (-1) * plate_w;
		v3 += (1 - esW_l[k]) * plate_w;
		restr.add(IloRange(env, 0, v3, IloInfinity));
		v3.end();
	}

	//Altura mínima
	for (int k = 0; k < numbins; k++)
	{
		for (int l = 0; l < numtiras; l++)
		{
			IloExpr v1(env);
			IloExpr v3(env);

			for (int i = 0; i < 2 * numpieces; i++)
			{
				IloExpr v2(env);
				v2 += H_i[i];
				v2 += (-1) * (1 - z_kli[k][l][i]) * plate_h;
				v2 += (-1) * h_kli[k][l][i];
				restr.add(IloRange(env, -IloInfinity, v2, 0));
				v2.end();
	//			v1 += G_Vector_Ordenado_Altura[i].altura * z_kli[k][l][i];
				v1 += h_kli[k][l][i];
				v3 += h_kli[k][l][i];
				
			}
			v1 += (-1) * plate_h;
			v1 += (1 - esH_kl[k][l]) * waste_min;
			v3 += (-1) * plate_h;
			v3 += (1 - esH_kl[k][l]) * plate_h;
			restr.add(IloRange(env, -IloInfinity, v1, 0));
			v1.end();
			restr.add(IloRange(env, 0, v3, IloInfinity));
			v3.end();
		}

	}
	if (P_orders)
	{

	}
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

		for (int k = 0; k < numtiras; k++)
		{
			Expression_fobj += w_kl[numbins - 1][k];
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

	if (Tipo_objetivo != 0 && Tipo_objetivo != 3)
		model.add(IloMinimize(env, Expression_fobj));
	if (Tipo_objetivo == 3)
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
//		if (Tipo_objetivo == 2)
//			cplex.setParam(IloCplex::Param::Simplex::Limits::LowerObj, LastBinWidth);
		if (G_Dibujar)
			cplex.exportModel("RoadefStageOrigwastes.lp");

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

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
		
		//cplex.MIPEmphasis()
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
			Best_Value_Formulation = cplex.getObjValue()+0.001;
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
							if (cplex.isExtracted(h_kli[k][l][j]) && cplex.getValue(h_kli[k][l][j]) > 0.001)
							{
								printf("%s %2.f\n", h_kli[k][l][j].getName(), cplex.getValue(h_kli[k][l][j]));

							}
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
						std::string str = file_idx + "_solution_" + std::to_string(G_Exacto) + "_" + std::to_string(Tipo_objetivo) + "_" + std::to_string(Duplicated)+ ".txt";
						fin3 = fopen(str.c_str(), "w+");
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
										val_alto = G_Vector_Ordenado_Altura[i].altura;
										pos_x_tira = pos_x;
										if (pos_x_tira > Best_Solution_Exacto)
											Best_Solution_Exacto = pos_x;
									}

									if (cplex.isExtracted(x_ij[j][i]) && cplex.getValue(x_ij[j][i]) > 0.001)
									{
										int pieza = G_Vector_Ordenado_Altura[i].item_id;
										fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", pieza, items[pieza].Getitem_stack(), items[pieza].Getitem_seq(), pos_x_tira, pos_y, pos_x_tira + G_Vector_Ordenado_Altura[i].anchura, pos_y + G_Vector_Ordenado_Altura[i].altura, k + 1);
										pos_x_tira += G_Vector_Ordenado_Altura[i].anchura;
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