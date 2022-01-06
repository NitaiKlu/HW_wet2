#ifndef LTREE_H_
#define LTREE_H_

#include "RTree.h"
#include "Level.h"

#define EXTRA_RANKS 2

class LevelTree
{
private:
    int scale;
    RTree<Level_ptr> tree;
    void checkBounds(int score) const;

public:
    LevelTree(int scale);
    LevelTree(const LevelTree &copy) = delete;
    ~LevelTree() = default;

    bool isEmpty() const;
    int getTreeSize() const;
    bool isExist(int key_to_find) const;
    Level_ptr &getData(int key_to_find) const;
    Level_ptr &insert(int key, const Level_ptr &data);
    void remove(int key);
    void clearAll();

    //"normal rank"- size=1- rank_index=0
    int rank(int key) const;
    int select(int rank) const;
    //int selectFromBelow(int upper_key, int rank) const;
    //int selectFromAbove(int lower_key, int rank) const;

    //"sum rank"- size=sum{sizes}- rank_index=scale
    int sumRank(int key) const;
    int sumSelect(int rank) const;
    //int sumSelectFromBelow(int upper_key, int rank) const;
    //int sumSelectFromAbove(int lower_key, int rank) const;

    //"product rank"- size=sum*level- rank_index=scale+1
    int prodRank(int key) const;
    int prodSelect(int rank) const;
    //int prodSelectFromBelow(int upper_key, int rank) const;
    //int prodSelectFromAbove(int lower_key, int rank) const;

    //"score(ind) rank"- size=(#score=ind)- rank_index=score
    int rankAtScore(int key, int score) const;

    void increaseScore(int key_to_update, int score);
    void decreaseScore(int key_to_update, int score);  

    // these two return a key
    int searchFromBelow(int upper_key) const;
    int searchFromAbove(int lower_key) const;

    void printNodeRanks(int key) const;

    //////Available ONLY FOR TESTING!//////
    int getSizeAt(int key, int ind) const;
    int getWeightAt(int key, int ind) const;
    ///////////////////////////////////////
};

#endif // LTREE_H_