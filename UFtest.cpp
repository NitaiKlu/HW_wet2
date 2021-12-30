#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "UnionFind.h"

int main(int argc, const char **argv)
	{
		UnionFind<int> uf(10);
        uf.printUnionFind();
        for (int i = 0; i < 10; i++)
        {
            uf.MakeSet(i, 5);
        }
        uf.printUnionFind();

        for (int i = 0; i < 5; i++)
        {
            uf.Union(i, 5);
            uf.printUnionFind();
        }

        for (int i = 0; i < 10; i++)
        {
            cout << "The root of "<< i <<" is: " << uf.Find(i) << endl;
        }
        
        UnionFind<int> uf2(10);
        for (int i = 0; i < 10; i++)
        {
            uf2.MakeSet(i, i);
        }
        cout<< "UF created:" << endl;
        uf2.printUnionFind();
        uf2.Union(0, 2);
        uf2.Union(1, 3);
        uf2.Union(4, 6);
        uf2.Union(5, 7);
        uf2.Union(8, 9);
        uf2.Union(0, 1);
        uf2.Union(4, 5);
        uf2.Union(4, 9);
        uf2.Union(4, 0);    
        cout<< "After some uniting:" << endl;            
        uf2.printUnionFind();
        cout << "The root of 3 is: " << uf2.Find(3) << endl;
        uf2.printUnionFind();

		return 0;
	}