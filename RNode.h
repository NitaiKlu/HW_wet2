#ifndef RNODE_H_
#define RNODE_H_

#include "Level.h"
#include "TNode.h"

class RNode: public TNode<Level_ptr>
{
private:
    int size;
    int* weight;

public:
    RNode(int size);
    RNode(int key, const Level_ptr &data, int size, RNode *parent = nullptr);
    RNode(const RNode &copy) = delete;
    ~RNode() = default;
    /**
     * changes in functions if necessary
     * */
    const int getWeight(int i) const;
    int updateWeight(); //update the rank array
    



};

#endif //RNODE_H_