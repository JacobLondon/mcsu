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

static int infantry_get_attack(struct Player *self, struct Player *other);
static int infantry_get_defense(struct Player *self, struct Player *other);
static void infantry_move(struct Player *self, struct Player list[], int len);
static void infantry_take_damage(struct Player *self, int damage);
static struct Player *infantry_select_opponent(struct Player *self, struct Player list[], int len);
static void infantry_do_charge(struct Player *self);

/**
 * Static Variables
 */

static struct PlayerVTable infantry_vtable = (struct PlayerVTable){
    .get_attack = infantry_get_attack,
    .get_defense = infantry_get_defense,
    .move = infantry_move,
    .take_damage = infantry_take_damage,
    .select_opponent = infantry_select_opponent,
    .do_charge = infantry_do_charge,
};

static struct Archetype arch_list[] = {
    {
        .archetype = "Infantry",
        .definition = (struct Player){
            .hp = 10,
            .ac = 10,
            .damage = 10,
            .speed = 60,
            .dex = 1,
            .training = 5,
            .vtable = &infantry_vtable,
        },
    },
    // TODO
    /*{
        .archetype = "Cavalry",
        .definition = (struct Player){
            .hp = 10,
            .ac = 20,
            .damage = 10,
            .speed = 120,
            .training = 5,
            .vtable = &cavalry_table,
        },
    },*/
};

int player_has_armor(struct Player *self, const char *armor_name)
{
    int rv = 0;
    int i;

    assert(self);
    assert(armor_name);

    for (i = 0; i < ARRAY_SIZE(self->armor); i++) {
        if ((self->armor[i].name != NULL) && (strcmp(self->armor[i].name, armor_name) == 0)) {
            rv = 1;
        }
    }

    return rv;
}

int player_has_armor_of_type(struct Player *self, enum ArmorType type)
{
    int rv = 0;
    int i;

    assert(self);

    for (i = 0; i < ARRAY_SIZE(self->armor); i++) {
        if ((self->armor[i].name != NULL) && (self->armor[i].type == type)) {
            rv = 1;
        }
    }

    return rv;
}

void player_choose_formation(struct Player *self)
{
    assert(self);
    input_table.choose_formation(self);
}


void player_roll_initiative(struct Player *self)
{
    assert(self);

    self->initiative = RAND_RANGE(1, 20) + self->dex;
}

/**
 * Qsort function for 'struct Player[]'
 */
int player_cmp(const void *player0, const void *player1)
{
    struct Player *p0 = (struct Player *)player0;
    struct Player *p1 = (struct Player *)player1;

    assert(player0);
    assert(player1);

    return (p1->initiative - p0->initiative);
}

/**
 * Return 1 if an attack of opportunity is available, otherwise 0
 */
int player_gets_ao(struct Player *self, struct Player *other)
{
    int rv = 0;

    assert(self);
    assert(other);

    if (weapon_in_range(self, other)
        && other->is_charging
        && ((strcmp(self->weapon.name, "Pike") == 0)
        || (strcmp(self->weapon.name, "Spear") == 0)
        || (strcmp(self->weapon.name, "Bill") == 0)
        || (strcmp(self->weapon.name, "Lance") == 0))
       )
    {
        rv = 1;
    }

    return rv;
}

int player_get_speed(struct Player *self)
{
    int rv = 0;

    assert(self);

    rv = self->speed;

    if ((strcmp(self->archetype, "Cavalry") == 0) && self->is_charging) {
        rv += 60;
    }

    return rv;

}

/**
 * Static Function Definitions
 */

static int infantry_get_attack(struct Player *self, struct Player *other)
{
    int bonus = 0;

    assert(self);
    assert(other);
    assert(self->formation.attack_bonus);
    assert(self->weapon.attack_bonus);

    // TODO: Split these checks into their class method
    if ((strcmp(self->archetype, "Cavalry") == 0) && self->is_charging) {
        bonus = 5;
    }
    else if ((strcmp(self->archetype, "Infantry") == 0)
          && (strcmp(other->archetype, "Cavalry") == 0)
          && (strcmp(self->weapon.name, "Pike") == 0))
    {
        bonus = 5;
    }
    else if ((strcmp(self->archetype, "Shock") == 0)
          && (strcmp(other->archetype, "Infantry") == 0))
    {
        bonus = 5;
    }

    return RAND_RANGE(1, 20)
        + self->damage
        + self->training
        + self->formation.attack_bonus(self, other)
        + self->weapon.attack_bonus(self, other)
        + bonus;
}

static int infantry_get_defense(struct Player *self, struct Player *other)
{
    int i;
    int ac;

    assert(self);
    assert(other);
    assert(self->formation.defend_bonus);

    ac = self->ac
        + self->training
        + self->formation.defend_bonus(self, other);

    for (i = 0; i < ARRAY_SIZE(self->armor); i++) {
        if (self->armor[i].name != NULL) {
            ac += self->armor[i].ac;
        }
    }

    return ac;
}

static void infantry_move(struct Player *self, struct Player list[], int len)
{
    assert(self);
    assert(list);
    assert(len > 0);

    input_table.choose_move_pos(self, list, len);
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

static void infantry_do_charge(struct Player *self)
{
    assert(self);

    input_table.do_charge(self);
}

/**
 * Public Function Definitions
 */

int player_new(const char *archetype, const char *name, struct Player *out)
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
            // default formation
            formation_get("Line", &out->formation);
            weapon_get("Sword", &out->weapon);
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

void player_add_armor(struct Player *self, const char *armor_name)
{
    int i;

    assert(self);
    assert(armor_name);

    for (i = 0; i < ARRAY_SIZE(self->armor); i++) {
        // we found the open armor slot
        if (self->armor[i].name == NULL) {
            armor_get(armor_name, &self->armor[i]);
            return;
        }
    }
    assert(0);
}
