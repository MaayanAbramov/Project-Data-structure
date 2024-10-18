//
// Created by maaya on 21/03/2024.
//
/*-------------------------- hash table ---------------*/
#include "hash_table.h"

hash_table::hash_table(): table_size(DEFAULT_LENGTH), num_of_elements(0){
    this->array = new RankTree<Team_Key,Team*>*[table_size];
    for(int i=0;i<this->table_size;i++){
        this->array[i]=new RankTree<Team_Key,Team*>();
    }
}

hash_table::~hash_table() {
    for(int i =0; i< this->table_size;i++){
        auto temp = this->array[i];
        RankTree<Team_Key,Team*>::destroy_the_tree_and_values(temp->root);
        delete temp;
        this->array[i]=nullptr;
    }
    delete[] array;
}


int hash_table::hash(const Team_Key& key) {
    int hash_num = key.id % table_size;
    return hash_num;
}


RankTree<Team_Key,Team*>::Node* hash_table::find(const Team_Key& key)  {

    int index_in_array = hash(key);
    RankTree<Team_Key,Team*>* current_tree = this->array[index_in_array];
    auto found = current_tree->find_algorithm(key);
    bool is_exist = (found != nullptr);
    if (is_exist == false) {
        return nullptr;
    }
    else {
        return found;
    }
}


void hash_table::assignObjectsToArrayTeam(typename RankTree<Team_Key,Team*>::Node* root, Team** arr, int* index) {
    if (root == nullptr) {
        return;
    }
    assignObjectsToArrayTeam(root->left, arr, index);
    assignObjectsToArrayTeam(root->right, arr, index);
    arr[*index] = root->value;
    (*index)++;
    root->value = nullptr;
}
void hash_table::assignObjectsToArrayTeamKey(typename RankTree<Team_Key,Team*>::Node* root, Team_Key* arr, int* index) {
    if (root == nullptr) {
        return;
    }
    assignObjectsToArrayTeamKey(root->left, arr, index);
    assignObjectsToArrayTeamKey(root->right, arr, index);
    arr[*index] = root->key;
    (*index)++;
}

void hash_table::changeSizeIfNeeded() {
    if (num_of_elements <= DEFAULT_LENGTH/2) {
        return;
    }
    int backupToSize = table_size;
    bool need_to_resize = false;
    if (num_of_elements >= table_size) {
        table_size = table_size * 2;
        need_to_resize = true;
    }
    else{
        if (num_of_elements <= table_size/4){
            table_size = table_size/2;
            need_to_resize = true;
        }

    }
    if ( need_to_resize == true){
        auto to_delete = this->array;
        auto updatedArray = new RankTree<Team_Key,Team*>*[table_size];
        for(int i = 0 ; i < table_size; i ++ ){
            updatedArray[i] = new RankTree<Team_Key,Team*>();
        }

        for (int i = 0 ; i < backupToSize ; i++) {
            RankTree<Team_Key,Team*>* current_tree = this->array[i];
            if (current_tree->root != nullptr)  {

                int num_of_elements_in_current_tree = current_tree->numOfNodes;
                Team_Key* key_arr = new Team_Key[num_of_elements_in_current_tree];
                Team** team_arr = new Team*[num_of_elements_in_current_tree];
                int team_array_index = 0;
                int team_key_array_index = 0;
                assignObjectsToArrayTeam(current_tree->root, team_arr, &team_array_index);
                assignObjectsToArrayTeamKey(current_tree->root, key_arr, &team_key_array_index);
                for (int j = 0 ; j < num_of_elements_in_current_tree ; j++) {
                    Team_Key key_to_insert = key_arr[j];
                    Team* team_to_insert = team_arr[j];
                    assert(team_to_insert!=nullptr);
                    assert(team_to_insert->team_id == key_to_insert.id && team_to_insert->team_strength == key_to_insert.strength);
                    RankTree<Team_Key,Team*>* tree_to_insert = updatedArray[hash(key_to_insert)];
                    tree_to_insert->insertAux(key_to_insert, team_to_insert);
                }

                delete[] key_arr;
                delete[] team_arr;
            }
        }
        for(int i = 0 ; i < backupToSize; i ++ ){
            auto temp = to_delete[i];
            RankTree<Team_Key,Team*>::destroy_the_tree_and_values(temp->root);
            delete temp;
        }
        this->array = updatedArray;
        delete[] to_delete;
    }
}



bool hash_table::insert(const Team_Key& key, Team* team ) {
    int hash_key = hash(key);
    RankTree<Team_Key,Team*>* current_tree = this->array[hash_key];
    if (find(key) != nullptr) {
        return false;
    }
    current_tree->insertAux(key,team);
    num_of_elements++;
    changeSizeIfNeeded();
    return true;

}


bool hash_table::remove(const Team_Key& key) {
    if (find(key) == nullptr) {
        return false;
    }
    int hash_key = hash(key);
    RankTree<Team_Key,Team*>* current_tree = this->array[hash_key];
    auto node = current_tree->find_algorithm(key);
    auto to_delete = node->value;
    current_tree->remove(key);
    delete to_delete;
    num_of_elements--;
    changeSizeIfNeeded();
    return true;

}
