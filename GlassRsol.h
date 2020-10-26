#ifndef GLASSRSOL_H
#define GLASSRSOL_H

#include <ostream>

/* This class is created
* to model a solution Rsol instance with its all attributes
**/
class GlassRsol {
public:
	GlassRsol();
	GlassRsol(int  plate, int  Rsol, int  x,
	int  y, int  w, int  h,
		int t, int  c, int  p, int  n);
	GlassRsol(int  plate, int  Rsol, int  x,
		int  y, int  w, int  h,
		int t, int  c, int  p);
	GlassRsol(int  plate, int  Rsol, int  x,
		int  y, int  w, int  h,
		int t, int  c);
	virtual ~GlassRsol();

	int  plateId() {
		return _plateId;
	}

	int  id() {
		return _id;
	}

	int  x() {
		return _x;
	}

	int  y() {
		return _y;
	}

	int  w() {
		return _w;
	}

	int  h() {
		return _h;
	}
	int  X() {
		return _X;
	}

	int  Y() {
		return _Y;
	}
	int RsolType() {
		return _RsolType;
	}

	int  Getcut() {
		return cut;
	}
	int  copiesx() {
		return _copiesx;
	}
	int  copiesy() {
		return _copiesy;
	}
	int  iditem() {
		return _iditem;
	}
	int  Getparent() {
		return parent;
	}

	int  Getsuccessor_nbr() {
		return successor_nbr;
	}



	


	void plateId(int  id) {
		_plateId = id;
	}

	void SetRsol_id(int  id) {
		_id = id;
	}

	void x(int  x) {
		_x = x;
	}

	void y(int  y) {
		_y = y;
	}

	void w(int  w) {
		_w = w;
	}
	void iditem(int  y) {
		_iditem = y;
	}
	void copiesx(int  y) {
		_copiesx = y;
	}
	void copiesy(int  w) {
		_copiesy = w;
	}
	void h(int  h) {
		_h = h;
	}
	void X(int  w) {
		_X = w;
	}

	void Y(int  h) {
		_Y = h;
	}
	void RsolType(int t) {
		_RsolType = t;
	}

	void Setcut(int  c) {
		cut = c;
	}

	void Setparent(int  p) {
		parent = p;
	}

	void Setchild_nbr(int  nbr) {
		successor_nbr = nbr;
	}



	friend std::ostream & operator<<(std::ostream& os, GlassRsol Rsol) {
		os << "Rsol (p,id,x,y,w,h,t,s) = (" << Rsol._plateId << "," << Rsol._id << "," << Rsol._x << "," << Rsol._y << "," << Rsol._w << "," << Rsol._h << "," << Rsol._RsolType << "," << Rsol.cut << ")";
		return os;
	}


protected:
private:
	int  _plateId; // Plate Id.
	int  _id; // Rsol Id.
	int  _iditem; // Rsol Id.
	int  _x; // x position.
	int  _y; // y position.
	int  _w; // Plate's width.
	int  _h; // Plate's height.
	int  _X; // Plate's width.
	int  _Y; // Plate's height.
	int  _copiesx; // Plate's width.
	int  _copiesy; // Plate's height.

	int _RsolType; /* Rsol type
				   * > 0 glass piece index in batch file.
				   * = -1 wasted glass.
				   * = -2 branch.
				   * = -3 residual.
				   */
	int  cut; // cut G_level (there are 1, 2, 3 and 4-cut G_level, could not be 0-cut because GlassRsol class instance can't be an entire plate).
	int  parent; // parent Rsol Id.
	int  successor_nbr; // number of G_children for this Rsol instance.
};

#endif // GLASSRSOL_H
