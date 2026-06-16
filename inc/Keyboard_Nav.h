
#ifndef KEYBOARD_Nav_H_
#define KEYBOARD_Nav_H_

/*==================[inclusions]=============================================*/

#include <stdint.h>
#include "gpio.h"
#include "keyCodes.h"

/*==================[macros]=================================================*/

#define KBD_Nav_COLS     3
#define KBD_Nav_ROWS     3
#define KBD_Nav_SCAN_MS  5    // scan period in ms (throttles the poll)
#define KBD_Nav_DEBOUNCE 4    // stable scans required (~SCAN_MS*DEBOUNCE)

/*==================[external functions declaration]=========================*/

void Keyboard_Nav_init(void);

void Keyboard_Nav_poll(void);




/*==================[end of file]============================================*/
#endif

