//
// Created by maaya on 16/03/2024.
//

#ifndef WINTERRANK_TEAM_KEY_H
#define WINTERRANK_TEAM_KEY_H
#include <cassert>

class Team_Key{
public:
    int id;
    int strength;
    bool by_strength= false;

    explicit Team_Key(int mid=-2,int strength=0,bool by_strength=true): id(mid),strength(strength),by_strength
            (by_strength){
    }
    Team_Key(const Team_Key& other) =default;
    ~Team_Key()=default;
    friend bool operator<(const Team_Key& key1, const Team_Key& key2){
        assert(key1.by_strength == key2.by_strength);
        if (key1.by_strength==false){
            return key1.id<key2.id;
        }
        else {
            return key1.strength<key2.strength? true: key1.strength==key2.strength && key1.id > key2.id;
        }
    }
    friend bool operator>(const Team_Key& key1, const Team_Key& key2){
        assert(key1.by_strength == key2.by_strength);
        if (key1.by_strength==false){
            return key1.id>key2.id;
        }
        else {
            return key1.strength>key2.strength? true: key1.strength==key2.strength && key1.id < key2.id;
        }
    }
    bool operator==(const Team_Key& key2) const{
        assert(this->by_strength == key2.by_strength);
        if (this->by_strength==false){
            return this->id == key2.id;
        }
        else {
            return this->strength == key2.strength && this->id == key2.id;
        }
    }

};
#endif //WINTERRANK_TEAM_KEY_H
