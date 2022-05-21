# HW_wet2
HomeWork assignment no. 2. Data Structures course, Technion- Israel Insititute of Technology.

Most is the implementation of the requested "game" API. 
The game.cpp is in control over the game, containing all players devided to groups and levels.
Each group has players devided to levels.
Each level within a group contains the players of the group in that level.

Implementation of several data structures are in the project:
1. **ChainHash**: a _non-finished_ chain based hash table. 
2. **HashTable**: a working open-addressed hash table. 
3. **RTree**: rank binary search tree. Each vertice contains a key value, data and a user's choice size array of "ranks". 
The RTree creates an additional N+1 rank which is sum of all ranks. 
<!-- in the game the rank of this tree used to indicate different scores of the players-->
4. **UnionFind**: a union-find data structure, array based.  
