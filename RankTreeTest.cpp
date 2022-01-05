#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "UnionFind.h"
#include "HashTable.h"
#include "RTree.h"

#define THEWALL() (cout << "-----------------------------" << endl)
#define TEST(i, message) (cout << "TEST " << (i) << " TESTING " << #message << endl)

int main(int argc, const char **argv)
{
    TEST(1, "Init and Insert");
    int num_of_nodes = 4, num_of_ranks = 3;
    RTree<int> t1(num_of_ranks);
    for (int i = 0; i < num_of_nodes; i++)
    {
        t1.insert(2 * i, 0);
    }
    for (int i = 0; i < num_of_nodes; i++)
    {
        t1.updateSize(2 * i, 0, i);
    }
    for (int i = 0; i < num_of_nodes; i++)
    {
        t1.printNodeRanks(i * 2);
    }
    THEWALL();

    TEST(2, "Boundaries");
    int j = num_of_ranks + 2;
    for (int i = 0; i < num_of_nodes; i++)
    {
        try
        {
            cout << "Key: " << 2 * i << ", Data: " << t1.getData(2 * i);
            cout << ", Size: " << t1.getSize(2 * i, j);
            cout << ", Weight: " << t1.getWeight(2 * i, j);
            cout << ", Rank at ind=" << j << ": " << t1.RankAt(2 * i, j);
            cout << ", Rank: " << t1.Rank(2 * i) << endl;
        }
        catch (const std::exception &e)
        {
            cout << endl << endl << "   EXCEPTION! ";
            std::cerr << e.what() << '\n';
            cout << endl;
        }
    }
    return 0;
}