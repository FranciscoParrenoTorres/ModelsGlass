#include "Glass.h"

Glass::Glass()
{
	//ctor
}



int Glass::randomfunc_1(int j)
{
	return rand() % j;
}

int Glass::get_random(const int & min, const int & max) {

	/*static thread_local mt19937 generator(seed + m_id);
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);*/
	seed = (seed * 1103515245 + 12345) & RAND_MAX_ROADEF;
return ((seed% (int)(((max)+1) - (min))) + (min));
//	boost::mt19937 generator(seed + m_id);
//	boost::random::uniform_int_distribution<int> distribution(min, max);
//	int kk = distribution(generator);
	
//	printf("%d %d %d %d %d \n",kk,seed,m_id,min,max);
 //		return kk;
//	return distribution(generator);

	//	return ((rand()% (int)(((max)+1) - (min))) + (min));
	//	return ((rand_r(&seed) % (int)(((max)+1) - (min))) + (min));
}

//Funciones de inicialización
Glass::Glass(int i)
{

}
Glass::Glass(string i)
{
	file_idx = i;
}
Glass::Glass(string i, int id, int s)
{
	m_id = id;
	seed = s + id;
	//	generator.seed(id+s);
	file_idx = i;
}
Glass::~Glass()
{
	//dtor
}
Item_orden::Item_orden()
{
	//ctor
}
Item_orden::~Item_orden()
{
	//dtor
}




inline bool Glass::Overlap(int x1, int y1, int X1, int Y1, int x2, int y2, int X2, int Y2)
{
	//Top
	if (y1 >= Y2) return false;
	//right
	if (x1 >= X2) return false;
	//Bottom
	if (Y1 <= y2) return false;
	//Left
	if (X1 <= x2) return false;
	return true;
}
inline bool Glass::Wastemin(int x1, int y1, int X1, int Y1, GlassRectangle & R)
{
	if (G_Rotate == false || Plates>1)
	{
		//	if (x1 > R.Getpos_x()) x1 = R.Getpos_x();
		//	if (y1 > R.Getpos_y()) y1 = R.Getpos_y();
		if (R.Get_y() == R.Getpos_y())
		{
			//Up and RIght
			if ((R.Getpos_X() - X1) >= waste_min && (R.Getpos_Y() - Y1) >= waste_min) return true;

			if ((plate_h - Y1) < waste_min && (plate_h - Y1) != 0) return false;
			if ((R.Getpos_Y() - Y1) < waste_min && (R.Getpos_Y() - Y1) != 0) return false;

			if ((plate_w - X1) < waste_min && (plate_w - X1) != 0) return false;
			if (R.Getsidecut() == 1) return true;

			if ((R.Getpos_X() - X1) < waste_min && (R.Getpos_X() - X1) != 0) return false;
			return true;
		}
		else
		{
			//top
			if (R.Getpos_Y() == Y1)
			{
				if ((R.Getpos_X() - X1) >= waste_min) return true;
				if ((plate_w - X1) < waste_min && (plate_w - X1) != 0) return false;
				if ((plate_h - Y1) < waste_min && (plate_h - Y1) != 0) return false;
				if ((y1 - R.Getpos_y()) < waste_min) return false;
				if (R.Getsidecut() == 1) return true;
				if ((R.Getpos_X() - X1) < waste_min && (R.Getpos_X() - X1) != 0) return false;

				return true;

			}
			if ((R.Get_y() - R.Getpos_y()) >= waste_min)
			{

				if ((plate_w - X1) < waste_min && (plate_w - X1) != 0) return false;
				if ((plate_h - Y1) < waste_min && (plate_h - Y1) != 0) return false;
				if ((R.Getpos_X() - X1) >= waste_min) return true;
				if (R.Getsidecut() == 1) return true;
				if ((R.Getpos_X() - X1) < waste_min && (R.Getpos_X() - X1) != 0) return false;
				return true;

			}
			else
			{
				//Up and RIght
				if ((R.Getpos_X() - X1) >= waste_min && (R.Getpos_Y() - Y1) >= waste_min) return true;

				if ((plate_h - Y1) < waste_min && (plate_h - Y1) != 0) return false;
				if ((R.Getpos_Y() - Y1) < waste_min && (R.Getpos_Y() - Y1) != 0) return false;

				if ((plate_w - X1) < waste_min && (plate_w - X1) != 0) return false;
				if (R.Getsidecut() == 1) return true;

				if ((R.Getpos_X() - X1) < waste_min && (R.Getpos_X() - X1) != 0) return false;
				return true;
			}

		}
		return true;

	}
	else //If ROTATE && Plate==1
	{
		//	if (x1 > R.Getpos_x()) x1 = R.Getpos_x();
		//	if (y1 > R.Getpos_y()) y1 = R.Getpos_y();
		if (R.Get_y() == R.Getpos_y())
		{
			//Up and RIght
			if ((R.Getpos_X() - X1) >= waste_min && (R.Getpos_Y() - Y1) >= waste_min)
				return true;
			//quitado
			//		if ((plate_h - Y1) < waste_min && (plate_h - Y1) != 0) return false;
			if ((R.Getpos_Y() - Y1) < waste_min && (R.Getpos_Y() - Y1) != 0) return false;

			if ((plate_w - X1) < waste_min && (plate_w - X1) != 0) return false;

			if (R.Getsidecut() == 1) return true;

			if ((plate_w != R.Getpos_X()) && ((R.Getpos_X() - X1) < waste_min && (R.Getpos_X() - X1) != 0))
				return false;
			return true;
		}
		else
		{
			//top
			if (R.Getpos_Y() == Y1)
			{

				if ((R.Getpos_X() - X1) >= waste_min) return true;

				//quitado
				//				if ((plate_w - X1) < waste_min && (plate_w - X1) != 0) return false;

				//			if ((plate_h - Y1) < waste_min && (plate_h - Y1) != 0) return false;
				if ((y1 - R.Getpos_y()) < waste_min) return false;
				if (R.Getsidecut() == 1) return true;
				if ((plate_w != R.Getpos_X()) && ((R.Getpos_X() - X1) < waste_min && (R.Getpos_X() - X1) != 0)) return false;

				return true;

			}
			if ((R.Get_y() - R.Getpos_y()) >= waste_min)
			{


				if ((plate_w - X1) < waste_min && (plate_w - X1) != 0) return false;


				//			if ((plate_h - Y1) < waste_min && (plate_h - Y1) != 0) return false;
				if ((R.Getpos_X() - X1) >= waste_min) return true;
				if (R.Getsidecut() == 1) return true;
				if ((plate_w != R.Getpos_X()) && ((R.Getpos_X() - X1) < waste_min && (R.Getpos_X() - X1) != 0)) return false;
				return true;

			}
			else
			{
				//Up and RIght
				if ((R.Getpos_X() - X1) >= waste_min && (R.Getpos_Y() - Y1) >= waste_min) return true;

				if ((plate_h - Y1) < waste_min && (plate_h - Y1) != 0) return false;
				if ((R.Getpos_Y() - Y1) < waste_min && (R.Getpos_Y() - Y1) != 0) return false;


				//					if ((plate_w - X1) < waste_min && (plate_w - X1) != 0) return false;
				//				if ((plate_w - X1) < waste_min && (plate_w - X1) != 0) return false;



				if (R.Getsidecut() == 1) return true;

				if ((plate_w != R.Getpos_X()) && ((R.Getpos_X() - X1) < waste_min && (R.Getpos_X() - X1) != 0)) return false;
				return true;
			}

		}

		return true;
	}
	return true;
}




//If tipo equals to 2 is width otherwise is height
std::pair<int, int > Glass::ItemGreaterRN(GlassRectangle &R)
{


	if (R.Getsidecut() == 2 || (3 * R.Get_w()) < (R.Get_h()))
	{
		return ItemGreaterRNH(R);
		//		return ItemGreaterRNV(R);
	}
	if (R.Getsidecut() != 2 || ((3 * R.Get_w()) > R.Get_h()))
	{
		if (R.Getsidecut() == 3)
			//	return ItemGreaterRNV(R);
			return ItemGreaterRNAreaV(R);
		else
			return ItemGreaterRNH(R);
	}
	return ItemGreaterRNV(R);
	//	return ItemGreaterRNH(R);
}

//If tipo equals to 2 is width otherwise is height
Coordinates Glass::ItemGreaterRN_J(GlassRectangle &R)
{


	std::pair< int, int > kp(1, 1);
	std::pair< int, int > kp2(-1, 0);
	Coordinates pk3(kp2, kp);
	int alea = get_random(0, 5);
	if (R.Getsidecut() == 2 || (3 * R.Get_w()) < (R.Get_h()))
	{
		if (alea <= 7)
			return ItemGreaterRNH_J(R);
		if (alea <= 9)
			return ItemGreaterRNAreaV_J(R);
		return ItemGreaterRNH_J(R);
		//		return ItemGreaterRNV(R);
	}
	if (R.Getsidecut() != 2 || ((3 * R.Get_w()) > R.Get_h()))
	{
		if (R.Getsidecut() == 3)
			//	return ItemGreaterRNV(R);
		{
			if (alea <= 3)
				return ItemGreaterRNAreaV_J(R);
			return ItemGreaterRNV_J(R);


		}
		else
		{

			if (alea <= 7)
				return ItemGreaterRNH_J(R);
			if (alea <= 9)
				return ItemGreaterRNAreaV_J(R);
			return ItemGreaterRNV_J(R);
		}
	}

	if (alea <= 7)
		return ItemGreaterRNH_J(R);
	if (alea <= 9)
		return ItemGreaterRNAreaV_J(R);
	return ItemGreaterRNH_J(R);


	//	return ItemGreaterRNH(R);
}
//we generate a new structure 
void Glass::SymetricProblem()
{
}
void Glass::PutRSolType(std::list< GlassRsol > ::iterator  &it, int toright, int toup)
{
	if (it == G_Best_Rsol_Items.end()) return;

	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (it2 = it; it2 != G_Best_Rsol_Items.end(); it2++)
	{

		if (NextRight(it2, toright) == -1)
		{
			(*it2).RsolType(1);
			return;
		}
		else
			(*it2).RsolType(2);

	}
	return;
}
void Glass::PutRSolTypeCurrent(std::list< GlassRsol > ::iterator  &it, int toright, int toup)
{
	if (it == G_Rsol_Items.end()) return;

	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Rsol_Items.end(); it2++)
	{

		if (NextRightCurrent(it2, toright) == -1)
		{
			(*it2).RsolType(1);
			return;
		}
		else
			(*it2).RsolType(1);

	}
	return;
}
//If tipo equals to 2 is width otherwise is height
Coordinates Glass::ItemGreaterRNRandom_J(GlassRectangle &R)
{


	To_the_Top = false;
	std::pair< int, int > kp(1, 1);
	std::pair< int, int > kp2(-1, 0);
	Coordinates pk3(kp2, kp);
	int random = get_random(0, 10);
	if (Param <= 1)
		random = 1;

	//	if (deterministic == true)
	//	random = 1;
	//	if (R.Getsidecut() == 2 || ((R.Get_w()) <= (R.Get_h()) && R.Getsidecut() != 3))
	if (R.Getsidecut() == 2)
	{
		if (random <= 6)
			return ItemGreaterRNH_J(R);
		if (random <= 9)
			return ItemGreaterRNAreaV_J(R);
		if (random<11)
			return ItemGreaterRNV_J(R);
		//		return ItemGreaterRNV(R);
	}

	if (R.Getsidecut() == 3)
		//	return ItemGreaterRNV(R);
	{
		if (random <= 5)
			return ItemGreaterRNAreaV_J(R);
		if (random <= 8)
			return ItemGreaterRNV_J(R);
		if (random <= 10)
			return ItemGreaterRNH_J(R);


	}

	//New 17/01/2019
	if ((2 * (plate_w - R.Getpos_x())) < (R.Get_h()))
	{
		//if R.Getsidecut()==1
		if (random <= 5)
			return ItemGreaterRNH_J(R);
		if (random <= 8)
			return ItemGreaterRNAreaV_J(R);
		if (random < 11)
			return ItemGreaterRNV_J(R);
	}
	//
	if ((R.Get_w()) >(R.Get_h()))
	{
		//if R.Getsidecut()==1
		if (random <= 4)
			return ItemGreaterRNH_J(R);
		if (random <= 7)
			return ItemGreaterRNAreaV_J(R);
		if (random < 11)
			return ItemGreaterRNV_J(R);
	}
	else
	{
		//if R.Getsidecut()==1
		if (random <= 4)
			return ItemGreaterRNV_J(R);
		if (random <= 7)
			return ItemGreaterRNAreaV_J(R);
		if (random < 11)
			return ItemGreaterRNH_J(R);
	}
	return ItemGreaterRNH_J(R);//change 21/09


							   //	return ItemGreaterRNH(R);
}
/*

//If tipo equals to 2 is width otherwise is height
Coordinates Glass::ItemGreaterRNRandom_J(GlassRectangle &R)
{


To_the_Top = false;
std::pair< int, int > kp(1, 1);
std::pair< int, int > kp2(-1, 0);
Coordinates pk3(kp2, kp);
int random = get_random(0, 10);

//	if (deterministic == true)
//	random = 1;
if (R.Getsidecut() == 2 || ((R.Get_w()) <= (R.Get_h()) && R.Getsidecut() != 3))
{
if (random <= 6)
return ItemGreaterRNH_J(R);
if (random <= 9)
return ItemGreaterRNAreaV_J(R);
if (random<11)
return ItemGreaterRNV_J(R);
//		return ItemGreaterRNV(R);
}

if (R.Getsidecut() == 3)
//	return ItemGreaterRNV(R);
{
if (random <= 5)
return ItemGreaterRNAreaV_J(R);
if (random <= 8)
return ItemGreaterRNV_J(R);
if (random <= 10)
return ItemGreaterRNH_J(R);


}
//if R.Getsidecut()==1
if (random <= 4)
return ItemGreaterRNH_J(R);
if (random <= 7)
return ItemGreaterRNAreaV_J(R);
if (random<11)
return ItemGreaterRNV_J(R);

return ItemGreaterRNH_J(R);//change 21/09


//	return ItemGreaterRNH(R);
}
*/
//If tipo equals to 2 is width otherwise is height
std::pair<int, int > Glass::ItemGreaterRNRandom(GlassRectangle &R)
{


	if (R.Getsidecut() == 2 || (3 * R.Get_w()) < (R.Get_h()))
	{
		int random = get_random(0, 10);
		if (random <= 5)
			return ItemGreaterRNH(R);
		if (random <= 6)
			return ItemGreaterRNV(R);
		if (random <= 9)
			return ItemGreaterRNAreaH(R);
		if (random <= 10)
			return ItemGreaterRNAreaV(R);
	}
	if (R.Getsidecut() != 2 || ((3 * R.Get_w()) > R.Get_h()))
	{
		if (R.Getsidecut() == 3)
		{
			//	return ItemGreaterRNV(R);
			if (get_random(0, 4) <= 3)
				return ItemGreaterRNAreaV(R);
			else
				return ItemGreaterRNV(R);
		}
		else
		{
			if (get_random(0, 4) <= 3)
				return ItemGreaterRNH(R);
			else
				return ItemGreaterRNAreaH(R);
		}

	}
	int random = get_random(0, 10);
	if (random <= 3)
		return ItemGreaterRNV(R);
	if (random <= 6)
		return ItemGreaterRNAreaV(R);
	if (random <= 8)
		return ItemGreaterRNAreaH(R);
	if (random <= 10)
		return ItemGreaterRNH(R);
	return ItemGreaterRNH(R);
}
//over
bool Glass::Overlap(int x1, int y1, int X1, int Y1, GlassRectangle &R)
{
	if (R.Getdefectlist().size() == 0)
		return false;
	for (std::list< GlassDefect > ::iterator it2 = R.Getdefectlist().begin(); it2 != R.Getdefectlist().end(); it2++)
	{
		if (Overlap(x1, y1, X1, Y1, (*it2).Getpos_x(), (*it2).Getpos_y(), (*it2).Getpos_x() + (*it2).Getwidth(), (*it2).Getpos_y() + (*it2).Getheight()) == true)
			return true;
	}
	return false;
}
/*
bool Overlap(int x1, int y1, int X1, int Y1, GlassRectangle &R)
{
if (R.Getdefectlist().size() == 0)
return false;
for (std::list< GlassDefect > ::iterator it2 = R.Getdefectlist().begin(); it2 != R.Getdefectlist().end(); it2++)
{
if (Overlap(x1, y1, X1, Y1, (*it2).Getpos_x(), (*it2).Getpos_y(), (*it2).Getpos_x() + (*it2).Getwidth(), (*it2).Getpos_y() + (*it2).Getheight()) == true)
return true;
}
return false;
}*/

void Glass::CreateMatrix()
{
	for (register int i = 0; i < batch_items; i++)
	{
		G_Vector_Items.push_back(false);
	}
	//Create vector
	for (register int i = 0; i < stack_nbr; i++)
	{
		//		std::pair<int, int > P(stacks[i].Getitem_nbr(), 0);
		G_Vector_Size_Stacks.push_back(stacks[i].Getitem_nbr());
		G_Vector_Stacks.push_back(0);
		G_Random_Stacks.push_back(i);
		//		G_Vector_Items.push_back(false);
	}
	for (register int i = 0; i < stack_nbr; i++)
	{
		std::vector <int> vec;
		if (G_Rotate == false)
		{
			for (register int j = 0; j < stacks[i].Getitem_nbr(); j++)
			{

				vec.push_back(stacks[i].Getitem(j).Getitem_id());
			}
		}
		else
		{
			for (register int j = stacks[i].Getitem_nbr() - 1; j >= 0; j--)
			{

				vec.push_back(stacks[i].Getitem(j).Getitem_id());
			}
		}
		G_Matrix_Items.push_back(vec);

	}

}

void Glass::CreateMatrixP(int Iter)
{
	for (register int j = 0; j < Iter; j++)
	{
		std::vector < bool > vec;
		for (register int i = 0; i < batch_items; i++)
		{
			vec.push_back(false);
		}
		GP_Vector_Items.push_back(vec);
	}
	//Create vector

	for (register int j = 0; j < Iter; j++)
	{
		std::vector < int > vec;
		std::vector < int > vec1;
		std::vector < int > vec2;
		for (register int i = 0; i < stack_nbr; i++)
		{
			//		std::pair<int, int > P(stacks[i].Getitem_nbr(), 0);
			vec.push_back(stacks[i].Getitem_nbr());
			vec1.push_back(0);
			vec2.push_back(i);
			//		G_Vector_Items.push_back(false);
		}
		GP_Vector_Size_Stacks.push_back(vec);
		GP_Vector_Stacks.push_back(vec1);
		GP_Random_Stacks.push_back(vec2);
	}
	for (register int k = 0; k < Iter; k++)
	{
		std::vector< std::vector <int >> matrix;
		for (register int i = 0; i < stack_nbr; i++)
		{
			std::vector <int> vec;
			for (register int j = 0; j < stacks[i].Getitem_nbr(); j++)
			{

				vec.push_back(stacks[i].Getitem(j).Getitem_id());
			}
			matrix.push_back(vec);

		}
		GP_Matrix_Items.push_back(matrix);
	}

}
void Glass::RestartMatrix()
{
	for (register int i = 0; i < batch_items; i++)
	{
		G_Vector_Items[i] = (false);
	}
	//Create vector
	for (register int i = 0; i < stack_nbr; i++)
	{
		G_Vector_Size_Stacks[i] = (stacks[i].Getitem_nbr());
		G_Vector_Stacks[i] = 0;
		//		G_Vector_Items.push_back(false);
	}
	for (register int i = 0; i < stack_nbr; i++)
	{
		if (G_Rotate == false)
		{
			for (register int j = 0; j < stacks[i].Getitem_nbr(); j++)
			{
				G_Matrix_Items[i][j] = stacks[i].Getitem(j).Getitem_id();

			}
		}
		else
		{
			for (register int j = 0; j < stacks[i].Getitem_nbr(); j++)
			{
				G_Matrix_Items[i][j] = stacks[i].Getitem((stacks[i].Getitem_nbr() - 1) - j).Getitem_id();

			}
		}



	}
}
//find de largest item that is lower than max_width and max_height
std::pair<int, int >  Glass::ItemGreaterHeight(int max_width, int max_height)
{

	int maximum = 0;
	int max_item = -1;
	std::pair<int, int > pk(-1, 0);
	for (register int i = 0; i < stack_nbr - 1; i++)
	{
		//If there are items in this stacks
		if (G_Vector_Stacks[i]  < (G_Vector_Size_Stacks[i]))
		{
			//If can not fii in that hole
			if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > max_width ||
				items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > max_height)
				continue;
			// if it can fit in width 
			if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() < max_width)
			{
				if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() < max_height)
				{
					if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum)
					{
						max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
						maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
					}

				}
				else
				{
					if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() < max_width)
					{
						if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum)
						{
							max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
							maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
						}
					}
				}
			}
			else
			{
				if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum)
				{
					max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
					maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
				}

			}
		}
	}
	pk.first = max_item;
	pk.second = maximum;
	return pk;

}

std::pair<int, int > Glass::ItemGreaterWidth(int max_width, int max_height)
{
	int maximum = 0;
	int max_item = -1;
	std::pair<int, int > pk(-1, 0);
	for (register int i = 0; i < stack_nbr - 1; i++)
	{
		//If there are items in this stacks
		if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
		{
			//If can not fii in that hole
			if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > max_width ||
				items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > max_height)
				continue;
			// if it can fit in height
			if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() < max_height)
			{
				if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() < max_width)
				{
					if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum)
					{
						max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
						maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
					}

				}
				else
				{
					if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() < max_height)
					{
						if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum)
						{
							max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
							maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
						}
					}
				}
			}
			else
			{
				if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum)
				{
					max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
					maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
				}

			}
		}
	}
	pk.first = max_item;
	pk.second = maximum;
	return pk;
}

//If tipo equals to 2 is width otherwise is height
std::pair<int, int > Glass::ItemGreater(int max_width, int max_height, short int tipo)
{
	bool change = false;
	if (tipo == 1 && (3 * max_width) < (max_height))
		change = true;
	if (tipo != 2 && ((3 * max_width)>max_height))
	{
		int temp = max_width;
		max_width = max_height;
		max_height = temp;
	}

	int maximum = 0;
	int max_item = -1;
	std::pair<int, int > pk(-1, 0);
	for (register int i = 0; i < stack_nbr; i++)
	{
		//If there are items in this stacks
		if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
		{
			//If can not fii in that hole
			if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > max_width ||
				items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > max_height)
				continue;
			// if it can fit in height
			if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() <= max_height)
			{
				if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= max_width)
				{
					if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum)
					{
						max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
						maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
					}

				}
				else
				{
					if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= max_height)
					{
						if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum)
						{
							max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
							maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
						}
					}
				}
			}
			else
			{
				if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum)
				{
					max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
					maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
				}

			}
		}
	}
	pk.first = max_item;
	pk.second = maximum;
	if (pk.first != -1 && change == true)
	{
		if (items[pk.first].Getitem_h() == pk.second)
			pk.second = items[pk.first].Getitem_w();
		else
			pk.second = items[pk.first].Getitem_h();
	}
	return pk;
}
bool Glass::Overlap(int x, int y, int X, int Y)
{

	/*	for (std::list <GlassRectangle> ::iterator it = lista.begin(); it != lista.end(); it++)
	{
	for (std::list < GlassDefect > ::iterator itD = (*it).Getdefectlist().begin(); itD != (*it).Getdefectlist().end() ; itD++)
	{
	if (Overlap(x, y, X, Y, (*itD).Getpos_x(), (*itD).Getpos_y(), (*itD).Getpos_x() + (*itD).Getwidth(), (*itD).Getpos_y() + (*itD).Getheight()))
	return true;
	}
	}*/
	if (G_Rotate == false)
	{
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[NumberOfPlate].begin(); itD != DefectsPlate[NumberOfPlate].end(); itD++)
		{
			if (Overlap(x, y, X, Y, (*itD).Getpos_x(), (*itD).Getpos_y(), (*itD).Getpos_x() + (*itD).Getwidth(), (*itD).Getpos_y() + (*itD).Getheight()))
				return true;
		}
	}
	else
	{
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[NumberOfPlate].begin(); itD != DefectsPlate[NumberOfPlate].end(); itD++)
		{
			if (Overlap(x, y, X, Y, plate_w - ((*itD).Getpos_x() + (*itD).Getwidth()), plate_h - ((*itD).Getpos_y() + (*itD).Getheight()), plate_w - ((*itD).Getpos_x()), plate_h - (*itD).Getpos_y()))
				return true;
		}
	}
	return false;

}
//side es si es a la derecha o arriba
//bool Glass::OverlapFullDerecha(int x, int y, int X, int Y)
bool Glass::OverlapFullDerecha(int x, int y, int X, int Y)
{

	/*	for (std::list <GlassRectangle> ::iterator it = lista.begin(); it != lista.end(); it++)
	{
	for (std::list < GlassDefect > ::iterator itD = (*it).Getdefectlist().begin(); itD != (*it).Getdefectlist().end() ; itD++)
	{
	if (Overlap(x, y, X, Y, (*itD).Getpos_x(), (*itD).Getpos_y(), (*itD).Getpos_x() + (*itD).Getwidth(), (*itD).Getpos_y() + (*itD).Getheight()))
	return true;
	}
	}*/
	if (G_Rotate == false)
	{
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[NumberOfPlate].begin(); itD != DefectsPlate[NumberOfPlate].end(); itD++)
		{
			if (Overlap(x, y, X, Y, (*itD).Getpos_x(), (*itD).Getpos_y(), (*itD).Getpos_x() + (*itD).Getwidth(), (*itD).Getpos_y() + (*itD).Getheight()))
				return true;
		}
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[NumberOfPlate].begin(); itD != DefectsPlate[NumberOfPlate].end(); itD++)
		{
			//
			if ((X > (*itD).Getpos_x()) && (X < ((*itD).Getpos_x() + (*itD).Getwidth())))
				return true;
		}

	}
	else
	{
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[NumberOfPlate].begin(); itD != DefectsPlate[NumberOfPlate].end(); itD++)
		{
			if (Overlap(x, y, X, Y, plate_w - ((*itD).Getpos_x() + (*itD).Getwidth()), plate_h - ((*itD).Getpos_y() + (*itD).Getheight()), plate_w - ((*itD).Getpos_x()), plate_h - (*itD).Getpos_y()))
				return true;
				if ((X > (plate_w - ((*itD).Getpos_x() + (*itD).Getwidth()))) && (X < (plate_w - ((*itD).Getpos_x()))))
					return true;
		}
	}
	return false;

}
//side es si es a la derecha o arriba
//bool Glass::OverlapFullDerecha(int x, int y, int X, int Y)
bool Glass::OverlapFullArriba(int x, int y, int X, int Y)
{

	/*	for (std::list <GlassRectangle> ::iterator it = lista.begin(); it != lista.end(); it++)
	{
	for (std::list < GlassDefect > ::iterator itD = (*it).Getdefectlist().begin(); itD != (*it).Getdefectlist().end() ; itD++)
	{
	if (Overlap(x, y, X, Y, (*itD).Getpos_x(), (*itD).Getpos_y(), (*itD).Getpos_x() + (*itD).Getwidth(), (*itD).Getpos_y() + (*itD).Getheight()))
	return true;
	}
	}*/
	if (G_Rotate == false)
	{
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[NumberOfPlate].begin(); itD != DefectsPlate[NumberOfPlate].end(); itD++)
		{
			if (Overlap(x, y, X, Y, (*itD).Getpos_x(), (*itD).Getpos_y(), (*itD).Getpos_x() + (*itD).Getwidth(), (*itD).Getpos_y() + (*itD).Getheight()))
				return true;
		}
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[NumberOfPlate].begin(); itD != DefectsPlate[NumberOfPlate].end(); itD++)
		{
			//
			if (Total_width_bin<= (*itD).Getpos_x())
				continue;
			if (Start_strip >= ((*itD).Getpos_x()+(*itD).Getwidth()))
					continue;
			if ((Y > (*itD).Getpos_y()) && (X < ((*itD).Getpos_y() + (*itD).Getheight())) )
				return true;
		}

	}
	else
	{
		for (std::list < GlassDefect > ::iterator itD = DefectsPlate[NumberOfPlate].begin(); itD != DefectsPlate[NumberOfPlate].end(); itD++)
		{
			if (Overlap(x, y, X, Y, plate_w - ((*itD).Getpos_x() + (*itD).Getwidth()), plate_h - ((*itD).Getpos_y() + (*itD).Getheight()), plate_w - ((*itD).Getpos_x()), plate_h - (*itD).Getpos_y()))
				return true;
			if (Total_width_bin <= (plate_w - ((*itD).Getpos_x() + (*itD).Getwidth())))
				continue;
				if (Start_strip >= ((plate_w - ((*itD).Getpos_x()))))
					continue;
			if ((Y > (plate_h - ((*itD).Getpos_y() + (*itD).Getheight()))) && (Y < (plate_h - ((*itD).Getpos_y()))))
				return true;
		}
	}
	return false;

}
bool Glass::CheckSolution()
{
	return true;
	if (active_log == false)	return true;
	if (G_Rsol_Items.size() == 0)
		return true;

	std::list< GlassRsol > ::iterator it, it_2;
	for (it = G_Rsol_Items.begin(); it != G_Rsol_Items.end(); it++)
	{
		//está fuera del plate 
		if ((*it).x() > plate_w || (*it).y() > plate_h || (*it).X() > plate_w || (*it).Y() > plate_h)
			PrintProblem("Alguna pieza fuera");
		if ((*it).X() > (plate_w - waste_min) && (*it).X() != (plate_w))
		{
			printf("Pieza x %d y %d X %d Y %d\t", (*it).x(), (*it).y(), (*it).X(), (*it).Y());

			PrintProblem("La X mayor que plate w menos 20");

		}
		if ((*it).Y() > (plate_h - waste_min) && (*it).Y() != (plate_h))
			PrintProblem("La Y mayor que plate w menos 20");
		if (G_Rotate == false)
		{

			for (std::list< GlassDefect > ::iterator it2 = DefectsPlate[(*it).plateId()].begin(); it2 != DefectsPlate[(*it).plateId()].end(); it2++)
			{
				bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it2).Getpos_x(), (*it2).Getpos_y(), (*it2).Getpos_x() + (*it2).Getwidth(), (*it2).Getpos_y() + (*it2).Getheight());

				if (overlapping == true)
				{
					printf("Pieza x %d y %d X %d Y %d\t", (*it).x(), (*it).y(), (*it).X(), (*it).Y());
					printf("Defecto x %d y %d X %d Y %d Rotate %d\t", (*it2).Getpos_x(), (*it2).Getpos_y(), (*it2).Getpos_x() + (*it2).Getwidth(), (*it2).Getpos_y() + (*it2).Getheight(), G_Rotate);
					PrintProblem("Pieza Corta defecto\n");
				}
			}
		}
		else
		{
			for (std::list< GlassDefect > ::iterator it2 = DefectsPlate[NumberOfPlates - (*it).plateId()].begin(); it2 != DefectsPlate[NumberOfPlates - (*it).plateId()].end(); it2++)
			{
				bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(),
					plate_w - ((*it2).Getpos_x() + (*it2).Getwidth()), plate_h - ((*it2).Getpos_y() + (*it2).Getheight()), plate_w - (*it2).Getpos_x(), plate_h - (*it2).Getpos_y());
				if (overlapping == true)
					PrintProblem("Pieza Corta defecto");
			}
		}
		it_2 = it;
		it_2++;
		if (it_2 == G_Rsol_Items.end())
			continue;
		//Items from different plates
		if ((*it).plateId() != (*it_2).plateId())
			continue;
		for (; it_2 != G_Rsol_Items.end(); it_2++)
		{
			if ((*it).plateId() != (*it_2).plateId())
				continue;
			bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it_2).x(), (*it_2).y(), (*it_2).X(), (*it_2).Y());
			if (overlapping == true)
				PrintProblem("Pieza Corta Pieza");
		}

		//		fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it)[0], items[(*it)[0]].Getitem_stack(), items[(*it)[0]].Getitem_seq(), (*it)[1], (*it)[2], (*it)[3], (*it)[4], (*it)[5]);
	}
	return false;

}
bool Glass::CheckSolutionEnd()
{

	if (G_Rsol_Items.size() == 0)
		return true;

	std::list< GlassRsol > ::iterator it, it_2;
	for (it = G_Rsol_Items.begin(); it != G_Rsol_Items.end(); it++)
	{
		//está fuera del plate 
		if ((*it).x() > plate_w || (*it).y() > plate_h || (*it).X() > plate_w || (*it).Y() > plate_h)
		{
			//		PrintProblem("Alguna pieza fuera");
			return false;
		}

		if ((*it).X() > (plate_w - waste_min) && (*it).X() != (plate_w))
		{
			//			PrintProblem("La X mayor que plate w menos 20");
			return false;
		}
		if ((*it).Y() > (plate_h - waste_min) && (*it).Y() != (plate_h))
		{
			//		PrintProblem("La Y mayor que plate w menos 20");
			return false;
		}
		if (G_Rotate == false)
		{

			for (std::list< GlassDefect > ::iterator it2 = DefectsPlate[(*it).plateId()].begin(); it2 != DefectsPlate[(*it).plateId()].end(); it2++)
			{
				bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it2).Getpos_x(), (*it2).Getpos_y(), (*it2).Getpos_x() + (*it2).Getwidth(), (*it2).Getpos_y() + (*it2).Getheight());

				if (overlapping == true)
				{
					//				printf("Pieza x %d y %d X %d Y %d\t", (*it).x(), (*it).y(), (*it).X(), (*it).Y());
					//			printf("Defecto x %d y %d X %d Y %d Rotate %d\t", (*it2).Getpos_x(), (*it2).Getpos_y(), (*it2).Getpos_x() + (*it2).Getwidth(), (*it2).Getpos_y() + (*it2).Getheight(), G_Rotate);
					return false;
				}
			}
		}
		else
		{
			for (std::list< GlassDefect > ::iterator it2 = DefectsPlate[NumberOfPlates - (*it).plateId()].begin(); it2 != DefectsPlate[NumberOfPlates - (*it).plateId()].end(); it2++)
			{
				bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(),
					plate_w - ((*it2).Getpos_x() + (*it2).Getwidth()), plate_h - ((*it2).Getpos_y() + (*it2).Getheight()), plate_w - (*it2).Getpos_x(), plate_h - (*it2).Getpos_y());
				if (overlapping == true)
					return false;
			}
		}
		it_2 = it;
		it_2++;
		if (it_2 == G_Rsol_Items.end())
			continue;
		//Items from different plates
		if ((*it).plateId() != (*it_2).plateId())
			continue;
		for (; it_2 != G_Rsol_Items.end(); it_2++)
		{
			if ((*it).plateId() != (*it_2).plateId())
				continue;
			bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it_2).x(), (*it_2).y(), (*it_2).X(), (*it_2).Y());
			if (overlapping == true)
				return false;
		}

		//		fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it)[0], items[(*it)[0]].Getitem_stack(), items[(*it)[0]].Getitem_seq(), (*it)[1], (*it)[2], (*it)[3], (*it)[4], (*it)[5]);
	}
	return true;
}

bool Glass::CheckSolutionPartial()
{
//	return true;
	if (G_Rsol_Items.size() == 0)
		return true;

	std::list< GlassRsol > ::reverse_iterator it, it_2;
	int  PlateId_current = G_Rsol_Items.back().plateId();
	for (it = G_Rsol_Items.rbegin(); it != G_Rsol_Items.rend() && (*it).plateId() == PlateId_current; it++)
	{

		//está fuera del plate 
		if ((*it).x() > plate_w || (*it).y() > plate_h || (*it).X() > plate_w || (*it).Y() > plate_h)
		{
			PrintProblem("Alguna pieza fuera");
//			exit(3);
			return false;
		}

		if ((*it).X() > (plate_w - waste_min) && (*it).X() != (plate_w))
		{
			return false;
			printf("Pieza x %d y %d X %d Y %d\t", (*it).x(), (*it).y(), (*it).X(), (*it).Y());
			
			PrintProblem("La X mayor que plate w menos 20");

		}
		if ((*it).Y() > (plate_h - waste_min) && (*it).Y() != (plate_h))
		{
			PrintProblem("La Y mayor que plate w menos 20");
//			exit(3);
			return false;
		}
		if (G_Rotate == false)
		{

			for (std::list< GlassDefect > ::iterator it2 = DefectsPlate[(*it).plateId()].begin(); it2 != DefectsPlate[(*it).plateId()].end(); it2++)
			{
				bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it2).Getpos_x(), (*it2).Getpos_y(), (*it2).Getpos_x() + (*it2).Getwidth(), (*it2).Getpos_y() + (*it2).Getheight());

				if (overlapping == true)
				{
					printf("Pieza x %d y %d X %d Y %d\t", (*it).x(), (*it).y(), (*it).X(), (*it).Y());
					printf("Defecto x %d y %d X %d Y %d Rotate %d\t", (*it2).Getpos_x(), (*it2).Getpos_y(), (*it2).Getpos_x() + (*it2).Getwidth(), (*it2).Getpos_y() + (*it2).Getheight(), G_Rotate);
//					exit(3);
					return false;
				}
			}
		}
		else
		{
			for (std::list< GlassDefect > ::iterator it2 = DefectsPlate[NumberOfPlates - (*it).plateId()].begin(); it2 != DefectsPlate[NumberOfPlates - (*it).plateId()].end(); it2++)
			{
				bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(),
					plate_w - ((*it2).Getpos_x() + (*it2).Getwidth()), plate_h - ((*it2).Getpos_y() + (*it2).Getheight()), plate_w - (*it2).Getpos_x(), plate_h - (*it2).Getpos_y());
				if (overlapping == true)
				{
					PrintProblem("Overlapping");
//					exit(3);
					return false;
				}
			}
		}
		it_2 = it;
		it_2++;
		if (it_2 == G_Rsol_Items.rend())
			continue;
		//Items from different plates
		if ((*it).plateId() != (*it_2).plateId())
			continue;
		for (; it_2 != G_Rsol_Items.rend() && (*it_2).plateId()==PlateId_current; it_2++)
		{

			bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it_2).x(), (*it_2).y(), (*it_2).X(), (*it_2).Y());
			if (overlapping == true)
			{
				PrintProblem("Overlapping");
//				exit(3);
				return false;
			}
		}

		//		fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it)[0], items[(*it)[0]].Getitem_stack(), items[(*it)[0]].Getitem_seq(), (*it)[1], (*it)[2], (*it)[3], (*it)[4], (*it)[5]);
	}
	return true;
}

bool Glass::CheckSolutionPartial(int Min)
{
	//	return true;
	if (G_Rsol_Items.size() == 0)
		return true;

	std::list< GlassRsol > ::reverse_iterator it, it_2;
	int  PlateId_current = G_Rsol_Items.back().plateId();
	for (it = G_Rsol_Items.rbegin(); it != G_Rsol_Items.rend() && (*it).plateId() == PlateId_current && (*it).x()>=Min; it++)
	{

		//está fuera del plate 
		if ((*it).x() > plate_w || (*it).y() > plate_h || (*it).X() > plate_w || (*it).Y() > plate_h)
		{
			PrintProblem("Alguna pieza fuera");
			//			exit(3);
			return false;
		}

		if ((*it).X() > (plate_w - waste_min) && (*it).X() != (plate_w))
		{
			return false;
			printf("Pieza x %d y %d X %d Y %d\t", (*it).x(), (*it).y(), (*it).X(), (*it).Y());

			PrintProblem("La X mayor que plate w menos 20");

		}
		if ((*it).Y() > (plate_h - waste_min) && (*it).Y() != (plate_h))
		{
			PrintProblem("La Y mayor que plate w menos 20");
			//			exit(3);
			return false;
		}
		if (G_Rotate == false)
		{

			for (std::list< GlassDefect > ::iterator it2 = DefectsPlate[(*it).plateId()].begin(); it2 != DefectsPlate[(*it).plateId()].end(); it2++)
			{
				bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it2).Getpos_x(), (*it2).Getpos_y(), (*it2).Getpos_x() + (*it2).Getwidth(), (*it2).Getpos_y() + (*it2).Getheight());

				if (overlapping == true)
				{
					printf("Pieza x %d y %d X %d Y %d\t", (*it).x(), (*it).y(), (*it).X(), (*it).Y());
					printf("Defecto x %d y %d X %d Y %d Rotate %d\t", (*it2).Getpos_x(), (*it2).Getpos_y(), (*it2).Getpos_x() + (*it2).Getwidth(), (*it2).Getpos_y() + (*it2).Getheight(), G_Rotate);
					//					exit(3);
					return false;
				}
			}
		}
		else
		{
			for (std::list< GlassDefect > ::iterator it2 = DefectsPlate[NumberOfPlates - (*it).plateId()].begin(); it2 != DefectsPlate[NumberOfPlates - (*it).plateId()].end(); it2++)
			{
				bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(),
					plate_w - ((*it2).Getpos_x() + (*it2).Getwidth()), plate_h - ((*it2).Getpos_y() + (*it2).Getheight()), plate_w - (*it2).Getpos_x(), plate_h - (*it2).Getpos_y());
				if (overlapping == true)
				{
					PrintProblem("Overlapping");
					//					exit(3);
					return false;
				}
			}
		}
		it_2 = it;
		it_2++;
		if (it_2 == G_Rsol_Items.rend())
			continue;
		//Items from different plates
		if ((*it).plateId() != (*it_2).plateId())
			continue;
		for (; it_2 != G_Rsol_Items.rend() && (*it_2).plateId() == PlateId_current && (*it_2).x()>=Min; it_2++)
		{

			bool overlapping = Overlap((*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it_2).x(), (*it_2).y(), (*it_2).X(), (*it_2).Y());
			if (overlapping == true)
			{
				PrintProblem("Overlapping");
				//				exit(3);
				return false;
			}
		}

		//		fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it)[0], items[(*it)[0]].Getitem_stack(), items[(*it)[0]].Getitem_seq(), (*it)[1], (*it)[2], (*it)[3], (*it)[4], (*it)[5]);
	}
	return true;
}

bool Glass::AlgoritmoDeterminista()
{
	Total_area_used = 0;//Suma of used area partial

	cont_plates = 0;
	bool piezas_por_colocar = false;
	//Re run all the plates 
	MinDimensionPieza = MinDimensionPieza_Inicial;
	int n_rectangle = 0;
	do
	{


		GlassRectangle *rectangle = new GlassRectangle(static_cast<int>(n_rectangle), static_cast<int>(plate_w), static_cast<int>(plate_h), DefectsPlate[cont_plates], this);
		GlassPlate Plato = GlassPlate(*rectangle, cont_plates, &node_nbr);
		Total_width_bin = 0;
		G_Plate_Sol.push_back(Plato);
		GlassRsol Rsol = GlassRsol(cont_plates, node_id++, 0, 0, plate_w, plate_h, -2, 0);
		G_Rsol_Items.push_back(Rsol);
		cont_plates++;

		piezas_por_colocar = AsignarItemsPlate_J(*rectangle);
		CheckSolution();
		delete rectangle;
	} while (MinDimensionPieza != MAXIMUM_INT_ROADEF);
	Objective_function = cont_plates * plate_h*plate_w + Total_width_bin * plate_h;
	Objective_function = ((cont_plates - 1) * plate_h * plate_w) + ((plate_w - Total_width_bin) * plate_h) - Total_area_used;
	G_Best_Rsol_Items = G_Rsol_Items;

	//	G_Best_Positions_Items = G_Positions_Items;
	
	Best_Objective_function = Objective_function;
	Best_plates_nbr = cont_plates;
	Best_Width = Total_width_bin;
	return true;
}
/*
bool AlgoritmoRandom()
{
Total_area_used = 0;//Suma of used area partial

cont_plates = 0;
bool piezas_por_colocar = false;
//Re run all the plates
for (int G_iter = 0; G_iter < 10000; G_iter++)
{
srand(G_iter);
RestartMatrix();
G_Positions_Items.clear();
cont_plates = 0;
Total_width_bin = 0;
MinDimensionPieza = MinDimensionPieza_Inicial;
Total_area_used = 0;
int n_rectangle = 0;
int inicio = 0;
do
{

GlassRectangle *rectangle = new GlassRectangle(n_rectangle, plate_w, plate_h, DefectsPlate[cont_plates],inicio);
GlassPlate Plato = GlassPlate(*rectangle, cont_plates, &node_nbr);
Total_width_bin = 0;
G_Plate_Sol.push_back(Plato);
cont_plates++;
GlassRsol Rsol = GlassRsol(cont_plates, node_id, 0, 0, plate_w,plate_h, -2, 0);
G_Rsol_Items.push_back(Rsol);

if (G_iter == 576 && cont_plates==5  )
int kk = 9;
deterministic = false;

piezas_por_colocar = AsignarItemsPlate_J(*rectangle);

CheckSolution();
} while (MinDimensionPieza != MAXIMUM_INT_ROADEF);
Objective_function =( (cont_plates-1) * plate_h*plate_w )+ (Total_width_bin * plate_h)-Total_area;
printf("G_iter %d Value %d Plates %d Width %d Best %d G_iter %d\n", G_iter, Objective_function, cont_plates, Total_width_bin,Best_Objective_function,Best_iter);
if (Objective_function < Best_Objective_function)
{
Best_iter = G_iter;
Best_plates_nbr = cont_plates;
Best_Objective_function = Objective_function;
G_Best_Positions_Items = G_Positions_Items;

}
//For writing all the solutions
if (draw_solution == true && Best_iter==G_iter )
{
G_Best_Positions_Items = G_Positions_Items;
Best_Objective_function = Objective_function;
WriteSolution();
DibujarOpenGL();
}

}
if (draw_solution == true)
{
WriteSolution();
DibujarOpenGL();
}
return true;
}*/
bool Glass::AlgoritmoRandom()
{
	Total_area_used = 0;//Suma of used area partial

	cont_plates = 0;

	//Re run all the plates 
	for (G_iter = 0; G_iter < 10; G_iter++)
	{
		//		srand(G_iter);
		RestartMatrix();
		//		G_Positions_Items.clear();
		G_Rsol_Items.clear();
		cont_plates = 1;
		Total_width_bin = 0;
		MinDimensionPieza = MinDimensionPieza_Inicial;
		Total_area_used = 0;
		//		int platec = 0;
		int n_rectangle1 = 0;
		int inicio = 0;
		do
		{

			GlassRectangle *rectangle = new GlassRectangle(n_rectangle1, plate_w, plate_h, DefectsPlate[cont_plates - 1], this, inicio);

			deterministic = false;

			inicio = AsignarItemsPlate_J(*rectangle);
			if (inicio == 0)
			{
				if (MinDimensionPieza != MAXIMUM_INT_ROADEF)
					Total_width_bin = 0;
				cont_plates++;
			}
			if (inicio == 0)
				CheckSolution();
			delete rectangle;
		} while (MinDimensionPieza != MAXIMUM_INT_ROADEF);
		Objective_function = ((cont_plates - 1) * plate_h*plate_w) + (Total_width_bin * plate_h) - Total_area;
		printf("G_iter %d Value %d Plates %d Width %d Best %d G_iter %d\n", G_iter, Objective_function, cont_plates, Total_width_bin, Best_Objective_function, Best_iter);
		if (Objective_function < Best_Objective_function)
		{
			Best_iter = G_iter;
			Best_plates_nbr = cont_plates;
			Best_Width = Total_width_bin;
			Best_Objective_function = Objective_function;
			G_Best_Rsol_Items = G_Rsol_Items;

			//			G_Best_Positions_Items = G_Positions_Items;

		}
		//For writing all the solutions
		if (draw_solution == true && Best_iter == G_iter)
		{
			G_Best_Rsol_Items = G_Rsol_Items;
			//		G_Best_Positions_Items = G_Positions_Items;
			Best_Objective_function = Objective_function;
			WriteSolution();
			DibujarOpenGL();
		}

	}
	if (draw_solution == true)
	{
		WriteSolution();
		DibujarOpenGL();
	}
	return true;
}
bool Glass::Dominated(GlassNodeB &node2)
{

	for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children.begin(); it != ListaNodosBeam_G_children.end(); it++)
	{
		//equal
		if ((*it).Total_area_used() == node2.Total_area_used())
		{
			if ((*it).Sol_Items().size() == node2.Sol_Items().size())
			{
				if ((*it).MinDimensionPieza() == node2.MinDimensionPieza())
				{
					if (((*it).Total_width_bin() + (*it).Plates()*plate_w) <= (node2.Total_width_bin() + node2.Plates()*plate_w))
					{

						bool equal = true;
						for (register int i = 0; i < stack_nbr && equal == true; i++)
						{
							if ((*it).Vector_Stacks()[i] != node2.Vector_Stacks()[i])
								equal = false;

						}
						if (equal == true)
						{
							return true;
						}
					}
				}
			}
		}//
		else
		{
			if ((*it).Total_area_used() > node2.Total_area_used())
			{
				if ((*it).Sol_Items().size() > node2.Sol_Items().size())
				{
					if ((*it).MinDimensionPieza() >= node2.MinDimensionPieza())
					{
						if (((*it).Total_width_bin() + (*it).Plates()*plate_w) <= (node2.Total_width_bin() + node2.Plates()*plate_w))
						{
							bool equal = true;
							for (register int i = 0; i < stack_nbr && equal == true; i++)
							{
								if ((*it).Vector_Stacks()[i] < node2.Vector_Stacks()[i])
									equal = false;

							}
							if (equal == true)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;


}
bool Glass::NotEqualCurrent(GlassNodeB &node2)
{
	if (node2.Partial_objective_function() != Partial_objective_function) return true;
	if (node2.Inicio() != Inicio) return true;
	if (node2.Plates() != Plates) return true;
	if (node2.Total_area() != Total_area) return true;
	if (node2.Total_area_used() != Total_area_used) return true;
	if (node2.Total_width_bin() != Total_width_bin) return true;
	if (node2.MinDimensionPieza() != MinDimensionPieza) return true;
	if (node2.Sol_Items().size() != G_Rsol_Items.size()) return true;
	return false;

}
void Glass::createNodeB(GlassNodeB &node3)
{
	node3.Utilization(Utilization);
	node3.Plates(Plates);
	node3.Inicio(Inicio);
	node3.Total_area_used(Total_area_used);
	node3.Total_area(Total_area);
	node3.MinDimensionPieza(MinDimensionPieza);
	node3.MaxMinDimensionPieza(MaxMinDimensionPieza);
	node3.Max_First_Bin(Max_First_Bin);
	node3.Total_width_bin(Total_width_bin);
	node3.Partial_objective_function(Partial_objective_function);
	node3.Matrix_Items(G_Matrix_Items);
	node3.Sol_Items(G_Rsol_Items);
	node3.Vector_Stacks(G_Vector_Stacks);
	node3.Vector_Items(G_Vector_Items);
	node3.Objective_function(Objective_function);
}
bool Glass::InsertPartialSolutionClassical()
{
	//We have a finished solution
	//or 

	int falta = 0;
	if (Total_area != Total_area_used)
	{
		if (MaxMinDimensionPieza < MinDimensionPieza)
			printf("Problem here");
		//		if (G_Rotate == false)
		//		{
		if ((Total_area - Total_area_used) < MaxMinDimensionPieza*plate_h)
			falta = MaxMinDimensionPieza * plate_h - (Total_area - Total_area_used);
		Partial_objective_function += falta;
		//		}
		/*		else
		if ((Total_area - Total_area_used) < MaxMinDimensionPieza*plate_h)
		falta = MaxMinDimensionPieza * plate_h - (Total_area - Total_area_used);
		Partial_objective_function += falta;*/
	}
	//we have a better solution already computed
	if (Partial_objective_function >= Best_Objective_function || Total_area_used == Total_area)
		return false;
	if (ListaNodosBeam_G_children_Classical.size() == 0)
	{
		if (CheckSolutionPartial(Inicio_Tira) == false)
			return false;
		//the last one
		GlassNodeB node;
		createNodeB(node);
		if (G_finish_instant)
		{
			FinishSolution();
			node.Objective_function(Objective_function);
		}
		if (BetterSolution(1) != false)
		{
			ListaNodosBeam_G_children_Classical.push_back(node);
		}
		else
		{
			if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);
		}
		

		ListaNodosBeam_G_children_Classical.push_back(node);
		Partial_objective_function_List = node.Partial_objective_function();
		return true;
	}
	if (Total_area_used != Total_area && Partial_objective_function<Best_Objective_function)
	{

		if ((ListaNodosBeam_G_children_Classical.size()<Max_List_Nodos_Beam_Per_Nodo || Utilization>(ListaNodosBeam_G_children_Classical.back().Utilization() - DBL_EPSILON)))
		{
			int cont = 0;
			for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children_Classical.begin(); it != ListaNodosBeam_G_children_Classical.end() && cont < Max_List_Nodos_Beam_Per_Nodo; it++, cont++)
			{
				if ((*it).Utilization() >(Utilization + DBL_EPSILON))
					continue;
				else
				{
					//has the same value of utilization
					if ((*it).Utilization() > (Utilization - DBL_EPSILON))
					{
						if (NotEqualCurrent((*it)) == true)
						{
							if (CheckSolutionPartial(Inicio_Tira) == false)
								return false;
							GlassNodeB node;
							createNodeB(node);
							if (Dominated(node) == true)
								return false;
							if (G_finish_instant)
							{
								FinishSolution();
								node.Objective_function(Objective_function);
							}
							if (BetterSolution(1)!=false)
							{
								ListaNodosBeam_G_children_Classical.insert(it, (node));
							}
							else
							{
								if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);
							}
							//If f
							
							if (ListaNodosBeam_G_children_Classical.size() >= Max_List_Nodos_Beam_Per_Nodo) ListaNodosBeam_G_children_Classical.pop_back();

							return true;
						}
						else
							return false;
					}
					else
					{
						if (CheckSolutionPartial(Inicio_Tira) == false)
							return false;
						GlassNodeB node;
						createNodeB(node);
						if (Dominated(node) == true)
							return false;
						if (G_finish_instant)
						{
							FinishSolution();
							node.Objective_function(Objective_function);
						}
						if (BetterSolution(1) != false)
						{
							ListaNodosBeam_G_children_Classical.insert(it, (node));
						}
						else
						{
							if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);
						}
						if (ListaNodosBeam_G_children_Classical.size() >= Max_List_Nodos_Beam_Per_Nodo) ListaNodosBeam_G_children_Classical.pop_back();
						return true;
					}

				}

			}
			if (CheckSolutionPartial(Inicio_Tira) == false)
				return false;
			//the last one
			GlassNodeB node;
			createNodeB(node);
			if (Dominated(node) == true)
				return false;
			if (G_finish_instant)
			{
				FinishSolution();
				node.Objective_function(Objective_function);
			}
			if (BetterSolution(1) != false)
			{
				ListaNodosBeam_G_children_Classical.push_back(node);
			}
			else
			{
				if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);
			}
			
			Partial_objective_function_List = node.Partial_objective_function();
			return true;


		}
		//The last one
		if (ListaNodosBeam_G_children_Classical.size() < Max_List_Nodos_Beam_Per_Nodo)
		{
			if (CheckSolutionPartial(Inicio_Tira) == false)
				return false;
			GlassNodeB node;
			createNodeB(node);
			if (Dominated(node) == true)
				return false;
			if (G_finish_instant)
			{
				FinishSolution();
				node.Objective_function(Objective_function);
			}
			if (BetterSolution(1) != false)
			{
				ListaNodosBeam_G_children_Classical.push_back(node);
			}
			else
			{
				if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);
			}
			
			Partial_objective_function_List = node.Partial_objective_function();
			return true;
		}

	}

	return false;

}

//Insert the partial solution in the list
bool Glass::InsertPartialSolution()
{
	//We have a finished solution
	//or 
	//	printf("Check IP1 \n");
	int falta = 0;
	if (Total_area != Total_area_used)
	{
		if (MaxMinDimensionPieza < MinDimensionPieza)
			printf("Problem here");
		if ((Total_area - Total_area_used)<MaxMinDimensionPieza*plate_h)
			falta = MaxMinDimensionPieza * plate_h - (Total_area - Total_area_used);
		Partial_objective_function += falta;
	}
	//we have a better solution already computed
	if (Partial_objective_function >= Best_Objective_function || Total_area_used == Total_area)
		return false;
	if (ListaNodosBeam_G_children.size() == 0)
	{
		//		printf("Check IP2 \n");
		if (CheckSolutionPartial(Inicio_Tira) == false)
			return false;
		//the last one
		GlassNodeB node;
		createNodeB(node);
		if (G_finish_instant)
		{
			FinishSolution();
			node.Objective_function(Objective_function);
		}
		node.Objective_function(Objective_function);
		if (BetterSolution(1) != false)
		{
			ListaNodosBeam_G_children.push_back(node);
		}
		else
		{
			if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);
		}

		


		Partial_objective_function_List = node.Partial_objective_function();
		return true;
	}
	if (Total_area_used != Total_area && Partial_objective_function<Best_Objective_function)
	{

		if ((ListaNodosBeam_G_children.size()<Max_List_Nodos_Beam || Utilization>(ListaNodosBeam_G_children.back().Utilization() - DBL_EPSILON)))
		{
			int cont = 0;
			for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children.begin(); it != ListaNodosBeam_G_children.end() && cont < Max_List_Nodos_Beam; it++, cont++)
			{
				//				printf("Check IP3 \n");
				if ((*it).Utilization() >(Utilization + DBL_EPSILON))
					continue;
				else
				{
					//has the same value of utilization
					if ((*it).Utilization() > (Utilization - DBL_EPSILON))
					{
						if (NotEqualCurrent((*it)) == true)
						{
							if (CheckSolutionPartial(Inicio_Tira) == false)
								return false;
							GlassNodeB node;
							createNodeB(node);
							if (Dominated(node) == true)
								return false;
							if (G_finish_instant)
							{
								FinishSolution();
								node.Objective_function(Objective_function);
							}
							if (BetterSolution(1) != false)
							{
								ListaNodosBeam_G_children.insert(it, (node));
							}
							else
							{
								if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);
							}
							//If f
							
							if (ListaNodosBeam_G_children.size() >= Max_List_Nodos_Beam) ListaNodosBeam_G_children.pop_back();

							return true;
						}
						else
							return false;
					}
					else
					{
						if (CheckSolutionPartial(Inicio_Tira) == false)
							return false;
						GlassNodeB node;
						createNodeB(node);
						if (Dominated(node) == true)
							return false;
						if (G_finish_instant)
						{
							FinishSolution();
							node.Objective_function(Objective_function);
						}
						if (BetterSolution(1) != false)
						{
							ListaNodosBeam_G_children.insert(it, (node));
						}
						else
						{
							if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);
						}
					
						if (ListaNodosBeam_G_children.size() >= Max_List_Nodos_Beam) ListaNodosBeam_G_children.pop_back();
						return true;
					}

				}

			}
			if (CheckSolutionPartial(Inicio_Tira) == false)
				return false;
			//the last one
			GlassNodeB node;
			createNodeB(node);
			if (Dominated(node) == true)
				return false;
			if (G_finish_instant)
			{
				FinishSolution();
				node.Objective_function(Objective_function);
			}
			if (BetterSolution(1) != false)
			{
				ListaNodosBeam_G_children.push_back(node);
			}
			else
			{
				if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);
			}
			
			Partial_objective_function_List = node.Partial_objective_function();
			return true;


		}
		//The last one
		if (ListaNodosBeam_G_children.size() < Max_List_Nodos_Beam)
		{
			if (CheckSolutionPartial(Inicio_Tira) == false)
				return false;
			GlassNodeB node;
			createNodeB(node);
			if (Dominated(node) == true)
				return false;
			if (G_finish_instant)
			{
				FinishSolution();
				node.Objective_function(Objective_function);
			}
			if (BetterSolution(1) != false)
			{
				ListaNodosBeam_G_children.push_back(node);
			}
			else
			{
				if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);
			}
			
			Partial_objective_function_List = node.Partial_objective_function();
			return true;
		}

	}

	return false;

}
void Glass::InsertClassical()
{
	for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children_Classical.begin(); it != ListaNodosBeam_G_children_Classical.end(); it++)
	{
		//Pass everything to the current solution
		Inicio = (*it).Inicio();
		Plates = (*it).Plates();
		Total_area = (*it).Total_area(); // Sum of total area of items.
		Total_area_used = (*it).Total_area_used();//Suma of used area partial
		Total_width_bin = (*it).Total_width_bin();
		Objective_function = (*it).Objective_function();
		Max_First_Bin = (*it).Max_First_Bin();
		Partial_objective_function = (*it).Partial_objective_function();
		MinDimensionPieza = (*it).MinDimensionPieza();
		MaxMinDimensionPieza = (*it).MaxMinDimensionPieza();
		G_Matrix_Items = (*it).Matrix_Items();
		G_Vector_Items = (*it).Vector_Items();
		G_Vector_Stacks = (*it).Vector_Stacks();
		G_Rsol_Items = (*it).Sol_Items();
		InsertPartialSolution();
	}
	ListaNodosBeam_G_children_Classical.clear();
}
//Insert the partial solution in the list
bool Glass::InsertPartialSolutionGlobal(GlassNodeB node)
{

	if (ListaNodosBeam.size() == 0)
	{

		ListaNodosBeam.push_back(node);
		return true;
	}
	if (node.Objective_function() <= ListaNodosBeam.back().Objective_function() || ListaNodosBeam.size()<Max_List_Nodos_Beam_Global_Global)
	{

		int cont = 0;
		for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam.begin(); it != ListaNodosBeam.end() && cont < Max_List_Nodos_Beam_Global_Global; it++, cont++)
		{
			if ((*it).Objective_function() <(node.Objective_function()))
				continue;
			if ((*it).Objective_function() > (node.Objective_function()))
			{
				ListaNodosBeam.insert(it, (node));
				if (ListaNodosBeam.size() >= Max_List_Nodos_Beam_Global_Global) ListaNodosBeam.pop_back();
				return true;
			}
			else
			{
				if ((*it).Utilization() > (node.Utilization() + DBL_EPSILON))
					continue;
				else
				{
					//has the same value of utilization
					if ((*it).Utilization() > (node.Utilization() - DBL_EPSILON))
					{
						ListaNodosBeam.insert(it, (node));
						if (ListaNodosBeam.size() >= Max_List_Nodos_Beam_Global_Global) ListaNodosBeam.pop_back();

						return true;
					}
					else
					{
						ListaNodosBeam.insert(it, (node));
						if (ListaNodosBeam.size() >= Max_List_Nodos_Beam_Global_Global) ListaNodosBeam.pop_back();
						return true;
					}

				}
			}

		}
		if (ListaNodosBeam.size() < Max_List_Nodos_Beam_Global_Global)
		{
			ListaNodosBeam.push_back(node);
			return true;
		}


	}


	return false;

}
//Insert the partial solution in the list
bool Glass::InsertPartialSolutionGlobalUtil2(GlassNodeB node)
{

	if (ListaNodosBeam_Utilization2.size() == 0)
	{

		ListaNodosBeam_Utilization2.push_back(node);
		return true;
	}
	if (node.Utilization2() >= ListaNodosBeam_Utilization2.back().Utilization2() || ListaNodosBeam_Utilization2.size()<Max_List_Nodos_Beam_Global_Double)
	{

		int cont = 0;
		for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_Utilization2.begin(); it != ListaNodosBeam_Utilization2.end() && cont < Max_List_Nodos_Beam_Global_Double; it++, cont++)
		{
			if ((*it).Utilization2() >(node.Utilization2() + DBL_EPSILON))
				continue;
			if ((*it).Utilization2() < (node.Utilization2() - DBL_EPSILON))
			{
				ListaNodosBeam_Utilization2.insert(it, (node));
				if (ListaNodosBeam_Utilization2.size() >= Max_List_Nodos_Beam_Global_Double) ListaNodosBeam_Utilization2.pop_back();
				return true;
			}
			else
			{
				if ((*it).Utilization() > (node.Utilization() + DBL_EPSILON))
					continue;
				else
				{
					//has the same value of utilization
					if ((*it).Utilization() > (node.Utilization() - DBL_EPSILON))
					{
						ListaNodosBeam_Utilization2.insert(it, (node));
						if (ListaNodosBeam_Utilization2.size() >= Max_List_Nodos_Beam_Global_Double) ListaNodosBeam_Utilization2.pop_back();

						return true;
					}
					else
					{
						ListaNodosBeam_Utilization2.insert(it, (node));
						if (ListaNodosBeam_Utilization2.size() >= Max_List_Nodos_Beam_Global_Double) ListaNodosBeam_Utilization2.pop_back();
						return true;
					}

				}
			}

		}
		if (ListaNodosBeam_Utilization2.size() < Max_List_Nodos_Beam_Global_Double)
		{
			ListaNodosBeam_Utilization2.push_back(node);
			return true;
		}


	}


	return false;

}

bool Glass::SecondFilter()
{
	bool remove = false;
	bool some_change = false;
	std::list< GlassNodeB > ::iterator it, it2;
	//	bool equal = true;
	remove = false;
	if (ListaNodosBeam_G_children.size() == 0)
		return some_change;
	it = ListaNodosBeam_G_children.begin();
	int  Val = 0;
	do
	{
		//Finish solution
		//		FinishSolution();
		// insert solutuion

		if (Val)
			it = ListaNodosBeam_G_children.erase(it2);
		else
			it++;

	} while (it != ListaNodosBeam_G_children.end());

	return some_change;
}

void Glass::SelectByGlobalObjectiveFunction()
{
	double tmp_first = 0;
	struct timeb t1;
	ListaNodosBeam.clear();
	ListaNodosBeam_Utilization2.clear();
	int cont = 0;
	if (ListaNodosBeam_G_children.size() > (Max_List_Nodos_Beam_Global_Utilization+ Max_List_Nodos_Beam_Global_Global+ Max_List_Nodos_Beam_Global_Double))
	{

		//Second the part of the Objective Function
		cont = 0;
		if (Max_List_Nodos_Beam_Global_Global > 0)
		{
			for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children.begin(); it != ListaNodosBeam_G_children.end(); it++, cont++)
			{
				if (cont > Max_List_Nodos_Beam_Global_Utilization)
					//Insert in list
				{
					InsertPartialSolutionGlobal(*it);

				}
			}
		}
		cont = 0;
		if (Max_List_Nodos_Beam_Global_Double > 0)
		{
			int typeOfEvaluation = 1;
			if (get_random(0, 2) == 0)
				typeOfEvaluation = 100;
			for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children.begin(); it != ListaNodosBeam_G_children.end() && tmp_first < G_Time; it++, cont++)
			{
				if (cont > Max_List_Nodos_Beam_Global_Utilization)
					//Insert in list
				{
					if (NotInTheList(*it) == true)
					{
						LevelEvaluation(typeOfEvaluation, (*it));
						InsertPartialSolutionGlobalUtil2(*it);
					}

				}
				ftime(&t1);
				tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;

			}
		}
		//These are the first in Utilization
		cont = 0;
		for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children.begin(); it != ListaNodosBeam_G_children.end(); it++, cont++)
		{
			if (cont <= Max_List_Nodos_Beam_Global_Utilization)
				//Insert in list
				ListaNodosBeam.push_back((*it));
		}
		ListaNodosBeam.insert(ListaNodosBeam.end(), ListaNodosBeam_Utilization2.begin(), ListaNodosBeam_Utilization2.end());

	}
	else
		ListaNodosBeam = ListaNodosBeam_G_children;
}

bool Glass::NotInTheList(GlassNodeB &nodo)
{
	for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam.begin(); it != ListaNodosBeam.end(); it++)
	{
		if ((nodo.Total_area_used() == (*it).Total_area_used()) &&
			(nodo.Inicio() == (*it).Inicio()) &&
			(nodo.MinDimensionPieza() == (*it).MinDimensionPieza()))
			return false;
	}
	return true;
}

//removing partial solutions included in others

bool Glass::RemoveDominated()
{
	bool remove = false;
	bool some_change = false;
	std::list< GlassNodeB > ::iterator it, it2;
	bool equal = true;
	remove = false;
	double tmp_first = 0;
	struct timeb t1;
	if (ListaNodosBeam_G_children.size() == 0)
		return some_change;
	it = ListaNodosBeam_G_children.begin();
	do
	{
		it2 = it;
		it2++;
		if (it2 == ListaNodosBeam_G_children.end())
			return some_change;
		do
		{
			remove = false;
			if ((*it).Total_area_used() == (*it2).Total_area_used())
			{
				if ((*it).Sol_Items().size() == (*it2).Sol_Items().size())
				{
					if ((*it).MinDimensionPieza() == (*it2).MinDimensionPieza())
					{

						equal = true;
						for (register int i = 0; i < stack_nbr && equal == true; i++)
						{
							if ((*it).Vector_Stacks()[i] != (*it2).Vector_Stacks()[i])
								equal = false;

						}
						if (equal == true)
						{
							remove = true;
							some_change = true;
							//Remove it
						}
					}
				}
			}//
			else
			{
				if ((*it).Total_area_used() > (*it2).Total_area_used())
				{
					if ((*it).Sol_Items().size() > (*it2).Sol_Items().size())
					{
						if ((*it).MinDimensionPieza() >= (*it2).MinDimensionPieza())
						{
							if (((*it).Total_width_bin() + plate_w * (*it).Plates()) <= ((*it2).Total_width_bin() + plate_w * (*it2).Plates()))
							{
								equal = true;
								for (register int i = 0; i < stack_nbr && equal == true; i++)
								{
									if ((*it).Vector_Stacks()[i] < (*it2).Vector_Stacks()[i])
										equal = false;

								}
								if (equal == true)
								{
									remove = true;
									some_change = true;
									//Remove it
								}
							}
						}
					}
				}
			}
			if (remove != true && it2 != ListaNodosBeam_G_children.end())
				it2++;
			if (remove == true)
				it2 = ListaNodosBeam_G_children.erase(it2);
			if (ListaNodosBeam_G_children.size() == 0)
				return some_change;
		} while (it2 != ListaNodosBeam_G_children.end());
		it++;
		ftime(&t1);
		tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;



	} while (it != ListaNodosBeam_G_children.end() && tmp_first<G_Time);

	return some_change;
}
void Glass::PutObjectiveFunction(int i)
{
	if (ListaNodosBeam.size()>1)
		i = i % (ListaNodosBeam.size() - 1);


	int cont = 0;

	for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam.begin(); it != ListaNodosBeam.end(); it++, cont++)
	{

		if (cont >= i)
		{
			(*it).Objective_function(Objective_function);
			return;
		}
	}
}
//Recover a Partial Solution
void Glass::RecoverPartialSolution(int i)
{
	if (ListaNodosBeam.size()>1)
		i = i % (ListaNodosBeam.size() - 1);


	int cont = 0;

	for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam.begin(); it != ListaNodosBeam.end(); it++, cont++)
	{

		if (cont >= i)
		{

			//Pass everything to the current solution
			Inicio = (*it).Inicio();
			Plates = (*it).Plates();
			Total_area = (*it).Total_area(); // Sum of total area of items.
			Total_area_used = (*it).Total_area_used();//Suma of used area partial
			Total_width_bin = (*it).Total_width_bin();
			Max_First_Bin = (*it).Max_First_Bin();
			Objective_function = (*it).Objective_function();
			Partial_objective_function = (*it).Partial_objective_function();
			MinDimensionPieza = (*it).MinDimensionPieza();
			MaxMinDimensionPieza = (*it).MinDimensionPieza();
			G_Matrix_Items = (*it).Matrix_Items();
			G_Vector_Items = (*it).Vector_Items();
			G_Vector_Stacks = (*it).Vector_Stacks();
			G_Rsol_Items = (*it).Sol_Items();
			return;
		}
	}

}
bool Glass::BeamSearchParallel(int Iterations, int SizeOfBea, int Many, int Hil, int seed_id)
{
	SizeOfBeam = SizeOfBea;
	Hilo = Hil;
	//	srand(seed_id + Many + Iterations * 1000 + SizeOfBeam * 10000);

	Max_List_Nodos_Beam = 200;
	Total_area_used = 0;//Suma of used area partial


	if (Many == 0)
		Many = 100;

	cont_plates = 0;

	//Re run all the plates 
	G_level = 0;
	ListaNodosBeam.clear();
	ListaNodosBeam_G_children.clear();
	/*	int Max_iter = 1000;
	Max_List_Nodos_Beam = Size_lista * 4 * SizeOfBeam;
	Max_List_Nodos_Beam_Global = Size_lista * SizeOfBeam;
	//First Strip
	#pragma omp for schedule(static,1)
	for (G_iter = 0; G_iter < Max_iter && tmp_first < G_Time; G_iter++)
	{

	//				G_Positions_Items.clear();
	G_Rsol_Items.clear();
	Plates = 1;
	Total_width_bin = 0;
	Total_area_used = 0;
	int n_rectangle = 0;
	Inicio = 0;

	//				srand(G_iter*SizeOfBeam);
	int Inicio2 = -1;

	for (int many = 0; (many < Many && Inicio2 != 0) && (MinDimensionPieza != MAXIMUM_INT_ROADEF || Total_area != Total_area_used); many++)
	{

	//					printf("Iter %d", many);
	cont_plates = Plates - 1;
	GlassRectangle *rectangle = new GlassRectangle(n_rectangle, min(Inicio + max1Cut, plate_w), plate_h, DefectsPlate[Plates - 1], this, Inicio);
	deterministic = false;
	//inicio ==0 means finish a plate
	int InicioPrevio = Total_width_bin;
	Inicio = AsignarItemsPlate_J(*rectangle);
	delete rectangle;


	if ((Total_area > Total_area_used) &&
	(Total_width_bin == InicioPrevio ||
	((plate_w - Total_width_bin) != MinDimensionPieza && (plate_w - Total_width_bin) < (MinDimensionPieza + 20))))
	{
	Total_width_bin = plate_w;
	Inicio = 0;
	}
	//InsertPartial Solution
	Partial_objective_function = ((Plates - 1) * plate_h*plate_w) + (Total_width_bin * plate_h) - Total_area_used;
	Utilization = ((double)Total_area_used) / ((double)(((Plates - 1) * plate_h*plate_w) + (Total_width_bin * plate_h)));
	if (Inicio == 0 && (MinDimensionPieza != MAXIMUM_INT_ROADEF))
	{
	Total_width_bin = 0;
	Plates++;
	}
	if (MinDimensionPieza != MAXIMUM_INT_ROADEF || Total_area != Total_area_used)
	solutions_to_fill = true;
	Inicio2 = Inicio;
	if (Many != 100)
	Inicio2 = 1;

	}
	}
	entered = InsertPartialSolution();

	//				if (entered == true && G_level >= 100 )

	//a2 es 54 plate y 1989
	//a20 es 5040
	//a17 sobre 7240
	//a19 2 paltes +1950

	BetterSolution();
	//			printf("Here2 %d\n",Many);

	//			printf("Here3 Time %f\n",tmp_first);
	//				else
	//				solutions_to_fill = true;
	//				if (inicio == 0)
	ftime(&t1);
	tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;
	//			printf("Here3 Time %f\n", tmp_first);

	//nuevo

	//			printf("G_Iter %d %d", G_iter, Max_iter);

	int n_rectangle = 0;

	if (G_level == 0)
	{
	RestartMatrix();
	//				G_Positions_Items.clear();
	G_Rsol_Items.clear();
	Plates = 1;
	Total_width_bin = 0;
	MinDimensionPieza = MinDimensionPieza_Inicial;
	Total_area_used = 0;
	int n_rectangle = 0;
	Inicio = 0;
	}


	int Max_children = 1;
	if (G_level != 0)
	Max_children = 200 * Iterations;
	if (G_iter == 4)
	int kk = 9;

	bool entered = false;
	for (G_children = 0; G_children < Max_children && tmp_first<G_Time; G_children++)
	{
	//				printf("G_children %d %d", G_children, Max_children);
	Best_Objective_function = Global_Best_Objective_Function;
	if (G_level != 0)
	RecoverPartialSolution(G_iter);
	if (G_children == 0 && G_level >= 1000 && G_iter >= 0)
	{
	G_Best_Rsol_Items = G_Rsol_Items;
	WriteSolution();
	DibujarOpenGL();
	}
	//				srand(G_iter*SizeOfBeam);
	int Inicio2 = -1;

	for (int many = 0; (many<Many && Inicio2 != 0) && (MinDimensionPieza != MAXIMUM_INT_ROADEF || Total_area != Total_area_used); many++)
	{

	//					printf("Iter %d", many);
	cont_plates = Plates - 1;
	GlassRectangle *rectangle = new GlassRectangle(n_rectangle, min(Inicio + max1Cut, plate_w), plate_h, DefectsPlate[Plates - 1], this, Inicio);
	deterministic = false;
	//inicio ==0 means finish a plate
	int InicioPrevio = Total_width_bin;
	Inicio = AsignarItemsPlate_J(*rectangle);
	delete rectangle;


	if ((Total_area > Total_area_used) &&
	(Total_width_bin == InicioPrevio ||
	((plate_w - Total_width_bin) != MinDimensionPieza && (plate_w - Total_width_bin) < (MinDimensionPieza + 20))))
	{
	Total_width_bin = plate_w;
	Inicio = 0;
	}
	if (Total_width_bin == 2809 && G_Rsol_Items.size() == 6 && Total_area_used == 7071457)
	int kk = 8;
	//				if (Total_width_bin)
	CheckSolution();
	//InsertPartial Solution
	Partial_objective_function = ((Plates - 1) * plate_h*plate_w) + (Total_width_bin * plate_h) - Total_area_used;
	Utilization = ((double)Total_area_used) / ((double)(((Plates - 1) * plate_h*plate_w) + (Total_width_bin * plate_h)));
	if (Inicio == 0)
	{
	if (MinDimensionPieza != MAXIMUM_INT_ROADEF)
	{
	Total_width_bin = 0;
	Plates++;
	}
	}
	if (MinDimensionPieza != MAXIMUM_INT_ROADEF || Total_area != Total_area_used)
	solutions_to_fill = true;
	Inicio2 = Inicio;
	if (Many != 100)
	Inicio2 = 1;

	}
	entered = InsertPartialSolution();

	//				if (entered == true && G_level >= 100 )

	//a2 es 54 plate y 1989
	//a20 es 5040
	//a17 sobre 7240
	//a19 2 paltes +1950

	BetterSolution();
	//			printf("Here2 %d\n",Many);

	//			printf("Here3 Time %f\n",tmp_first);
	//				else
	//				solutions_to_fill = true;
	//				if (inicio == 0)
	ftime(&t1);
	tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;
	//			printf("Here3 Time %f\n", tmp_first);

	CheckSolution();
	}
	ftime(&t1);
	tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;

	//		printf("Here4 Time %f Total tiem %d\n", tmp_first,G_Time);


	}
	//		printf("Here42 Time %f Total tiem %d\n", tmp_first, G_Time);
	FinishSolutions();
	ftime(&t1);
	tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;
	//		printf("Here5 Time %f Total tiem %d\n", tmp_first, G_Time);

	//		printf("Level %d Tam Lista Children %d Iterations %d Size %d Many %d Thread %d\n \n", G_level, ListaNodosBeam_G_children.size(), Iterations, SizeOfBeam, Many,Hilo);
	RemoveDominated();
	if (active_log_error) printf("Level %d Tam Lista Children %d Iterations %d Size %d Many %d Thread %d Iter %d Children %d\n \n", G_level, ListaNodosBeam_G_children.size(), Iterations, SizeOfBeam, Many, Hilo, G_iter, G_children);

	SelectByGlobalObjectiveFunction();
	//		FinishSolutions();
	G_level++;



	} while (solutions_to_fill == true && (ListaNodosBeam_G_children.size()>0) && tmp_first<G_Time);
	//For writing all the solutions
	if (draw_solution == true && G_level >= 8000)
	{
	//		G_Best_Rsol_Items = G_Rsol_Items;
	//		G_Best_Positions_Items = G_Positions_Items;
	//		Best_Objective_function = Objective_function;
	WriteSolution();
	DibujarOpenGL();
	}


	*/
	return true;
}
bool Glass::BeamSearchRotate(int Hil, int seed_id)
{
	//	TransformarSolucionSimetrica("tempkk.txt");
	//	Iterations = Iterations / 2;
	//	SizeOfBea = SizeOfBea / 2;
	//	Many = Many / 2;
//	Global_Best_Objective_Function = 4769940		;
	SizeOfBeam = G_Size_width;
	Hilo = m_id;
	int Many = G_type_strip;

	Initial_Objective_function = Global_Best_Objective_Function;
	NumberOfPlates = (Global_Best_Objective_Function + Total_area) / (plate_w*plate_h);
	RestOfLastPlate = (plate_w - (((Global_Best_Objective_Function + Total_area) % (plate_w*plate_h)) / plate_h)) + 1;
	Best_Width_First_Rotate = plate_w - RestOfLastPlate;
	//	tipo_lista = 1;
	SizeOfBeam = G_Size_width;
	Hilo = m_id;
	//	srand(seed_id + m_id);
	if (active_log) printf("Beam Search Rotate S %d Iter %d M %d I %d SB %d Gof %d\n", seed_id, m_id, Many, G_Iterations, SizeOfBeam, Global_Best_Objective_Function);
	Max_List_Nodos_Beam = 200;
	Total_area_used = 0;//Suma of used area partial
	struct timeb t1;

	if (Many == 0)
		Many = 100;
	double tmp_first = 0;
	cont_plates = 0;

	//Re run all the plates 
	G_level = 0;
	ListaNodosBeam.clear();
	ListaNodosBeam_G_children.clear();
	bool solutions_to_fill = false;
	int Size_lista = 4;//V8

	Max_List_Nodos_Beam = 12 * (G_Size_alpha + G_Size_Chosen)* (G_iteraciones / 200) * SizeOfBeam;
	Max_List_Nodos_Beam_Per_Nodo = Max_List_Nodos_Beam;
	Max_List_Nodos_Beam_Global_Utilization = 4 * G_Size_Chosen*(G_iteraciones / 200)* SizeOfBeam;
	Max_List_Nodos_Beam_Global_Global = Max_List_Nodos_Beam_Global_Utilization;
	Max_List_Nodos_Beam_Global_Double = Max_List_Nodos_Beam_Global_Utilization;


	if (G_tipo_lista == 1)
	{
		Max_List_Nodos_Beam_Global_Utilization = 3 * Max_List_Nodos_Beam_Global_Utilization;
		Max_List_Nodos_Beam_Global_Global = 0;
		Max_List_Nodos_Beam_Global_Double = 0;
	}
	if (G_tipo_lista == 2)
	{
		Max_List_Nodos_Beam_Global_Double = 0;
		Max_List_Nodos_Beam_Global_Utilization = 0;
		Max_List_Nodos_Beam_Global_Global = 3 * Max_List_Nodos_Beam_Global_Global;
	}
	if (G_tipo_lista == 3)
	{
		Max_List_Nodos_Beam_Global_Utilization = 0;
		Max_List_Nodos_Beam_Global_Double = 3 * Max_List_Nodos_Beam_Global_Double;
		Max_List_Nodos_Beam_Global_Global = 0;
	}	
	do
	{
		int Max_iter = G_iteraciones;
		//		ListaNodosBeam = ListaNodosBeam_G_children;
		ListaNodosBeam_G_children.clear();


		if (G_level == 0)
			Max_iter = G_iteraciones * G_Iterations;
		else
			Max_iter = ListaNodosBeam.size() - 1;
		solutions_to_fill = false;
		for (G_iter = 0; G_iter < Max_iter && tmp_first<G_Time && Initial_Objective_function <= Global_Best_Objective_Function; G_iter++)
		{
			//nuevo

			//			printf("G_Iter %d %d", G_iter, Max_iter);

			int n_rectangle = 0;

			if (G_level == 0)
			{
				RestartMatrix();
				//				G_Positions_Items.clear();
				G_Rsol_Items.clear();
				//Starting from the last
				Plates = 1;
				Total_width_bin = 0;
				MinDimensionPieza = MinDimensionPieza_Inicial;
				MaxMinDimensionPieza = MaxMinDimensionPieza_Inicial;
				Total_area_used = 0;

				Inicio = 0;
			}


			int Max_children = 1;
			if (G_level != 0)
				Max_children = (G_iteraciones / 5) * G_Iterations;


			bool entered = false;
			for (G_children = 0; G_children < Max_children && tmp_first<G_Time; G_children++)
			{
				//				printf("G_children %d %d", G_children, Max_children);
				Best_Objective_function = Global_Best_Objective_Function;
				if (G_level != 0)
				{
					RecoverPartialSolution(G_iter);
				}
				/*		if (Total_width_bin == 10480)
				{
				G_Best_Rsol_Items = G_Rsol_Items;
				WriteSolution();
				DibujarOpenGL();
				}
				*/

				/*			if (active_log==true && G_children == 0 && G_level >= 1 && G_iter >= 0)
				{
				G_Best_Rsol_Items = G_Rsol_Items;
				WriteSolution();
				DibujarOpenGL();
				}*/
				//				srand(G_iter*SizeOfBeam);
				int Inicio2 = -1;
				int Inicio3 = Inicio;
				for (int many = 0; (many<Many && Inicio2 != 0) && (MinDimensionPieza != MAXIMUM_INT_ROADEF || Total_area != Total_area_used); many++)
				{

					//					printf("Iter %d", many);
					if (G_level == 0 && Plates == 1 && Inicio2 == -1)
					{
						Inicio = RestOfLastPlate;
						Total_width_bin = RestOfLastPlate;
						//En el trozo no tiene que cumplir lo de 20 o menor
					}
					NumberOfPlate = NumberOfPlates - (Plates - 1);
					cont_plates = Plates - 1;
					GlassRectangle *rectangle = new GlassRectangle(n_rectangle, min(Inicio + max1Cut, plate_w), plate_h, DefectsPlate[NumberOfPlate], this, Inicio);

					deterministic = false;
					//inicio ==0 means finish a plate



					Inicio = AsignarItemsPlate_J(*rectangle);
					delete rectangle;
					//					if (Total_area == Total_area_used && Total_width_bin > (plate_w - waste_min))
					//					if (Total_area == Total_area_used )






					//				if (Total_width_bin)
					CheckSolution();
					//InsertPartial Solution
					if (Inicio == 0 && (MinDimensionPieza != MAXIMUM_INT_ROADEF))
					{
						if (Plates == 1)
							Max_First_Bin = Total_width_bin;
						Total_width_bin = 0;
						Plates++;
						/*						//This solution is not good
						if (Plates > NumberOfPlates)
						{

						}
						*/
					}

					if (Plates == 1)
						TrozoBin = RestOfLastPlate;
					else
						TrozoBin = 0;

					//				int Width_First_Bin = MaxWidthLastPiece(G_Rsol_Items);
					//		Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Width_First_Bin - RestOfLastPlate) * plate_h) - Total_area_used;

					//TODO
					/*
					if (Plates==1)
					Partial_objective_function = ((Plates - 1) * plate_h*plate_w) + ((Total_width_bin- RestOfLastPlate) * plate_h) - Total_area_used;
					else
					Partial_objective_function = ((Plates - 2) * plate_h*plate_w) + ((Max_First_Bin - RestOfLastPlate) * plate_h) + ((Total_width_bin ) * plate_h) - Total_area_used;
					if (Plates == 1)
					Utilization = ((double)Total_area_used) / ((double)(((Plates - 1) * plate_h*plate_w) + ((Total_width_bin - RestOfLastPlate) * plate_h)));
					else
					Utilization = ((double)Total_area_used) / ((double)(((Plates - 2) * plate_h*plate_w) + ((Max_First_Bin - RestOfLastPlate) * plate_h))+ ((Total_width_bin)* plate_h));
					*/
					if (Plates == 1)
						Partial_objective_function = ((Plates - 1) * plate_h*plate_w) + ((Total_width_bin - RestOfLastPlate) * plate_h) - Total_area_used;
					else
						Partial_objective_function = ((Plates - 2) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h) + ((Total_width_bin)* plate_h) - Total_area_used;
					if (Plates == 1)
						Utilization = ((double)Total_area_used) / ((double)(((Plates - 1) * plate_h*plate_w) + ((Total_width_bin - RestOfLastPlate) * plate_h)));
					else
						Utilization = ((double)Total_area_used) / ((double)(((Plates - 2) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h)) + ((Total_width_bin)* plate_h));

					if (MinDimensionPieza != MAXIMUM_INT_ROADEF || Total_area != Total_area_used)
						solutions_to_fill = true;

					Inicio2 = Inicio;
					if (Many != 100)
						Inicio2 = 1;

					//					if (G_children == 0 && G_level >= 3 && G_iter >= 0)


				}
	//			if (CheckSolutionPartial(Inicio3) == false)
		//			continue;
				if ((Plates - 1) <= NumberOfPlates)
				{
					if (G_Classic == false || G_level == 0)
						entered = InsertPartialSolution();
					else
						entered = InsertPartialSolutionClassical();
				}
				//				if (entered == true && G_level >= 100 )

				/*			if (G_iter == 4 && G_level == 2)
				{
				G_Best_Rsol_Items = G_Rsol_Items;
				WriteSolution();
				DibujarOpenGL();
				}*/
				//a2 es 54 plate y 1989
				//a20 es 5040
				//a17 sobre 7240
				//a19 2 paltes +1950

				if (G_finish_instant == false && BetterSolution(4) == false)
				{
					if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);

					G_children = Max_children;

					//					RemoveNodo();
				}
				//			printf("Here2 %d\n",Many);

				//			printf("Here3 Time %f\n",tmp_first);
				//				else
				//				solutions_to_fill = true;
				//				if (inicio == 0)
				ftime(&t1);
				tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;
				//			printf("Here3 Time %f\n", tmp_first);

				//				CheckSolution();
			}
			if (G_Classic == true && G_level != 0)
				InsertClassical();
			ftime(&t1);
			tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;

			//		printf("Here4 Time %f Total tiem %d\n", tmp_first,G_Time);


		}
		//		printf("Here42 Time %f Total tiem %d\n", tmp_first, G_Time);
		ftime(&t1);
		tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;

		if (tmp_first > G_Time)
			return true;
		RemoveDominated();
		FinishSolutions();
		//		DoubleEvaluation();
		ftime(&t1);
		tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;
		//		printf("Here5 Time %f Total tiem %d\n", tmp_first, G_Time);
		if (tmp_first > G_Time)
			return true;
		//		printf("Level %d Tam Lista Children %d Iterations %d Size %d Many %d Thread %d\n \n", G_level, ListaNodosBeam_G_children.size(), Iterations, SizeOfBeam, Many,Hilo);
		int size_possibles = ListaNodosBeam_G_children.size();

		SelectByGlobalObjectiveFunction();
		if (active_log_error) printf("Level %d Tam Lista Possibles %d Chosen %d Iterations %d Size %d Many %d Thread %d Iter %d Children %d\n \n", G_level, size_possibles, ListaNodosBeam.size(), G_Iterations, SizeOfBeam, Many, Hilo, G_iter, G_children);
		//		DibujarChosen();
		//		FinishSolutions();
		G_level++;

		//For writing all the solutions
		/*		if (draw_solution == true && Best_iter == G_iter)
		{
		G_Best_Rsol_Items = G_Rsol_Items;
		G_Best_Positions_Items = G_Positions_Items;
		Best_Objective_function = Objective_function;
		WriteSolution();
		DibujarOpenGL();
		}
		*/
	} while (solutions_to_fill == true && (ListaNodosBeam_G_children.size()>0) && tmp_first<G_Time && Initial_Objective_function <= Global_Best_Objective_Function);
	//For writing all the solutions

	/*	if (G_Best_Rsol_Items.size() > 0 && (Global_Best_Objective_Function == Best_Objective_function))
	{
	MakeFileTextSolution(file_idx + "_solution_" + std::to_string(SizeOfBeam) + "_" + std::to_string(Hilo) + ".csv");
	WriteSolution(file_idx + "_solutionFran_" + std::to_string(SizeOfBeam) + "_" + std::to_string(Hilo) + ".txt");
	}*/


	return true;
}
bool Glass::BeamSearch(int Hil, int seed_id)
{
	SizeOfBeam = G_Size_width;
	Hilo = m_id;
	int Many = G_type_strip;
	NumberOfPlates = (Global_Best_Objective_Function + Total_area) / (plate_w*plate_h);
	RestOfLastPlate = (plate_w - (((Global_Best_Objective_Function + Total_area) % (plate_w*plate_h)) / plate_h)) + 1;

	//	srand(seed_id + m_id);
	//	NumberOfPlates = (Global_Best_Objective_Function + Total_area) / (plate_w*plate_h);
	//	RestOfLastPlate = (plate_w - (((Global_Best_Objective_Function + Total_area) % (plate_w*plate_h)) / plate_h)) + 1;

	Max_List_Nodos_Beam = 200;
	Total_area_used = 0;//Suma of used area partial
	struct timeb t1;

	if (Many == 0)
		Many = 100;
	double tmp_first = 0;
	cont_plates = 0;

	//Re run all the plates 
	G_level = 0;
	ListaNodosBeam.clear();
	ListaNodosBeam_G_children.clear();
	bool solutions_to_fill = false;
	int Size_lista = 4;//V8
//	printf("Valore %d %d %d\n", G_Size_alpha, G_Size_Chosen, SizeOfBeam);

	Max_List_Nodos_Beam = 12 * (G_Size_alpha+ G_Size_Chosen )* (G_iteraciones / 200) * SizeOfBeam;
	Max_List_Nodos_Beam_Per_Nodo = Max_List_Nodos_Beam;
	Max_List_Nodos_Beam_Global_Utilization = 4 * G_Size_Chosen*(G_iteraciones / 200)* SizeOfBeam;
	Max_List_Nodos_Beam_Global_Global = Max_List_Nodos_Beam_Global_Utilization;
	Max_List_Nodos_Beam_Global_Double = Max_List_Nodos_Beam_Global_Utilization;

	if (seed_id == 44)
	{
		Max_List_Nodos_Beam = 5;
		G_iteraciones = 10;


	}
	if (G_tipo_lista == 1)
	{
		Max_List_Nodos_Beam_Global_Utilization = 3 * Max_List_Nodos_Beam_Global_Utilization;
		Max_List_Nodos_Beam_Global_Global = 0;
		Max_List_Nodos_Beam_Global_Double = 0;
	}
	if (G_tipo_lista == 2)
	{
		Max_List_Nodos_Beam_Global_Double = 0;
		Max_List_Nodos_Beam_Global_Utilization = 0;
		Max_List_Nodos_Beam_Global_Global = 3 * Max_List_Nodos_Beam_Global_Global;
	}
	if (G_tipo_lista == 3)
	{
		Max_List_Nodos_Beam_Global_Utilization = 0;
		Max_List_Nodos_Beam_Global_Double = 3 * Max_List_Nodos_Beam_Global_Double;
		Max_List_Nodos_Beam_Global_Global = 0;
	}
	do
	{
		int Max_iter = G_iteraciones;
		//		ListaNodosBeam = ListaNodosBeam_G_children;
		ListaNodosBeam_G_children.clear();
		/*		int Size_lista = max(20 - G_level, static_cast<unsigned>(10));

		if (G_level > 20)
		Size_lista = 10;
		Size_lista = 10; //V2,V5,V6
		Size_lista = 7;//V7*/


		if (G_level == 0)
			Max_iter = G_iteraciones * G_Iterations;
		else
			Max_iter = ListaNodosBeam.size() - 1;
		solutions_to_fill = false;
		if (G_type_strip == 3)
		{
			int alea = get_random(0, 10);
			if (alea <= 6)
				Many = 1;
			else
				if (alea <= 9)
					Many = 2;
				else
					Many = 100;
		}
		for (G_iter = 0; G_iter < Max_iter && tmp_first<G_Time; G_iter++)
		{
			//nuevo

			//			printf("G_Iter %d %d", G_iter, Max_iter);

			int n_rectangle = 0;

			if (G_level == 0)
			{
				RestartMatrix();
				//				G_Positions_Items.clear();
				G_Rsol_Items.clear();
				Plates = 1;
				Total_width_bin = 0;
				MinDimensionPieza = MinDimensionPieza_Inicial;
				MaxMinDimensionPieza = MaxMinDimensionPieza_Inicial;
				Total_area_used = 0;

				Inicio = 0;
			}


			int Max_children = 1;
			if (G_level != 0)
				Max_children = (G_iteraciones / 5) * G_Iterations;


			bool entered = false;
			
			for (G_children = 0; G_children < Max_children && tmp_first<G_Time; G_children++)
			{
				//				printf("G_children %d %d", G_children, Max_children);
				Best_Objective_function = Global_Best_Objective_Function;
				if (G_level != 0)
					RecoverPartialSolution(G_iter);
				Inicio_Tira = Inicio;
				/*		if (Total_width_bin == 10480)
				{
				G_Best_Rsol_Items = G_Rsol_Items;
				WriteSolution();
				DibujarOpenGL();
				}
				if (G_children == 0 && G_level >= 1000 && G_iter>=0)
				{
				G_Best_Rsol_Items = G_Rsol_Items;
				WriteSolution();
				DibujarOpenGL();
				}*/

				//				srand(G_iter*SizeOfBeam);
				int Inicio2 = -1;
				int Inicio3 = Inicio;
				for (int many = 0; (many<Many && Inicio2 != 0) && (MinDimensionPieza != MAXIMUM_INT_ROADEF || Total_area != Total_area_used); many++)
				{
					//					printf("Many I %d\n", many);

					//					printf("Iter %d", many);
					cont_plates = Plates - 1;
					GlassRectangle *rectangle = new GlassRectangle(n_rectangle, min(Inicio + max1Cut, plate_w), plate_h, DefectsPlate[Plates - 1], this, Inicio);
					deterministic = false;
					//inicio ==0 means finish a plate
					if (G_level == 0 && G_iter == 3845 )
						int kk = 9;
					NumberOfPlate = Plates - 1;
					if (G_children == 30 && G_level == 5 && G_iter == 16)
						int kk = 9;
					Inicio = AsignarItemsPlate_J(*rectangle);
					delete rectangle;

					/*				if (G_children >= 0 && G_level >= 100 && G_iter>=0)
					{
					G_Best_Rsol_Items = G_Rsol_Items;
					WriteSolution();
					DibujarOpenGL();
					}*/


					//				if (Total_width_bin)
					CheckSolution();
					//InsertPartial Solution
					if (Inicio == 0 && (MinDimensionPieza != MAXIMUM_INT_ROADEF))
					{
						Total_width_bin = 0;
						Plates++;


					}
					Partial_objective_function = ((Plates - 1) * plate_h*plate_w) + (Total_width_bin * plate_h) - Total_area_used;
					Utilization = ((double)Total_area_used) / ((double)(((Plates - 1) * plate_h*plate_w) + (Total_width_bin * plate_h)));

//					printf("%d %d\n", Partial_objective_function, G_iter);
					if (MinDimensionPieza != MAXIMUM_INT_ROADEF || Total_area != Total_area_used)
						solutions_to_fill = true;
					Inicio2 = Inicio;
					if (Many != 100)
						Inicio2 = 1;
					//					printf("Many F %d\n", many);
				}
				if (G_Rsol_Items.size() == 22 && G_Rsol_Items.back().iditem()==201)
					int kk = 9;
				if (G_level == 1 && G_iter == 0 && G_children == 385)
					int kk = 9;
				//				printf("Check I \n");

				//				printf("Check I1 \n");
				if (G_Classic == false || G_level == 0)
					entered = InsertPartialSolution();
				else
					entered = InsertPartialSolutionClassical();
				//				printf("Check F2 \n");
				//				if (entered == true && G_level >= 100 )

				/*			if (G_iter == 4 && G_level == 2)
				{
				G_Best_Rsol_Items = G_Rsol_Items;
				WriteSolution();
				DibujarOpenGL();
				}*/
				//a2 es 54 plate y 1989
				//a20 es 5040
				//a17 sobre 7240
				//a19 2 paltes +1950
//				BetterSolution(4);
				if (G_finish_instant==false && BetterSolution(4) == false)
				{
					if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);

					G_children = Max_children;

//					RemoveNodo();
				}
				
				//			printf("Here2 %d\n",Many);

				//			printf("Here3 Time %f\n",tmp_first);
				//				else
				//				solutions_to_fill = true;
				//				if (inicio == 0)
				ftime(&t1);
				tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;
				//			printf("Here3 Time %f\n", tmp_first);
				//				printf("Tiempo Total %.3f  ahora %d iter %d Hilo %d\n", tmp_first, G_Time, Max_iter, Hilo);
				CheckSolution();
			}
			if (G_Classic == true && G_level != 0)
				InsertClassical();
			ftime(&t1);
			tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;
			//			printf("Tiempo Total 0 %.3f  ahora %d iter %d Hilo %d\n", tmp_first, G_Time, Max_iter, Hilo);
			//		printf("Here4 Time %f Total tiem %d\n", tmp_first,G_Time);


		}
		//		printf("Here42 Time %f Total tiem %d\n", tmp_first, G_Time);
		ftime(&t1);
		tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;
		//		printf("Tiempo Total 1 %.3f  ahora %d iter %d Hilo %d\n", tmp_first,G_Time,G_iter,Hilo);
		if (tmp_first > G_Time)
			return true;
		RemoveDominated();
		FinishSolutions();
		//		printf("Tiempo Total 2 %.3f  ahora %d iter %d Hilo %d\n", tmp_first, G_Time, G_iter, Hilo);

		//		DoubleEvaluation();
		ftime(&t1);
		tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;
		//		printf("Here5 Time %f Total tiem %d\n", tmp_first, G_Time);
		if (tmp_first > G_Time)
			return true;
		//		printf("Level %d Tam Lista Children %d Iterations %d Size %d Many %d Thread %d\n \n", G_level, ListaNodosBeam_G_children.size(), Iterations, SizeOfBeam, Many,Hilo);
		int size_possibles = ListaNodosBeam_G_children.size();

		SelectByGlobalObjectiveFunction();
		if (active_log_error) printf("Level %d Tam Lista Possibles %d Chosen %d Iterations %d Size %d Many %d Thread %d Iter %d Children %d MAXP %d MAX %d\n \n", G_level, size_possibles, ListaNodosBeam.size(), G_Iterations, SizeOfBeam, Many, Hilo, G_iter, G_children, Max_List_Nodos_Beam, Max_List_Nodos_Beam_Global_Utilization);
		//		DibujarChosen();
		//		FinishSolutions();
		G_level++;
		//		printf("Tiempo Total 3 %.3f  ahora %d iter %d Hilo %d\n", tmp_first, G_Time, G_iter, Hilo);

		/*
		//For writing all the solutions
		if (draw_solution == true && Best_iter == G_iter)
		{
		G_Best_Rsol_Items = G_Rsol_Items;
		G_Best_Positions_Items = G_Positions_Items;
		Best_Objective_function = Objective_function;
		WriteSolution();
		DibujarOpenGL();
		}*/

	} while (solutions_to_fill == true && (ListaNodosBeam_G_children.size()>0) && tmp_first<G_Time);
	//For writing all the solutions

	/*	if (G_Best_Rsol_Items.size() > 0 && (Global_Best_Objective_Function == Best_Objective_function))
	{
	MakeFileTextSolution(file_idx + "_solution_" + std::to_string(SizeOfBeam) + "_" + std::to_string(Hilo) + ".csv");
	WriteSolution(file_idx + "_solutionFran_" + std::to_string(SizeOfBeam) + "_" + std::to_string(Hilo) + ".txt");
	}*/


	return true;
}
void Glass::RemoveNodo()
{
	if (G_Classic == false)
	{
		for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children.begin(); it != ListaNodosBeam_G_children.end(); it++)
		{
			//				printf("Check IP3 \n");
			if (NotEqualCurrent((*it)) == false)
			{
				ListaNodosBeam_G_children.erase(it);
				return;
			}
		}
	}
	else
	{
		for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children_Classical.begin(); it != ListaNodosBeam_G_children_Classical.end(); it++)
		{
			if (NotEqualCurrent((*it)) == false)
			{
				ListaNodosBeam_G_children_Classical.erase(it);
				return;
			}
		}
	}


}
void Glass::DibujarChosen()
{
	for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam.begin(); it != ListaNodosBeam.end(); it++)
	{


		//Pass everything to the current solution
		Inicio = (*it).Inicio();
		Plates = (*it).Plates();
		Total_area = (*it).Total_area(); // Sum of total area of items.
		Total_area_used = (*it).Total_area_used();//Suma of used area partial
		Total_width_bin = (*it).Total_width_bin();
		Max_First_Bin = (*it).Max_First_Bin();
		Objective_function = (*it).Objective_function();
		Partial_objective_function = (*it).Partial_objective_function();
		MinDimensionPieza = (*it).MinDimensionPieza();
		G_Matrix_Items = (*it).Matrix_Items();
		G_Vector_Items = (*it).Vector_Items();
		G_Vector_Stacks = (*it).Vector_Stacks();
		G_Rsol_Items = (*it).Sol_Items();
		G_Best_Rsol_Items = G_Rsol_Items;
		WriteSolution();
		DibujarOpenGL();
	}

}

void Glass::LevelEvaluation(int level, GlassNodeB  &nodo)
{
	double Utilization2 = 0, Max_Utilization2 = 0;
	int Many = level;
	for (int i = 0; i < Max_children_Level; i++)
	{

		RecoverPartialSolution(nodo);
		/*	if (G_children == 0 && G_level >= 1000 && G_iter >= 0)
		{
		G_Best_Rsol_Items = G_Rsol_Items;
		WriteSolution();
		DibujarOpenGL();
		}*/
		//				srand(G_iter*SizeOfBeam);
		int Inicio2 = -1;

		for (int many = 0; (many < Many && Inicio2 != 0) && (MinDimensionPieza != MAXIMUM_INT_ROADEF || Total_area != Total_area_used); many++)
		{

			//					printf("Iter %d", many);
			//Revisar
			cont_plates = Plates - 1;
			GlassRectangle *rectangle = new GlassRectangle(1, min(Inicio + max1Cut, plate_w), plate_h, DefectsPlate[Plates - 1], this, Inicio);
			deterministic = false;
			//inicio ==0 means finish a plate

			NumberOfPlate = (Plates - 1);
			Inicio = AsignarItemsPlate_J(*rectangle);
			delete rectangle;

			/*				if (G_children >= 0 && G_level >= 100 && G_iter>=0)
			{
			G_Best_Rsol_Items = G_Rsol_Items;
			WriteSolution();
			DibujarOpenGL();
			}*/

			//				if (Total_width_bin)
			CheckSolution();
			if (Inicio == 0 && (MinDimensionPieza != MAXIMUM_INT_ROADEF))
			{
				Total_width_bin = 0;
				Plates++;

			}

			Inicio2 = Inicio;
			if (Many != 100)
				Inicio2 = 1;

		}
		Utilization2 = ((double)Total_area_used) / ((double)(((Plates - 1) * plate_h*plate_w) + (Total_width_bin * plate_h)));
		BetterSolution(1);
		if (Utilization2 > Max_Utilization2)
			Max_Utilization2 = Utilization2;


	}
	nodo.Utilization2(Max_Utilization2);
}
int Glass::MaxWidthLastPiece(std::list< GlassRsol >  &lista_temp)
{
	std::list< GlassRsol > ::iterator it;
	int max = 0;
	for (it = lista_temp.begin(); it != lista_temp.end(); it++)
	{
		if ((*it).plateId() != 0)
			return max;
		if ((*it).X() > max)
			max = (*it).X();
	}
	return max;
}
int Glass::MinWidthLastPiece(std::list< GlassRsol >  &lista_temp)
{
	std::list< GlassRsol > ::iterator it;
	int min = plate_w;
	for (it = lista_temp.begin(); it != lista_temp.end(); it++)
	{
		if ((*it).plateId() != 0)
			return min;
		if ((*it).x() < min)
			min = (*it).X();
	}
	return min;
}
void Glass::GenerarInstanciasExacto()
{
	for (int type = 0; type < 2; type++)
	{
		for (int NPiezas = 10; NPiezas < 12; NPiezas += 10)
		{


//			std::size_t found = file_idx.find("\\");
//			std::string name_r;
//			if (found != std::string::npos)
//				name_r = file_idx.substr( file_idx.find("\\")+1, std::string::npos);

			std::string name = file_idx + "_" + std::to_string(NPiezas) + "_" + std::to_string(type) + "_batch.csv";
			FILE* read;
			read = fopen(name.c_str(), "w+");
			if (read == NULL)
			{
				fprintf(stderr, "Can't open input file %s!\n", name.c_str());
				exit(0);
			}
			fprintf(read, "ITEM_ID;LENGTH_ITEM;WIDTH_ITEM;STACK;SEQUENCE\n");

			if (type == 0)
			{
				for (register int i = 0; i < min(NPiezas, batch_items); i++)
				{


					fprintf(read, "%d;%d;%d;%d;%d\n", i, items[i].Getitem_h(), items[i].Getitem_w(), items[i].Getitem_stack(), items[i].Getitem_seq());

				}
			}
			else
			{
				int puestas = 0;
				std::vector<int> piezas_por_colocar(stack_nbr);
				for (register int j = 0; j < NPiezas && puestas < NPiezas; j++)
				{
					for (register int i = 0; i < min(stack_nbr, NPiezas) && puestas < NPiezas; i++)
					{
						if (j < stacks[i].Getitem_nbr())
						{
							puestas++;
							piezas_por_colocar[i]++;
						}
					}
				}
				//ahora las coloco
				int cont = 0;
				int puestas_ahora = 0;
				for (register int i = 0; i < min(stack_nbr, NPiezas) && puestas_ahora < NPiezas; i++)
				{
					for (register int j = 0; j < piezas_por_colocar[i]; j++)
					{

						fprintf(read, "%d;%d;%d;%d;%d\n", puestas_ahora, stacks[i].Getitem(j).Getitem_h(), stacks[i].Getitem(j).Getitem_w(), stacks[i].Getitem(j).Getitem_stack(), stacks[i].Getitem(j).Getitem_seq());
						puestas_ahora++;
					}
				}

			}
		}
	}
	//Copiar defectos
	for (int type = 0; type < 2; type++)
	{
		for (int NPiezas = 10; NPiezas < 12; NPiezas += 10)
		{
//			std::size_t found = file_idx.find("\\");
//			std::string name_r;
//			if (found != std::string::npos)
//				name_r = file_idx.substr(file_idx.find("\\") + 1, std::string::npos);

//			std::string name = name_r + "_" + std::to_string(NPiezas) + "_" + std::to_string(type) +"_defects.csv";
			
			std::string name = file_idx + "_" + std::to_string(NPiezas) + "_" + std::to_string(type) + "_defects.csv";
			FILE* read;
			read = fopen(name.c_str(), "w+");
			if (read == NULL)
			{
				fprintf(stderr, "Can't open input file %s!\n", name.c_str());
				exit(0);
			}
			std::string name_defectos = file_idx + "_defects.csv";
			FILE * read2;
			read2 = fopen(name_defectos.c_str(), "r+");
			if (read2 == NULL)
			{
				fprintf(stderr, "Can't open input file %s!\n", name.c_str());
				exit(0);
			}
			int data1 = 0;
			while ((data1 = fgetc(read2)) != EOF) {
				fputc(data1, read);
			}

			fclose(read);
			fclose(read2);
		}
	}

}
bool Glass::BetterSolution(int a)
{
	if (MinDimensionPieza == MAXIMUM_INT_ROADEF || Total_area == Total_area_used)
	{
		int Aleatorio = get_random(1, 99999);
		if (G_Rotate == true)
		{
			//			G_Best_Rsol_Items = G_Rsol_Items;

			//							if (G_level >=1990)
			//					{
			//		WriteSolution();
			//	DibujarOpenGL();
			if ((Plates - 1) > NumberOfPlates)  return true;

			//First without remove  the rest, after removing
			//			int Width_First_Bin = 0;
			//		Width_First_Bin = MaxWidthLastPiece(G_Rsol_Items);
			if (Plates == 1)
				Objective_function = ((Plates - 1) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h) - Total_area_used;
			else
				Objective_function = ((Plates - 1) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h) - Total_area_used;

			if (Objective_function < Global_Best_Objective_Function)
			{
				if (CheckSolutionEnd() == false)
					return false;
				if (active_log_error) printf("Objective_function %d best %d Partial Utilization %f Place %d  Iter %d Rotate %d\n", Objective_function, Global_Best_Objective_Function, Utilization, a, m_id, G_Rotate);
				//				ChangeRectanglesSimmetry(Width_First_Bin);
				std::list<GlassRsol> lista_Temp = G_Best_Rsol_Items;

				G_Best_Rsol_Items = G_Rsol_Items;

				//							if (G_level >=1990)
				//					{
				if (G_Dibujar == true)		WriteSolution();
				if (G_Dibujar==true)		DibujarOpenGL();
				//				}
				//						printf("Path %s",file_idx.c_str());

				MakeFileTextSolution(file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv");
				TransformarSolucionSimetrica(file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv", false);

				if (parseSolution(file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv") != EXIT_FAILURE)
				{
					if (verify_main() != EXIT_SUCCESS)
					{
						//					WriteSolution();
						//				DibujarOpenGL();
						G_Best_Rsol_Items = lista_Temp;
						remove((file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv").c_str());
						return false;
						//						return;
						//					{

						//				PrintProblem("Verificar");
					}
					else
					{
						Best_Utilization = ((double)Total_area_used) / ((double)((Plates - 1) * plate_h*plate_w) + ((Total_width_bin - RestOfLastPlate) * plate_h));
						Best_iter = G_level;
						Best_plates_nbr = Plates;
						G_Best_Rsol_Items = G_Rsol_Items;
						Best_Width = (Total_width_bin - RestOfLastPlate);
						G_Best_Solution_Rotated = G_Solution_Rotated;
						Best_Objective_function = Objective_function;
						Best_Objective_function_Thread = Objective_function;
						Global_Best_Objective_Function = Best_Objective_function;
					}
				}
				else
				{
					G_Best_Rsol_Items = lista_Temp;
					remove((file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv").c_str());
					return false;
				}
				remove((file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv").c_str());
			}

			int Width_First_Bin = 0;

			Width_First_Bin = MaxWidthLastPiece(G_Rsol_Items);
			//			if (Width_First_Bin>)
			Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Width_First_Bin - RestOfLastPlate) * plate_h) - Total_area_used;
			if (Objective_function < Global_Best_Objective_Function)
			{
				if (CheckSolutionEnd() == false)
					return false;
				//				if (active_log_error) printf("Objective_function %d best %d Partial Utilization %f Place %d\n", Objective_function, Global_Best_Objective_Function, Utilization, a);
				if (active_log_error) printf("Objective_function %d best %d Partial Utilization %f Place %d  Iter %d Rotate %d\n", Objective_function, Global_Best_Objective_Function, Utilization, a, m_id, G_Rotate);

				ChangeRectanglesSimmetry(Width_First_Bin);
				std::list<GlassRsol> lista_Temp = G_Best_Rsol_Items;
				G_Best_Rsol_Items = G_Rsol_Items;

				//							if (G_level >=1990)
				//					{
				//				WriteSolution();
				//			DibujarOpenGL();
				//				}
				//						printf("Path %s",file_idx.c_str());

				MakeFileTextSolution(file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv");
				TransformarSolucionSimetrica(file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv", true);

				if (parseSolution(file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv") != EXIT_FAILURE)
				{
					if (verify_main() != EXIT_SUCCESS)
					{
//						WriteSolution();
	//					DibujarOpenGL();
						G_Best_Rsol_Items = lista_Temp;
						remove((file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv").c_str());
						return false;
						//					{
						//					WriteSolution();
						//				DibujarOpenGL();
						//				PrintProblem("Verificar");
					}
					else
					{
						Best_Utilization = ((double)Total_area_used) / ((double)((Plates - 1) * plate_h*plate_w) + ((Width_First_Bin - RestOfLastPlate) * plate_h));
						Best_iter = G_level;
						Best_plates_nbr = Plates;
						Best_Width = (Width_First_Bin - RestOfLastPlate);
						G_Best_Rsol_Items = G_Rsol_Items;
						G_Best_Solution_Rotated = G_Solution_Rotated;
						Best_Objective_function = Objective_function;
						Best_Objective_function_Thread = Objective_function;
						Global_Best_Objective_Function = Best_Objective_function;
					}
				}
				
				else
				{
					G_Best_Rsol_Items = lista_Temp;
					remove((file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv").c_str());
					return false;
				}
				remove((file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv").c_str());
				//					printf("Here\n");
			}
		}
		else
		{
			//		if (G_Rotate == false)
			int allways = true;
			Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Total_width_bin)* plate_h) - Total_area;
			//					if (Objective_function < Best_Objective_function)Global_Best_Objective_Function
			if (Objective_function < Global_Best_Objective_Function )

			{
				if (CheckSolutionEnd() == false)
					return false;
				if (active_log_error) printf("Objective_function_1 %d best %d Partial Utilization %f Place %d  Iter %d Rotate %d\n", Objective_function, Global_Best_Objective_Function, Utilization, a, m_id, G_Rotate);

				//				if (active_log_error) printf("Objective_function1 %d best %d Partial Utilization %f Place %d\n", Objective_function, Global_Best_Objective_Function, Utilization, a);
				//				Global_Best_Objective_Function = Objective_function;
				//			if (G_Rotate == true)
				//			ChangeRectanglesSimmetry(Width_First_Bin);
				//				printf("G_level %d G_iter %d G_children %d Value %d Plates %d Width %d Best %d G_iter %d Thread %d\n", G_level, G_iter, G_children, Objective_function, Plates, Total_width_bin, Best_Objective_function, Best_iter,Hilo);
				std::list<GlassRsol> lista_Temp = G_Best_Rsol_Items;
				G_Best_Rsol_Items = G_Rsol_Items;
				//					WriteSolution();
				//						DibujarOpenGL();

				//							if (G_level >=1990)
				//					{
//							WriteSolution();
//							DibujarOpenGL();

				if (G_Dibujar == true)		WriteSolution();
				if (G_Dibujar == true)		DibujarOpenGL();
				//				}
				//						printf("Path %s",file_idx.c_str());

				MakeFileTextSolution(file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv");
				//			if (G_Rotate == true)
				//			TransformarSolucionSimetrica(file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Hilo) + "_" + std::to_string(G_Time) + ".csv");

				if (parseSolution(file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv") != EXIT_FAILURE)
				{
					if (G_Param!=4)
					parseDefects(file_idx + "_defects.csv");
					if (verify_main() != EXIT_SUCCESS)


					{


																WriteSolution();
												//			exit(2);
										if (G_Dibujar)							DibujarOpenGL();
						if (active_log_error) printf("falla \n");
						G_Best_Rsol_Items = lista_Temp;
						remove((file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv").c_str());
//
						return true;
						return false;
						
						//					{
						//				WriteSolution(file_idx+"Fran.txt");
						//			exit(2);
						//					DibujarOpenGL();
						//				PrintProblem("Verificar");
					}
					else
					{
						//						if (G_Rotate == false)
						Best_Utilization = ((double)Total_area_used) / ((double)(((Plates - 1) * plate_h*plate_w) + ((Total_width_bin - TrozoBin) * plate_h)));
						//					else
						//				{
						// Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Width_First_Bin - RestOfLastPlate) * plate_h) - Total_area_used;
						//				if (Plates == 1)
						//				Best_Utilization = ((double)Total_area_used) / ((double)((Plates - 1) * plate_h*plate_w) + ((Width_First_Bin - RestOfLastPlate) * plate_h));
						//						else
						//						Best_Utilization = ((double)Total_area_used) / ((double)(((Plates - 2) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h)) + ((Total_width_bin)* plate_h));

						//		}
						Best_iter = G_level;
						Best_plates_nbr = Plates;
						Best_Width = (Total_width_bin - TrozoBin);

						G_Best_Rsol_Items = G_Rsol_Items;
						Best_Objective_function = Objective_function;
						Best_Objective_function_Thread = Objective_function;
						Global_Best_Objective_Function = Best_Objective_function;
					}
				}

				else
				{
//					WriteSolution();
//					DibujarOpenGL();
					if (active_log_error) printf("falla el primer parse\n");
					G_Best_Rsol_Items = lista_Temp;
					remove((file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv").c_str());
					return true;
					return false;
				}
				remove((file_idx + "_solution_" + std::to_string(m_id) + "_" + std::to_string(Aleatorio) + "_" + std::to_string(G_Time) + ".csv").c_str());
				//					printf("Here\n");
			}

		}
	}
	return true;
}
void Glass::FinishSolutions()
{
	double tmp_first = 0;
	struct timeb t1;
	if (G_finish_instant)
		return;
	for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children.begin(); it != ListaNodosBeam_G_children.end() && tmp_first < G_Time; it++)
	{


		//Pass everything to the current solution
		Inicio = (*it).Inicio();
		Plates = (*it).Plates();
		Total_area = (*it).Total_area(); // Sum of total area of items.
		Total_area_used = (*it).Total_area_used();//Suma of used area partial
		Total_width_bin = (*it).Total_width_bin();
		Max_First_Bin = (*it).Max_First_Bin();
		Objective_function = (*it).Objective_function();
		Partial_objective_function = (*it).Partial_objective_function();
		MinDimensionPieza = (*it).MinDimensionPieza();
		G_Matrix_Items = (*it).Matrix_Items();
		G_Vector_Items = (*it).Vector_Items();
		G_Vector_Stacks = (*it).Vector_Stacks();
		G_Rsol_Items = (*it).Sol_Items();
		if (G_level == 0 && G_iter == 1880 && G_children == 1 )
			int kk = 0;
		FinishSolution();
		if (BetterSolution(3) == false)
		{
			if (active_log_error) printf("Fallo Level %d Iter %d Children %d \n", G_level, G_iter, G_children);

			Objective_function = MAXIMUM_INT_ROADEF;
			(*it).Partial_objective_function(MAXIMUM_INT_ROADEF);
		}
		(*it).Objective_function(Objective_function);

		ftime(&t1);
		tmp_first = ((double)((t1.time - G_Time_Initial.time) * 1000 + t1.millitm - G_Time_Initial.millitm)) / 1000;



	}
	//Delete the worse than before
	if (ListaNodosBeam_G_children.size() == 0)
		return;
	std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children.begin();
	do
	{
		if ((*it).Partial_objective_function() >= Best_Objective_function)
			it = ListaNodosBeam_G_children.erase(it);
		else
			it++;

	} while (it != ListaNodosBeam_G_children.end());
}

void Glass::DoubleEvaluation()
{

	for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children.begin(); it != ListaNodosBeam_G_children.end(); it++)
	{


		//Pass everything to the current solution
		Inicio = (*it).Inicio();
		Plates = (*it).Plates();
		Total_area = (*it).Total_area(); // Sum of total area of items.
		Total_area_used = (*it).Total_area_used();//Suma of used area partial
		Total_width_bin = (*it).Total_width_bin();
		Max_First_Bin = (*it).Max_First_Bin();
		Objective_function = (*it).Objective_function();
		Partial_objective_function = (*it).Partial_objective_function();
		MinDimensionPieza = (*it).MinDimensionPieza();
		G_Matrix_Items = (*it).Matrix_Items();
		G_Vector_Items = (*it).Vector_Items();
		G_Vector_Stacks = (*it).Vector_Stacks();
		G_Rsol_Items = (*it).Sol_Items();
		LevelEvaluation(1, (*it));


	}

	//	InsertarSolucionGlobal();
}
void Glass::RecoverPartialSolution(GlassNodeB &nodo)
{
	//Pass everything to the current solution
	Inicio = nodo.Inicio();
	Plates = nodo.Plates();
	Total_area = nodo.Total_area(); // Sum of total area of items.
	Total_area_used = nodo.Total_area_used();//Suma of used area partial
	Total_width_bin = nodo.Total_width_bin();
	Objective_function = nodo.Objective_function();
	Partial_objective_function = nodo.Partial_objective_function();
	MinDimensionPieza = nodo.MinDimensionPieza();
	G_Matrix_Items = nodo.Matrix_Items();
	G_Vector_Items = nodo.Vector_Items();
	G_Vector_Stacks = nodo.Vector_Stacks();
	G_Rsol_Items = nodo.Sol_Items();
}
void Glass::InsertarSolucionGlobal()
{
	std::list<GlassNodeB> lista_temp;
	//We have a finished solution
	//or 
	//we have a better solution already computed

	for (std::list< GlassNodeB > ::iterator it = ListaNodosBeam_G_children.begin(); it != ListaNodosBeam_G_children.end(); it++)
	{



		if (lista_temp.size() == 0)
		{
			lista_temp.push_back((*it));
			break;
		}

		if ((lista_temp.size() < 2 * Max_List_Nodos_Beam || Objective_function < (lista_temp.back().Utilization() - DBL_EPSILON)))
		{
			int cont = 0;
			for (std::list< GlassNodeB > ::iterator it2 = lista_temp.begin(); it2 != lista_temp.end() && cont < Max_List_Nodos_Beam; it2++, cont++)
			{
				if ((*it).Objective_function() < Objective_function)
					continue;
				else
				{
					if ((*it2).Utilization() > ((*it).Utilization() - DBL_EPSILON))
					{
						lista_temp.insert(it2, (*it));
					}

				}

			}
			lista_temp.push_back((*it));
			break;

		}
		//The last one
		if (lista_temp.size() < 2 * Max_List_Nodos_Beam)
		{
			lista_temp.push_back((*it));
			break;
		}


	}
	return;

}
void Glass::Randomize(int iter)
{
	for (int i = 0; i < iter; i++)
	{
		RestartMatrix();
		//				G_Positions_Items.clear();
		G_Rsol_Items.clear();
		Plates = 1;
		Total_width_bin = 0;
		MinDimensionPieza = MinDimensionPieza_Inicial;
		MaxMinDimensionPieza = MaxMinDimensionPieza_Inicial;
		Total_area_used = 0;

		Inicio = 0;
		RandomSolution();
		if (Objective_function < Global_Best_Objective_Function)
			Global_Best_Objective_Function = Objective_function;

	}
}
void Glass::RandomSolution()
{

	//	printf("Check F1 \n");
	int n_rectangle = 0;
	deterministic = true;//V3
	do
	{
		int defectsPlate = Plates - 1;
		cont_plates = Plates - 1;
		if (G_Rotate == true)
		{
			defectsPlate = NumberOfPlates - (Plates - 1);
			if (defectsPlate < 0)
			{
				if (Max_List_Nodos_Beam_Global_Global == 0)
				{
					deterministic = false;
					Objective_function = ((NumberOfPlates + 1)* plate_h*plate_w) - Total_area_used;
					//				printf("Check F2 \n");
					return;
				}
				else
					defectsPlate = Plates;

			}
			NumberOfPlate = defectsPlate;

		}
		else
			NumberOfPlate = (Plates - 1);



		GlassRectangle *rectangle = new GlassRectangle(n_rectangle, min(Inicio + max1Cut, plate_w), plate_h, DefectsPlate[NumberOfPlate], this, Inicio);

		//inicio ==0 means finish a plate

		int InicioPrevio = Total_width_bin;
		//		printf("Check AI1 %d\n", MinDimensionPieza);
		Inicio = AsignarItemsPlate_J(*rectangle);
		//		printf("Check AI2 %d\n", MinDimensionPieza);
		delete rectangle;
		if ((Total_area > Total_area_used && Inicio != 0) &&
			(Total_width_bin == InicioPrevio ||
			((plate_w - Total_width_bin) != MinDimensionPieza && (plate_w - Total_width_bin)<(MinDimensionPieza + waste_min))))
		{
			Total_width_bin = plate_w;
			Inicio = 0;
		}
		//				if (Total_width_bin)
		CheckSolution();

		//InsertPartial Solution
		if (Inicio == 0)
		{
			if (MinDimensionPieza != MAXIMUM_INT_ROADEF)
			{
				Total_width_bin = 0;
				Plates++;
				/*				if (G_Rotate == true)
				{
				if (Plates > NumberOfPlates)
				{
				deterministic = false;
				Objective_function = MAXIMUM_INT_ROADEF;
				}
				}*/

			}
		}
		//New
		if (Max_List_Nodos_Beam_Global_Global >= 0)
		{
			if (G_Rotate == true)
			{
				if (Plates == 1)
					Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Total_width_bin - RestOfLastPlate) * plate_h) - Total_area_used;
				else
					//			Objective_function = ((Plates - 2) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h) - Total_area_used;
					Objective_function = ((Plates - 2) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h) + ((Total_width_bin)* plate_h) - Total_area_used;

			}
			else
				Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Total_width_bin)* plate_h) - Total_area_used;
			if (Objective_function > Best_Objective_function)
			{
				deterministic = false;
				Objective_function = ((NumberOfPlates + 1)* plate_h*plate_w) - Total_area_used;
				return;

			}
		}
		//		printf("Check F2 %d\n",MinDimensionPieza);
		//
	} while (MinDimensionPieza != MAXIMUM_INT_ROADEF);
	//	printf("Check F3 %d\n", MinDimensionPieza);
	deterministic = false;
	if (G_Rotate == true)
	{
		if (Plates == 1)
			Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Total_width_bin - RestOfLastPlate) * plate_h) - Total_area_used;
		else
			//			Objective_function = ((Plates - 2) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h) - Total_area_used;
			Objective_function = ((Plates - 2) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h) + ((Total_width_bin)* plate_h) - Total_area_used;

	}
	else
		Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Total_width_bin)* plate_h) - Total_area_used;

	/*	if (Objective_function < Best_Objective_function)
	{
	printf("terminando %d %d Inicio %d Veces %d Iter %d Level %d Children %d\n", Objective_function, Best_Objective_function,G_iter_inicio, G_iter_veces,G_iter,G_level,G_children);
	Best_iter = 80000 ;
	Best_plates_nbr = Plates;
	Best_Objective_function = Objective_function;
	G_Best_Rsol_Items = G_Rsol_Items;

	WriteSolution();
	DibujarOpenGL();
	MakeFileTextSolution(file_idx + "_solution.csv");
	parseSolution(file_idx + "_solution.csv");
	if (verify_main() != EXIT_SUCCESS)
	{

	PrintProblem("Verificar");
	}
	}
	*/
}



void Glass::FinishSolution()
{

	//	printf("Check F1 \n");
	int n_rectangle = 0;
	deterministic = true;//V3
	do
	{
		int defectsPlate = Plates - 1;
		cont_plates = Plates - 1;
		if (G_Rotate == true)
		{
			defectsPlate = NumberOfPlates - (Plates - 1);
			if (defectsPlate < 0)
			{
				if (Max_List_Nodos_Beam_Global_Global == 0)
				{
					deterministic = false;
					Objective_function = ((NumberOfPlates + 1)* plate_h*plate_w) - Total_area_used;
					//				printf("Check F2 \n");
					return;
				}
				else
					defectsPlate = Plates;

			}
			NumberOfPlate = defectsPlate;

		}
		else
			NumberOfPlate = (Plates - 1);



		GlassRectangle *rectangle = new GlassRectangle(n_rectangle, min(Inicio + max1Cut, plate_w), plate_h, DefectsPlate[NumberOfPlate], this, Inicio);

		//inicio ==0 means finish a plate

		int InicioPrevio = Total_width_bin;
		//		printf("Check AI1 %d\n", MinDimensionPieza);
		Inicio = AsignarItemsPlate_J(*rectangle);
		//		printf("Check AI2 %d\n", MinDimensionPieza);
		delete rectangle;
		if ((Total_area > Total_area_used && Inicio != 0) &&
			(Total_width_bin == InicioPrevio ||
			((plate_w - Total_width_bin) != MinDimensionPieza && (plate_w - Total_width_bin)<(MinDimensionPieza + waste_min))))
		{
			Total_width_bin = plate_w;
			Inicio = 0;
		}
		//				if (Total_width_bin)
		CheckSolution();

		//InsertPartial Solution
		if (Inicio == 0)
		{
			if (MinDimensionPieza != MAXIMUM_INT_ROADEF)
			{
				Total_width_bin = 0;
				Plates++;
				/*				if (G_Rotate == true)
				{
				if (Plates > NumberOfPlates)
				{
				deterministic = false;
				Objective_function = MAXIMUM_INT_ROADEF;
				}
				}*/

			}
		}
		//New
		if (Max_List_Nodos_Beam_Global_Global >= 0)
		{
			if (G_Rotate == true)
			{
				if (Plates == 1)
					Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Total_width_bin - RestOfLastPlate) * plate_h) - Total_area_used;
				else
					//			Objective_function = ((Plates - 2) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h) - Total_area_used;
					Objective_function = ((Plates - 2) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h) + ((Total_width_bin)* plate_h) - Total_area_used;

			}
			else
				Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Total_width_bin)* plate_h) - Total_area_used;
			if (Objective_function > Best_Objective_function)
			{
				deterministic = false;
				Objective_function = ((NumberOfPlates + 1)* plate_h*plate_w) - Total_area_used;
				return;

			}
		}
		//		printf("Check F2 %d\n",MinDimensionPieza);
		//
	} while (MinDimensionPieza != MAXIMUM_INT_ROADEF);
	//	printf("Check F3 %d\n", MinDimensionPieza);
	deterministic = false;
	if (G_Rotate == true)
	{
		if (Plates == 1)
			Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Total_width_bin - RestOfLastPlate) * plate_h) - Total_area_used;
		else
			//			Objective_function = ((Plates - 2) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h) - Total_area_used;
			Objective_function = ((Plates - 2) * plate_h*plate_w) + ((plate_w - RestOfLastPlate) * plate_h) + ((Total_width_bin)* plate_h) - Total_area_used;

	}
	else
		Objective_function = ((Plates - 1) * plate_h*plate_w) + ((Total_width_bin)* plate_h) - Total_area_used;

	/*	if (Objective_function < Best_Objective_function)
	{
	printf("terminando %d %d Inicio %d Veces %d Iter %d Level %d Children %d\n", Objective_function, Best_Objective_function,G_iter_inicio, G_iter_veces,G_iter,G_level,G_children);
	Best_iter = 80000 ;
	Best_plates_nbr = Plates;
	Best_Objective_function = Objective_function;
	G_Best_Rsol_Items = G_Rsol_Items;

	WriteSolution();
	DibujarOpenGL();
	MakeFileTextSolution(file_idx + "_solution.csv");
	parseSolution(file_idx + "_solution.csv");
	if (verify_main() != EXIT_SUCCESS)
	{

	PrintProblem("Verificar");
	}
	}
	*/
}

/*
bool SecondFilter()
{
bool remove = false;
bool some_change = false;
std::list< GlassNodeB > ::iterator it, it2;
bool equal = true;
remove = false;
if (ListaNodosBeam_G_children.size() == 0)
return some_change;
it = ListaNodosBeam_G_children.begin();
int  Val = 0;
do
{
//Finish solution
//		FinishSolution();
// insert solutuion

if (Val)
it = ListaNodosBeam_G_children.erase(it2);
else
it++;

} while (it != ListaNodosBeam_G_children.end());

return some_change;



}*/
int Glass::MinimaDimension(GlassRectangle &Rect, GlassRsol &Sol)
{
	int Min = MAXIMUM_INT_ROADEF;
	int MaxPieces = stack_nbr;
	int AreaPosible = 0;
	if (Rect.Getsidecut() == 1) MaxPieces = 2 * stack_nbr;
	if (Rect.Getsidecut() == 2) MaxPieces = stack_nbr;
	int Pieces = 0;
	for (register int j = 0; j < 2; j++)
	{
		if (Pieces > MaxPieces)
			return Min;
		for (register int i = 0; i < stack_nbr; i++)
		{
			//If there are items in this stacks
			if ((G_Vector_Stacks[i] + j) < (G_Vector_Size_Stacks[i]))
			{
				Pieces++;
				AreaPosible += items[G_Matrix_Items[i][G_Vector_Stacks[i] + j]].Getitem_area();
				if (items[G_Matrix_Items[i][G_Vector_Stacks[i] + j]].Getitem_min() < Min)
					Min = items[G_Matrix_Items[i][G_Vector_Stacks[i] + j]].Getitem_min();
			}
		}
	}
	return Min;
}
//Add the item to a list of place items
//Add one in the list of vector items
//Create two new rectangles one to the right and other to the top
Coordinates Glass::PlaceItem(Coordinates p, GlassRectangle &Rect)
{

	int stMore = 0;
	//	if (deterministic != true)
	//	stMore = get_random(0, 500);

	//int parent = Rect.GetRectangle_id();
	

	bool primer = false;
	if (Rect.Getpos_x() == 0 && Rect.Getpos_X() == plate_w)
		primer = true;
	std::pair <int, int > xy(0, 0);
	std::pair <int, int > wh(0, 0);
	Coordinates p1(xy, wh);
	p1.second.first = Rect.Getpos_X();
	p1.second.second = Rect.Getpos_Y();
	//
	Rect.Print();
	GlassRsol Sol = GlassRsol();
	std::vector < int > vec(9);
	Sol.iditem(p.first.first);
	Sol.x(Rect.Get_x());
	Sol.y(Rect.Get_y());
	Sol.plateId(cont_plates);
	Sol.Setcut(Rect.Getsidecut());

	vec[0] = p.first.first;
	vec[1] = Rect.Get_x();
	vec[2] = Rect.Get_y();
	vec[5] = cont_plates;

	//For doing the plates
	std::vector <int > temp(6);
	temp[1] = Rect.Getpos_x();
	temp[2] = Rect.Getpos_y();
	temp[3] = Rect.Getpos_X();
	temp[4] = Rect.Getpos_Y();
	vec[5] = cont_plates;
	vec[6] = Rect.Getsidecut();
	vec[7] = p.second.first; //copies x
	vec[8] = p.second.second; // copies y




							  //	PrintProblem("Pone mas cortes");
							  //We have to place the item in the position
							  //left bottom corner
	int width_x = items[p.first.first].Getitem_max();
	//Place the item
	//Horizontal
	if (p.first.second == true)
	{
		//if not ( type 3 and there is a defect)
		//		if ((!(Rect.Getsidecut() == 3 && Rect.Get_y() != Rect.Getpos_y())) || ((Rect.Getpos_Y() - Rect.Get_y()) < items[p.first.first].Getitem_min()*p.second.second))
		if (To_the_Top == false)
		{
			vec[3] = Rect.Get_x() + items[p.first.first].Getitem_max()*p.second.first;
			vec[4] = Rect.Get_y() + items[p.first.first].Getitem_min()*p.second.second;
			if ((Rect.Getpos_Y() - Rect.Get_y()) < items[p.first.first].Getitem_min()*p.second.second)
				p1.second.second = vec[4];

		}
		else
		{
			vec[3] = Rect.Get_x() + items[p.first.first].Getitem_max()*p.second.first;
			vec[4] = Rect.Getpos_Y();
			vec[2] = Rect.Getpos_Y() - items[p.first.first].Getitem_min()*p.second.second;

		}
	}
	else //vertical
	{
		//if not ( type 3 and there is a defect)
		//		if (!(Rect.Getsidecut() == 3 && Rect.Get_y() != Rect.Getpos_y()) && ((Rect.Getpos_Y() - Rect.Get_y()) >= items[p.first.first].Getitem_min()*p.second.second))
		//		if ((!(Rect.Getsidecut() == 3 && Rect.Get_y() != Rect.Getpos_y())) || ((Rect.Getpos_Y() - Rect.Get_y()) < items[p.first.first].Getitem_max()*p.second.second))
		if (To_the_Top == false)
		{

			width_x = items[p.first.first].Getitem_min();
			vec[3] = Rect.Get_x() + items[p.first.first].Getitem_min()*p.second.first;
			vec[4] = Rect.Get_y() + items[p.first.first].Getitem_max()*p.second.second;
			if ((Rect.Getpos_Y() - Rect.Get_y()) < items[p.first.first].Getitem_max()*p.second.second)
				p1.second.second = vec[4];

		}
		else
		{
			width_x = items[p.first.first].Getitem_min();
			vec[3] = Rect.Get_x() + items[p.first.first].Getitem_min()*p.second.first;
			vec[4] = Rect.Getpos_Y();
			vec[2] = Rect.Getpos_Y() - items[p.first.first].Getitem_max()*p.second.second;

		}
	}
	if (Rect.Getsidecut() != 1 && (Rect.Getpos_X() - vec[3]) != 0 && (((Rect.Getpos_X() - vec[3]) < waste_min) && (plate_w - vec[3] >= waste_min)))
	{
		int dif = vec[3] - vec[1];
		vec[3] = Rect.Getpos_X();
		vec[1] = Rect.Getpos_X() - dif;
	}
	Sol.Y(vec[4]);
	Sol.X(vec[3]);
	Sol.y(vec[2]);

	//Add the items
	//Add one to the list of items
	for (int k = 0; k < (p.second.first + p.second.second - 1); k++)
	{
		G_Vector_Stacks[items[p.first.first].Getitem_stack()]++;
	}
	if (G_Rotate == false)
	{
		for (int k = 0; k < (p.second.first + p.second.second - 1); k++)
		{

			G_Vector_Items[p.first.first + k] = true;
		}
	}
	else
	{
		for (int k = 0; k < (p.second.first + p.second.second - 1); k++)
		{

			G_Vector_Items[p.first.first - k] = true;
		}
	}
	items[p.first.first].Print();


	int MinDimensionPieza_Temp = MinDimensionPieza;

	bool MoreBase = true;
	int cuantas = get_random(0, 1);

	if (MoreThanOne > cuantas && Rect.Getpos_X() != plate_w)
	{

		MoreBase = false;
		MoreThanOne = 0;
		p1.second.first = vec[3];
		//	Rect.Set_w(Rect.Get_w() - (Rect.Getpos_X() - vec[3]));
		Rect.Add_W(-(Rect.Getpos_X() - vec[3]));


	}
	int cuantas2 = 0;//get_random(0, 1);//cambiado 01102018 por 3
	if (get_random(0, 10) >= 8)
		cuantas2 = 1;


	//More than one

	int stMoreDerecha = 0, stMoreArriba = 0;
	if (Rect.Getsidecut() == 1 && deterministic == false
		//		&& Rect.Getpos_X() != plate_w && get_random(0,3)==3 && MoreBase==true)
		&& MoreBase == true && (cuantas2 >= 1))
		//		&& MoreBase == true && (get_random(0, 3) >= 2 || (batch_items - get_random(2, 4)>G_Rsol_Items.size())))

		//		&& Rect.Getpos_X() != plate_w && get_random(0, 3) >= 3 && MoreBase == true && (batch_items - 2)>G_Rsol_Items.size())

	{

		stMoreDerecha = Rect.Getpos_X() - Rect.Getpos_x();
		MoreThanOne++;
	}
	else
	{
		int val2 = 1;
		if (deterministic == false)
			val2 = get_random(1, 4);
		if (Rect.Getsidecut() == 1)
		{
			if (Rect.Getpos_X() == plate_w && ((plate_w - Sol.X()) < (val2 * MinDimensionPieza)))
			{
				if (deterministic == true)
					MinDimensionPieza_Temp = MinimaDimension(Rect, Sol);
				else
				{
					if (get_random(0, 5) <= 4)
						MinDimensionPieza_Temp = MinimaDimension(Rect, Sol);
					else
					{
						if ((batch_items - 2) > G_Rsol_Items.size())
							stMoreDerecha = plate_w - Rect.Getpos_x();
					}
				}
			}
		}
	}
	int val = 2;
	if (deterministic == false)
		val = get_random(1, 4);
	if (Rect.Getsidecut() == 2)
	{
		if ((plate_h - Sol.Y()) < (val * MinDimensionPieza))
		{
			if (deterministic == true || get_random(0, 4) <= 2)
				MinDimensionPieza_Temp = MinimaDimension(Rect, Sol);
			else
				stMoreArriba = plate_h - Sol.Y();
		}
		else
		{
			MinDimensionPieza_Temp = MinimaDimension(Rect, Sol);

		}
		//		stMoreDerecha = MinDimensionPieza_Temp - MinDimensionPieza;
	}
	stMore = MinDimensionPieza_Temp - MinDimensionPieza;


	//Create rectangles
	// 1: p1 top ; Rect right
	// 2: p1 right ; Rect top
	// 3: p1 top Rect right
	bool last_strip = false;


	//If the right space is greater than minimum piece
	if (Rect.Getsidecut() == 1 && ((((temp[3] - vec[3]) >= (MinDimensionPieza + stMore + stMoreDerecha))) || (Rect.Getpos_X() == (Start_strip + max1Cut) && stMoreDerecha == 0)))
		//	if (Rect.Getsidecut() == 1 && ((((temp[3] - vec[3])>(MinDimensionPieza + stMore + stMoreDerecha))) || (Rect.Getpos_X() == min((Start_strip + max1Cut), plate_w) && stMoreDerecha == 0)))
	{

		//vertical 

		p1.first.first = Rect.Getpos_x();
		p1.first.second = vec[4];
		p1.second.first = vec[3];

		//Change the dimensions of the previous one
		//		Rect.Set_w(max(Rect.Get_w() - (p1.second.first - max(Rect.Getpos_x(),Rect.Get_x())),0));
		if (Rect.Get_w() > (p1.second.first - max(Rect.Get_x(), Rect.Getpos_x())))
			Rect.Add_W(-(p1.second.first - max(Rect.Get_x(), Rect.Getpos_x())));
		else
			Rect.Set_w(0);


		Rect.Setpos_x(p1.second.first);
		Rect.Setpos_y(Rect.Getpos_y());
		Rect.Set_h(Rect.Get_H());

		//		Rect.ModifyDefects();
		//		Rect.Addcut();
		/*		parent = node_id;
		GlassRsol Rsol = GlassRsol(cont_plates - 1, node_id++, temp[1], temp[2], vec[3] - temp[1], plate_h, -2, 1, Rect.GetParent());

		G_Rsol_Items.push_back(Rsol);
		*/
	}
	else
	{
		//If it is a right cut and the left space is small
		if (Rect.Getsidecut() == 2 || ((temp[3] - vec[3]) < (MinDimensionPieza + stMore + stMoreDerecha) && Rect.Getsidecut() <= 2))
		{
			//bottom left corner
			//horizontal
			if (((temp[3] - vec[3]) < (MinDimensionPieza + stMore + stMoreDerecha)) && Rect.Getsidecut() == 1 && (Rect.Getpos_X() == plate_w || MoreThanOne >0))
			{
				last_strip = true;
				Rect.Addcut();
				//			if (plate_w < (Start_strip + max1Cut))
				//				{
				Sol.X(plate_w);
				vec[7] = plate_w;
				/*				}
				else
				{
				Sol.X((Start_strip + max1Cut));
				vec[7] = (Start_strip + max1Cut);

				}
				*/
			}
			p1.first.first = vec[3];
			p1.second.first = max(vec[3], p1.second.first);
			p1.first.second = Rect.Getpos_y();
			p1.second.second = vec[4];
			//Change the dimensions of the previous one at the top of the plate
			if (Rect.Getpos_Y() == plate_h && (Rect.Getpos_Y() - p1.second.second) < (MinDimensionPieza + stMore + stMoreArriba))
			{
				//top
				if (Rect.Getpos_y() != Rect.Get_y())
				{
					//Place top
					if (Overlap(vec[1], plate_h - (vec[4] - vec[2]), vec[3], plate_h, Rect) == false)
					{
						p1.second.second = Rect.Getpos_Y();
						//						Rect.Set_w(p1.first.second - Rect.Getpos_x());
						Rect.Set_w(p1.second.first - Rect.Getpos_x());
						//change position
						Sol.y(plate_h - (Sol.Y() - Sol.y()));
						Sol.Y(plate_h);
						vec[2] = plate_h - (vec[4] - vec[2]);
						vec[4] = plate_h;
					}
					else
					{
						Rect.Set_w(Rect.Getpos_X() - Rect.Getpos_x());
					}
				}
				else
				{
					//place bottom
					p1.second.second = Rect.Getpos_Y();
					Rect.Set_w(p1.second.first - Rect.Getpos_x());
					//					Rect.Set_w(p1.first.second - Rect.Getpos_x());
				}
				/*				GlassRsol Rsol = GlassRsol(cont_plates - 1, node_id++, temp[1], vec[2], vec[3] - temp[1], temp[4] - vec[2], -1, 2, Rect.GetParent());

				G_Rsol_Items.push_back(Rsol);
				*/
			}
			else
			{

				Rect.Set_w(Rect.Getpos_X() - Rect.Getpos_x());
			}
			Rect.Set_h(Rect.Get_h() - (p1.second.second - max(Rect.Get_y(), Rect.Getpos_y())));
			//			Rect.Set_h(Rect.Getpos_Y() - (p1.second.second - max(Rect.Get_y(), Rect.Getpos_y())));

			Rect.Setpos_y(p1.second.second);
			Rect.Setpos_x(Rect.Getpos_x());


			/*			parent = node_id;

			GlassRsol Rsol = GlassRsol(cont_plates - 1, node_id++, temp[1], temp[2], temp[3] - temp[1], temp[4] - temp[2], -2, 2, Rect.GetParent());

			G_Rsol_Items.push_back(Rsol);
			*/
			//		Rect.ModifyDefects();
			//		Rect.Addcut();

		}
		else
		{

			if (Rect.Getsidecut() > 2)
			{
				//vertical


				p1.first.first = vec[3];
				p1.first.second = Rect.Getpos_y();
				p1.second.first = max(vec[3], p1.second.first);
				//Change the dimensions of the previous one
				//Change the dimensions of the previous one
				if (Rect.Get_w() > (p1.second.first - max(Rect.Get_x(), Rect.Getpos_x())))
					Rect.Add_W(-(p1.second.first - max(Rect.Get_x(), Rect.Getpos_x())));
				else
					Rect.Set_w(0);
				//				if (Rect.Get_w() > 6000)
				//				int kk = 9;
				Rect.Setpos_x(p1.second.first);
				Rect.Setpos_y(Rect.Getpos_y());
				Rect.Set_h(Rect.Getpos_Y() - Rect.Getpos_y());

				//		Rect.ModifyDefects();
				//		Rect.Addcut();
				/*				parent = node_id;
				GlassRsol Rsol = GlassRsol(cont_plates - 1, node_id++, temp[1], temp[2], temp[3] - temp[1], temp[4] - temp[2], -2, 3, Rect.GetParent());

				G_Rsol_Items.push_back(Rsol);
				*/
			}
		}
	}

	//para cuando ponemos varias abajo
	vec[7] = vec[3];
	//	if (Rect.Getpos_X())
	Sol.copiesx(Sol.X());



	if (Rect.Getsidecut() == 1 || last_strip == true)
	{
		vec[8] = 0;
		Sol.copiesy(0);
	}
	else
		Sol.copiesy(1);
	Rect.Set_id(Rect.GetRectangle_id() + 1);


	if (silent == false)	printf("Pongo pieza %d %d %d %d %d\n", vec[0], vec[1], vec[2], vec[3], vec[4]);


	//Vertical cut
	/*	if (primer==true ||  G_Positions_Items[G_Positions_Items.size()-1][3]!=vec[3])
	{
	temp[3] = vec[3];
	GlassPlate Plato2 = GlassPlate(G_Plate_Sol.back().Getnode_id(), temp, G_Plate_Sol.back().Getplate_id(), &node_nbr, -2, 1, plate_h);
	G_Plate_Sol.push_back(Plato2);
	}*/
	//vertical cut and horizontal cut
	/*	if (temp[2]!=vec[2])
	{
	temp[3] = vec[3];
	temp[4] = vec[2];
	GlassPlate Plato2 = GlassPlate(G_Plate_Sol.back().Getnode_id(), temp, G_Plate_Sol.back().Getplate_id(), &node_nbr, -1, Rect.Getsidecut(), plate_h);
	G_Plate_Sol.push_back(Plato2);
	}
	*/
	//	GlassPlate Plato = GlassPlate(G_Plate_Sol.back().Getnode_id(), vec, G_Plate_Sol.back().Getplate_id(), &node_nbr,p.first,Rect.Getsidecut(),0);
	//	G_Plate_Sol.push_back(Plato);
	//total area used 


	//We have to put space
	//to the left

	/*	if (temp[2] != vec[2])
	{
	GlassRsol Rsol = GlassRsol(cont_plates - 1, node_id++, temp[1], temp[2], vec[3] - temp[1], vec[2] - temp[2], -1, Rect.Getsidecut()+1, parent);
	G_Rsol_Items.push_back(Rsol);
	}
	if (temp[1] != vec[1])
	{
	//Biggest
	parent = node_id;
	GlassRsol Rsol = GlassRsol(cont_plates - 1, node_id++, temp[1], vec[2], vec[3] - temp[1], vec[4] - vec[2], -2, 2, Rect.GetParent());

	G_Rsol_Items.push_back(Rsol);
	Rsol = GlassRsol(cont_plates - 1, node_id++, temp[1], vec[2], vec[1] - temp[1], vec[4] - vec[2], -1, Rect.Getsidecut()+1, parent);
	G_Rsol_Items.push_back(Rsol);
	}*/
	Total_area_used += items[p.first.first].Getitem_area()*(p.second.first + p.second.second - 1);

	/*	if (Total_area_used == Total_area)
	{
	//one
	GlassRsol Rsol = GlassRsol(cont_plates - 1, node_id++, p1.first.first, p1.first.second, p1.second.first- p1.first.first, p1.second.second - p1.first.second, -1, Rect.Getsidecut() + 1, parent);
	G_Rsol_Items.push_back(Rsol);
	if (Rect.Getsidecut() != 1)
	{
	GlassRsol Rsol = GlassRsol(cont_plates - 1, node_id++, Rect.Getpos_x(), Rect.Getpos_y(), Rect.Getpos_X()-Rect.Getpos_x(), Rect.Getpos_X() - Rect.Getpos_x(), -1, Rect.Getsidecut() + 1, parent);
	G_Rsol_Items.push_back(Rsol);
	}
	}*/
	if (vec[3] > Total_width_bin)
		Total_width_bin = vec[3];

	//	if (Sol.X() > Total_width_bin)
	//	Total_width_bin = Sol.X();
	vec[6] = Rect.Getsidecut();
	if (vec[6] > 3)
		Rect.Setcut(Rect.Getsidecut() - 1);
	Sol.Setcut(Rect.Getsidecut());
	std::vector <int > vec2(9);
	GlassRsol Sol2 = GlassRsol(Sol);
	vec2 = vec;

	for (int k = 1; k <= (p.second.first + p.second.second - 1); k++)
	{

		//	GlassRsol Rsol = GlassRsol(cont_plates - 1, node_id++, vec2[1], vec2[2], temp[3] - vec2[1], vec2[4] - vec2[2], -2, Rect.Getsidecut() + 1, parent);
		//	G_Rsol_Items.push_back(Rsol);
		vec2[3] = vec[1] + width_x * k;

		Sol2.X(Sol.x() + width_x * k);
		if (k != 1)
		{
			vec2[7] = vec2[3];
			Sol2.copiesx(Sol2.X());
		}
		//Last item of the bottom
		if (vec[8] == 0 && k>1 && k == (p.second.first + p.second.second - 1))
			vec2[8] = 2;
		if (Sol.copiesy() == 0 && k>1 && k == (p.second.first + p.second.second - 1))
			Sol2.copiesy(2);
		//		G_Positions_Items.push_back(vec2);
		if (Sol2.iditem() == 172 && Sol2.x() == 4428 && Sol2.y() == 0 && Sol2.plateId() == 1)
			int kk = 9;
		G_Rsol_Items.push_back(Sol2);
		//		GlassRsol Rsol = GlassRsol(cont_plates-1, node_id++,vec2[1], vec2[2], vec2[3]-vec2[1],vec2[4]-vec2[2],vec2[0], Rect.Getsidecut()+1, parent);
		//	G_Rsol_Items.push_back(Rsol);
		if (G_Rotate == true)
			Sol2.iditem(Sol2.iditem() - 1);
		else
			Sol2.iditem(Sol2.iditem() + 1);

		Sol2.x(Sol2.X());
		vec2[0] = vec2[0] + 1;
		vec2[1] = vec2[3];
		//		vec2[7]=vec2[7]-1;
		
		if (Sol.Getcut() != 3)
			Sol2.Setcut(Sol.Getcut() + 1);
		
		if (vec[6] != 3)
			vec2[6] = vec[6] + 1;

		vec2[8] = 1;

		Sol2.copiesy(1);
	}

	Rect.Print();
	//The other two rectangles

	/*	GlassRsol Rsol = GlassRsol(cont_plates-1, node_id++, Rect.Getpos_x(), Rect.Getpos_y(), Rect.Getpos_X()- Rect.Getpos_x(), temp[4] - Rect.Getpos_y(), -2, Rect.Getsidecut(), parent);
	G_Rsol_Items.push_back(Rsol);
	GlassRsol Rsol2 = GlassRsol(cont_plates-1, node_id++, p1.first.first, p1.first.second, p1.second.first-p1.first.first, p1.second.second- p1.first.second, -2, Rect.Getsidecut()+1, parent);
	G_Rsol_Items.push_back(Rsol2);*/

	return p1;

}
//Add the item to a list of place items
//Add one in the list of vector items
//Create two new rectangles one to the right and other to the top
Coordinates Glass::PlaceItem(std::pair<int, int> p, GlassRectangle &Rect)
{


	bool primer = false;
	if (Rect.Getpos_x() == 0 && Rect.Getpos_X() == plate_w)
		primer = true;
	std::pair <int, int > xy(0, 0);
	std::pair <int, int > wh(0, 0);
	Coordinates p1(xy, wh);
	p1.second.first = Rect.Getpos_X();
	p1.second.second = Rect.Getpos_Y();
	//
	Rect.Print();

	std::vector <int > vec(9);
	vec[0] = p.first;
	vec[1] = Rect.Get_x();
	vec[2] = Rect.Get_y();
	vec[5] = cont_plates;

	//For doing the plates
	std::vector <int > temp(6);
	temp[1] = Rect.Getpos_x();
	temp[2] = Rect.Getpos_y();
	temp[3] = Rect.Getpos_X();
	temp[4] = Rect.Getpos_Y();
	vec[5] = cont_plates;
	vec[6] = Rect.Getsidecut();
	vec[7] = 1; //copies x
	vec[8] = 1; // copies y

				//	PrintProblem("Pone mas cortes");
				//We have to place the item in the position
				//left bottom corner

				//Place the item
	if (p.second == true)
	{
		vec[3] = Rect.Get_x() + items[p.first].Getitem_max();
		vec[4] = Rect.Get_y() + items[p.first].Getitem_min();
	}
	else
	{
		vec[3] = Rect.Get_x() + items[p.first].Getitem_min();
		vec[4] = Rect.Get_y() + items[p.first].Getitem_max();
	}

	//Create rectangles
	// 1: p1 top ; Rect right
	// 2: p1 right ; Rect top
	// 3: p1 top Rect right

	if (Rect.Getsidecut() == 1 && (temp[3] - vec[3])>MinDimensionPieza)
	{

		//vertical 

		p1.first.first = Rect.Getpos_x();
		p1.first.second = vec[4];
		p1.second.first = vec[3];
		//Change the dimensions of the previous one
		Rect.Add_W(-(p1.second.first - max(Rect.Getpos_x(), Rect.Get_x())));

		Rect.Setpos_x(p1.second.first);
		Rect.Setpos_y(Rect.Getpos_y());
		Rect.Set_h(Rect.Getpos_Y() - Rect.Getpos_y());
		//		Rect.ModifyDefects();
		//		Rect.Addcut();

	}
	else
	{
		if (Rect.Getsidecut() == 2 || ((temp[3] - vec[3]) <= MinDimensionPieza && Rect.Getsidecut() <= 2))
		{
			//bottom left corner
			//horizontal
			if ((temp[3] - vec[3])<MinDimensionPieza && Rect.Getsidecut() == 1)
				Rect.Addcut();

			p1.first.first = vec[3];
			p1.first.second = Rect.Getpos_y();
			p1.second.second = vec[4];
			//Change the dimensions of the previous one
			//Change the dimensions of the previous one
			if (Rect.Getpos_Y() == plate_h && (Rect.Getpos_Y() - p1.second.second) <= MinDimensionPieza)
			{
				p1.second.second = Rect.Getpos_Y();
				Rect.Set_w(p1.first.second - Rect.Getpos_x());
			}
			else
			{

				Rect.Set_w(Rect.Getpos_X() - Rect.Getpos_x());
			}
			Rect.Add_H(-(p1.second.second - max(Rect.Get_y(), Rect.Getpos_y())));
			Rect.Setpos_y(p1.second.second);
			Rect.Setpos_x(Rect.Getpos_x());
			//		Rect.ModifyDefects();
			//		Rect.Addcut();

		}
		else
		{

			if (Rect.Getsidecut() > 2)
			{
				//vertical


				p1.first.first = vec[3];
				p1.first.second = Rect.Getpos_y();

				//Change the dimensions of the previous one
				//Change the dimensions of the previous one
				Rect.Add_W(-(p1.second.first - max(Rect.Get_x(), Rect.Getpos_x())));

				Rect.Setpos_x(p1.second.first);
				Rect.Setpos_y(Rect.Getpos_y());
				Rect.Set_h(Rect.Getpos_Y() - Rect.Getpos_y());
				//		Rect.ModifyDefects();
				//		Rect.Addcut();


			}
		}
	}
	Rect.Set_id(Rect.GetRectangle_id() + 1);

	//Add one to the list of items
	G_Vector_Stacks[items[p.first].Getitem_stack()]++;
	if (silent == false)	printf("Pongo pieza %d %d %d %d %d\n", vec[0], vec[1], vec[2], vec[3], vec[4]);


	//Vertical cut
	//total area used 
	Total_area_used += items[p.first].Getitem_area();
	if (vec[3] > Total_width_bin)
		Total_width_bin = vec[3];
	G_Positions_Items.push_back(vec);
	G_Vector_Items[p.first] = true;
	items[p.first].Print();
	Rect.Print();

	return p1;

}
//There is something in this strip
int  Glass::ThereIsSomethingInThisStrip(std::list< GlassRsol > ::iterator  &it, int ini, int fin, int fin_strip)
{
	std::list< GlassRsol > ::iterator  it2 = it;
	int minimo = (*it).x();
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) return minimo;
		if ((*it2).x() < fin)
		{
			if ((*it2).x() < minimo)
				minimo = (*it2).x();
		}
		if ((*it2).x() >= fin_strip) return minimo;
		if ((*it2).plateId() < (*it).plateId()) return minimo;

	}
	return minimo;
}


void Glass::PushWastes(int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int &pa, int &nod, int &pa_cut)
{
	if (py != y)
	{
		Glass_Corte  corte_temp(pla, nod++, x, y, pX - x, py - y, -1, pa_cut + 1, pa);
		G_Solution_Rotated.push_front(corte_temp);


	}
	if (px != x)
	{
		//Biggest

		Glass_Corte  corte_temp(pla, nod++, x, py, px - x, pY - py, -1, pa_cut + 1, pa);
		G_Solution_Rotated.push_front(corte_temp);

	}
}
void Glass::PushWastesBottom(int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int pa, int &nod, int pa_cut)
{
	if (py > y)
	{
		Glass_Corte  corte_temp(pla, nod++, x, y, pX - x, py - y, -1, pa_cut + 1, pa);
		G_Solution_Rotated.push_front(corte_temp);


	}

}
void Glass::PushWastesLeft(int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int pa, int &nod, int pa_cut)
{

	if (px != x)
	{
		//Biggest

		Glass_Corte  corte_temp(pla, nod++, x, py, px - x, pY - py, -1, pa_cut + 1, pa);
		G_Solution_Rotated.push_front(corte_temp);

	}
}
void Glass::WriteWastes(FILE *files, int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int &pa, int &nod, int &pa_cut)
{
	if (py != y)
	{
		fprintf(files, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", pla, nod++, x, y, pX - x, py - y, -1, pa_cut + 1, pa);


	}
	if (px != x)
	{
		//Biggest

		fprintf(files, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", pla, nod++, x, py, px - x, pY - py, -1, pa_cut + 1, pa);

	}
}
void Glass::WriteWastesBottom(FILE *files, int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int pa, int &nod, int pa_cut)
{
	if (py > y)
	{
		fprintf(files, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", pla, nod++, x, y, pX - x, py - y, -1, pa_cut + 1, pa);


	}

}
void Glass::WriteWastesLeft(FILE *files, int x, int y, int X, int Y, int px, int py, int pX, int pY, int pla, int pa, int &nod, int pa_cut)
{

	if (px != x)
	{
		//Biggest
		if ((pa_cut + 1) == 1) pY = plate_h;
		fprintf(files, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", pla, nod++, x, py, px - x, pY - py, -1, pa_cut + 1, pa);

	}
}
//Look up into the list an item down of this
bool Glass::NothingDown(std::vector< std::vector <int> > ::iterator  &it)
{
	if (it == G_Best_Positions_Items.begin()) return true;
	bool nada = true;
	std::vector< std::vector <int> > ::iterator  it2;
	for (it2 = it - 1; ; it2--)
	{

		//if starts above it
		if (((*it2)[5] != (*it)[5]) || ((*it2)[4] > (*it)[2]) || ((*it2)[1] >= (*it)[3]) || ((*it2)[3] <= (*it)[1]))
		{
			if (it2 == G_Best_Positions_Items.begin())
				return nada;
			continue;
		}

		return false;
	}
	return true;
}
void  Glass::ChangeUnder(GlassRectangle & rectangleit, int val)
{

	//we can have both equal
	int val2 = 0;
	bool one = false;
	std::list< GlassRsol > ::reverse_iterator  it;
	for (it = G_Rsol_Items.rbegin(); it != G_Rsol_Items.rend() && (*it).plateId() == cont_plates; it++)
	{
		if ((*it).x() < Start_strip) return;
		//Is the first one 
		if ((*it).copiesy() == 0)
		{
			if (one == false)
			{
				val2 = (*it).copiesx();
				(*it).copiesx((*it).copiesx() + val);
				if ((*it).copiesx() > Total_width_bin)
					Total_width_bin = (*it).copiesx();

				one = true;

			}
			else
			{
				if ((*it).copiesx() != val2)
					return;
				(*it).copiesx((*it).copiesx() + val);
				if ((*it).copiesx() > Total_width_bin)
					Total_width_bin = (*it).copiesx();

			}
		}
	}
	if (one == true)
		return;
	PrintProblem("No ha encontrado ninguno debajo");
	return;
}
void  Glass::ChangeWidthRectangles()
{



	bool fin = false;

	//set the width
	for (std::list< GlassRsol > ::reverse_iterator it = G_Rsol_Items.rbegin(); it != G_Rsol_Items.rend() && (*it).plateId() >= cont_plates && (*it).x() >= Start_strip && fin != true; it++)
	{


		if ((*it).plateId() == cont_plates && (*it).X() != Total_width_bin && (*it).X() > (Total_width_bin - waste_min))
		{


			Total_width_bin = Total_width_bin + waste_min;
			if ((plate_w - Total_width_bin) < waste_min)
				Total_width_bin = plate_w;

			fin = true;
		}
	}

	bool first_base = false;
//	for (std::list< GlassRsol > ::reverse_iterator it = G_Rsol_Items.rbegin(); it != G_Rsol_Items.rend() && (*it).plateId() >= cont_plates && (*it).x() >= Start_strip && first_base != true; it++) //change 060319

	for (std::list< GlassRsol > ::iterator it = G_Rsol_Items.begin(); it != G_Rsol_Items.end()  && first_base != true; it++)
	{
		if ((*it).plateId() < cont_plates)
			continue;
		if ((*it).x() < Start_strip)
			continue;
		if ((*it).copiesx() >= Total_width_bin)
		{
			if ((*it).copiesx() == plate_w || (*it).copiesx() == (Start_strip + max1Cut))
			{
				if ((NextRightCurrent(it, plate_w) > 0) && (*it).Getcut() == 2)
				{

					(*it).RsolType(2);


					PutRSolTypeCurrent(it, max(Start_strip + max1Cut, plate_w), plate_w);
				}
				else
				{
					//					last_base = true;
					(*it).Setcut(1);

				}
			}

			//		if ((((*it).copiesx() != plate_w && (*it).copiesx() != (Start_strip + max1Cut)) && ((*it).copiesx() - 20) > Total_width_bin )
			//		|| ((*it).copiesx()>(Start_strip + max1Cut)) || (MinDimensionPieza == MAXIMUM_INT_ROADEF && ((*it).copiesx() - 20) > Total_width_bin))
			first_base = true;
			if ((*it).copiesx()  > Total_width_bin)
				(*it).copiesx(Total_width_bin);
			//			else
			//			Total_width_bin = Total_width_bin + 20;
		}
	}

	return;
}
void Glass::ChangeRectanglesSimmetry(int maxWidth)
{
	bool first_base = false;

	for (std::list< GlassRsol > ::iterator it = G_Rsol_Items.begin(); it != G_Rsol_Items.end() && (*it).plateId() <= 0 && first_base != true; it++)
	{

		if ((*it).copiesx() >= maxWidth)
		{
			if ((*it).copiesx() == plate_w)
			{
				if ((NextRightCurrent(it, plate_w) > 0) && (*it).Getcut() == 2)
				{

					(*it).RsolType(2);


					PutRSolTypeCurrent(it, plate_w, plate_w);
				}
				else
				{
					//					last_base = true;
					(*it).Setcut(1);

				}
			}
			first_base = true;
			if ((*it).copiesx()  > maxWidth)
				(*it).copiesx(maxWidth);
			//			else
			//			Total_width_bin = Total_width_bin + 20;
		}
	}
}
void  Glass::ChangeBase(int width)
{


	std::list< GlassRsol > ::reverse_iterator  it;
	bool change = false;
	int max = 0;
	for (it = G_Rsol_Items.rbegin(); it != G_Rsol_Items.rend(); it++)
	{
		if ((*it).plateId() != (cont_plates))
			continue;
		//Is the first one 
		if ((*it).copiesx() > width)
		{
			if ((*it).X() > max)
				max = (*it).X();
		}
	}
	if (!((width - max) == 0 || (width - max) > waste_min))
		width += waste_min;

	bool first = false;
	bool second = false;
	for (it = G_Rsol_Items.rbegin(); it != G_Rsol_Items.rend(); it++)
	{
		if ((*it).plateId() != (cont_plates) || (*it).x()<Start_strip)
			return;
		//Is the first one 
		if ((*it).copiesx() >= width)
		{
			(*it).copiesx(width);


			(*it).copiesy(0);
			if (first == true)
			{

				second = true;
				(*it).RsolType(2);
			}
			if (first == false && NextPut(it, width) == true)
			{

				first = true;
				(*it).RsolType(1);
			}


			if ((*it).copiesx() == (*it).X() && (*it).RsolType() == 0)
				(*it).Setcut(1);

			change = true;
		}
		else
		{
			if (first == true && second == false)
				(*it).RsolType(1);
		}
	}
	if (change == true)
		return;
	PrintProblem("No ha encontrado ninguno debajo");
	return;
}

bool Glass::AsignarItemsPlate(GlassRectangle &rectangle)
{
	LRectangles list_rectangles;
	list_rectangles.push_back(rectangle);
	//We need a list of rectangles that we have to fill
	bool placed = false;

	//While there is rectangle to pack
	do
	{
		rectangle = (*(list_rectangles.begin()));
		bool posible = false;
		int inicio = 0;

		do
		{

			posible = false;
			if (rectangle.Getmin() < MinDimensionPieza)
			{
				if (inicio == 1)
					continue;
			}
			int cambiado = 0;
		PLACE:
			if (cambiado == 8 || rectangle.Getmin() < MinDimensionPieza)
			{
				if (inicio != 0)
					continue;


			}
			std::pair<int, int > p(-1, 0);
			if (deterministic == false)
				p = ItemGreaterRNRandom(rectangle);
			else
				p = ItemGreaterRN(rectangle);

			if (p.first == (-1)) //It means there is no piece in this hole
			{
				//Improve this case TODO LIST
				// If there is a defect you can change the dimensions

				//We are going to put to the top or to the right of the first
				bool change = false;

				do
				{
					change = ChangePositionToPlace(rectangle);
				} while (rectangle.Getmin() < MinDimensionPieza && (change == true));
				if (change == true)
				{

					cambiado++;
					if (inicio == 0 && cambiado == 8 && (*(list_rectangles.begin())).Getpos_x() == rectangle.Getpos_x() && (*(list_rectangles.begin())).Getpos_y() == rectangle.Getpos_y())
					{
						list_rectangles.pop_front();
						if (list_rectangles.size() == 0)
							return false;
						//						list_rectangles.push_front(rectangle);
					}
					goto PLACE;
					//Antiguo 26/04/2018
					/*					//Change the dimensions of the rectangle
					rectangle.ChangeDimensionsNextDefect();
					list_rectangles.pop_front();
					list_rectangles.push_front(rectangle);
					p = rectangle.ItemGreaterR();
					if (p.first != (-1))
					posible = true;
					*/
				}
				else
				{
					//If it comes from bottom
					if (inicio == 1)
						//You should close this rectangle.
						rectangle.Setcut(4);
					else
						list_rectangles.pop_front();
				}


			}
			else
				posible = true;
			if (posible == true)
			{

				//Place this item in the plate
				//Then can appear other rectangles
				int rectan_id = rectangle.GetRectangle_id();

				Coordinates xy = PlaceItem(p, rectangle);

				placed = true;
				UpdateMinDimension(p);
				GlassRectangle rectangle_new = GlassRectangle(1, xy, rectangle, this);

				if (rectangle.Getmin()>0)
					rectangle.ModifyDefects();
				//We have to add the previous rectangle modified
				EraseRectangleFromList(list_rectangles, rectan_id);
				if (rectangle.Getsidecut() != 4 && min(rectangle.Get_h(), rectangle.Getmin())>MinDimensionPieza)
					list_rectangles.push_front(rectangle);
				rectangle = rectangle_new;
				inicio = 1;
			}
			if (MinDimensionPieza == MAXIMUM_INT_ROADEF)
				return false;

		} while (posible == true);



	} while (list_rectangles.size() != 0);
	return placed;
}
void Glass::ChangeRectangles(int Right, int Up, GlassRectangle &Rect, std::list< GlassRectangle> &lista, std::list< GlassDefect > ldefects)
{
	Rect.Print();
	Rect.EverythingOK(Start_strip + max1Cut);
	if (Right > 0)
	{
		//Change Rectangles 
		for (std::list <GlassRectangle> ::iterator it = lista.begin(); it != lista.end(); it++)
		{
			if ((*it).Getpos_X() == Rect.Getpos_X())
			{
				(*it).Add_W(Right);

				(*it).Getdefectlist().clear();
				if (G_Rotate == false)
					(*it).InsertDefects(ldefects);
				else
					(*it).InsertDefectsRotate(ldefects);
			}
			if ((*it).Getpos_x() == Rect.Getpos_X())
			{
				if ((*it).Get_w() > Right)
					(*it).Add_W(-Right);
				else
					(*it).Set_w(0);

				(*it).Setpos_x((*it).Getpos_x() + Right);
				(*it).Getdefectlist().clear();
				if (G_Rotate == false)
					(*it).InsertDefects(ldefects);
				else
					(*it).InsertDefectsRotate(ldefects);
			}
		}
		Rect.EverythingOK(Start_strip + max1Cut);
		Rect.Add_W(Right);
		Rect.EverythingOK(Start_strip + max1Cut);

		//11/01/2018 CAMBIADO DESDE
		//		if (Rect.Getpos_X()>Total_width_bin)
		//		Total_width_bin = Rect.Getpos_X();
		//HASTA 

	}
	if (Up > 0)
	{
		//Change Rectangles 
		for (std::list <GlassRectangle> ::iterator it = lista.begin(); it != lista.end(); it++)
		{
			if ((*it).Getpos_Y() == Rect.Getpos_Y())
			{
				(*it).Add_H(Up);
				(*it).Getdefectlist().clear();
				if (G_Rotate == false)
					(*it).InsertDefects(ldefects);
				else
					(*it).InsertDefectsRotate(ldefects);
			}
			if ((*it).Getpos_y() == Rect.Getpos_Y())
			{
				if ((*it).Get_h() > Up)
					(*it).Add_H(-Up);
				else
					(*it).Set_h(0);
				(*it).Setpos_y((*it).Getpos_y() + Up);
				(*it).Getdefectlist().clear();
				if (G_Rotate == false)
					(*it).InsertDefects(ldefects);
				else
					(*it).InsertDefectsRotate(ldefects);
			}
		}
		Rect.Add_H(Up);
		Rect.EverythingOK(Start_strip + max1Cut);

	}

	Rect.Getdefectlist().clear();
	if (G_Rotate == false)
		Rect.InsertDefects(ldefects);
	else
		Rect.InsertDefectsRotate(ldefects);
}
bool Glass::Overlap(std::list< GlassRectangle> &lista, int x, int y, int X, int Y)
{

	/*	for (std::list <GlassRectangle> ::iterator it = lista.begin(); it != lista.end(); it++)
	{
	for (std::list < GlassDefect > ::iterator itD = (*it).Getdefectlist().begin(); itD != (*it).Getdefectlist().end() ; itD++)
	{
	if (Overlap(x, y, X, Y, (*itD).Getpos_x(), (*itD).Getpos_y(), (*itD).Getpos_x() + (*itD).Getwidth(), (*itD).Getpos_y() + (*itD).Getheight()))
	return true;
	}
	}*/
	for (std::list < GlassDefect > ::iterator itD = DefectsPlate[Plates - 1].begin(); itD != DefectsPlate[Plates - 1].end(); itD++)
	{
		if (Overlap(x, y, X, Y, (*itD).Getpos_x(), (*itD).Getpos_y(), (*itD).Getpos_x() + (*itD).Getwidth(), (*itD).Getpos_y() + (*itD).Getheight()))
			return true;
	}
	return false;

}
std::pair< int, int > Glass::ComputeIfBetterToDoChange(GlassRectangle &Rect, Coordinates &pitem)
{
	std::pair< int, int > par(0, 0);
	//If do not place anything lost area = rectangl
	int Dimw = items[pitem.first.second].Getitem_w(pitem.second.first);
	int Dimh = items[pitem.first.second].Getitem_h(pitem.second.first);
	bool right = true;
	if (Dimw*pitem.second.second > Rect.Get_w())
		right = true;
	else
		right = false;
	int  SaleDerecha = 0, SaleArriba = 0, LostArea_Changed = 0;
	if (right == true)
	{
		SaleDerecha = Rect.Get_x() + Dimw * pitem.second.second - Rect.Getpos_X();
		bool few = false;
		if (SaleDerecha < waste_min)
		{
			few = true;
			SaleDerecha += waste_min;


			//Smaller than 20)
			if (((Rect.Getpos_X() + SaleDerecha)>(plate_w - waste_min))
				&& ((Rect.Getpos_X() + SaleDerecha)<(plate_w)))
				SaleDerecha = plate_w - Rect.Getpos_X();
		}
		par.first = SaleDerecha;
		if ((SaleDerecha + Rect.Getpos_X()) > min(plate_w, Start_strip + max1Cut))
			par.first = -1;
		LostArea_Changed = plate_h * (SaleDerecha)+
			Rect.Getarea() - (items[pitem.first.second].Getitem_area()*pitem.second.second);

		if (wider == true || get_random(0, 3) == 0 || deterministic == true)
			LostArea_Changed = Rect.Get_y()*(SaleDerecha);
		if (deterministic != true && few == true && (Rect.Get_x() + Dimw * pitem.second.second + waste_min) > min(Start_strip + max1Cut, plate_w))
			LostArea_Changed = 2 * Rect.Getarea();

	}
	else
	{
		SaleArriba = Rect.Get_y() + Dimh - Rect.Getpos_Y();
		//TODO find the value
		bool few = false;
		if (SaleArriba < waste_min)
		{
			few = true;
			SaleArriba += waste_min;
			//If all in the same cut starts at the same height
			if (Rect.Getpos_y() != Rect.Get_y())
				SaleArriba = plate_h;
			else
			{
				if (AllStartsAtTheSameHeightInTheStrip(Rect) == false)
					SaleArriba = plate_h;
				else
				{
					if (((Rect.Getpos_Y() + SaleArriba)>(plate_h - waste_min))
						&& ((Rect.Getpos_Y() + SaleArriba)<(plate_h)))
						SaleArriba = plate_h - Rect.Getpos_Y();
				}
			}

		}
		par.second = SaleArriba;
		if ((SaleArriba + Rect.Getpos_Y()) > plate_h)
			par.first = -1;

		LostArea_Changed = Rect.Getpos_X()*(SaleArriba)+
			Rect.Getarea() - (items[pitem.first.second].Getitem_area()*pitem.second.second);

		if (higher == true || get_random(0, 3) == 0 || deterministic == true)
			LostArea_Changed = Rect.Get_x()*(SaleArriba);
		if (deterministic != true && few == true && (Rect.Get_y() + Dimh + waste_min) >  plate_h)
			LostArea_Changed = 2 * Rect.Getarea();

	}

	double factor_corrected = 1 + (double)((double)get_random(-8, 8) / (double)10);
	if (deterministic == true)
		factor_corrected = 1;
	if (Rect.Getarea() < (double)factor_corrected*LostArea_Changed)
		par.first = -1;
	return par;

}
//					if (LostArea_Changed < LostArea_Original || (Total_area_used+2* items[p.first.second].Getitem_area()>=Total_area))

void Glass::Randomize(std::vector< int > &lista)
{

	for (register int i = 0; i < lista.size(); i++)
	{
		int change = get_random(0, lista.size() - 1);
		int temp = lista[i];
		lista[i] = lista[change];
		lista[change] = temp;
	}
}

int Glass::AsignarItemsPlate_J(GlassRectangle &Rectan)
{

	LRectangles list_Rectans;
	std::list< GlassDefect > LDefects = Rectan.Getdefectlist();
	list_Rectans.push_back(Rectan);
	Start_strip = Rectan.Get_x();
	Rectan.EverythingOK(Start_strip + max1Cut);
	//We need a list of Rectans that we have to fill
	bool placed = false;
	bool start_strip = true;
	MoreThanOne = 0;
	int Width_start = Total_width_bin;
	//While there is Rectan to pack
	do
	{
		//	printf("Check AIP1 %d\n", MinDimensionPieza);
		if (MoreThanOne > 0)
		{
			ChangeRectangleFromList(list_Rectans, Total_width_bin);
			ChangeBase(Total_width_bin);
			MoreThanOne = 0;
		}
		Rectan = (*(list_Rectans.begin()));
		Rectan.EverythingOK(Start_strip + max1Cut);
		bool posible = false;
		int inicio = 0;
		int cambiado = 0;
		if ((Rectan.Get_y() == 0 || Rectan.Getpos_y() == 0) && start_strip != true)
		{

			Total_width_bin = Rectan.Get_x();
			ChangeWidthRectangles();

			if ((Total_area > Total_area_used) &&
				(Total_width_bin == Width_start ||
				((plate_w - Total_width_bin) != MinDimensionPieza && (plate_w - Total_width_bin) < (MinDimensionPieza + waste_min))))
			{
				return 0;
			}
			else
			{
				if (Rectan.Get_x() < Total_width_bin)
					int kk = 9;
				return Rectan.Get_x();
			}
				
		}
		start_strip = false;
		do
		{

			//		printf("Check AIP2 %d\n", MinDimensionPieza);
			posible = false;
			//			if (Rectan.Getmin() < MinDimensionPieza || Rectan.Getmax()< MinDimensionPieza)
			if (Rectan.Getmax() < MinDimensionPieza || Rectan.Getmin() == 0)
			{
				if (inicio == 1)
					continue;
			}
			cambiado = 0;
		PLACE:
			if (cambiado == 8 || (Rectan.Getmin() < MinDimensionPieza && Rectan.Getmax() < MinDimensionPieza))
			{
				if (inicio != 0)
					continue;


			}
			std::pair<int, int > p2(-1, -1);
			Coordinates p(p2, p2);
			Para_Random = get_random(1, 4);

			wider = false;
			higher = false;
			Rectan.EverythingOK(Start_strip + max1Cut);
			//			printf("Check AIP3 %d\n", MinDimensionPieza);
			if (get_random(0, 10) >= 8) one_item_each = true;
			else
				one_item_each = false;
			if ((Rectan.Get_x() != Rectan.Getpos_x()) || (Rectan.Get_y() != Rectan.Getpos_y()))
				one_item_each = true;

			if (deterministic == false && Rectan.Getnumdefects()>0 && get_random(0, 5) <= 2)
			{
				//				if (((Rectan.Getdefectminw() - Rectan.Get_x()) ^ 2 + (Rectan.Getdefectminh() - Rectan.Get_y()) ^ 2)<(400 ^ 2 + (get_random(1, 10) * 50) ^ 2))
				//					Rectan.ChangePositionToPlaceNearest(2);
				//				int val1 = pow(Rectan.Getdefectminw() - Rectan.Get_x(), 2) + pow(Rectan.Getdefectminh() - Rectan.Get_y(), 2);
				//			int val2 = pow(400, 2) + pow(get_random(1, 400), 2);
				if (min(Rectan.Getdefectminw() - Rectan.Get_x(), Rectan.Getdefectminh() - Rectan.Get_y())<get_random(0, 200))

					//				if (val1 <val2)
				{
					deterministic = true;
					Rectan.ChangePositionToPlaceNearest(2);
					deterministic = false;
				}
			}
			//		printf("Check AIP4 %d\n", MinDimensionPieza);
			//			if (G_iter_veces == 18231)
			//			int kk = 9;
			Randomize(G_Random_Stacks);

			//				random_shuffle(G_Random_Stacks.begin(), G_Random_Stacks.end(),randomfunc_1);
			p = ItemGreaterRNRandom_J(Rectan);
			//another oner
			//			printf("Check AIP5 %d\n", MinDimensionPieza);
			/*			}
			else
			p = ItemGreaterRN_J(Rectan);*/
			G_iter_veces++;

			Rectan.EverythingOK(Start_strip + max1Cut);


			if (p.first.first == (-1)) //It means there is no piece in this hole
			{

				//It means there is a piece overlapping the Rectan
				if (p.first.second != (-1))
				{

					Rectan.EverythingOK(Start_strip + max1Cut);

					//Change the first parameter is to the right, the second one to the top

					std::pair< int, int > change = ComputeIfBetterToDoChange(Rectan, p);

					//					if (LostArea_Changed < LostArea_Original || (Total_area_used+2* items[p.first.second].Getitem_area()>=Total_area))


					if (change.first >= 0)
					{

						//change Rectans 
						bool overlap_other = false;
						//If the change is to the top
						//It could be an infeasible solution
						if (change.second > 0)
						{
							bool feasible = CheckChangesToTheTop(change.second, Rectan.Getpos_y());
							if (feasible == true)
							{
								MakeFeasibleToTheTop(change.second, Rectan.Getpos_y());
							}
							else
								overlap_other = true;
						}



						if (overlap_other == false)
						{
							Rectan.EverythingOK(Start_strip + max1Cut);
							ChangeRectangles(change.first, change.second, Rectan, list_Rectans, DefectsPlate[NumberOfPlate]);
							Rectan.EverythingOK(Start_strip + max1Cut);
							p.first.first = p.first.second;
							p.first.second = p.second.first;
							p.second.first = p.second.second;
							p.second.second = 1;

							//We have to change the solution of the item above
							if (change.first > 0)
								ChangeUnder(Rectan, change.first);

							posible = true;
							Rectan.EverythingOK(Start_strip + max1Cut);
							goto PLACE2;
						}

					}
					Rectan.EverythingOK(Start_strip + max1Cut);
				}
				//Improve this case TODO LIST
				// If there is a defect you can change the dimensions
				//We are going to put to the top or to the right of the first
				//				printf("Check AIP6 %d\n", MinDimensionPieza);
				bool change = false;
				do
				{
					Rectan.EverythingOK(Start_strip + max1Cut);
					GlassRectangle kkrec = Rectan;
					change = Rectan.ChangePositionToPlaceNearest(2);
					Rectan.EverythingOK(Start_strip + max1Cut);
					//					printf("Check AIP6_1 change %d x: %d y: %d  X: %d Y: %d Min %d MinPieza %d\n",change,Rectan.Getpos_x(),Rectan.Getpos_y(), Rectan.Getpos_X(), Rectan.Getpos_Y(), Rectan.Getmin(), MinDimensionPieza);
				} while (Rectan.Getmin() < MinDimensionPieza && (change == true));
				//			printf("Check AIP6_2 %d\n", MinDimensionPieza);
				Rectan.EverythingOK(Start_strip + max1Cut);
				if (change == true)
				{

					cambiado++;
					if (inicio == 0 && cambiado == 8 && (*(list_Rectans.begin())).Getpos_x() == Rectan.Getpos_x() && (*(list_Rectans.begin())).Getpos_y() == Rectan.Getpos_y())
					{
						list_Rectans.pop_front();
						if (list_Rectans.size() == 0)
						{
							ChangeWidthRectangles();
							return 0;
						}


					}
					goto PLACE;
				}

				else
				{
					//If it comes from bottom
					if (inicio == 1)
						//You should close this Rectan.
						Rectan.Setcut(4);
					else
						list_Rectans.pop_front();
				}


			}
			else
				posible = true;
			//			printf("Check AIP7 %d\n", MinDimensionPieza);
			if (posible == true)
			{
			PLACE2:

				//Place this item in the plate
				//Then can appear other Rectans

				Rectan.EverythingOK(Start_strip + max1Cut);
				int rectan_id = Rectan.GetRectangle_id();

				int MoreOne = MoreThanOne;

				Coordinates xy = PlaceItem(p, Rectan);

				Rectan.EverythingOK(Start_strip + max1Cut);
				int MoreOne2 = MoreThanOne;
				if ((MoreOne - MoreOne2) > 0)
				{

					ChangeRectangleFromList(list_Rectans, Total_width_bin);
					ChangeBase(Total_width_bin);
				}

				placed = true;
				UpdateMinDimension(p.first);
				GlassRectangle Rectan_new = GlassRectangle(1, xy, Rectan, this);
				if (active_log_error && G_Rotate == false)
				{

					if (G_Rsol_Items.back().X() > (plate_w - waste_min) && G_Rsol_Items.back().X() != (plate_w))
					{

						printf("Iter %d", G_iter_veces);
						PrintProblem("Alguna pieza fuera X menos 20");
					}
				}
				if (active_log_error)
				{
					if (G_Rsol_Items.back().Y() > (plate_h - waste_min) && G_Rsol_Items.back().Y() != (plate_h))
					{

						printf("Iter %d", G_iter_veces);
						PrintProblem("Alguna pieza fuera X menos 20");
					}
				}				if (Rectan.Getmin()>0)
					Rectan.ModifyDefects();
				//We have to add the previous Rectan modified
				EraseRectangleFromList(list_Rectans, rectan_id);
				//Change 05062018

				Rectan.EverythingOK(Start_strip + max1Cut);

				if (Rectan.Getsidecut() != 4 && min(Rectan.Get_h(), Rectan.Getmin()) >= MinDimensionPieza)
					list_Rectans.push_front(Rectan);
				Rectan = Rectan_new;
				Rectan.EverythingOK(Start_strip + max1Cut);


				//				if (Rectan.Get)
				inicio = 1;
			}
			if (MinDimensionPieza == MAXIMUM_INT_ROADEF)
			{
				ChangeWidthRectangles();
				//			GlassRsol Rsol = GlassRsol(cont_plates - 1, node_id++, Total_width_bin, 0, plate_w-Total_width_bin, plate_h, -3,1, node_ini_plate);
				//		G_Rsol_Items.push_back(Rsol);
				return Total_width_bin;
			}


		} while (posible == true);



	} while (list_Rectans.size() != 0);
	if (Width_start == Total_width_bin)
	{
		ChangeWidthRectangles();
		return 0;
	}
	else
	{
		ChangeWidthRectangles();
		if ((Total_area > Total_area_used) &&
			(Total_width_bin == Width_start ||
			((plate_w - Total_width_bin) != MinDimensionPieza && (plate_w - Total_width_bin) < (MinDimensionPieza + waste_min))))
		{
			return 0;
		}
		else
			return Total_width_bin;
	}
}
bool   Glass::CheckChangesToTheTop(int top, int bottom)
{

	//we can have both equal

	std::list< GlassRsol > ::reverse_iterator  it;
	for (it = G_Rsol_Items.rbegin(); it != G_Rsol_Items.rend() && (*it).plateId() == cont_plates; it++)
	{
		//not in the strip
		if ((*it).x() < Start_strip) return true;
		if ((*it).Y() <= bottom) return true;
		//Well positionated
		if ((*it).y() <= bottom) continue;

		if (Overlap((*it).x(), (*it).y() + top, (*it).X(), (*it).Y() + top) == true)
			return false;

	}
	return true;
}

bool   Glass::MakeFeasibleToTheTop(int top, int bottom)
{

	//we can have both equal

	std::list< GlassRsol > ::reverse_iterator  it;
	for (it = G_Rsol_Items.rbegin(); it != G_Rsol_Items.rend() && (*it).plateId() == cont_plates; it++)
	{
		if ((*it).x() < Start_strip) return true;
		if ((*it).Y() <= bottom) return true;
		if ((*it).y() <= bottom) continue;
		//Change this
		(*it).y((*it).y() + top);
		(*it).Y((*it).Y() + top);
	}
	return true;
}
//Change the position where can be a rectangle
bool Glass::ChangePositionToPlace(GlassRectangle &R)
{
	//We are going to try to place to the top
	if (R.Getsidecut() != 2)
	{
		if ((R.Getdefectminw() - R.Getpos_x()) <= (R.Getdefectminh() - R.Getpos_y()))
		{
			//Nothing to the bottom
			bool change_st = R.FindTheFollowingDefect(R.Getdefectminw(), 1);
			if (change_st == false && R.Getsidecut() == 1)
				change_st = R.FindTheFollowingDefect(R.Getdefectminh(), 2);
			return change_st;
		}
		else
		{
			//nothing to the left
			bool change_st = R.FindTheFollowingDefect(R.Getdefectminh(), 2);
			if (change_st == false)
				change_st = R.FindTheFollowingDefect(R.Getdefectminw(), 1);
			return change_st;
		}

	}
	else
	{
		if ((R.Getdefectminw() - R.Getpos_x()) <= (R.Getdefectminh() - R.Getpos_y()))
		{
			//Nothing to the bottom
			bool change_st = R.FindTheFollowingDefect(R.Getdefectminw(), 1);
			if (change_st == false && R.Getsidecut() == 1)
				change_st = R.FindTheFollowingDefect(R.Getdefectminh(), 2);
			return change_st;
		}
		else
		{
			//nothing to the left
			bool change_st = R.FindTheFollowingDefect(R.Getdefectminh(), 2);
			if (change_st == false)
				change_st = R.FindTheFollowingDefect(R.Getdefectminw(), 1);
			return change_st;
		}
	}
}

void Glass::UpdateMinDimension(std::pair<int, int> p)
{
	//If that item is not the minimum
	if (items[p.first].Getitem_min() != MinDimensionPieza)
		return;
	MinDimensionPieza = MAXIMUM_INT_ROADEF;
	MaxMinDimensionPieza = 0;

	for (int i = 0; i < batch_items; i++)
	{
		if (G_Vector_Items[i] != true)
		{
			if (items[i].Getitem_min() < MinDimensionPieza)
				MinDimensionPieza = items[i].Getitem_min();
			if (items[i].Getitem_min() > MaxMinDimensionPieza)
				MaxMinDimensionPieza = items[i].Getitem_min();
		}
	}

}

void Glass::PrintProblem(string problem)
{
	double perro = 0;
	printf("%s", problem.c_str());
//	scanf("%f", &perro);

}
void Glass::EraseRectangleFromList(LRectangles &lista, GlassRectangle &R)
{
	LRectangles::iterator it;
	for (it = lista.begin(); it != lista.end(); it++)
	{
		if ((*it).GetRectangle_id() == R.GetRectangle_id())
			//		if (Overlap((*it).Getpos_x(), (*it).Getpos_y(), (*it).Getpos_X(), (*it).Getpos_Y(), R.Getpos_x(), R.Getpos_y(), R.Getpos_X(), R.Getpos_Y()))
		{
			it = lista.erase(it);
			return;
		}
	}


	//No tiene por qué borrar otros
	//	PrintProblem("No puedo borrar ese elemento");
}
void Glass::ChangeRectangleFromList(LRectangles &lista, int width)
{
	LRectangles::iterator it;
	for (it = lista.begin(); it != lista.end(); it++)
	{
		if ((*it).Getpos_X()>width)
		{
			//		(*it).Set_w((*it).Get_w() - ((*it).Getpos_X() - width));
			(*it).Add_W(-((*it).Getpos_X() - width));

		}
	}


	//No tiene por qué borrar otros
	//	PrintProblem("No puedo borrar ese elemento");
}
void Glass::EraseRectangleFromList(LRectangles &lista, int id_rect)
{
	LRectangles::iterator it;
	for (it = lista.begin(); it != lista.end(); it++)
	{
		if ((*it).GetRectangle_id() == id_rect)
			//		if (Overlap((*it).Getpos_x(), (*it).Getpos_y(), (*it).Getpos_X(), (*it).Getpos_Y(), R.Getpos_x(), R.Getpos_y(), R.Getpos_X(), R.Getpos_Y()))
		{
			it = lista.erase(it);
			return;
		}
	}


	//No tiene por qué borrar otros
	//	PrintProblem("No puedo borrar ese elemento");
}
void Glass::TransformarSolucionSimetrica(string filep, bool quitar)
{
	FILE *fin3;
	G_Rotate_Solution.clear();
	G_Solution_Rotated.clear();
	G_first_time_node = false;

	fin3 = fopen(filep.c_str(), "r+");
	if (fin3 == NULL)
	{
		printf("Problemas al leer el archivo %s, no se encuentra el archivo", filep.c_str());
		exit(4);
	}
	char kk[256];
	fscanf(fin3, "%s\n", kk);
	int bin, node, x1, y1, x2, y2, type, cut, parent;

	do
	{
		fscanf(fin3, "%d;%d;%d;%d;%d;%d;%d;%d;", &bin, &node, &x1, &y1, &x2, &y2, &type, &cut);
		if (cut != 0)
		{
			fscanf(fin3, "%d\n", &parent);
		}
		else
		{
			parent = -1;
			fscanf(fin3, "\n");
		}


		Glass_Corte corte_temp;
		corte_temp.bin = bin; 		corte_temp.node = node;
		corte_temp.x1 = x1; 		corte_temp.y1 = y1;
		corte_temp.width = x2; 		corte_temp.height = y2;
		corte_temp.type = type; 		corte_temp.cut = cut;
		corte_temp.parent = parent;
		G_Solution_Rotated.push_front(corte_temp);


	} while (node != (G_Total_Nodes - 1));
	fclose(fin3);
	int waste_strip = 0;
	for (std::list< Glass_Corte>::iterator it = G_Solution_Rotated.begin(); it != G_Solution_Rotated.end(); it++)
	{
		if (quitar == true)
		{
			//Do not add the cut's of the first bin with waste
			if (!((*it).bin == 0 && (*it).cut == 1 && ((*it).type == (-1) || (*it).type == (-3)) && (*it).x1 != 0))
				G_Rotate_Solution.push_back((*it));
			else
				waste_strip = (*it).width;
		}
		else
			G_Rotate_Solution.push_back((*it));


	}
	//	int MaxWidth=MaxWidthLastPiece(G_Rsol_Items);
	G_Solution_Rotated.clear();


	int Bin_actual = 0;
	//Ya tenemos leída la solución y además en orden inverso
	int pos_ini_strip = -1;
	int pos_fin_strip = 0;
	int pos_ini_bin = 0;
	int pos_fin_bin = 0;
	G_rotated_nodes = 0;
	int Node_Bin = 0;


	Glass_Corte corte_temp;
	corte_temp.bin = 0;
	corte_temp.node = 0;
	corte_temp.x1 = 0;
	corte_temp.y1 = 0;
	corte_temp.width = plate_w;
	corte_temp.height = plate_h;
	corte_temp.type = -2;
	corte_temp.cut = 0;
	corte_temp.parent = -1;
	G_Solution_Rotated.push_back(corte_temp);
	G_rotated_nodes++;


	for (int i = 0; i<G_Rotate_Solution.size() - 1; i++)
	{

		//Is the bin packing
		if (G_Rotate_Solution[i].cut == 0)
		{

			pos_ini_bin = max(pos_fin_bin, pos_ini_bin);
			pos_fin_bin = i;
			//If we have change of bin
			pos_ini_strip = i;

			//			pos_ini_bin = i;

			Bin_actual++;
			Node_Bin = G_rotated_nodes;
			G_Solution_Rotated.push_back(G_Rotate_Solution[i]);
			G_rotated_nodes++;
		}
		else
		{

			if (G_Rotate_Solution[i].height == plate_h)
			{
				if (G_Rotate_Solution[i].type == -1)
				{

					//					ChangeDimensionsSimetry(G_Rotate_Solution[i]);
					G_Solution_Rotated.push_back(G_Rotate_Solution[i]);
					G_rotated_nodes++;
					pos_ini_strip++;


				}
				else
				{

					pos_ini_strip = max(pos_fin_strip, pos_ini_strip);
					pos_fin_strip = i;

					G_Solution_Rotated.push_back(G_Rotate_Solution[i]);

					G_rotated_nodes++;
					CorteHorizontalSimetry(pos_ini_strip, pos_fin_strip);
				}
				//				ChangeDimensionsSimetry(G_Rotate_Solution[i]);
				//				G_Solution_Rotated.push_back(G_Rotate_Solution[i]);

			}


		}




	}

	//Verificar nodos
	//linux quitar
	/*
	for (std::list< Glass_Corte>::iterator it = G_Solution_Rotated.begin(); it != G_Solution_Rotated.end(); it++)
	{
	int buscar = (*it).node;
	bool encontrado = false;
	for (int i = 0; i < (G_Rotate_Solution.size() - 1) && encontrado==false; i++)
	{
	if (G_Rotate_Solution[i].node == buscar)
	{
	encontrado = true;
	break;
	}
	}

	}
	for (int i = 0; i < (G_Rotate_Solution.size() - 1) ; i++)
	{
	int buscar = G_Rotate_Solution[i].node;
	bool encontrado = false;

	for (std::list< Glass_Corte>::iterator it = G_Solution_Rotated.begin(); it != G_Solution_Rotated.end() && encontrado == false; it++)
	{
	if ((*it).node == buscar)
	{
	encontrado = true;
	break;
	}

	}

	}
	*/
	//If change the bin, and nodes
	Bin_actual = 0;
	int node_Bin = 0;
	int node_strip = 0;
	int node_width = 0;
	int node_trim = 0;
	G_rotated_nodes = 0;
	G_Solution_Rotated.front().bin = NumberOfPlates + 1;
	G_Solution_Rotated.back().type = -3;
	for (std::list< Glass_Corte>::iterator it = G_Solution_Rotated.begin(); it != G_Solution_Rotated.end(); it++)
	{
		//Quitar trozo

		if ((*it).bin == NumberOfPlates && (*it).type == -3)
			(*it).type = -1;

		(*it).bin = NumberOfPlates - (*it).bin;
		(*it).node = G_rotated_nodes;

		if ((*it).cut == 0)
		{

			(*it).bin++;
			node_Bin = G_rotated_nodes;
		}
		else
		{
			//Vertical Strip
			if ((*it).cut == 1)
			{
				(*it).parent = node_Bin;
				node_strip = G_rotated_nodes;
			}
			else
			{
				if ((*it).cut == 2)
				{
					(*it).parent = node_strip;
					node_width = G_rotated_nodes;
				}
				else
				{
					if ((*it).cut == 3)
					{
						(*it).parent = node_width;
						node_trim = G_rotated_nodes;

					}
					else
					{
						(*it).parent = node_trim;
					}
				}

			}
		}
		ChangeDimensionsSimetry(*it);

		if ((*it).bin == NumberOfPlates && (*it).cut != 0)
		{
			(*it).x1 -= waste_strip;
			if ((*it).type == (-3))
				(*it).width += waste_strip;
		}
		G_rotated_nodes++;
	}

	//I have to add the last strip when quitar ==true
	if (quitar == false)
	{
		int cont = 0;
		for (std::list< Glass_Corte>::iterator it = G_Solution_Rotated.begin(); it != G_Solution_Rotated.end(); it++, cont++)
		{
			if ((*it).type == (-3) && cont != (G_Solution_Rotated.size() - 1))
				(*it).type = -1;
		}
	}
/*	else
	{
		int LastBin = G_Solution_Rotated.back().bin;
		int LastNodeBin = 0;
		//Find the last bin and strip
		for (std::list< Glass_Corte>::reverse_iterator it = G_Solution_Rotated.rbegin(); it != G_Solution_Rotated.rend() && (*it).bin ==LastBin; it++)
		{
			LastNodeBin = (*it).node;

		}
		Glass_Corte corte_temp;
		corte_temp.bin = LastBin;
		corte_temp.node = G_rotated_nodes;
		corte_temp.x1 = plate_w-waste_strip;
		corte_temp.y1 = 0;
		corte_temp.width = waste_strip;
		corte_temp.height = plate_h;
		corte_temp.type = -3;
		corte_temp.cut = 1;
		corte_temp.parent = LastNodeBin;
		G_Solution_Rotated.push_back(corte_temp);
		G_rotated_nodes++;
	}*/
	//

	//Write the rotated solution

	fin3 = fopen(filep.c_str(), "w+");
	if (fin3 == NULL)
	{
		printf("Problemas al leer el archivo %s, no se encuentra el archivo de solution", name_instance.c_str());
		exit(4);
	}


	//	PLATE_ID	NODE_ID	X	Y	WIDTH	HEIGHT	TYPE	CUT	PARENT
	fprintf(fin3, "PLATE_ID;NODE_ID;X;Y;WIDTH;HEIGHT;TYPE;CUT;PARENT\n");
	for (std::list< Glass_Corte>::iterator it = G_Solution_Rotated.begin(); it != G_Solution_Rotated.end(); it++)
	{
		if ((*it).cut != 0)
			fprintf(fin3, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).bin,
			(*it).node, (*it).x1, (*it).y1, (*it).width,
				(*it).height, (*it).type, (*it).cut, (*it).parent);
		else
			fprintf(fin3, "%d;%d;%d;%d;%d;%d;%d;%d;\n", (*it).bin,
			(*it).node, (*it).x1, (*it).y1, (*it).width,
				(*it).height, (*it).type, (*it).cut);

	}

	fclose(fin3);

}
void Glass::EscribirSolutionRotated(string filep, std::list< Glass_Corte> &lista)
{
	//Write the rotated solution
	FILE *fin3;
	fin3 = fopen(filep.c_str(), "w+");
	if (fin3 == NULL)
	{
		printf("Problemas al leer el archivo %s, no se encuentra el archivo de solution", name_instance.c_str());
		exit(4);
	}


	//	PLATE_ID	NODE_ID	X	Y	WIDTH	HEIGHT	TYPE	CUT	PARENT
	fprintf(fin3, "PLATE_ID;NODE_ID;X;Y;WIDTH;HEIGHT;TYPE;CUT;PARENT\n");
	for (std::list< Glass_Corte>::iterator it = lista.begin(); it != lista.end(); it++)
	{
		if ((*it).cut != 0)
			fprintf(fin3, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).bin,
			(*it).node, (*it).x1, (*it).y1, (*it).width,
				(*it).height, (*it).type, (*it).cut, (*it).parent);
		else
			fprintf(fin3, "%d;%d;%d;%d;%d;%d;%d;%d;\n", (*it).bin,
			(*it).node, (*it).x1, (*it).y1, (*it).width,
				(*it).height, (*it).type, (*it).cut);

	}

	fclose(fin3);
}
void Glass::CorteHorizontalSimetry(int ini, int fin)
{

	int pos_ini_horizontal = ini;
	int pos_fin_horizontal = 0;
	//It is a strip with only a piece
	if (ini == (fin - 1)) return;
	for (int i2 = ini; i2<fin; i2++)
	{
		if (G_Rotate_Solution[i2].cut == 2)
		{
			G_Solution_Rotated.push_back(G_Rotate_Solution[i2]);
			G_rotated_nodes++;
			//Other strip inside
			if (G_Rotate_Solution[i2].type == -2)
			{
				pos_ini_horizontal = max(pos_fin_horizontal, pos_ini_horizontal);
				pos_fin_horizontal = i2;
				CorteVerticalSimetry(pos_ini_horizontal, pos_fin_horizontal);
			}
			else
			{

				pos_ini_horizontal++;
				ini++;

			}

		}
	}
}
void Glass::CorteVerticalSimetry(int ini, int fin)
{
	//It is a strip with only a piece
	if (ini == fin) return;
	int pos_ini_vertical = ini;
	int pos_fin_vertical = 0;
	//It is a strip with only a piece
	if (ini == (fin - 1)) return;
	for (int i3 = ini; i3<fin; i3++)
	{
		if (G_Rotate_Solution[i3].cut == 3)
		{
			G_Solution_Rotated.push_back(G_Rotate_Solution[i3]);

			G_rotated_nodes++;
			//Other strip inside
			if (G_Rotate_Solution[i3].type == -2)
			{
				pos_ini_vertical = max(pos_fin_vertical, pos_ini_vertical);
				pos_fin_vertical = i3;
				/*				if (pos_ini_vertical == 0 && ini==0)
				{
				pos_ini_vertical = -1;
				//					G_first_time_node = true;
				}*/
				if (pos_ini_vertical == 0 &&
					G_Rotate_Solution[0].parent == G_Solution_Rotated.back().node)
					pos_ini_vertical = -1;
				for (int i4 = pos_ini_vertical + 1; i4 < pos_fin_vertical; i4++)
				{
					if (EstaYaPuesto(G_Rotate_Solution[i4].node) == false)
					{
						G_Solution_Rotated.push_back(G_Rotate_Solution[i4]);

						G_rotated_nodes++;
					}
				}

				if (pos_ini_vertical < 0)
					pos_ini_vertical = 0;
			}
			else
			{
				if (pos_ini_vertical != 0)
				{
					pos_ini_vertical = max(i3, pos_ini_vertical);
				}
				//			pos_ini_vertical++;

			}

		}
	}
}
bool Glass::EstaYaPuesto(int buscar)
{

	for (std::list< Glass_Corte>::iterator it = G_Solution_Rotated.begin(); it != G_Solution_Rotated.end(); it++)
	{
		if ((*it).node == buscar)
			return true;

	}
	return false;
}
void Glass::ChangeDimensionsSimetry(Glass_Corte &d)
{
	int newx1 = plate_w - d.x1 - d.width;
	int newy1 = plate_h - d.y1 - d.height;
	d.x1 = newx1;
	d.y1 = newy1;
}
//This function give back the new rectangle that we have to pack
/*GlassRectangle * AsignarRectangulo(GlassRectangle &rectangulo)
{

};*/



void Glass::JointItems()
{
	//	printf("Llego aquí  batch %d\n\n\n", batch_items);
	if (G_Rotate == false)
	{
		for (register int i = 0; i < batch_items - 2; i++)
		{
			int equals = 0;
			bool end = false;
			for (register int j = i + 1; j < batch_items - 1 && end != true; j++)
			{

				if ((items[i].Getitem_stack() == items[j].Getitem_stack()) &&
					(items[i].Getitem_h() == items[j].Getitem_h()) &&
					(items[i].Getitem_w() == items[j].Getitem_w()))
				{
					equals++;
					continue;
				}
				end = true;
			}
			if (equals == 0)
				continue;
			//		printf("Llego aquí %d \t%d\n\n\n",i,batch_items);
			for (; equals >= 0; equals--, i++)
			{
				items[i].Setitem_num(equals + 1);
			}

		}
	}
	else
	{
		for (register int i = batch_items - 1; i >= 1; i--)
		{
			int equals = 0;
			bool end = false;
			for (register int j = i - 1; j >= 0 && end != true; j--)
			{

				if ((items[i].Getitem_stack() == items[j].Getitem_stack()) &&
					(items[i].Getitem_h() == items[j].Getitem_h()) &&
					(items[i].Getitem_w() == items[j].Getitem_w()))
				{
					equals++;
					continue;
				}
				end = true;
			}
			if (equals == 0)
				continue;
			//		printf("Llego aquí %d \t%d\n\n\n",i,batch_items);
			for (; equals >= 0; equals--, i--)
			{
				items[i].Setitem_num(equals + 1);
			}

		}
	}

}




/***************************/

Coordinates Glass::ItemGreaterRNH_J(GlassRectangle &R)
{
	bool horizontal = true;
	int maximum = 0;
	int maximum2 = 0;
	int max_item = -1;
	int max_copies = 1;
	std::pair<int, int>  pk3(-1, -1);
	std::pair<int, int>  pk2(1, 1);
	Coordinates pk(pk3, pk2);
	int Y = R.Get_y();
	int multiplier = 1;
	bool before_greater = false;
	To_the_Top = false;
	//IT has to be on the top
	if ((R.Getsidecut() >= 3 && R.Get_y() != R.Getpos_y())
		|| ((R.Get_y() - R.Getpos_y()) >= waste_min && R.Get_y() >= 1210 + get_random(1, 10) * 150))
	{
		//We have to place to the top then we have to ask about this.
		Y = R.Getpos_Y();
		multiplier = 0;


	}

	//	if (deterministic == false && get_random(0, 2) == 0 && R.Get_y() != 0 && max_item == (-1))
	if (deterministic == false && get_random(0, 2) == 0 && R.Get_y() != 0 && R.Getpos_X() != plate_w && max_item == (-1))
	{

		before_greater = true;
		Coordinates p2 = ComputeLowestRectangle_H(R);
		if (p2.first.second != -1)
		{
			wider = true;
			return p2;
		}

	}
	int to_copies = 1;
	for (std::vector<int> ::iterator it = G_Random_Stacks.begin(); it != G_Random_Stacks.end(); it++)
	{
		int i = (*it);
		//If there are items in this stacks
		if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
		{
			if (one_item_each == true)
				to_copies = 1;
			else
				to_copies = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_num();
			for (register int copies = 1; copies <= to_copies; copies++)
			{
				int max_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
				int min_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
				//Randomness


				if (deterministic == false && max_item != (-1) && get_random(0, G_Vector_Stacks.size()) <= (G_Vector_Stacks.size() / (Para_Random)))
				{
					if (get_random(0, G_Vector_Size_Stacks[i] - G_Vector_Stacks[i]) <= (Para_Random)) //Cambiado
						continue;
				}
				//If can not fii in that hole
				if (min_dim_item*copies > R.Get_w() ||
					min_dim_item > R.Get_h())
					continue;
				else
				{

					//both measures are possible
					if (max_dim_item*copies <= R.Get_w() &&
						max_dim_item <= R.Get_h() &&
						max_dim_item*copies <= max1Cut)
					{
						if ((max_dim_item*copies > maximum)
							|| (max_dim_item*copies == maximum && min_dim_item > maximum2))
						{

							if (Overlap(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == false
								&& Wastemin(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == true)
							{
								max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
								max_copies = copies;
								maximum = max_dim_item * copies;
								maximum2 = min_dim_item;
								horizontal = true;
							}
							else
							{
								if (((min_dim_item*copies <= max1Cut) &&
									(min_dim_item*copies > maximum))
									|| (min_dim_item*copies == maximum && max_dim_item > maximum2))
								{
									if (Overlap(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == false
										&& Wastemin(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == true)

									{
										max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
										max_copies = copies;
										maximum = min_dim_item * copies;
										maximum2 = max_dim_item;
										horizontal = false;
									}
								}
							}

						}

					}
					else
					{
						if (max_dim_item*copies <= R.Get_w()
							&& max_dim_item*copies <= max1Cut)
						{
							if ((max_dim_item*copies > maximum)
								|| (max_dim_item*copies == maximum && min_dim_item > maximum2))
							{
								if (Overlap(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == false
									&& Wastemin(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == true)
								{
									max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
									max_copies = copies;
									maximum = max_dim_item * copies;
									maximum2 = min_dim_item;
									horizontal = true;
								}
							}
						}
						else
						{
							if (max_dim_item*copies <= R.Get_h())
							{
								if (((min_dim_item*copies <= max1Cut) &&
									(min_dim_item*copies > maximum))
									|| (min_dim_item*copies == maximum && max_dim_item > maximum2))
								{
									if (Overlap(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == false
										&& Wastemin(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == true)
									{
										max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
										max_copies = copies;
										maximum = min_dim_item * copies;
										maximum2 = max_dim_item;
										horizontal = false;
									}
								}
							}
						}
					}
				}
			}
		}

	}
	if (max_item == (-1) && R.Getpos_X() != plate_w && before_greater != true)
	{
		if (R.Getpos_X() != plate_w)
			return ComputeLowestRectangle_H(R);
		else
		{
			if (R.Getpos_Y() != plate_h)
				return ComputeLowestRectangle_V(R);
		}

	}
	To_the_Top = false;
	if (multiplier == 0) To_the_Top = true;
	pk.first.first = max_item;
	if (max_item != (-1))
		pk.first.second = horizontal;
	pk.second.first = max_copies;
	pk.second.second = 1;
	return pk;
}
//The lowest rectangel than exceeds the widt
Coordinates Glass::ComputeLowestRectangle_H(GlassRectangle &R)
{

	int Min = MAXIMUM_INT_ROADEF;
	std::pair<int, int>  pk3(-1, 0);
	std::pair<int, int>  pk2(1, 1);
	Coordinates pk(pk3, pk2);
	pk.first.first = -1;
	pk.first.second = -1;
	int to_copies = 1;
	int Y = R.Get_y();
	int multiplier = 1;
	//Compute lowest in this rectangle
	//	for (register int i = 0; i < stack_nbr; i++)
	//	{

	if (R.Getsidecut() >= 3 && R.Get_y() != R.Getpos_y())
	{
		//We have to place to the top then we have to ask about this.
		Y = R.Getpos_Y();
		multiplier = 0;

	}
	for (std::vector<int> ::iterator it = G_Random_Stacks.begin(); it != G_Random_Stacks.end(); it++)
	{
		int i = (*it);

		if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
		{
			if (one_item_each == true)
				to_copies = 1;
			else
				to_copies = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_num();
			for (register int copies = 1; copies <= to_copies; copies++)
			{
				if (deterministic == false && (Min != MAXIMUM_INT_ROADEF) && get_random(0, G_Vector_Stacks.size()) <= (G_Vector_Stacks.size() / (Para_Random)))
					continue;
				int max_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
				int min_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();


				if (min_dim_item > R.Get_h())
					continue;
				if (max_dim_item*copies <= R.Get_w())
					continue;

				if (min_dim_item*copies> R.Get_w() && max_dim_item <= R.Get_h() && (R.Get_x() + min_dim_item * copies) <= min(plate_w, Start_strip + max1Cut))
				{
					if (!(((max_dim_item) <= (R.Get_h() - waste_min)) || ((max_dim_item) == R.Get_h())))
						continue;




					//		if (min_dim_item*copies < Min && Overlap(R.Get_x(),R.Get_y(),R.Get_x()+ (min_dim_item*copies),R.Get_y()+max_dim_item)==false)
					if (min_dim_item*copies < Min && OverlapFullDerecha(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item) == false)
					{

						Min = min_dim_item * copies;
						pk.first.first = -1;
						pk.first.second = G_Matrix_Items[i][G_Vector_Stacks[i]];
						pk.second.first = false;
						pk.second.second = copies;
					}
				}
				else
				{
					if (max_dim_item*copies> R.Get_w() && min_dim_item <= R.Get_h() && (R.Get_x() + max_dim_item * copies) <= min(plate_w, Start_strip + max1Cut))
					{
						if (!(((min_dim_item) <= (R.Get_h() - waste_min)) || ((min_dim_item) == R.Get_h())))
							continue;

						if (max_dim_item*copies < Min && OverlapFullDerecha(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item) == false)
						{
							Min = max_dim_item * copies;
							pk.first.first = -1;
							pk.first.second = G_Matrix_Items[i][G_Vector_Stacks[i]];
							pk.second.first = true;
							pk.second.second = copies;
						}
					}

				}

			}
		}
	}

	if (Min == MAXIMUM_INT_ROADEF || ((Min + R.Get_x()) > (plate_w - waste_min) && (Min + R.Get_x()) <plate_w) || (Min + R.Get_x() > (min(plate_w, Start_strip + max1Cut))))
		//	||
		//		((Min -R.Get_w())<20 && (Min - R.Get_w())>0))
	{
		pk.first.second = -1;
	}
	To_the_Top = false;
	if (multiplier == 0) To_the_Top = true;
	return pk;
}
//The lowest rectangel than exceeds the height
Coordinates Glass::ComputeLowestRectangle_V(GlassRectangle &R)
{
	int Min = MAXIMUM_INT_ROADEF;
	std::pair<int, int>  pk3(-1, 0);
	std::pair<int, int>  pk2(1, 1);
	Coordinates pk(pk3, pk2);
	pk.first.first = -1;
	pk.first.second = -1;
	int to_copies = 1;
	//Compute lowest in this rectangle
	//	for (register int i = 0; i < stack_nbr; i++)
	//	{

	for (std::vector<int> ::iterator it = G_Random_Stacks.begin(); it != G_Random_Stacks.end(); it++)
	{
		int i = (*it);

		if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
		{
			if (one_item_each == true)
				to_copies = 1;
			else
				to_copies = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_num();
			for (register int copies = 1; copies <= to_copies; copies++)
			{
				if (deterministic == false && (Min != MAXIMUM_INT_ROADEF) && get_random(0, G_Vector_Stacks.size()) <= (G_Vector_Stacks.size() / (Para_Random)))
					continue;
				int max_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
				int min_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();

				if (min_dim_item > R.Get_w())
					continue;
				if (max_dim_item*copies <= R.Get_h())
					continue;

				if (min_dim_item> R.Get_h() && max_dim_item*copies <= R.Get_w() && (R.Get_y() + min_dim_item) <= plate_h)
				{
					if (!((max_dim_item*copies <= (R.Get_w() - waste_min)) || (max_dim_item*copies == R.Get_w())))
						continue;
					if (min_dim_item < Min && OverlapFullArriba(R.Get_x(), R.Get_y(), R.Get_x() + (max_dim_item*copies), R.Get_y() + min_dim_item) == false)
					{
						Min = min_dim_item;
						pk.first.first = -1;
						pk.first.second = G_Matrix_Items[i][G_Vector_Stacks[i]];
						pk.second.first = true;
						pk.second.second = copies;
					}
				}
				else
				{
					if (max_dim_item> R.Get_h() && min_dim_item*copies <= R.Get_w() && (R.Get_y() + max_dim_item) <= plate_h)
					{
						if (!((min_dim_item*copies <= (R.Get_w() - waste_min)) || (min_dim_item*copies == R.Get_w())))
							continue;
						if (max_dim_item < Min && OverlapFullArriba(R.Get_x(), R.Get_y(), R.Get_x() + (min_dim_item*copies), R.Get_y() + max_dim_item) == false)
						{
							Min = max_dim_item;
							pk.first.first = -1;
							pk.first.second = G_Matrix_Items[i][G_Vector_Stacks[i]];
							pk.second.first = false;
							pk.second.second = copies;
						}
					}

				}

			}
		}
	}
	if (Min == MAXIMUM_INT_ROADEF || ((Min + R.Get_y()) > (plate_h - waste_min) && (Min + R.Get_y()) <plate_h))
		//		||
		//		((Min - R.Get_h())<20 && (Min - R.Get_h())>0)) 
	{
		pk.first.second = -1;
	}

	return pk;
}

Coordinates Glass::ItemGreaterRNV_J(GlassRectangle &R)
{

	bool horizontal = false;
	int maximum2 = 0;
	int maximum = 0;
	int max_item = -1;
	int max_copies = 1;
	std::pair<int, int>  pk3(-1, -1);
	std::pair<int, int>  pk2(1, 1);
	Coordinates pk(pk3, pk2);
	int Y = R.Get_y();
	int multiplier = 1;
	bool before_greater = false;
	if ((R.Getsidecut() >= 3 && R.Get_y() != R.Getpos_y())
		|| ((R.Get_y() - R.Getpos_y()) >= waste_min && R.Get_y() >= 1000 + get_random(1, 20) * 100))
	{
		//We have to place to the top then we have to ask about this.
		Y = R.Getpos_Y();
		multiplier = 0;

	}
	//	if (deterministic == false && get_random(0, 2) == 0 && R.Get_y() != 0 && R.Getpos_Y() != plate_h && max_item == (-1))
	if (deterministic == false && get_random(0, 2) == 0 && R.Getpos_Y() != plate_h && max_item == (-1))
	{
		before_greater = true;
		Coordinates p2 = ComputeLowestRectangle_V(R);
		if (p2.first.second != -1)
		{
			higher = true;
			return p2;
		}

	}
	int to_copies = 1;
	for (std::vector<int> ::iterator it = G_Random_Stacks.begin(); it != G_Random_Stacks.end(); it++)
	{
		int i = (*it);
		//If there are items in this stacks
		if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
		{
			if (one_item_each == true)
				to_copies = 1;
			else
				to_copies = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_num();

			for (register int copies = 1; copies <= to_copies; copies++)
			{

				int max_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
				int min_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();

				if (deterministic == false && max_item != (-1) && get_random(0, G_Vector_Stacks.size()) <= (G_Vector_Stacks.size() / (Para_Random)))
				{
					if (get_random(0, G_Vector_Size_Stacks[i] - G_Vector_Stacks[i]) <= Para_Random)

						continue;
				}
				//If can not fii in that hole
				if (min_dim_item*copies > R.Get_w() ||
					min_dim_item > R.Get_h())
					continue;
				else
				{

					//both measures are possible
					if (max_dim_item*copies <= R.Get_w() &&
						max_dim_item <= R.Get_h() &&
						max_dim_item*copies <= max1Cut)
					{
						if ((max_dim_item > maximum)
							|| (max_dim_item == maximum && min_dim_item*copies > maximum2))
						{
							if (Overlap(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == false
								&& Wastemin(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == true)
							{
								max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
								max_copies = copies;
								maximum = max_dim_item;
								maximum2 = min_dim_item * copies;
								horizontal = false;
							}
							else
							{
								if ((min_dim_item > maximum)
									|| (min_dim_item == maximum && max_dim_item*copies > maximum2))
								{
									if (Overlap(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == false
										&& Wastemin(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == true)
									{
										max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
										max_copies = copies;
										maximum = min_dim_item;
										maximum2 = max_dim_item * copies;
										horizontal = true;
									}
								}
							}
						}

					}
					else
					{
						//one measure is possible
						if (max_dim_item*copies <= R.Get_w() &&
							max_dim_item*copies <= max1Cut)
						{
							if ((min_dim_item > maximum)
								|| (min_dim_item == maximum && max_dim_item*copies > maximum2))
							{
								if (Overlap(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == false
									&& Wastemin(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == true)
								{
									max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
									max_copies = copies;
									maximum = min_dim_item;
									maximum2 = max_dim_item * copies;
									horizontal = true;
								}
							}
						}
						else
						{
							if (max_dim_item <= R.Get_h() &&
								min_dim_item*copies <= max1Cut)
							{

								if ((max_dim_item > maximum)
									|| (max_dim_item == maximum && min_dim_item*copies > maximum2))
								{
									if (Overlap(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == false
										&& Wastemin(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == true)
									{
										max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
										max_copies = copies;
										maximum = max_dim_item;
										maximum2 = min_dim_item * copies;
										horizontal = false;
									}
								}
							}
						}
					}
				}

			}
		}

	}
	//	if (max_item == (-1) && R.Getnumdefects() == 0 && before_greater != true)
	if (max_item == (-1) && before_greater != true)
	{
		if (R.Getpos_Y() != plate_h)
			return ComputeLowestRectangle_V(R);
		else
		{
			if (R.Getpos_X() != plate_w)
				return ComputeLowestRectangle_H(R);

		}

	}
	To_the_Top = false;
	if (multiplier == 0) To_the_Top = true;
	pk.first.first = max_item;
	if (max_item != -1)
		pk.first.second = horizontal;
	pk.second.first = max_copies;
	pk.second.second = 1;
	return pk;
}
Coordinates Glass::ItemGreaterRNAreaV_J(GlassRectangle &R)
{
	bool horizontal = false;
	int maximum2 = 0;
	int maximum = 0;
	int max_item = -1;
	int max_copies = 1;
	std::pair<int, int>  pk3(-1, -1);
	std::pair<int, int>  pk2(1, 1);
	Coordinates pk(pk3, pk2);
	int Y = R.Get_y();
	int multiplier = 1;
	if (R.Getsidecut() >= 3 && R.Get_y() != R.Getpos_y())
	{
		//We have to place to the top then we have to ask about this.
		Y = R.Getpos_Y();
		multiplier = 0;

	}
	int to_copies = 1;
	for (std::vector<int> ::iterator it = G_Random_Stacks.begin(); it != G_Random_Stacks.end(); it++)
	{
		int i = (*it);


		//If there are items in this stacks
		if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
		{
			if (one_item_each == true)
				to_copies = 1;
			else
				to_copies = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_num();
			for (register int copies = 1; copies <= to_copies; copies++)
			{
				int max_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
				int min_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
				if (deterministic == false && max_item != (-1) && get_random(0, G_Vector_Stacks.size()) <= (G_Vector_Stacks.size() / (Para_Random)))
				{
					if (get_random(0, G_Vector_Size_Stacks[i] - G_Vector_Stacks[i]) <= Para_Random)
						continue;
				}
				//If can not fii in that hole
				if (min_dim_item*copies > R.Get_w() ||
					min_dim_item > R.Get_h())
					continue;
				else
				{

					//both measures are possible
					if (max_dim_item*copies <= R.Get_w() &&
						max_dim_item <= R.Get_h() &&
						max_dim_item*copies<max1Cut)
					{
						if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area()*copies > maximum)
							|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area()*copies == maximum && min_dim_item > maximum2))
						{
							if (Overlap(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == false
								&& Wastemin(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == true)
							{
								max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
								max_copies = copies;
								maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area()*copies;
								maximum2 = min_dim_item;
								horizontal = false;
							}
							else
							{
								if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area()*copies > maximum)
									|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area()*copies == maximum && max_dim_item > maximum2))
								{
									if (Overlap(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == false
										&& Wastemin(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == true)
									{
										max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
										max_copies = copies;
										maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area();
										maximum2 = max_dim_item * copies;
										horizontal = true;
									}
								}
							}
						}

					}
					else
					{
						//one measure is possible
						if (max_dim_item*copies <= R.Get_w() &&
							max_dim_item*copies <= max1Cut)
						{
							if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area()*copies > maximum)
								|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area()*copies == maximum && min_dim_item > maximum2))
							{
								if (Overlap(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == false
									&& Wastemin(R.Get_x(), Y - (1 - multiplier)*min_dim_item, R.Get_x() + max_dim_item * copies, Y + (multiplier)*min_dim_item, R) == true)
								{
									max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
									max_copies = copies;
									maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area()*copies;
									maximum2 = min_dim_item;
									horizontal = true;
								}
							}
						}
						else
						{
							if (max_dim_item <= R.Get_h() &&
								min_dim_item*copies <= max1Cut)
							{

								if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area()*copies > maximum)
									|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area()*copies == maximum && max_dim_item > maximum2))
								{
									if (Overlap(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == false
										&& Wastemin(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item * copies, Y + (multiplier)*max_dim_item, R) == true)
									{
										max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
										max_copies = copies;
										maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area()*copies;
										maximum2 = max_dim_item;
										horizontal = false;
									}
								}
							}
						}
					}
				}
			}
		}

	}
	if (max_item == (-1)) //Poner ComputeLowestRectangle lo de la perdida
						  //	if (max_item == (-1) && R.Getnumdefects() == 0 )
	{
		if (R.Getpos_Y() != plate_h)
			return ComputeLowestRectangle_V(R);
		else
		{
			if (R.Getpos_X() != plate_w)
				return ComputeLowestRectangle_H(R);
		}
	}
	To_the_Top = false;
	if (multiplier == 0) To_the_Top = true;
	pk.first.first = max_item;
	if (max_item != -1)
		pk.first.second = horizontal;
	pk.second.first = max_copies;
	pk.second.second = 1;
	return pk;
}

//Write solutions


//From a list of pieces, create the solution
void Glass::MakeFileTextSolutionNew(string path)
{
	//	return;
	//	RsolType 2 when starts a piece
	//	Rsoltype 1 when it is the last one


	FILE *file;
	//	strcat(name_instance, "_solution.csv");

	file = fopen(path.c_str(), "w+");
	if (file == NULL)
	{
		printf("Problemas al leer el archivo %s, no se encuentra el archivo", name_instance.c_str());
		exit(4);
	}

	int plateCod = -1, node = 0, parent = 0;
	int from_vertical_strip = 0;
	int to_vertical_strip = plate_w;
	int node_vertical_strip = -1;
	int node_horizontal_strip = -1;
	int parent_cut_vertical_strip = -1;
	bool vertical_strip = true;
	int top_item = -1;
	int node_plate = 0;
	int right_item = 0;
	int from_horizontal_strip = 0;
	int to_horizontal_strip = plate_h;
	int punto_x = 0;
	int punto_y = 0;
	int parent_cut = 0;
	bool start_strip = false;
	bool start_strip_vertical = false;



	//	PLATE_ID	NODE_ID	X	Y	WIDTH	HEIGHT	TYPE	CUT	PARENT
	fprintf(file, "PLATE_ID;NODE_ID;X;Y;WIDTH;HEIGHT;TYPE;CUT;PARENT\n");
	for (std::list<GlassRsol > ::iterator it = G_Best_Rsol_Items.begin(); it != G_Best_Rsol_Items.end(); it++)
	{
		start_strip = false;
		start_strip_vertical = false;

		//Change of plate //I have to write the first block down
		if ((*it).plateId() != plateCod)
		{

			if (to_horizontal_strip != (plate_h))
				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plateCod, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);
			if (to_vertical_strip != (plate_w))
				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plateCod, node++, to_vertical_strip, 0, plate_w - to_vertical_strip, plate_h, -1, 1, node_plate);
			/*if (to_horizontal_strip != (plate_h))
			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plate-1, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);
			if (to_vertical_strip != (plate_w))
			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plate-1, node++, to_vertical_strip, 0, plate_w - to_vertical_strip, plate_h, -1, 1, node_plate);
			*/
			punto_x = 0;
			punto_y = 0;
			top_item = -1;
			from_vertical_strip = 0;
			to_vertical_strip = plate_w;
			node_vertical_strip = -1;
			parent_cut_vertical_strip = -1;
			vertical_strip = true;
			node_plate = node;
			from_horizontal_strip = 0;
			to_horizontal_strip = plate_h;
			parent_cut = 0;
			plateCod = (*it).plateId();
			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;\n", (*it).plateId(), node, 0, 0, plate_w, plate_h, -2, 0);
			node++;
		}
		//the last item of the strip above
		if ((*it).copiesy() == 2 && to_vertical_strip == plate_w)
		{
			from_horizontal_strip = (*it).y();


		}
		if ((*it).iditem() == 162)
			int kk = 9;
		//If the item is down and it is a vertical cut
		//		if (((*it)[2] == 0 || NothingDown((it)) == true) )
		//Let start vertical strip
		if (((*it).copiesy() == 0) && ((*it).copiesx() != to_vertical_strip) && (*it).RsolType() != 1)
			//			if (((*it).copiesy() == 0))
		{

			if (to_horizontal_strip != (plate_h))
				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plateCod, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);

			vertical_strip = true;
			from_vertical_strip = to_vertical_strip;
			if (from_vertical_strip == plate_w)
				from_vertical_strip = 0;
			//If vertical cut
			//			to_vertical_strip = (*it)[7];
			to_vertical_strip = (*it).copiesx();
			//It could have an strip before this
			if ((*it).x() != from_vertical_strip)
			{
				int Min = ThereIsSomethingInThisStrip(it, from_vertical_strip, (*it).x(), to_vertical_strip);
				if (Min>from_vertical_strip)
				{
					WriteWastesLeft(file, from_vertical_strip, punto_y, (*it).X(), (*it).Y(), Min, 0, (*it).X(), to_horizontal_strip, (*it).plateId(), node_plate, node, 0);
					from_vertical_strip = Min;
				}
			}
			//		if ((*it)[6] == 2)
			//		to_vertical_strip = plate_w;
			//			if ((*it).Getcut() == 2 && (*it).RsolType() == 0)
			//			to_vertical_strip = plate_w;
			/*			if ((*it)[6] == 1)
			{
			//			to_vertical_strip = (*it)[3] * (*it)[7];
			to_vertical_strip = (*it)[3] ;
			}
			else
			to_vertical_strip = plate_w;
			*/			punto_x = from_vertical_strip;
			punto_y = 0;
			from_horizontal_strip = punto_y;
			//			to_horizontal_strip = (*it)[4];
			//			to_horizontal_strip = BottomInTheStripNext(it, to_vertical_strip);
			// change for
			to_horizontal_strip = BottomInTheStripNext(it, to_vertical_strip);
			int tallest = TallestInTheStrip(it, to_vertical_strip);

			if (tallest < to_horizontal_strip)
			{
				//The tallest in the strip beginnning
				int tallest2 = TallestBeginning(it, to_vertical_strip, tallest);
				int tallest3 = BottomBeginning(it, to_vertical_strip, tallest);

				//					int right=
				if (tallest2 > tallest3 || NoItemBetweenTallMin(it, to_vertical_strip, to_horizontal_strip) == false)
				{
					to_horizontal_strip = tallest;
				}
				//	int right = NextRightBottom(it, to_vertical_strip);

			}


			node_vertical_strip = node;
			parent_cut_vertical_strip = 1;
			//Write the vertical strip
			parent = node_vertical_strip;
			parent_cut = 1;
			parent = node;
			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, from_vertical_strip, 0, to_vertical_strip - from_vertical_strip, plate_h, -2, 1, node_plate);

			//Possible waste down the items
			//	if 
			if ((*it).RsolType() == 0)
			{
				WriteWastesBottom(file, punto_x, punto_y, (*it).X(), (*it).Y(), from_vertical_strip, (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId(), parent, node, parent_cut);
				if ((*it).y() > punto_y)
				{
					from_horizontal_strip = (*it).y();
				}
			}
			//			change
			//			parent_cut = 2;
			node_horizontal_strip = node;
			//Write horizontal strip
			if (((*it).X() - (*it).x()) != (to_vertical_strip - from_vertical_strip))
			{
				if (G_Rotate == false || ((to_horizontal_strip - from_horizontal_strip) < plate_h))
				{
					if ((*it).RsolType() == 0)
					{
						parent = node;
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, from_vertical_strip, (*it).y(), to_vertical_strip - from_vertical_strip, to_horizontal_strip - (*it).y(), -2, 2, node_vertical_strip);
						parent_cut = 2;//change 190918_1317
						start_strip_vertical = true;
						if ((*it).Getcut() != 2)
							(*it).Setcut((*it).Getcut() + 1);
					}
					else
					{
						parent = node;
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, to_horizontal_strip - from_horizontal_strip, -2, 2, node_vertical_strip);
						start_strip_vertical = true;
						parent_cut = 2;//change 190918_1317
						if ((*it).Getcut() != 2)
							(*it).Setcut((*it).Getcut() + 1);
					}
					//				parent = node;
				}
			}
			int node_ant = node;
			//	if ((*it).RsolType() != 0)//change 050119
			if ((*it).RsolType() >= 0)//change 050119
			{

				//				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut + 1); //V6
				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut); //change 200918 //V5
				punto_x = (*it).x();

			}
			//Write vertical strip here
			//		if ((to_horizontal_strip - from_horizontal_strip) != ((*it).Y() - (*it).y()) && ((*it).X() - from_vertical_strip) != (to_vertical_strip - from_vertical_strip)) //change 050319
			if ((to_horizontal_strip - from_horizontal_strip) != ((*it).Y() - (*it).y()) && ((*it).X() - (*it).x()) != (to_vertical_strip - from_vertical_strip))
			{
				//	if ((*it).RsolType() == 0) //change 050319
				if ((*it).RsolType() < 0) //change 050319
				{
					if (node_ant < node)
						parent = node - 1;
					else
						parent = node;
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, from_vertical_strip, (*it).y(), (*it).X() - from_vertical_strip, to_horizontal_strip - (*it).y(), -2, 3, parent - 1);
					(*it).Setcut((*it).Getcut() + 1);
					parent_cut = (*it).Getcut();
				}
				else
				{

					if (node_ant < node)
						parent = node - 1;
					else
						parent = node;
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, 3, parent - 1);
					parent = node - 1;//change 040319
					(*it).Setcut((*it).Getcut() + 1);
					parent_cut = (*it).Getcut();
					if ((*it).y() > from_horizontal_strip)
					{
						WriteWastesBottom(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);

					}

				}
			}
			//Possible waste left the items
			//			WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);
			if ((*it).RsolType() == 0)
			{
				//				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut + 1);
				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);//change 190918

																																									 //punto_x = from_vertical_strip;//change 050319
			}
			punto_y = (*it).Y();
			if (((*it).copiesx() == plate_w && NextRight(it, plate_w) > 0) && (*it).Getcut() == 3)
			{
				(*it).RsolType(2);
				PutRSolType(it, to_vertical_strip, to_horizontal_strip);
			}
			//change
			//			if ((*it).RsolType() <= 1 && (!((*it).copiesx() == plate_w && NextRight(it, plate_w)>0)))
			//			from_horizontal_strip = punto_y;

			//Write the square if there is space under o above
			if (parent_cut == 1 && (*it).Getcut() == 2)//change 190918_1317
				(*it).Setcut(1);//change 190918_1317

		}
		else
		{
			vertical_strip = false;

		}

		//If the item is placed cut horizontal
		if ((*it).Getcut() == 2 && vertical_strip != true && start_strip_vertical != true)
		{
			//WE are going to start a strip
			if (punto_x == from_vertical_strip)
			{
				//				if (NothingUp(it, to_vertical_strip) == (-1))
				//				to_horizontal_strip = (*it).Y();
				//		else
				to_horizontal_strip = BottomInTheStripNext(it, to_vertical_strip);
				int tallest = TallestInTheStrip(it, to_vertical_strip);

				if (tallest < to_horizontal_strip)
				{
					//The tallest in the strip beginnning
					int tallest2 = TallestBeginning(it, to_vertical_strip, tallest);
					int tallest3 = BottomBeginning(it, to_vertical_strip, tallest);

					//					int right=
					//if (tallest2 > tallest3 )//change 050319
					if (tallest2 > tallest3 || (from_horizontal_strip<tallest3 && tallest<to_horizontal_strip))//change 050319
					{
						to_horizontal_strip = tallest;
					}
					//	int right = NextRightBottom(it, to_vertical_strip);

				}
				if (to_horizontal_strip == plate_h)
				{
					//					int tallest = TallestInTheStrip(it, to_vertical_strip);
					//				if (tallest < to_horizontal_strip)
					//				to_horizontal_strip = tallest;
				}
				//tira completa de perdida bottom
				int right = NextRightBottom(it, to_vertical_strip);

				if (right == (*it).y())
				{
					WriteWastesBottom(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId(), node_vertical_strip, node, parent_cut - 1);
					from_horizontal_strip = (*it).y();
				}
				if (right > (*it).y())
				{
					int righttop = NextRightTop(it, to_vertical_strip);
					if (righttop > (*it).Y())
					{
						WriteWastesBottom(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId(), node_vertical_strip, node, parent_cut - 1);
						from_horizontal_strip = (*it).y();
					}

				}
				node_horizontal_strip = node;
				parent = node_vertical_strip;

				parent_cut = (*it).Getcut();
				//If it is smaller then I have to put something before
				/*/				if ((*it).x() != punto_x)
				{
				start_strip = true;
				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut);
				punto_x = (*it).x();
				}*/
				//tira completa 
				bool larga = false;
				//stripHeH
				if (((*it).X() - (*it).x()) != (to_vertical_strip - from_vertical_strip))
				{
					larga = true;
					start_strip = true;
					parent = node;
					parent_cut = (*it).Getcut();
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, to_horizontal_strip - from_horizontal_strip, -2, (*it).Getcut(), node_vertical_strip);
				}
				//If it is smaller then I have to put something before
				if ((*it).x() != punto_x)
				{
					start_strip = true;
					WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut);
					punto_x = (*it).x();
				}


				//if it is short than the strip
				if ((to_horizontal_strip - from_horizontal_strip) >((*it).Y() - (*it).y()) && larga == true)
					//				if (to_horizontal_strip != (*it).Y() && larga == true)
				{
					parent = node;
					parent_cut = (*it).Getcut() + 1;
					if (larga == true)
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, parent_cut, node_horizontal_strip);
					else
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, parent_cut_vertical_strip + 1, node_vertical_strip);
					//					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, 2, node_vertical_strip);
					if ((*it).y() > from_horizontal_strip)
					{
						int node_par = node - 1;
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), (*it).y() - from_horizontal_strip, -2, 4, node_par);
					}
					if (larga == true)
						(*it).Setcut((*it).Getcut() + 1);
					start_strip = true;
				}
				//I have to add something down 

				//change
				punto_y = (*it).y();
				//Completa la tira entera
				if (larga == false)
					(*it).Setcut((*it).Getcut() - 1);



			}
			else

				//change 210518
				//		WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId() - 1, parent, node,parent_cut);
			{
				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut);
				if ((to_horizontal_strip - from_horizontal_strip) >((*it).Y() - (*it).y()))
					//				if (to_horizontal_strip != (*it).Y() && larga == true)
				{

					(*it).Setcut((*it).Getcut() + 1);
				}
				//			parent = node_horizontal_strip;//change 31082018
			}
			//			if (((*it).Y() - (*it).y()) != (to_horizontal_strip - from_horizontal_strip))
			//		{
			//		WriteWastesBottom(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId() - 1, node_vertical_strip, node, parent_cut - 1);

			//			from_horizontal_strip
		}
		//If the item is placed cut horizontal
		//		if ((*it).Getcut() == 3 && (*it).copiesy() != 2 && start_strip != true && start_strip_vertical != true)
		if ((*it).Getcut() == 3 && start_strip != true && start_strip_vertical != true)
		{
			if (((*it).Y() - (*it).y()) != (to_horizontal_strip - from_horizontal_strip))
			{

				parent_cut = 2;

				//It could be one to the left
				WriteWastesLeft(file, punto_x, to_horizontal_strip, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), node_horizontal_strip, node, parent_cut);
				parent = node;
				parent_cut = (*it).Getcut();
				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, 3, node_horizontal_strip);

				//It could be one down
				if ((*it).RsolType() != 1)
					//					WriteWastesBottom(file, (*it).x(), punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);
					WriteWastesBottom(file, (*it).x(), from_horizontal_strip, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);
				else
					WriteWastesBottom(file, (*it).x(), 0, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);


				//				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId() - 1, node++,(*it).x()  , (*it).y(), (*it).X() - (*it).x(), to_horizontal_strip-(*it).y(), -2, (*it).Getcut()+1, parent);
				//			parent = node-1;
				parent_cut = (*it).Getcut();
			}
			else
			{
				(*it).Setcut((*it).Getcut() - 1);
				parent = node_horizontal_strip;
				parent_cut = 2;
				WriteWastesLeft(file, punto_x, to_horizontal_strip, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), node_horizontal_strip, node, parent_cut);
			}

			//			WriteWastesLeft(file, (*it).x() , punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId() - 1, parent, node, parent_cut);

			punto_x = (*it).x();
		}

		fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), (*it).y(), (*it).X() - (*it).x(), (*it).Y() - (*it).y(), (*it).iditem(), (*it).Getcut() + 1, parent);
		//		fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), (*it).y(), (*it).X() - (*it).x(), (*it).Y() - (*it).y(), (*it).iditem(), parent_cut+1, parent);
		//	int up = NothingUp(it, from_vertical_strip, to_vertical_strip);
		//change
		//		int up = NothingUp(it, to_vertical_strip);

		//		int up = NothingUp(it, to_vertical_strip,to_horizontal_strip);
		//nothing arriba
		if (to_horizontal_strip == plate_h) //if (up == -1)
		{
			if ((to_horizontal_strip - (*it).Y()) > 0)
			{

				// si el anterio 
				if (parent_cut != 2)
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, to_horizontal_strip - (*it).Y(), -1, parent_cut + 1, parent);
				else
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, to_horizontal_strip - (*it).Y(), -1, parent_cut, parent);

			}
			//			else
			//			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, plate_h - (*it).Y(), -1, parent_cut + 1, parent);

			//		else
			//			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId() - 1, node++, from_vertical_strip, to_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - to_horizontal_strip, -1, 2, node_vertical_strip);

			//				WriteWastesBottom(file, from_vertical_strip, to_horizontal_strip, to_vertical_strip-from_vertical_strip, plate_h-to_horizontal_strip, (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId() - 1, node_vertical_strip, node, parent_cut - 1);

			//			punto_y = 0;
		}
		else
		{
			int up = to_horizontal_strip;
			if ((to_horizontal_strip - (*it).Y()) > 0)
			{
				if (parent_cut != 2)
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, up - (*it).Y(), -1, parent_cut + 1, parent);
				else
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, up - (*it).Y(), -1, parent_cut, parent);
			}

			//			if ((up <= to_horizontal_strip) && (up - (*it).Y())>0)
			//				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, up - (*it).Y(), -1, parent_cut + 1, parent);


		}

		int right = NextRight(it, to_vertical_strip, to_horizontal_strip);
		//Strip to the right
		if (right == -1)
		{

			if ((*it).Getcut() != 1)
			{
				parent = node_horizontal_strip;
				parent_cut = 2;
			}
			punto_x = from_vertical_strip;

			if (to_vertical_strip > (*it).X())
			{
				//change 210518_035
				//	if (vertical_strip != true && (*it)[6] != 3)
				if ((*it).copiesy() != 2)
				{
					if (vertical_strip != true)
					{
						/*						if ((*it).RsolType() != 1)
						{
						if (punto_y != from_horizontal_strip)
						int kk = 9;
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).X(), punto_y, to_vertical_strip - (*it).X(), to_horizontal_strip - punto_y, -1, parent_cut + 1, parent);
						}
						else
						*/							fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).X(), from_horizontal_strip, to_vertical_strip - (*it).X(), to_horizontal_strip - from_horizontal_strip, -1, parent_cut + 1, parent);

					}
					else
					{
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).X(), (*it).y(), to_vertical_strip - (*it).X(), to_horizontal_strip - (*it).y(), -1, parent_cut + 1, parent);
					}
				}
				else
				{
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).X(), (*it).y(), to_vertical_strip - (*it).X(), to_horizontal_strip - (*it).y(), -1, parent_cut + 1, parent);

				}
			}

		}
		else
		{
			punto_x = (*it).X();
		}
		//		if (punto_x == from_vertical_strip && vertical_strip != true && right == -1)
		if (punto_x == from_vertical_strip && right == -1)
		{
			from_horizontal_strip = to_horizontal_strip;
			punto_y = from_horizontal_strip;
		}


		top_item = (*it).Y();
		right_item = (*it).Y();

		plateCod = (*it).plateId();

	}
	if (to_horizontal_strip != (plate_h))
		fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plateCod, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);

	if (to_vertical_strip != (plate_w))
		fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plateCod, node++, to_vertical_strip, 0, plate_w - to_vertical_strip, plate_h, -3, 1, node_plate);

	G_Total_Nodes = node;
	fclose(file);
}
//From a list of pieces, create the solution
void Glass::MakeFileTextSolution(string path)
{
	//	return;
	//	RsolType 2 when starts a piece
	//	Rsoltype 1 when it is the last one


	FILE *file;
	//	strcat(name_instance, "_solution.csv");

	file = fopen(path.c_str(), "w+");
	if (file == NULL)
	{
		printf("Problemas al leer el archivo %s, no se encuentra el archivo", name_instance.c_str());
		exit(4);
	}

	int plateCod = -1, node = 0, parent = 0;
	int from_vertical_strip = 0;
	int to_vertical_strip = plate_w;
	int node_vertical_strip = -1;
	int node_horizontal_strip = -1;
	int parent_cut_vertical_strip = -1;
	bool vertical_strip = true;
	int top_item = -1;
	int node_plate = 0;
	int right_item = 0;
	int from_horizontal_strip = 0;
	int to_horizontal_strip = plate_h;
	int punto_x = 0;
	int punto_y = 0;
	int parent_cut = 0;
	bool start_strip = false;
	bool start_strip_vertical = false;



	//	PLATE_ID	NODE_ID	X	Y	WIDTH	HEIGHT	TYPE	CUT	PARENT
	fprintf(file, "PLATE_ID;NODE_ID;X;Y;WIDTH;HEIGHT;TYPE;CUT;PARENT\n");
	for (std::list<GlassRsol > ::iterator it = G_Best_Rsol_Items.begin(); it != G_Best_Rsol_Items.end(); it++)
	{
		start_strip = false;
		start_strip_vertical = false;

		//Change of plate //I have to write the first block down
		if ((*it).plateId() != plateCod)
		{


				if (to_horizontal_strip != (plate_h))
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plateCod, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);
				if (to_vertical_strip != (plate_w))
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plateCod, node++, to_vertical_strip, 0, plate_w - to_vertical_strip, plate_h, -1, 1, node_plate);
				if ((*it).plateId() > (plateCod + 1))
				{
					for (int i = plateCod + 1; i < (*it).plateId(); i++)
					{
						int nodo_padre = node;
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;\n", i, node++, 0, 0, plate_w, plate_h, -2,0);
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", i, node++, 0, 0, plate_w, plate_h, -1, 1, nodo_padre);
					}


				}
			/*if (to_horizontal_strip != (plate_h))
			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plate-1, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);
			if (to_vertical_strip != (plate_w))
			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plate-1, node++, to_vertical_strip, 0, plate_w - to_vertical_strip, plate_h, -1, 1, node_plate);
			*/
			punto_x = 0;
			punto_y = 0;
			top_item = -1;
			from_vertical_strip = 0;
			to_vertical_strip = plate_w;
			node_vertical_strip = -1;
			parent_cut_vertical_strip = -1;
			vertical_strip = true;
			node_plate = node;
			from_horizontal_strip = 0;
			to_horizontal_strip = plate_h;
			parent_cut = 0;
			plateCod = (*it).plateId();
			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;\n", (*it).plateId(), node, 0, 0, plate_w, plate_h, -2, 0);
			//change 23/04/2020 he metido esta línea
	//		parent = node;
			node++;

		}
		//the last item of the strip above
		//change 080319
		/*
		if ((*it).copiesy() == 2 && to_vertical_strip == plate_w)
		{
		from_horizontal_strip = (*it).y();


		}*/
		if ((*it).iditem() == 158 && G_level >= 10)
			int kk = 9;
		//If the item is down and it is a vertical cut
		//		if (((*it)[2] == 0 || NothingDown((it)) == true) )
		//Let start vertical strip
		if (((*it).copiesy() == 0) && ((*it).copiesx() != to_vertical_strip) && (*it).RsolType() != 1)
			//			if (((*it).copiesy() == 0))
		{

			if (to_horizontal_strip != (plate_h))
				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plateCod, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);


			vertical_strip = true;
			from_vertical_strip = to_vertical_strip;
			if (from_vertical_strip == plate_w)
				from_vertical_strip = 0;
			//If vertical cut
			//			to_vertical_strip = (*it)[7];
			to_vertical_strip = (*it).copiesx();

			//It could have an strip before this
			//		if ((*it).x() != from_vertical_strip)
			if ((*it).x() != from_vertical_strip && (to_horizontal_strip != (plate_h) || (G_Rotate==true && node==1)))//change 060319
			{
				int Min = ThereIsSomethingInThisStrip(it, from_vertical_strip, (*it).x(), to_vertical_strip);
				if (Min>from_vertical_strip)
				{
					WriteWastesLeft(file, from_vertical_strip, punto_y, (*it).X(), (*it).Y(), Min, 0, (*it).X(), to_horizontal_strip, (*it).plateId(), node_plate, node, 0);
					from_vertical_strip = Min;
				}
			}
			//		if ((*it)[6] == 2)
			//		to_vertical_strip = plate_w;
			//			if ((*it).Getcut() == 2 && (*it).RsolType() == 0)
			//			to_vertical_strip = plate_w;
			/*			if ((*it)[6] == 1)
			{
			//			to_vertical_strip = (*it)[3] * (*it)[7];
			to_vertical_strip = (*it)[3] ;
			}
			else
			to_vertical_strip = plate_w;
			*/			punto_x = from_vertical_strip;
			punto_y = 0;
			from_horizontal_strip = punto_y;
			//			to_horizontal_strip = (*it)[4];
			//			to_horizontal_strip = BottomInTheStripNext(it, to_vertical_strip);
			// change for
			to_horizontal_strip = BottomInTheStripNext(it, to_vertical_strip);
			int tallest = TallestInTheStrip(it, to_vertical_strip);

			if (tallest < to_horizontal_strip)
			{
				//The tallest in the strip beginnning
				int tallest2 = TallestBeginning(it, to_vertical_strip, tallest);
				int tallest3 = BottomBeginning(it, to_vertical_strip, tallest);

				//					int right=
				if (tallest2 > tallest3 || NoItemBetweenTallMin(it, to_vertical_strip, to_horizontal_strip) == false)
				{
					to_horizontal_strip = tallest;
				}
				//	int right = NextRightBottom(it, to_vertical_strip);

			}


			node_vertical_strip = node;
			parent_cut_vertical_strip = 1;
			//Write the vertical strip
			parent = node_vertical_strip;
			parent_cut = 1;
			parent = node;
			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, from_vertical_strip, 0, to_vertical_strip - from_vertical_strip, plate_h, -2, 1, node_plate);

			//Possible waste down the items
			//	if 
			if ((*it).RsolType() == 0)
			{
				WriteWastesBottom(file, punto_x, punto_y, (*it).X(), (*it).Y(), from_vertical_strip, (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId(), parent, node, parent_cut);
				if ((*it).y() > punto_y)
				{
					from_horizontal_strip = (*it).y();
				}
			}
			//			change
			//			parent_cut = 2;
			node_horizontal_strip = node;
			//Write horizontal strip
			if (((*it).X() - (*it).x()) != (to_vertical_strip - from_vertical_strip))
			{
				if (G_Rotate == false || ((to_horizontal_strip - from_horizontal_strip) < plate_h))
				{
					if ((*it).RsolType() == 0)
					{
						parent = node;
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, from_vertical_strip, (*it).y(), to_vertical_strip - from_vertical_strip, to_horizontal_strip - (*it).y(), -2, 2, node_vertical_strip);
						parent_cut = 2;//change 190918_1317
						start_strip_vertical = true;
						if ((*it).Getcut() != 2)
							(*it).Setcut((*it).Getcut() + 1);
					}
					else
					{
						parent = node;
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, to_horizontal_strip - from_horizontal_strip, -2, 2, node_vertical_strip);
						start_strip_vertical = true;
						parent_cut = 2;//change 190918_1317
						if ((*it).Getcut() != 2)
							(*it).Setcut((*it).Getcut() + 1);
					}
					//				parent = node;
				}
			}
			int node_ant = node;
			//	if ((*it).RsolType() != 0)//change 050119
			if ((*it).RsolType() >= 0)//change 050119
			{

				//				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut + 1); //V6
				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut); //change 200918 //V5
				punto_x = (*it).x();

			}
			//Write vertical strip here
			//		if ((to_horizontal_strip - from_horizontal_strip) != ((*it).Y() - (*it).y()) && ((*it).X() - from_vertical_strip) != (to_vertical_strip - from_vertical_strip)) //change 050319
			if ((to_horizontal_strip - from_horizontal_strip) != ((*it).Y() - (*it).y()) && ((*it).X() - (*it).x()) != (to_vertical_strip - from_vertical_strip))
			{
				//	if ((*it).RsolType() == 0) //change 050319
				if ((*it).RsolType() < 0) //change 050319
				{
					if (node_ant < node)
						parent = node - 1;
					else
						parent = node;
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, from_vertical_strip, (*it).y(), (*it).X() - from_vertical_strip, to_horizontal_strip - (*it).y(), -2, 3, parent - 1);
					(*it).Setcut((*it).Getcut() + 1);
					parent_cut = (*it).Getcut();
				}
				else
				{

					if (node_ant < node)
						parent = node - 1;
					else
						parent = node;
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, 3, parent - 1);
					parent = node - 1;//change 040319
					(*it).Setcut((*it).Getcut() + 1);
					parent_cut = (*it).Getcut();
					if ((*it).y() > from_horizontal_strip)
					{
						WriteWastesBottom(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);

					}

				}
			}
			//Possible waste left the items
			//			WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);
			if ((*it).RsolType() == 0)
			{
				//				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut + 1);
				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);//change 190918

																																									 //punto_x = from_vertical_strip;//change 050319
			}
			punto_y = (*it).Y();
			if (((*it).copiesx() == plate_w && NextRight(it, plate_w) > 0) && (*it).Getcut() == 3)
			{
				(*it).RsolType(2);
				PutRSolType(it, to_vertical_strip, to_horizontal_strip);
			}
			//change
			//			if ((*it).RsolType() <= 1 && (!((*it).copiesx() == plate_w && NextRight(it, plate_w)>0)))
			//			from_horizontal_strip = punto_y;

			//Write the square if there is space under o above
			if (parent_cut == 1 && (*it).Getcut() == 2)//change 190918_1317
				(*it).Setcut(1);//change 190918_1317

		}
		else
		{
			vertical_strip = false;

		}

		//If the item is placed cut horizontal
		if ((*it).Getcut() == 2 && vertical_strip != true && start_strip_vertical != true)
		{
			//WE are going to start a strip
			if (punto_x == from_vertical_strip)
			{
				//				if (NothingUp(it, to_vertical_strip) == (-1))
				//				to_horizontal_strip = (*it).Y();
				//		else
				to_horizontal_strip = BottomInTheStripNext(it, to_vertical_strip);
				int tallest = TallestInTheStrip(it, to_vertical_strip);

				if (tallest < to_horizontal_strip)
				{
					//The tallest in the strip beginnning
					int tallest2 = TallestBeginning(it, to_vertical_strip, tallest);
					int tallest3 = BottomBeginning(it, to_vertical_strip, tallest);

					//					int right=
					//if (tallest2 > tallest3 )//change 050319
					if (tallest2 > tallest3 || (from_horizontal_strip<tallest3 && tallest<to_horizontal_strip))//change 050319
					{
						to_horizontal_strip = tallest;
					}
					else
					{
						if (to_horizontal_strip != plate_h)
						{
							int Secondtallest = SecondTallestInTheStrip(it, to_vertical_strip, tallest);
							if (Secondtallest <= (tallest - waste_min))
							{
								to_horizontal_strip = tallest;
							}
						}
					}
					//	int right = NextRightBottom(it, to_vertical_strip);

				}
				if (to_horizontal_strip == plate_h)
				{
					//					int tallest = TallestInTheStrip(it, to_vertical_strip);
					//				if (tallest < to_horizontal_strip)
					//				to_horizontal_strip = tallest;
				}
				//tira completa de perdida bottom
				int right = NextRightBottom(it, to_vertical_strip);

				//				if (right == (*it).y())//change 060319
				if (right == (*it).y() && (*it).y()>punto_y)//change 060319
				{
					//If 
					if (((*it).Y() < to_horizontal_strip) || ((*it).x() == from_vertical_strip && (*it).X() == to_vertical_strip) || LessTopStrip(it, to_vertical_strip) < to_horizontal_strip)//change 060319
					{
						WriteWastesBottom(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId(), node_vertical_strip, node, parent_cut - 1);
						from_horizontal_strip = (*it).y();
					}
				}
				if (right > (*it).y())
				{
					int righttop = NextRightTop(it, to_vertical_strip);
					if (righttop > (*it).Y())
					{
						WriteWastesBottom(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId(), node_vertical_strip, node, parent_cut - 1);
						from_horizontal_strip = (*it).y();
					}

				}
				node_horizontal_strip = node;
				parent = node_vertical_strip;

				parent_cut = (*it).Getcut();
				//If it is smaller then I have to put something before
				/*/				if ((*it).x() != punto_x)
				{
				start_strip = true;
				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut);
				punto_x = (*it).x();
				}*/
				//tira completa 
				bool larga = false;
				//stripHeH
				
				
	//			if ((((*it).X() - (*it).x()) != (to_vertical_strip - from_vertical_strip))&&((to_horizontal_strip - from_horizontal_strip)>0))
				if (((*it).X() - (*it).x()) != (to_vertical_strip - from_vertical_strip)) 
				{
//					if ((to_horizontal_strip - from_horizontal_strip) > 0)
	//				{
					larga = true;
					start_strip = true;
					
					parent_cut = (*it).Getcut();

					parent = node;
					//change 23/04/2020 he metido este if
	
						
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, to_horizontal_strip - from_horizontal_strip, -2, (*it).Getcut(), node_vertical_strip);
	//				}
				}
				//If it is smaller then I have to put something before
				if ((*it).x() != punto_x)
				{
					start_strip = true;
					WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut);
					punto_x = (*it).x();
				}


				//if it is short than the strip
				if ((to_horizontal_strip - from_horizontal_strip) >((*it).Y() - (*it).y()) && larga == true)
					//				if (to_horizontal_strip != (*it).Y() && larga == true)
				{
					parent = node;
					parent_cut = (*it).Getcut() + 1;
					if (larga == true)
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, parent_cut, node_horizontal_strip);
					else
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, parent_cut_vertical_strip + 1, node_vertical_strip);
					//					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, 2, node_vertical_strip);
					if ((*it).y() > from_horizontal_strip)
					{
						int node_par = node - 1;
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), (*it).y() - from_horizontal_strip, -2, 4, node_par);
					}
					if (larga == true)
						(*it).Setcut((*it).Getcut() + 1);
					start_strip = true;
				}
				//I have to add something down 

				//change
				punto_y = (*it).y();
				//Completa la tira entera
				if (larga == false)
					(*it).Setcut((*it).Getcut() - 1);



			}
			else

				//change 210518
				//		WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId() - 1, parent, node,parent_cut);
			{
				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut);
				if ((to_horizontal_strip - from_horizontal_strip) >((*it).Y() - (*it).y()))
					//				if (to_horizontal_strip != (*it).Y() && larga == true)
				{

					(*it).Setcut((*it).Getcut() + 1);
				}
				//			parent = node_horizontal_strip;//change 31082018
			}
			//			if (((*it).Y() - (*it).y()) != (to_horizontal_strip - from_horizontal_strip))
			//		{
			//		WriteWastesBottom(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId() - 1, node_vertical_strip, node, parent_cut - 1);

			//			from_horizontal_strip
		}
		//If the item is placed cut horizontal
		//		if ((*it).Getcut() == 3 && (*it).copiesy() != 2 && start_strip != true && start_strip_vertical != true)
		if ((*it).Getcut() == 3 && start_strip != true && start_strip_vertical != true)
		{
			if (((*it).Y() - (*it).y()) != (to_horizontal_strip - from_horizontal_strip))
			{

				parent_cut = 2;

				//It could be one to the left
				WriteWastesLeft(file, punto_x, to_horizontal_strip, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), node_horizontal_strip, node, parent_cut);
				parent = node;
				parent_cut = (*it).Getcut();
				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, 3, node_horizontal_strip);

				//It could be one down
				if ((*it).RsolType() != 1)
					//					WriteWastesBottom(file, (*it).x(), punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);
					WriteWastesBottom(file, (*it).x(), from_horizontal_strip, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);
				else
					WriteWastesBottom(file, (*it).x(), 0, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);


				//				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId() - 1, node++,(*it).x()  , (*it).y(), (*it).X() - (*it).x(), to_horizontal_strip-(*it).y(), -2, (*it).Getcut()+1, parent);
				//			parent = node-1;
				parent_cut = (*it).Getcut();
			}
			else
			{
				(*it).Setcut((*it).Getcut() - 1);
				parent = node_horizontal_strip;
				parent_cut = 2;
				WriteWastesLeft(file, punto_x, to_horizontal_strip, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), node_horizontal_strip, node, parent_cut);
			}

			//			WriteWastesLeft(file, (*it).x() , punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId() - 1, parent, node, parent_cut);

			punto_x = (*it).x();
		}

		fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), (*it).y(), (*it).X() - (*it).x(), (*it).Y() - (*it).y(), (*it).iditem(), (*it).Getcut() + 1, parent);
		//		fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), (*it).y(), (*it).X() - (*it).x(), (*it).Y() - (*it).y(), (*it).iditem(), parent_cut+1, parent);
		//	int up = NothingUp(it, from_vertical_strip, to_vertical_strip);
		//change
		//		int up = NothingUp(it, to_vertical_strip);

		//		int up = NothingUp(it, to_vertical_strip,to_horizontal_strip);
		//nothing arriba
		if (to_horizontal_strip == plate_h) //if (up == -1)
		{
			if ((to_horizontal_strip - (*it).Y()) > 0)
			{

				// si el anterio 
				if (parent_cut != 2)
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, to_horizontal_strip - (*it).Y(), -1, parent_cut + 1, parent);
				else
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, to_horizontal_strip - (*it).Y(), -1, parent_cut, parent);

			}
			//			else
			//			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, plate_h - (*it).Y(), -1, parent_cut + 1, parent);

			//		else
			//			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId() - 1, node++, from_vertical_strip, to_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - to_horizontal_strip, -1, 2, node_vertical_strip);

			//				WriteWastesBottom(file, from_vertical_strip, to_horizontal_strip, to_vertical_strip-from_vertical_strip, plate_h-to_horizontal_strip, (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId() - 1, node_vertical_strip, node, parent_cut - 1);

			//			punto_y = 0;
		}
		else
		{
			int up = to_horizontal_strip;
			if ((to_horizontal_strip - (*it).Y()) > 0)
			{
				if (parent_cut != 2)
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, up - (*it).Y(), -1, parent_cut + 1, parent);
				else
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, up - (*it).Y(), -1, parent_cut, parent);
			}

			//			if ((up <= to_horizontal_strip) && (up - (*it).Y())>0)
			//				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, up - (*it).Y(), -1, parent_cut + 1, parent);


		}

		int right = NextRight(it, to_vertical_strip, to_horizontal_strip);
		//Strip to the right
		if (right == -1)
		{

			if ((*it).Getcut() != 1)
			{
				parent = node_horizontal_strip;
				parent_cut = 2;
			}
			punto_x = from_vertical_strip;

			if (to_vertical_strip > (*it).X())
			{
				//change 210518_035
				//	if (vertical_strip != true && (*it)[6] != 3)
				if ((*it).copiesy() != 2)
				{
					if (vertical_strip != true)
					{
						/*						if ((*it).RsolType() != 1)
						{
						if (punto_y != from_horizontal_strip)
						int kk = 9;
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).X(), punto_y, to_vertical_strip - (*it).X(), to_horizontal_strip - punto_y, -1, parent_cut + 1, parent);
						}
						else
						*/							fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).X(), from_horizontal_strip, to_vertical_strip - (*it).X(), to_horizontal_strip - from_horizontal_strip, -1, parent_cut + 1, parent);

					}
					else
					{
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).X(), (*it).y(), to_vertical_strip - (*it).X(), to_horizontal_strip - (*it).y(), -1, parent_cut + 1, parent);
					}
				}
				else
				{
					//					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).X(), (*it).y(), to_vertical_strip - (*it).X(), to_horizontal_strip - (*it).y(), -1, parent_cut + 1, parent);//change 060319
					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).X(), from_horizontal_strip, to_vertical_strip - (*it).X(), to_horizontal_strip - from_horizontal_strip, -1, parent_cut + 1, parent);//change 060319

				}
			}

		}
		else
		{
			punto_x = (*it).X();
		}
		//		if (punto_x == from_vertical_strip && vertical_strip != true && right == -1)
		if (punto_x == from_vertical_strip && right == -1)
		{
			from_horizontal_strip = to_horizontal_strip;
			punto_y = from_horizontal_strip;
		}


		top_item = (*it).Y();
		right_item = (*it).Y();

		plateCod = (*it).plateId();

	}
	if (to_horizontal_strip != (plate_h))
		fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plateCod, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);

	if (to_vertical_strip != (plate_w))
		fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plateCod, node++, to_vertical_strip, 0, plate_w - to_vertical_strip, plate_h, -3, 1, node_plate);

	G_Total_Nodes = node;
	fclose(file);
}


//From a list of pieces, create the solution
void Glass::SaveSolution()
{
	//	return;
	//	RsolType 2 when starts a piece
	//	Rsoltype 1 when it is the last one

	int plateCod = -1, node = 0, parent = 0;
	int from_vertical_strip = 0;
	int to_vertical_strip = plate_w;
	int node_vertical_strip = -1;
	int node_horizontal_strip = -1;
	int parent_cut_vertical_strip = -1;
	bool vertical_strip = true;
	int top_item = -1;
	int node_plate = 0;
	int right_item = 0;
	int from_horizontal_strip = 0;
	int to_horizontal_strip = plate_h;
	int punto_x = 0;
	int punto_y = 0;
	int parent_cut = 0;
	bool start_strip = false;
	bool start_strip_vertical = false;



	//	PLATE_ID	NODE_ID	X	Y	WIDTH	HEIGHT	TYPE	CUT	PARENT
	//	fprintf(file, "PLATE_ID;NODE_ID;X;Y;WIDTH;HEIGHT;TYPE;CUT;PARENT\n");
	for (std::list<GlassRsol > ::iterator it = G_Best_Rsol_Items.begin(); it != G_Best_Rsol_Items.end(); it++)
	{
		start_strip = false;
		start_strip_vertical = false;

		//Change of plate //I have to write the first block down
		if ((*it).plateId() != plateCod)
		{

			if (to_horizontal_strip != (plate_h))
			{

				Glass_Corte corte_temp(plateCod, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);
				G_Solution_Rotated.push_front(corte_temp);

			}
			if (to_vertical_strip != (plate_w))
			{
				Glass_Corte  corte_temp(plateCod, node++, to_vertical_strip, 0, plate_w - to_vertical_strip, plate_h, -1, 1, node_plate);
				G_Solution_Rotated.push_front(corte_temp);

			}
			/*if (to_horizontal_strip != (plate_h))
			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plate-1, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);
			if (to_vertical_strip != (plate_w))
			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", plate-1, node++, to_vertical_strip, 0, plate_w - to_vertical_strip, plate_h, -1, 1, node_plate);
			*/
			punto_x = 0;
			punto_y = 0;
			top_item = -1;
			from_vertical_strip = 0;
			to_vertical_strip = plate_w;
			node_vertical_strip = -1;
			parent_cut_vertical_strip = -1;
			vertical_strip = true;
			node_plate = node;
			from_horizontal_strip = 0;
			to_horizontal_strip = plate_h;
			parent_cut = 0;
			plateCod = (*it).plateId();
			Glass_Corte  corte_temp((*it).plateId(), node, 0, 0, plate_w, plate_h, -2, 0);
			G_Solution_Rotated.push_front(corte_temp);

			node++;
		}
		//the last item of the strip above
		if ((*it).copiesy() == 2 && to_vertical_strip == plate_w)
		{
			from_horizontal_strip = (*it).y();


		}
		//If the item is down and it is a vertical cut
		//		if (((*it)[2] == 0 || NothingDown((it)) == true) )
		//Let start vertical strip
		if (((*it).copiesy() == 0) && ((*it).copiesx() != to_vertical_strip) && (*it).RsolType() != 1)
			//			if (((*it).copiesy() == 0))
		{

			if (to_horizontal_strip != (plate_h))
			{
				Glass_Corte  corte_temp(plateCod, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);
				G_Solution_Rotated.push_front(corte_temp);

			}
			vertical_strip = true;
			from_vertical_strip = to_vertical_strip;
			if (from_vertical_strip == plate_w)
				from_vertical_strip = 0;
			//If vertical cut
			//			to_vertical_strip = (*it)[7];
			to_vertical_strip = (*it).copiesx();
			//It could have an strip before this
			if ((*it).x() != from_vertical_strip)
			{
				int Min = ThereIsSomethingInThisStrip(it, from_vertical_strip, (*it).x(), to_vertical_strip);
				if (Min>from_vertical_strip)
				{
					PushWastesLeft(from_vertical_strip, punto_y, (*it).X(), (*it).Y(), Min, 0, (*it).X(), to_horizontal_strip, (*it).plateId(), node_plate, node, 0);
					from_vertical_strip = Min;
				}
			}
			//		if ((*it)[6] == 2)
			//		to_vertical_strip = plate_w;
			//			if ((*it).Getcut() == 2 && (*it).RsolType() == 0)
			//			to_vertical_strip = plate_w;
			/*			if ((*it)[6] == 1)
			{
			//			to_vertical_strip = (*it)[3] * (*it)[7];
			to_vertical_strip = (*it)[3] ;
			}
			else
			to_vertical_strip = plate_w;
			*/			punto_x = from_vertical_strip;
			punto_y = 0;
			from_horizontal_strip = punto_y;
			//			to_horizontal_strip = (*it)[4];
			//			to_horizontal_strip = BottomInTheStripNext(it, to_vertical_strip);
			// change for
			to_horizontal_strip = BottomInTheStripNext(it, to_vertical_strip);
			int tallest = TallestInTheStrip(it, to_vertical_strip);

			if (tallest < to_horizontal_strip)
			{
				//The tallest in the strip beginnning
				int tallest2 = TallestBeginning(it, to_vertical_strip, tallest);
				int tallest3 = BottomBeginning(it, to_vertical_strip, tallest);

				//					int right=
				if (tallest2 > tallest3 || NoItemBetweenTallMin(it, to_vertical_strip, to_horizontal_strip) == false)
				{
					to_horizontal_strip = tallest;
				}
				//	int right = NextRightBottom(it, to_vertical_strip);

			}


			node_vertical_strip = node;
			parent_cut_vertical_strip = 1;
			//Write the vertical strip
			parent = node_vertical_strip;
			parent_cut = 1;
			parent = node;
			Glass_Corte  corte_temp((*it).plateId(), node++, from_vertical_strip, 0, to_vertical_strip - from_vertical_strip, plate_h, -2, 1, node_plate);
			G_Solution_Rotated.push_front(corte_temp);



			//Possible waste down the items
			//	if 
			if ((*it).RsolType() == 0)
			{
				PushWastesBottom(punto_x, punto_y, (*it).X(), (*it).Y(), from_vertical_strip, (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId(), parent, node, parent_cut);
				if ((*it).y() > punto_y)
				{
					from_horizontal_strip = (*it).y();
				}
			}
			//			change
			//			parent_cut = 2;
			node_horizontal_strip = node;
			//Write horizontal strip
			if (((*it).X() - (*it).x()) != (to_vertical_strip - from_vertical_strip))
			{
				if (G_Rotate == false || ((to_horizontal_strip - from_horizontal_strip) < plate_h))
				{
					if ((*it).RsolType() == 0)
					{
						parent = node;
						Glass_Corte  corte_temp((*it).plateId(), node++, from_vertical_strip, (*it).y(), to_vertical_strip - from_vertical_strip, to_horizontal_strip - (*it).y(), -2, 2, node_vertical_strip);
						G_Solution_Rotated.push_front(corte_temp);
						parent_cut = 2;//change 190918_1317
						start_strip_vertical = true;
						if ((*it).Getcut() != 2)
							(*it).Setcut((*it).Getcut() + 1);
					}
					else
					{
						parent = node;
						Glass_Corte  corte_temp((*it).plateId(), node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, to_horizontal_strip - from_horizontal_strip, -2, 2, node_vertical_strip);
						G_Solution_Rotated.push_front(corte_temp);
						start_strip_vertical = true;
						parent_cut = 2;//change 190918_1317
						if ((*it).Getcut() != 2)
							(*it).Setcut((*it).Getcut() + 1);
					}
					//				parent = node;
				}
			}
			int node_ant = node;
			if ((*it).RsolType() != 0)
			{

				//				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut + 1); //V6
				PushWastesLeft(punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut); //change 200918 //V5
				punto_x = (*it).x();

			}
			//Write vertical strip here
			if ((to_horizontal_strip - from_horizontal_strip) != ((*it).Y() - (*it).y()) && ((*it).X() - from_vertical_strip) != (to_vertical_strip - from_vertical_strip))
			{
				if ((*it).RsolType() == 0)
				{
					if (node_ant < node)
						parent = node - 1;
					else
						parent = node;
					Glass_Corte  corte_temp((*it).plateId(), node++, from_vertical_strip, (*it).y(), (*it).X() - from_vertical_strip, to_horizontal_strip - (*it).y(), -2, 3, parent - 1);
					G_Solution_Rotated.push_front(corte_temp);
					(*it).Setcut((*it).Getcut() + 1);
					parent_cut = (*it).Getcut();
				}
				else
				{

					if (node_ant < node)
						parent = node - 1;
					else
						parent = node;
					Glass_Corte  corte_temp((*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, 3, parent - 1);
					G_Solution_Rotated.push_front(corte_temp);
					(*it).Setcut((*it).Getcut() + 1);
					parent_cut = (*it).Getcut();
					if ((*it).y() > from_horizontal_strip)
					{
						PushWastesBottom(punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);

					}

				}
			}
			//Possible waste left the items
			//			WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);
			if ((*it).RsolType() == 0)
			{
				//				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut + 1);
				PushWastesLeft(punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);//change 190918

				punto_x = from_vertical_strip;
			}
			punto_y = (*it).Y();
			if (((*it).copiesx() == plate_w && NextRight(it, plate_w) > 0) && (*it).Getcut() == 3)
			{
				(*it).RsolType(2);
				PutRSolType(it, to_vertical_strip, to_horizontal_strip);
			}
			//change
			//			if ((*it).RsolType() <= 1 && (!((*it).copiesx() == plate_w && NextRight(it, plate_w)>0)))
			//			from_horizontal_strip = punto_y;

			//Write the square if there is space under o above
			if (parent_cut == 1 && (*it).Getcut() == 2)//change 190918_1317
				(*it).Setcut(1);//change 190918_1317

		}

		else
		{
			vertical_strip = false;

		}

		//If the item is placed cut horizontal
		if ((*it).Getcut() == 2 && vertical_strip != true && start_strip_vertical != true)
		{
			//WE are going to start a strip
			if (punto_x == from_vertical_strip)
			{
				//				if (NothingUp(it, to_vertical_strip) == (-1))
				//				to_horizontal_strip = (*it).Y();
				//		else
				to_horizontal_strip = BottomInTheStripNext(it, to_vertical_strip);
				int tallest = TallestInTheStrip(it, to_vertical_strip);

				if (tallest < to_horizontal_strip)
				{
					//The tallest in the strip beginnning
					int tallest2 = TallestBeginning(it, to_vertical_strip, tallest);
					int tallest3 = BottomBeginning(it, to_vertical_strip, tallest);

					//					int right=
					if (tallest2 > tallest3)
					{
						to_horizontal_strip = tallest;
					}
					//	int right = NextRightBottom(it, to_vertical_strip);

				}
				if (to_horizontal_strip == plate_h)
				{
					//					int tallest = TallestInTheStrip(it, to_vertical_strip);
					//				if (tallest < to_horizontal_strip)
					//				to_horizontal_strip = tallest;
				}
				//tira completa de perdida bottom
				int right = NextRightBottom(it, to_vertical_strip);

				if (right == (*it).y())
				{
					PushWastesBottom(punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId(), node_vertical_strip, node, parent_cut - 1);
					from_horizontal_strip = (*it).y();
				}
				if (right > (*it).y())
				{
					int righttop = NextRightTop(it, to_vertical_strip);
					if (righttop > (*it).Y())
					{
						PushWastesBottom(punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId(), node_vertical_strip, node, parent_cut - 1);
						from_horizontal_strip = (*it).y();
					}

				}
				node_horizontal_strip = node;
				parent = node_vertical_strip;

				parent_cut = (*it).Getcut();
				//If it is smaller then I have to put something before
				/*/				if ((*it).x() != punto_x)
				{
				start_strip = true;
				WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut);
				punto_x = (*it).x();
				}*/
				//tira completa 
				bool larga = false;
				//stripHeH
				if (((*it).X() - (*it).x()) != (to_vertical_strip - from_vertical_strip))
				{
					larga = true;
					start_strip = true;
					parent = node;
					parent_cut = (*it).Getcut();
					Glass_Corte  corte_temp((*it).plateId(), node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, to_horizontal_strip - from_horizontal_strip, -2, (*it).Getcut(), node_vertical_strip);
					G_Solution_Rotated.push_front(corte_temp);
				}
				//If it is smaller then I have to put something before
				if ((*it).x() != punto_x)
				{
					start_strip = true;
					PushWastesLeft(punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut);
					punto_x = (*it).x();
				}


				//if it is short than the strip
				if ((to_horizontal_strip - from_horizontal_strip) >((*it).Y() - (*it).y()) && larga == true)
					//				if (to_horizontal_strip != (*it).Y() && larga == true)
				{
					parent = node;
					parent_cut = (*it).Getcut() + 1;
					if (larga == true)
					{
						Glass_Corte  corte_temp((*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, parent_cut, node_horizontal_strip);
						G_Solution_Rotated.push_front(corte_temp);
					}
					else
					{
						Glass_Corte  corte_temp((*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, parent_cut_vertical_strip + 1, node_vertical_strip);
						G_Solution_Rotated.push_front(corte_temp);
					}
					//					fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, 2, node_vertical_strip);
					if ((*it).y() > from_horizontal_strip)
					{
						int node_par = node - 1;
						Glass_Corte  corte_temp((*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), (*it).y() - from_horizontal_strip, -2, 4, node_par);
						G_Solution_Rotated.push_front(corte_temp);
					}
					if (larga == true)
						(*it).Setcut((*it).Getcut() + 1);
					start_strip = true;
				}
				//I have to add something down 

				//change
				punto_y = (*it).y();
				//Completa la tira entera
				if (larga == false)
					(*it).Setcut((*it).Getcut() - 1);



			}
			else

				//change 210518
				//		WriteWastesLeft(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId() - 1, parent, node,parent_cut);
			{
				PushWastesLeft(punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), parent, node, parent_cut);
				if ((to_horizontal_strip - from_horizontal_strip) >((*it).Y() - (*it).y()))
					//				if (to_horizontal_strip != (*it).Y() && larga == true)
				{

					(*it).Setcut((*it).Getcut() + 1);
				}
				//			parent = node_horizontal_strip;//change 31082018
			}
			//			if (((*it).Y() - (*it).y()) != (to_horizontal_strip - from_horizontal_strip))
			//		{
			//		WriteWastesBottom(file, punto_x, punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId() - 1, node_vertical_strip, node, parent_cut - 1);

			//			from_horizontal_strip
		}
		//If the item is placed cut horizontal
		//		if ((*it).Getcut() == 3 && (*it).copiesy() != 2 && start_strip != true && start_strip_vertical != true)
		if ((*it).Getcut() == 3 && start_strip != true && start_strip_vertical != true)
		{
			if (((*it).Y() - (*it).y()) != (to_horizontal_strip - from_horizontal_strip))
			{

				parent_cut = 2;

				//It could be one to the left
				PushWastesLeft(punto_x, to_horizontal_strip, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), node_horizontal_strip, node, parent_cut);
				parent = node;
				parent_cut = (*it).Getcut();
				Glass_Corte  corte_temp((*it).plateId(), node++, (*it).x(), from_horizontal_strip, (*it).X() - (*it).x(), to_horizontal_strip - from_horizontal_strip, -2, 3, node_horizontal_strip);
				G_Solution_Rotated.push_front(corte_temp);
				//It could be one down
				if ((*it).RsolType() != 1)
					//					WriteWastesBottom(file, (*it).x(), punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);
					PushWastesBottom((*it).x(), from_horizontal_strip, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);
				else
					PushWastesBottom((*it).x(), 0, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId(), parent, node, parent_cut);


				//				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId() - 1, node++,(*it).x()  , (*it).y(), (*it).X() - (*it).x(), to_horizontal_strip-(*it).y(), -2, (*it).Getcut()+1, parent);
				//			parent = node-1;
				parent_cut = (*it).Getcut();
			}
			else
			{
				(*it).Setcut((*it).Getcut() - 1);
				parent = node_horizontal_strip;
				parent_cut = 2;
				PushWastesLeft(punto_x, to_horizontal_strip, (*it).X(), (*it).Y(), (*it).x(), from_horizontal_strip, (*it).X(), to_horizontal_strip, (*it).plateId(), node_horizontal_strip, node, parent_cut);
			}

			//			WriteWastesLeft(file, (*it).x() , punto_y, (*it).X(), (*it).Y(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId() - 1, parent, node, parent_cut);

			punto_x = (*it).x();
		}

		Glass_Corte  corte_temp((*it).plateId(), node++, (*it).x(), (*it).y(), (*it).X() - (*it).x(), (*it).Y() - (*it).y(), (*it).iditem(), (*it).Getcut() + 1, parent);
		G_Solution_Rotated.push_front(corte_temp);
		//		fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).x(), (*it).y(), (*it).X() - (*it).x(), (*it).Y() - (*it).y(), (*it).iditem(), parent_cut+1, parent);
		//	int up = NothingUp(it, from_vertical_strip, to_vertical_strip);
		//change
		//		int up = NothingUp(it, to_vertical_strip);

		//		int up = NothingUp(it, to_vertical_strip,to_horizontal_strip);
		//nothing arriba
		if (to_horizontal_strip == plate_h) //if (up == -1)
		{
			if ((to_horizontal_strip - (*it).Y()) > 0)
			{

				// si el anterio 
				if (parent_cut != 2)
				{
					Glass_Corte  corte_temp((*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, to_horizontal_strip - (*it).Y(), -1, parent_cut + 1, parent);
					G_Solution_Rotated.push_front(corte_temp);
				}
				else
				{
					Glass_Corte  corte_temp((*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, to_horizontal_strip - (*it).Y(), -1, parent_cut, parent);
					G_Solution_Rotated.push_front(corte_temp);
				}

			}
			//			else
			//			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, plate_h - (*it).Y(), -1, parent_cut + 1, parent);

			//		else
			//			fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId() - 1, node++, from_vertical_strip, to_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - to_horizontal_strip, -1, 2, node_vertical_strip);

			//				WriteWastesBottom(file, from_vertical_strip, to_horizontal_strip, to_vertical_strip-from_vertical_strip, plate_h-to_horizontal_strip, (*it).x(), (*it).y(), to_vertical_strip, (*it).Y(), (*it).plateId() - 1, node_vertical_strip, node, parent_cut - 1);

			//			punto_y = 0;
		}
		else
		{
			int up = to_horizontal_strip;
			if ((to_horizontal_strip - (*it).Y()) > 0)
			{
				if (parent_cut != 2)
				{
					Glass_Corte  corte_temp((*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, up - (*it).Y(), -1, parent_cut + 1, parent);
					G_Solution_Rotated.push_front(corte_temp);
				}
				else
				{
					Glass_Corte  corte_temp((*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, up - (*it).Y(), -1, parent_cut, parent);
					G_Solution_Rotated.push_front(corte_temp);
				}
			}

			//			if ((up <= to_horizontal_strip) && (up - (*it).Y())>0)
			//				fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, punto_x, (*it).Y(), (*it).X() - punto_x, up - (*it).Y(), -1, parent_cut + 1, parent);


		}

		int right = NextRight(it, to_vertical_strip, to_horizontal_strip);
		//Strip to the right
		if (right == -1)
		{

			if ((*it).Getcut() != 1)
			{
				parent = node_horizontal_strip;
				parent_cut = 2;
			}
			punto_x = from_vertical_strip;

			if (to_vertical_strip > (*it).X())
			{
				//change 210518_035
				//	if (vertical_strip != true && (*it)[6] != 3)
				if ((*it).copiesy() != 2)
				{
					if (vertical_strip != true)
					{
						/*						if ((*it).RsolType() != 1)
						{
						if (punto_y != from_horizontal_strip)
						int kk = 9;
						fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", (*it).plateId(), node++, (*it).X(), punto_y, to_vertical_strip - (*it).X(), to_horizontal_strip - punto_y, -1, parent_cut + 1, parent);
						}
						else
						*/							Glass_Corte  corte_temp((*it).plateId(), node++, (*it).X(), from_horizontal_strip, to_vertical_strip - (*it).X(), to_horizontal_strip - from_horizontal_strip, -1, parent_cut + 1, parent);
					G_Solution_Rotated.push_front(corte_temp);
					}
					else
					{
						Glass_Corte  corte_temp((*it).plateId(), node++, (*it).X(), (*it).y(), to_vertical_strip - (*it).X(), to_horizontal_strip - (*it).y(), -1, parent_cut + 1, parent);
						G_Solution_Rotated.push_front(corte_temp);
					}
				}
				else
				{
					Glass_Corte  corte_temp((*it).plateId(), node++, (*it).X(), (*it).y(), to_vertical_strip - (*it).X(), to_horizontal_strip - (*it).y(), -1, parent_cut + 1, parent);
					G_Solution_Rotated.push_front(corte_temp);
				}
			}

		}
		else
		{
			punto_x = (*it).X();
		}
		//		if (punto_x == from_vertical_strip && vertical_strip != true && right == -1)
		if (punto_x == from_vertical_strip && right == -1)
		{
			from_horizontal_strip = to_horizontal_strip;
			punto_y = from_horizontal_strip;
		}


		top_item = (*it).Y();
		right_item = (*it).Y();

		plateCod = (*it).plateId();

	}
	if (to_horizontal_strip != (plate_h))
	{
		Glass_Corte  corte_temp(plateCod, node++, from_vertical_strip, from_horizontal_strip, to_vertical_strip - from_vertical_strip, plate_h - from_horizontal_strip, -1, 2, node_vertical_strip);
		G_Solution_Rotated.push_front(corte_temp);
	}
	if (to_vertical_strip != (plate_w))
	{
		Glass_Corte  corte_temp(plateCod, node++, to_vertical_strip, 0, plate_w - to_vertical_strip, plate_h, -3, 1, node_plate);
		G_Solution_Rotated.push_front(corte_temp);
	}
	G_Total_Nodes = node;

}

bool Glass::NextPut(std::list< GlassRsol > ::reverse_iterator  itk, int WID)
{

	std::list< GlassRsol > ::reverse_iterator  it2;
	it2 = itk;
	it2++;
	for (; it2 != G_Rsol_Items.rend(); it2++)
	{
		if ((*it2).plateId() != (*itk).plateId()) //different plate
			return false;
		if ((*it2).copiesx() >= WID && ((*it2).X() <= (*itk).x()))
		{

			return true;
		}
	}
	return false;
}

int  Glass::NextRight(std::vector< std::vector <int> > ::iterator  &it, int toright)
{
	if (it == G_Best_Positions_Items.end()) return -1;
	int maximo = -1;
	std::vector< std::vector <int> > ::iterator  it2;
	for (it2 = it + 1; it2 != G_Best_Positions_Items.end(); it2++)
	{
		if ((*it2)[5] != (*it)[5]) //different plate
			return maximo;
		if ((*it2)[3] > toright) //not in the horizontal strip
			return maximo;
		if ((*it2)[2] >= (*it)[4]) //not in the horizontal strip
			return maximo;
		if ((*it2)[1] >= (*it)[3]) //out of the strip
			return (*it2)[1];

	}
	return maximo;
}
int  Glass::NextRightBottom(std::vector< std::vector <int> > ::iterator  &it, int toright)
{
	if (it == G_Best_Positions_Items.end()) return -1;
	int minimo = (*it)[4];
	std::vector< std::vector <int> > ::iterator  it2;
	for (it2 = it + 1; it2 != G_Best_Positions_Items.end(); it2++)
	{
		if ((*it2)[5] != (*it)[5]) //different plate
			return minimo;
		if ((*it2)[3] > toright) //not in the horizontal strip
			return minimo;
		if ((*it2)[2] < minimo)
			minimo = (*it2)[2];


	}
	return minimo;
}
bool Glass::AllStartsAtTheSameHeightInTheStrip(GlassRectangle &Rect)
{
	std::list< GlassRsol > ::reverse_iterator  it;
	for (it = G_Rsol_Items.rbegin(); it != G_Rsol_Items.rend() && (*it).plateId() == cont_plates; it++)
	{
		if ((*it).y() < Rect.Getpos_y())
			return true;
		if ((*it).y() > Rect.Getpos_y())
			return false;
	}
	return true;
}
int  Glass::TallestInTheStrip(std::vector< std::vector <int> > ::iterator  &it, int toright)
{
	if (it == G_Best_Positions_Items.end()) return (*it)[4];
	int maximo = (*it)[4];
	std::vector< std::vector <int> > ::iterator  it2;
	for (it2 = it + 1; it2 != G_Best_Positions_Items.end(); it2++)
	{
		if ((*it2)[5] != (*it)[5]) //different plate
			return maximo;
		if ((*it2)[3] >= toright) //not in the horizontal strip
			return maximo;
		if ((*it2)[2] >= (*it)[4]) //not in the horizontal strip
			return maximo;
		if ((*it2)[4] >= (*it)[4]) maximo = (*it2)[4];

	}
	return maximo;
}
int  Glass::NothingUp(std::vector< std::vector <int> > ::iterator  &it, int toright)
{
	if (it == G_Best_Positions_Items.end()) return -1;
	int maximo = -1;
	std::vector< std::vector <int> > ::iterator  it2;
	for (it2 = it + 1; it2 != G_Best_Positions_Items.end(); it2++)
	{
		if ((*it2)[5] != (*it)[5]) //different plate
			return maximo;
		if ((*it2)[3] > toright) //not in the horizontal strip
			return maximo;
		if ((*it2)[2] >= (*it)[4])
			return (*it2)[2];

	}
	return maximo;
}
int  Glass::NothingUp(std::vector< std::vector <int> > ::iterator  &it, int from, int tosize)
{
	if (it == G_Best_Positions_Items.end()) return -1;
	int maximo = -1;
	std::vector< std::vector <int> > ::iterator  it2;
	for (it2 = it + 1; it2 != G_Best_Positions_Items.end(); it2++)
	{
		if ((*it2)[5] != (*it)[5]) //different plate
			return maximo;
		if ((*it2)[2] >= (*it)[4]) //not in the horizontal strip
			return (*it2)[2];
		if ((*it2)[3] > tosize) //out of the strip
			return maximo;

		if ((*it2)[4] > maximo) maximo = (*it2)[4];
	}
	return maximo;
}
//NothingLeft(it, from_vertical_strip) == true)
//Look up into the list an item down of this
bool Glass::NothingLeft(std::vector< std::vector <int> > ::iterator  &it, int min)
{
	if (it == G_Best_Positions_Items.begin()) return true;
	bool nada = true;
	std::vector< std::vector <int> > ::iterator  it2;
	for (it2 = it - 1; ; it2--)
	{

		//if starts above it
		if (((*it2)[5] != (*it)[5]) || ((*it2)[1] < min) || ((*it2)[3] > (*it)[1]) || ((*it2)[2] >= (*it)[4]) || ((*it2)[4] <= (*it)[2]))
		{
			if (it2 == G_Best_Positions_Items.begin())
				return nada;
			continue;
		}

		return false;
	}
	return true;
}
void Glass::WriteSolution(string filep)
{
	if (!G_Dibujar)
		return;
	printf("Aqui 11\n");

	//Open a file to write  
	FILE *fin3;
	int last_plate = G_Best_Rsol_Items.back().plateId() + 1;
	int defects = 0;
	int plate_defectos = 0;
	if (G_Rotate == false)
	{
		for (int i = 0; i < last_plate; i++)
		{
			if (DefectsPlate[i].size() > 0)
				plate_defectos++;
			defects += DefectsPlate[i].size();
		}
	}
	else
	{
		int hasta = (NumberOfPlates - last_plate + 1);
		for (int ki = NumberOfPlates; ki >= hasta; ki--)
		{
			if (DefectsPlate[ki].size() > 0)
				plate_defectos++;
			defects += DefectsPlate[ki].size();
		}
	}
	fin3 = fopen(filep.c_str(), "w+");
	if (fin3 == NULL)
	{
		printf("Problemas al leer el archivo temp2.txt, no se encuentra el archivo");
		exit(4);
	}
	fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", plate_w, plate_h, last_plate, G_Best_Rsol_Items.size(), defects, plate_defectos, Objective_function, Best_Width);

	if (G_Rotate == false)
	{
		for (int i = 0; i < last_plate; i++)
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
		for (int i = 0; i < last_plate; i++)
		{
			if (DefectsPlate[i].size() > 0)
				fprintf(fin3, "%d\t%d\n", i, DefectsPlate[i].size());
		}
	}
	else
	{
		int hasta = (NumberOfPlates - last_plate + 1);
		for (int i = NumberOfPlates; i >= hasta; i--)
		{

			for (std::list< GlassDefect > ::iterator it = DefectsPlate[i].begin(); it != DefectsPlate[i].end(); it++)
			{
				// Get defect coordinates.
				int defect_x = plate_w - (*it).Getpos_x() - (*it).Getwidth();
				int defect_y = plate_h - (*it).Getpos_y() - (*it).Getheight();
				int defect_width = (*it).Getwidth();
				int defect_height = (*it).Getheight();

				fprintf(fin3, "%d\t%d\t%d\t%d\t%d\n", defect_x, defect_y, defect_width, defect_height, NumberOfPlates - i);
			}

		}
		for (int i = NumberOfPlates; i >= hasta; i--)
		{
			if (DefectsPlate[i].size() > 0)
				fprintf(fin3, "%d\t%d\n", NumberOfPlates - i, DefectsPlate[i].size());
		}

	}
	for (std::list< GlassRsol > ::iterator it = G_Best_Rsol_Items.begin(); it != G_Best_Rsol_Items.end(); it++)
	{

		fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it).iditem(), items[(*it).iditem()].Getitem_stack(), items[(*it).iditem()].Getitem_seq(), (*it).x(), (*it).y(), (*it).X(), (*it).Y(), (*it).plateId() + 1);
	}


	fclose(fin3);
}
void Glass::WritePlates()
{


	//Open a file to write  
	FILE *fin3;

	fin3 = fopen(name_instance.c_str(), "w+");
	if (fin3 == NULL)
	{
		printf("Problemas al leer el archivo De la solución de la instancia, no se encuentra el archivo");
		exit(4);
	}

	for (std::list< GlassPlate > ::iterator it = G_Plate_Sol.begin(); it != G_Plate_Sol.end(); it++)
	{

		fprintf(fin3, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", (*it).Getplate_id(), (*it).Getnode_id(), (*it).Getpos_x(), (*it).Getpos_y(), (*it).Getwidth(), (*it).Getheight(), (*it).Gettype(), (*it).Getcut(), (*it).GetParent_Id());
	}



	fclose(fin3);
}
//Look up into the list an item down of this
bool Glass::NothingDown(std::list< GlassRsol >::iterator  &it)
{
	if (it == G_Rsol_Items.begin()) return true;
	bool nada = true;
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2--;
	for (; ; it2--)
	{

		//if starts above it
		if (((*it2).plateId() != (*it).plateId()) || ((*it2).Y() > (*it).y()) || ((*it2).x() >= (*it).X()) || ((*it2).X() <= (*it).x()))
		{
			if (it2 == G_Rsol_Items.begin())
				return nada;
			continue;
		}

		return false;
	}
	return true;
}
int  Glass::NextRight(std::list< GlassRsol > ::iterator  &it, int toright)
{
	if (it == G_Best_Rsol_Items.end()) return -1;
	int maximo = -1;
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return maximo;
		if ((*it2).X() > toright) //not in the horizontal strip
			return maximo;
		if ((*it2).y() >= (*it).Y()) //not in the horizontal strip
			return maximo;
		if ((*it2).x() >= (*it).X()) //out of the strip
			return (*it2).x();

	}
	return maximo;
}
int  Glass::NextRight(std::list< GlassRsol > ::iterator  &it, int toright, int toup)
{
	if (it == G_Best_Rsol_Items.end()) return -1;
	int maximo = -1;
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return maximo;
		if ((*it2).X() > toright) //not in the horizontal strip
			return maximo;
		if ((*it2).y() >= toup) //not in the horizontal strip
			return maximo;
		if ((*it2).x() >= (*it).X()) //out of the strip
			return (*it2).x();

	}
	return maximo;
}
int  Glass::NextRightCurrent(std::list< GlassRsol > ::iterator  &it, int toright)
{
	if (it == G_Rsol_Items.end()) return -1;
	int maximo = -1;
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return maximo;
		if ((*it2).X() > toright) //not in the horizontal strip
			return maximo;
		if ((*it2).y() >= (*it).Y()) //not in the horizontal strip
			return maximo;
		if ((*it2).x() >= (*it).X()) //out of the strip
			return (*it2).x();

	}
	return maximo;
}
int Glass::TallestBeginning(std::list< GlassRsol > ::iterator  &it, int toright, int tall)
{
	if (it == G_Best_Rsol_Items.end()) return -1;
	int max = (*it).y();


	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return max;
		if ((*it2).y() >= tall)
			return max;
		if ((*it2).X() > toright) //not in the horizontal strip
			return max;
		if ((*it2).y() > max)
		{
			max = (*it2).y();

		}


	}
	return max;

}
int Glass::BottomBeginning(std::list< GlassRsol > ::iterator  &it, int toright, int tall)
{
	if (it == G_Best_Rsol_Items.end()) return -1;
	int min = (*it).y();


	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return min;
		if ((*it2).y() >= tall)
			return min;
		if ((*it2).X() > toright) //not in the horizontal strip
			return min;
		if ((*it2).y() < min)
		{
			min = (*it2).y();

		}


	}
	return min;

}
//Less top strip
int Glass::LessTopStrip(std::list< GlassRsol > ::iterator  &it, int toright)
{
	if (it == G_Best_Rsol_Items.end()) return -1;
	int minimo = (*it).Y();//change 040119
						   //	int minimo = (*it).Y();//change 040119
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return minimo;
		if ((*it2).X() > toright) //not in the horizontal strip
			return minimo;
		if ((*it2).Y() < minimo)
			minimo = (*it2).Y();


	}
	return minimo;
}
int  Glass::NextRightBottom(std::list< GlassRsol > ::iterator  &it, int toright)
{
	if (it == G_Best_Rsol_Items.end()) return -1;
	int minimo = (*it).y();//change 040119
//	int minimo = (*it).Y();//change 040119
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return minimo;
		if ((*it2).X() > toright) //not in the horizontal strip
			return minimo;
		if ((*it2).y() < minimo)
			minimo = (*it2).y();


	}
	return minimo;
}
bool   Glass::NotOther(std::list< GlassRsol > ::iterator  &it, int fin, int inicio, int toright)
{
	if (it == G_Best_Rsol_Items.end()) return true;
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return true;
		if ((*it2).X() > toright) //not in the horizontal strip
			return true;
		if ((*it2).y() < fin && (*it2).Y()>inicio) //not in the horizontal strip
			return false;

	}
	return true;

}
int  Glass::TallestInTheStrip(std::list< GlassRsol > ::iterator  &it, int toright)
{
	if (it == G_Best_Rsol_Items.end()) return (*it).Y();
	int maximo = (*it).Y();
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return maximo;
		if ((*it2).X() > toright || (*it2).x() < (*it).X()) //not in the horizontal strip
			return maximo;
		if ((*it2).y() >= (*it).Y() && NotOther(it, (*it).Y(), (*it2).y(), toright) == false) //not in the horizontal strip
			return (*it2).Y();
		if ((*it2).y() > (*it).y() && (*it2).y() <(*it).Y())
			return (*it2).Y();
		if ((*it2).Y() >= maximo)
			maximo = (*it2).Y();

	}
	return maximo;
}
int  Glass::SecondTallestInTheStrip(std::list< GlassRsol > ::iterator  &it, int toright, int tall)
{
	if (it == G_Best_Rsol_Items.end()) return (*it).Y();
	int maximo = -1;
	if ((*it).Y() != tall)
		maximo = (*it).Y();
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return maximo;
		if ((*it2).X() > toright || (*it2).x() < (*it).X()) //not in the horizontal strip
			return maximo;
		if ((*it2).y() >= (*it).Y() && NotOther(it, (*it).Y(), (*it2).y(), toright) == false) //not in the horizontal strip
			return (*it2).Y();
		if ((*it2).y() > (*it).y() && (*it2).y() <(*it).Y())
			return (*it2).Y();
		if ((*it2).Y() >= maximo && (*it2).Y()!=tall )
			maximo = (*it2).Y();

	}
	return maximo;
}

int  Glass::BottomInTheStripNext(std::list< GlassRsol > ::iterator  &it, int toright)
{
	if (it == G_Best_Rsol_Items.end()) return plate_h;
	int minimo = plate_h;
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return minimo;
		if ((*it2).X() > toright) //not in the horizontal strip
			return minimo;
		if ((*it2).y() >= (*it).Y() && NotOther(it, (*it).Y(), (*it2).y(), toright) == true)
		{
			if ((*it2).y() < minimo)
				minimo = (*it2).y();
		}
		//	if ((*it2).Getcut()<=2 && (*it2).y() >= (*it).Y() && NotOther(it, (*it).Y(), (*it2).y(), toright) == true) //not in the horizontal strip
		//	return (*it2).y();

	}
	return minimo;
}
int  Glass::NothingUp(std::list< GlassRsol > ::iterator  &it, int toright)
{
	if (it == G_Best_Rsol_Items.end()) return -1;
	int maximo = -1;
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return maximo;
		if ((*it2).X() > toright) //not in the horizontal strip
			return maximo;
		if ((*it2).y() >= (*it).Y())
			return (*it2).y();

	}
	return maximo;
}
int  Glass::NothingUp(std::list< GlassRsol > ::iterator  &it, int toright, int totop)
{
	if (it == G_Best_Rsol_Items.end()) return -1;
	int maximo = -1;
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return maximo;
		if ((*it2).y() >= totop)
			return maximo;
		if ((*it2).X() > toright) //not in the horizontal strip
			return maximo;
		if ((*it2).y() >= (*it).Y())
			return (*it2).y();

	}
	return maximo;
}
//NothingLeft(it, from_vertical_strip) == true)
//Look up into the list an item down of this
bool Glass::NothingLeft(std::list< GlassRsol > ::iterator  &it, int min)
{
	if (it == G_Best_Rsol_Items.begin()) return true;
	bool nada = true;
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2--;
	for (; ; it2--)
	{

		//if starts above it
		if (((*it).plateId() != (*it).plateId()) || ((*it2).x() < min) || ((*it2).X() > (*it).x()) || ((*it2).y() >= (*it).Y()) || ((*it2).Y() <= (*it).y()))
		{
			if (it2 == G_Best_Rsol_Items.begin())
				return nada;
			continue;
		}

		return false;
	}
	return true;
}
bool Glass::NoItemBetweenTallMin(std::list< GlassRsol > ::iterator  &it, int toright, int tall)
{
	if (it == G_Best_Rsol_Items.end()) return true;



	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it2).plateId() != (*it).plateId()) //different plate
			return true;
		if ((*it2).X() > toright) //not in the horizontal strip
			return true;
		if ((*it2).y() > tall && (*it2).y() < tall + waste_min)
			return false;
	}
	return true;
}
int  Glass::NextRightTop(std::list< GlassRsol > ::iterator  &it, int toright)
{
	if (it == G_Best_Rsol_Items.end()) return -1;
	int maximo = (*it).y();
	std::list< GlassRsol > ::iterator  it2;
	it2 = it;
	it2++;
	for (; it2 != G_Best_Rsol_Items.end(); it2++)
	{
		if ((*it).plateId() != (*it2).plateId()) //different plate
			return maximo;
		if ((*it2).x() < (*it).X())
			return maximo;
		if ((*it2).X() > toright) //not in the horizontal strip
			return maximo;
		if ((*it2).Y() > maximo)
			maximo = (*it2).Y();


	}
	return maximo;
}


//We are not using this functions

std::pair<int, int > Glass::ItemGreaterRNAreaV(GlassRectangle &R)
{
	bool horizontal = false;
	int maximum2 = 0;
	int maximum = 0;
	int max_item = -1;
	int Y = R.Get_y();
	int multiplier = 1;
	if (R.Getsidecut() >= 3 && R.Get_y() != R.Getpos_y())
	{
		//We have to place to the top then we have to ask about this.
		Y = R.Getpos_Y();
		multiplier = 0;

	}



	std::pair<int, int > pk(-1, 0);
	for (register int i = 0; i < stack_nbr; i++)
	{
		//If there are items in this stacks
		if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
		{
			//If can not fii in that hole
			if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > R.Get_w() ||
				items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > R.Get_h())
				continue;
			else
			{
				int max_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
				int min_dim_item = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();

				//both measures are possible
				if (max_dim_item <= R.Get_w() &&
					max_dim_item <= R.Get_h())
				{
					if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() > maximum)
						|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() == maximum && min_dim_item > maximum2))
					{
						if (Overlap(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item, Y + (multiplier)* max_dim_item, R) == false
							&& Wastemin(R.Get_x(), Y - (1 - multiplier)*max_dim_item, R.Get_x() + min_dim_item, Y + (multiplier)* max_dim_item, R) == true)
						{
							max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
							maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area();
							maximum2 = min_dim_item;
							horizontal = false;
						}
						else
						{
							if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() > maximum)
								|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() == maximum && max_dim_item > maximum2))
							{
								if (Overlap(R.Get_x(), Y - (1 - multiplier)* min_dim_item, R.Get_x() + max_dim_item, Y + (multiplier)* min_dim_item, R) == false
									&& Wastemin(R.Get_x(), Y - (1 - multiplier)* min_dim_item, R.Get_x() + max_dim_item, Y + (multiplier)* min_dim_item, R) == true)
								{
									max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
									maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area();
									maximum2 = max_dim_item;
									horizontal = true;
								}
							}
						}
					}

				}
				else
				{
					//one measure is possible
					if (max_dim_item <= R.Get_w())
					{
						if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() > maximum)
							|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() == maximum && min_dim_item > maximum2))
						{
							if (Overlap(R.Get_x(), Y - (1 - multiplier)* min_dim_item, R.Get_x() + max_dim_item, Y + (multiplier)* min_dim_item, R) == false
								&& Wastemin(R.Get_x(), Y - (1 - multiplier)* min_dim_item, R.Get_x() + max_dim_item, Y + (multiplier)* min_dim_item, R) == true)
							{
								max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
								maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area();
								maximum2 = min_dim_item;
								horizontal = true;
							}
						}
					}
					else
					{
						if (max_dim_item <= R.Get_h())
						{

							if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() > maximum)
								|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() == maximum && max_dim_item > maximum2))
							{
								if (Overlap(R.Get_x(), Y - (1 - multiplier)* max_dim_item, R.Get_x() + min_dim_item, Y + (multiplier)* max_dim_item, R) == false
									&& Wastemin(R.Get_x(), Y - (1 - multiplier)* max_dim_item, R.Get_x() + min_dim_item, Y + (multiplier)* max_dim_item, R) == true)
								{
									max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
									maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area();
									maximum2 = max_dim_item;
									horizontal = false;
								}
							}
						}
					}
				}
			}
		}

	}
	pk.first = max_item;
	pk.second = horizontal;
	return pk;
}



std::pair<int, int > Glass::ItemGreaterRNV(GlassRectangle &R)
{
	bool horizontal = false;
	int maximum2 = 0;
	int maximum = 0;
	int max_item = -1;


	std::pair<int, int > pk(-1, 0);
	for (register int i = 0; i < stack_nbr; i++)
	{
		//If there are items in this stacks
		if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
		{
			//If can not fii in that hole
			if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > R.Get_w() ||
				items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > R.Get_h())
				continue;
			else
			{
				//both measures are possible
				if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_w() &&
					items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_h())
				{
					if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum)
						|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum2))
					{
						if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == false
							&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == true)
						{
							max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
							maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
							maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
							horizontal = false;
						}
						else
						{
							if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum)
								|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum2))
							{
								if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == false
									&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == true)
								{
									max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
									maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
									maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
									horizontal = true;
								}
							}
						}
					}

				}
				else
				{
					//one measure is possible
					if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_w())
					{
						if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum)
							|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum2))
						{
							if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == false
								&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == true)
							{
								max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
								maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
								maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
								horizontal = true;
							}
						}
					}
					else
					{
						if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_h())
						{

							if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum)
								|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum2))
							{
								if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == false
									&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == true)
								{
									max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
									maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
									maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
									horizontal = false;
								}
							}
						}
					}
				}
			}
		}

	}
	pk.first = max_item;
	pk.second = horizontal;
	return pk;
}
std::pair<int, int > Glass::ItemGreaterRNAreaH(GlassRectangle &R)
{
	bool horizontal = true;
	int maximum = 0;
	int maximum2 = 0;
	int max_item = -1;
	std::pair<int, int > pk(-1, 0);
	for (register int i = 0; i < stack_nbr; i++)
	{
		//If there are items in this stacks
		if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
		{
			//If can not fii in that hole
			if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > R.Get_w() ||
				items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > R.Get_h())
				continue;
			else
			{
				//both measures are possible
				if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_w() &&
					items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_h())
				{
					if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() > maximum)
						|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum2))
					{

						if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == false
							&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == true)
						{
							max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
							maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area();
							maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
							horizontal = true;
						}
						else
						{
							if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() > maximum)
								|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum2))
							{
								if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == false
									&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == true)

								{
									max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
									maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area();
									maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
									horizontal = false;
								}
							}
						}

					}

				}
				else
				{
					if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_w())
					{
						if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() > maximum)
							|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum2))
						{
							if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == false
								&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == true)
							{
								max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
								maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area();
								maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
								horizontal = true;
							}
						}
					}
					else
					{
						if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_h())
						{
							if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() > maximum)
								|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum2))
							{
								if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == false
									&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == true)
								{
									max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
									maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_area();
									maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
									horizontal = false;
								}
							}
						}
					}
				}
			}
		}

	}
	pk.first = max_item;
	pk.second = horizontal;
	return pk;
}

std::pair<int, int > Glass::ItemGreaterRNH(GlassRectangle &R)
{
	bool horizontal = true;
	int maximum = 0;
	int maximum2 = 0;
	int max_item = -1;
	std::pair<int, int > pk(-1, 0);
	for (register int i = 0; i < stack_nbr; i++)
	{
		//If there are items in this stacks
		if (G_Vector_Stacks[i] < (G_Vector_Size_Stacks[i]))
		{
			//If can not fii in that hole
			if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > R.Get_w() ||
				items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > R.Get_h())
				continue;
			else
			{
				//both measures are possible
				if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_w() &&
					items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_h())
				{
					if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum)
						|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum2))
					{

						if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == false
							&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == true)
						{
							max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
							maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
							maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
							horizontal = true;
						}
						else
						{
							if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum)
								|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum2))
							{
								if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == false
									&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == true)

								{
									max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
									maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
									maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
									horizontal = false;
								}
							}
						}

					}

				}
				else
				{
					if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_w())
					{
						if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum)
							|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum2))
						{
							if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == false
								&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R) == true)
							{
								max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
								maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
								maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
								horizontal = true;
							}
						}
					}
					else
					{
						if (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() <= R.Get_h())
						{
							if ((items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() > maximum)
								|| (items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min() == maximum && items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max() > maximum2))
							{
								if (Overlap(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == false
									&& Wastemin(R.Get_x(), R.Get_y(), R.Get_x() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min(), R.Get_y() + items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max(), R) == true)
								{
									max_item = G_Matrix_Items[i][G_Vector_Stacks[i]];
									maximum = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_min();
									maximum2 = items[G_Matrix_Items[i][G_Vector_Stacks[i]]].Getitem_max();
									horizontal = false;
								}
							}
						}
					}
				}
			}
		}

	}
	pk.first = max_item;
	pk.second = horizontal;
	return pk;
}
void Glass::CrearVectorOrdenado(int how )
{
//	G_Vector_Ordenado_Altura.clear();
//	G_Vector_Ordenado_Altura_Simple.clear();
	//es con el doble de piezas
	//Tenemos que crear un vector de piezas ordenados
	for (int i = 0; i < batch_items; i++)
	{
		Item_orden p;
		p.rotado = false; p.anchura = items[i].Getitem_w(); p.altura = items[i].Getitem_h(); p.item_id = i;
		G_Vector_Ordenado_Altura.push_back(p);
		Item_orden p1;
		p1.rotado = true; p1.anchura = items[i].Getitem_h(); p1.altura = items[i].Getitem_w(); p1.item_id = i;
		G_Vector_Ordenado_Altura.push_back(p1);

	}
	std::sort(G_Vector_Ordenado_Altura.begin(), G_Vector_Ordenado_Altura.end());
//	printf("Vector_ordenado1\n");
	//Tenemos que crear un vector de piezas ordenados
	for (int i = 0; i < batch_items; i++)
	{
//		printf("%d i", i);
		Item_orden p2;
		p2.rotado = false; p2.anchura = items[i].Getitem_w(); p2.altura = items[i].Getitem_min(); p2.item_id = i;
//		printf("%d i", i);
		G_Vector_Ordenado_Altura_Simple.push_back(p2);

	}
//	printf("Vector_ordenado2\n");
	std::sort(G_Vector_Ordenado_Altura_Simple.begin(), G_Vector_Ordenado_Altura_Simple.end());
//	printf("Vector_ordenado\n");
}