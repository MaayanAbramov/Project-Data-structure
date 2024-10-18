#include "olympics24a2.h"

olympics_t::olympics_t()
{
    this->table = new hash_table();
    this->teams = new RankTree<Team_Key,Team*>();

}

olympics_t::~olympics_t()
{
    delete this->table;
    if(this->teams!=nullptr) {
        destroy_teams(this->teams->root);
    }
    delete this->teams;
}


StatusType olympics_t::add_team(int teamId)
{
    Team  *t_for_table=nullptr,*t_for_teams=nullptr;
    try {
        if (teamId <= 0) {
            return StatusType::INVALID_INPUT;
        }
        Team_Key key_for_table = Team_Key(teamId,0,false);
        t_for_table = new Team(teamId);
        Team_Key key_for_teams = Team_Key(teamId,0,true);
        t_for_teams = new Team(teamId);
        if(table->find(key_for_table)!=nullptr){
            key_for_teams.strength = table->find(key_for_table)->value->team_strength;
            assert(teams->find_algorithm(key_for_teams)!=nullptr);
            delete t_for_table;
            delete t_for_teams;
            return StatusType::FAILURE;
        }
        bool res = table->insert(key_for_table,t_for_table);
        assert(res == true);
        teams->insertAux(key_for_teams,t_for_teams);
        assert(teams->find_algorithm(key_for_teams)!=nullptr);
        assert(table->find(key_for_table)!=nullptr);
        //teams->update_bubble_all_tree(teams->root);

        return StatusType::SUCCESS;
    }
    catch(...) {
        delete t_for_teams;
        delete t_for_table;
        return StatusType::ALLOCATION_ERROR;
    }
}
RankTree<Team_Key, Team*>::Node* olympics_t::find_node_in_tree(int teamId) {
    auto table_node = find_node_in_table(teamId);
    Team_Key tree_key = Team_Key(table_node->key);
    assert(table_node->key.strength ==table_node->value->team_strength );
    tree_key.strength = table_node->value->team_strength;
    tree_key.by_strength = true;
    auto node = teams->find_algorithm(tree_key);
    return node;
}
RankTree<Team_Key, Team*>::Node* olympics_t::find_node_in_table(int teamId) {
    Team_Key mock_key = Team_Key(teamId, 0, false);
    auto table_node = table->find(mock_key);
    return table_node;
}

int olympics_t::medals_of_team(int teamId) {
    auto node_in_table = find_node_in_table(teamId);
    assert(node_in_table!=nullptr);
    auto node_in_tree = find_node_in_tree(teamId);
    assert(node_in_tree != nullptr);
    //teams->update_bubble_all_tree(teams->root);
    return teams->acumulated_sum(node_in_tree);
}
int olympics_t::calc_team_strength(int teamId){
    auto node_in_table = find_node_in_table(teamId);
    assert(node_in_table!=nullptr);
    auto players_by_strength = find_node_in_tree(teamId)->value->players_by_strength;
    assert(players_by_strength!=nullptr);
    if(players_by_strength->numOfNodes == 0){
        return 0;
    }
    auto median_player = players_by_strength->select((int(floor(players_by_strength->numOfNodes/2))));
    assert(median_player != nullptr);
    int num_of_nodes_in_team = players_by_strength->numOfNodes;
    //teams->update_bubble_all_tree(teams->root);
    return (median_player->key.strength) * num_of_nodes_in_team;
}
void olympics_t::update_strength(int new_strength, int teamId) {
    Team* toAdd = nullptr;
    try {
        //backing up fields
        auto node_in_table2 = find_node_in_table(teamId);
        assert(node_in_table2!=nullptr);
        int medals = this->medals_of_team(teamId);
        auto node_in_tree = find_node_in_tree(teamId);
        int counter = node_in_tree->value->counter;
        auto players_sorted_by_strength = node_in_tree->value->players_by_strength;
        auto players_sorted_by_id = node_in_tree->value->players_by_id;
        node_in_tree->value->players_by_strength = nullptr;
        node_in_tree->value->players_by_id = nullptr;
        int old_strength = node_in_tree->key.strength;
        auto to_delete = teams->find_algorithm(Team_Key(teamId, old_strength,true))->value;
        teams->remove(Team_Key(teamId, old_strength,true));
        delete to_delete;
        Team_Key tree_key_updated = Team_Key(teamId, new_strength, true);
        toAdd = new Team(teamId, new_strength, counter, players_sorted_by_strength, players_sorted_by_id); //check where should be deleted
        teams->insertAux(tree_key_updated, toAdd);
        auto prev = teams->findprevNodeInOrder(tree_key_updated);
        if (prev != nullptr) {
            teams->add_amount(tree_key_updated, medals);
            teams->add_amount(prev->key, (-1)*medals);
        }
        else {
            teams->add_amount(tree_key_updated, medals);
        }
        auto node_in_table = find_node_in_table(teamId);
        assert(node_in_table != nullptr);
        node_in_table->key.strength = new_strength;
        node_in_table->value->team_strength = new_strength;
        //teams->update_bubble_all_tree(teams->root);
    }
    catch(...){
        delete toAdd;
        throw bad_exception();
    }

}

