#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "formation.h"
#include "input.h"
#include "util.h"


/**
 * Static Function Prototypes
 */

static void cli_choose_move_pos(struct Player *player, struct Player list[], int len);
static struct Player *cli_choose_opponent(struct Player *player, struct Player list[], int len);
static void cli_choose_formation(struct Player *player);
static void cli_display_players(struct Player list[], int len);
static void cli_do_charge(struct Player *player);

/**
 * Variables
 */

struct InputVTable input_table = (struct InputVTable){
    .choose_move_pos = cli_choose_move_pos,
    .choose_opponent = cli_choose_opponent,
    .choose_formation = cli_choose_formation,
    .display_players = cli_display_players,
    .do_charge = cli_do_charge,
};

/**
 * Static Function Definitions
 */

/**
 * Pass x/y_out as the current value that the player is at
 */
static void cli_choose_move_pos(struct Player *player, struct Player list[], int len)
{
    int i;
    int x, y;
    char buf[64];
    float dist;

    assert(player);
    assert(list);
    assert(len > 0);

    for (;;) {
    again:
        (void)printf("Move (x y): ");
        (void)fgets(buf, sizeof(buf), stdin);

        // no movement was decided
        if (strlen(buf) <= 1) {
            break;
        }

        if (sscanf(buf, "%d %d", &x, &y) != 2) {
            (void)printf("Unknown position: '%s'\n", buf);
            continue;
        }

        // TODO: Can add player charge movement bonuses by class
        dist = distance(x, y, player->x, player->y);
        if (dist > player_get_speed(player)) {
            (void)printf("Too far to move: %.1f / %d ft\n", dist, player->speed);
            continue;
        }

        // now make sure the spot is available
        for (i = 0; i < len; i++) {
            // need to try again
            if ((x == list[i].x) && (y == list[i].y)) {
                (void)printf("Position '%d, %d' already taken\n", x, y);
                goto again;
            }
        }

        // the space is available
        player->x = x;
        player->y = y;
        break;
    }
}

static struct Player *cli_choose_opponent(struct Player *player, struct Player list[], int len)
{
    int i;
    int x, y;
    char buf[64];
    struct Player *opponent = NULL;

    assert(player);
    assert(list);
    assert(len > 0);

    for (;;) {
        (void)printf("Attack (x y): ");
        (void)fgets(buf, sizeof(buf), stdin);

        // no opponent was decided
        if (strlen(buf) <= 1) {
            break;
        }

        if (sscanf(buf, "%d %d", &x, &y) != 2) {
            (void)printf("Unknown position: '%s'\n", buf);
            continue;
        }

        // now make sure the spot is available
        for (i = 0; i < len; i++) {
            // need to try again
            if ((x == list[i].x) && (y == list[i].y)) {
                opponent = &list[i];
                goto out;
            }
        }
        (void)printf("Position '%d, %d' has no player\n", x, y);
        continue;
    }
out:
    return opponent;
}

/**
 * Allow the player to change formation
 */
void cli_choose_formation(struct Player *player)
{
    char *p;
    char buf[64];

    assert(player);

    for (;;) {
        (void)printf("Set Formation: ");
        (void)fgets(buf, sizeof(buf), stdin);

        p = strstr(buf, "\n");
        if (p != NULL) {
            *p = '\0';
        }

        // don't change formation
        if (strlen(buf) <= 1) {
            break;
        }

        if (!formation_is_available(buf)) {
            (void)printf("Formation '%s' is not a formation\n", buf);
            continue;
        }

        formation_get(buf, &player->formation);
        break;
    }
}

static void cli_display_players(struct Player list[], int len)
{
    int i;

    assert(list);
    assert(len > 0);

    (void)printf("\n");
    for (i = 0; i < len; i++) {
        if (list[i].name != NULL) {
            (void)printf("%s %s (%d) is at (%d, %d)\n",
                list[i].archetype, list[i].name, list[i].hp,
                list[i].x, list[i].y);
        }
    }
}

static void cli_do_charge(struct Player *player)
{
    char *p;
    char buf[64];

    assert(player);

    for (;;) {
        (void)printf("Charge (Yy/Nn): ");
        (void)fgets(buf, sizeof(buf), stdin);

        p = strstr(buf, "\n");
        if (p != NULL) {
            *p = '\0';
        }

        // don't change formation
        if (strlen(buf) <= 1) {
            break;
        }

        if (strchr(buf, 'N') || strchr(buf, 'n')) {
            break;
        }

        if (strchr(buf, 'Y') || strchr(buf, 'y')) {
            player->is_charging = 1;
            break;
        }

        (void)printf("Unknown choice '%s'\n", buf);
    }
}
