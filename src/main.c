#include <raylib.h>
#include <rlu/rlu.h>
#include <mcsu/mcsu.h>

static Image icon;

static void context_init(void);
static void context_init_cb_autumn(scene *self);
static void context_update(void);
static void context_draw(void);
static void context_cleanup(void);

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
	InitWindow(1600, 900, "Mass Combat");
	icon = LoadImage(ASSET_DIRECTORY "/icon_sword.png");
	SetWindowIcon(icon);
	SetTargetFPS(60);

	// library initialization
	texture_man_init();
	scene_man_init(definitions, sets);
	player_man_init(1600, 900, 16, 9);

	// library loading
	player_man_me_load("player.conf");
	scene_man_load_set("Autumn Field");
}

static void context_init_cb_autumn(scene *self)
{
	so *s = SCENE_MAN_LOAD_SO(ASSET_DIRECTORY "/field_0.png", 1, 1);
	so_set_pos(s, 0, 0);
	so_set_scale(s, 1600.0 / 1920.0);
	scene_load_object(self, s);
}

static void context_update(void)
{
	// TODO: Temp
	{
		static int x;
		static int y;
		if (IsMouseButtonPressed(0)) {
			x = GetMouseX();
			y = GetMouseY();
		}
		if (IsMouseButtonReleased(0)) {
			// convert to world coordinates
			x = x / 1600.0 * 16;
			y = y / 900.0 * 9;
			player_man_me_move(x, y);
		}
	}

	scene_man_update();
	player_man_update();
}

static void context_draw(void)
{
	scene_man_draw();
	player_man_draw();
	DrawFPS(0, 0);
}

static void context_cleanup(void)
{
	player_man_cleanup();
	scene_man_cleanup();
	UnloadImage(icon);
	CloseWindow();
}

