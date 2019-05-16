#include "q_ludo_player.h"

q_ludo_player::q_ludo_player()
{

}

void q_ludo_player::printQTable(int round){
    std::cout<<"Q table\tRound:  "<<round<<std::endl;
    for (int i = 0; i< QLtable.size(); i++) {
        for (int j = 0; j< QLtable[0].size(); j++) {
            std::cout<<QLtable[i][j];
        }
        std::cout<<std::endl;
    }
}

int q_ludo_player::make_decision(){
    static int round = 0;       // Counts the number of rounds played in a game
    round+=1;
    printQTable(round);



    if(dice_roll == 6){
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
    } else {
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){ //maybe they are all locked in
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
    }
    return -1;
}

void q_ludo_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void q_ludo_player::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}
