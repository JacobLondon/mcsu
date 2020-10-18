#include <assert.h>
#include <string.h>

#include "armor.h"
#include "util.h"

static struct Armor armor_list[] = {
    {
        .name = "Shield",
        .ac = 5,
    },
    {
        .name = "Gambason",
        .ac = 2,
    },
    {
        .name = "Chain Mail",
        .ac = 5,
    },
    {
        .name = "Full Helmet",
        .ac = 3,
    },
    {
        .name = "Kettle Helm",
        .ac = 2,
    },
    {
        .name = "Mail Coif",
        .ac = 1,
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

    assert(0);
}
