#ifndef GLASSDEFECT_H
#define GLASSDEFECT_H

/* This class is created
 * to model a defect instance with its all attributes 
**/
class GlassDefect
{
    public:
        GlassDefect();
        GlassDefect(int  id, int  pid, int  x, int  y, int  w, int  h);
        virtual ~GlassDefect();
        int  Getdefect_id() { return defect_id; }
        int  Getplate_id() { return plate_id; }
        int  Getpos_x() { return pos_x; }
        int  Getpos_y() { return pos_y; }
        int  Getwidth() { return width; }
        int  Getheight() { return height; }
		bool GetUsedx() { return used_x; }
		bool GetUsedy() { return used_y; }


        void Setdefect_id(int  val) { defect_id = val; }
        void Setplate_id(int  val) { plate_id = val; }
        void Setpos_x(int  val) { pos_x = val; }
        void Setpos_y(int  val) { pos_y = val; }
        void Setwidth(int  val) { width = val; }
        void Setheight(int  val) { height = val; }
		void SetUsed(bool a) { used_x = a; used_y = a; }
		void SetUsedx(bool a) { used_x = a; };
		void SetUsedy(bool a) { used_y = a; };
    protected:
    private:
        int  defect_id; // Defect's Id.
        int  plate_id; // Defect's plate Id
        int  pos_x; // x position.
        int  pos_y; // y position.
        int  width; // Defect's width.
        int  height; // Defect's height.
		bool used_x; //if we have used this deffect in x
		bool used_y; //if we have used this deffect in y
};

#endif // GLASSDEFECT_H
