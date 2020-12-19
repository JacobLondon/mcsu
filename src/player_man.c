#include <rlu/rlu.h>
#include <mcsu/mcsu.h>

#define PLAYER_MAX 16

static bool initialized = false;

static PlayerStruct me; // this client's player
static PlayerStruct players[PLAYER_MAX];

static float screen_width;
static float screen_height;
static int world_width;
static int world_height;

void player_man_init(float screenw, float screenh, int worldw, int worldh)
{
	assert(initialized == false);

	memset(&me, 0, sizeof(me));
	memset(players, 0, sizeof(players));

	screen_width = screenw;
	screen_height = screenh;
	world_width = worldw;
	world_height = worldh;

	initialized = true;
}

void player_man_cleanup(void)
{
	int i;
	assert(initialized == true);

	player_cleanup(&me);
	for (i = 0; i < ARRAY_SIZE(players); i++) {
		player_cleanup(&players[i]);
	}

	screen_width = 0;
	screen_height = 0;
	world_width = 0;
	world_height = 0;

	initialized = false;
}

void player_man_draw(void)
{
	int i;
	assert(initialized == true);

	if (me.object) {
		so_draw(me.object);
	}

	for (i = 0; i < ARRAY_SIZE(players); i++) {
		if (players[i].object) {
			so_draw(players[i].object);
		}
	}
}

void player_man_update(void)
{
	int i;
	assert(initialized == true);

	// move scene objects to their player position

	if (me.object) {
		so_set_pos(me.object,
			screen_width / world_width * me.data.x,
			screen_height / world_height * me.data.y
		);
		so_update(me.object);
	}

	for (i = 0; i < ARRAY_SIZE(players); i++) {
		if (players[i].object) {
			so_set_pos(me.object,
				screen_width / world_width * players[i].data.x,
				screen_height / world_height * players[i].data.y
			);
			so_update(players[i].object);
		}
	}
}

void player_man_me_load(const char *filename)
{
	FILE *fp;
	char buf[256];
	char key[256];
	char value[256];
	unsigned lineno;
	int tmp;

	// defaults if the configs are missing
	char name[sizeof(me.data.name)] = "NULL";
	PlayerArchetypeEnum arch = 0;

	assert(initialized == true);
	assert(filename);

	fp = fopen(filename, "r");
	assert(fp);

	for (lineno = 0; fgets(buf, sizeof(buf), fp); lineno++) {
		if (buf[0] == '#') {
			continue;
		}
		
		if (strlen(buf) < 3) {
			continue;
		}

		if (sscanf(buf, "%s %s", key, value) != 2) {
			fprintf(stderr, "Error: Could not parse player config %s:%u\n",
				filename, lineno);
			continue;
		}

		if (streq(key, "name")) {
			snprintf(name, sizeof(name), "%s", value);
		}
		else if (streq(key, "archetype")) {
			arch = player_get_archetype_by_name(value);
		}
		else if (streq(key, "armor")) {
			player_set_armor(&me, player_get_armor_by_name(value));
		}
		else if (streq(key, "weapon")) {
			player_set_weapon(&me, player_get_weapon_by_name(value));
		}
		else if (streq(key, "training")) {
			if (sscanf(value, "%d", &tmp) == 1) {
				me.data.training = tmp;
			}
		}
		else if (streq(key, "dex")) {
			if (sscanf(value, "%d", &tmp) == 1) {
				me.data.dex = tmp;
			}
		}
		else {
			fprintf(stderr, "Error: Unknown entry %s:%u\n",
				filename, lineno);
		}
	}

	fclose(fp);

	player_init(&me, name, arch);

	// initiative
	me.data.initiative = RAND_RANGE(1, 20) + me.data.dex;

	// scale the texture to fit the world tile size
	int width = anim_get_width(so_get_anim(me.object)); // tex width in pixels
	float gwidth = screen_width / world_width; // pixel width for 1 grid
	float ratio;

	// ratio of texture pix to grid pix
	if (width > gwidth) {
		ratio = gwidth / width;
	}
	else {
		ratio = width / gwidth;
	}

	so_set_scale(me.object, ratio);
}

void player_man_me_move(int worldx, int worldy)
{
	me.data.x = worldx;
	me.data.y = worldy;
}
