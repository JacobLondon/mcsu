#include "player.h"

struct armor {
    int ac;
    ArmorTypeEnum type;
};

static struct armor equips[] = {
    [ARMOR_NONE] = {
        .ac=0,
        .type=ARMOR_TYPE_OTHER,
    },
    [ARMOR_SHIELD] = {
        .ac=5,
        .type=ARMOR_TYPE_OTHER,
    },
    [ARMOR_GAMBASON] = {
        .ac=2,
        .type=ARMOR_TYPE_LIGHT,
    },
    [ARMOR_CHAINMAIL] = {
        .ac=5,
        .type=ARMOR_TYPE_HEAVY,
    },
    [ARMOR_FULLHELM] = {
        .ac=3,
        .type=ARMOR_TYPE_OTHER,
    },
    [ARMOR_KETTLEHELM] = {
        .ac=2,
        .type=ARMOR_TYPE_OTHER,
    },
    [ARMOR_MAILCOIF] = {
        .ac=1,
        .type=ARMOR_TYPE_OTHER,
    },
};

int player_get_ac(PlayerStruct *player)
{
    int i;
    int rv = 0;

    assert(player);

    rv = player->data.ac;

    for (i = 0; i < ARRAY_SIZE(player->data.armor_enum); i++) {
        rv += equips[player->data.armor_enum[i]].ac;
    }

    return rv;
}
