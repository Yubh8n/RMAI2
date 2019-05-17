#include "ga_ludo_player.h"
#include <random>

ga_ludo_player::ga_ludo_player()
{

}
std::vector<Chromosomes> ga_ludo_player::initialPopulation(){
    std::vector<Chromosomes> populationHolder;
    for (int i =0; i<POPULATION_SIZE; i++) {
        populationHolder.push_back(Chromosomes());
    }
    return populationHolder;
}

int ga_ludo_player::make_decision(){

    static int drawCnt = 0;
    drawCnt ++;
    if(drawCnt == 1){
        population = initialPopulation();
        if(VERBOSE == true){
            std::cout<<"Population size:\t"<<population.size()<<std::endl;
            std::cout<<"Gene lenght:\t"<<population[0].Genes.size()<<std::endl;
        }
    }



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

void ga_ludo_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void ga_ludo_player::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}
