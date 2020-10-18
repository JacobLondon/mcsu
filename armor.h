#ifndef MCSU_ARMOR_H
#define MCSU_ARMOR_H

struct Armor {
    const char *name;
    int ac;
};

void armor_get(const char *name, struct Armor *out);

#endif // MCSU_ARMOR_H
