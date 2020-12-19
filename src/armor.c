#include <mcsu/mcsu.h>

struct armor {
	int ac;
	ArmorTypeEnum type;
	const char *name;
};

static struct armor equips[] = {
	[ARMOR_NONE] = {
		.ac=0,
		.type=ARMOR_TYPE_OTHER,
		.name="none",
	},
	[ARMOR_SHIELD] = {
		.ac=5,
		.type=ARMOR_TYPE_OTHER,
		.name="shield",
	},
	[ARMOR_GAMBASON] = {
		.ac=2,
		.type=ARMOR_TYPE_LIGHT,
		.name="gambason",
	},
	[ARMOR_CHAINMAIL] = {
		.ac=5,
		.type=ARMOR_TYPE_HEAVY,
		.name="chainmail",
	},
	[ARMOR_FULLHELM] = {
		.ac=3,
		.type=ARMOR_TYPE_OTHER,
		.name="fullhelm",
	},
	[ARMOR_KETTLEHELM] = {
		.ac=2,
		.type=ARMOR_TYPE_OTHER,
		.name="kettlehelm",
	},
	[ARMOR_MAILCOIF] = {
		.ac=1,
		.type=ARMOR_TYPE_OTHER,
		.name="mailcoif",
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

int player_has_armor(PlayerStruct *player, ArmorEnum armor)
{
	int i;

	assert(player);

	for (i = 0; i < ARRAY_SIZE(player->data.armor_enum); i++) {
		if (player->data.armor_enum[i] == armor) {
			return 1;
		}
	}
	return 0;
}

int player_has_armor_type(PlayerStruct *player, ArmorTypeEnum armor_type)
{
	int i;
	int j;

	assert(player);

	for (i = 0; i < ARRAY_SIZE(player->data.armor_enum); i++) {
		for (j = 0; j < ARRAY_SIZE(equips); j++) {
			// match armor and match armor type
			if (j == player->data.armor_enum[i] && equips[j].type == armor_type) {
				return 1;
			}
		}
	}
	return 0;
}

void player_set_armor(PlayerStruct *player, ArmorEnum armor)
{
	int i;

	assert(player);

	for (i = 0; i < ARRAY_SIZE(player->data.armor_enum); i++) {
		if (player->data.armor_enum[i] == ARMOR_NONE) {
			player->data.armor_enum[i] = armor;
		}
	}

	// can't keep equipping
}

ArmorEnum player_get_armor_by_name(char *armor)
{
	int i;

	assert(armor);

	for (i = 0; i < ARRAY_SIZE(equips); i++) {
		if (streq(armor, equips[i].name)) {
			return i;
		}
	}

	// default
	return 0;
}
