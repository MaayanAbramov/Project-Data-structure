//
// 234218 Data Structures 1.
// Semester: 2024A (winter).
// Wet Exercise #2.
//
// Recommended TAB size to view this file: 8.
//
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
//

#ifndef OLYMPICSA2_H_
#define OLYMPICSA2_H_

#include "hash_table.h"
#include "wet2util.h"


class olympics_t {
public:
    hash_table* table;
    RankTree<Team_Key,Team*>* teams;
    void destroy_teams(RankTree<Team_Key,Team*>::Node* root);
    int medals_of_team(int teamId);
    void update_strength(int new_strength, int teamId);
    RankTree<Team_Key, Team*>::Node* find_node_in_tree(int teamId);
    RankTree<Team_Key, Team*>::Node* find_node_in_table(int teamId);
    int calc_team_strength(int TeamId);
    Player_Key* merge_arrays_keys(Player_Key* array1, int size1, Player_Key* array2, int size2);
    Player** merge_arrays_value(Player** array1, int size1, Player** array2, int size2);
public:

    // <DO-NOT-MODIFY> {

    olympics_t();

    virtual ~olympics_t();

    StatusType add_team(int teamId);

    StatusType remove_team(int teamId);

    StatusType add_player(int teamId, int playerStrength);

    StatusType remove_newest_player(int teamId);

    output_t<int> play_match(int teamId1, int teamId2);

    output_t<int> num_wins_for_team(int teamId);

    output_t<int> get_highest_ranked_team();

    StatusType unite_teams(int teamId1, int teamId2);

    output_t<int> play_tournament(int lowPower, int highPower);

    // } </DO-NOT-MODIFY>
};

#endif // OLYMPICSA2_H_
