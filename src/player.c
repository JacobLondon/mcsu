#include <rlu/rlu.h>
#include <mcsu/mcsu.h>

// TODO: Player archetype list of defaults

struct player_definition {
	int8_t hp;
	uint8_t ac;
	uint8_t damage;
	uint8_t speed;
	const char *texture;
	const char *unitname;
};

static struct player_definition definitions[] = {
	[ARCHETYPE_INFANTRY] = {
		.hp=10,
		.ac=10,
		.damage=10,
		.speed=60,
		.texture=ASSET_DIRECTORY "/unit_inf.png",
		.unitname="infantry",
	},
	[ARCHETYPE_CAVALRY] = {
		.hp=15,
		.ac=8,
		.damage=10,
		.speed=120,
		.texture=ASSET_DIRECTORY "/unit_cav.png",
		.unitname="cavalry",
	},
	[ARCHETYPE_SHOCK] = {
		.hp=10,
		.ac=10,
		.damage=10,
		.speed=60,
		.texture=ASSET_DIRECTORY "/unit_inf.png",
		.unitname="shock",
	},
};

void player_init(PlayerStruct *out, char *name, PlayerArchetypeEnum archetype)
{
	Texture2D *t;
	anim *a;

	assert(out);
	assert(name);

	memset(out, 0, sizeof(*out));

	out->data.hp = definitions[archetype].hp;
	out->data.ac = definitions[archetype].ac;
	out->data.damage = definitions[archetype].damage;
	out->data.speed = definitions[archetype].speed;

	t = texture_man_load(definitions[archetype].texture);
	a = anim_new(t, 1, 1);
	out->object = so_new_own(a);

	snprintf(out->data.name, sizeof(out->data.name), "%s", name);
	out->data.archetype_enum = archetype;

	// everything else load thru config file, this is just the preset
}

void player_cleanup(PlayerStruct *player)
{
	assert(player);

	if (player->object != NULL) {
		so_del(player->object);
	}

	memset(player, 0, sizeof(*player));
}

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

void player_take_damage(PlayerStruct *player, int8_t amount)
{
	assert(player);
	// TODO: Complexity to this?
	player->data.hp -= amount;
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

PlayerArchetypeEnum player_get_archetype_by_name(char *archetype)
{
	int i;

	assert(archetype);

	for (i = 0; i < ARRAY_SIZE(definitions); i++) {
		if (streq(archetype, definitions[i].unitname)) {
			return i;
		}
	}

	// default
	return 0;
}
