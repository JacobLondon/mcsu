#include <raylib.h>
#include <rlu/rlu.h>
#include <mcsu/mcsu.h>

static Image icon;

static void context_init(void);
static void context_init_cb_autumn(scene *self);
static void context_update(void);
static void context_draw(void);
static void context_cleanup(void);

float gbl_screen_width = 1600;
float gbl_screen_height = 900;
int gbl_world_width = 16;
int gbl_world_height = 9;
int gbl_fps = 60;

int main(void)
{
	context_init();

	while (!WindowShouldClose()) {
		context_update();
		BeginDrawing();
		context_draw();
		EndDrawing();
	}

	context_cleanup();

	return 0;
}

static void context_init(void)
{
	static scene_definition definitions[] = {
		{"Autumn", 1, context_init_cb_autumn},
		{0}
	};

	static scene_set sets[] = {
		{"Autumn Field", {"Autumn", NULL}},
		{0}
	};

	// graphics initialization
	InitWindow(gbl_screen_width, gbl_screen_height, "Mass Combat");
	icon = LoadImage(ASSET_DIRECTORY "/icon_sword.png");
	SetWindowIcon(icon);
	SetTargetFPS(gbl_fps);

	// library initialization
	texture_man_init();
	scene_man_init(definitions, sets);
	player_man_init();
	uictrl_init();

	// library loading
	player_man_me_load("player.conf");
	scene_man_load_set("Autumn Field");
}

static void context_init_cb_autumn(scene *self)
{
	so *s = SCENE_MAN_LOAD_SO(ASSET_DIRECTORY "/field_0.png", 1, 1);
	so_set_pos(s, 0, 0);
	so_set_scale(s, gbl_screen_width / 1920.0); // fit the screen :)
	scene_load_object(self, s);
}

static void context_update(void)
{
	scene_man_update();
	player_man_update();
	uictrl_update();
}

static void context_draw(void)
{
	scene_man_draw();
	player_man_draw();
	uictrl_draw();
	DrawFPS(0, 0);
}

static void context_cleanup(void)
{
	uictrl_cleanup();
	player_man_cleanup();
	scene_man_cleanup();
	UnloadImage(icon);
	CloseWindow();
}

