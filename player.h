#ifndef MCSU_PLAYER_H
#define MCSU_PLAYER_H

struct PlayerVTable {
    int (* get_attack)(struct Player *self);
    int (* get_defense)(struct Player *self);
    void (* move)(struct Player *self, struct Player list[], int len);
    void (* take_damage)(struct Player *self, int damage);
    struct Player *(* select_opponent)(struct Player *self, struct Player list[], int len);
};

struct Player {
    int hp;
    int ac;
    int damage;
    int range;
    int x;
    int y;
    struct PlayerVTable *vtable;
    const char *name;
    const char *archetype;
};

/**
 * Return 1 on success, otherwise failure
 */
int player_make_archetype(const char *archetype, const char *name, struct Player *out);

void player_position_rand(struct Player *self, struct Player list[], int len, int boardsize);

#endif // MCSU_PLAYER_H
