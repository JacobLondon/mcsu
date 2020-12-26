#include <mcsu/mcsu.h>

#define ANNOUNCEMENT_MAX_LINES 20
#define ANNOUNCEMENT_DISAPPEAR_TIME 10.0 /* seconds */

#define BUTTON_SIZE 30
#define BUTTON_HEIGHT 30

typedef enum UIMode {
	MODE_GAME,
	MODE_OPTIONS,
	MODE_FORMATION,
} UIMode;

// insert a line, and move all other lines back by 1
static void announce_insert(char *buf, size_t size, char *text, size_t len, size_t maxlines);
// count number of lines
static int announce_newlines(char *buf, size_t size);

static char announcements[2048];
static int announcement_lines = 0;
static label *l_announcements;
static double last_announcement = 0;

static button *b_options;
static button *b_options_fullscreen;
static button *b_options_volume_up;
static button *b_options_volume_down;
static void func_options(void *client);
static void func_options_fullscreen(void *client);
static void func_options_volume_up(void *client);
static void func_options_volume_down(void *client);

static button *b_form;
static button *b_form_square;
static button *b_form_skine;
static button *b_form_line;
static button *b_form_scatter;
static button *b_form_surround;
static void func_form(void *client);
static void func_form_square(void *client);
static void func_form_skine(void *client);
static void func_form_line(void *client);
static void func_form_scatter(void *client);
static void func_form_surround(void *client);

static button *b_charge;

// defaults
static int initialized = 0;
static int b_width;
#define BUTTON_TOP 10
#define BUTTON_ROW_TOP 30

// query-able items
static UIMode mode;
static int is_charging = 0;
static FormationEnum formation = FORMATION_SQUARE;

void uictrl_init(void)
{
	assert(initialized == 0);

	formation = FORMATION_SQUARE;
	mode = MODE_GAME;
	is_charging = 0;

	b_width = gbl_screen_width / 6.0 - 10;
	last_announcement = GetTime();

	/**
	 * [Options][Charge][Form]
	 * 
	 * [Options]
	 * [Fullscreen][Volume Up][Volume Down]
	 * 
	 *                  [Form]
	 * [Square][Skine][Line][Scatter][Surround]
	 * 
	 * Display announcement after a selection
	 */

	// options
	{
		b_options = button_new("Options", func_options, NULL);
		b_options_fullscreen = button_new("Fullscreen", func_options_fullscreen, NULL);
		b_options_volume_up = button_new("Volume Up", func_options_volume_up, NULL);
		b_options_volume_down = button_new("Volume Down", func_options_volume_down, NULL);

		component_set_pos(b_options, 0, 0);
		component_set_pos(b_options_fullscreen, 0, BUTTON_ROW_TOP);
		component_set_pos(b_options_volume_up, b_width, BUTTON_ROW_TOP);
		component_set_pos(b_options_volume_down, 2 * b_width, BUTTON_ROW_TOP);

		component_set_size(b_options, BUTTON_SIZE, b_width, BUTTON_HEIGHT);
		component_set_size(b_options_fullscreen, BUTTON_SIZE, b_width, BUTTON_HEIGHT);
		component_set_size(b_options_volume_up, BUTTON_SIZE, b_width, BUTTON_HEIGHT);
		component_set_size(b_options_volume_down, BUTTON_SIZE, b_width, BUTTON_HEIGHT);

		component_set_color(b_options, WHITE, BROWN);
		component_set_color(b_options_fullscreen, WHITE, BROWN);
		component_set_color(b_options_volume_up, WHITE, BROWN);
		component_set_color(b_options_volume_down, WHITE, BROWN);
	}

	// formation
	{
		b_form = button_new("Formation", func_form, NULL);
		b_form_square = button_new("Square", func_form_square, NULL);
		b_form_skine = button_new("Skine", func_form_skine, NULL);
		b_form_line = button_new("Line", func_form_line, NULL);
		b_form_scatter = button_new("Scatter", func_form_scatter, NULL);
		b_form_surround = button_new("Surround", func_form_surround, NULL);

		component_set_pos(b_form, b_width, 0);
		component_set_pos(b_form_square, 0, BUTTON_ROW_TOP);
		component_set_pos(b_form_skine, 1 * b_width, BUTTON_ROW_TOP);
		component_set_pos(b_form_line, 2 * b_width, BUTTON_ROW_TOP);
		component_set_pos(b_form_scatter, 3 * b_width, BUTTON_ROW_TOP);
		component_set_pos(b_form_surround, 4 * b_width, BUTTON_ROW_TOP);

		component_set_size(b_form, BUTTON_SIZE, b_width, BUTTON_HEIGHT);
		component_set_size(b_form_square, BUTTON_SIZE, b_width, BUTTON_HEIGHT);
		component_set_size(b_form_skine, BUTTON_SIZE, b_width, BUTTON_HEIGHT);
		component_set_size(b_form_line, BUTTON_SIZE, b_width, BUTTON_HEIGHT);
		component_set_size(b_form_scatter, BUTTON_SIZE, b_width, BUTTON_HEIGHT);
		component_set_size(b_form_surround, BUTTON_SIZE, b_width, BUTTON_HEIGHT);

		component_set_color(b_form, WHITE, BROWN);
		component_set_color(b_form_square, WHITE, BROWN);
		component_set_color(b_form_skine, WHITE, BROWN);
		component_set_color(b_form_line, WHITE, BROWN);
		component_set_color(b_form_scatter, WHITE, BROWN);
		component_set_color(b_form_surround, WHITE, BROWN);
	}

	// announcements
	{
		l_announcements = label_new("", sizeof(announcements));
		component_set_size(l_announcements, 20, 500, 800);
		component_set_color(l_announcements, (Color){ 255, 0, 0, 255 }, BLACK);
		component_set_pos(l_announcements, 25, 80);
	}


	initialized = 1;
}

