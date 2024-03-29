#ifndef MCSU_PLAYER_MAN_H_
#define MCSU_PLAYER_MAN_H_

/**
 * Manage the player and the other connected players.
 * 
 * Allow for non-UI input control. See uictrl.c for
 * UI input controls.
 */

// init and tell how to draw the players at locations
void player_man_init(void);
void player_man_cleanup(void);
void player_man_draw(void);
void player_man_update(void);

/**
 * Load the config for this program's player instance.
 * 
 * This is the player from this client, no other. Others
 * will be loaded through the network.
 */
void player_man_me_load(const char *filename);

/**
 * Move this client's player to the world x/y
 */
void player_man_me_move(int worldx, int worldy);

#endif // MCSU_PLAYER_MAN_H_
