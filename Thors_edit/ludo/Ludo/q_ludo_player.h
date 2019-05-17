#ifndef Q_LUDO_PLAYER_H
#define Q_LUDO_PLAYER_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"


struct states{
    // Adds a many characteristics as thinkeble for the ludo player.
    /*
     * 9 states (Enterboard,move to safe zone, Send enemy home, Block,
     * Move normally, Move to star, Move to globe,
     * move in safe zone, finish pieces) 16/5-2019
     * */
        float enterBoard    = 0;
        float moveToSafeZone= 0;
        float sendEnemyHome = 0;
        float block         = 0;
        float moveNormal    = 0;
        float move2Star     = 0;
        float move2Globe    = 0;
        float moveInSafeZone= 0;
        float finishPiece   = 0;

        std::vector<float> gameStates {enterBoard,moveToSafeZone,sendEnemyHome,block,
                    moveNormal,move2Star,move2Globe,moveInSafeZone,finishPiece};
};


class q_ludo_player: public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    int make_decision();
public:
    q_ludo_player();

    void printQTable(int round);

    states piece1,piece2,piece3,piece4;
    // 4 Action (move which piece)
    std::vector<std::vector<float>> QLtable = {piece1.gameStates,piece2.gameStates,piece3.gameStates,piece4.gameStates};
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // Q_LUDO_PLAYER_H
