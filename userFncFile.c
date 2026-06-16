#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "inc/userFncFile.h"
#include "inc/systemTimer.h"
#include "inc/led_Led1.h"
#include "inc/led_Led2.h"
#include "inc/timer_api1.h"
#include "inc/conversionFunctions.h"
#include "inc/Graphics_OLED.h"
#include "inc/Edit_Edit1.h"
#include "inc/Keyboard_Pad.h"
#include "inc/Keyboard_Nav.h"
#include "inc/EMICBus.h"

/* User Variables */
uint16_t nivel = 1234;

void onReset()
{
    Graphics_OLED_clear();
    Graphics_OLED_bindFncAt(20, 24, 1, Edit_Edit1_editableVar, &nivel, 1, "####");
    Edit_Edit1_begin(&nivel, 1, "####");
}


void Keyboard_Pad_onPress(char key)
{
    if (key == 13)
    {
        Edit_Edit1_accept();
    }
    else
    {
        Edit_Edit1_changeCharacter();
    }
}



