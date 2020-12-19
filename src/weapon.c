#include <mcsu/mcsu.h>

struct weapon {
	WeaponTypeEnum type;
	uint8_t range_close;
	uint8_t range_max;
	int8_t (* attack_bonus)(PlayerStruct *player, PlayerStruct *defender);
	const char *name;
};

static int pike_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender);
static int spear_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender);
static int bill_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender);
static int sword_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender);
static int axe_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender);
static int warhammer_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender);
static int bow_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender);
static int crossbow_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender);
static int javelin_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender);
static int lance_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender);

static struct weapon weapons[] = {
	[WEAPON_PIKE] = {
		.type=WEAPON_TYPE_MELEE,
		.range_close=5,
		.range_max=15,
		.attack_bonus=pike_attack_bonus,
		.name="pike",
	},
	[WEAPON_SPEAR] = {
		.type=WEAPON_TYPE_MELEE,
		.range_close=5,
		.range_max=10,
		.attack_bonus=spear_attack_bonus,
		.name="spear",
	},
	[WEAPON_BILL] = {
		.type=WEAPON_TYPE_MELEE,
		.range_close=5,
		.range_max=5,
		.attack_bonus=bill_attack_bonus,
		.name="bill",
	},
	[WEAPON_SWORD] = {
		.type=WEAPON_TYPE_MELEE,
		.range_close=5,
		.range_max=5,
		.attack_bonus=sword_attack_bonus,
		.name="sword",
	},
	[WEAPON_AXE] = {
		.type=WEAPON_TYPE_MELEE,
		.range_close=5,
		.range_max=5,
		.attack_bonus=axe_attack_bonus,
		.name="axe",
	},
	[WEAPON_WARHAMMER] = {
		.type=WEAPON_TYPE_MELEE,
		.range_close=5,
		.range_max=5,
		.attack_bonus=warhammer_attack_bonus,
		.name="warhammer",
	},
	[WEAPON_BOW] = {
		.type=WEAPON_TYPE_RANGED,
		.range_close=50,
		.range_max=250,
		.attack_bonus=bow_attack_bonus,
		.name="bow",
	},
	[WEAPON_CROSSBOW] = {
		.type=WEAPON_TYPE_RANGED,
		.range_close=50,
		.range_max=250,
		.attack_bonus=crossbow_attack_bonus,
		.name="crossbow",
	},
	[WEAPON_JAVELIN] = {
		.type=WEAPON_TYPE_MELEE,
		.range_close=20,
		.range_max=60,
		.attack_bonus=javelin_attack_bonus,
		.name="javelin",
	},
	[WEAPON_LANCE] = {
		.type=WEAPON_TYPE_MELEE,
		.range_close=5,
		.range_max=10,
		.attack_bonus=lance_attack_bonus,
		.name="lance",
	},
};

int player_get_weapon_attack_bonus(PlayerStruct *player, PlayerStruct *defender)
{
	return weapons[player->data.weapon_enum].attack_bonus(player, defender);
}

void player_set_weapon(PlayerStruct *player, WeaponEnum weapon)
{
	assert(player);

	player->data.weapon_enum = weapon;
}

WeaponEnum player_get_weapon_by_name(char *weapon)
{
	int i;

	assert(weapon);

	for (i = 0; i < ARRAY_SIZE(weapons); i++) {
		if (streq(weapon, weapons[i].name)) {
			return i;
		}
	}

	// default
	return WEAPON_SWORD;
}

static int pike_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender)
{
	int rv = 0;

	assert(attacker);
	assert(defender);

	if (defender->data.archetype_enum == ARCHETYPE_CAVALRY) {
		rv = 8;
	}
	else if (defender->data.archetype_enum == ARCHETYPE_INFANTRY) {
		rv = 2;
	}

	return rv;
}

static int spear_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender)
{
	int rv = 0;

	assert(attacker);
	assert(defender);

	if (defender->data.archetype_enum == ARCHETYPE_CAVALRY) {
		rv = 6;
	}
	else if (defender->data.archetype_enum == ARCHETYPE_INFANTRY) {
		rv = 3;
	}

	return rv;
}

static int bill_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender)
{
	int rv = 0;

	assert(attacker);
	assert(defender);

	if (defender->data.archetype_enum == ARCHETYPE_CAVALRY) {
		rv = 5;
	}
	else if (defender->data.archetype_enum == ARCHETYPE_INFANTRY) {
		rv = 4;
	}

	if (player_has_armor(defender, ARMOR_SHIELD)) {
		rv += 1;
	}

	return rv;
}

static int sword_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender)
{
	int rv = 0;

	assert(attacker);
	assert(defender);

	if (defender->data.archetype_enum == ARCHETYPE_INFANTRY) {
		rv = 5;
	}

	return rv;
}

static int axe_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender)
{
	int rv = 0;

	assert(attacker);
	assert(defender);

	if (defender->data.archetype_enum == ARCHETYPE_INFANTRY) {
		rv = 4;
	}

	if (player_has_armor(defender, ARMOR_SHIELD)) {
		rv += 3;
	}

	return rv;
}

static int warhammer_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender)
{
	int rv = 0;

	assert(attacker);
	assert(defender);

	if (defender->data.archetype_enum == ARCHETYPE_INFANTRY) {
		rv = 3;
	}

	if (player_has_armor_type(defender, ARMOR_TYPE_HEAVY)) {
		rv += 3;
	}

	return rv;
}

static int bow_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender)
{
	int rv = 3;

	assert(attacker);
	assert(defender);

	return rv;
}

static int crossbow_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender)
{
	int rv = 6;

	assert(attacker);
	assert(defender);

	return rv;
}

static int javelin_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender)
{
	int rv = 3;

	assert(attacker);
	assert(defender);

	if (attacker->data.is_charging) {
		rv += 5;
	}

	return rv;
}

static int lance_attack_bonus(PlayerStruct *attacker, PlayerStruct *defender)
{
	int rv = 0;

	assert(attacker);
	assert(defender);

	if (attacker->data.is_charging) {
		rv += 10;
	}
	else {
		rv += 4;
	}

	return rv;
}
