#ifndef GA_LUDO_PLAYER_H
#define GA_LUDO_PLAYER_H


#include <QObject>
#include <iostream>
#include "positions_and_dice.h"

class ga_ludo_player : public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    int make_decision();
public:
    ga_ludo_player();
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // GA_LUDO_PLAYER_H