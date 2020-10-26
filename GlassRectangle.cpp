#include "GlassRectangle.h"

#include "main.h"
Glass* GlassRectangle::m_Glass = NULL;

void GlassRectangle::Print() {
	if (m_Glass->silent==false)
	printf("x1 %d y1 %d x2 %d y2 %d w %d h %d Defects %d Cut %d \n", rectangle_x, rectangle_y, rectangle_x + rectangle_w, rectangle_y + rectangle_h, rectangle_w, rectangle_h, rectangle_defect_list.size(), rectangle_scut);
};
GlassRectangle::GlassRectangle(Glass *p)
{
	m_Glass = p;
    //ctor
}
void GlassRectangle::PrintSolution()
{
	//ctor
}
GlassRectangle::GlassRectangle(int id, Coordinates &xy, GlassRectangle &R, Glass *p)
{
	m_Glass = p;
	//ctor
//	if (R.Getmin()>0 || R.Getpos_y()==3210)
//	rectangle_scut = R.Getsidecut() ;
	//change to >= 24072018
	if (R.Getsidecut() == 2 ||  R.Getmin() >= 0)
//	if (R.Getsidecut() == 2 )
			rectangle_scut = R.Getsidecut() + 1;
	else
		rectangle_scut = R.Getsidecut();

	rectangle_id = R.GetRectangle_id() + 1;
	rectangle_id = R.GetRectangle_id()+1;
	rectangle_x = xy.first.first;
	rectangle_y = xy.first.second;
	rectangle_w = xy.second.first- xy.first.first;
	rectangle_h = xy.second.second-xy.first.second;
	R_w = rectangle_w;
	R_h = rectangle_h;
	pos_x = rectangle_x;
	pos_y = rectangle_y;
	rectangle_minx_defect = rectangle_x + rectangle_w;
	rectangle_miny_defect = rectangle_y + rectangle_h;
	//CAmbiado 08012019
//	InsertDefects(R.Getdefectlist());
	if (m_Glass->G_Rotate == false)
		InsertDefects(m_Glass->DefectsPlate[m_Glass->NumberOfPlate]);
	else
		InsertDefectsRotate(m_Glass->DefectsPlate[m_Glass->NumberOfPlate]);
	rectangle_nd = rectangle_defect_list.size();
	ComputeLowest();
	parent = 0;
	
}
//GlassRectangle::GlassRectangle(int id, int w, int h, std::list< GlassDefect >  & lista)
GlassRectangle::GlassRectangle(int id, int w, int h, std::list< GlassDefect > &lista, Glass *p, int ini)
{
	m_Glass = p;
	//ctor
	rectangle_id = id;
	rectangle_x = ini;
	rectangle_y = 0;
	pos_x = rectangle_x;
	pos_y = rectangle_y;
	rectangle_w = w-ini;
	rectangle_h = h;
	R_w = rectangle_w;
	R_h = rectangle_h;
	rectangle_scut = 1;
	
	rectangle_minx_defect = rectangle_x;
	rectangle_miny_defect = rectangle_y;
	if (p->G_Rotate==false)
	InsertDefects(lista);
	else
		InsertDefectsRotate(lista);
	rectangle_nd = rectangle_defect_list.size();
	
	//Compute the coordinate of the closest defect
	ComputeLowest();	
	parent = 0;

}
GlassRectangle::GlassRectangle(int id, int x, int y, int w, int h, int side, int d, Glass *p)
{
	m_Glass = p;
	//ctor
	rectangle_id = id;
	rectangle_x = x;
	rectangle_y = y;
	pos_x = rectangle_x;
	pos_y = rectangle_y;
	rectangle_w = w;
	rectangle_h = h;
	R_w = rectangle_w;
	R_h = rectangle_h;
	rectangle_scut = side;
	rectangle_nd = d;
	rectangle_minx_defect = rectangle_x;
	rectangle_miny_defect = rectangle_y;
	parent = 0;
}
GlassRectangle::~GlassRectangle()
{
    //dtor
}
/*GlassRectangle::GlassRectangle(int id,GlassPlate &Plate)
{
	rectangle_id = id;
	rectangle_x = 0;
	rectangle_y = 0;
	rectangle_w = Plate.Getpos_x();
	rectangle_h = Plate.Getpos_y();
	rectangle_scut = 1;
	rectangle_nd = Plate.Getdefect_nbr();
	rectangle_minx_defect = rectangle_w;
	rectangle_miny_defect = rectangle_h;
}
*/
/*
void GlassRectangle::ChangeDimensionsNextDefect()
{
	std::list< GlassDefect >::iterator it;
	for (it = rectangle_defect_list.begin(); it != rectangle_defect_list.end(); it++)
	{
		if (((*it).Getpos_x() - rectangle_x) <= ((*it).Getpos_y() - rectangle_y))
		{
			if ((*it).GetUsedx() == false)
			{
				pos_x = (*it).Getpos_x();
				return;
			}
			if ((*it).GetUsedy() == false)
			{
				pos_y = (*it).Getpos_y();
				return;
			}

		}
		else
		{
			if ((*it).GetUsedy() == false)
			{
				pos_y = (*it).Getpos_y();
				return;
			}
			if ((*it).GetUsedx() == false)
			{
				pos_x = (*it).Getpos_x();
				return;
			}
		}

	}
}*/
//Change de dimensions of the rectangle until the finish of the defect
void GlassRectangle::ChangeDimensionsNextDefect()
{
	//variable to say where cut
	bool Intox = false;
	//We are going to cut this place
	if (rectangle_minx_defect - rectangle_x < rectangle_miny_defect - rectangle_y)
		Intox = true;
	

	int cont = 0;
	int cont_max = 0;
	std::list< GlassDefect >::iterator it;
	for (it = rectangle_defect_list.begin(); it != rectangle_defect_list.end() ; it++,cont++)
	{
		if (Intox == true && (*it).Getpos_x() == rectangle_minx_defect)
		{
			int  prev_x = rectangle_x;
			rectangle_x = (*it).Getpos_x() + (*it).Getwidth();
			rectangle_w = rectangle_w - (rectangle_x-prev_x);
			cont_max = cont;
		}
		if (Intox == false && (*it).Getpos_y() == rectangle_miny_defect)
		{
			int  prev_y = rectangle_y;
			rectangle_y = (*it).Getpos_y() + (*it).Getheight();
			rectangle_h = rectangle_h - (rectangle_y - prev_y);
			cont_max = cont;
		}

	}
	cont = 0;
	for (it = rectangle_defect_list.begin(); it != rectangle_defect_list.end() && cont<=cont_max; it++,cont++)
	{
		if (cont == cont_max)
		{
			rectangle_defect_list.erase(it);
			break;
		}
	}
	ComputeLowest();
	
}
void GlassRectangle::ComputeLowest()
{
	if (rectangle_defect_list.size() == 0)
		return;
	rectangle_minx_defect = MAXIMUM_INT_ROADEF;
	rectangle_miny_defect = MAXIMUM_INT_ROADEF;
	std::list< GlassDefect >:: iterator it;
	for (it = rectangle_defect_list.begin(); it != rectangle_defect_list.end(); it++)
	{
		//horizontal
		if (rectangle_scut != 2)
		{
			if ((*it).Getpos_x() < rectangle_minx_defect )
			{
				rectangle_minx_defect = max((*it).Getpos_x(),m_Glass->waste_min);
				rectangle_miny_defect = max((*it).Getpos_y(), m_Glass->waste_min);
			}
		}
		else
		{
			if ((*it).Getpos_y() < rectangle_miny_defect)
			{
				rectangle_minx_defect = max((*it).Getpos_x(), m_Glass->waste_min);
				rectangle_miny_defect = max((*it).Getpos_y(), m_Glass->waste_min);
			}
		}

	}
}
void GlassRectangle::InsertDefect(GlassDefect &D)
{
	std::list< GlassDefect >::iterator it;
	if (rectangle_defect_list.size() <= 0)
	{
		rectangle_defect_list.push_back(D);
		return;
	}

	int dis = m_Glass->Distancia(D, rectangle_x, rectangle_y);
	for (it = rectangle_defect_list.begin(); it != rectangle_defect_list.end(); it++)
	{
		if (dis < m_Glass->Distancia((*it), rectangle_x, rectangle_y))
		{
			rectangle_defect_list.insert(it, D);
			return;
		}
	}
	rectangle_defect_list.push_back(D);
}

