//
// Created by maaya on 16/03/2024.
//

#ifndef WINTERRANK_TEAM_H
#define WINTERRANK_TEAM_H
#include "RankTree.h"
#include "Player_Key.h"
#include "Player.h"
#include <cmath>
class Team {
public:
    int team_id;

    int getId() const;

    void setTeamId(int teamId);


    int getParticipantCount() const;
    int getStrength() const;

    void setTeamStrength(int teamStrength);

    int getCounter() const;

    void setCounter(int counter);



    bool addPlayer(int player_strength);
    bool removePlayer(int player_strength, int player_id);

    int team_strength;
    int counter;
    RankTree<Player_Key,Player*>* players_by_strength;
    RankTree<Player_Key,Player*>* players_by_id;

    explicit Team(int teamId);
    Team(int teamId, int team_strength, int counter, RankTree<Player_Key,Player*>* players_by_strength,  RankTree<Player_Key,Player*>* players_by_id);
    ~Team();
    Team(const Team& other) = delete;



};
#endif //WINTERRANK_TEAM_H
