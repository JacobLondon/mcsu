#include <mcsu/mcsu.h>

#define BUTTON_TOP 10

// insert a line, and move all other lines back by 1
static void announce_insert(char *buf, size_t size, char *text, size_t len, size_t maxlines);
// count number of lines
static int announce_newlines(char *buf, size_t size);

static char announcements[2048];
static int announcement_lines = 0;

static button *b_form;
static button *b_form_square;
static button *b_form_skine;
static button *b_form_line;
static button *b_form_scatter;
static button *b_form_surround;
static button *b_form_cancel;

static button *b_charge;

static label *l_announcements;

// defaults
static FormationEnum formation = FORMATION_SQUARE;
static int formations_shown = 0; // formation tree
static int is_charging = 0;
static int initialized = 0;

static int b_width;

void uictrl_init(void)
{
	assert(initialized == 0);

	formation = FORMATION_SQUARE;
	formations_shown = 0;
	is_charging = 0;

	b_width = gbl_screen_width / 6.0 - 10;

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

	l_announcements = label_new("", sizeof(announcements));
	component_set_size(l_announcements, 20, 500, 800);
	component_set_color(l_announcements, (Color){ 255, 0, 0, 255 }, BLACK);
	component_set_pos(l_announcements, 25, 50);

	initialized = 1;
}

void uictrl_cleanup(void)
{
	assert(initialized == 1);

	initialized = 0;
}

void uictrl_update(void)
{
	assert(initialized == 1);

	component_update(l_announcements);
}

void uictrl_draw(void)
{
	assert(initialized == 1);

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
	announce_insert(announcements, sizeof(announcements), text, strlen(text), 2);
	announcement_lines = announce_newlines(announcements, sizeof(announcements));
	label_set_text(l_announcements, announcements);
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
