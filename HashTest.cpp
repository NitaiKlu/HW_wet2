#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "UnionFind.h"
#include "HashTable.h"

#define THEWALL() (cout<<"-----------------------------"<<endl)
#define TEST(i, message) (cout<< "TEST " << (i) <<" TESTING "<<#message<< endl) 


int main(int argc, const char **argv)
	{
        TEST(1.1, insert);
		HashTable<int> table;
        for (int i = 0; i < 10; i++)
        {
            table.insert(i, i);
        }
        table.printHashTable();
        THEWALL();

        TEST(1.2, find);
        for (int i = 0; i < 20; i++)
        {
            int res;
            if(table.isExist(i))
               res = table.find(i);
            else {
                res = NOT_EXIST;
            }
            cout<< res << endl;
        }
        cout<<"EXPECTED: 0 THROUGH 9 ONLY" <<endl;
        THEWALL();

        TEST(1.3, collisions);
        table.insert(10,10); //suppose to be in place 10 of the array
        table.insert(51,51); //also gets into place 10. we expect it to be in place 33
        table.insert(33,33); //suppose to be in place 33, we expect it to be in place 37
        table.printHashTable();
        THEWALL();

        TEST(1.4, remove);
        table.remove(0); //success
        cout<<"////WITHOUT 0:///"<<endl;
        table.printHashTable();  
        table.remove(2); //success
        cout<<"////WITHOUT 2:///"<<endl;
        table.printHashTable();
        table.remove(10);
        cout<<"////WITHOUT 10:///"<<endl;
        table.printHashTable(); 
        for (int i = 0; i < 52; i++)
        {
            table.remove(i);
        }
        cout<<"////empty:///"<<endl;
        table.printHashTable(); 

        TEST(2.1, reHashing);
        for (int i = 0; i < 42; i++)
        {
            int complicated = (i*(i+10)) % 79;
            table.insert(complicated, complicated);
        }
        table.printHashTable();
        THEWALL();
        for(int i=0; i<10;i++) //exceeding the maximum size :O oh shitttttt
        {
            int complicated = (i*(i+10)) % 79;
            table.insert(complicated, complicated);
        }
        table.printHashTable();
		return 0;
	}