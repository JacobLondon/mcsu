#ifndef MCSU_UICTRL_H_
#define MCSU_UICTRL_H_

#include "player.h"

/**
 * All UI input control is done here. For non-UI input
 * (keyboard or pure clicking on a player for example),
 * then see player_man.c
 */

void uictrl_init(void);
void uictrl_cleanup(void);
void uictrl_update(void);
void uictrl_draw(void);

FormationEnum uictrl_get_formation(void);
int uictrl_get_charging(void);
void uictrl_announce(char *text); // will get truncated if too long

#endif // MCSU_UICTRL_H_
