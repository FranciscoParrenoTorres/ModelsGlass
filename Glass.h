#ifndef GLASS_H
#define GLASS_H

#include "definitions.h"
#include <iostream>
#include <fstream>
#include <set>

using namespace std;

extern int Global_Best_Objective_Function;
extern int Param;
#ifdef _WIN32 
extern  int DibujarOpenGL();
#endif
class Item_orden;
class GlassPlate;
 class GlassRectangle;
 class GlassStack;
 class GlassItem;
class GlassDefect;
class Glass_Corte;


typedef std::pair <int, int> ParInt;

typedef  std::list< GlassDefect > LGlassDefect;

typedef std::pair< std::pair< int, int >, std::pair< int, int > >  Coordinates;

typedef std::list< GlassRectangle > LRectangles;

class Item_orden
{
public:
	Item_orden();
	virtual ~Item_orden();
	int item_id;
	bool rotado;
	int altura;
	int anchura;
	int stack_nbr;
	int number;
	bool  operator <(const Item_orden& p2) const
	{
		if (altura < p2.altura)
			return false;
		else
			return true;
	}


	//bool  operator simple(const Item_orden& p2) const
	//{
	//	if (min(altura,anchura) < min(p2.altura,p2.anchura))
	//		return false;
	//	else
	//		return true;
	//}

} ;

//bool PIECE::operator <(const PIECE& p2) const



class Glass
{
public:

	GlassPlate * plate; // List of solution used plates.

	GlassNode *sol_items; // List of solution nodes.
	GlassStack *stacks; // List of batch stacks.
	GlassItem *items; // List of batch items.

	LGlassDefect DefectsPlate[PLATES_NBR_LIMIT]; //List of defects per plate
	LGlassDefect DefectsPlateRotate[PLATES_NBR_LIMIT]; //List of defects per plate

	std::list< GlassPlate > G_Plate_Sol; // List of solution used plates.

			
										 
										 /// Optimization Parameters file Parser
	//Type=0 Roadef
	//Type=1 BerkeyWang
	//Type=2 2DCP
	//Type=4 Dimensions double
	int Type = 0;
	bool G_Dibujar = false;
	bool G_Draw_Exacto = false;
	int G_Exacto = 2;
	int  active_log = 1;
	int  active_log_error = 0;// Once log file is generated this var will be cleared.
//	std::ofstream log_file; // Reference to log file.

//	std::ofstream statistics_file; // Reference to statisticsLog file.
//	std::ofstream statistics_ins_file;
	bool opened_file = true; // Boolean to test if one of needed files (batch, solution, defects and optimization parameters) was not opened.

	std::string file_idx ; // To save used file index (Normal mode).
	std::string testPath ; // To save path according to chosen test.
	std::string instance ; // To save used file index (Test mode).
	std::string P_file_to_write;

	int  s_idx = 0; // Solution index, used precise where to insert solution node in sol_items array.
	int  plates_nbr = 0; // Save solution used plates number.
	int  stack_nbr = 0; // Save batch stack number.
	int  plates_list[PLATES_NBR_LIMIT][2];   // Save number of nodes for each plate used in user solution.
	
											 // plates_list[X][Y]:   X dimension contains plates Ids list
	bool P_pilas = false;
	bool P_tiras = false;
	int P_maxnivelesh = 0;
	int P_maxnivelesw = 0;
	int P_maxnivelesm = 0;
	bool P_trimming = false;
	bool P_orders = false;
	bool P_order_tiras = false;
	bool P_defects = false;
	bool P_minimo_waste = false;
	bool P_cota = false;
	bool P_heur = false;
	bool P_WithoutRotation = false;
	bool P_WithoutPiles = true;
	//Set of integers where there is a piece of the same size of others 
	vector< bool > P_set_xa;
	vector< bool > P_set_xar;
	vector< bool > P_set_xpa;
	vector< bool > P_set_xrpa;
	int P_BinsCotaArea = 0;
	int P_TrozoArea = 0;

	int P_piezas_juntas = 0;
	int P_piezas_juntas_posibilidades = 0;
	bool P_initial_solution = false;
	//                      Y dimension contains number of nodes of plate X
													 //change
	int  plate_defect[100][2];

