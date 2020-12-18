#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "input.h"
#include "player.h"
#include "util.h"
#include "share.h"

/**
 * Static Variables
 */

static struct Player player_list[PLAYERS_MAX];

/**
 * Function Prototypes
 */

void announce(const char *format, ...);

int game_player_insert(struct Player *player);
struct Player *game_player_get(const char *name);
void game_parse_player_config(const char *filename);

int turn_attack(struct Player *attacker, struct Player *defender);
void turn_play_initiative(void);
struct Player *turn_yield_current_player(void);
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
 * 1 if player was inserted into player_list, otherwise 0 because no room
 */
int game_player_insert(struct Player *player)
{
    int i;

    assert(player);

    for (i = 0; i < ARRAY_SIZE(player_list); i++) {
        if (player_list[i].name == NULL) {
            player_list[i] = *player;
            return 1;
        }
    }

    return 0;
}

/**
 * Get a player by name, or NULL if not present
 */
struct Player *game_player_get(const char *name)
{
    int i;

    assert(name);

    for (i = 0; i < ARRAY_SIZE(player_list); i++) {
        if (player_list[i].name && (strcmp(name, player_list[i].name) == 0)) {
            return &player_list[i];
        }
    }

    return NULL;
}

struct Player *game_player_next_available(const char *name)
{
    int i;

    assert(name);

    for (i = 0; i < ARRAY_SIZE(player_list); i++) {
        if (player_list[i].name == NULL) {
            return &player_list[i];
        }
    }

    return NULL;
}

void game_parse_player_config(const char *filename)
{
    int i;
    struct Player *player;
    FILE *fp;
    char buf[256];
    char name[256];
    char type[256];
    char value[256];

    assert(filename);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        announce("Could not open player config: '%s'\n", filename);
        return;
    }

    /** Read each line, do an operation for creating a player
     * 
     * Syntax:
     * player_name [item_type] [item_value]
     * 
     * types:
     *      archetype
     *          Infantry
     *          Cavalry
     *          Shock
     *      armor
     *          Shield
     *          Gambason
     *          Chain_Mail
     *          Full_Helmet
     *          Kettle_Helm
     *          Mail_Coif
     *      weapon
     *          Pike
     *          Spear
     *          Bill
     *          Sword
     *          Axe
     *          War_Hammer
     *          Bow
     *          Crossbow
     *          Javelin
     *          Lance
     *      dex
     *          <int> // bonus modifier
     *      training
     *          <int> // bonus training
     *      pos
     *          North
     *          East
     *          South
     *          West
     *          Random (default)
     * 
     * MUST be first instance of a player (also minimum acceptable player):
     * name archetype Infantry
     * 
     * Larger Example:
     * Bob archetype Infantry
     * Bob armor Shield
     * Bob armor Chain_Mail
     * Bob pos South
     */
    while (fgets(buf, sizeof(buf), fp)) {
        if (strlen(buf) < 5) {
            continue;
        }

        if (sscanf(buf, "%s %s %s", name, type, value) != 3) {
            (void)fprintf(stderr, "Error: Could not parse: '%s'", buf);
            break;
        }

        // get player location in the list
        player = game_player_get(name);
        if (player == NULL) {
            player = game_player_next_available(name);
            if (player == NULL) {
                (void)fprintf(stderr, "Error: No more player slots available for '%s'\n", name);
                break;
            }
            if (strcmp(type, "archetype") != 0) {
                (void)fprintf(stderr, "Error: Expected 'archetype' found '%s'\n", type);
                break;
            }
            if (!player_new(value, name, player)) {
                (void)fprintf(stderr, "Error: Unknown archetype '%s'\n", value);
                break;
            }
        }

        else if (strcmp(type, "armor") == 0) {
            player_add_armor(player, value);
        }
        else if (strcmp(type, "weapon") == 0) {
            player_equip_weapon(player, value);
        }
        else if (strcmp(type, "dex") == 0) {
            if (sscanf(value, "%d", &player->dex) != 1) {
                (void)fprintf(stderr, "Error: Dex modifier '%s' could not be parsed to a number\n", value);
                break;
            }
        }
        else if (strcmp(type, "training") == 0) {
            if (sscanf(value, "%d", &player->training) != 1) {
                (void)fprintf(stderr, "Error: Training modifier '%s' could not be parsed to a number\n", value);
                break;
            }
        }
        else if (strcmp(type, "pos") == 0) {
            player_position_rand(player, player_list, ARRAY_SIZE(player_list), BOARD_SIZE);
        }
        else if (strcmp(type, "archetype") == 0) {
            (void)fprintf(stderr, "Error: 'archetype' respecified for player '%s'\n", name);
            break;
        }
        else {
            (void)fprintf(stderr, "Error: Unknown type '%s'\n", type);
            break;
        }
    }

    // load defaults if they were left empty by user
    for (i = 0; i < ARRAY_SIZE(player_list); i++) {
        if (player_list[i].name == NULL) {
            continue;
        }

        if ((player_list[i].x == -1) || (player_list[i].y == -1)) {
            player_position_rand(&player_list[i], player_list, ARRAY_SIZE(player_list), BOARD_SIZE);
        }

        if (player_list[i].weapon.name == NULL) {
            player_equip_weapon(&player_list[i], "Sword");
        }
    }

    (void)fclose(fp);
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

void turn_play_initiative(void)
{
    struct Player *current;

    while ((current = turn_yield_current_player()) != NULL) {
        player_roll_initiative(current);
        announce("%s rolled %d for initiative!\n", current->name, current->initiative);
    }

    // put players in initiative order
    qsort(player_list, ARRAY_SIZE(player_list), sizeof(player_list[0]), player_cmp);
}

struct Player *turn_yield_current_player(void)
{
    static int i = 0;

    for (; i < ARRAY_SIZE(player_list); i++) {
        if (player_list[i].hp > 0 && player_list[i].name != NULL) {
            return &player_list[i++];
        }
    }

    i = 0;
    return NULL;
}

void turn_play_one(void)
{
    struct Player *current;
    struct Player *opponent;

    /**
     * Order:
     * 1. Change Formation?
     * 2. Move?
     * 3. Attack?
     */

    while ((current = turn_yield_current_player()) != NULL) {
        // you don't get a turn if you're dead
        if (current->hp <= 0) {
            continue;
        }

        input_table.display_players(player_list, ARRAY_SIZE(player_list));
        announce(" === %s's Turn ===\n", current->name);

        // 1. Change Formation
        player_choose_formation(current);

        // 2. Do Charge
        current->vtable->do_charge(current);

        // 3. Move
        current->vtable->move(current, player_list, ARRAY_SIZE(player_list));

        // 4. Attack
        opponent = current->vtable->select_opponent(current, player_list, ARRAY_SIZE(player_list));
        if (opponent == NULL) {
            goto end_of_turn;
        }

        if (player_gets_ao(opponent, current)) {
            announce("%s got an attack of opportunity on %s!\n", opponent->name, current->name);
            if (turn_attack(opponent, current)) {
                announce("%s defeated %s!\n", opponent->name, current->name);
            }
        }

        // perform attack
        if (turn_attack(current, opponent)) {
            announce("%s defeated %s!\n", current->name, opponent->name);
        }

end_of_turn:
        // end of turn means you are no longer charging
        current->is_charging = 0;
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
    turn_play_initiative();
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
    srand(time(NULL));

    game_parse_player_config("players.conf");

    turn_play_game();
    return 0;
}
