
#ifndef KEYBOARD_Pad_H_
#define KEYBOARD_Pad_H_

/*==================[inclusions]=============================================*/

#include <stdint.h>
#include "gpio.h"
#include "keyCodes.h"

/*==================[macros]=================================================*/

#define KBD_Pad_COLS     3
#define KBD_Pad_ROWS     4
#define KBD_Pad_SCAN_MS  5    // scan period in ms (throttles the poll)
#define KBD_Pad_DEBOUNCE 4    // stable scans required (~SCAN_MS*DEBOUNCE)

/*==================[external functions declaration]=========================*/

void Keyboard_Pad_init(void);

void Keyboard_Pad_poll(void);


extern void Keyboard_Pad_onPress(uint8_t key);


/*==================[end of file]============================================*/
#endif

