



/*
 * Francisco Parreño Torres
 UCLM

 */


#include "definitions.h"
#include "Glass.h"
#include <omp.h>


int Global_Best_Objective_Function = MAXIMUM_INT_ROADEF;// 11266923; //2329354
int Param = 30;

// int  Global_Best_Objective_Function = 2976090;
//int Global_Best_Objective_Function = 2308734;//// 1551385;// 1551385; 3617251



/*
int get_random(const int & min, const int & max) {
static thread_local std::mt19937 generator(1);
std::uniform_int_distribution<int> distribution(min, max);
return distribution(generator);
}*/

int main(int argc, char **argv)
{

	int nThreads = 8;
	int LazyConst = 0;
	int CotaTira = 0;
	int CotaNB = 0;
	Global_Best_Objective_Function = MAXIMUM_INT_ROADEF;
	Param = 21;
	int Exacto = 0;
	int tiempo_heuristico = 180;//30;
	string file_to_read, file_to_write, batchPath, solutionPath, defectsPath, optParamsPath, file_idx2;
	int Type = 0;
	int TeamId = 0, seed = 1, Seconds = 0;
	int Doble = 2;
	int Iter = 10;
	//Procesamiento de los parametros

	if ((argc > 2) && (argc % 2 == 1))
	{ //Se comprueba que haya pares de argumentos (y al menos un par)

		while (argc > 1)
		{
			if (strcmp(argv[1], "-t") == 0)
			{
				//Tiempo
				sscanf(argv[2], "%d", &Seconds);
			}
			else if (strcmp(argv[1], "-p") == 0)
			{
				//File to read
				file_to_read = argv[2];

			}
			else if (strcmp(argv[1], "-o") == 0)
			{
				file_to_write = argv[2];
			}
			else if (strcmp(argv[1], "-s") == 0)
			{
				sscanf(argv[2], "%d", &seed);
			}
			else if (strcmp(argv[1], "-name") == 0)
			{
				sscanf(argv[2], "%d", &TeamId);
			}
			else if (strcmp(argv[1], "-iter") == 0)
			{
				sscanf(argv[2], "%d", &Iter);
			}
			//For the test 
			else if (strcmp(argv[1], "-Param") == 0)
			{
				sscanf(argv[2], "%d", &Param);
			}
			else if (strcmp(argv[1], "-Exacto") == 0)
			{
				sscanf(argv[2], "%d", &Exacto);
			}
			//For the test 
			else if (strcmp(argv[1], "-Lazy") == 0)
			{
				sscanf(argv[2], "%d", &LazyConst);
			}
			else if (strcmp(argv[1], "-Tira") == 0)
			{
				sscanf(argv[2], "%d", &CotaTira);
			}
			else if (strcmp(argv[1], "-NB") == 0)
			{
				sscanf(argv[2], "%d", &CotaNB);
			}
			else if (strcmp(argv[1], "-Doble") == 0)
			{
				sscanf(argv[2], "%d", &Doble);
			}
			else if (strcmp(argv[1], "-Type") == 0)
			{
				sscanf(argv[2], "%d", &Type);
				//Type=0 is the usual
				//Type=1 is the new one
			}
			argc -= 2;
			argv += 2;

		}
	}
//	Doble = 0;
//	DibujarOpenGL();
//	return 1;
	srand(seed);
	if (Type == 1) Seconds = Seconds;
	const int seedId = seed, maxSeconds = (Seconds - 5);

	
	if (Type == 1) tiempo_heuristico = Seconds;
	if (Type==2) tiempo_heuristico = 0;
	file_idx2 = file_to_read;

	if (Type !=1)
	{
		batchPath = file_idx2 + "_batch.csv";


	}
	else
	{
		batchPath = file_idx2 + ".txt";
	}
	defectsPath = file_idx2 + "_defects.csv";
	solutionPath = file_idx2 + "_solution.csv";
	if (Type!=2)
	optParamsPath = PATH_TO_INSTANCES "global_param.csv";
	else
		optParamsPath = PATH_TO_INSTANCES "global_paramGW.csv";



	struct timeb time_final, time_initial;
	ftime(&time_initial);
	const struct timeb time_ini = time_initial;
	if (Param <= 20)
	{


		//***************************Initial test

		int i = 6;
		Glass Problema(file_to_read, i, seed);
		//REVISAR TODO
		Problema.G_Exacto = Exacto;
		Problema.G_Param = Param;
		Problema.G_Rotate = false;
//		if (Param==17)
	//		Problema.G_Rotate = true;
//		if (get_aleatorio(0, 4) <= 0 && (i > 12))
	//		Problema.G_Rotate = true;
		//		if (i > 12)
		//		Problema.G_type_strip = get_aleatorio(0, 3);
		//		if (i>=0)

		Problema.G_Repeticiones = Iter;
		Problema.G_Classic = false;
		Problema.name_instance = file_to_read;
		Problema.G_Time = maxSeconds;
		Problema.G_Time_Initial = time_ini;
		Problema.active_log = true;
		Problema.active_log_error = true;
		if (Exacto>3 && Type==0)
		Problema.parseBatch(batchPath);
		else
		Problema.parseBatchWithoutStacks(batchPath);
		if (Param != 4)
		{
			if (Exacto>=7)
			Problema.parseDefects(defectsPath);
		}
		printf("Aqui Pasa Algo 11\n");
		Problema.G_iteraciones = max(2000 - Problema.batch_items * 3 - Problema.stack_nbr * 3, 200);
//		Problema.G_iteraciones = 200;
		Problema.parseOptimizationParams(optParamsPath);
		Problema.G_iter_veces = 0;

		printf("Aqui Pasa Algo 12\n");
		//MultiStart();
		Problema.statistics();

		printf("Aqui Pasa Algo 13\n");
		Problema.CreateMatrix();
		printf("Aqui Pasa Algo 14\n");

//		return 0;
		if (Param != 0 && Param!=2 && Param!=10)
		Problema.JointItems();

		Problema.one_item_each = false;
		Problema.draw_solution = true;

		Problema.deterministic = false;
		Problema.G_iteraciones = (Problema.G_iteraciones * 10) / 10;


		//	Problema.G_Iterations = (i / 4) + (i % 4) + 1;
		//	Problema.G_Size_width = (i / 4) + (i % 4) + 1;
		//		Problema.G_Size_Chosen = 1;

		//		if ((i + 1) % 4 == 2) Problema.G_tipo_lista = 3;

		if (Param == 0)
		{
			Problema.Plates = 1;
			Problema.Total_width_bin = 0;
			Problema.MinDimensionPieza = Problema.MinDimensionPieza_Inicial;
			Problema.MaxMinDimensionPieza = Problema.MaxMinDimensionPieza_Inicial;
			Problema.Total_area_used = 0;

			Problema.Inicio = 0;
			Problema.FinishSolution();
			Global_Best_Objective_Function = Problema.Objective_function;
			//		printf("%d ", Param);
		}
		if (Param == 1)
		{
			Problema.Plates = 1;
			Problema.Total_width_bin = 0;
			Problema.MinDimensionPieza = Problema.MinDimensionPieza_Inicial;
			Problema.MaxMinDimensionPieza = Problema.MaxMinDimensionPieza_Inicial;
			Problema.Total_area_used = 0;

			Problema.Inicio = 0;

			Problema.FinishSolution();
			Global_Best_Objective_Function = Problema.Objective_function;
			//		printf("%d ", Param);
		}
		if (Param == 2)
		{
			Problema.Plates = 1;
			Problema.Total_width_bin = 0;
			Problema.MinDimensionPieza = Problema.MinDimensionPieza_Inicial;
			Problema.MaxMinDimensionPieza = Problema.MaxMinDimensionPieza_Inicial;
			Problema.Total_area_used = 0;

			Problema.Inicio = 0;

			//		for (int i=0;i<100;i++)
			Problema.Randomize(10000);
		}
		if (Param == 3)
		{
			Problema.Plates = 1;
			Problema.Total_width_bin = 0;
			Problema.MinDimensionPieza = Problema.MinDimensionPieza_Inicial;
			Problema.MaxMinDimensionPieza = Problema.MaxMinDimensionPieza_Inicial;
			Problema.Total_area_used = 0;

			Problema.Inicio = 0;

			//		for (int i=0;i<100;i++)
			Problema.Randomize(20000);
		}
		//Tipo Lista
		if (Param == 4)
		{
			Problema.G_finish_instant = false;

			Problema.G_tipo_lista = 0;
		//	Problema.BeamSearch(1, 1);
			Problema.BeamSearch(1, 44);;
			FILE* file;
			

			file = fopen(file_to_write.c_str(), "a+");
			fprintf(file, "%s   Objective %d Plates %d Tira %d\n", file_idx2.c_str(), Problema.Best_Objective_function,Problema.Best_plates_nbr,Problema.Best_Width_First_Rotate);
			fclose(file);
			return 0;
		}

		if (Param == 5)
		{
			Problema.G_finish_instant = true;
			Problema.G_tipo_lista = 0;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 6)
		{
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 1;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 7)
		{
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 2;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 8)
		{
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 3;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 9)
		{
			Problema.G_Iterations = 1;
			Problema.G_Size_Chosen = 2;
			Problema.G_Size_alpha = 3;

			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 10)
		{
			Problema.G_Iterations = 2;
			Problema.G_Size_Chosen = 2;
			Problema.G_Size_alpha = 2;
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
			int tiempo = Problema.G_Time;
			Problema.G_Time = 120;
			//No queremos defectos
			if (Exacto <= 6) Problema.G_Param = 4;
			Problema.BeamSearch(10, 10);
			Problema.G_Time = tiempo;
//			Problema.Best_plates_nbr = 1;
//			Problema.Best_Width = 3000;
//			printf("Param %d\n", Param);

		}
		if (Param == 11)
		{
			Problema.G_Iterations = 1;
			Problema.G_Size_Chosen = 4;
			Problema.G_Size_alpha = 2;
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 12)
		{
			Problema.G_Iterations = 2;
			Problema.G_Size_Chosen = 2;
			Problema.G_Size_alpha = 2;
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 13)
		{
			Problema.G_Iterations = 2;
			Problema.G_Size_Chosen = 3;
			Problema.G_Size_alpha = 1;
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 14)
		{
			Problema.G_Iterations = 2;
			Problema.G_Size_Chosen = 4;
			Problema.G_Size_alpha = 1;
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 15)
		{
			Problema.G_Iterations = 3;
			Problema.G_Size_Chosen = 1;
			Problema.G_Size_alpha = 2;
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 16)
		{
			Problema.G_Iterations = 3;
			Problema.G_Size_Chosen = 2;
			Problema.G_Size_alpha = 1;
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 17)
		{
			Problema.G_Iterations = 1;
			Problema.G_Size_Chosen = 1;
			Problema.G_Size_alpha = 1;
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
	//		Problema.G_Rotate = true;
		//	Problema.BeamSearchRotate(1, 1);
			Problema.BeamSearch(1, 1);
		}

		if (Param == 18)
		{
			Problema.G_Iterations = 4;
			Problema.G_Size_Chosen = 3;
			Problema.G_Size_alpha = 1;
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
			Problema.G_type_strip = 2;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 19)
		{
			Problema.G_Iterations = 6;
			Problema.G_Size_Chosen = 3;
			Problema.G_Size_alpha = 1;
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
			Problema.G_type_strip = 0;
			Problema.BeamSearch(1, 1);
		}
		if (Param == 20)
		{
			Problema.G_Iterations = 5;//4
			Problema.G_Size_Chosen = 3;//2
			Problema.G_Size_alpha = 1;//1
			Problema.G_finish_instant = false;
			Problema.G_tipo_lista = 0;
			Problema.G_type_strip = 3;
			Problema.BeamSearch(1, 1);
		}
		if (Param == -1)
		{
			Problema.G_Iterations = 3;
			Problema.G_Size_Chosen = 3;
			Problema.G_Size_alpha = 1;
			Problema.G_finish_instant = true;
			Problema.G_tipo_lista = 0;
			Problema.G_Rotate = true;
			Problema.BeamSearchRotate(1, 1);
			//Problema.BeamSearch(1, 1);
		}
		//Falta probar lo de qué ponemos, 1 strip, 2 strip, bin, mezcla. //Poner iteraciones similares
		// 
		//Falta probar el aleatorizado , Hecho es el param =14
		int maxLevel = Problema.G_level;
		int maxIter = 0;
		int Level_Best_Solution = Problema.Best_iter;
		int Iterations = Problema.G_iteraciones;
		bool Rotate = Problema.G_Rotate;
		int Tipo_Lista = Problema.G_tipo_lista;
		int Type_Strip = 1;
		int Iter_Best = -1;
		bool Classic = Problema.G_Classic;
		bool FInstant = Problema.G_finish_instant;

		//Parámetros
		/*	if (Param == 6)
		{
		Problema.G_tipo_lista = 2;
		Problema.BeamSearch(1, 1);
		}
		*/	ftime(&time_final);
		double tmp_first = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;

		//	Size = max((1000 / tmp_first) , 2);
		double tmp_second = 0;
		int NB = CotaNB;
		if (Problema.Best_plates_nbr < 80)
			NB = Problema.Best_plates_nbr;
		if (Problema.G_Best_Rsol_Items.size() != 0)
		{
			Problema.WriteSolution();
//			DibujarOpenGL();
		}

		Problema.Best_Min_Perdida = (((Problema.Best_plates_nbr - 1) * Problema.plate_h*Problema.plate_w) + (Problema.Best_Width * Problema.plate_h)) - Problema.Total_area;
		printf("*************Solucion Heuristico Bins %d Tira %d******************\n", Problema.Best_plates_nbr, Problema.Best_Width);
//		Problema.FormulationStagesOrig(NB, nThreads);
//		Problema.FormulationStages(NB, nThreads);


		//Knapsack formulation ***************
/*		Problema.FormulationMultiKnapsack(Problema.batch_items, nThreads);
		ftime(&time_final);
		double tmp_firstB = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;

		NB = Problema.Best_Value_Formulation;
		Problema.FormulationMultiKnapsack(Problema.Best_Value_Formulation, nThreads);
		ftime(&time_final);
		double tmp_firstStripe = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;
		if (Problema.Best_Value_Formulation == 0)
		{
			NB--;
			Problema.FormulationMultiKnapsack(NB, nThreads);
			ftime(&time_final);
			tmp_firstStripe = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;

		}
				printf("%s Time B %f Time S %f  Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d %d %d %d %d %d %d NB %d Seed %d Tira %d F %d LB %d\n\n", file_idx2.c_str(), tmp_firstB,tmp_firstStripe, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problema.G_Iterations, Problema.G_Size_Chosen, Problema.G_Size_alpha, Problema.Max_List_Nodos_Beam, Problema.Max_List_Nodos_Beam_Global_Double + Problema.Max_List_Nodos_Beam_Global_Global + Problema.Max_List_Nodos_Beam_Global_Utilization,Problema.Best_plates_nbr,Problema.batch_items, Problema.stack_nbr,Problema.Total_defects,Problema.Total_area_defects,Problema.Total_area, NB, seed, CotaTira, Problema.Best_Value_Formulation, Problema.LB_Best_Value_Formulation);

		************************/
		
			//		printf("%s Time Ini %f   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d\n", file_idx2.c_str(), tmp_first, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param);
//		printf("%s Time Ini %f   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d NB %d Seed %d Tira %d F %d LB %d\n", file_idx2.c_str(), tmp_first, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problemas[Iter_Best].G_Iterations, Problemas[Iter_Best].G_Size_Chosen, Problemas[Iter_Best].G_Size_alpha, Problemas[Iter_Best].Max_List_Nodos_Beam, Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Double + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Global + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Utilization, NB, seed, Tira, Problemas[0].Best_Value_Formulation, Problemas[0].LB_Best_Value_Formulation);
		//
//		FILE* f = fopen("./Resultados.txt", "a+");
//		Problema.GenerarInstanciasExacto();
//		return 1;
		Problema.CotaArea();

		printf("Aqui1");
//		fprintf(f,"%s   BinArea %d TrozoArea %d", file_idx2.c_str(), Problema.P_BinsCotaArea, Problema.P_TrozoArea);
		Problema.P_maxnivelesh = 0; Problema.P_maxnivelesw = 0;

		Problema.CalculoMaximoPilasNivelesEasy();
		printf("Aqui2");

//		fprintf(f,"\t%d\t %d", Problema.P_maxnivelesh, Problema.P_maxnivelesw);
//		Problema.CalculoMaximoPilasNivelesKnapsack(Problema.plate_h);
//		printf("Aqui3");

//		Problema.CalculoMaximoPilasNivelesKnapsack(Problema.plate_w);
		ftime(&time_final);
		double tmp_firstB = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;
//		return 1;
//		fprintf(f,"\t%d\t %d Time %f\n", Problema.P_maxnivelesh, Problema.P_maxnivelesw,tmp_firstB);
			//si incluimos las restricciones de order
		Problema.P_orders = false;
		//si incluimos también trimming
		Problema.P_trimming = false;
		//si incluimos orden de las tiras y que no sean vacías
		Problema.P_order_tiras = false;
		//si incluimos restricciones de defecto
		Problema.P_defects = false;
		//si incluimos restricciones de minimo waste
		Problema.P_minimo_waste = false;
		//si incluimos variable de cota
		Problema.P_cota = false;
		Problema.G_Dibujar = false;
		Problema.P_file_to_write = file_to_write;
		Problema.P_initial_solution = true;
		if (Problema.P_initial_solution)
		{
//			if (Rotate == false)
				Problema.MakeFileTextSolution(file_to_write);
//			else
//				Problema.EscribirSolutionRotated(file_to_write, Global_Best_Solution_Rotated);
		}
		if (Exacto >= 3) Problema.P_cota = false;
		if (Exacto >= 3) Problema.P_order_tiras = true;
		if (Exacto >= 3) Problema.P_minimo_waste = true;

		if (Exacto >= 4) Problema.P_orders = true;
		if (Exacto >= 5) Problema.P_trimming = true;
		if (Exacto >= 7) Problema.P_defects = true;

		if (Exacto <= 2)
			Problema.FormulationStagesOrig(NB,nThreads);
		else
		{
			if (Exacto <= 3)
				Problema.FormulationStagesOrigWastes(NB, nThreads);
			else
			{
				
					if (Exacto < 6)
						Problema.FormulacionCompleta3Indices(NB, nThreads, false, false);
					else
						Problema.FormulacionCompleta4IndicesDefectos(NB, nThreads, false, false);

			}
		}
		ftime(&time_final);
		tmp_firstB = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;
		printf("%s   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d %d %d %d %d %d %d NB %d Seed %d Tira %d BinsH %d TiraH %d F %d LB %d Time %f MW %d MP %d MM %d E %d NS %d PJ %d PJP %d\n", file_idx2.c_str(),  Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problema.G_Iterations, Problema.G_Size_Chosen, Problema.G_Size_alpha, Problema.Max_List_Nodos_Beam, Problema.Max_List_Nodos_Beam_Global_Double + Problema.Max_List_Nodos_Beam_Global_Global + Problema.Max_List_Nodos_Beam_Global_Utilization, Problema.Best_plates_nbr, Problema.batch_items, Problema.stack_nbr, Problema.Total_defects, Problema.Total_area_defects, Problema.Total_area, NB, seed, CotaTira, Problema.Best_plates_nbr, Problema.Best_Width,Problema.Best_Value_Formulation, Problema.LB_Best_Value_Formulation, tmp_firstB,Problema.P_maxnivelesw,Problema.P_maxnivelesh, Problema.P_maxnivelesm, Exacto, Problema.stack_nbr,Problema.P_piezas_juntas,Problema.P_piezas_juntas_posibilidades);

		if (!Problema.opened_file)
			return EXIT_FAILURE;
//		return EXIT_SUCCESS;
	}
	else
	{
		/***********************************End initial tests******************/
		if (Type == 0 || Type == 2)
			nThreads = 8;
		else
			nThreads = 1;
		//linux change to 8
		omp_set_num_threads(nThreads);
		int hilos = 8;



		//		printf("NThreads %d\n");


		std::vector< Glass >  Problemas;
		const int Number_Of_Iterations = 100;
		
//		if (Type==1) Number_Of_Iterations = 1;
		for (int iter_NB = 0; iter_NB <= Number_Of_Iterations; iter_NB++)
		{




			Glass Problema(file_to_read, iter_NB, seed);
			//REVISAR TODO
			Problema.G_Exacto = Exacto;
			Problema.G_Rotate = false;
			if (LazyConst == 0)
				Problema.Lazy = false;
			else
				Problema.Lazy = true;

			if (Param == 30 || Param == 37)
			{
				if (iter_NB >= (hilos) && ((iter_NB % hilos) <= 0))
					Problema.G_Rotate = true;
			}
			//		if (i > 12)
			//		Problema.G_type_strip = get_aleatorio(0, 3);
			//		if (i>=0)
			Problema.Duplicated = Doble;
			Problema.G_Repeticiones = Iter;
			//			Problema.G_Classic = true;
			Problema.name_instance = file_to_read;
			Problema.parseOptimizationParams(optParamsPath);
			Problema.G_Time = tiempo_heuristico;
			Problema.G_Time_Initial = time_ini;
			Problema.active_log = true;
			Problema.active_log_error = true;
			Problema.G_Dibujar =false;
			Problema.Type = Type;
			if (Exacto > 3 && ( Type==0 || Type==2))
				Problema.parseBatch(batchPath);
			else
				Problema.parseBatchWithoutStacks(batchPath);
			if (Param != 4)
			{
				if (Exacto >= 7)
					Problema.parseDefects(defectsPath);
			}
			if (Param != 23)
			{

				if (Param == 25 || Param == 26)
					Problema.parseBatchWithoutStacks(batchPath);
				else
					Problema.parseBatch(batchPath);
				if (Param != 24 && Param != 26)
				{
					Problema.parseDefects(defectsPath);
				}
			}
			Problema.G_iteraciones = max(2000 - Problema.batch_items * 3 - Problema.stack_nbr * 3, 200);


			Problema.G_iter_veces = 0;

			Problema.G_Classic = false;
			//MultiStart();
			Problema.statistics();


			Problema.CreateMatrix();
			//			if (Problema.get_random(0, 12) > 0)
			//				Problema.JointItems();
			if (Exacto <= 6) Problema.G_Param = 4;
			Problema.one_item_each = false;
			Problema.draw_solution = true;
			if (Exacto == 2)
				Problema.waste_min = 0;
			Problema.deterministic = false;
			if (Problema.get_random(0, 4) <= 2)
				Problema.G_finish_instant = false;
			else
				Problema.G_finish_instant = true;
			Problema.G_tipo_lista = 0;
			if (iter_NB > nThreads)
				Problema.G_tipo_lista = iter_NB % 4;
			if (Problema.get_random(0, 4) <= 0)
				Problema.G_tipo_lista = 0;

			if (Problema.get_random(0, 10) <= 0)
				Problema.G_Classic = true;
			else
				Problema.G_Classic = false;
			//		Problema.G_iteraciones = (Problema.G_iteraciones * 10) / 10;






			if (Param == 21)
			{

				Problema.G_Iterations = 1;

				Problema.G_Size_alpha = 1;
				Problema.G_Size_Chosen = 1;
				Problema.G_finish_instant = false;
				Problema.G_tipo_lista = 0;


			}
			if (Param == 22)
			{
				Problema.G_Iterations = 1 + (iter_NB / 3);

				Problema.G_Size_alpha = 1 + (iter_NB / 2);
				Problema.G_Size_Chosen = 1 + (iter_NB / 3);


			}
			if (Param >= 23)
			{

				Problema.G_finish_instant = false;
				Problema.G_tipo_lista = 0;
				hilos = max(hilos, 2);
				if (iter_NB <= hilos)
				{
					Problema.G_Iterations = (iter_NB / (hilos / 2)) + 1;
					Problema.G_Size_alpha = ((iter_NB + 3) / (hilos / 2)) + 1;
					Problema.G_Size_Chosen = ((iter_NB + 5) / (hilos / 2)) + 1;
				}
				else
				{
					Problema.G_Iterations = (iter_NB / (hilos / 4)) + 1;
					Problema.G_Size_alpha = ((iter_NB + 1) / (hilos / 4)) + 1;
					Problema.G_Size_Chosen = ((iter_NB + 2) / (hilos / 4)) + 1;

				}

				//			Problema.active_log_error = true;


			}
			Problemas.push_back(Problema);

		}



#pragma omp parallel  num_threads(nThreads)
		{



			//#pragma omp for schedule(static,1) nowait

#pragma omp for schedule(dynamic,1)


			for (int Global_Iter = 0; Global_Iter <= Number_Of_Iterations; Global_Iter++)
			{




				struct timeb  time_ini2;

				ftime(&time_ini2);
				double tmp_first2 = ((double)((time_ini2.time - time_ini.time) * 1000 + time_ini2.millitm - time_ini.millitm)) / 1000;
				if (tmp_first2 < maxSeconds)
				{

					struct timeb  time_ini21;
					ftime(&time_ini21);




					if (Problemas[Global_Iter].active_log_error)
						printf("\nTipo Nivel %d Thread %d Iter \t%d\n", Problemas[Global_Iter].G_type_strip, omp_get_thread_num(), Global_Iter);
					//					else
						//					printf("\nTipo Nivel %d Thread NO Error %d Iter \t%d\n", Problemas[Global_Iter].G_type_strip, omp_get_thread_num(), Global_Iter);


					if (Problemas[Global_Iter].G_Rotate == true)
						Problemas[Global_Iter].BeamSearchRotate(omp_get_thread_num(), seedId);
					else
						Problemas[Global_Iter].BeamSearch(omp_get_thread_num(), seedId);

					ftime(&time_ini2);
					tmp_first2 = ((double)((time_ini2.time - time_ini21.time) * 1000 + time_ini2.millitm - time_ini21.millitm)) / 1000;
					if (Problemas[Global_Iter].active_log_error) printf("\nTiempo %f Thread %d Iter \t%d\n", tmp_first2, omp_get_thread_num(), Global_Iter);
					if (Problemas[Global_Iter].active_log_error) printf("\nTiempo2 %f Thread %d Iter \t%d\n", tmp_first2, omp_get_thread_num(), Global_Iter);

				}

			}
		}



		std::list< GlassRsol > Global_Best_Solution;
		std::list< Glass_Corte> Global_Best_Solution_Rotated;
		int Iter_Best = -1;//;__max((1200 / tmp_first) / 2, 2);
						   //	int Global_Iter = 0;
		int maxLevel = 0;
		int maxIter = 0;
		int Level_Best_Solution = 0;
		int Iterations = 0;
		bool Rotate = false;
		int Tipo_Lista = 0;
		int Type_Strip = 1;
		bool Classic = false;
		bool FInstant = false;
		int NB = 0;
		for (int Global_Iter = 0; Global_Iter <= Number_Of_Iterations; Global_Iter++)
		{
			if (Problemas[Global_Iter].G_iter_veces != 0)
				maxIter = Global_Iter;
			else
				continue;
			if (Problemas[Global_Iter].Best_Objective_function_Thread == Global_Best_Objective_Function)
			{
				if (Iter_Best == -1)
				{
					Global_Best_Solution = Problemas[Global_Iter].G_Best_Rsol_Items;
					Iter_Best = Global_Iter;
					Level_Best_Solution = Problemas[Global_Iter].Best_iter;
					Iterations = Problemas[Global_Iter].G_Iterations;
					Rotate = Problemas[Global_Iter].G_Rotate;
					if (Rotate == true)
						Global_Best_Solution_Rotated = Problemas[Global_Iter].G_Best_Solution_Rotated;
					Tipo_Lista = Problemas[Global_Iter].G_tipo_lista;
					Type_Strip = Problemas[Global_Iter].G_type_strip;
					Classic = Problemas[Global_Iter].G_Classic;
					FInstant = Problemas[Global_Iter].G_finish_instant;
					NB = Problemas[Global_Iter].Best_plates_nbr;
				}
			}

			if (Problemas[Global_Iter].G_level > maxLevel)
				maxLevel = Problemas[Global_Iter].G_level;
		}
		if (Problemas[0].active_log_error) printf("Best_Solution %d", Global_Best_Objective_Function);
		int Tira = Problemas[0].plate_w + ((Global_Best_Objective_Function - NB * Problemas[0].plate_h * Problemas[0].plate_w + Problemas[0].Total_area) / Problemas[0].plate_h);
		//Hacemos la formulación
		if (CotaNB == 0)
			NB = CotaNB;

		//	Problemas[0].FormulationStages(NB,nThreads);
			//	printf("here1");
		Problemas[0].Best_Objective_function = Global_Best_Objective_Function;
		Problemas[0].G_Best_Rsol_Items = Global_Best_Solution;
		//	Problem.MakeFileTextSolution(file_idx2 + "_solution" + std::to_string(maxSeconds) + "_" + std::to_string(nThreads) + ".csv");
		if (Rotate == false)
		{
			Problemas[0].MakeFileTextSolution(file_to_write);
		}

		else
			Problemas[0].EscribirSolutionRotated(file_to_write, Global_Best_Solution_Rotated);
		//	printf("here2");
		//	Problemas[0].WriteSolution(file_idx2 + "_solutionFranB"+ std::to_string(maxSeconds)+"_" +std::to_string(nThreads)+".txt");

		Problemas[0].G_Dibujar = false;

		//if (Problemas[0].G_Best_Rsol_Items.size() != 0)
		//{
//		Problemas[0].G_Dibujar = true;
/*		if (Problemas[0].G_Dibujar)
		{
			Problemas[0].WriteSolution();
		DibujarOpenGL();
		}
		*/
//}
//		Problemas[0].G_Dibujar = false;

		Problemas[0].G_Time = maxSeconds - tiempo_heuristico;
		ftime(&time_final);
		double tmp_first = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;

		//	Size = max((1000 / tmp_first) , 2);
		double tmp_second = 0;
		if (Exacto == 9 || Type==2)
			Iter_Best = 0;
		//		printf("%s Time Ini %f   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d\n", file_idx2.c_str(), tmp_first, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param);
		printf("%s Time Ini %f   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d NB %d Seed %d Tira %d F %d LB %d", file_idx2.c_str(), tmp_first, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problemas[Iter_Best].G_Iterations, Problemas[Iter_Best].G_Size_Chosen, Problemas[Iter_Best].G_Size_alpha, Problemas[Iter_Best].Max_List_Nodos_Beam, Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Double + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Global + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Utilization, NB, seed, Tira, Problemas[0].Best_Value_Formulation, Problemas[0].LB_Best_Value_Formulation);
		//		if (Problemas[0].Best_plates_nbr < 80)
		//			NB = Problemas[0].Best_plates_nbr;
		if (Exacto > 10 || Type==1)
		{
			FILE* file8;
			file8 = fopen("Results.txt", "w+");
			fprintf(file8, "%s Time Ini %f   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d NB %d Seed %d Tira %d F %d LB %d", file_idx2.c_str(), tmp_first, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problemas[Iter_Best].G_Iterations, Problemas[Iter_Best].G_Size_Chosen, Problemas[Iter_Best].G_Size_alpha, Problemas[Iter_Best].Max_List_Nodos_Beam, Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Double + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Global + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Utilization, NB, seed, Tira, Problemas[0].Best_Value_Formulation, Problemas[0].LB_Best_Value_Formulation);
			fclose(file8);
			return EXIT_SUCCESS;

		}
		Problemas[0].Best_plates_nbr = NB;
		Problemas[0].Best_Width = Tira;

		Problemas[0].Best_Min_Perdida = (((Problemas[0].Best_plates_nbr - 1) * Problemas[0].plate_h * Problemas[0].plate_w) + (Problemas[0].Best_Width * Problemas[0].plate_h)) - Problemas[0].Total_area;
		printf("*************Solucion Heuristico Bins %d Tira %d******************\n", NB, Tira);
		//		Problema.FormulationStagesOrig(NB, nThreads);
		//		Problema.FormulationStages(NB, nThreads);


				//Knapsack formulation ***************
		/*		Problema.FormulationMultiKnapsack(Problema.batch_items, nThreads);
				ftime(&time_final);
				double tmp_firstB = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;

				NB = Problema.Best_Value_Formulation;
				Problema.FormulationMultiKnapsack(Problema.Best_Value_Formulation, nThreads);
				ftime(&time_final);
				double tmp_firstStripe = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;
				if (Problema.Best_Value_Formulation == 0)
				{
					NB--;
					Problema.FormulationMultiKnapsack(NB, nThreads);
					ftime(&time_final);
					tmp_firstStripe = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;

				}
						printf("%s Time B %f Time S %f  Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d %d %d %d %d %d %d NB %d Seed %d Tira %d F %d LB %d\n\n", file_idx2.c_str(), tmp_firstB,tmp_firstStripe, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problema.G_Iterations, Problema.G_Size_Chosen, Problema.G_Size_alpha, Problema.Max_List_Nodos_Beam, Problema.Max_List_Nodos_Beam_Global_Double + Problema.Max_List_Nodos_Beam_Global_Global + Problema.Max_List_Nodos_Beam_Global_Utilization,Problema.Best_plates_nbr,Problema.batch_items, Problema.stack_nbr,Problema.Total_defects,Problema.Total_area_defects,Problema.Total_area, NB, seed, CotaTira, Problema.Best_Value_Formulation, Problema.LB_Best_Value_Formulation);

				************************/

				//		printf("%s Time Ini %f   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d\n", file_idx2.c_str(), tmp_first, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param);
	//		printf("%s Time Ini %f   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d NB %d Seed %d Tira %d F %d LB %d\n", file_idx2.c_str(), tmp_first, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problemas[Iter_Best].G_Iterations, Problemas[Iter_Best].G_Size_Chosen, Problemas[Iter_Best].G_Size_alpha, Problemas[Iter_Best].Max_List_Nodos_Beam, Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Double + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Global + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Utilization, NB, seed, Tira, Problemas[0].Best_Value_Formulation, Problemas[0].LB_Best_Value_Formulation);
			//
	//		FILE* f = fopen("./Resultados.txt", "a+");
	//		Problema.GenerarInstanciasExacto();
	//		return 1;	
		Problemas[0].CotaArea();

		printf("Aqui1");
		//		fprintf(f,"%s   BinArea %d TrozoArea %d", file_idx2.c_str(), Problema.P_BinsCotaArea, Problema.P_TrozoArea);
		Problemas[0].P_maxnivelesh = 0; Problemas[0].P_maxnivelesw = 0;
		if (Type==2)
			Problemas[0].P_WithoutRotation = true;
		Problemas[0].CalculoMaximoPilasNivelesEasy();
		printf("Aqui2");

		//		fprintf(f,"\t%d\t %d", Problema.P_maxnivelesh, Problema.P_maxnivelesw);
		//		Problema.CalculoMaximoPilasNivelesKnapsack(Problema.plate_h);
		//		printf("Aqui3");

		//		Problema.CalculoMaximoPilasNivelesKnapsack(Problema.plate_w);
		ftime(&time_final);
		double tmp_firstB = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;
		//		return 1;
		//		fprintf(f,"\t%d\t %d Time %f\n", Problema.P_maxnivelesh, Problema.P_maxnivelesw,tmp_firstB);
					//si incluimos las restricciones de order
		Problemas[0].P_orders = false;
		//si incluimos también trimming
		Problemas[0].P_trimming = false;
		//si incluimos orden de las tiras y que no sean vacías
		Problemas[0].P_order_tiras = false;
		//si incluimos restricciones de defecto
		Problemas[0].P_defects = false;
		//si incluimos restricciones de minimo waste
		Problemas[0].P_minimo_waste = false;
		//si incluimos variable de cota
		Problemas[0].P_cota = false;
		//		Problemas[0].G_Dibujar = false;
		Problemas[0].P_file_to_write = file_to_write;
		Problemas[0].P_initial_solution = true;
		Problemas[0].G_Dibujar = false;
		Problemas[0].G_Draw_Exacto = true;
		if (Exacto >= 3) Problemas[0].P_cota = true;
		//		if (Exacto >= 3) Problemas[0].P_order_tiras = true;
		if (Exacto >= 3) Problemas[0].P_minimo_waste = true;

		if (Exacto >= 4) Problemas[0].P_orders = true;
		if (Exacto >= 5) Problemas[0].P_trimming = true;
		if (Exacto >= 7) Problemas[0].P_defects = true;
		else Problemas[0].P_defects = false;
		if (Exacto >= 8) Problemas[0].P_heur = true;
		timeb time_final_heur;
		ftime(&time_final_heur);
		timeb time_final_ini_exacto;
		timeb time_final_fin_exacto;
		ftime(&time_final_ini_exacto);
		if (Exacto <= 2)
		{
			Problemas[0].FormulationStagesOrig(NB, nThreads);
			ftime(&time_final_fin_exacto);
		}
		else
		{
			if (Exacto <= 3)
			{
				Problemas[0].FormulationStagesOrigWastes(NB, nThreads);
				ftime(&time_final_fin_exacto);
			}
			else
			{
				//				if (Exacto < 6)
				//					Problemas[0].FormulacionCompleta3Indices(NB, nThreads, false, false);
				//				if (Exacto == 6)
				//					Problemas[0].FormulacionCompleta4IndicesDefectos(NB, nThreads, false, false);

				if (Exacto <= 6)
				{
					int i = Exacto;
					for (; i < 7; i++)
					{

						//						timeb time_final_ini;
						Problemas[0].G_Exacto = i;
						Exacto = i;
						if (Exacto >= 5)
							Problemas[0].P_trimming = true;
						printf("Exacto %d\n", Exacto);
						printf("%s Time Ini %f   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d NB %d Seed %d Tira %d F %d LB %d", file_idx2.c_str(), tmp_first, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problemas[Iter_Best].G_Iterations, Problemas[Iter_Best].G_Size_Chosen, Problemas[Iter_Best].G_Size_alpha, Problemas[Iter_Best].Max_List_Nodos_Beam, Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Double + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Global + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Utilization, NB, seed, Tira, Problemas[0].Best_Value_Formulation, Problemas[0].LB_Best_Value_Formulation);
						ftime(&time_final_ini_exacto);
						if (Exacto < 6)
							Problemas[0].FormulacionCompleta3Indices(NB, nThreads, false, false);
						if (Exacto == 6)
							Problemas[0].FormulacionCompleta4IndicesDefectos(NB, nThreads, false, false);
						ftime(&time_final_fin_exacto);

						tmp_firstB = ((double)(((time_final_fin_exacto.time - time_final_ini_exacto.time) + (time_final_heur.time - time_ini.time)) * 1000 + ((time_final_fin_exacto.millitm - time_final_ini_exacto.millitm) + (time_final_heur.millitm - time_ini.millitm)))) / 1000;
						FILE* file_e;

						string file_Exacto = file_to_read + "_" + std::to_string(Exacto) + ".txt";

						file_e = fopen(file_Exacto.c_str(), "w+");
						fprintf(file_e, "%s   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d %d %d %d %d %d %d NB %d Seed %d Tira %d BinsH %d TiraH %d F %d LB %d Time %f MW %d MP %d MM %d E %d NS %d PJ %d PJP %d D %d BSE %d\n", file_idx2.c_str(), Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problemas[0].G_Iterations, Problemas[0].G_Size_Chosen, Problemas[0].G_Size_alpha, Problemas[0].Max_List_Nodos_Beam, Problemas[0].Max_List_Nodos_Beam_Global_Double + Problemas[0].Max_List_Nodos_Beam_Global_Global + Problemas[0].Max_List_Nodos_Beam_Global_Utilization, NB, Problemas[0].batch_items, Problemas[0].stack_nbr, Problemas[0].Total_defects, Problemas[0].Total_area_defects, Problemas[0].Total_area, NB, seed, CotaTira, Problemas[0].Best_plates_nbr, Tira, Problemas[0].Best_Value_Formulation, Problemas[0].LB_Best_Value_Formulation, tmp_firstB, Problemas[0].P_maxnivelesw, Problemas[0].P_maxnivelesh, Problemas[0].P_maxnivelesm, Exacto, Problemas[0].stack_nbr, Problemas[0].P_piezas_juntas, Problemas[0].P_piezas_juntas_posibilidades, 0, Problemas[0].Best_Solution_Exacto);
						fclose(file_e);
					}
				}
				//				if (Exacto < 6)
				//					Problemas[0].FormulacionCompleta3Indices(NB, nThreads, false, false);
				//				if (Exacto == 6)
				//					Problemas[0].FormulacionCompleta4IndicesDefectos(NB, nThreads, false, false);

			}
		}
		//Read the number of defects
		int defects = 0;
		if (Exacto > 6)
		{

			for (int i = 0; i < NB; i++)
			{
				if (i < (NB - 1))
					defects += Problemas[0].DefectsPlate[i].size();
				else
				{
					for (std::list< GlassDefect > ::iterator it = Problemas[0].DefectsPlate[i].begin(); it != Problemas[0].DefectsPlate[i].end(); it++)
					{
						if ((*it).Getpos_x() < Tira)
							defects++;
					}

				}
			}

			tmp_firstB = ((double)(((time_final_fin_exacto.time - time_final_ini_exacto.time) + (time_final_heur.time - time_ini.time)) * 1000 + ((time_final_fin_exacto.millitm - time_final_ini_exacto.millitm) + (time_final_heur.millitm - time_ini.millitm)))) / 1000;
			printf("Exacto %d\n", Exacto);
			printf("%s   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d %d %d %d %d %d %d NB %d Seed %d Tira %d BinsH %d TiraH %d F %d LB %d Time %f MW %d MP %d MM %d E %d NS %d PJ %d PJP %d D %d BSE %d\n", file_idx2.c_str(), Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problemas[0].G_Iterations, Problemas[0].G_Size_Chosen, Problemas[0].G_Size_alpha, Problemas[0].Max_List_Nodos_Beam, Problemas[0].Max_List_Nodos_Beam_Global_Double + Problemas[0].Max_List_Nodos_Beam_Global_Global + Problemas[0].Max_List_Nodos_Beam_Global_Utilization, NB, Problemas[0].batch_items, Problemas[0].stack_nbr, Problemas[0].Total_defects, Problemas[0].Total_area_defects, Problemas[0].Total_area, NB, seed, CotaTira, Problemas[0].Best_plates_nbr, Tira, Problemas[0].Best_Value_Formulation, Problemas[0].LB_Best_Value_Formulation, tmp_firstB, Problemas[0].P_maxnivelesw, Problemas[0].P_maxnivelesh, Problemas[0].P_maxnivelesm, Exacto, Problemas[0].stack_nbr, Problemas[0].P_piezas_juntas, Problemas[0].P_piezas_juntas_posibilidades, defects, Problemas[0].Best_Solution_Exacto);
			timeb time_final2;



			for (int i = Exacto; i <= 10; i++)
			{




				if (i == 8)
				{
					Exacto = 8;
					Problemas[0].G_Exacto = 8;
					Problemas[0].P_heur = true;
				}
#ifdef _WIN32
										Problemas[0].G_Dibujar = true;
										Problemas[0].G_Draw_Exacto = true;
#endif
#ifdef __linux__ 
										Problemas[0].G_Dibujar = false;
										Problemas[0].G_Draw_Exacto = true;
#endif
				if (i == 9)
				{
					Exacto = 9;
				
					Problemas[0].G_Exacto = 9;
					if (Type == 2)
					{
						Problemas[0].P_heur = false;
						Problemas[0].P_WithoutRotation = true;
						Problemas[0].P_initial_solution = false;
						Problemas[0].P_WithoutPiles = false;

					}
				}
				if (i == 10)
				{
					Exacto = 10;

					Problemas[0].G_Exacto = 10;
					if (Type == 2)
					{
						Problemas[0].P_heur = false;
						Problemas[0].P_WithoutRotation = true;
						Problemas[0].P_initial_solution = false;

						Problemas[0].P_WithoutPiles = true;
					}
				}
				printf("Exacto %d\n", Exacto);
				printf("%s Time Ini %f   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d NB %d Seed %d Tira %d F %d LB %d", file_idx2.c_str(), tmp_first, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problemas[Iter_Best].G_Iterations, Problemas[Iter_Best].G_Size_Chosen, Problemas[Iter_Best].G_Size_alpha, Problemas[Iter_Best].Max_List_Nodos_Beam, Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Double + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Global + Problemas[Iter_Best].Max_List_Nodos_Beam_Global_Utilization, NB, seed, Tira, Problemas[0].Best_Value_Formulation, Problemas[0].LB_Best_Value_Formulation);
				ftime(&time_final_ini_exacto);
				if (Type!=2)
				Problemas[0].FormulacionCompleta4IndicesDefectos(NB, nThreads, false, false);
				else
					Problemas[0].FormulacionCompleta4IndicesDefectosPilas(NB, nThreads, false, false);
				ftime(&time_final_fin_exacto);

				FILE* file8;
				tmp_firstB = ((double)(((time_final_fin_exacto.time - time_final_ini_exacto.time) + (time_final_heur.time - time_ini.time)) * 1000 + ((time_final_fin_exacto.millitm - time_final_ini_exacto.millitm) + (time_final_heur.millitm - time_ini.millitm)))) / 1000;
				string file_eight = file_to_read + "_" + std::to_string(Exacto) + ".txt";
				file8 = fopen(file_eight.c_str(), "w+");
				fprintf(file8, "%s   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d %d %d %d %d %d %d NB %d Seed %d Tira %d BinsH %d TiraH %d F %d LB %d Time %f MW %d MP %d MM %d E %d NS %d PJ %d PJP %d D %d BSE %d\n", file_idx2.c_str(), Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problemas[0].G_Iterations, Problemas[0].G_Size_Chosen, Problemas[0].G_Size_alpha, Problemas[0].Max_List_Nodos_Beam, Problemas[0].Max_List_Nodos_Beam_Global_Double + Problemas[0].Max_List_Nodos_Beam_Global_Global + Problemas[0].Max_List_Nodos_Beam_Global_Utilization, NB, Problemas[0].batch_items, Problemas[0].stack_nbr, Problemas[0].Total_defects, Problemas[0].Total_area_defects, Problemas[0].Total_area, NB, seed, CotaTira, Problemas[0].Best_plates_nbr, Tira, Problemas[0].Best_Value_Formulation, Problemas[0].LB_Best_Value_Formulation, tmp_firstB, Problemas[0].P_maxnivelesw, Problemas[0].P_maxnivelesh, Problemas[0].P_maxnivelesm, Exacto, Problemas[0].stack_nbr, Problemas[0].P_piezas_juntas, Problemas[0].P_piezas_juntas_posibilidades, defects, Problemas[0].Best_Solution_Exacto);
				fclose(file8);
//				if (Exacto==7) return EXIT_SUCCESS;
				if (Exacto==8 || Exacto==9)
				return EXIT_SUCCESS;
			}
		}
		else
		{
	

			tmp_firstB = ((double)(((time_final_fin_exacto.time - time_final_ini_exacto.time) + (time_final_heur.time - time_ini.time)) * 1000 + ((time_final_fin_exacto.millitm - time_final_ini_exacto.millitm) + (time_final_heur.millitm - time_ini.millitm)))) / 1000;
			printf("Exacto %d\n", Exacto);
			printf("%s   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d Param %d Iterations %d SWidht %d Salpha %d SizeLista %d SizeChosen %d %d %d %d %d %d %d NB %d Seed %d Tira %d BinsH %d TiraH %d F %d LB %d Time %f MW %d MP %d MM %d E %d NS %d PJ %d PJP %d D %d BSE %d\n", file_idx2.c_str(), Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter, Rotate, Iterations, Tipo_Lista, Type_Strip, Classic, FInstant, Iter, Param, Problemas[0].G_Iterations, Problemas[0].G_Size_Chosen, Problemas[0].G_Size_alpha, Problemas[0].Max_List_Nodos_Beam, Problemas[0].Max_List_Nodos_Beam_Global_Double + Problemas[0].Max_List_Nodos_Beam_Global_Global + Problemas[0].Max_List_Nodos_Beam_Global_Utilization, NB, Problemas[0].batch_items, Problemas[0].stack_nbr, Problemas[0].Total_defects, Problemas[0].Total_area_defects, Problemas[0].Total_area, NB, seed, CotaTira, Problemas[0].Best_plates_nbr, Tira, Problemas[0].Best_Value_Formulation, Problemas[0].LB_Best_Value_Formulation, tmp_firstB, Problemas[0].P_maxnivelesw, Problemas[0].P_maxnivelesh, Problemas[0].P_maxnivelesm, Exacto, Problemas[0].stack_nbr, Problemas[0].P_piezas_juntas, Problemas[0].P_piezas_juntas_posibilidades, defects, Problemas[0].Best_Solution_Exacto);

		}
		if (!Problemas[0].opened_file)
			return EXIT_FAILURE;
		
		

		
		return EXIT_SUCCESS;
	}

}

