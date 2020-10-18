#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "input.h"
#include "player.h"
#include "util.h"

/**
 * Macros
 */

#define PLAYERS_MAX 2

/**
 * Type Definitions
 */

/**
 * Static Variables
 */

static struct Player player_list[PLAYERS_MAX];

/**
 * Function Prototypes
 */

void announce(const char *format, ...);



int turn_attack(struct Player *attacker, struct Player *defender);
void turn_play_one(void);
int turn_game_is_over(void);
void turn_play_game(void);

/**
 * Function Definitions
 */

void announce(const char *format, ...)
{
    va_list ap;

    assert(format);

    va_start(ap, format);
    (void)vfprintf(stdout, format, ap);
    va_end(ap);
}

/**
 * return 1 if defender dies, otherwise 0
 */
int turn_attack(struct Player *attacker, struct Player *defender)
{
    int damage;

    assert(attacker);
    assert(attacker->vtable);
    assert(attacker->vtable->get_attack);

    assert(defender);
    assert(defender->vtable);
    assert(defender->vtable->get_defense);
    assert(defender->vtable->take_damage);

    damage = attacker->vtable->get_attack(attacker, defender);

    if (!weapon_in_range(attacker, defender)) {
        announce("%s %s attacked but was too far away to hit %s %s!\n",
            attacker->archetype, attacker->name,
            defender->archetype, defender->name);
            goto skip;
    }

    announce("%s %s's %s rolled %d against %s %s's %d AC!\n",
        attacker->archetype, attacker->name, attacker->weapon.name, damage,
        defender->archetype, defender->name,
        defender->vtable->get_defense(defender, attacker));

    damage -= defender->vtable->get_defense(defender, attacker);
    defender->vtable->take_damage(defender, damage);

skip:
    return (defender->hp <= 0);
}

void turn_play_one(void)
{
    int i;
    struct Player *current;
    struct Player *opponent;

    /**
     * Order:
     * 1. Change Formation?
     * 2. Move?
     * 3. Attack?
     */

    for (i = 0; i < ARRAY_SIZE(player_list); i++) {
        current = &player_list[i];

        // you don't get a turn if you're dead
        if (current->hp <= 0) {
            continue;
        }

        input_table.display_players(player_list, ARRAY_SIZE(player_list));
        announce(" === %s's Turn ===\n", player_list[i].name);

        // 1. Change Formation
        player_choose_formation(current);

        // 2. Move
        current->vtable->move(current, player_list, ARRAY_SIZE(player_list));

        // 3. Attack
        opponent = current->vtable->select_opponent(current, player_list, ARRAY_SIZE(player_list));
        if (opponent == NULL) {
            continue;
        }

        // perform attack
        if (turn_attack(current, opponent)) {
            announce("%s defeated %s!\n", current->name, opponent->name);
        }
    }
}

int turn_game_is_over(void)
{
    int i;
    int count = ARRAY_SIZE(player_list);

    for (i = 0; i < ARRAY_SIZE(player_list); i++) {
        if (player_list[i].hp <= 0) {
            count--;
        }
    }

    return (count <= 1);
}

void turn_play_game(void)
{
    while (!turn_game_is_over()) {
        turn_play_one();
    }
}

/**
 * Turn:
 * Every player goes in initiative order:
 * 1. Move?
 * 2. Attack? -> an encounter
 */

int main(void)
{
    struct Player *bob = &player_list[0];
    struct Player *alice = &player_list[1];

    srand(time(NULL));

    player_make_archetype("Infantry", "Bob", bob);
    player_make_archetype("Infantry", "Alice", alice);

    player_add_armor(bob, "Shield");
    player_add_armor(bob, "Chain Mail");

    player_add_armor(alice, "Chain Mail");

    player_position_rand(bob, player_list, ARRAY_SIZE(player_list), 10);
    player_position_rand(alice, player_list, ARRAY_SIZE(player_list), 10);

    turn_play_game();
    return 0;
}
