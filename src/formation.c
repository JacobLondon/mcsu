#include <mcsu/mcsu.h>

struct formation {
    int (* attack_bonus)(PlayerStruct *player, PlayerStruct *defender);
    int (* defend_bonus)(PlayerStruct *player, PlayerStruct *attacker);
};

static int square_attack_bonus(PlayerStruct *player, PlayerStruct *defender);
static int square_defend_bonus(PlayerStruct *player, PlayerStruct *attacker);

static int skine_attack_bonus(PlayerStruct *player, PlayerStruct *defender);
static int skine_defend_bonus(PlayerStruct *player, PlayerStruct *attacker);

static int line_attack_bonus(PlayerStruct *player, PlayerStruct *defender);
static int line_defend_bonus(PlayerStruct *player, PlayerStruct *attacker);

static int scatter_attack_bonus(PlayerStruct *player, PlayerStruct *defender);
static int scatter_defend_bonus(PlayerStruct *player, PlayerStruct *attacker);

static int surround_attack_bonus(PlayerStruct *player, PlayerStruct *defender);
static int surround_defend_bonus(PlayerStruct *player, PlayerStruct *attacker);

static struct formation formations[] = {
    [FORMATION_SQUARE] = {square_attack_bonus, square_defend_bonus},
    [FORMATION_SKINE] = {skine_attack_bonus, skine_defend_bonus},
    [FORMATION_LINE] = {line_attack_bonus, line_defend_bonus},
    [FORMATION_SCATTER] = {scatter_attack_bonus, scatter_defend_bonus},
    [FORMATION_SURROUND] = {surround_attack_bonus, surround_defend_bonus},
};

int player_get_formation_attack_bonus(PlayerStruct *player, PlayerStruct *defender)
{
    return formations[player->data.formation_enum].attack_bonus(player, defender);
}

int player_get_formation_defend_bonus(PlayerStruct *player, PlayerStruct *attacker)
{
    return formations[player->data.formation_enum].defend_bonus(player, attacker);
}

static int skine_attack_bonus(PlayerStruct *player, PlayerStruct *defender)
{
    int rv = 0;

    assert(player);
    assert(defender);

    if (!player->data.is_charging) {
        goto skip;
    }

    if (defender->data.archetype_enum != ARCHETYPE_INFANTRY) {
        goto skip;
    }

    rv = 5;

    // 50% to lose 1 man
    if (RAND_RANGE(0, 2)) {
        player_take_damage(player, 1);
    }

skip:
    return rv;
}

static int skine_defend_bonus(PlayerStruct *player, PlayerStruct *attacker)
{
    assert(attacker);
    assert(player);
    return 0;
}

static int square_attack_bonus(PlayerStruct *player, PlayerStruct *defender)
{
    int rv = 0;

    assert(player);
    assert(defender);

    if (defender->data.archetype_enum == ARCHETYPE_CAVALRY) {
        rv = 2;
    }

    return rv;
}

static int square_defend_bonus(PlayerStruct *player, PlayerStruct *attacker)
{
    assert(player);
    assert(attacker);

    return 2;
}

static int line_attack_bonus(PlayerStruct *player, PlayerStruct *defender)
{
    int rv = 0;

    assert(player);
    assert(defender);

    if (defender->data.archetype_enum == ARCHETYPE_INFANTRY) {
        rv = 2;
    }

    return rv;
}

static int line_defend_bonus(PlayerStruct *player, PlayerStruct *attacker)
{
    assert(player);
    assert(attacker);

    return 0;
}

static int scatter_attack_bonus(PlayerStruct *player, PlayerStruct *defender)
{
    assert(player);
    assert(defender);

    return 0;
}

static int scatter_defend_bonus(PlayerStruct *player, PlayerStruct *attacker)
{
    int rv = 0;

    assert(attacker);
    assert(player);

    if (attacker->data.weapon_type_enum == WEAPON_TYPE_RANGED) {
        rv = 2;
    }
    else if (attacker->data.weapon_type_enum == WEAPON_TYPE_MELEE) {
        rv = -2;
    }

    return rv;
}

static int surround_attack_bonus(PlayerStruct *player, PlayerStruct *defender)
{
    assert(player);
    assert(defender);

    return 5;
}

static int surround_defend_bonus(PlayerStruct *player, PlayerStruct *attacker)
{
    assert(player);
    assert(attacker);

    return 0;
}
