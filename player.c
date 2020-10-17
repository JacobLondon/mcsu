#include <assert.h>
#include <string.h>

#include "input.h"
#include "player.h"
#include "util.h"

/**
 * Data Types
 */

// player definition
struct Archetype {
    const char *archetype;
    struct Player definition; // copy from
};

/**
 * Static Function Prototypes
 */

static int infantry_get_attack(struct Player *self);
static int infantry_get_defense(struct Player *self);
static void infantry_move(struct Player *self, struct Player list[], int len);
static void infantry_take_damage(struct Player *self, int damage);
static struct Player *infantry_select_opponent(struct Player *self, struct Player list[], int len);

/**
 * Static Variables
 */

static struct PlayerVTable infantry_vtable = (struct PlayerVTable){
    .get_attack = infantry_get_attack,
    .get_defense = infantry_get_defense,
    .move = infantry_move,
    .take_damage = infantry_take_damage,
    .select_opponent = infantry_select_opponent,
};

static struct Archetype arch_list[] = {
    {
        .archetype = "Infantryman",
        .definition = (struct Player){
            .hp = 10,
            .ac = 20,
            .damage = 10,
            .range = 5, // feet
            .vtable = &infantry_vtable,
        },
    }
};

/**
 * Static Function Definitions
 */

static int infantry_get_attack(struct Player *self)
{
    assert(self);
    return RAND_RANGE(1, 20) + self->damage;
}

static int infantry_get_defense(struct Player *self)
{
    assert(self);
    return self->ac;
}

static void infantry_move(struct Player *self, struct Player list[], int len)
{
    assert(self);

    input_table.choose_move_pos(&self->x, &self->y, list, len);
}

static void infantry_take_damage(struct Player *self, int damage)
{
    assert(self);

    // nothing, beat the AC
    if (damage <= 0) {
        return;
    }

    self->hp -= damage;
}

static struct Player *infantry_select_opponent(struct Player *self, struct Player list[], int len)
{
    assert(self);
    assert(list);
    assert(len > 0);

    return input_table.choose_opponent(self, list, len);
}

/**
 * Public Function Definitions
 */

int player_make_archetype(const char *archetype, const char *name, struct Player *out)
{
    int i;
    int rv = 0;

    assert(archetype);
    assert(name);
    assert(out);

    (void)memset(out, 0, sizeof(struct Player));

    for (i = 0; i < ARRAY_SIZE(arch_list); i++) {
        if (strcmp(archetype, arch_list[i].archetype) == 0) {
            (void)memcpy(out, &arch_list[i].definition, sizeof(struct Player));
            out->archetype = archetype;
            out->name = name;
            rv = 1;
            break;
        }
    }

    return rv;
}

void player_position_rand(struct Player *self, struct Player list[], int len, int boardsize)
{
    int i;

    assert(self);
    assert(list);
    assert(len > 0);
    assert(boardsize > 0);

again:
    self->x = RAND_RANGE(0, boardsize);
    self->y = RAND_RANGE(0, boardsize);

    // make sure the values aren't repeated
    for (i = 0; i < len; i++) {
        // don't compare self with self
        if (self == &list[i]) {
            continue;
        }

        // try again...
        if ((self->x == list[i].x) || (self->y == list[i].y)) {
            goto again;
        }
    }

    // no identical positions created, done
    return;
}