void GlassRectangle::InsertDefectN(GlassDefect D)
{
	std::list< GlassDefect >::iterator it;
	if (rectangle_defect_list.size() <= 0)
	{
		rectangle_defect_list.push_back(D);
		return;
	}

	int dis = m_Glass->Distancia(D, rectangle_x, rectangle_y);
	for (it = rectangle_defect_list.begin(); it != rectangle_defect_list.end(); it++)
	{
		if (dis < m_Glass->Distancia((*it), rectangle_x, rectangle_y))
		{
			rectangle_defect_list.insert(it, D);
			return;
		}
	}
	rectangle_defect_list.push_back(D);
}

void GlassRectangle::ModifyDefects()
{

	rectangle_minx_defect = MAXIMUM_INT_ROADEF;
	rectangle_miny_defect = MAXIMUM_INT_ROADEF;
	if (rectangle_defect_list.size() == 0)
	{

		return;
	}
		

	std::list< GlassDefect >::iterator it;
	it = rectangle_defect_list.begin();
//	for (it = rectangle_defect_list.begin(); it != rectangle_defect_list.end(); it++)
	do
	{
		//left - bottom corner of the waste inside the rectangle
		if (((*it).Getpos_x() >= rectangle_x && (*it).Getpos_x() < (rectangle_x + rectangle_w))
			&& ((*it).Getpos_y() >= rectangle_y && (*it).Getpos_y() < (rectangle_y + rectangle_h)))
		{
			
			if (max((*it).Getpos_x(), rectangle_x)  < rectangle_minx_defect)
				rectangle_minx_defect = max((*it).Getpos_x(), rectangle_x);
			if (max((*it).Getpos_y(), rectangle_y) < rectangle_miny_defect)
				rectangle_miny_defect = max((*it).Getpos_y(), rectangle_y);
			(*it).SetUsed(false);
			it++;
			continue;
		}
		//right - top corner of the waste inside the rectangle
		if ((((*it).Getpos_x() + (*it).Getwidth()) > rectangle_x && ((*it).Getpos_x() + (*it).Getwidth()) <= (rectangle_x + rectangle_w))
			&& (((*it).Getpos_y() + (*it).Getheight()) > rectangle_y && ((*it).Getpos_y() + (*it).Getheight()) <= (rectangle_y + rectangle_h)))
		{
			
			if (max((*it).Getpos_x(), rectangle_x)  < rectangle_minx_defect)
				rectangle_minx_defect = max((*it).Getpos_x(), rectangle_x);
			if (max((*it).Getpos_y(), rectangle_y) < rectangle_miny_defect)
				rectangle_miny_defect = max((*it).Getpos_y(), rectangle_y);
			(*it).SetUsed(false);
			it++;
			continue;
		}
		//right - bottom corner of the waste inside the rectangle
		if ((((*it).Getpos_x() + (*it).Getwidth()) > rectangle_x && ((*it).Getpos_x() + (*it).Getwidth()) <= (rectangle_x + rectangle_w))
			&& ((*it).Getpos_y() >= rectangle_y && (*it).Getpos_y() < (rectangle_y + rectangle_h)))
		{
			
			if (max((*it).Getpos_x(), rectangle_x)  < rectangle_minx_defect)
				rectangle_minx_defect = max((*it).Getpos_x(), rectangle_x);
			if (max((*it).Getpos_y(), rectangle_y) < rectangle_miny_defect)
				rectangle_miny_defect = max((*it).Getpos_y(), rectangle_y);
			(*it).SetUsed(false);
			it++;

			continue;
		}
		//left- top corner of the waste inside the rectangle
		if (((*it).Getpos_x() >= rectangle_x && (*it).Getpos_x() < (rectangle_x + rectangle_w))
			&& (((*it).Getpos_y() + (*it).Getheight()) > rectangle_y && ((*it).Getpos_y() + (*it).Getheight()) <= (rectangle_y + rectangle_h)))
		{
			
			if (max((*it).Getpos_x(), rectangle_x)  < rectangle_minx_defect)
				rectangle_minx_defect = max((*it).Getpos_x(), rectangle_x);
			if (max((*it).Getpos_y(), rectangle_y) < rectangle_miny_defect)
				rectangle_miny_defect = max((*it).Getpos_y(), rectangle_y);
			(*it).SetUsed(false);
			it++;
			continue;
		}
		it = rectangle_defect_list.erase(it);
	} while (it != rectangle_defect_list.end());

}

