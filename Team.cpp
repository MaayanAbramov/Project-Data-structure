//
// Created by maaya on 16/03/2024.
//
#include "Team.h"
Team::Team(int teamId) {
    team_id = teamId;

    team_strength = 0;
    counter = 0;
    players_by_strength = new RankTree<Player_Key, Player*>();
    players_by_id = new RankTree<Player_Key, Player*>();

}
Team::Team(int teamId, int teamStrength, int counter, RankTree<Player_Key,Player*>* players_by_strength,
           RankTree<Player_Key,Player*>*  players_by_id) {
    this->team_id = teamId;
    this->team_strength = teamStrength;
    this->counter = counter;
    this->players_by_strength = players_by_strength;
    this->players_by_id = players_by_id;
}

Team::~Team() {
    if(players_by_strength!=nullptr ){
        RankTree<Player_Key, Player *>::destroy_the_tree_and_values(players_by_strength->root);
    }
    if(players_by_id!=nullptr) {
        RankTree<Player_Key, Player *>::destroy_the_tree_and_values(players_by_id->root);
    }
    delete players_by_id;
    delete players_by_strength;
    players_by_id = nullptr;
    players_by_strength = nullptr;
}

int Team::getId() const {
    return team_id;
}

void Team::setTeamId(int teamId) {
    team_id = teamId;
}


int Team::getStrength() const {
    return team_strength;
}

void Team::setTeamStrength(int teamStrength) {
    team_strength = teamStrength;
}

int Team::getCounter() const {
    return counter;
}

void Team::setCounter(int counter) {
    Team::counter = counter;
}


bool Team::addPlayer(int player_strength) {
    int player_id = this->counter;
    Player_Key key = Player_Key(player_id, player_strength);
    auto node = players_by_strength->find_algorithm(key);
    if (node != nullptr) {
        return false;
    }
    auto p_str =new Player(player_strength, player_id);
    auto p_id = new Player(player_strength, player_id);
    p_id->setTwin(p_str);
    p_str->setTwin(p_id);
    p_id->setByStrength(false);
    p_str->setByStrength(true);
    players_by_strength->insertAux(key, p_str);
    Player_Key key_by_id = key;
    key_by_id.by_strength = false;
    players_by_id->insertAux(key_by_id, p_id);
    this->counter +=1;

    return true;
}

bool Team::removePlayer(int player_strength, int player_id) {
    Player_Key key_by_strength = Player_Key(player_id, player_strength, true);
    Player_Key key_by_id = key_by_strength;
    key_by_id.by_strength = false;
    auto to_del_id = players_by_id->find_algorithm(key_by_id)->value;
    auto to_del_strength = players_by_strength->find_algorithm(key_by_strength)->value;
    players_by_id->remove(key_by_id);
    players_by_strength->remove(key_by_strength);
    delete to_del_id;
    delete to_del_strength;
    this->counter-=1;
    assert(this->counter == this->players_by_strength->numOfNodes);
    return true;
}

int Team::getParticipantCount() const {
    if(this->players_by_strength!=nullptr) {
        return this->players_by_strength->numOfNodes;
    }
    else return 0;
}

