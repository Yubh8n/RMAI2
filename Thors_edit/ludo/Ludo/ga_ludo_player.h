#ifndef GA_LUDO_PLAYER_H
#define GA_LUDO_PLAYER_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"

#define POPULATION_SIZE 10
#define VERBOSE false
#define MAX_GEN 10
#define gamesPrChromozone 10


#define SAFE 99
#define HOME -1
#define FIRST_ON_BOARD 0
#define LAST_ON_BOARD 56
#define FIRST_IN_GOAL_STRETCH 51
#define ACTIONS 11

struct Chromosomes
{
    Chromosomes() {}

    // Adds a many GENES as thinkable for the ludo player.
    /*
     * 16/5-2019
     * */
        double enterBoard    = 0.0;
        double moveToSafeZone= 0.0;
        double sendEnemyHome = 0.0;
        double block         = 0.0;
        double moveNormal    = 0.0;
        double move2Star     = 0.0;
        double move2Globe    = 0.0;
        double moveInSafeZone= 0.0;
        double finishPiece   = 0.0;
        double enemysHome    = 0.0;
        double selfHome      = 0.0;

        double fitness       = 0.0;

        std::vector<double> Genes {enterBoard,moveToSafeZone,sendEnemyHome,block,
                    moveNormal,move2Star,move2Globe,moveInSafeZone,finishPiece,enemysHome,selfHome};
};


class ga_ludo_player : public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    int make_decision();
    Chromosomes choose_best();
    std::vector<Chromosomes> population;

public:
    ga_ludo_player();
    void updatePopulation();
    void mutate_population(float mutation_rate, float Mutation_probability);
    std::vector<float> fitness;
    std::vector<Chromosomes> initialPopulation();
    void printPopulationGenes();
    void printAvailableActions(std::vector<std::vector<bool>> actions);
    void set_fitness(std::vector<float> wins);
    void print_best_chromozone();
    std::vector<std::vector<bool>> checkoutBoard();
    int choosePiece(std::vector<std::vector<bool>> moves, int ChromosomeNr);
    int ChromosomeNr;


signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // GA_LUDO_PLAYER_H