/*

#include "definitions.h"
#include "Glass.h"
#include <omp.h>


int Global_Best_Objective_Function = MAXIMUM_INT_ROADEF;// 11266923; //2329354

// int  Global_Best_Objective_Function = 2976090;
//int Global_Best_Objective_Function = 2308734;//// 1551385;// 1551385; 3617251





int main(int argc, char **argv)
{
	
	string file_to_read,file_to_write,batchPath, solutionPath, defectsPath, optParamsPath, file_idx2;
	int TeamId=0,seed=1, Seconds=0;
	int Iter = 10;
	//Procesamiento de los parametros
	
	if ((argc > 2) && (argc % 2 == 1)) 
	{ //Se comprueba que haya pares de argumentos (y al menos un par)

		while (argc > 1)
		{
			if (strcmp(argv[1], "-t") == 0)
			{
				//Tiempo
				sscanf(argv[2], "%d", &Seconds);
			}
			else if (strcmp(argv[1], "-p") == 0)
			{
				//File to read
				file_to_read=argv[2];

			}
			else if (strcmp(argv[1], "-o") == 0)
			{
				file_to_write = argv[2];
			}
			else if (strcmp(argv[1], "-s") == 0)
			{
				sscanf(argv[2], "%d", &seed);
			}
			else if (strcmp(argv[1], "-name") == 0)
			{
				sscanf(argv[2], "%d", &TeamId);
			}
			else if (strcmp(argv[1], "-iter") == 0)
			{
				sscanf(argv[2], "%d", &Iter);
			}
			argc -= 2;
			argv += 2;

		}
	}
	if (argc == 2)
	{
		printf("Team 17");
		return 17;
	}
	srand(seed);
		

		const int seedId = seed, maxSeconds = (Seconds-5);

	file_idx2 = file_to_read;

	batchPath = file_idx2 + "_batch.csv";
	solutionPath =  file_idx2 + "_solution.csv";
	defectsPath =  file_idx2 + "_defects.csv";
    optParamsPath = PATH_TO_INSTANCES "global_param.csv";
	


	struct timeb time_final,time_initial;
	ftime(&time_initial);
	const struct timeb time_ini = time_initial;

	int nThreads=omp_get_max_threads();
	//linux change to 8
	omp_set_num_threads(8);
	


	std::vector< Glass >  Problemas;
	const int Number_Of_Iterations = 100;

	for (int i = 0; i <= Number_Of_Iterations; i++)
	{

		

		Glass Problema(file_to_read, i,seed);
		//REVISAR TODO
		Problema.G_Rotate = false;
//		if (get_aleatorio(0,4)<=0 && (i>12))
	//		Problema.G_Rotate = true;


		Problema.G_Repeticiones = Iter;
		Problema.G_Classic = true;
		Problema.name_instance = file_to_read;
		Problema.G_Time = maxSeconds;
		Problema.G_Time_Initial = time_ini;
		Problema.active_log =false ;
		Problema.active_log_error = false;
		Problema.parseBatch(batchPath);
		Problema.parseDefects(defectsPath);
		if (Problema.batch_items < 30 )
			Problema.G_iteraciones = 2000;
		if (Problema.batch_items > 60)
			Problema.G_iteraciones = 1000;
		if (Problema.batch_items > 300 || Problema.stack_nbr > 60)
			Problema.G_iteraciones = 800;
		if (Problema.batch_items > 600 || Problema.stack_nbr > 120)
			Problema.G_iteraciones = 600;
		if (Problema.batch_items > 900 || Problema.stack_nbr > 180)
			Problema.G_iteraciones = 400;
		Problema.parseOptimizationParams(optParamsPath);
		Problema.G_iter_veces = 0;
		if (get_aleatorio(0,10)>=9)
			Problema.G_Classic = true;
		else
		Problema.G_Classic = false;
		if (get_aleatorio(0, 3) == 0)
			Problema.G_finish_instant = true;
		else
			Problema.G_finish_instant = false;
		//MultiStart();
		Problema.statistics();


		Problema.CreateMatrix();

		Problema.JointItems();

		Problema.one_item_each = false;
		Problema.draw_solution = true;

		Problema.deterministic = false;
		Problema.G_iteraciones = (Problema.G_iteraciones * 10) / 10;


		Problema.G_Iterations= (i / 4) + (i % 4) + 1;
		Problema.G_Size_width= (i / 4) + (i % 4) + 1;


		if ((i + 1) % 4 == 2) Problema.G_tipo_lista = 3;

	

		Problemas.push_back(Problema);
	

	}
	

	
#pragma omp parallel  
	{



#pragma omp for schedule(static,1) nowait




		for (int Global_Iter = 0; Global_Iter <= Number_Of_Iterations; Global_Iter++)
		{




			struct timeb  time_ini2;

			ftime(&time_ini2);
			double tmp_first2 = ((double)((time_ini2.time - time_ini.time) * 1000 + time_ini2.millitm - time_ini.millitm)) / 1000;
			if (tmp_first2 < maxSeconds)
			{

				struct timeb  time_ini21;
				ftime(&time_ini21);




				if (Problemas[Global_Iter].active_log_error)
					printf("\nTipo Nivel %d Thread %d Iter \t%d\n", Problemas[Global_Iter].G_type_strip, omp_get_thread_num(), Global_Iter);


				if (Problemas[Global_Iter].G_Rotate==true)
				Problemas[Global_Iter].BeamSearchRotate(omp_get_thread_num(), seedId);
				else
					Problemas[Global_Iter].BeamSearch(  omp_get_thread_num(), seedId);

				ftime(&time_ini2);
				tmp_first2 = ((double)((time_ini2.time - time_ini21.time) * 1000 + time_ini2.millitm - time_ini21.millitm)) / 1000;
				if (Problemas[Global_Iter].active_log_error) printf("\nTiempo %f Thread %d Iter \t%d\n", tmp_first2, omp_get_thread_num(), Global_Iter);
				if (Problemas[Global_Iter].active_log_error) printf("\nTiempo2 %f Thread %d Iter \t%d\n", tmp_first2, omp_get_thread_num(), Global_Iter);

			}

		}
	}
	
	std::list< GlassRsol > Global_Best_Solution;
	std::list< Glass_Corte> Global_Best_Solution_Rotated;
	int Iter_Best = -1;//;__max((1200 / tmp_first) / 2, 2);
					  //	int Global_Iter = 0;
	int maxLevel = 0;
	int maxIter = 0;
	int Level_Best_Solution = 0;
	int Iterations = 0;
	bool Rotate = false;
	int Tipo_Lista = 0;
	int Type_Strip = 1;
	bool Classic = false;
	bool FInstant = false;
	for (int Global_Iter = 0; Global_Iter <= Number_Of_Iterations; Global_Iter++)
	{
		if (Problemas[Global_Iter].G_iter_veces != 0)
			maxIter = Global_Iter;
		else
			continue;
		if (Problemas[Global_Iter].Best_Objective_function_Thread == Global_Best_Objective_Function)
		{
			if (Iter_Best == -1)
			{
				Global_Best_Solution = Problemas[Global_Iter].G_Best_Rsol_Items;
				Iter_Best = Global_Iter;
				Level_Best_Solution = Problemas[Global_Iter].Best_iter;
				Iterations=Problemas[Global_Iter].G_Iterations;
				Rotate= Problemas[Global_Iter].G_Rotate;
				if (Rotate == true)
					Global_Best_Solution_Rotated = Problemas[Global_Iter].G_Best_Solution_Rotated;
				Tipo_Lista= Problemas[Global_Iter].G_tipo_lista;
				Type_Strip = Problemas[Global_Iter].G_type_strip;
				Classic= Problemas[Global_Iter].G_Classic;
				FInstant = Problemas[Global_Iter].G_finish_instant;
			}
		}

		if (Problemas[Global_Iter].G_level > maxLevel)
			maxLevel = Problemas[Global_Iter].G_level;
	}
	if (Problemas[0].active_log_error) printf("Best_Solution %d", Global_Best_Objective_Function);


	//	printf("here1");
	Problemas[0].Best_Objective_function = Global_Best_Objective_Function;
	Problemas[0].G_Best_Rsol_Items = Global_Best_Solution;
	//	Problem.MakeFileTextSolution(file_idx2 + "_solution" + std::to_string(maxSeconds) + "_" + std::to_string(nThreads) + ".csv");
	if (Rotate == false)
		Problemas[0].MakeFileTextSolution(file_to_write);
	else
		Problemas[0].EscribirSolutionRotated(file_to_write, Global_Best_Solution_Rotated);
	//	printf("here2");



	ftime(&time_final);
	double tmp_first = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm - time_ini.millitm)) / 1000;

	//	Size = max((1000 / tmp_first) , 2);
	double tmp_second = 0;



	printf( "%s Time Ini %f   Best %d  Global Iter Best %d Max Iter %d MaxLevel %d Level Best Solution %d Iter %d Rotate %d Iterations %d Tipo Lista %d Tipo Strip %d Classic %d Finstant %d Iter %d\n", file_idx2.c_str(), tmp_first, Global_Best_Objective_Function, Iter_Best, maxIter, maxLevel, Level_Best_Solution, Iter,Rotate,Iterations,Tipo_Lista,Type_Strip,Classic,FInstant,Iter);

	if (!Problemas[0].opened_file)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;

}

*/