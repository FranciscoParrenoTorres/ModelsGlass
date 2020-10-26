#include "GlassDefect.h"

GlassDefect::GlassDefect()
{
    //ctor
}

GlassDefect::GlassDefect(int  id, int  pid, int  x, int  y, int  w, int  h)
{
    defect_id = id;
    plate_id = pid;
    pos_x = x;
    pos_y = y;
    width = w;
    height = h;
	used_x = false;
	used_y = false;
}

GlassDefect::~GlassDefect()
{
    //dtor
}
