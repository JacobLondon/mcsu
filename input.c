#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "input.h"


/**
 * Static Function Prototypes
 */

static void cli_choose_move_pos(int *x_out, int *y_out, struct Player list[], int len);
static struct Player *cli_choose_opponent(struct Player *player, struct Player list[], int len);
static void cli_display_players(struct Player list[], int len);

/**
 * Variables
 */

struct InputVTable input_table = (struct InputVTable){
    .choose_move_pos = cli_choose_move_pos,
    .choose_opponent = cli_choose_opponent,
    .display_players = cli_display_players,
};

/**
 * Static Function Definitions
 */

static void cli_choose_move_pos(int *x_out, int *y_out, struct Player list[], int len)
{
    int i;
    int x, y;
    char buf[64];

    assert(x_out);
    assert(y_out);
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

        // now make sure the spot is available
        for (i = 0; i < len; i++) {
            // need to try again
            if ((x == list[i].x) && (y == list[i].y)) {
                (void)printf("Position '%d, %d' already taken\n", x, y);
                goto again;
            }
        }

        // the space is available
        *x_out = x;
        *y_out = y;
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

static void cli_display_players(struct Player list[], int len)
{
    int i;

    assert(list);
    assert(len > 0);

    for (i = 0; i < len; i++) {
        (void)printf("%s %s (%d) is at (%d, %d)\n",
            list[i].archetype, list[i].name, list[i].hp,
            list[i].x, list[i].y);
    }
}