	int  lines_nbr = 0; // csv file lines number.
	int  node_nbr = 0; // Solution file nodes number.
	int  useful_node = 0; // Solution useful nodes number (node not a branch or waste or residual).
	int  branch_node = 0; // Solution branch nodes number
	int  residual_node = 0; // Solution residual nodes number.
	int  waste_node = 0; // Solution waste nodes number.
	int  batch_items = 0; // Batch items number.
	int  constraint_error = 0; // Success constraint, combination of solution's occured errors (refer to main.h to find errors list).
	int  defects_nbr = 0; // Defects file defects number.

	int  total_waste = 0; // Sum of wasted area.
	int  total_useful = 0; // Sum of used area.
	int G_Param = 30;
	int max_cut_stage = 0; // Solution max used cut stage.

	int  Total_area = 0;
	int  plate_nbr_limit = 0; // To save Optimization parameters plate number limit constraint.
	int  plate_w = 0; // To save Optimization parameters plate's length constraint.
	int  plate_h = 0; // To save Optimization parameters plate's width constraint.
	int  min1Cut = 0; // To save Optimization parameters cut 1 & 2 min1Cut constraint.
	int  min2Cut = 0; // To save Optimization parameters cut 1 & 2 min2Cut constraint.
	int  max1Cut = 0; // To save Optimization parameters cut 1 & 2 max1Cut  constraint.
	int  waste_min = 0; // To save Optimization parameters cut 1 & 2 minWasteWidth constraint.

	int  success = 1;
	int Total_defects = 0;
	int MinDimensionPieza = MAXIMUM_INT_ROADEF;
	int MaxMinDimensionPieza = 0;
	int MinDimensionPieza_Inicial = MAXIMUM_INT_ROADEF;
	int MaxMinDimensionPieza_Inicial = 0;
	string name_instance; //name to the instance
	bool deterministic = false; //Variable to indicate if we use a deterministic version
	bool joint_items = false; //variable to indicate if we joint items equals
	bool silent = true; //Do not show anything 
	int Para_Random = 2;
	int Start_strip = 0;
	int MoreThanOne = 0;
	int  G_iter_veces = 0;
	int G_Time = 0;
	bool G_Classic = false;
	bool G_Rotate = false;
	int TrozoBin = 0;
	struct timeb G_Time_Initial ;
	
	int Total_area_defects = 0;
	int  Total_area_used = 0;//Suma of used area partial
	int  Total_width_bin = 0;
	int  Max_First_Bin = 0;
	int  Objective_function = 0;
	int  Partial_objective_function = 1000;
	int ManyDefects=1;
	//Conjunto de piezas que pueden ir debajo de la pieza i 
	//en horizontal la de abajo en horizontal la de arriba
	std::vector < std::set<int>> Dxx;
	//en vertical la de abajo en horizontal la de arriba
	std::vector < std::set<int>> Dyx;
	//en horizontal la de abajo en vertical la de arriba
	std::vector < std::set<int>> Dxy;
	//en vertical la de abajo en vertical la de arriba
	std::vector < std::set<int>> Dyy;
	//Conjunto de piezas que pueden ir encima de la pieza i 
	//en horizontal x, en vertical y
	std::vector < std::set<int>> Ax;
	std::vector < std::set<int>> Ay;
	int  Inicio = 0;
	int  Plates = 0;
	int  NumberOfPlate = 0;
	bool G_finish_instant =true;
	int SizeOfBeam = 1;
	int Hilo = 0;
	double Utilization = 0;
	double Best_Utilization = 0;
	int iter_finish = 0;
	std::list< GlassNodeB > ListaNodosBeam;
	std::list< GlassNodeB > ListaNodosBeam_Utilization2;
	std::list< GlassNodeB > ListaNodosBeam_G_children;
	std::list< GlassNodeB > ListaNodosBeam_G_children_Classical;
	int  seed = 1;
	int G_Iterations = 1;
	int G_Size_width = 1;
	int  Partial_objective_function_List = MAXIMUM_INT_ROADEF; //Least value of Partial_objective_function int percentaje per thousand
	int  Max_List_Nodos_Beam_Per_Nodo = 10;
	int  Max_List_Nodos_Beam = 100;
	int  Max_List_Nodos_Beam_Global = 50;
	int  Max_List_Nodos_Beam_Global_Double = 50;
	int  Max_List_Nodos_Beam_Global_Utilization = 50;
	int  Max_List_Nodos_Beam_Global_Global = 50;
	int  Initial_Objective_function = 0;
	int  Best_Objective_function = MAXIMUM_INT_ROADEF;
	int  Best_Objective_function_Thread = MAXIMUM_INT_ROADEF;
	int  Best_plates_nbr = 100;
	int  Best_iter = 0;
	int  G_iteraciones = 1500;
	int  G_level = 0;
	int  G_iter = 0;
	int  Duplicated = 0;
	int  G_children = 0;
	int  NumberOfPlates = 100;
	int  RestOfLastPlate = 0;
	int Best_Width = 0;
	int  Best_Width_First_Rotate = 0;
	int Best_Min_Perdida = MAXIMUM_INT_ROADEF;
	int Best_Solution_Exacto = 0;
	bool one_item_each = false;
	bool draw_solution = true;
	int  node_id = 0;
	int  rect_id = 0;
	std::list< GlassRsol>  G_Rsol_Items;
	std::list< GlassRsol>  G_Best_Rsol_Items;
	bool wider = false;
	bool higher = false;
	bool To_the_Top = false;
	int MinAncho = MAXIMUM_INT_ROADEF;
	int cont_plates = 0;

