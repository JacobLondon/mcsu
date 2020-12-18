#ifndef MCSU_PLAYER_H
#define MCSU_PLAYER_H

#include "armor.h"
#include "formation.h"
#include "weapon.h"

#define ARMOR_EQUIPS_MAX 4

struct PlayerVTable {
    int (* get_attack)(struct Player *self, struct Player *other);
    int (* get_defense)(struct Player *self, struct Player *other);
    void (* move)(struct Player *self, struct Player list[], int len);
    void (* take_damage)(struct Player *self, int damage);
    struct Player *(* select_opponent)(struct Player *self, struct Player list[], int len);
    void (* do_charge)(struct Player *self);
};

struct Player {
    int hp;
    int ac;
    int damage;
    int speed;
    int dex;
    int is_charging;
    int training;
    int x;
    int y;
    struct PlayerVTable *vtable;
    char *name;
    char *archetype;
    struct Armor armor[ARMOR_EQUIPS_MAX];
    struct Formation formation;
    struct Weapon weapon;
    int initiative;
};

/**
 * Return 1 on success, otherwise failure
 */
int player_new(const char *archetype, const char *name, struct Player *out);
void player_del(struct Player *self);

void player_position_rand(struct Player *self, struct Player list[], int len, int boardsize);

void player_add_armor(struct Player *self, const char *armor_name);

int player_has_armor(struct Player *self, const char *armor_name);

int player_has_armor_of_type(struct Player *self, enum ArmorType type);

void player_choose_formation(struct Player *self);

void player_roll_initiative(struct Player *self);

int player_cmp(const void *player0, const void *player1);

int player_gets_ao(struct Player *self, struct Player *other);

int player_get_speed(struct Player *self);

void player_equip_weapon(struct Player *self, const char *weapon_name);

#endif // MCSU_PLAYER_H
