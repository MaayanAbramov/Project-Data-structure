//
// Created by maaya on 03/03/2024.
//

#ifndef WINTERAVLTREE_Player_KEY_H
#define WINTERAVLTREE_Player_KEY_H
#include <cassert>
class Player_Key{
public:
    int id;
    int strength; // id
    bool by_strength= true;

    explicit Player_Key(int mid=-2,int strength=-2,bool by_strength=true): id
    (mid),
    strength
    (strength),
    by_strength
            (by_strength){
//        assert(this->id!=-2);
//        assert(this->strength!=-2);
    }
    Player_Key(int id): id(id),strength(1),by_strength(true){}
    Player_Key(const Player_Key& other) =default;
    ~Player_Key()=default;
    friend bool operator<(const Player_Key& key1, const Player_Key& key2){
        assert(key1.by_strength == key2.by_strength);
        if (key1.by_strength==false){
            return key1.id<key2.id;
        }
        else {
            return key1.strength<key2.strength? true: key1.strength==key2.strength && key1
            .id < key2.id;
        }
    }
    friend bool operator>(const Player_Key& key1, const Player_Key& key2){
        assert(key1.by_strength == key2.by_strength);
        if (key1.by_strength==false){
            return key1.id>key2.id;
        }
        else {
            return key1.strength>key2.strength? true: key1.strength==key2.strength && key1
            .id >
            key2
            .id;
        }
    }
    bool operator==(const Player_Key& key2){
        assert(this->by_strength == key2.by_strength);
        if (this->by_strength==false){
            return this->id == key2.id;
        }
        else {
            return this->strength == key2.strength && this->id == key2.id;
        }
    }

};
#endif //WINTERAVLTREE_Player_KEY_H