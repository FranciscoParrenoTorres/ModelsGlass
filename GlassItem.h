#ifndef GLASSITEM_H
#define GLASSITEM_H

#include <stdio.h>

#include <algorithm>
#include "list"

using namespace std;

/* This class is created
 * to model a batch item instance with its all attributes 
**/
class GlassItem
{
    public:
        GlassItem();
        virtual ~GlassItem();
        int Getitem_id() { return item_id; }
		int  Getitem_w(bool h) { return (h==true) ? (max(item_w,item_h)): (min(item_w, item_h)); }
		int  Getitem_h(bool h) { return (h==true) ? (min(item_w, item_h)) : (max(item_w, item_h));}
        int  Getitem_w() { return item_w; }
        int  Getitem_h() { return item_h; }
        int  Getitem_stack() { return item_stack; }
        int  Getitem_seq() { return item_seq; }
		int  Getitem_num() { return item_num; }
        int  Getitem_value() { return item_value; }
		inline int  Getitem_area() { return (item_h*item_w); }
		inline int  Getitem_max() { return (item_w > item_h)?item_w:item_h; }
		inline int  Getitem_min() { return (item_w > item_h)?item_h:item_w; }
		void Setitem_id(int val) { item_id = val; }
        void Setitem_w(int  val) { item_w = val; }
        void Setitem_h(int  val) { item_h = val; }
        void Setitem_stack(int  val) { item_stack = val; }
        void Setitem_seq(int  val) { item_seq = val; }
		void Setitem_num(int  val) { item_num = val; }
        void Setitem_value(int  val) { item_value = val; }
		void Add_num() { item_num++; }
		bool item_compare(const GlassItem &GA, const GlassItem &GB);
		void Print(bool a=false) { if (a==true) printf("Pieza %d Stack %d w %d h %d\n", item_id,item_stack,item_h,item_w); };
		
    protected:
    private:
        int item_id; // Item's Id.
        int  item_w; // Item's width.
        int  item_h; // Item's height.
        int  item_stack; // Item's stack Id
        int  item_seq; // Item's stack sequence.
		int  item_num;//Item's num copies.
        int item_value; //Item's value
		
};

#endif // GLASSITEM_H