///  TO  DO Change to R_h , R_w
void GlassRectangle::InsertDefects(std::list< GlassDefect >  & lista2)
{
	if (lista2.size() == 0) return;

	
	for (std::list< GlassDefect >::iterator it = lista2.begin(); it != lista2.end(); it++)
	{
		//left - bottom corner of the waste inside the rectangle
		if (((*it).Getpos_x() >= rectangle_x && (*it).Getpos_x() < (rectangle_x + R_w))
			&& ((*it).Getpos_y() >= rectangle_y && (*it).Getpos_y() < (rectangle_y + R_h)))
		{
			(*it).SetUsed(false);
			InsertDefect((*it));
			continue;
		}
		//right - top corner of the waste inside the rectangle
		if ((((*it).Getpos_x() + (*it).Getwidth()) > rectangle_x && ((*it).Getpos_x() + (*it).Getwidth()) <= (rectangle_x + R_w))
			&& (((*it).Getpos_y() + (*it).Getheight()) > rectangle_y && ((*it).Getpos_y() + (*it).Getheight()) <= (rectangle_y + R_h)))
		{
			(*it).SetUsed(false);
			InsertDefect((*it));
			continue;
		}
		//right - bottom corner of the waste inside the rectangle
		if ((((*it).Getpos_x() + (*it).Getwidth()) > rectangle_x && ((*it).Getpos_x() + (*it).Getwidth()) <= (rectangle_x + R_w))
			&& ((*it).Getpos_y() >= rectangle_y && (*it).Getpos_y() < (rectangle_y + R_h)))
		{
			(*it).SetUsed(false);
			InsertDefect((*it));
			continue;
		}
		//left- top corner of the waste inside the rectangle
		if (((*it).Getpos_x() >= rectangle_x && (*it).Getpos_x() < (rectangle_x + R_w))
			&& (((*it).Getpos_y() + (*it).Getheight()) > rectangle_y && ((*it).Getpos_y() + (*it).Getheight()) <= (rectangle_y + R_h)))
		{
			(*it).SetUsed(false);
			InsertDefect((*it));
			continue;
		}
	}
}
void GlassRectangle::InsertDefectsRotate(std::list< GlassDefect >  & lista)
{
	if (lista.size() == 0) return;
	std::list< GlassDefect >::iterator it;

	for (it = lista.begin(); it != lista.end(); it++)
	{
		int x = m_Glass->plate_w -(*it).Getpos_x()- (*it).Getwidth();
		int y= m_Glass->plate_h - (*it).Getpos_y() - (*it).Getheight();
		GlassDefect newD = (*it);
		newD.Setpos_x(x);
		newD.Setpos_y(y);
		newD.SetUsed(false);
		//left - bottom corner of the waste inside the rectangle
		if ((x >= rectangle_x && x < (rectangle_x + R_w))
			&& (y >= rectangle_y && y < (rectangle_y + R_h)))
		{
			(*it).SetUsed(false);
			InsertDefectN(newD);
			continue;
		}
		//right - top corner of the waste inside the rectangle
		if (((x + (*it).Getwidth()) > rectangle_x && (x + (*it).Getwidth()) <= (rectangle_x + R_w))
			&& ((y + (*it).Getheight()) > rectangle_y && (y + (*it).Getheight()) <= (rectangle_y + R_h)))
		{
			(*it).SetUsed(false);
			InsertDefectN(newD);
			continue;
		}
		//right - bottom corner of the waste inside the rectangle
		if (((x + (*it).Getwidth()) > rectangle_x && (x + (*it).Getwidth()) <= (rectangle_x + R_w))
			&& (y >= rectangle_y && y < (rectangle_y + R_h)))
		{
			(*it).SetUsed(false);
			InsertDefectN(newD);
			continue;
		}
		//left- top corner of the waste inside the rectangle
		if ((x >= rectangle_x && x < (rectangle_x + R_w))
			&& ((y + (*it).Getheight()) > rectangle_y && (y + (*it).Getheight()) <= (rectangle_y + R_h)))
		{
			(*it).SetUsed(false);
			InsertDefectN(newD);
			continue;
		}
	}
}
/*
std::pair< int, int > GlassRectangle::NextDefect()
{
	for (std::list< GlassDefect >::iterator it = rectangle_defect_list.begin(); it != rectangle_defect_list.end(); it++)
	{
		if ((*it).Getpos_x()<min_x && (*it).Getpos_x()>=min_x
	}
}*/
//Find the following rectangle, if type 1 to the X, if type 2 to the Y
bool GlassRectangle::FindTheFollowingDefect(int x, int type)
{
	if (rectangle_defect_list.size() == 0)
		return false;
	bool change_defect = false;
	int max_width = 0;
	rectangle_minx_defect = MAXIMUM_INT_ROADEF;
	rectangle_miny_defect = MAXIMUM_INT_ROADEF;
	std::list< GlassDefect >::iterator it2 = rectangle_defect_list.begin();
	for (std::list< GlassDefect >::iterator it = rectangle_defect_list.begin(); it != rectangle_defect_list.end(); it++)
	{
		if ((*it).GetUsedx() == true && (*it).GetUsedy() == true)
			continue;
		if (type == 1)
		{
			if ((*it).Getpos_x() <= x)
			{
				if ((*it).Getwidth() <= max_width)
					continue;
				it2 = it;
				max_width = (*it).Getwidth();
				change_defect = true;

				pos_x =max( (*it).Getpos_x() + (*it).Getwidth(),rectangle_x+m_Glass->waste_min);
				if (pos_x > rectangle_x + R_w)
					rectangle_w = 0;
				else
					//change the height of the rectangle
					rectangle_w = rectangle_x + R_w - pos_x;

			}
			else
			{
				if ((*it).Getpos_x() >= x && (*it).Getpos_x() < rectangle_minx_defect)
				{
					rectangle_minx_defect = (*it).Getpos_x();
					rectangle_miny_defect = (*it).Getpos_y();

				}
			}

		}
		else
		{
			if ((*it).Getpos_y() <= x)
			{
				if ((*it).Getheight() <= max_width)
					continue;
				it2 = it;
				max_width = (*it).Getheight();
				change_defect = true;

				pos_y = max((*it).Getpos_y() + (*it).Getheight(),rectangle_y+m_Glass->waste_min);
				if (pos_y > rectangle_y + R_h)
					rectangle_h = 0;
				else
					//change the height of the rectangle
					rectangle_h = rectangle_y + R_h - pos_y;
			}
			else
			{
				if ((*it).Getpos_y() >= x && (*it).Getpos_y() < rectangle_miny_defect)
				{
					rectangle_minx_defect = (*it).Getpos_x();
					rectangle_miny_defect = (*it).Getpos_y();

				}
			}
		}
	}
	if (change_defect==true)
		(*it2).SetUsed(true);
	return change_defect;
}
bool GlassRectangle::EverythingOK(int a)
{
	if (m_Glass->active_log_error == false)	return false;
	bool wrong = false;
	if ((max(rectangle_x, pos_x) + rectangle_w > m_Glass->plate_w))
		wrong = true;
	if (max(rectangle_y, pos_y) + rectangle_h > m_Glass->plate_h)
		wrong = true;
	if	(max(rectangle_x, pos_x) + rectangle_w >a) 
		wrong = true;
	if 	(rectangle_w>0 && (max(rectangle_x, pos_x) + rectangle_w)!= (rectangle_x + R_w)) 
		wrong = true;
	if (rectangle_h> 0 && (max(rectangle_y, pos_y) + rectangle_h) != (rectangle_y + R_h))
		wrong = true;
	if (wrong == true)
	{
		printf("Algo mal en el rectángulo x %d y %d pos_x %d pos_y %d w %d h %d  R_w %d R_h %d IterVeces %d GlobalIter %d\n", rectangle_x, rectangle_y, pos_x, pos_y, rectangle_w, rectangle_h, R_w, R_h, m_Glass->G_iter_veces, m_Glass->m_id);
		return true;
		if (m_Glass->active_log_error)
		{
			printf("Algo mal en el rectángulo x %d y %d pos_x %d pos_y %d w %d h %d  R_w %d R_h %d IterVeces %d GlobalIter %d\n", rectangle_x, rectangle_y, pos_x, pos_y, rectangle_w, rectangle_h, R_w, R_h, m_Glass->G_iter_veces, m_Glass->m_id);
			return true;
//			scanf("%d", a);
		}
	}
	else 
		return false;
	

}
// Change the position where can be a rectangle
bool GlassRectangle::ChangePositionToPlaceNearest(int type)
{
	if (rectangle_defect_list.size() == 0) return false;
	//&	if (deterministic==false)
	for (std::list< GlassDefect >::iterator it = rectangle_defect_list.begin(); it != rectangle_defect_list.end(); it++)
	{
		if ((*it).GetUsedx() == true && (*it).GetUsedy() == true)
			continue;
		//get_random(0,3)!=1
		if ((m_Glass->deterministic==false && (((*it).Getpos_x() - rectangle_x) <= (((*it).Getpos_y() - rectangle_y) + m_Glass->get_random(0,400)- m_Glass->get_random(0,400)))) ||
		(m_Glass->deterministic==true && (((*it).Getpos_x() - rectangle_x) <= (((*it).Getpos_y() - rectangle_y) ))))
//		if (((*it).Getpos_x() - rectangle_x) <= (((*it).Getpos_y() - rectangle_y))) //V4
		{
			if ((*it).GetUsedx() == false)
			{
				if (((*it).Getpos_x() + (*it).Getwidth()) >= (rectangle_x + R_w))
				{
					(*it).SetUsedx(true);
					continue;
				}
				pos_x = min((*it).Getpos_x() + (*it).Getwidth(), rectangle_x + R_w);
				pos_y = BuscarPositionY(pos_x, (*it).Getpos_y());
				//At least waste min
				if (pos_x>rectangle_x)
				pos_x = max(rectangle_x + m_Glass->waste_min, pos_x);
				//At least waste min change 05092018
				if (pos_y>rectangle_y)
				pos_y = max(rectangle_y + m_Glass->waste_min, pos_y);
				if (pos_y > rectangle_y + R_h)
					pos_y = rectangle_y + R_h;
				if (pos_x > rectangle_x + R_w)
					pos_x = rectangle_x + R_w;
				(*it).SetUsedx(true);
				if (R_w > (pos_x - rectangle_x))
					rectangle_w = R_w - (pos_x - rectangle_x);
				else
					rectangle_w = 0;
				if (R_h > (pos_y - rectangle_y))
					rectangle_h = R_h - (pos_y - rectangle_y);
				else
					rectangle_h = 0;

				return true;
			}
			else
			{
				pos_y = min((*it).Getpos_y() + (*it).Getheight(), rectangle_y + R_h);
				pos_x = BuscarPositionX((*it).Getpos_x(), pos_y);
				(*it).SetUsedy(true);
				//At least waste min
				if (pos_y < rectangle_y + R_h)
				{
					if (pos_y>rectangle_y)
					pos_y = max(rectangle_y + m_Glass->waste_min, pos_y);
				}
				//At least waste min change 05092018
				if (pos_x>rectangle_x)
				pos_x = max(rectangle_x + m_Glass->waste_min, pos_x);
				if (pos_x > rectangle_x + R_w)
					pos_x = rectangle_x + R_w;
				//
				if (pos_y > rectangle_y + R_h)
					pos_y = rectangle_y + R_h;
				if (R_w > (pos_x - rectangle_x))
					rectangle_w = R_w - (pos_x - rectangle_x);
				else
					rectangle_w = 0;
				if (R_h > (pos_y - rectangle_y))
					rectangle_h = R_h - (pos_y - rectangle_y);
				else
					rectangle_h = 0;

				return true;
			}

		}
		else
		{
			if ((*it).GetUsedy() == false)
			{
				if (((*it).Getpos_y() + (*it).Getheight()) >= (rectangle_y + R_h))
				{
					(*it).SetUsedy(true);
					continue;
				}

				pos_y = min((*it).Getpos_y() + (*it).Getheight(), rectangle_y + R_h);
				pos_x = BuscarPositionX((*it).Getpos_x(), pos_y);
				//At least waste min
				if (pos_y < rectangle_y + R_h)
				{
					if (pos_y>rectangle_y)
					pos_y = max(rectangle_y + m_Glass->waste_min, pos_y);
				}
				if (pos_y > rectangle_y + R_h)
					pos_y = rectangle_y + R_h;
				//At least waste min change 05092018
				if (pos_x>rectangle_x)
				pos_x = max(rectangle_x + m_Glass->waste_min, pos_x);
				if (pos_x > rectangle_x + R_w)
					pos_x = rectangle_x + R_w;
				(*it).SetUsedy(true);
				if (R_w > (pos_x - rectangle_x))
					rectangle_w = R_w - (pos_x - rectangle_x);
				else
					rectangle_w = 0;
				if (R_h > (pos_y - rectangle_y))
					rectangle_h = R_h - (pos_y - rectangle_y);
				else
					rectangle_h = 0;

				return true;
			}
			else
			{
				pos_x = min((*it).Getpos_x() + (*it).Getwidth(), rectangle_x + R_w);
				pos_y = BuscarPositionY(pos_x, (*it).Getpos_y());
				//At least waste min
				if (pos_x < rectangle_x + R_w)
				{
					if (pos_x>rectangle_x)
					pos_x = max(rectangle_x + m_Glass->waste_min, pos_x);
				}
				if (pos_x > rectangle_x + R_w)
					pos_x = rectangle_x + R_w;
				//At least waste min change 05092018
				if (pos_y>rectangle_y)
				pos_y = max(rectangle_y + m_Glass->waste_min, pos_y);
				if (pos_y > rectangle_y + R_h)
					pos_y = rectangle_y + R_h;
				if (R_w > (pos_x - rectangle_x))
					rectangle_w = R_w - (pos_x - rectangle_x);
				else
					rectangle_w = 0;
				if (R_h > (pos_y - rectangle_y))
					rectangle_h = R_h - (pos_y - rectangle_y);
				else
					rectangle_h = 0;
				(*it).SetUsedx(true);

				return true;
			}
		}
	}
	return false;

}
//Change the position where can be a rectangle
int GlassRectangle::BuscarPositionY(int val_x,int val_y )
{
	int max_y = rectangle_y;
	if (rectangle_defect_list.size() == 0) return max_y;
	for (std::list< GlassDefect >::iterator it = rectangle_defect_list.begin(); it != rectangle_defect_list.end(); it++)
	{
		if ((*it).Getpos_y() <val_y  && (*it).GetUsedy() == true)
		{
			if ((*it).Getpos_x() > val_x && (*it).Getpos_y() > max_y)
				max_y = (*it).Getpos_y() + (*it).Getheight();
		}
	}
	return max_y;

}
int GlassRectangle::BuscarPositionX(int val_x, int val_y)
{
	int max_x = rectangle_x;
	if (rectangle_defect_list.size() == 0) return max_x;
		
	for (std::list< GlassDefect >::iterator it = rectangle_defect_list.begin(); it != rectangle_defect_list.end(); it++)
	{
		if ((*it).Getpos_x() <val_x && (*it).GetUsedx() == true)
		{
			if ((*it).Getpos_y() > val_y && (*it).Getpos_x() > max_x)
				max_x = (*it).Getpos_x() + (*it).Getwidth();
		}
	}
	return max_x;

}

