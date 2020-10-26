#ifndef GLASSPLATE_H
#define GLASSPLATE_H

#include "GlassNode.h"
#include "GlassDefect.h"
#include "GlassRectangle.h"
#pragma warning(disable: 4786)

#include <stdlib.h>
#include <vector>
/* This class is created
 * to model a solution used plate instance with its all attributes
**/
 class GlassRectangle;

class GlassPlate
{
    public:
        GlassPlate();
		GlassPlate(int, GlassRectangle &, int, int  *);
		GlassPlate(int nodoant, std::vector<int> &vecp, int nplate, int  *nodos, int ntype, int  ncut, int);

		GlassPlate(GlassRectangle & R,int , int  * );
        virtual ~GlassPlate();
        int  Getplate_id() { return plate_id; }
        int  Getnode_id() { return node_id; }
        int  Getpos_x() { return pos_x; }
        int  Getpos_y() { return pos_y; }
        int  Getwidth() { return width; }
        int  Getheight() { return height; }
        int Gettype() { return type; }
        int  Getcut() { return cut; }
        int  Getnodes_nbr () { return nodes_nbr; }
        int  Getwaste() { return waste; }
        int  Getuseful() { return useful; }
        GlassNode Getsuccessor (int  index) { return successor[index]; }
        GlassDefect Getdefect (int  index) { return defect[index]; }
        int  Getsuccessor_nbr () { return successor_nbr; }
        int  Getdefect_nbr () { return defect_nbr; }
        GlassNode Getresidual () { return residual; }
		int GetParent_Id() { return parent_id; }

        void Setplate_id(int  val) { plate_id = val; }
        void Setnode_id(int  val) { node_id = val; }
        void Setpos_x(int  val) { pos_x = val; }
        void Setpos_y(int  val) { pos_y = val; }
        void Setwidth (int  w) { width = w; }
        void Setheight (int  h) { height = h; }
        void Settype(int val) { type = val; }
        void Setcut(int  val) { cut = val; }
        void Setnodes_nbr(int  val) { nodes_nbr = val; }
        void Setwaste(int  val) { waste = val; }
        void Setuseful(int  val) { useful = val; }
        void Setsuccessor (GlassNode c) { successor[successor_nbr] = c; successor_nbr++; }
        void Setdefect (GlassDefect c) { defect[defect_nbr] = c; defect_nbr++; }
        void Setresidual (GlassNode r) { residual = r; }
		void Setparent_id(int a) { parent_id=a; }
    public:
        GlassNode *successor; // Successors list.
        GlassNode residual; // Residual node (only if last used plate).
        GlassDefect *defect; // Defects list of plate instance.
    protected:
    private:
        int  plate_id; // Plate Id.
        int  node_id; // Node Id.
        int  pos_x; // x position.
        int  pos_y; // y position.
        int  width; // Plate's width.
        int  height; // Plate's height.
        int type;   /* node type
                     * > 0 glass piece index in batch file
                     * = -1 wasted glass
                     * = -2 branch
                     * = -3 residual
                     * Note: since this is plate class, plate's instance type should be -2 (branch).
                     */
        int  cut; // Cut stage (Note: since this is plate class, plate's instance cut G_level should be 0-cut).
        int  nodes_nbr; // Number of all nodes on a plate instance.
        int  waste; // Sum of Wasted area.
        int  useful; // Sum of Used area.
        int  successor_nbr; // Plate's successor nodes number (G_children).
        int  defect_nbr; // Defects number.
		int  node_parent;
		int  parent_id;
};

#endif // GLASSPLATE_H
