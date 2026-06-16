#ifndef USERFNCFILE_H
#define USERFNCFILE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// includes type definitions
#include "streamOut.h"
#include "streamIn.h"

/* Function Prototypes */
void onReset();
void Keyboard_Pad_onPress(uint8_t key);
void Keyboard_Nav_onPress(uint8_t key);
void displayTest(void);

#endif /* USERFNCFILE_H */

