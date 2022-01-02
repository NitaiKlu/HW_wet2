#ifndef RTREE_H_
#define RTREE_H_

#include "Level.h"
#include "Tree.h"

class RTree: public Tree<Level_ptr>
{
private:
    int rank_size;
    /**
     * rotate - should have updateWeight whenever a RNode has a new son
     * all changes of polimorphism shit?
     * internal insert (and elsewhere if needed) change TNode c'tor to RNode c'tor
     * add Rank(key, index): returns the indexish rank of key 
     * add Search(rank, index): return the key with the indexish rank (except for the last 2 ranks in the array) 
     * 
     * */
public:
    

};

#endif //RTREE_H_