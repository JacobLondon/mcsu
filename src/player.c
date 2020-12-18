#include "player.h"

// TODO: Player archetype list of defaults

int player_get_attack(PlayerStruct *player, PlayerStruct *defender)
{
    int damage;
    int tmp;
    int advantage;

    assert(player);
    assert(defender);

    if (player->data.archetype_enum == ARCHETYPE_CAVALRY &&
        player->data.is_charging)
    {
        damage = 5;
    }
    else if (player->data.archetype_enum == ARCHETYPE_INFANTRY &&
             defender->data.archetype_enum == ARCHETYPE_CAVALRY &&
             player->data.weapon_enum == WEAPON_PIKE)
    {
        damage = 5;
    }
    else if (player->data.archetype_enum == ARCHETYPE_SHOCK &&
        defender->data.archetype_enum == ARCHETYPE_INFANTRY)
    {
        damage = 5;
    }

    tmp = RAND_RANGE(1, 20)
        + player->data.damage
        + player->data.training
        + player_get_formation_attack_bonus(player, defender)
        + player_get_weapon_attack_bonus(player, defender)
        + damage;

    if (player->data.has_advantage) {
        advantage = RAND_RANGE(1, 20)
            + player->data.damage
            + player->data.training
            + player_get_formation_attack_bonus(player, defender)
            + player_get_weapon_attack_bonus(player, defender)
            + damage;

        if (advantage > tmp) {
            tmp = advantage;
        }
    }

    damage = tmp;
    return damage;
}

int player_get_defense(PlayerStruct *player, PlayerStruct *attacker)
{
    int ac;

    assert(player);
    assert(attacker);

    ac = player_get_ac(player)
        + player->data.training
        + player_get_formation_defend_bonus(player, attacker);

    return ac;
}
