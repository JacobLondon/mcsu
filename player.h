#ifndef MCSU_PLAYER_H
#define MCSU_PLAYER_H

#include "armor.h"
#include "formation.h"

#define ARMOR_EQUIPS_MAX 4

struct PlayerVTable {
    int (* get_attack)(struct Player *self, struct Player *other);
    int (* get_defense)(struct Player *self, struct Player *other);
    void (* move)(struct Player *self, struct Player list[], int len);
    void (* take_damage)(struct Player *self, int damage);
    struct Player *(* select_opponent)(struct Player *self, struct Player list[], int len);
};

struct Player {
    int hp;
    int ac;
    int damage;
    int range;
    int speed;
    int is_charging;
    int training;
    int x;
    int y;
    struct PlayerVTable *vtable;
    const char *name;
    const char *archetype;
    struct Armor armor[ARMOR_EQUIPS_MAX];
    struct Formation formation;
};

/**
 * Return 1 on success, otherwise failure
 */
int player_make_archetype(const char *archetype, const char *name, struct Player *out);

void player_position_rand(struct Player *self, struct Player list[], int len, int boardsize);

void player_add_armor(struct Player *self, const char *armor_name);

void player_choose_formation(struct Player *self);

#endif // MCSU_PLAYER_H
