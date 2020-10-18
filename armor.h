#ifndef MCSU_ARMOR_H
#define MCSU_ARMOR_H

enum ArmorType {
    ARMOR_TYPE_OTHER,
    ARMOR_TYPE_LIGHT,
    ARMOR_TYPE_HEAVY,
};

struct Armor {
    const char *name;
    int ac;
    enum ArmorType type;
};

void armor_get(const char *name, struct Armor *out);

#endif // MCSU_ARMOR_H
