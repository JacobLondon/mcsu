#include <assert.h>
#include <string.h>

#include "player.h"
#include "util.h"
#include "weapon.h"

int pike_attack_bonus(struct Player *attacker, struct Player *defender);
int spear_attack_bonus(struct Player *attacker, struct Player *defender);
int bill_attack_bonus(struct Player *attacker, struct Player *defender);
int sword_attack_bonus(struct Player *attacker, struct Player *defender);
int axe_attack_bonus(struct Player *attacker, struct Player *defender);
int warhammer_attack_bonus(struct Player *attacker, struct Player *defender);
int bow_attack_bonus(struct Player *attacker, struct Player *defender);
int crossbow_attack_bonus(struct Player *attacker, struct Player *defender);
int javelin_attack_bonus(struct Player *attacker, struct Player *defender);
int lance_attack_bonus(struct Player *attacker, struct Player *defender);

static struct Weapon weapon_list[] = {
    {
        .name = "Pike",
        .type = WEAPON_TYPE_MELEE,
        .range_close = 5,
        .range_max = 15,
        .attack_bonus = pike_attack_bonus,
    },
    {
        .name = "Spear",
        .type = WEAPON_TYPE_MELEE,
        .range_close = 5,
        .range_max = 10,
        .attack_bonus = spear_attack_bonus,
    },
    {
        .name = "Bill",
        .type = WEAPON_TYPE_MELEE,
        .range_close = 5,
        .range_max = 5,
        .attack_bonus = bill_attack_bonus,
    },
    {
        .name = "Sword",
        .type = WEAPON_TYPE_MELEE,
        .range_close = 5,
        .range_max = 5,
        .attack_bonus = sword_attack_bonus,
    },
    {
        .name = "Axe",
        .type = WEAPON_TYPE_MELEE,
        .range_close = 5,
        .range_max = 5,
        .attack_bonus = axe_attack_bonus,
    },
    {
        .name = "War Hammer",
        .type = WEAPON_TYPE_MELEE,
        .range_close = 5,
        .range_max = 5,
        .attack_bonus = warhammer_attack_bonus,
    },
    {
        .name = "Bow",
        .type = WEAPON_TYPE_RANGED,
        .range_close = 50,
        .range_max = 250,
        .attack_bonus = bow_attack_bonus,
    },
    {
        .name = "Crossbow",
        .type = WEAPON_TYPE_RANGED,
        .range_close = 50,
        .range_max = 250,
        .attack_bonus = crossbow_attack_bonus,
    },
    {
        .name = "Javelin",
        .type = WEAPON_TYPE_RANGED,
        .range_close = 20,
        .range_max = 60,
        .attack_bonus = javelin_attack_bonus,
    },
    {
        .name = "Lance",
        .type = WEAPON_TYPE_MELEE,
        .range_close = 5,
        .range_max = 10,
        .attack_bonus = lance_attack_bonus,
    },
};

void weapon_get(const char *name, struct Weapon *out)
{
    int i;

    assert(name);
    assert(out);

    for (i = 0; i < ARRAY_SIZE(weapon_list); i++) {
        if (strcmp(name, weapon_list[i].name) == 0) {
            *out = weapon_list[i];
        }
    }
}

int weapon_is_available(const char *name)
{
    int rv = 0;
    int i;

    assert(name);

    for (i = 0; i < ARRAY_SIZE(weapon_list); i++) {
        if (strcmp(name, weapon_list[i].name) == 0) {
            rv = 1;
            break;
        }
    }

    return rv;
}

int weapon_in_range(struct Player *attacker, struct Player *defender)
{
    assert(attacker);
    assert(defender);

    return (distance(attacker->x, attacker->y, defender->x, defender->y) <= attacker->weapon.range_max);
}

int pike_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 0;

    assert(attacker);
    assert(defender);

    if (strcmp(defender->archetype, "Cavalry") == 0) {
        rv = 8;
    }
    else if (strcmp(defender->archetype, "Infantry") == 0) {
        rv = 2;
    }

    return rv;
}

int spear_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 0;

    assert(attacker);
    assert(defender);

    if (strcmp(defender->archetype, "Cavalry") == 0) {
        rv = 6;
    }
    else if (strcmp(defender->archetype, "Infantry") == 0) {
        rv = 3;
    }

    return rv;
}

int bill_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 0;

    assert(attacker);
    assert(defender);
    assert(defender->archetype);

    if (strcmp(defender->archetype, "Cavalry") == 0) {
        rv = 5;
    }
    else if (strcmp(defender->archetype, "Infantry") == 0) {
        rv = 4;
    }

    if (player_has_armor(defender, "Shield")) {
        rv += 1;
    }

    return rv;
}

int sword_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 0;

    assert(attacker);
    assert(defender);
    assert(defender->archetype);

    if (strcmp(defender->archetype, "Infantry") == 0) {
        rv = 5;
    }

    return rv;
}

int axe_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 0;

    assert(attacker);
    assert(defender);
    assert(defender->archetype);

    if (strcmp(defender->archetype, "Infantry") == 0) {
        rv = 4;
    }

    if (player_has_armor(defender, "Shield")) {
        rv += 3;
    }

    return rv;
}

int warhammer_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 0;

    assert(attacker);
    assert(defender);
    assert(defender->archetype);

    if (strcmp(defender->archetype, "Infantry") == 0) {
        rv = 3;
    }

    if (player_has_armor_of_type(defender, ARMOR_TYPE_HEAVY)) {
        rv += 3;
    }

    return rv;
}

int bow_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 3;

    assert(attacker);
    assert(defender);

    return rv;
}

int crossbow_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 6;

    assert(attacker);
    assert(defender);

    return rv;
}

int javelin_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 3;

    assert(attacker);
    assert(defender);

    if (attacker->is_charging) {
        rv += 5;
    }

    return rv;
}

int lance_attack_bonus(struct Player *attacker, struct Player *defender)
{
    int rv = 0;

    assert(attacker);
    assert(defender);

    if (attacker->is_charging) {
        rv += 10;
    }

    return rv;
}