void uictrl_cleanup(void)
{
	assert(initialized == 1);

	component_del(l_announcements);

	component_del(b_options);
	component_del(b_options_fullscreen);
	component_del(b_options_volume_up);
	component_del(b_options_volume_down);

	component_del(b_form);
	component_del(b_form_square);
	component_del(b_form_skine);
	component_del(b_form_line);
	component_del(b_form_scatter);
	component_del(b_form_surround);

	initialized = 0;
}

void uictrl_update(void)
{
	static int override = 0;

	assert(initialized == 1);

	// announcements: wait, then erase everything
	if (override || (GetTime() > last_announcement + ANNOUNCEMENT_DISAPPEAR_TIME)) {
		uictrl_announce("\n");
		if (++override > ANNOUNCEMENT_MAX_LINES) {
			override = 0;
		}
	}

	// player movement
	if (mode == MODE_GAME) {
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

	component_update(b_options);
	if (mode == MODE_OPTIONS) {
		component_update(b_options_fullscreen);
		component_update(b_options_volume_up);
		component_update(b_options_volume_down);
	}

	component_update(b_form);
	if (mode == MODE_FORMATION) {
		component_update(b_form_square);
		component_update(b_form_skine);
		component_update(b_form_line);
		component_update(b_form_scatter);
		component_update(b_form_surround);
	}

	component_update(l_announcements);
}

void uictrl_draw(void)
{
	assert(initialized == 1);

	component_draw(b_options);
	if (mode == MODE_OPTIONS) {
		component_draw(b_options_fullscreen);
		component_draw(b_options_volume_up);
		component_draw(b_options_volume_down);
	}

	component_draw(b_form);
	if (mode == MODE_FORMATION) {
		component_draw(b_form_square);
		component_draw(b_form_skine);
		component_draw(b_form_line);
		component_draw(b_form_scatter);
		component_draw(b_form_surround);
	}

	// announcements
	component_draw(l_announcements);
}

FormationEnum uictrl_get_formation(void)
{
	return formation;
}

int uictrl_get_charging(void)
{
	return is_charging;
}

void uictrl_announce(char *text)
{
	announce_insert(announcements, sizeof(announcements), text, strlen(text), ANNOUNCEMENT_MAX_LINES);
	announcement_lines = announce_newlines(announcements, sizeof(announcements));
	label_set_text(l_announcements, announcements);
	last_announcement = GetTime();
}

/**
 * If not enough room in buf, override the first line in the buf with
 * everything after it until text may fit. If len is greater than size
 * then we cannot do
 */
static void announce_insert(char *buf, size_t size, char *text, size_t len, size_t maxlines)
{
	size_t usage;
	char *p;

	assert(buf);

	if (len >= size) {
		// failure
		return;
	}

	usage = strlen(buf);
	if (usage > size) {
		// failure
		return;
	}

	// override the first (oldest) line in the buf
	if ((size - usage < len) || (announcement_lines >= maxlines)) {
		p = strchr(buf, '\n');
		if (p == NULL) {
			// nuke the whole buf
			memset(buf, 0, size);
		}
		else {
			// memmove and override oldest line
			memmove(buf, p + 1, size - (p - buf));
		}
	}

	// cat the text
	usage = strlen(buf);
	snprintf(&buf[usage], size - usage, "%s", text);
}

static int announce_newlines(char *buf, size_t size)
{
	size_t i;
	int count = 0;

	assert(buf);

	for (i = 0; i < size; i++) {
		if (buf[i] == '\n') {
			count++;
		}
	}

	return count;
}

static void func_options(void *client)
{
	if (mode == MODE_OPTIONS) {
		mode = MODE_GAME;
	}
	else {
		mode = MODE_OPTIONS;
	}
}

static void func_options_fullscreen(void *client)
{
	ToggleFullscreen();
}

static void func_options_volume_up(void *client)
{
	char buf[32];
	float v = music_man_get_volume();

	if (v + 5 <= 100) {
		v += 5;
	}

	snprintf(buf, sizeof(buf), "Volume: %.0f\n", v);
	music_man_set_volume(v);
	uictrl_announce(buf);
}

static void func_options_volume_down(void *client)
{
	char buf[32];
	float v = music_man_get_volume();

	if (v - 5 >= 0) {
		v -= 5;
	}

	snprintf(buf, sizeof(buf), "Volume: %.0f\n", v);
	music_man_set_volume(v);
	uictrl_announce(buf);
}

static void func_form(void *client)
{
	if (mode == MODE_FORMATION) {
		mode = MODE_GAME;
	}
	else {
		mode = MODE_FORMATION;
	}
}

static void func_form_square(void *client)
{
	formation = FORMATION_SQUARE;
	uictrl_announce("Formation is SQUARE\n");
}

static void func_form_skine(void *client)
{
	formation = FORMATION_SKINE;
	uictrl_announce("Formation is SKINE\n");
}

static void func_form_line(void *client)
{
	formation = FORMATION_LINE;
	uictrl_announce("Formation is LINE\n");
}

static void func_form_scatter(void *client)
{
	formation = FORMATION_SCATTER;
	uictrl_announce("Formation is SCATTER\n");
}

static void func_form_surround(void *client)
{
	formation = FORMATION_SURROUND;
	uictrl_announce("Formation is SURROUND\n");
}