StatusType olympics_t::remove_team(int teamId)
{
    try {
        if (teamId <= 0) {
            return StatusType::INVALID_INPUT;
        }
        Team_Key key_for_table = Team_Key(teamId,0,false);
        auto node = table->find(key_for_table);
        if(node == nullptr){
            return StatusType::FAILURE;
        }
        assert(node->key.id == teamId);
        Team_Key key_for_teams = Team_Key(node->key.id, node->key.strength, true);
        assert(node->value != nullptr);
        assert(node->key.strength == node->value->team_strength);
        Team* to_delete =  this->find_node_in_tree(key_for_teams.id)->value;
        teams->remove(key_for_teams); //destructor of team deletes the players
        key_for_table.strength = node->value->team_strength;
        bool res = table->remove(key_for_table);
        assert(res == true);
        delete to_delete;

    }
    catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }
    //teams->update_bubble_all_tree(teams->root);
    return StatusType::SUCCESS;
}

StatusType olympics_t::add_player(int teamId, int playerStrength)
{
    try {
        if (playerStrength <= 0 || teamId <= 0) {
            return StatusType::INVALID_INPUT;
        }
        Team_Key mock_key = Team_Key(teamId, 0, false);
        auto table_node = table->find(mock_key);
        if (table_node == nullptr) {
            return StatusType::FAILURE;
        }
        Team_Key tree_key = Team_Key(table_node->key);
        assert(tree_key.strength == table_node->value->team_strength);
        tree_key.by_strength = true;
        auto node = teams->find_algorithm(tree_key);
        assert(node!=nullptr);
        bool res = node->value->addPlayer(playerStrength);
        assert(res == true);
        assert(node->value!=nullptr);
        update_strength(calc_team_strength(teamId), teamId);

    }
    catch(...) {
        return StatusType::ALLOCATION_ERROR;
    }
    //teams->update_bubble_all_tree(teams->root);
    return StatusType::SUCCESS;
}

StatusType olympics_t::remove_newest_player(int teamId)
{
    try {
        if (teamId <= 0) {
            return StatusType::INVALID_INPUT;
        }
        auto table_node = this->find_node_in_table(teamId);
        bool isFailure = (table_node == nullptr) ;//||
        if (isFailure == true) {
            return StatusType::FAILURE;
        }
        auto tree_node = find_node_in_tree(teamId);
        if(tree_node->value->counter == 0) return StatusType::FAILURE;
        auto node_player = tree_node->value->players_by_id->findMax();
        assert(node_player!=nullptr);
        assert(node_player->key.id == tree_node->value->counter-1);
        tree_node->value->removePlayer(node_player->value->player_strength, node_player->value->player_id);
        update_strength(calc_team_strength(teamId), teamId);
        //teams->update_bubble_all_tree(teams->root);
        return StatusType::SUCCESS;
    }
    catch(...) {
        return StatusType::ALLOCATION_ERROR; //if the try jumps in the middle, it jumps to catch
    }
}

