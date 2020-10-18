#ifndef MCSU_FORMATION_H
#define MCSU_FORMATION_H


struct Player;

struct Formation {
    const char *name;
    // return damage bonus based on configuration
    int (* attack_bonus)(struct Player *attacker, struct Player *defender);
    // return defender AC bonus from attacker
    int (* defend_bonus)(struct Player *defender, struct Player *attacker);
};

void formation_get(const char *name, struct Formation *out);

// get if a given name is a formation
int formation_is_available(const char *name);

#endif // MCSU_FORMATION_H