	int Max_children_Level = 10;
	int G_rotated_nodes = 0;
	//We have a vector por each stack an we save the item
	std::vector< std::vector < int > > G_Matrix_Items;
	// the size of this vector is the number of stacks, the first number is 
	//which is the first item possible to place
	std::vector<int > G_Area_Defectos_Bin;
	std::vector< int > G_Vector_Stacks;
	std::vector< bool > G_Vector_Items;
	std::vector< int > G_Random_Stacks;
	std::vector< Glass_Corte > G_Rotate_Solution;
	std::list< Glass_Corte > G_Solution_Rotated;
	std::list< Glass_Corte > G_Best_Solution_Rotated;
	int G_Total_Nodes = 0;
	int G_type_strip = 1;
	int G_tipo_lista = 0;
	int G_Repeticiones = 1;
	int G_Size_alpha = 3;
	int G_Size_Chosen = 1;
	int G_bad_solutions = 0;
	std::vector< int > G_Vector_Size_Stacks;
	bool G_first_time_node = false;
	int Inicio_Tira = 0;
	//VAriables Formulation
	short int maxTiras=0;
	short int maxPilas=0;
	short int maxPiezasPila = 0;
	short int numtiras=0;
	short int numpilas=0;
	short int numbins=0;
	bool Lazy = true;
	int Best_Value_Formulation=0; 
	int LB_Best_Value_Formulation=0;
	//Parallel
	std::vector< std::vector< std::vector < int > > >  GP_Matrix_Items;
	std::vector < std::vector< int > > GP_Vector_Stacks;
	std::vector < std::vector< bool > >GP_Vector_Items;
	std::vector < std::vector< int > > GP_Random_Stacks;

	std::vector < std::vector< int > > GP_Vector_Size_Stacks;
	std::vector < Item_orden > G_Vector_Ordenado_Altura;
	std::vector < Item_orden > G_Vector_Ordenado_Altura_Simple;
	//List with each coordinate is the item, the x's position, y's position, top corner x position, top corner y position
	std::vector< std::vector < int> > G_Positions_Items;
	//List with each coordinate is the item, the x's position, y's position, top corner x position, top corner y position
	std::vector< std::vector < int> > G_Best_Positions_Items;
	int m_id=0;
	//functions copy
	Glass();
	Glass(int i);
	Glass(string i);
	Glass(string i,int,int );
	virtual ~Glass();

	//functions
	bool BeamSearch(int Hilo,int );
	bool BeamSearchRotate( int Hilo, int);
	void TransformarSolucionSimetrica(string filep,bool);
	void CorteHorizontalSimetry(int ini, int fin);
	void CorteVerticalSimetry(int ini, int fin);

	bool BeamSearchParallel();
	std::pair<int, int > ItemGreater(int max_width, int max_height, short int tipo);
	std::pair<int, int > ItemGreaterRN(GlassRectangle &R);
	bool CheckSolution();