output_t<int> olympics_t::play_match(int teamId1, int teamId2)
{
    if (teamId1 <= 0 || teamId2 <=0 || teamId1 == teamId2) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    try {
        auto team1_in_table = find_node_in_table(teamId1);
        auto team2_in_table = find_node_in_table(teamId2);
        if (team1_in_table == nullptr || team2_in_table == nullptr) {
            return output_t<int>(StatusType::FAILURE);
        }
        auto team1_in_tree = find_node_in_tree(teamId1);
        auto team2_in_tree = find_node_in_tree(teamId2);
        if (team1_in_tree->value->counter == 0 || team2_in_tree->value->counter == 0 ){
            return output_t<int>(StatusType::FAILURE);
        }
        assert(team1_in_tree!=nullptr && team2_in_tree!=nullptr);

        int winnning_team_id = -1;
        if (team1_in_tree->key > team2_in_tree->key ) {
            teams->add(team1_in_tree->key, team1_in_tree->key, 1);
            winnning_team_id = teamId1;
        }
        else {
            teams->add(team2_in_tree->key, team2_in_tree->key, 1);
            winnning_team_id = teamId2;
        }

        /*if (team1_in_tree->key.strength > team2_in_tree->key.strength) {
            teams->add(team1_in_tree->key, team1_in_tree->key, 1);
            winnning_team_id = teamId1;
        }
        else if (team1_in_tree->key.strength < team2_in_tree->key.strength) {
            teams->add(team2_in_tree->key, team2_in_tree->key, 1);
            winnning_team_id = teamId2;
        }
        else { //tie
            team1_in_tree->key.id < team2_in_tree->key.id ? teams->add(team1_in_tree->key, team1_in_tree->key, 1)
            : teams->add(team2_in_tree->key, team2_in_tree->key, 1);
            winnning_team_id = (team1_in_tree->key.id < team2_in_tree->key.id) ? teamId1 : teamId2;
        }*/
        assert(winnning_team_id!=-1);
        //teams->update_bubble_all_tree(teams->root);
        return output_t<int>(winnning_team_id);
    }
    catch(...) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }

}

output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    try {
        if (teamId <= 0) {
            return output_t<int>(StatusType::INVALID_INPUT);
        }
        auto team_in_table= find_node_in_table(teamId);
        if (team_in_table == nullptr) {
            return output_t<int>(StatusType::FAILURE);
        }
        auto team_in_tree = find_node_in_tree(teamId);
        assert(team_in_tree->key.id==teamId);
        //teams->update_bubble_all_tree(teams->root);
        assert(teams->acumulated_sum(team_in_tree)>=0);
        return output_t<int>(teams->acumulated_sum(team_in_tree));
    }
    catch(...) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }


}

