#include <mcsu/mcsu.h>

// insert a line, and move all other lines back by 1
static void announce_insert(char *buf, size_t size, char *text, size_t len, size_t maxlines);
// count number of lines
static int announce_newlines(char *buf, size_t size);

static char announcements[2048];
static int announcement_lines = 0;
#define ANNOUNCEMENT_MAX_LINES 20

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
static button *b_form_cancel;

static button *b_charge;

static label *l_announcements;
static double last_announcement = 0;
#define ANNOUNCEMENT_DISAPPEAR_TIME 10.0 /* seconds */

// defaults
static FormationEnum formation = FORMATION_SQUARE;
static int formations_shown = 0; // formation tree
static int options_shown = 0; // option tree
static int is_charging = 0;
static int initialized = 0;

static int b_width;
#define BUTTON_TOP 10
#define BUTTON_ROW_TOP 30

void uictrl_init(void)
{
	assert(initialized == 0);

	formation = FORMATION_SQUARE;
	formations_shown = 0;
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
	 * [Square][Skine][Line][Scatter][Surround][Cancel]
	 * 
	 * Display announcement after a selection
	 */

	b_options = button_new("Options", func_options, NULL);
	b_options_fullscreen = button_new("Fullscreen", func_options_fullscreen, NULL);
	b_options_volume_up = button_new("Volume Up", func_options_volume_up, NULL);
	b_options_volume_down = button_new("Volume Down", func_options_volume_down, NULL);

	component_set_pos(b_options, 0, 0);
	component_set_pos(b_options_fullscreen, 0, BUTTON_ROW_TOP);
	component_set_pos(b_options_volume_up, b_width, BUTTON_ROW_TOP);
	component_set_pos(b_options_volume_down, 2 * b_width, BUTTON_ROW_TOP);

	component_set_size(b_options, 30, b_width, 30);
	component_set_size(b_options_fullscreen, 30, b_width, 30);
	component_set_size(b_options_volume_up, 30, b_width, 30);
	component_set_size(b_options_volume_down, 30, b_width, 30);

	component_set_color(b_options, WHITE, BROWN);
	component_set_color(b_options_fullscreen, WHITE, BROWN);
	component_set_color(b_options_volume_up, WHITE, BROWN);
	component_set_color(b_options_volume_down, WHITE, BROWN);

	// scrolling text infos
	l_announcements = label_new("", sizeof(announcements));
	component_set_size(l_announcements, 20, 500, 800);
	component_set_color(l_announcements, (Color){ 255, 0, 0, 255 }, BLACK);
	component_set_pos(l_announcements, 25, 80);

	initialized = 1;
}

void uictrl_cleanup(void)
{
	assert(initialized == 1);

	component_del(l_announcements);
	initialized = 0;
}

void uictrl_update(void)
{
	static int override = 0;

	assert(initialized == 1);

	// wait, then erase everything
	if (override || (GetTime() > last_announcement + ANNOUNCEMENT_DISAPPEAR_TIME)) {
		uictrl_announce("\n");
		if (++override > ANNOUNCEMENT_MAX_LINES) {
			override = 0;
		}
	}

	component_update(b_options);
	if (options_shown) {
		component_update(b_options_fullscreen);
		component_update(b_options_volume_up);
		component_update(b_options_volume_down);
	}

	component_update(l_announcements);
}

void uictrl_draw(void)
{
	assert(initialized == 1);

	component_draw(b_options);
	if (options_shown) {
		component_draw(b_options_fullscreen);
		component_draw(b_options_volume_up);
		component_draw(b_options_volume_down);
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
	options_shown = !options_shown;
	formations_shown = 0;
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
