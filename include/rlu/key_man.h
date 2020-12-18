#ifndef RLU_KEY_MAN_H
#define RLU_KEY_MAN_H


typedef void (*load_cb)(ko *self);

/* definitions to look for when loading */
typedef struct key_definition_tag {
	char *name;
	load_cb load;
} key_definition;

/**
 * Manage groupings of key objects. Call
 * update on many key objects at once
 * for convenience.
 * Key objects can still be manually
 * managed however.
 * 
 * The key manager has its own internal
 * state as a file module. This should
 * be the highest level of key object
 * hierarchy.
 */

void key_man_init(key_definition *definitions);
void key_man_cleanup(void);

/* load static key object initializations by name, end with NULL term */
void key_man_load(char **names);

/* call update on all key objects */
void key_man_update(void);

#endif // RLU_KEY_MAN_H
