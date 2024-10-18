//
// Created by maaya on 20/03/2024.
//

#ifndef WINTERRANK_PLAYER_H
#define WINTERRANK_PLAYER_H
#include "Player_Key.h"
#include "cassert"
class Player {

public:
    int player_strength;
    int player_id;
    Player* twin;
    bool by_strength;

    bool isByStrength() const;

    void setByStrength(bool byStrength);

    Player *getTwin() const;

    void setTwin(Player *twin);

    int getStrength() const;

    void setPlayerStrength(int playerStrength);

    int getId() const;

    void setPlayerId(int playerId);
    int getParticipantCount();
    //counter from team
public:
    Player(int playerStrength, int playerId) : player_strength(playerStrength), player_id(playerId) {}
    ~Player() = default ;
    Player(const Player& other) = default;

    Player_Key generate_key(bool by_strength = false);
    friend bool operator<(const Player& key1, const Player& key2){
        assert(key1.by_strength == key2.by_strength);
        if (key1.by_strength==false){
            return key1.player_id<key2.player_id;
        }
        else {
            return key1.player_strength<key2.player_strength? true: key1.player_strength==key2.player_strength && key1
                                                                                              .player_id <
                                                                                      key2
                                                                                              .player_id;
        }
    }
    friend bool operator>(const Player& key1, const Player& key2){
        assert(key1.by_strength == key2.by_strength);
        if (key1.by_strength==false){
            return key1.player_id>key2.player_id;
        }
        else {
            return key1.player_strength>key2.player_strength? true: key1.player_strength==key2.player_strength && key1
                                                                                              .player_id >
                                                                                      key2
                                                                                              .player_id;
        }
    }
    bool operator==(const Player& key2){
        assert(this->by_strength == key2.by_strength);
        if (this->by_strength==false){
            return this->player_id == key2.player_id;
        }
        else {
            return this->player_strength == key2.player_strength && this->player_id == key2.player_id;
        }
    }
};


#endif //WINTERRANK_PLAYER_H
