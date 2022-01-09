#include "LevelTree.h"

LevelTree::LevelTree(int scale)
    : scale(scale), tree(scale + EXTRA_RANKS) {}

void LevelTree::checkBounds(int score) const
{
    if (score <= 0 || score > scale)
        throw std::out_of_range("Out of scale");
}

RTree<Level_ptr>::const_iterator LevelTree::begin() const
{
    return tree.begin();
}
RTree<Level_ptr>::const_iterator LevelTree::reverseBegin() const
{
    return tree.reverseBegin();
}
RTree<Level_ptr>::const_iterator LevelTree::end() const
{
    return tree.end();
}

bool LevelTree::isEmpty() const
{
    return tree.isEmpty();
}
int LevelTree::getTreeSize() const
{
    return tree.getTreeSize();
}
bool LevelTree::isExist(int key_to_find) const
{
    return tree.isExist(key_to_find);
}
Level_ptr &LevelTree::getData(int key_to_find) const
{
    return tree.getData(key_to_find);
}

int LevelTree::getHighestLevel() const
{
    return tree.getHighestLevel();
}

Level_ptr &LevelTree::insert(int key, const Level_ptr &data)
{
    return tree.insert(key, data);
}
void LevelTree::remove(int key)
{
    tree.remove(key);
}
void LevelTree::clearAll()
{
    tree.clearAll();
}

void LevelTree::ArrayToTree(RNode<Level_ptr> **array, int start, int end)
{
    tree.ArrayToTree(array, start, end);
}
    

//"normal rank"- size=1- rank_index=0
int LevelTree::getWeight(int key) const
{
    return tree.getWeight(0);
}
int LevelTree::rank(int key) const
{
    return tree.rank(key);
}
int LevelTree::select(int rank) const
{
    return tree.select(rank);
}
/* NEEDS TO BE IMPLEMENTED!///////////////////////////////////
int LevelTree::selectFromBelow(int upper_key, int rank) const
{
    return tree.selectFromBelow(upper_key, rank);
}
int LevelTree::selectFromAbove(int lower_key, int rank) const
{
    return tree.selectFromAbove(lower_key, rank);
}
*/

//"sum rank"- size=sum{sizes}- rank_index=scale+1
int LevelTree::getSumSize(int key) const
{
    return tree.getSizeAt(key, scale +1);
}
int LevelTree::getSumWeight(int key) const
{
    return tree.getWeightAt(key, scale +1);
}
int LevelTree::sumRank(int key) const
{
    return tree.rankAt(key, scale + 1);
}
int LevelTree::sumSelect(int rank) const
{
    return tree.selectAt(rank, scale + 1);
}
/* NEEDS TO BE IMPLEMENTED!///////////////////////////////////
int LevelTree::sumSelectFromAbove(int upper_key, int rank) const
{
    return tree.selectFromBelowAt(upper_key, rank, scale+1);
}**/

int LevelTree::sumSelectFromAbove(int rank) const
{
    return tree.selectFromAboveAt(rank, scale + 1);
}


//"product rank"- size=sum*level- rank_index=scale+2
int LevelTree::getProdSize(int key) const
{
    return tree.getSizeAt(key, scale +2);
}
int LevelTree::getProdWeight(int key) const
{
    return tree.getWeightAt(key, scale +2);
}
int LevelTree::prodRank(int key) const
{
    return tree.rankAt(key, scale + 2);
}
int LevelTree::prodSelect(int rank) const
{
    return tree.selectAt(rank, scale + 2);
}
/* NEEDS TO BE IMPLEMENTED!///////////////////////////////////
int LevelTree::prodSelectFromBelow(int upper_key, int rank) const
{
    return tree.selectFromBelowAt(upper_key, rank, scale + 2);
}
int LevelTree::prodSelectFromAbove(int lower_key, int rank) const
{
    return tree.selectFromAboveAt(lower_key, rank, scale + 2);
}
*/

//"score(ind) rank"- 0<score<=scale- rank_index=ind
int LevelTree::getSizeAt(int key, int score) const
{
    checkBounds(score);
    return tree.getSizeAt(key, score);
}
int LevelTree::getWeightAt(int key, int score) const
{
    checkBounds(score);
    return tree.getWeightAt(key, score);
}
int LevelTree::rankAtScore(int key, int score) const
{
    checkBounds(score);
    return tree.rankAt(key, score);
}

void LevelTree::increaseScore(int key_to_update, int score)
{
    checkBounds(score);
    tree.addToSize(key_to_update, score, 1);
    tree.addToSize(key_to_update, scale + 1, 1);
    tree.addToSize(key_to_update, scale + 2, key_to_update);
}
void LevelTree::decreaseScore(int key_to_update, int score)
{
    checkBounds(score);
    tree.addToSize(key_to_update, score, -1);
    tree.addToSize(key_to_update, scale + 1, -1);
    tree.addToSize(key_to_update, scale + 2, -key_to_update);
}

// these two return a key
int LevelTree::searchFromBelow(int upper_key) const
{
    return tree.searchFromBelow(upper_key);
}
int LevelTree::searchFromAbove(int lower_key) const
{
    return tree.searchFromAbove(lower_key);
}

void LevelTree::printNodeRanks(int key) const
{
    for (int score = 0; score <= scale + EXTRA_RANKS; score++)
    {
        cout << "Key=" << key << ":";
        if (score == 0)
            cout << "   norm:  ";
        else if (score == scale + 1)
            cout << "   sum:   ";
        else if (score == scale + 2)
            cout << "   prod:  ";
        else
            cout << "   score:" << score;
        tree.printNodeRank(key, score);
    }
}

