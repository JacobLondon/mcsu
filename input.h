#ifndef MCSU_INPUT_H
#define MCSU_INPUT_H

#include "player.h"

struct InputVTable {
    // choose a location to move to, affirm that it is not taken before
    void (* choose_move_pos)(int *x_out, int *y_out, struct Player list[], int len);
    // choose a location which has an opponent on it, affirm it is not empty
    struct Player *(* choose_opponent)(struct Player *player, struct Player list[], int len);

    void (* display_players)(struct Player list[], int len);
};

extern struct InputVTable input_table;

#endif // MCSU_INPUT_H