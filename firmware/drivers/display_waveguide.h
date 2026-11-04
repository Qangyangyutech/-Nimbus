#ifndef NIMBUS_DISPLAY_WAVEGUIDE_H
#define NIMBUS_DISPLAY_WAVEGUIDE_H

#include <stdint.h>

int  nb_display_init(void);
void nb_display_commit(void);
void nb_display_set_brightness(uint8_t percent);

#endif /* NIMBUS_DISPLAY_WAVEGUIDE_H */
