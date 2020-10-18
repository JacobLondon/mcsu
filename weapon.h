#ifndef MSCU_WEAPON_H
#define MSCU_WEAPON_H

struct Player;

enum WeaponType {
    WEAPON_TYPE_MELEE,
    WEAPON_TYPE_RANGED,
};

struct Weapon {
    const char *name;
    enum WeaponType type;
    int range_close;
    int range_max;
    // based on the circumstances between the players, return the attack bonus
    // does not care about range, this should not be checked here
    int (* attack_bonus)(struct Player *attacker, struct Player *defender);
};

void weapon_get(const char *name, struct Weapon *out);
// check if a weapon can be used by name, 1 if so, otherwise 0
int weapon_is_available(const char *name);
// 1 if attacker is in range of defender, otherwise 0
int weapon_in_range(struct Player *attacker, struct Player *defender);

#endif // MSCU_WEAPON_H
