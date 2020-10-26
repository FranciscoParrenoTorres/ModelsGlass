#ifndef GLASSNODE_H
#define GLASSNODE_H

#include <ostream>

/* This class is created
* to model a solution node instance with its all attributes
**/
class GlassNode {
public:
	GlassNode();
	GlassNode(int  plate, int  node, int  x,
		int  y, int  w, int  h,
		int t, int  c, int  p, int  *c_array, int  n);
	virtual ~GlassNode();

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

	int nodeType() {
		return _nodeType;
	}

	int  Getcut() {
		return cut;
	}

	int  Getparent() {
		return parent;
	}

	int  Getsuccessor_nbr() {
		return successor_nbr;
	}

	GlassNode Getsuccessor(int  idx) {
		return successor[idx];
	}

	void plateId(int  id) {
		_plateId = id;
	}

	void Setnode_id(int  id) {
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

	void h(int  h) {
		_h = h;
	}

	void nodeType(int t) {
		_nodeType = t;
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

	int  Setsuccessor(GlassNode c) {
		if (successor_nbr >= 2 && cut > 2) {
			return 1;
		}
		successor[successor_nbr] = c;
		successor_nbr++;
		return 0;
	}

	friend std::ostream & operator<<(std::ostream& os, GlassNode node) {
		os << "Node (p,id,x,y,w,h,t,s) = (" << node._plateId << "," << node._id << "," << node._x << "," << node._y << "," << node._w << "," << node._h << "," << node._nodeType << "," << node.cut << ")";
		return os;
	}

public:
	GlassNode * successor; // Successor nodes list.
protected:
private:
	int  _plateId; // Plate Id.
	int  _id; // Node Id.
	int  _x; // x position.
	int  _y; // y position.
	int  _w; // Plate's width.
	int  _h; // Plate's height.
	int _nodeType; /* node type
				   * > 0 glass piece index in batch file.
				   * = -1 wasted glass.
				   * = -2 branch.
				   * = -3 residual.
				   */
	int  cut; // cut G_level (there are 1, 2, 3 and 4-cut G_level, could not be 0-cut because GlassNode class instance can't be an entire plate).
	int  parent; // parent node Id.
	int  successor_nbr; // number of G_children for this node instance.
};

#endif // GLASSNODE_H