output_t<int> olympics_t::get_highest_ranked_team()
{
    try {
        if (teams->numOfNodes == 0) {
            return output_t<int>(-1);
        }
        if(teams->root->to_bubble.getNumOfParticipants() ==0){
            return output_t<int>(0);
        }
        //teams->update_bubble_all_tree(teams->root);
        return output_t<int>(teams->root->to_bubble.getToBubble());
    }
    catch(...) {
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
}

StatusType olympics_t::unite_teams(int teamId1, int teamId2)
{
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }
    auto node_team_id1 = find_node_in_table(teamId1);
    auto node_team_id2 = find_node_in_table(teamId2);

    if (node_team_id1 == nullptr || node_team_id2 == nullptr) {
        return StatusType::FAILURE;
    }

    Team_Key key_for_removing_later = Team_Key(node_team_id2->key);
    auto node_team_id1_tree = find_node_in_tree(teamId1);
    auto node_team_id2_tree = find_node_in_tree(teamId2);
    auto team1_by_id_in_tree = node_team_id1_tree->value->players_by_id;
    auto team1_by_strength_in_tree = node_team_id1_tree->value->players_by_strength;
    auto team2_by_id_in_tree = node_team_id2_tree->value->players_by_id;
    auto team2_by_strength_in_tree = node_team_id2_tree->value->players_by_strength;
    int team1_size = team1_by_id_in_tree->numOfNodes;
    int team2_size = team2_by_id_in_tree->numOfNodes;
//    if(team2_size == 0 ){
//        this->remove_team(teamId2);
//        return StatusType::SUCCESS;
//    }
    auto team2_keys_array_id = new Player_Key[team2_by_id_in_tree->numOfNodes]();
    auto team2_value_array_id = new Player*[team2_by_id_in_tree->numOfNodes]();
    auto team1_keys_array_id = new Player_Key[team1_by_id_in_tree->numOfNodes]();
    auto team1_value_array_id = new Player*[team1_by_id_in_tree->numOfNodes]();
    auto team2_keys_array_str= new Player_Key[team2_by_id_in_tree->numOfNodes]();
    auto team2_value_array_str= new Player*[team2_by_id_in_tree->numOfNodes]();
    auto team1_keys_array_str= new Player_Key[team1_by_id_in_tree->numOfNodes]();
    auto team1_value_array_str= new Player*[team1_by_id_in_tree->numOfNodes]();
    int index1=0,index2=0,index3=0,index4=0;
    team2_by_strength_in_tree->inorderFromTreeToArray(team2_keys_array_str,team2_value_array_str,&index1);
    team2_by_id_in_tree->inorderFromTreeToArray(team2_keys_array_id,team2_value_array_id,&index2);
    team1_by_strength_in_tree->inorderFromTreeToArray(team1_keys_array_str,team1_value_array_str,&index3);
    team1_by_id_in_tree->inorderFromTreeToArray(team1_keys_array_id,team1_value_array_id,&index4);

//    if(team2_size != 0) {
//        node_team_id1_tree->value->counter++;
//    }
    for (int j = 0 ; j < team2_size ; j++) {
        team2_keys_array_id[j].id = node_team_id1_tree->value->counter;
        team2_value_array_id[j]->setPlayerId(node_team_id1_tree->value->counter);
        team2_value_array_id[j]->getTwin()->setPlayerId(node_team_id1_tree->value->counter);
        node_team_id1_tree->value->counter++;
    }
    for (int i = 0; i <team2_size; ++i) {
        team2_keys_array_str[i] = team2_value_array_str[i]->generate_key(true);

    }
    //TODO please delete the double for loop  before submission
    for (int i = 0; i <team2_size ; i++) {
        bool found = false;
        auto to_find = team2_keys_array_id[i];
        for (int j = 0; j <team2_size ; j++) {
            auto curr = team2_value_array_str[j];
            found = found || ( to_find.id ==curr->player_id && to_find.strength == curr->player_strength);
        }
        assert(found == true);
        found = false;
    }
    //todo please delete above
    auto merged_keys_id = this->merge_arrays_keys(team1_keys_array_id,team1_size,team2_keys_array_id,team2_size);
    auto merged_values_id = this->merge_arrays_value(team1_value_array_id,team1_size,team2_value_array_id,team2_size);
    auto merged_keys_str = this->merge_arrays_keys(team1_keys_array_str,team1_size,team2_keys_array_str,team2_size);
    auto merged_values_str = this->merge_arrays_value(team1_value_array_str,team1_size,team2_value_array_str,team2_size);
    auto root_str = RankTree<Player_Key,Player*>::createCompleteTree(team1_size+team2_size);
    auto root_id = RankTree<Player_Key,Player*>::createCompleteTree(team1_size+team2_size);
    int index5=0,index6=0;

    RankTree<Player_Key,Player*>::inorderFromArrayToTree(root_str, merged_keys_str, merged_values_str,&index5);
    RankTree<Player_Key,Player*>::inorderFromArrayToTree(root_id, merged_keys_id, merged_values_id,&index6);
    team1_by_id_in_tree->DeleteWholeTree(team1_by_id_in_tree->root);
    team1_by_strength_in_tree->DeleteWholeTree(team1_by_strength_in_tree->root);
    team2_by_id_in_tree->DeleteWholeTree(team1_by_id_in_tree->root);
    team2_by_strength_in_tree->DeleteWholeTree(team1_by_strength_in_tree->root);

//    node_team_id1_tree->value->players_by_id = nullptr;
//    node_team_id1_tree->value->players_by_strength = nullptr;
//
//    node_team_id1_tree->value->players_by_id = new RankTree<Player_Key, Player*>();
//    node_team_id1_tree->value->players_by_strength = new RankTree<Player_Key, Player*>();
    node_team_id1_tree->value->players_by_id->root = root_id;
    node_team_id1_tree->value->players_by_strength->root = root_str;
    node_team_id1_tree->value->players_by_strength->numOfNodes = team1_size+ team2_size;
    node_team_id1_tree->value->players_by_id->numOfNodes = team1_size+ team2_size;
//    table->remove(key_for_removing_later);

    table->remove(key_for_removing_later);
    key_for_removing_later.by_strength = true;
    key_for_removing_later.strength = node_team_id2_tree->value->team_strength;
    auto to_delete = this->teams->find_algorithm(key_for_removing_later)->value;
    to_delete->team_strength = 0;
    this->teams->remove(key_for_removing_later);
    delete to_delete;
    update_strength(calc_team_strength(teamId1),teamId1);
    //assert(this->remove_team(teamId2)==StatusType::SUCCESS);
//    delete team1_by_id_in_tree;
//    delete team1_by_strength_in_tree;
    delete[] team2_keys_array_id ;
    delete[] team2_value_array_id ;
    delete[] team1_keys_array_id ;
    delete[] team1_value_array_id ;
    delete[] team2_keys_array_str;
    delete[] team2_value_array_str;
    delete[] team1_keys_array_str;
    delete[] team1_value_array_str;
    delete[] merged_keys_id; //does not destroy each of the elements in the array
    delete[] merged_values_id;
    delete[] merged_keys_str;
    delete[] merged_values_str;
    //teams->update_bubble_all_tree(teams->root);
    //todo delete merged_*
    return StatusType::SUCCESS;
}


