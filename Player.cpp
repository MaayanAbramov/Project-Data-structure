//
// Created by maaya on 20/03/2024.
//

#include "Player.h"

int Player::getStrength() const {
    return player_strength;
}

void Player::setPlayerStrength(int playerStrength) {
    player_strength = playerStrength;
}

int Player::getId() const {
    return player_id;
}

void Player::setPlayerId(int playerId) {
    player_id = playerId;
}

Player *Player::getTwin() const {
    return twin;
}

void Player::setTwin(Player *twin) {
    Player::twin = twin;
}
Player_Key Player::generate_key(bool by_strength){
    return Player_Key(this->getId(),this->getStrength(),by_strength);
}

bool Player::isByStrength() const {
    return by_strength;
}

void Player::setByStrength(bool byStrength) {
    by_strength = byStrength;
}

int Player::getParticipantCount() {
    return 0;
}
