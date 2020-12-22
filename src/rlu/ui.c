#include <rlu/rlu.h>

enum comp_type {
	TYPE_LABEL,
	TYPE_BUTTON,
	TYPE_COUNT,
};

typedef struct component_tag {
	int x;
	int y;
	int w;
	int h;
	int size;
	enum comp_type type;
	Color fg;
	Color bg;
	union {
		struct {
			char *text;
			size_t len;
		} label;
		struct {
			char *text;
			button_func callback;
			void *client;
		} button;
	} data;
} component;

static component *component_new(void);
static int component_contains(component *comp, int x, int y);

void component_set_pos(component *comp, int x, int y)
{
	assert(comp);
	comp->x = x;
	comp->y = y;
}

void component_set_size(component *comp, int size, int w, int h)
{
	assert(comp);
	comp->size = size;
	comp->w = w;
	comp->h = h;
}

void component_set_color(component *comp, Color fg, Color bg)
{
	assert(comp);
	comp->fg = fg;
	comp->bg = bg;
}

component *label_new(char *text, size_t len)
{
	component *comp;
	assert(text);

	comp = component_new();
	comp->type = TYPE_LABEL;
	comp->data.label.len = len;
	comp->data.label.text = malloc(len);
	assert(comp->data.label.text);

	snprintf(comp->data.label.text, len, "%s", text);

	return comp;
}

void label_set_text(component *comp, char *text)
{
	assert(text);
	assert(comp);
	assert(comp->type == TYPE_LABEL);

	snprintf(comp->data.label.text, comp->data.label.len, "%s", text);
}

component *button_new(char *text, button_func callback, void *client)
{
	component *comp;
	size_t len;

	assert(text);
	assert(callback);

	len = strlen(text);
	comp = component_new();
	comp->type = TYPE_BUTTON;
	comp->data.button.text = malloc(len + 1);
	assert(comp->data.button.text);

	snprintf(comp->data.button.text, len + 1, "%s", text);
	comp->data.button.callback = callback;
	comp->data.button.client = client;

	return comp;
}

void component_del(component *comp)
{
	assert(comp);
	switch (comp->type) {
	case TYPE_LABEL:
		if (comp->data.label.text) {
			free(comp->data.label.text);
		}
		break;
	case TYPE_BUTTON:
		if (comp->data.button.text) {
			free(comp->data.button.text);
		}
	default:
		break;
	}
	free(comp);
}

void component_update(component *comp)
{
	assert(comp);

	switch (comp->type) {
	case TYPE_LABEL:
		break;
	case TYPE_BUTTON:
		if (component_contains(comp, GetMouseX(), GetMouseY())) {
			if (IsMouseButtonReleased(0)) {
				comp->data.button.callback(comp->data.button.client);
			}
		}
		break;
	default:
		break;
	}
}

void component_draw(component *comp)
{
	Color tmp;
	assert(comp);

	switch (comp->type) {
	case TYPE_LABEL:
		DrawText(comp->data.label.text, comp->x, comp->y, comp->size, comp->fg);
		break;
	case TYPE_BUTTON:
		tmp = comp->bg;
		if (component_contains(comp, GetMouseX(), GetMouseY())) {
			tmp.r = min(255, tmp.r * 1.2);
			tmp.g = min(255, tmp.g * 1.2);
			tmp.b = min(255, tmp.b * 1.2);
		}
		DrawRectangle(comp->x, comp->y, comp->w, comp->h, tmp);
		DrawText(comp->data.button.text, comp->x, comp->y, comp->size, comp->fg);
		break;
	default:
		break;
	}
}

const char *component_get_type_str(component *comp)
{
	assert(comp);
	switch (comp->type) {
	case TYPE_LABEL: return "Label";
	case TYPE_BUTTON: return "Button";
	default: return "Unknown";
	}
	return "Unknown";
}

static component *component_new(void)
{
	component *comp = malloc(sizeof(*comp));
	assert(comp);
	memset(comp, 0, sizeof(*comp));
	return comp;
}

static int component_contains(component *comp, int x, int y)
{
	assert(comp);
	return (comp->x < x && x < comp->x + comp->w &&
	        comp->y < y && y < comp->y + comp->h);
}
