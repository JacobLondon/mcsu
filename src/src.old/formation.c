#include <assert.h>
#include <string.h>

#include "util.h"
#include "formation.h"
#include "player.h"
#include "weapon.h"

static int skine_attack_bonus(struct Player *attacker, struct Player *defender);
static int skine_defend_bonus(struct Player *defender, struct Player *attacker);

static int square_attack_bonus(struct Player *attacker, struct Player *defender);
static int square_defend_bonus(struct Player *defender, struct Player *attacker);

static int line_attack_bonus(struct Player *attacker, struct Player *defender);
static int line_defend_bonus(struct Player *defender, struct Player *attacker);

static int scatter_attack_bonus(struct Player *attacker, struct Player *defender);
static int scatter_defend_bonus(struct Player *defender, struct Player *attacker);

static int c_attack_bonus(struct Player *attacker, struct Player *defender);
static int c_defend_bonus(struct Player *defender, struct Player *attacker);

static struct Formation formation_list[] = {
    {
        .name = "Skine",
        .attack_bonus = skine_attack_bonus,
        .defend_bonus = skine_defend_bonus,
    },
    {
        .name = "Square",
        .attack_bonus = square_attack_bonus,
        .defend_bonus = square_defend_bonus,
    },
    {
        .name = "Line",
        .attack_bonus = line_attack_bonus,
        .defend_bonus = line_defend_bonus,
    },
    {
        .name = "Scatter",
        .attack_bonus = scatter_attack_bonus,
        .defend_bonus = scatter_defend_bonus,
    },
    {
        .name = "C",
        .attack_bonus = c_attack_bonus,
        .defend_bonus = c_defend_bonus,
    },
};


void formation_get(const char *name, struct Formation *out)
{
    int i;

    assert(name);
    assert(out);

    for (i = 0; i < ARRAY_SIZE(formation_list); i++) {
        if (strcmp(name, formation_list[i].name) == 0) {
            *out = formation_list[i];
            return;
        }
    }

    assert(0);
}

int formation_is_available(const char *name)
{
    int rv = 0;
    int i;

    assert(name);

    for (i = 0; i < ARRAY_SIZE(formation_list); i++) {
        if (strcmp(name, formation_list[i].name) == 0) {
            rv = 1;
            break;
        }
    }

    return rv;
}

static int skine_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 0;

    assert(attacker);
    assert(attacker->vtable);
    assert(attacker->vtable->take_damage);
    assert(defender);

    if (!attacker->is_charging) {
        goto skip;
    }

    if (strcmp(defender->archetype, "Infantry") != 0) {
        goto skip;
    }

    rv = 5;

    // 50% to lose 1 man
    if (RAND_RANGE(0, 1)) {
        attacker->vtable->take_damage(attacker, 1);
    }

skip:
    return rv;
}

static int skine_defend_bonus(struct Player *defender, struct Player *attacker)
{
    assert(attacker);
    assert(defender);
    return 0;
}

static int square_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 0;

    assert(attacker);
    assert(defender);

    if (strcmp(defender->archetype, "Cavalry") != 0) {
        goto skip;
    }

    rv = 2;

skip:
    return rv;
}

static int square_defend_bonus(struct Player *defender, struct Player *attacker)
{
    assert(attacker);
    assert(defender);

    return 2;
}

static int line_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 0;

    assert(attacker);
    assert(defender);

    if (strcmp(defender->archetype, "Infantry") != 0) {
        goto skip;
    }

    rv = 2;

skip:
    return rv;
}

static int line_defend_bonus(struct Player *defender, struct Player *attacker)
{
    assert(attacker);
    assert(defender);

    return 0;
}

static int scatter_attack_bonus(struct Player *attacker, struct Player *defender)
{
    assert(attacker);
    assert(defender);

    return 0;
}

static int scatter_defend_bonus(struct Player *defender, struct Player *attacker)
{
    int rv = 0;

    assert(attacker);
    assert(defender);

    if (attacker->weapon.type == WEAPON_TYPE_RANGED) {
        rv = 2;
    }
    else if (attacker->weapon.type == WEAPON_TYPE_MELEE) {
        rv = -2;
    }

    return rv;
}

static int c_attack_bonus(struct Player *attacker, struct Player *defender)
{
    assert(attacker);
    assert(defender);

    return 5;
}

static int c_defend_bonus(struct Player *defender, struct Player *attacker)
{
    assert(attacker);
    assert(defender);

    return 0;
}
