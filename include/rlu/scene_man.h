#ifndef RLU_SCENE_MAN_H
#define RLU_SCENE_MAN_H

#include "scene.h"

#define ACTIVE_SCENES_MAX 16 /* max number of scenes to use at once */

typedef struct scene_set_tag {
	const char *name;
	char *scene_names[ACTIVE_SCENES_MAX];
} scene_set;

/**
 * Manage groupings of scenes. Call update
 * and draw on many scenes at once for convenience.
 * Scenes can still be manually manaaged however.
 * 
 * Instead of a struct containing the data, the
 * scene manager has its own internal state as a
 * file module. This module contains its own
 * animation manager and its own data structures to
 * manage scenes and its animation manager. This should
 * be the highest level of hierarchy of managing background
 * scenes, and probably shouldn't be used to manage
 * midground and foreground scenes which will probably
 * need differing utilities that scene manager isn't
 * meant to handle.
 */

/* Allocate anim_man for scene management */
void scene_man_init(scene_definition *definitions, scene_set *sets);

/* Deallocate anim_man */
void scene_man_cleanup(void);

/**
 * Load all given definitions by name. Each definition
 * statically exists inside the corresponding *.c file
 * 'names' must be NULL terminated and all names MUST
 * exist */
void scene_man_load_set(char *name);
void scene_man_load_idx(int idx);
void scene_man_load_rand(void);

/* call update on all scenes */
void scene_man_update(void);

/* call draw on all scenes */
void scene_man_draw(void);

/**
 * Bind visibility to a local variable,
 * is_visible == NULL means always draw
 * 
 * Control scene visibility at the manager level. Scenes
 * themselves (and scene objects for that matter) do not
 * know about visibility as that is not their job. They
 * just draw when told to. The manager can decide who is told. */
void scene_man_tie_visibility(char *scene_name, bool *is_visible);

/**
 * Get the anim manager if you want it to insert an animation to be
 * automatically updated and draw by the scene manager
 */
anim_man *scene_man_get_anim_man(void);

/**
 * Given a png file, animation width, height, get shared so*
 */
#define SCENE_MAN_LOAD_SO(path, width, height) \
	(so_new(anim_man_load(scene_man_get_anim_man(), texture_man_load(path), (width), (height))))

#endif // RLU_SCENE_MAN_H
