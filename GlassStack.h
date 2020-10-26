#ifndef GLASSSTACK_H
#define GLASSSTACK_H

#include "GlassItem.h"

/* This class is created
 * to model a batch stack instance with its all attributes
**/
class GlassStack
{
    public:
        GlassStack();
        virtual ~GlassStack();
        int  Getstack_id() { return stack_id; }
        int  Getitem_nbr() { return item_nbr; }
        int  Getcur_item_idx() { return cur_item_idx; }
        GlassItem Getitem (int  a) { return item[a]; }

        void Setstack_id(int  val) { stack_id = val; }
        void Setitem_nbr(int  val) { item_nbr = val; }
        void Setcur_item_idx(int  val) { cur_item_idx = val; }
        void Setitem (GlassItem i) { item[idx] = i; idx++; }

        void AllocateItems () { item = new GlassItem[item_nbr]; }
        void Increaseitem_nbr () { item_nbr++; }
        void Increasecur_item_idx () { if (cur_item_idx >= item_nbr) {cur_item_idx = 0xffff;} else { cur_item_idx++; } }

    public:
        GlassItem *item; // Items list.
    protected:
    private:
        int  stack_id; // Stack Id.
        int  item_nbr;  // Stack's associated items number.
        int  cur_item_idx; // Current items index of stack instance.
        int  idx; // Used to add item on top of items list of stack instance.
};

#endif // GLASSSTACK_H