std::pair<int, int > GlassRectangle::ItemGreaterR()
{


	if (rectangle_scut != 2)
	{
		//If the space is small
		if ((rectangle_minx_defect - rectangle_x)<m_Glass->MinDimensionPieza)
			return m_Glass->ItemGreater(rectangle_w, min(rectangle_miny_defect - rectangle_y, rectangle_h), rectangle_scut);
		else
		{  //it could be we have other
			std::pair<int, int > p = m_Glass->ItemGreater(min(rectangle_minx_defect - rectangle_x, rectangle_w), rectangle_h, rectangle_scut);
			if (p.first != (-1)) return p;
			return m_Glass->ItemGreater(rectangle_w, min(rectangle_miny_defect - rectangle_y, rectangle_h), rectangle_scut);
		}
	}
	else
	{
		if ((rectangle_miny_defect - rectangle_y) < m_Glass->MinDimensionPieza)
			return m_Glass->ItemGreater(min(rectangle_minx_defect - rectangle_x, rectangle_w), rectangle_h, rectangle_scut);
		else
		{
			std::pair<int, int > p = m_Glass->ItemGreater(rectangle_w, min(rectangle_miny_defect - rectangle_y, rectangle_h), rectangle_scut);
			if (p.first != (-1)) return p;
			return m_Glass->ItemGreater(min(rectangle_minx_defect - rectangle_x, rectangle_w), rectangle_h, rectangle_scut);

		}
	}
}
//bool GlassItem::item_compare(const GlassItem &GA, const GlassItem &GB)
//{
//	return (GA.Getitem_max() <GB.Getitem_max());
//}