#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "UnionFind.h"
#include "HashTable.h"
#include "LevelTree.h"

#define THEWALL() (cout << "-----------------------------" << endl)
#define TEST(i, message) (cout << "TEST " << (i) << " TESTING " << #message << endl)

int main(int argc, const char **argv)
{
    Level_ptr ptr;
    TEST(1, "Init, Insert, increase");
    int num_of_nodes = 4, scale = 3;
    LevelTree t1(scale);
    for (int i = 0; i < num_of_nodes; i++)
    {
        t1.insert(2 * i, ptr);
    }
    for (int i = 0; i < num_of_nodes; i++)
    {
        int key = 2 * i;
        //increasing rank #1 of all the levels by two
        t1.increaseScore(key, 1);
        t1.increaseScore(key, 1);
        //increasing rank #2 of all the levels by one
        t1.increaseScore(key, 2);
    }
    for (int i = 0; i < num_of_nodes; i++)
    {
        t1.printNodeRanks(i*2);
    }
    THEWALL();

    TEST(2, "Boundaries");
    for (int i = 0; i < num_of_nodes; i++)
    {
        int key = 2 * i;
        try
        {
            cout << "Rank: " << t1.rank(key);
            cout << " sumRank: " << t1.sumRank(key);
            cout << " prodRank: " << t1.prodRank(key);
            cout << " rank at 0: " << t1.rankAtScore(key, 0);
            cout << " rank at scale+1: " << t1.rankAtScore(key, scale+1);
            cout << " rank at scale+10: " << t1.rankAtScore(key, scale+10);
        }
        catch (const std::exception &e)
        {
            cout << "EXCEPTION! -" << e.what() << endl;
        }
    }
    return 0;
}