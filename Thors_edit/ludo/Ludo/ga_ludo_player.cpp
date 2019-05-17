#include "ga_ludo_player.h"
#include <random>

ga_ludo_player::ga_ludo_player(){

}



void randomize(std::vector<Chromosomes> &chromozones)
{
    for (int i = 0; i<chromozones.size(); i++)
    {
        for (int j = 0; j<chromozones[i].Genes.size(); j++)
        {
            chromozones[i].Genes[j] = ((double) rand()/(RAND_MAX));
            if(VERBOSE == true){
                std::cout<<"gene value:\t"<<chromozones[i].Genes[j]<<std::endl;
            }
        }
    }

}
std::vector<Chromosomes> ga_ludo_player::initialPopulation(){
    std::vector<Chromosomes> populationHolder;
    for (int i =0; i<POPULATION_SIZE; i++) {
        populationHolder.push_back(Chromosomes());
    }
    return populationHolder;
}

void ga_ludo_player::printPopulationGenes(){
    for (int i = 0;i<population.size();i++) {
        std::cout<<"Chromosome: "<<i<<std::endl;
        for (int j = 0; j<population[i].Genes.size(); j++) {
            std::cout<<population[i].Genes[j]<<" ";
        }
        std::cout<<std::endl;
    }
}
/**
 * @brief ga_ludo_player::checkoutBoard
 * Takes a look at the board and returns a matrice of the possible moves that each pieces can do.
 * The matrix will be ActionsXpieces -> ActionsX4
 */
std::vector<std::vector<bool>> ga_ludo_player::checkoutBoard(){
    std::cout<<"in"<<std::endl;
    std::vector<std::vector<bool>> moves ={{false,false,false,false,false,false,false,false,false},{false,false,false,false,false,false,false,false,false},{false,false,false,false,false,false,false,false,false},{false,false,false,false,false,false,false,false,false}};

    /*****************************
          looking at board
      *****************************/
    for(int pieces = 0; pieces <4 ; pieces++){  // Lookign through each players possible moves
        int index = 0;
        if(pos_start_of_turn[pieces]!= GOAL){   // Check if piece is in goal
            index = pos_start_of_turn[pieces]+dice_roll;
            if((dice_roll == 6)&&(pos_start_of_turn[pieces]==HOME)){
                std::cout<<"out"<<std::endl;
                moves[pieces][0]=1;
            }
            // Move to safe zone/ goal stretch
            if(pos_start_of_turn[pieces]+dice_roll == FIRST_IN_GOAL_STRETCH ){
                moves[pieces][1]= true;
                //moveToSafeZoneCounter+=1;
            }
            /*
            // SendEnemyHome
            for( int enemyPiece = 0; enemyPiece < EnemyPiecesPos.size(); enemyPiece++)
                if(pos_start_of_turn[pieces] + dice_roll == EnemyPiecesPos[enemyPiece] ){
                    moves[pieces][2]= true;
                    sendEnemyHomeCounter+=1;
                }
            // Block with friend
            if( ( pos_start_of_turn[pieces] +dice_roll == (MyPiecesPos[pieces+1]
                                                  || MyPiecesPos[pieces-1] )  )
                    && pos_start_of_turn[pieces] !=-1){
                moves[pieces][3]= true;
                blockCounter+=1;
            }
            // moveNormal forward, !! Tjek der ikke står 2 brikker !!
            if(pos_start_of_turn[pieces]+dice_roll< 52 &&
                    pos_start_of_turn[pieces] !=-1  ){
                moves[pieces][4]= true;
                moveNormalCounter+=1;
            }
            */
            // Move onto a star if possible
            if( (index == 5  ||
                index == 18 ||
                index == 31 ||
                index == 44 ||
                index == 11 ||
                index == 24 ||
                index == 37 ||
                    index == 50 ) && pos_start_of_turn[pieces] !=-1){
                 moves[pieces][5]= true;
                 //move2StarCounter+=1;
            }
            // Move onto globe - using index instead of Piece vector container
            if(index < 52 && pos_start_of_turn[pieces] !=-1){     //check only the indexes on the board, not in the home streatch
                if(index % 13 == 0 || (index - 8) % 13 == 0){  //doesn't check for more people at same spot
                    moves[pieces][6]= true;
                    //move2GlobeCounter+=1;
                }
            }
            // Move in Safe zone
            if(pos_start_of_turn[pieces] >= 51  ){
                moves[pieces][7]= true;
                //moveInSafeZoneCounter+=1;
            }
            // Finish piece
            if(pos_start_of_turn[pieces] > 51 && pos_start_of_turn[pieces]+dice_roll ==56 ){
                moves[pieces][8]= true;
                //finishPieceCounter+=1;
            }
        }

    }
    return moves;

}
void ga_ludo_player::printAvailableActions(std::vector<std::vector<bool>> actions){

    for (int pieces = 0; pieces < actions.size(); ++pieces) {
        for (int i = 0; i< actions[pieces].size() ;i++) {
            std::cout<<actions[pieces][i]<<" ";
        }
        std::cout<<std::endl;
    }
}
int ga_ludo_player::make_decision(){

    static int drawCnt = 0;
    drawCnt ++;
    if(drawCnt == 1){
        population = initialPopulation();
        randomize(population);
        if(VERBOSE == true){
            std::cout<<"Population size:\t"<<population.size()<<std::endl;
            std::cout<<"Gene lenght:\t"<<population[0].Genes.size()<<std::endl;
        }

    printPopulationGenes();
    }
    /*
    std::cout<<"size of pos_start"<<pos_start_of_turn[0]<<std::endl;
    std::cout<<"size of pos_start"<<pos_start_of_turn[1]<<std::endl;
    std::cout<<"size of pos_start"<<pos_start_of_turn[2]<<std::endl;
    std::cout<<"size of pos_start"<<pos_start_of_turn[3]<<std::endl;
    std::cout<<"size of pos_start"<<pos_start_of_turn.size()<<std::endl;
    */

    std::vector<std::vector<bool>> possibleMoves;
    possibleMoves = checkoutBoard();
    printAvailableActions(possibleMoves);

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