output_t<int> olympics_t::play_tournament(int lowPower, int highPower) {
    try {
        if (lowPower <= 0 || highPower <= 0 || highPower <= lowPower) {
            return output_t<int>( StatusType::INVALID_INPUT);
        }
        if(teams->numOfNodes == 0 ){
            return output_t<int>(StatusType::FAILURE);
        }
        auto closest_to_low_from_above = teams->next_to_low_from_above(Team_Key(INT_MAX, lowPower));
        auto closest_to_upper_from_below = teams->next_to_up_bound_from_down(Team_Key(0, highPower));
        if(closest_to_low_from_above == nullptr || closest_to_upper_from_below == nullptr ){
            return  output_t<int>(StatusType::FAILURE);
        }
        if (closest_to_low_from_above->key == closest_to_upper_from_below->key) {//test
            //teams->update_bubble_all_tree(teams->root);
            return output_t<int>(output_t<int>(closest_to_low_from_above->key.id));
        }
        assert(closest_to_low_from_above != nullptr);
        assert(closest_to_upper_from_below != nullptr);
        int order_rank_high = teams->algorithm_rank(closest_to_upper_from_below->key);

        int order_rank_low = teams->algorithm_rank(closest_to_low_from_above->key);
        int i = order_rank_high - order_rank_low + 1;
        if (log2(i) - floor(log2(i)) != 0.0000000000000000000000) {
            return output_t<int>(StatusType::FAILURE);
        }
        int num_of_division = int(floor(log2(i)));
        auto left_bound = order_rank_low;
        if(num_of_division == 0){
            //teams->update_bubble_all_tree(teams->root);
            assert(closest_to_upper_from_below->key == closest_to_low_from_above->key);
            return  output_t<int>(output_t<int>(closest_to_low_from_above->key.id));
        }
        while (num_of_division > 0) {
            i = i / 2;
            left_bound += i;
            auto mid = teams->select(left_bound);
            assert(mid != nullptr);
            teams->add(closest_to_upper_from_below->key, mid->key, 1);
            num_of_division--;
        }
        //teams->update_bubble_all_tree(teams->root);
        return output_t<int>(closest_to_upper_from_below->key.id);//here i changed
    }
    catch (...){
        return StatusType::ALLOCATION_ERROR;
    }
}



void olympics_t::destroy_teams(RankTree<Team_Key,Team*>::Node* root) {
    if(root==nullptr){
        return;
    }
    if(root->left!=nullptr) {
        destroy_teams(root->left);
    }
    if(root->right!=nullptr) {
        destroy_teams(root->right);
    }
    Team* team_to_delete = root->value;
    root->value = nullptr;
    delete team_to_delete;

}

Player **olympics_t::merge_arrays_value(Player **array1, int size1, Player **array2, int size2) {
    if(size1+size2 == 0){
        return nullptr;
    }
    int merged_size = size1 + size2;
    Player** merged = new Player*[merged_size]();
    int i, j, k;
    for (i = 0, j = 0 , k = 0; i < size1 && j < size2 ; k++) {
        if (*(array1[i]) < *(array2[j])) {
            merged[k] = array1[i];
            i++;
        }
        else {
            merged[k] = array2[j];
            j++;
        }
    }
    for ( ; i < size1 ; i++, k++) {
        merged[k] = array1[i];
    }
    for ( ; j < size2 ; j++, k++) {
        merged[k] = array2[j];
    }
    return merged;
}

Player_Key *olympics_t::merge_arrays_keys(Player_Key *array1, int size1, Player_Key *array2, int size2) {
    if(size1+size2 == 0){
        return nullptr;
    }
    int merged_size = size1 + size2;
    Player_Key* merged = new Player_Key[merged_size]();
    int i, j, k;
    for (i = 0, j = 0 , k = 0; i < size1 && j < size2 ; k++) {
        if (array1[i] < array2[j]) {
            merged[k] = array1[i];
            i++;
        }
        else {
            merged[k] = array2[j];
            j++;
        }
    }
    for ( ; i < size1 ; i++, k++) {
        merged[k] = array1[i];
    }
    for ( ; j < size2 ; j++, k++) {
        merged[k] = array2[j];
    }
    return merged;
}
