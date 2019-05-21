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
    /* This sections create the matrix of the possible moves */
    std::vector<bool> tmp;
    std::vector<std::vector<bool>> moves;
    for (int i = 0;i<4;i++) {
        for (int j = 0;j<ACTIONS;j++) {
            tmp.push_back(false);
        }
        moves.push_back(tmp);
        tmp.clear();
    }

    /*****************************
          looking at board
      *****************************/
    for(int pieces = 0; pieces <4 ; pieces++){  // Lookign through each players possible moves
        int index = 0;
        std::vector<int> enemiesAtSamePos;
        if(pos_start_of_turn[pieces]!= SAFE){   // Check if piece is in goal
            index = pos_start_of_turn[pieces]+dice_roll;
            if((dice_roll == 6)&&(pos_start_of_turn[pieces]==HOME)){
                //std::cout<<"out"<<std::endl;
                moves[pieces][0]=1;
            }
            // Move to safe zone/ goal stretch
            if(pos_start_of_turn[pieces]+dice_roll >= FIRST_IN_GOAL_STRETCH && index < LAST_ON_BOARD){
                moves[pieces][1]= true;
            }

            // SendEnemyHome && Self home
            for(int enemyPiece = 4; enemyPiece < pos_start_of_turn.size(); enemyPiece++){ //Loop over enemies
                if((index == pos_start_of_turn[enemyPiece]) && (pos_start_of_turn[pieces] != HOME )){
                    moves[pieces][2]= true;
                }

            // Self home P1
                if(index == pos_start_of_turn[enemyPiece]){
                    enemiesAtSamePos.push_back(enemyPiece);
                }
            }
            // Self home P2
            if(enemiesAtSamePos.size()>1){
                moves[pieces][10] = true;
            }
            // Block with friend
            for (int i = 0; i < 4; ++i) {
                if(pos_start_of_turn[pieces]+dice_roll ==pos_start_of_turn[i] && pos_start_of_turn[pieces] !=HOME){
                    moves[pieces][3]= true;
                    }
            }
            // moveNormal forward, !! Tjek der ikke står 2 brikker !!
            if(pos_start_of_turn[pieces] != HOME && index != SAFE){
                moves[pieces][4]= true;
                //moveNormalCounter+=1;
            }

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
            if((index == 8 ||
                index == 21 ||
                index == 34 ||
                    index== 47)&& (pos_start_of_turn[pieces] !=-1)){  //doesn't check for more people at same spot
                    moves[pieces][6]= true;
            }

            // Move in Safe zone
            if(pos_start_of_turn[pieces] >= FIRST_IN_GOAL_STRETCH  ){
                moves[pieces][7]= true;
                //moveInSafeZoneCounter+=1;
            }
            // Finish piece
            if(pos_start_of_turn[pieces]+dice_roll == LAST_ON_BOARD ){
                moves[pieces][8]= true;
                //finishPieceCounter+=1;
            }
            // Anorther players home

            if( index == 13 ||
                index == 26 ||
                    index == 39){
                moves[pieces][9] = true;
            }
        }

    }
    return moves;

}
void ga_ludo_player::printAvailableActions(std::vector<std::vector<bool>> actions){
    std::cout<<"\tEnter\tMove2Safe\tJmp2Enemy\tBlock\tMvNormal\tMv2star\tMv2Globe\tMvINsafe\tFinishP\tEnemysHome\tSelfhome"<<std::endl;
    for (int pieces = 0; pieces < actions.size(); ++pieces) {
            std::cout<<"Piece "<<pieces<<" :\t";
            for (int i = 0; i< actions[pieces].size() ;i++) {
                if(actions[pieces][i] == 1){
                    std::cout<<population[0].Genes[i]<<"\t";
                }
                else {
                    std::cout<<actions[pieces][i]<<"\t";
                }

            }
            std::cout<<std::endl;
    }
}
int ga_ludo_player::choosePiece(std::vector<std::vector<bool> > moves, int ChromosomeNr){
    std::vector<int> candidates;        //Candidate holder
    for (int piece = 0; piece < moves.size(); ++piece) {            // Loop pieces
        for (int action = 0; action < moves[0].size(); ++action) {  // Loop actions
            //Find max value gene if the move is allowed
            int tmpAction = 0;
            int tmpPiece = 0;
            // Comparison of the boolean action matrix and the Gene-value
            if(moves[piece][action]==true && population[ChromosomeNr].Genes[action]>tmpAction){
                tmpAction =population[ChromosomeNr].Genes[action];
                //std::cout<<"tmpAction "<<tmpAction<<std::endl;

                /*
                auto it = std::max_element(std::begin(population[0].Genes),std::end(population[0].Genes));  //Pointer iterator for finding max
                //std::cout<<"MAX"<<*it<<std::endl;
                ptrdiff_t pos = std::find(population[0].Genes.begin(),population[0].Genes.end(), *it)-population[0].Genes.begin(); //Finding position for the max-pointer
                std::cout<<"pos "<<pos<<std::endl;

                std::cout<<"High score"<<piece<<std::endl;
                */

                candidates.push_back(piece);
            }
        }
    }
    if(candidates.size()==0){   // Return "void" answer as to which piece
        return -1;
    }
    if(candidates.size()==1){   // Choise The only candidate there is
        return candidates[0];
    }
    if(candidates.size()>1){    // Choice a random candidate of those who scored equally
        std::random_shuffle(candidates.begin(),candidates.end());
        int choice = *candidates.begin();
        return choice;
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


    std::vector<std::vector<bool>> moves = checkoutBoard();
    printAvailableActions(moves);
    std::cout<<"Dice "<<dice_roll<<std::endl;


    /*
    //std::cout<<"size of pos_start"<<pos_start_of_turn.size()<<std::endl;
    std::cout<<"pos_start[0]"<<pos_start_of_turn[0]<<std::endl;
    std::cout<<"pos_start[1]"<<pos_start_of_turn[1]<<std::endl;
    std::cout<<"pos_start[2]"<<pos_start_of_turn[2]<<std::endl;
    std::cout<<"pos_start[3]"<<pos_start_of_turn[3]<<std::endl;
    /*
    if(pos_start_of_turn[0]+dice_roll == 56 && pos_start_of_turn[3] <56){
        std::cout<<"\t\t\tgo in0"<<std::endl;
    }
    if(pos_start_of_turn[1]+dice_roll == 56 && pos_start_of_turn[3] <56){
        std::cout<<"\t\t\tgo in1"<<std::endl;
    }
    if(pos_start_of_turn[2]+dice_roll == 56 && pos_start_of_turn[3] <56){
        std::cout<<"\t\t\tgo in2"<<std::endl;
    }
    if(pos_start_of_turn[3]+dice_roll == 56 && pos_start_of_turn[3] <56){
        std::cout<<"\t\t\tgo in3"<<std::endl;
    }
    std::cout<<"dice roll"<<dice_roll<<std::endl;
    std::cout<<std::endl;
    */

    int piece = choosePiece(moves,ChromosomeNr);
    //std::cout<<"Chromo # "<<ChromosomeNr<<std::endl;
    std::cout<<"Piece Choosen "<<piece<<std::endl;

    return piece;
}

/*
 * @brief ga_ludo_player::mutate_population
 * Mutates the current population with a mutation rate.
 * This rate should be low, else the GA will turn into a generalized search algorithm.
 * mutation probability should be between 0 and 1
*/
void ga_ludo_player::mutate_population(float mutation_rate, float Mutation_probability)
{

    for (int i = 0; i < population.size(); i++)
    {
        int random_integer = rand()%100;
        if (random_integer < Mutation_probability*100)
        {
            int mutation_gene = rand()%ACTIONS;
            int mutation_direction = rand()%100;

            if (mutation_direction < 51)
                mutation_rate = -mutation_rate;
            std::cout << "mutating chromozone: " << i << " with: " << mutation_rate << std::endl;
            if (population[i].Genes[mutation_gene] + mutation_rate < 1.0)
                population[i].Genes[mutation_gene] = 1.0;
            else
                population[i].Genes[mutation_gene] += mutation_rate;

        }
    }
}

void ga_ludo_player::print_best_chromozone()
{
    Chromosomes best_chromozone;
    for (int i = 0; i<population.size(); i++)
    {
        if (population[i].fitness > best_chromozone.fitness)
            best_chromozone = population[i];
    }

    std::cout << "best chromozone info:\t" << std::endl << std::endl;
    //std::cout << "Enter\t\t" << "SafeZone\t" << "send enemy home\t" << "block\t" << "move normal\t" << "move 2 star\t" << "move 2 globe\t" << "move in safe zone\t" << "finish piece\t" << "fitness";
    //std::cout << std::endl;
    for (int i = 0; i<best_chromozone.Genes.size(); i++)
    {
        std::cout << best_chromozone.Genes[i] << "\t";
    }
    std::cout << best_chromozone.fitness;
    std::cout << std::endl;
}

void ga_ludo_player::updatePopulation()
{
    for (int i = 0; i<population.size(); i++)
    {
        population[i] = choose_best();
    }
}

Chromosomes ga_ludo_player::choose_best()
{
    Chromosomes best;
    Chromosomes second_best;
    Chromosomes Cross;
    std::vector<Chromosomes> candidates;

    for (int j = 0; j<10; j++)
    {
        int random_integer = rand()%POPULATION_SIZE;
        candidates.push_back(population[random_integer]);
    }

    for (int k = 0; k < candidates.size(); k++)
    {
        if (candidates[k].fitness > best.fitness)
            best = candidates[k];
    }

    for (int k = 0; k < candidates.size(); k++)
    {
        if (candidates[k].fitness > second_best.fitness && second_best.fitness < best.fitness)
            second_best = candidates[k];
    }

    //std::cout << "best fitness: " << best.fitness << " from a pool of: " << "5" << " " << std::endl;
    Cross = Crossover(best, second_best);

    /*for (int i = 0; i<Cross.Genes.size(); i++)
    {
        //std::cout << Cross.Genes[i] << std::endl;
    }*/

    return Cross;
}

Chromosomes ga_ludo_player::Crossover(Chromosomes parent_mother, Chromosomes parent_father)
{
    Chromosomes Cross;
    Chromosomes Cross_gene;


    int parent = rand()%2;
    if (parent > 0)
    {
        Cross = parent_father;
        Cross_gene = parent_mother;
    }
    else
    {
        Cross = parent_mother;
        Cross_gene = parent_father;
    }

    //std::cout << "Cross_gene fitness: " << Cross_gene.fitness << std::endl;
    /*Gene chooser*/

    int Gene_chooser = rand()%Cross.Genes.size();
    std::cout << "Gene Chooser: " <<Gene_chooser << std::endl;
    if (((Cross.Genes[Gene_chooser] + Cross_gene.Genes[Gene_chooser])/2.0) > 1)
        Cross.Genes[Gene_chooser] = 1;
    else
        Cross.Genes[Gene_chooser] = (Cross.Genes[Gene_chooser] + Cross_gene.Genes[Gene_chooser])/2.0;
    return Cross;
}





void ga_ludo_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void ga_ludo_player::set_fitness(std::vector<float> wins)
{
    fitness = wins;
    for (int i = 0; i<fitness.size(); i++)
    {
        population[i].fitness = fitness[i]/gamesPrChromozone;
    }

    for (int i = 0; i<population.size(); i++)
    {
        //std::cout << "chromozone " << i << " have a fitness of " << population[i].fitness << std::endl;
    }
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