	bool CheckSolutionEnd();
	bool CheckSolutionPartial();
	bool CheckSolutionPartial(int Min);
	void PrintProblem(string problem);
	Coordinates ItemGreaterRN_J(GlassRectangle &R);
	void PutRSolType(std::list< GlassRsol > ::iterator  &it, int toright, int toup);
	void PutRSolTypeCurrent(std::list< GlassRsol > ::iterator  &it, int toright, int toup);
	Coordinates ItemGreaterRNRandom_J(GlassRectangle &R);
	std::pair<int, int > ItemGreaterRNRandom(GlassRectangle &R);
	std::pair<int, int > ItemGreaterRNAreaV(GlassRectangle &R);
	std::pair<int, int > ItemGreaterRNAreaH(GlassRectangle &R);
	std::pair<int, int > ItemGreaterRNH(GlassRectangle &R);
	std::pair<int, int > ItemGreaterRNV(GlassRectangle &R);
	Coordinates ItemGreaterRNH_J(GlassRectangle &);
	Coordinates ItemGreaterRNAreaV_J(GlassRectangle &);
	Coordinates ItemGreaterRNV_J(GlassRectangle &);
	int MinimaDimension(GlassRectangle &Rect, GlassRsol &Sol);
	Coordinates ComputeLowestRectangle_H(GlassRectangle &R);
	Coordinates ComputeLowestRectangle_V(GlassRectangle &);
	int TallestBeginning(std::list< GlassRsol > ::iterator  &, int, int);

	int BottomBeginning(std::list< GlassRsol > ::iterator  &it, int toright, int tall);
	Coordinates PlaceItem(Coordinates p, GlassRectangle &Rect);
	Coordinates PlaceItem(std::pair<int, int> p, GlassRectangle &Rect);
	bool NotEqualCurrent(GlassNodeB &node2);
	bool AlgoritmoRandom();
	bool Dominated(GlassNodeB &node2);
	void createNodeB(GlassNodeB &node3);
	bool InsertPartialSolution();
	bool InsertPartialSolutionClassical();
	bool SecondFilter();
	void SelectByGlobalObjectiveFunction();
	bool RemoveDominated();
	void RecoverPartialSolution(int i);
	bool BetterSolution(int a);
	void SymetricProblem();
	void DibujarChosen();
	void SaveSolution();

	
//	void DibujarOpenGL();
	//EN Linux poner return y comentar extern arriba
	//En Windows quitar y poner extern arriba

#ifdef __linux__ 
//linux code goes here
		void DibujarOpenGL() {
		return;
	};
#endif

	//void DibujarOpenGL() {
	//	return;
	//};
	//
	
	///


	
	//From a list of pieces, create the solution
	void MakeFileTextSolution(string path);
	void MakeFileTextSolutionOld(string path);
	void MakeFileTextSolutionNew(string path);

	int ThereIsSomethingInThisStrip(std::list< GlassRsol > ::iterator  &it, int ini, int fin, int fin_strip);



	//Look up into the list an item down of this
	bool NothingDown(std::vector< std::vector <int> > ::iterator  &);

	//Look up into the list an item left of this
	bool NothingLeft(std::vector< std::vector <int> > ::iterator  &, int);

	//The biggest piece in the horizontal strip
	int  NothingUp(std::vector< std::vector <int> > ::iterator  &it, int from, int tosize);

	int  NextRightBottom(std::vector< std::vector <int> > ::iterator  &it, int toright);
	int LessTopStrip(std::list< GlassRsol > ::iterator  &it, int toright);
	//somtehing up -1 or the value
	int  NothingUp(std::vector< std::vector <int> > ::iterator  &it, int toright);

	int  NextRight(std::vector< std::vector <int> > ::iterator  &it, int toright);

	int  NextRight(std::list< GlassRsol > ::iterator  &it, int toright, int toup);

	bool NextPut(std::list< GlassRsol > ::reverse_iterator  itk, int WID);

	int  TallestInTheStrip(std::vector< std::vector <int> > ::iterator  &it, int toright);

	int  BottomInTheStripNext(std::list< GlassRsol > ::iterator  &it, int toright);
	//Look up into the list an item down of this
	bool NothingDown(std::list< GlassRsol > ::iterator  &);

	//Look up into the list an item left of this
	bool NothingLeft(std::list< GlassRsol > ::iterator  &, int);

	//The biggest piece in the horizontal strip
	int  NothingUp(std::list< GlassRsol > ::iterator  &it, int from, int tosize);

	int  NextRightBottom(std::list< GlassRsol > ::iterator  &it, int toright);
	int  SecondTallestInTheStrip(std::list< GlassRsol > ::iterator  &it, int toright,int );
	//somtehing up -1 or the value
	int  NothingUp(std::list< GlassRsol > ::iterator  &it, int toright);

	int  NextRight(std::list< GlassRsol > ::iterator  &it, int toright);
	int  NextRightCurrent(std::list< GlassRsol > ::iterator  &it, int toright);
	int  TallestInTheStrip(std::list< GlassRsol > ::iterator  &it, int toright);

