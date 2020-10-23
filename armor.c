#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "armor.h"
#include "util.h"

static struct Armor armor_list[] = {
    {
        .name = "Shield",
        .ac = 5,
        .type = ARMOR_TYPE_OTHER,
    },
    {
        .name = "Gambason",
        .ac = 2,
        .type = ARMOR_TYPE_LIGHT,
    },
    {
        .name = "Chain_Mail",
        .ac = 5,
        .type = ARMOR_TYPE_HEAVY,
    },
    {
        .name = "Full_Helmet",
        .ac = 3,
        .type = ARMOR_TYPE_OTHER,
    },
    {
        .name = "Kettle_Helm",
        .ac = 2,
        .type = ARMOR_TYPE_OTHER,
    },
    {
        .name = "Mail_Coif",
        .ac = 1,
        .type = ARMOR_TYPE_OTHER,
    },
};

/**
 * Get an existing armor by name
 */
void armor_get(const char *name, struct Armor *out)
{
    int i;

    assert(name);
    assert(out);

    for (i = 0; i < ARRAY_SIZE(armor_list); i++) {
        if (strcmp(name, armor_list[i].name) == 0) {
            *out = armor_list[i];
            return;
        }
    }

    (void)fprintf(stderr, "Error: Armor '%s' does not exist\n", name);
    assert(0);
}
