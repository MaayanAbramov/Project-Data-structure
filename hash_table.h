

#ifndef WINTER_HASH_TABLE_H
#define WINTER_HASH_TABLE_H


#include <iostream>
#include <cassert>
#include "Team.h"
#include "Team_Key.h"
#define  DEFAULT_LENGTH 16
class hash_table{

public:

     hash_table();
    ~hash_table();
    hash_table(const hash_table& other) = delete;
    int table_size;
    int num_of_elements;
    RankTree<Team_Key,Team*>** array;
    int hash(const Team_Key& key);

    RankTree<Team_Key,Team*>::Node* find(const Team_Key& key);


    void changeSizeIfNeeded();
    void assignObjectsToArrayTeamKey(typename RankTree<Team_Key,Team*>::Node* root, Team_Key* arr, int* index);
    void assignObjectsToArrayTeam(typename RankTree<Team_Key,Team*>::Node* root, Team** arr, int* index);
    bool insert(const Team_Key& key, Team* team);
    bool remove(const Team_Key& key);
};




#endif //WINTER_HASH_TABLE_H