	void ChangeRectanglesSimmetry(int maxWidth);
	void WriteWastesBottom(FILE *files, int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int pa, int &nod, int pa_cut);
	void WriteWastesLeft(FILE *files, int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int pa, int &nod, int pa_cut);
	void WriteWastes(FILE *files, int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int &pa, int &nod, int &pa_cut);
	void PushWastesBottom( int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int pa, int &nod, int pa_cut);
	void PushWastesLeft( int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int pa, int &nod, int pa_cut);
	void PushWastes( int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int &pa, int &nod, int &pa_cut);

	void ChangeWidthRectangles();
	void  ChangeBase(int width);
	void  ChangeUnder(GlassRectangle &, int);
	bool Overlap(std::list<GlassRectangle>& lista, int x, int y, int X, int Y);
	bool OverlapFull(std::list<GlassRectangle>& lista, int x, int y, int X, int Y);
	bool OverlapFullDerecha(int x, int y, int X, int Y);
	bool OverlapFullArriba(int x, int y, int X, int Y);
	//	void ChangeRectangles(int Right, int Up, GlassRectangle &Rect, std::list< GlassRectangle> &lista, std::list< GlassDefect > ldefects);
	void Randomize(std::vector<int>& lista);
	bool NotOther(std::list<GlassRsol>::iterator & it, int fin, int inicio, int toright);
	bool AllStartsAtTheSameHeightInTheStrip(GlassRectangle &Rect);
	//Set how many are equals
	void JointItems();
	std::pair<int, int> ComputeIfBetterToDoChange(GlassRectangle & Rect, Coordinates & pitem);
	//Create Matrix
	void CreateMatrix();

	//Initialize Matrix
	void RestartMatrix();

	//Update Min Dimension
	void UpdateMinDimension(std::pair<int, int>);

	//Change position where place
	bool ChangePositionToPlace(GlassRectangle &R);

	//Write the solution to draw
	void WriteSolution(string filep = "temp2.txt");

	//Write the solution 
	void WritePlates();

	//Check Overlap rectangle with its defects
	bool Overlap(int x1, int y1, int X1, int Y1, GlassRectangle &R);
	bool Overlap(int x1, int y1, int X1, int Y1);
	void ChangeRectangles(int Right, int Up, GlassRectangle &Rect, std::list< GlassRectangle> &lista, std::list< GlassDefect > ldefects);

	//Choose
	std::pair<int, int > ItemGreaterHeight(int max_width, int max_height);
	std::pair<int, int > ItemGreaterWidth(int max_width, int max_height);

	bool AlgoritmoDeterminista();
	bool AsignarItemsPlate(GlassRectangle &);
	int AsignarItemsPlate_J(GlassRectangle &);

	GlassRectangle * AsignarRectangulo(GlassRectangle &rectangulo);

	//Erase an element from the list
	void EraseRectangleFromList(LRectangles &lista, GlassRectangle &R);

	void EraseRectangleFromList(LRectangles &lista, int id_rect);

	void ChangeRectangleFromList(LRectangles &, int);
	bool   CheckChangesToTheTop(int, int);
	bool   MakeFeasibleToTheTop(int, int);
	bool BeamSearch();
	void FinishSolutions();
	void FinishSolution();
	void PutObjectiveFunction(int);
	void InsertClassical();
	void InsertarSolucionGlobal();
	bool InsertPartialSolutionGlobal(GlassNodeB);
	inline int Distancia(GlassDefect &D, int x1, int y1)
	{
		return (((D.Getpos_x() - x1)*(D.Getpos_x() - x1)) + ((D.Getpos_y() - y1)*(D.Getpos_y() - y1)));
	}
	int randomfunc_1(int j);
	inline bool Overlap(int x1, int y1, int X1, int Y1, int x2, int y2, int X2, int Y2);
	inline bool Wastemin(int x1, int y1, int X1, int Y1, GlassRectangle & R);

	//Originals
	int parseBatchWithoutStacks(string path);
	int parseBatchWithoutStacksBerkey(string path);
	int parseOptimizationParams(string path);

	/// Batch file Parser
	int parseBatch2DC(string path);
	/// Batch file Parser
	int parseBatch(string path);

	/// Batch file Parser
	void parseBatchJointItems(string path);

	/// Solution file Parser
	int parseSolution(string path);


