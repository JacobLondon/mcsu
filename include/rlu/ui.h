#ifndef RLU_UI_H_
#define RLU_UI_H_

#include <raylib.h>

typedef struct component_tag component;
typedef component label;
typedef component button;

typedef void (*button_func)(void *client);

void component_set_pos(component *comp, int x, int y);
void component_set_size(component *comp, int size, int w, int h);
void component_set_color(component *comp, Color fg, Color bg);

component *label_new(char *text, size_t len);
void label_set_text(component *comp, char *text); // set the label text, it will be truncated if longer than initialized len
component *button_new(char *text, button_func callback, void *client);

void component_del(component *comp);

void component_update(component *comp);
void component_draw(component *comp);

const char *component_get_type_str(component *comp);

#endif // RLU_UI_H_
