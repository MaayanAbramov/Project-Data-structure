#include <iostream>
#include "olympics24a2.h"
int calc_strength(int TeamSize){
    if(TeamSize%2 == 0){
        return TeamSize*((TeamSize/2) +1 );
    }
    else return TeamSize*(TeamSize+1)/2;
}
int main() {
    olympics_t* olympics = new olympics_t();
    for (int i = 1; i <= 10; ++i) {
        assert(olympics->add_team(i)== StatusType::SUCCESS);
        assert(olympics->add_player(i,(100%i)*i*2000+3)==StatusType::SUCCESS);
    }
    assert(olympics->remove_team(1)==StatusType::SUCCESS);
    for (int i = 2; i <= 10; ++i) {
        assert(olympics->num_wins_for_team(i).ans()==0);
    }

    delete olympics;

    return 0;
}