	/// Defects file Parser
	int parseDefects(string path);


	/// Data Structure Builder
	void buildDataStructure(GlassNode *node);


	/// Data Structure Builder
	void checkSuccessors(GlassNode *node, GlassNode *sol_items, int plate_id, GlassNode &parent, int c_end, int index);


	/// Create Plate Instance
	int createPlate(string *token, int p);


	/// Create Node Instance
	int createNode(GlassNode * node, string *token, int p);


	/// Create Item Instance
	int createItem(GlassItem *items, string *token, int p);


	/// Create Item Joint Instance
	bool createItemJoint(GlassItem *items, string *token, int p);

	/// Display Plates Area Usage
	void displayPlatesAreaUsage(void);


	/// Verify Identity & Sequence constraints
	void verifyIdt_Sequence(void);


	/// Verify Dimensions constraint
	void verifyDimensions(void);


	/// Verify Dimensions constraint of a node successors
	void checkSuccDimensions(GlassNode parent);


	/// Verify Defects overlap constraint
	void verifyDefects(void);


	/// Verify Item Production constraint
	void verifyItemProduction(void);


	/// Create statisticsLog log file
	void statisticsLog(void);

	/// List violated constraints.
	void violatedConstraints(void);

	///Old funciton verify
	int verify_main(void);

	//statisticsItemsDefects
	void statistics(void);
	int verify_main_interactive(void);

	int  explore(GlassNode n);
	bool NoItemBetweenTallMin(std::list< GlassRsol > ::iterator  &it, int toright, int tall);
	int  NextRightTop(std::list< GlassRsol > ::iterator  &it, int toright);
	//Parallel

	void CreateMatrixP(int Iter);
	bool BeamSearchParallel(int Iterations, int SizeOfBea, int Many, int Hil, int seed_id);
	void LevelEvaluation(int level,  GlassNodeB  &nodo);
	void RecoverPartialSolution(GlassNodeB &nodo);
	void DoubleEvaluation();
	bool InsertPartialSolutionGlobalUtil2(GlassNodeB node);
	bool NotInTheList(GlassNodeB &nodo);
	void ChangeDimensionsSimetry(Glass_Corte &d);
	int MaxWidthLastPiece(std::list< GlassRsol >  &lista_temp);
	int MinWidthLastPiece(std::list< GlassRsol >  &lista_temp);
	int get_random(const int & min, const int & max);
	void EscribirSolutionRotated(string filep, std::list< Glass_Corte> &);
	bool EstaYaPuesto(int);
	void RemoveNodo();
	void Randomize(int iter);
	void RandomSolution();

	//Formulacion CPLEX del problema

	void FormulationStages(short int, short int nt=1);
	void FormulationStagesOrig(short int nbins, short int nt=1);
	void knapsack_bouknap();
	void CrearVectorOrdenado(int how = 1);
	void CalcularAreaDefectosBin();
	void FormulationMultiKnapsack(short int nbins, short int nt);
	void CotaArea(); 
	void CalculoMaximoPilasNivelesEasy();
	void CalculoMaximoPilasNivelesKnapsack(int Val);
	void FormulacionCompleta4IndicesDefectosPilas(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes);

	void FormulationStagesMinimoCorte(short int nbins, short int nt, bool ,bool);
	void FormulationStagesIIMinimoCorte(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes);
	void FormulationCompleta4Indices(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes);
	//	void dibujar_solucion(NumVarMatrix2& x_ij, NumVector& z_i, int& numpedidos, int& numtiposcaja);
	void FormulationStagesIIIMinimoCorteTiras(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes);
	void GenerarInstanciasExacto();
	void ComputePiecesEquals();
	void knapsack_bouknap_tiras();
	int CalcularPerdidaPieza(int, int,int);
	void FormulationStagesIIIMax();
	int SubSetSumEntera(int Val);
	int BiKnapsackEntera(int Val);
	int MultiKnapsackEntera(int Val);
	void FormulacionCompleta4IndicesDefectos(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes);
	void FormulacionCompleta3Indices(short int nbins, short int nt, bool Lazy_minimo, bool Lazy_cortes);
	void FormulacionPuchinger(short int nbins, short int nt, bool Lazy_inimo, bool Lazy_cortes);
	void FormulationStagesOrigWastes(short int nbins, short int nt);
	void WriteSolutionCPlex(string path);
	void CalculateMax(void);
	int BuscarPosicion(int id, int height);
};


#endif // GLASS_H